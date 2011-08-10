#include "stdafx.h"
#include "ActionBuhReport.h"
#include "SingeltonApp.h"
#include "json-aux-ext.h"

CActionBuhReport::CActionBuhReport(LiteWnd* parent /* = NULL */):LiteWnd(parent)
{
}

CActionBuhReport::~CActionBuhReport(void)
{
}

BOOL CActionBuhReport::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"Buh Report";
	return TRUE;
}

int CActionBuhReport::OnCreate()
{

	SetWindowPos(m_hWnd, NULL, -1, -1, 450, 450, 
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	PBYTE pb; DWORD cb;
	if (GetHtmlResource(L"IDR_HTML_BUH_REPORT", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	else
		assert(FALSE);

	InitDomElement();
	return 0;
}

void CActionBuhReport::InitDomElement()
{
	_o_all		= link_element("o-all");
	_o_spec		= link_element("o-spec");
	_o_grp		= link_element("o-grp");
	_field_grp	= link_element("field-grp");
	_field_fio	= link_element("field-fio");
	_field_pas	= link_element("field-pas");
	_pay_year_1 = link_element("pay-year-1");
	_pay_year_2 = link_element("pay-year-1");
	_cat_month_1 = link_element("cat-month-1");
	_cat_month_2 = link_element("cat-month-2");
	_cat_year_1	 = link_element("cat-year-1");
	_cat_year_2  = link_element("cat-year-2");
	_date_box    = link_element("date-box");
	_date_1      = link_element("date-1");
	_date_2		 = link_element("date-2");
 
 	// присоединяем процедуру, отвлавливающую выбор строки с дисциплиной
	HTMLayoutAttachEventHandlerEx(link_element("pay-radio"), ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("cat-radio"), ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(_field_grp, ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(_field_fio, ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(_field_pas, ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

 	// присоединяем обоработчики к кнопкам
 	HTMLayoutAttachEventHandlerEx(link_element("bt-report"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
 	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}

// обрабатывает кнопки
BOOL CALLBACK CActionBuhReport::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CActionBuhReport* dlg = static_cast<CActionBuhReport*>(tag);
	const wchar_t*	id	  = htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-report"))
	{
		dlg->Report();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-close"))
	{
		dlg->Close();
		return TRUE;
	}

	return FALSE;
}

// обрабатывает выбор предмета/семестра
BOOL CALLBACK CActionBuhReport::ElementEventProcFor(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CActionBuhReport* dlg = static_cast<CActionBuhReport*>(tag);
	dlg->StateChange();

	return TRUE;
}

// отображает выбор
void CActionBuhReport::StateChange()
{
	// когда выбран выбор учебного года, то включим его и выключим категорию и наоборот
	bool pay = link_element("pay-radio").get_state(STATE_CHECKED);
	link_element("year-div").set_state(pay ? 0 : STATE_DISABLED, !pay ? 0 : STATE_DISABLED);
	link_element("cat-div").set_state(!pay ? 0 : STATE_DISABLED, pay ? 0 : STATE_DISABLED);

	// поля можно включать только последовательно, т.к. одно без другого не имеет смысла
	if (!_field_grp.get_state(STATE_CHECKED))
		_field_fio.set_state(STATE_DISABLED);
	else
		_field_fio.set_state(0, STATE_DISABLED);
	if (!_field_fio.get_state(STATE_CHECKED) || _field_fio.get_state(STATE_DISABLED))
		_field_pas.set_state(STATE_DISABLED);
	else
		_field_pas.set_state(0, STATE_DISABLED);
}

void CActionBuhReport::Report(void)
{
// 	element find = _table.find_first("tr:current");
// 	if (!find.is_valid())
// 		return;
// 
// 	string_t plan_sem	= json::v2t(element(find.child(1)).get_value());
// 	string_t iddiscip	= find.get_attribute("iddiscip");
// 	bool	 exam		= _exam.get_value();
// 
// 	if (plan_sem.empty() || iddiscip.empty())
// 		return;
// 
// 	string_t query;
// 	if (exam)
// 		query = string_t() + 
// 		" UPDATE students AS st,progress AS pr "
// 		" SET pr.estimation = CASE  "
// 		" WHEN ball >= 2.5 AND ball < 3.5 then 2 "
// 		" WHEN ball >= 3.5 AND ball < 4.5 then 1 "
// 		" WHEN ball >= 4.5 AND ball <= 5.0 then 0 END "
// 		" WHERE st.deleted = 0 AND st.grpid = " + aux::itow(theApp.GetCurrentGroupID()) + " AND "
// 		" pr.idstud = st.id AND pr.numplansemestr = " + plan_sem + " AND pr.deleted = 0 "
// 		" AND pr.iddiscip = " + iddiscip + " AND pr.ball >= 2.5 ";
// 	else
// 		query = string_t() + 
// 		" UPDATE students AS st,progress AS pr "
// 		" SET pr.estimation = 4 "
// 		" WHERE st.deleted = 0 AND st.grpid = " + aux::itow(theApp.GetCurrentGroupID()) + " AND "
// 		" pr.idstud = st.id AND pr.numplansemestr = " + plan_sem + " AND pr.deleted = 0 "
// 		" AND pr.iddiscip = " + iddiscip + " AND pr.ball >= 2.5 ";
// 
// 	theApp.GetCon().Query(query);
// 
// 	UpdateView();
// 	ShowSelection();
}
