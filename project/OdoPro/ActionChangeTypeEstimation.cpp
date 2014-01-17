#include "stdafx.h"
#include "ActionChangeTypeEstimation.h"
#include "SingeltonApp.h"
#include "json-aux-ext.h"

CActionChangeTypeEstimation::CActionChangeTypeEstimation(LiteWnd* parent /* = NULL */):LiteWnd(parent)
{
}

CActionChangeTypeEstimation::~CActionChangeTypeEstimation(void)
{
}

BOOL CActionChangeTypeEstimation::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"Изменение типа оценки";

	cs.cx = 600;
	cs.cy = 600;

	return TRUE;
}

int CActionChangeTypeEstimation::OnCreate()
{
	CenterWindow();

	PBYTE pb; DWORD cb;
	if (GetHtmlResource(L"IDR_HTML_CHANGE_ESTIM", pb, cb))
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

void CActionChangeTypeEstimation::InitDomElement()
{
	_table	= link_element("table");
	_exam	= link_element("radio-exam");
	_test	= link_element("radio-test");

	// присоединяем процедуру, отвлавливающую выбор строки с дисциплиной
	HTMLayoutAttachEventHandlerEx(_table, ElementEventProcForTable, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

	// присоединяем обоработчики к кнопкам
	HTMLayoutAttachEventHandlerEx(link_element("bt-save"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}

// обрабатывает кнопки
BOOL CALLBACK CActionChangeTypeEstimation::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CActionChangeTypeEstimation* dlg	= static_cast<CActionChangeTypeEstimation*>(tag);
	const wchar_t*	id					= htmlayout::dom::element(he).get_attribute("id");

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

void CActionChangeTypeEstimation::UpdateView(void)
{
	assert(_table.is_valid());

	// удаляем строки кроме заголовка таблицы
  t::ClearTable(_table, 1);

	string_t  query = string_t() +
		" SELECT DISTINCT pr.numplansemestr, dis.fulltitle, pr.id, dis.shorttitle, pr.estimation, pr.ball, pr.iddiscip "
		" FROM progress AS pr, disciplines AS dis "
		" WHERE pr.idstud = " + aux::itow(theApp.GetCurrentStudentID()) + " AND pr.deleted = 0 AND dis.id = pr.iddiscip AND dis.deleted = 0  "
		" ORDER BY pr.numplansemestr, LEFT(dis.fulltitle, 50)";

	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

	string_t			buf;
	mybase::MYFASTROW	row;
	int					count = 0;
	string_t            type[] = {"экзамен", "зачет"};
	while (row = res.fetch_row())	
	{
		int estim = t::cod2type(aux::wtoi(row["estimation"]));
			buf += "<tr iddiscip=" + row["iddiscip"] + " type_estim=" + aux::itow(estim) + ">";
		buf += string_t() + 
			"<td>" + aux::itow(++count)	+ "</td>"
			"<td>" + row["numplansemestr"] + "</td>"
			"<td>" + type[estim] + "</td>"
			"<td>" + row["ball"] + "(" + t::cod2text(row["estimation"]) + ")</td>"
			"<td>" + row["fulltitle"] + "</td>"
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
void CActionChangeTypeEstimation::ShowSelection()
{
	element find = _table.find_first("tr:current");
	if (!find.is_valid())
		return;

	if (find.get_attribute_int("type_estim") == 1)
		_test.set_value(json::value(true));
	else
		_exam.set_value(json::value(true));
}

// обрабатывает выбор предмета/семестра
BOOL CALLBACK CActionChangeTypeEstimation::ElementEventProcForTable(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != TABLE_ROW_CLICK)
		return FALSE;

	CActionChangeTypeEstimation* dlg = static_cast<CActionChangeTypeEstimation*>(tag);
	dlg->ShowSelection();

	return TRUE;
}

void CActionChangeTypeEstimation::Save(void)
{
	element find = _table.find_first("tr:current");
	if (!find.is_valid())
		return;

	string_t plan_sem	= json::v2t(element(find.child(1)).get_value());
	string_t iddiscip	= find.get_attribute("iddiscip");
	bool	 exam		= _exam.get_value();

	if (plan_sem.empty() || iddiscip.empty())
		return;

	string_t query;
	if (exam)
		query = string_t() + 
		" UPDATE students AS st,progress AS pr "
		" SET pr.estimation = CASE  "
		" WHEN ball >= 2.5 AND ball < 3.5 then 2 "
		" WHEN ball >= 3.5 AND ball < 4.5 then 1 "
		" WHEN ball >= 4.5 AND ball <= 5.0 then 0 END "
		" WHERE st.deleted = 0 AND st.grpid = " + aux::itow(theApp.GetCurrentGroupID()) + " AND "
		" pr.idstud = st.id AND pr.numplansemestr = " + plan_sem + " AND pr.deleted = 0 "
		" AND pr.iddiscip = " + iddiscip + " AND pr.ball >= 2.5 ";
	else
		query = string_t() + 
		" UPDATE students AS st,progress AS pr "
		" SET pr.estimation = 4 "
		" WHERE st.deleted = 0 AND st.grpid = " + aux::itow(theApp.GetCurrentGroupID()) + " AND "
		" pr.idstud = st.id AND pr.numplansemestr = " + plan_sem + " AND pr.deleted = 0 "
		" AND pr.iddiscip = " + iddiscip + " AND pr.ball >= 2.5 ";

	theApp.GetCon().Query(query);

	UpdateView();
	ShowSelection();
}
