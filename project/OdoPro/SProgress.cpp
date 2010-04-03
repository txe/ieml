#include "stdafx.h"
#include "SProgress.h"
#include "LiteWnd.h"
#include "string_t.h"
#include "SingeltonApp.h"
#include "json-aux-ext.h"
#include "FastInput.h"
#include "ProgressForSem.h"
#include "Progress_Disciplines.h"

SProgress::SProgress(void)
{
}

SProgress::~SProgress(void)
{
}

// инициализирует дом объекты и все такое
void SProgress::Init(htmlayout::dom::element root)
{
	_root   = root;
	_discip = LiteWnd::link_element(_root, "prog-discip"); 

	// инициируем обработчики 
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-bt-del"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-bt-save"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-bt-fast-input"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-bt-progress"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-bt-discip"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	
	// присоединяем процедуру, отвлавливающую выбор строки со студентом
	HTMLayoutAttachEventHandlerEx(_discip, ElementEventProcForDiscip, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
	// присоединяем процедуру, отвлавливающую выбор строки с баллом
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-estim"), ElementEventProcForBall, this, HANDLE_ALL);


	// заполняем список дисциплин
	LoadListDiscip();
}

// заполняем список дисциплин
void SProgress::LoadListDiscip(void)
{
	// удаляем все строки
	while (_discip.children_count())
		HTMLayoutDetachElement(_discip.child(0));

	string_t query = string_t() +
		"SELECT id, fulltitle, shorttitle, idclass " 
		" FROM disciplines "
		" WHERE idspec = " + aux::itow(theApp.GetSpecIdForStudId(theApp.GetCurrentStudentID())) +
		" AND deleted = 0 "
		" ORDER BY fulltitle";

	
	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

	string_t			buf;
	mybase::MYFASTROW	row;

	while ((row = res.fetch_row()))	
		buf += "<option value=" + row["id"] + ">" + row["fulltitle"] + " [" + row["shorttitle"] + "]" + "</option>";

	assert(_mbslen(buf));
	if (_mbslen(buf))
		_discip.set_html(buf, _mbslen(buf), SIH_REPLACE_CONTENT);
	
	_discip.update();
	if (_discip.children_count() > 1)
		htmlayout::dom::element(_discip.child(0)).set_state(STATE_CURRENT | STATE_CHECKED);
	
	_discip.update();
}

// обрабатывает кнопки
BOOL CALLBACK SProgress::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	SProgress* dlg = static_cast<SProgress*>(tag);
	const wchar_t* id = htmlayout::dom::element(he).get_attribute("id");

	
	if (aux::wcseq(id, L"prog-bt-save"))
	{
		dlg->SaveBall();
		return TRUE;
	}
	if (aux::wcseq(id, L"prog-bt-del"))
	{
		dlg->DeleteBall();
		return TRUE;
	}
	if (aux::wcseq(id, L"prog-bt-fast-input"))
	{	
		dlg->FastInput();
		return TRUE;
	}
	if (aux::wcseq(id, L"prog-bt-progress"))
	{	
		dlg->ProgressForSem();
		return TRUE;
	}
	if (aux::wcseq(id, L"prog-bt-discip"))
	{	
		dlg->EditDiscip();
		return TRUE;
	}

	MessageBox(::GetActiveWindow(), L"Еще не работает", L"Предупреждение", 0);
	return FALSE;
}

// обновляет на экране успеваимость студента
void SProgress::UpdateView(void)
{
	UpdateViewEstimForDiscip();
}

