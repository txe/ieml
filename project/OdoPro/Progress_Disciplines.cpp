#include "stdafx.h"
#include "Progress_Disciplines.h"
#include <boost\algorithm\string.hpp>
#include "SingeltonApp.h"
#include "json-aux-ext.h"
#include "logger.h"

using namespace htmlayout::dom; 

CManagDisciplinesDlg::CManagDisciplinesDlg(LiteWnd* pParent /* = NULL */):LiteWnd(pParent)
{
}

CManagDisciplinesDlg::~CManagDisciplinesDlg(void)
{
}

BOOL CManagDisciplinesDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"–едактирование дисциплин";
	return TRUE;
}

int CManagDisciplinesDlg::OnCreate()
{
	SetWindowPos(m_hWnd, NULL, -1, -1, 640, 550, 
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	PBYTE pb; DWORD cb;
	if(GetHtmlResource(L"IDR_HTML_DISCIPLINES", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	InitDomElement();
	UpdateView();

	return 0;
}

// обрабатывает кнопки
BOOL CALLBACK CManagDisciplinesDlg::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CManagDisciplinesDlg* dlg = static_cast<CManagDisciplinesDlg*>(tag);
	const wchar_t* id = htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-close"))
	{
		dlg->Close();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-del"))
	{
		dlg->DeleteDiscip();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-add"))
	{
		dlg->AddDiscip();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-update"))
	{
		dlg->SaveUpdateDiscip();
		return TRUE;
	}
	return FALSE;
}

// обрабатывает изменение семестров
BOOL CALLBACK CManagDisciplinesDlg::ElementEventProcDiscip(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	LOG_DEBUG << pr->cmd;
	if (pr->cmd != TABLE_ROW_CLICK)
		return FALSE;

	CManagDisciplinesDlg* dlg = static_cast<CManagDisciplinesDlg*>(tag);
	dlg->UpdateViewDiscipData();

	return TRUE;
}

// св€зывает элементы дом с отображением и инициирует
void CManagDisciplinesDlg::InitDomElement(void)
{
	discip_table_	= link_element("discip-table");
	fulltitle_		= link_element("fulltitle");
	shorttitle_		= link_element("shorttitle");
	discip_class_	= link_element("discip_class");
	scan_namber_	= link_element("scan_namber");
	num_hours_		= link_element("num_hours");
	spec_			= link_element("spec");
	
	// св€зываем с общими часами
	for (htmlayout::dom::element h = link_element("hours").child(0); h.is_valid(); h = h.next_sibling())
	{
		htmlayout::dom::element find = h.find_first("input"); 
		if (find.is_valid())
			sem_hours_.push_back(find);
	}
	// св€зывает с событи€ми кнопок
	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-del"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-add"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-update"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	
	// заполн€ем списки
	t::LoadContentFromVocForList(discip_class_, "discipclassific");
	t::LoadContentFromVocForList(spec_, "spec");

	// присоедин€ем процедуру, отвлавливающую выбор строки с дисциплинами
	HTMLayoutAttachEventHandlerEx(discip_table_, ElementEventProcDiscip, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

}

// проверка введенных значений
bool CManagDisciplinesDlg::Check()
{
	return true;
}

// получение и отображение свойств дисциплин
bool CManagDisciplinesDlg::UpdateView(void)
{
	UpdateViewDiscipTable();
	UpdateViewDiscipData();
	return true;
}

// обновл€ет таблицу дисциплин
void CManagDisciplinesDlg::UpdateViewDiscipTable(void)
{
	while (discip_table_.children_count() > 1)
		htmlayout::dom::element(discip_table_.child(1)).destroy();

	string_t specid = aux::itow(theApp.GetSpecIdForStudId(theApp.GetCurrentStudentID()));

	string_t query = string_t() +
		" SELECT id, fulltitle, shorttitle, idspec, idclass, num_hours, scan_number, sem_hours "
		" FROM disciplines "
		" WHERE deleted = 0 AND "
		" idspec = " + specid + 
		" ORDER BY idclass, fulltitle";
	
	mybase::MYFASTRESULT	res = theApp.GetCon().Query(query);
	mybase::MYFASTROW		row;
	int						pos = 1;
	string_t				buf;
	while (row = res.fetch_row())	
	{	
		string_t idclass;
		try
		{
			idclass = theApp.GetTitleForKeyFromVoc("discipclassific", aux::wtoi(row["idclass"]));
		}
		catch(wss::exception&)
		{
			;
		}
		buf += "<tr discip_id=" + row["id"] + 
			" sem_hours=\"" + row["sem_hours"] + "\""
			" idspec=" + row["idspec"] + 
			" idclass=" + row["idclass"] + " >";
		buf += string_t() +
			"<td>" + aux::itow(pos++)	+ "</td>"
			"<td>" + row["id"]	        + "</td>"
			"<td>" + row["fulltitle"]	+ "</td>"
			"<td>" + row["shorttitle"]	+ "</td>"
			"<td>" + row["num_hours"]	+ "</td>"
			"<td>" + idclass			+ "</td>"
			"<td>" + row["scan_number"]	+ "</td>"
			"</tr>";
	}
	discip_table_.update();
	if (_mbslen(buf))
		discip_table_.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);

	if (discip_table_.children_count() > 1)
		htmlayout::dom::element(discip_table_.child(1)).set_state(STATE_CURRENT);

	discip_table_.update();
}

// отображает данные на выбранную дисциплину
void CManagDisciplinesDlg::UpdateViewDiscipData(void)
{
	element discip = t::GetSelectedRow(discip_table_);

	json::t2v(fulltitle_,  json::v2t(element(discip.child(2)).get_value()));
	json::t2v(shorttitle_, json::v2t(element(discip.child(3)).get_value()));
	json::t2v(num_hours_,  json::v2t(element(discip.child(4)).get_value()));
	json::t2v(scan_namber_,json::v2t(element(discip.child(6)).get_value()));
	
	std::wstring hours = discip.get_attribute("sem_hours");
	std::vector<std::wstring> hs;
	boost::split(hs, hours, boost::is_any_of(L";"));
	for (uint i = 0; i < sem_hours_.size(); ++i)
		json::t2v(sem_hours_[i], (i < hs.size())?hs[i]:L"1");

	json::t2v(spec_, discip.get_attribute("idspec"));
	json::t2v(discip_class_, discip.get_attribute("idclass"));

}

// удал€ет дисциплину из базы
void CManagDisciplinesDlg::DeleteDiscip(void)
{
	string_t msg = "¬ы действительно хотите удалить эту запись?\n"
		"¬едь при еЄ удалении пропадут некоторые данные о студентах,\n"
		"а при попытке исправить ситуацию и внести запись с теми же данными структура базы не восстановитс€!\n"
		"Ѕудьте аккуратны!";

	if( IDNO == MessageBox(m_hWnd, msg, 
		L"ѕредупреждение", MB_YESNO | MB_ICONERROR | MB_APPLMODAL))
		return;

	string_t discip_id = t::GetSelectedRow(discip_table_).get_attribute("discip_id");
	string_t query = "UPDATE disciplines SET deleted = 1 WHERE id = " + discip_id;
	theApp.GetCon().Query(query);

	UpdateView();
}

// провер€ет существует ли дисциплина с такими параметрами
bool CManagDisciplinesDlg::IsExistDiscip(bool include_cur_discip /* = true */)
{
	string_t specid			= aux::itow(theApp.GetSpecIdForStudId(theApp.GetCurrentStudentID()));
	string_t fulltitle		= json::v2t(fulltitle_.get_value());
	string_t shorttitle		= json::v2t(shorttitle_.get_value());
	string_t incl			= string_t() + " id != " + t::GetSelectedRow(discip_table_).get_attribute("discip_id") + " AND ";

	string_t query = "SELECT id FROM disciplines "
		" WHERE idspec = " + specid + " AND "
		" fulltitle = '" + fulltitle + "' AND "
		" shorttitle = '" + shorttitle + "' AND " +
		(include_cur_discip?"":incl) +
		" deleted = 0 LIMIT 1";

	mybase::MYFASTRESULT	res = theApp.GetCon().Query(query);
	return res.size() > 0;
}

// создает новую дисциплину
void CManagDisciplinesDlg::AddDiscip(void)
{
	if (IsExistDiscip())
	{
		string_t msg = "«апись c такими параметрами дл€ данной специальности уже существует в базе данных.\n"
			"ƒобавлени€ не произошло!";
		MessageBox(m_hWnd, msg, L"—ообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
		return;
	}

	string_t specid			= aux::itow(theApp.GetSpecIdForStudId(theApp.GetCurrentStudentID()));
	string_t fulltitle		= json::v2t(fulltitle_.get_value());
	string_t shorttitle		= json::v2t(shorttitle_.get_value());
	string_t discip_class	= json::v2t(discip_class_.get_value());
	string_t scan_namber	= json::v2t(scan_namber_.get_value());
	string_t num_hours		= json::v2t(num_hours_.get_value());
	string_t sem_hours		= AudHoursToStr();

	string_t query = 
		" INSERT INTO disciplines (idspec, fulltitle, shorttitle, idclass, num_hours, scan_number, sem_hours) "
		" VALUES(" + specid + ", '" + fulltitle + "', '" + shorttitle + "', " + 
		discip_class + ", " + num_hours + ", " + scan_namber + ", '" + sem_hours+"')";
	theApp.GetCon().Query(query);

	UpdateView();
}

// переводит аудиторные часы в строку
string_t CManagDisciplinesDlg::AudHoursToStr(void)
{
	string_t buf;
	for (uint i = 0; i < sem_hours_.size(); ++i)
	{
		string_t hour = "0";
		if (0 == sem_hours_[i].get_attribute("invalid"))
		{
			string_t val = json::v2t(sem_hours_[i].get_value());
			if (aux::wtoi(val) < 1000)
				hour = val;
		}
		buf += hour + ";";
	}
	return buf;
}

// сохран€ет изменение дисциплины
void CManagDisciplinesDlg::SaveUpdateDiscip(void)
{
	if (IsExistDiscip(false))
	{
		string_t msg = "«апись c такими параметрами дл€ данной специальности уже существует в базе данных.\n"
			"—охранение не произошло!";
		MessageBox(m_hWnd, msg, L"—ообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
		return;
	}

	string_t discip_id		= t::GetSelectedRow(discip_table_).get_attribute("discip_id");
	string_t specid			= aux::itow(theApp.GetSpecIdForStudId(theApp.GetCurrentStudentID()));
	string_t fulltitle		= json::v2t(fulltitle_.get_value());
	string_t shorttitle		= json::v2t(shorttitle_.get_value());
	string_t discip_class	= json::v2t(discip_class_.get_value());
	string_t scan_namber	= json::v2t(scan_namber_.get_value());
	string_t num_hours		= json::v2t(num_hours_.get_value());
	string_t sem_hours		= AudHoursToStr();

	string_t query = 
		" UPDATE disciplines "
		" SET idspec = " + specid + ", fulltitle = '" + fulltitle + 
		"', shorttitle = '" + shorttitle + "', idclass = " + discip_class + 
		", num_hours = " + num_hours + ", scan_number = " + scan_namber + 
		", sem_hours = '" + sem_hours + "'"
		" WHERE id = " + discip_id;
	theApp.GetCon().Query(query);

	UpdateView();
}
