#include "StdAfx.h"
#include "ChangeDisc.h"
#include "json-aux-ext.h"

ChangeDisc::ChangeDisc(LiteWnd* pParent /* NULL */):LiteWnd(pParent)
{
	spec_ = theApp.GetSpecIdForStudId(theApp.GetCurrentStudentID());
}

ChangeDisc::~ChangeDisc(void)
{
}

BOOL ChangeDisc::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"Перевод оценок из одной дисциплины в другую";
	return TRUE;
}

int ChangeDisc::OnCreate()
{
	SetWindowPos(m_hWnd, NULL, -1, -1, 640, 550, 
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	PBYTE pb; DWORD cb;
	if(GetHtmlResource(L"IDR_HTML_CHANGE_DISC", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	InitDomElement();
	UpdateTable();

	return 0;
}

// обрабатывает кнопки
BOOL CALLBACK ChangeDisc::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	ChangeDisc* dlg = static_cast<ChangeDisc*>(tag);
	const wchar_t* id = htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-run"))
	{
		dlg->Run();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-close"))
	{
		dlg->Close();
		return TRUE;
	}

	return FALSE;
}

// связывает элементы дом с отображением и инициирует
void ChangeDisc::InitDomElement(void)
{
	table1_ = link_element("table1");
	table2_ = link_element("table2");

	// связывает с событиями кнопок
	HTMLayoutAttachEventHandlerEx(link_element("bt-run"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}

// отображение дисциплин
void ChangeDisc::UpdateTable()
{
	// удаляем все строки
  t::ClearTable(table1_, 1);
  t::ClearTable(table2_, 1);

	string_t query = string_t() +
		"SELECT id, fulltitle, shorttitle, idclass " 
		" FROM disciplines "
		" WHERE idspec = " + aux::itow(spec_) +
		" AND deleted = 0 "
		" ORDER BY fulltitle";

	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

	string_t			buf;
	mybase::MYFASTROW	row;

	int count = 1;
	while ((row = res.fetch_row()))	
		buf += "<tr value=" + row["id"] + ">" 
		+  "<td>" + aux::itoa(count++)  + "</td>"
		+  "<td>" + row["id"]           + "</td>"
		+  "<td>" + row["fulltitle"]    + "</td>" 
		+  "<td>" + row["shorttitle"]   + "</td>"
		+  "</tr>";

	assert(_mbslen(buf));
	if (_mbslen(buf))
	{
		table1_.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
		table2_.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
	}

	table1_.update();
	table2_.update();

	if (table1_.children_count() > 1)
	{
		htmlayout::dom::element(table1_.child(1)).set_state(STATE_CURRENT);
		htmlayout::dom::element(table2_.child(2)).set_state(STATE_CURRENT);
		table1_.update();
		table2_.update();
	}
}

// сменим дисциплину у оценок на другую
void ChangeDisc::Run()
{
	long disc1 = GetDisc(table1_);	
	long disc2 = GetDisc(table2_);

	string_t msg;
	if (disc1 == -1 || disc2 == -1)
		msg = "Выберете дисциплину в обеих таблицах";
	else if (disc1 == disc2)
		msg = "Вы выбрали одинаковые дисциплины";

	if (msg.size())
	{
		MessageBox(::GetActiveWindow(), msg, L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}

	if (IDNO == MessageBox(::GetActiveWindow(), L"Вы действительно хотите перенести оценки?", L"Сообщение", 
		MB_YESNO |MB_ICONINFORMATION | MB_APPLMODAL | MB_SETFOREGROUND))
		return ;

	string_t query = string_t() +
		" UPDATE students AS st, progress AS pr "
		" SET pr.iddiscip = " + aux::itow(disc2) +
		" , from_disc = "     + aux::itow(disc1) +
		" WHERE st.deleted = 0 AND st.grpid = " + aux::itow(theApp.GetCurrentGroupID()) + 
		" AND pr.idstud = st.id  AND pr.iddiscip = " + aux::itow(disc1);

	theApp.GetCon().Query(query);

	MessageBox(::GetActiveWindow(), L"Оценки для текущей группы успешно переведены", L"Информация", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
	Close();
}

// возвращает выбранный дисциплину
long ChangeDisc::GetDisc(htmlayout::dom::element table)
{
	// ищем выбранного студента
	assert(table.is_valid());
	htmlayout::dom::element row;
	for (UINT i = 1; i < table.children_count(); ++i)
	{
		htmlayout::dom::element t = table.child(i);
		if (t.get_state(STATE_CURRENT))
			return t.get_attribute_int("value", -1);
	}	

	return -1;
}