// получает выбранную дисциплину
long SProgress::GetIDSelectedDiscip(void)
{
	return t::GetSelectedRow(_discip).get_attribute_int("value", -1);
}
// обновляет таблицу оценок для выбранного предмета
void SProgress::UpdateViewEstimForDiscip()
{
	htmlayout::dom::element ball_table		= LiteWnd::link_element(_root, "prog-estim"); 
	// удаляем все строки
	while (ball_table.children_count() > 2)
		HTMLayoutDetachElement(ball_table.child(2));

	long discip_id;
	if ((discip_id = GetIDSelectedDiscip()) == -1)
		return;

	string_t query = string_t() +
		"SELECT id, numplansemestr, numgraphsemestr, estimation, ball "
		" FROM progress "
		" WHERE idstud = " + aux::itow(theApp.GetCurrentStudentID()) + 
		" AND iddiscip = " + aux::itow(discip_id) + " AND deleted=0 ORDER BY numplansemestr";
		
	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

	string_t			buf;
	mybase::MYFASTROW	row;

	while (row = res.fetch_row())	
	{	
		string_t type_estim = aux::itow(t::cod2type(aux::wtoi(row["estimation"], -1)));
		buf += "<tr value=" + row["id"] + ">";
		buf += string_t() +
		+  "<td>" + row["numplansemestr"]				+ "</td>"
		+  "<td>" + row["numgraphsemestr"]				+ "</td>"
		+  "<td estim=" + type_estim + ">" + t::cod2text(row["estimation"])		+ "</td>"
		+  "<td>" + row["ball"]							+ "</td>"
		+  "</tr>";
	}

	if (_mbslen(buf))
		ball_table.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
	
	ball_table.update();
	if (ball_table.children_count() > 2)
		htmlayout::dom::element(ball_table.child(2)).set_state(STATE_CURRENT);

	// зададим сверху текущее название дисциплины
	string_t txt = "Оценка: " + json::get_caption(_discip);
	htmlayout::dom::element title	= LiteWnd::link_element(_root, "prog-discip-title");
	title.set_text(txt);
	title.update(false);


	ball_table.update();
}

// обрабатывает выбор дисциплины
BOOL CALLBACK SProgress::ElementEventProcForDiscip(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != SELECT_SELECTION_CHANGED)
		return FALSE;

	SProgress* dlg = static_cast<SProgress*>(tag);
	dlg->UpdateViewEstimForDiscip();

	return TRUE;
}
// сохраняет оценку для дисциплины текущего студента
void SProgress::SaveBall(void)
{
	if (CheckValue())
		return;

	string_t studid   = aux::itow(theApp.GetCurrentStudentID());
	string_t discipid = aux::itow(GetIDSelectedDiscip());
	string_t graph_sem	= json::v2t(LiteWnd::link_element(_root, "prog-graph-sem").get_value());
	string_t plan_sem	= json::v2t(LiteWnd::link_element(_root, "prog-plan-sem").get_value());
	string_t ball		= t::ball2sql(json::v2t(LiteWnd::link_element(_root, "prog-ball").get_value()));
	string_t estim		= aux::itow(t::type2cod(aux::wtoi(json::v2t(LiteWnd::link_element(_root, "prog-est").get_value()), -1),
		aux::strtod(ball, -1)));

	string_t query = string_t() + 
		"INSERT INTO progress "
		" (idstud, iddiscip, numplansemestr, numgraphsemestr, estimation, ball) VALUES"
		" (" + studid + ", " +discipid + ", " + plan_sem + ", " +  graph_sem + 
		", " + estim + ", " + ball + ")";
	 
	theApp.GetCon().Query(query);
	if (theApp.GetCon().AffectedRows() == 0)
		throw wss::exception(wss::reason_message(FULL_LOCATION()));

	UpdateView();
}

// удаляет выбранный балл
void SProgress::DeleteBall(void)
{
	long ball_id = GetIDSelectedBall();
	if (ball_id == -1)
		return;

	if (IDNO == MessageBox(::GetActiveWindow(), L"Удалить выбранную оценку?", L"Сообщение", 
		MB_YESNO |MB_ICONINFORMATION | MB_APPLMODAL | MB_SETFOREGROUND))
		return ;

	string_t query = string_t() + 
		" UPDATE progress "
		" SET deleted = 1 "
		" WHERE id = " + aux::itow(ball_id);
	
	theApp.GetCon().Query(query);
	if (theApp.GetCon().AffectedRows() == 0)
		throw wss::exception(wss::reason_message(FULL_LOCATION()));

	UpdateView();
}

