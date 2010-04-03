#include "stdafx.h"
#include "SingeltonApp.h"
#include "ActionChangeSem.h"
#include "json-aux-ext.h"

CActionChangeSem::CActionChangeSem(LiteWnd* parent /* = NULL */):LiteWnd(parent), _type_sem(PLAN_SEM)
{
}

CActionChangeSem::~CActionChangeSem(void)
{
}

BOOL CActionChangeSem::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	if (_type_sem == PLAN_SEM)
		cs.lpszName = (LPCWSTR)"Изменение семестра по плану";
	else
		cs.lpszName = (LPCWSTR)"Изменение семестра по графику";
	
	cs.cx = 600;
	cs.cy = 600;

	return TRUE;
}

int CActionChangeSem::OnCreate()
{
	CenterWindow();

	PBYTE pb; DWORD cb;
	if (GetHtmlResource(L"IDR_HTML_CHANGE_SEM", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	else
		assert(FALSE);

	InitDomElement();
	UpdateView();
	ShowSelection();
	return 0;
}

void CActionChangeSem::InitDomElement()
{
	_table		= link_element("table");
	_cur_sem	= link_element("cur-sem");
	_new_sem	= link_element("new-sem");
	//_discip		= link_element("discip");
	
	// присоединяем процедуру, отвлавливающую выбор строки с дисциплиной
	HTMLayoutAttachEventHandlerEx(_table, ElementEventProcForTable, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

	// присоединяем обоработчики к кнопкам
	HTMLayoutAttachEventHandlerEx(link_element("bt-save"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}

// обрабатывает кнопки
BOOL CALLBACK CActionChangeSem::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CActionChangeSem* dlg	= static_cast<CActionChangeSem*>(tag);
	const wchar_t*	id		= htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-save"))
	{
		dlg->Save();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-close"))
	{
		dlg->Close();
		return TRUE;
	}

	return FALSE;
}

void CActionChangeSem::UpdateView(void)
{
	assert(_table.is_valid());

	// удаляем строки кроме заголовка таблицы
	while (_table.children_count() > 1)
		HTMLayoutDetachElement(_table.child(1));

	string_t sem = _type_sem == PLAN_SEM?"numplansemestr":"numgraphsemestr";
	string_t  query = string_t() +
		" SELECT DISTINCT pr." + sem + " AS sem, dis.fulltitle, dis.id, dis.shorttitle"
		" FROM progress AS pr, disciplines AS dis"
		" WHERE pr.idstud = " + aux::itow(theApp.GetCurrentStudentID()) + " AND pr.deleted = 0 AND dis.id = pr.iddiscip AND dis.deleted = 0 "
		" ORDER BY pr." + sem + ", LEFT(dis.fulltitle, 50)";
	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

	string_t			buf;
	mybase::MYFASTROW	row;
	int					count = 0;

	while (row = res.fetch_row())	
	{
		buf += "<tr id=" + row["id"] + ">";
		buf += string_t() + "<td>" + aux::itow(++count)	+ "</td>"
			"<td>" + row["sem"]	+ "</td>"
			"<td>" + row["shorttitle"] + "("	+ row["fulltitle"] + ")</td>"
			"</tr>";
	}

	if (_mbslen(buf))
		_table.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
	_table.update();
	if (_table.children_count() > 1)
	{
		element first = _table.child(1);
		first.set_state(STATE_CURRENT);
	}

	_table.update(true);
}

// отображает выбор
void CActionChangeSem::ShowSelection()
{
	element find = _table.find_first("tr:current");
	if (!find.is_valid())
		return;

	//string_t discip = json::v2t(element(find.child(2)).get_value());
	string_t sem	= json::v2t(element(find.child(1)).get_value());
	
//	json::t2v(_discip, discip);
	json::t2v(_cur_sem, sem);
}

// обрабатывает выбор предмета/семестра
BOOL CALLBACK CActionChangeSem::ElementEventProcForTable(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != TABLE_ROW_CLICK)
		return FALSE;

	CActionChangeSem* dlg = static_cast<CActionChangeSem*>(tag);
	dlg->ShowSelection();
	
	return TRUE;
}

void CActionChangeSem::Save(void)
{
	element find = _table.find_first("tr:current");
	if (!find.is_valid())
		return;
	
	string_t old_sem = json::v2t(element(find.child(1)).get_value());
	string_t new_sem = json::v2t(_new_sem.get_value());
	string_t discip_id = find.get_attribute("id");
	string_t sem = _type_sem == PLAN_SEM?"numplansemestr":"numgraphsemestr";
	
	if (discip_id.empty() || old_sem.empty() || new_sem.empty())
		return;

	string_t query = string_t() + 
		" UPDATE students AS st, progress AS pr "
		" SET pr." + sem + " = " + new_sem +
		" WHERE st.deleted = 0 AND st.grpid = " + aux::itow(theApp.GetCurrentGroupID()) + " AND "
		" pr.idstud = st.id AND pr." + sem + " = " + old_sem + " AND pr.deleted = 0 "
		" AND pr.iddiscip = " + discip_id;
	theApp.GetCon().Query(query);

	UpdateView();
	ShowSelection();
}