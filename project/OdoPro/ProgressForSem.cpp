#include "stdafx.h"
#include "ProgressForSem.h"
#include "SingeltonApp.h"
#include "json-aux-ext.h"

CProgressViewBallForSemDlg::CProgressViewBallForSemDlg(LiteWnd* pParent /* = NULL */):LiteWnd(pParent)
{
}

CProgressViewBallForSemDlg::~CProgressViewBallForSemDlg(void)
{
}

BOOL CProgressViewBallForSemDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"”спеваимость за определенный семестр";
	return TRUE;
}

int CProgressViewBallForSemDlg::OnCreate()
{
	SetWindowPos(m_hWnd, NULL, -1, -1, 640, 550, 
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	PBYTE pb; DWORD cb;
	if(GetHtmlResource(L"IDR_HTML_PROGRESS_FOR_SEM", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	InitDomElement();
	UpdateView();

	return 0;
}

// обрабатывает кнопки
BOOL CALLBACK CProgressViewBallForSemDlg::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CProgressViewBallForSemDlg* dlg = static_cast<CProgressViewBallForSemDlg*>(tag);
	const wchar_t* id = htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-close"))
	{
		dlg->Close();
		return TRUE;
	}

	return FALSE;
}

// обрабатывает изменение семестров
BOOL CALLBACK CProgressViewBallForSemDlg::ElementEventProcSem(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (!(pr->cmd == EDIT_VALUE_CHANGED || pr->cmd == BUTTON_STATE_CHANGED))
		return FALSE;

	CProgressViewBallForSemDlg* dlg = static_cast<CProgressViewBallForSemDlg*>(tag);
	dlg->UpdateView();

	return TRUE;
}

// св€зывает элементы дом с отображением и инициирует
void CProgressViewBallForSemDlg::InitDomElement(void)
{
	json::t2v(link_element("plansem"), plan_sem_);
	json::t2v(link_element("graphsem"), graph_sem_);
	
	// св€зывает с событи€ми кнопок
	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);

	// при обновление семестров по плану и графику производит обновление таблицы балов
	HTMLayoutAttachEventHandlerEx(link_element("radio-plan"), ElementEventProcSem, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("radio-graph"), ElementEventProcSem, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("plansem"), ElementEventProcSem, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("graphsem"), ElementEventProcSem, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}

// проверка введенных значений
bool CProgressViewBallForSemDlg::Check()
{
	int graph_sem = aux::wtoi(json::v2t(link_element("graphsem").get_value()), -1);
	int plan_sem  = aux::wtoi(json::v2t(link_element("plansem").get_value()), -1);

	string_t msg;
	if (graph_sem < 1 || graph_sem > 12)
		msg = "«начение граф-семестра должно быть в диапазоне [1-12]";
	if (plan_sem < 1 || plan_sem > 12)
		msg = "«начение план-семестра должно быть в диапазоне [1-12]";

	if (msg.empty())
		return true;

	MessageBox(m_hWnd, msg,	L"ќшибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);

	return false;
}

// получение и отображение свойств студента
bool CProgressViewBallForSemDlg::UpdateView(void)
{
	htmlayout::dom::element table = link_element("ball-table");
	while (table.children_count() > 1)
		htmlayout::dom::element(table.child(1)).destroy();

	if (!Check())
		return false;

	string_t graph_sem	= json::v2t(link_element("graphsem").get_value());
	string_t plan_sem	= json::v2t(link_element("plansem").get_value());
	string_t radio		= json::v2t(link_element("radio-plan").get_value()); // переключатель
	string_t query;

	if (aux::wcseq(radio, L"1")) // установлен план семестр
		query = string_t() + 
		" SELECT oc.id, disc.fulltitle, oc.estimation, oc.ball "
		" FROM disciplines as disc, progress as oc, students as stud "
		" WHERE oc.deleted = 0 AND disc.deleted = 0 AND stud.deleted = 0 AND "
		" oc.idstud = " + aux::itow(theApp.GetCurrentStudentID()) + " AND stud.id = oc.idstud AND "
		" disc.idspec = stud.specid AND oc.iddiscip=disc.id AND" 
		" numplansemestr = " + plan_sem + " ORDER BY fulltitle";
	else
		query = string_t() +
		" SELECT oc.id, disc.fulltitle, oc.estimation, oc.ball "
		" FROM disciplines as disc, progress as oc, students as stud "
		" WHERE oc.deleted = 0 AND disc.deleted = 0 AND stud.deleted = 0 AND "
		" oc.idstud = " + aux::itow(theApp.GetCurrentStudentID()) + " AND stud.id = oc.idstud AND "
		" disc.idspec = stud.specid AND oc.iddiscip = disc.id AND "
		" numgraphsemestr = " + graph_sem + " ORDER BY fulltitle";

	mybase::MYFASTRESULT	res = theApp.GetCon().Query(query);
	mybase::MYFASTROW		row;
	int						pos = 1;
	string_t				buf;
	while (row = res.fetch_row())	
	{	
		buf += "<tr>";
		buf += string_t() +
			"<td>" + aux::itow(pos++)				+ "</td>"
			"<td>" + row["fulltitle"]				+ "</td>"
			"<td>" + row["ball"]					+ "</td>"
			"<td>" + t::cod2text(row["estimation"])	+ "</td>"
			"</tr>";
	}
	if (_mbslen(buf))
		table.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);

	if (table.children_count() > 1)
	{
		htmlayout::dom::element(table.child(1)).set_state(STATE_CURRENT);
	}

	// обновл€ем заголовок
	link_element("header").set_text(theApp.GetFIO());
	link_element("header").update();

	table.update();

	return true;
}

void CProgressViewBallForSemDlg::SetParam(string_t plan_sem, string_t graph_sem)
{
	plan_sem_	= plan_sem;
	graph_sem_	= graph_sem;
}