// проверяет на корректность введенный балл
int SProgress::CheckValue(void)
{
	int		graph_sem	= aux::wtoi(json::v2t(LiteWnd::link_element(_root, "prog-graph-sem").get_value()), -1);
	int		plan_sem	= aux::wtoi(json::v2t(LiteWnd::link_element(_root, "prog-plan-sem").get_value()), -1);
	double	ball		= aux::strtod(json::v2t(LiteWnd::link_element(_root, "prog-ball").get_value()), -1);
	int		estim_type	= aux::wtoi(json::v2t(LiteWnd::link_element(_root, "prog-est").get_value()), -1);

	string_t msg;
	if (graph_sem < 1 || graph_sem > 12)
		msg = "Значение граф-семестра должно быть в диапазоне [1-12]";
	if (plan_sem < 1 || plan_sem > 12)
		msg = "Значение план-семестра должно быть в диапазоне [1-12]";
	if (ball < 2.5 || ball > 5.0)
		msg = "Значение балла должно быть в диапазоне [2.5-5.0]";
	
	if (msg.empty())
		return 0;
	
	MessageBox(::GetActiveWindow(), msg, 
		L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	
	return 1;
}

// возвращает ид выбранного бала
long SProgress::GetIDSelectedBall(void)
{
	// ищем выбранный балл
	htmlayout::dom::element ball_table = LiteWnd::link_element(_root, "prog-estim"); 
	assert(_discip.is_valid());
	htmlayout::dom::element row;
	for (UINT i = 2; i < ball_table.children_count(); ++i)
	{
		htmlayout::dom::element t = ball_table.child(i);
		if (t.get_state(STATE_CURRENT))
		{
			row = t;
			break;
		}
	}	
	if (!row.is_valid()) // если нет выбранных строк
		return -1;

	return row.get_attribute_int("value", -1);
}

// вызывает окно быстрого ввода оценок для всей группы для данной дисциплины
void SProgress::FastInput(void)
{
	CProgressFastInputBallDlg dlg;
	dlg.SetParam(GetIDSelectedDiscip(),
		json::v2t(LiteWnd::link_element(_root, "prog-plan-sem").get_value()),
		json::v2t(LiteWnd::link_element(_root, "prog-graph-sem").get_value()),
		json::v2t(LiteWnd::link_element(_root, "prog-est").get_value()));
	dlg.DoModal();
}

// обрабатывает выбор балла
BOOL CALLBACK SProgress::ElementEventProcForBall(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != TABLE_ROW_CLICK)
		return FALSE;

	SProgress* dlg = static_cast<SProgress*>(tag);

	// обновим значения семестров, значения берем из выбранной строки с балом
	htmlayout::dom::element row = htmlayout::dom::element(he).child(pr->reason);
	htmlayout::dom::element plan = row.child(0);
	htmlayout::dom::element graph = row.child(1);
	htmlayout::dom::element estim = row.child(2);
	htmlayout::dom::element ball = row.child(3);
	
	json::t2v(LiteWnd::link_element(dlg->_root, "prog-plan-sem"), string_t(plan.get_text()));
	json::t2v(LiteWnd::link_element(dlg->_root, "prog-graph-sem"), string_t(graph.get_text()));
	json::t2v(LiteWnd::link_element(dlg->_root, "prog-est"), string_t(estim.get_attribute("estim")));

	return FALSE;
}

// показвает дисциплины за семестр
void SProgress::ProgressForSem(void)
{
	CProgressViewBallForSemDlg dlg;
	dlg.SetParam(json::v2t(LiteWnd::link_element(_root, "prog-plan-sem").get_value()),
		json::v2t(LiteWnd::link_element(_root, "prog-graph-sem").get_value()));
	dlg.DoModal();
}

// показывает диалог редактирования дисциплин
void SProgress::EditDiscip(void)
{
	CManagDisciplinesDlg dlg;
	dlg.DoModal();
}
