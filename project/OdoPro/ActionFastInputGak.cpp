#include "stdafx.h"
#include "SingeltonApp.h"
#include "ActionFastInputGak.h"
#include "json-aux-ext.h"

using namespace htmlayout::dom;
CActionFastInputGakDlg::CActionFastInputGakDlg(LiteWnd* pParent /*= NULL */) : LiteWnd(pParent)
{
}

CActionFastInputGakDlg::~CActionFastInputGakDlg(void)
{
}

BOOL CActionFastInputGakDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"Быстрый ввод данных протокола ГАК для группы";
	return TRUE;
}

int CActionFastInputGakDlg::OnCreate()
{
	SetWindowPos(m_hWnd, NULL, -1, -1, 640, 550, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	PBYTE pb; DWORD cb;
	if(GetHtmlResource(L"IDR_HTML_FAST_INPUT_GAK", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	InitDomElement();
	UpdateView();

	return 0;
}

// обрабатывает кнопки
BOOL CALLBACK CActionFastInputGakDlg::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CActionFastInputGakDlg* dlg = static_cast<CActionFastInputGakDlg*>(tag);
	const wchar_t* id = htmlayout::dom::element(he).get_attribute("id");

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
	if (aux::wcseq(id, L"bt-copy"))
	{
		dlg->CopyToBuf();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-ins"))
	{
		dlg->InsertFromBuf();
		return TRUE;
	}
	return FALSE;
}

// обрабатывает изменение семестров
BOOL CALLBACK CActionFastInputGakDlg::ElementEventProcSem(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != EDIT_VALUE_CHANGED)
		return FALSE;

	CActionFastInputGakDlg* dlg = static_cast<CActionFastInputGakDlg*>(tag);
	dlg->UpdateView();

	return TRUE;
}

// связывает элементы дом с отображением и инициирует
void CActionFastInputGakDlg::InitDomElement(void)
{
	// связывает с событиями кнопок
	HTMLayoutAttachEventHandlerEx(link_element("bt-save"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	//HTMLayoutAttachEventHandlerEx(link_element("bt-copy"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	//HTMLayoutAttachEventHandlerEx(link_element("bt-ins"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}

// получение и отображение свойств студента
bool CActionFastInputGakDlg::UpdateView(void)
{
	htmlayout::dom::element table = link_element("gak-table");
	while (table.children_count() > 1)
		htmlayout::dom::element(table.child(1)).destroy();

	// получаем список студентов
	string_t query = string_t() +
		" SELECT id, CONCAT(secondname,' ',firstname,' ',thirdname) AS fio, edudatequalif, edunumprotgak"
		" FROM students "
		" WHERE deleted = 0 AND grpid = " + aux::itow(theApp.GetCurrentGroupID()) +
		" ORDER BY fio, id ";

	mybase::MYFASTRESULT stud_res = theApp.GetCon().Query(query);
	mybase::MYFASTROW	 stud_row;
	string_t			 buf;
	int					 pos = 1;
	while (stud_row = stud_res.fetch_row())	
	{	
		string_t stud_id = stud_row["id"];
		buf += "<tr "
			" stud_id=" + stud_id + 
			">";
		buf += string_t() +
			"<td>" + aux::itow(pos++)	+ "</td>"
			"<td>" + stud_row["fio"]	+ "</td>"
			"<td>" + stud_row["edunumprotgak"]	+ "</td>"
			"<td>" + stud_row["edudatequalif"]	+ "</td>"
			"</tr>";
	}
	if (_mbslen(buf))
		table.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);

	if (table.children_count() > 1)
	{
		//htmlayout::dom::element(htmlayout::dom::element(table.child(1)).child(2)).post_event(CHECK_ALL);
		htmlayout::dom::element(table.child(1)).set_state(STATE_CURRENT);
	}

	// обновляем заголовок
	link_element("header").set_text(theApp.GetGroupName(theApp.GetCurrentGroupID()));
	link_element("header").update();

	table.update();

	return true;
}

// сохранение
void CActionFastInputGakDlg::Save(void)
{
	
// 	htmlayout::dom::element table = link_element("ball-table");
// 	
// 	if (table.children_count() < 2)
// 		return;
// 
// 	if (!Check())
// 		return;
// 
// 	string_t graph_sem	= json::v2t(link_element("graphsem").get_value());
// 	string_t plan_sem	= json::v2t(link_element("plansem").get_value());	
// 	string_t discip_id	= aux::itoa(discip_id_);
// 	std::string query;
// 
// 	for (htmlayout::dom::element row = table.child(1); row.is_valid(); row = row.next_sibling())
// 	{
// 		htmlayout::dom::element ball = row.child(2);
// 		string_t state = ball.get_attribute("state");
// 		
// 		if (aux::wcseq(state, L"bad"))
// 		{
// 			row.scroll_to_view();
// 			MessageBeep(MB_OK);
// 			return;
// 		}
// 		if (aux::wcseq(state, L"good"))
// 		{
// 			string_t stud_id	= row.get_attribute("stud_id"); 
// 			string_t ball_		= t::ball2sql(json::v2t(ball.get_value()));
// 			string_t estim		= aux::itow(t::type2cod(aux::wtoi(json::v2t(link_element("estim").get_value()), -1),
// 				aux::strtod(ball_, -1)));
// 
// 			query += std::string() + "(" +
// 				stud_id + ", " +
// 				discip_id + ", " +
// 				plan_sem + ", " +
// 				graph_sem + ", " +
// 				estim + ", " +
// 				ball_ + "),";
// 		}
// 	}
// 	if (!query.empty()) // может нет предметов или еще что то, типа удалили все оценки
// 	{
// 		// удалим последнюю ненужную запятую
// 		query.erase(query.size()-1, 1);
// 		query = " INSERT INTO progress (idstud, iddiscip, numplansemestr, numgraphsemestr, estimation, ball) VALUES " + query;
// 	}
// 	// составим запрос на удаление предыдущих оценок
// 	string_t delete_query = string_t() + 
// 		" DELETE pr.* "
// 		" FROM students AS st, progress AS pr "
// 		" WHERE "
// 		" st.deleted = 0 AND st.grpid = " + aux::itow(theApp.GetCurrentGroupID()) + " AND "
// 		" pr.deleted = 0 AND pr.idstud = st.id AND pr.`numplansemestr` = " + plan_sem + " AND "
// 		" pr.`numgraphsemestr` = " + graph_sem + " AND pr.`iddiscip` = " + discip_id;
// 
// 	// удаление и сохранение оценок
// 	mysql_autocommit(theApp.GetCon().GetCon(), false);
// 	theApp.GetCon().Query("START TRANSACTION;");
// 	try{
// 		theApp.GetCon().Query(delete_query.c_str());
// 		if (!query.empty())
// 			theApp.GetCon().Query(query.c_str());
// 		// применить все изменения
// 		theApp.GetCon().Query("COMMIT;");
// 	}
// 	catch(...)
// 	{
// 		mysql_autocommit(theApp.GetCon().GetCon(), true);
// 		theApp.ExceptionManage();
// 		MessageBeep(MB_ICONEXCLAMATION);
// 		theApp.GetCon().Query("ROLLBACK;");		
// 		return;		
// 	}
// 
// 	mysql_autocommit(theApp.GetCon().GetCon(), true);
// 	Close();
}

void CActionFastInputGakDlg::CopyToBuf( void )
{
// 	_buf_ball.clear();
// 	//третья ячейка - оценка
// 	for (element row = link_element("ball-table").child(1); row.is_valid(); row = row.next_sibling())
// 		_buf_ball.push_back(element(row.child(2)).get_value());
}

void CActionFastInputGakDlg::InsertFromBuf()
{
// 	htmlayout::dom::element table = link_element("ball-table");
// 
// 	if (table.children_count()-1 != _buf_ball.size())
// 	{
// 		string_t msg = "Количество оценок в буфере не совпадает с количеством студентов.\n"
// 			"Действие отменено.";
// 		MessageBox(m_hWnd, msg, L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
// 		return;
// 	}
// 
// 	element row = table.child(1);
// 	std::deque<json::value>::iterator ball = _buf_ball.begin();
// 	for (; row.is_valid(); row = row.next_sibling(), ++ball)
// 	{
// 		if ((*ball).is_undefined())
// 			json::t2v(element(row.child(2)), "");
// 		else
// 			element(row.child(2)).set_value(*ball);			
// 	}
// 	element(element(table.child(1)).child(2)).post_event(CHECK_ALL);
// 	element(table.child(1)).set_state(STATE_CURRENT);
// 	table.update();
}
// задать все гак
void CActionFastInputGakDlg::SetGak()
{
	htmlayout::dom::element table = link_element("gak-table");
	string_t gak_value	= json::v2t(link_element("gak-value").get_value());
	if (gak_value.empty())
		return;
}
