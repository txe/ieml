#include "stdafx.h"
#include "SProgress.h"
#include "LiteWnd.h"
#include "string_t.h"
#include "SingeltonApp.h"
#include "json-aux-ext.h"
#include "FastInput.h"
#include "ProgressForSem.h"
#include "Progress_Disciplines.h"
#include "ChangeDisc.h"

SProgress::SProgress(void)
{
}

SProgress::~SProgress(void)
{
}

// �������������� ��� ������� � ��� �����
void SProgress::Init(htmlayout::dom::element root)
{
	_root   = root;
	_discip = LiteWnd::link_element(_root, "prog-discip"); 

	// ���������� ����������� 
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-bt-del"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-bt-save"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-bt-fast-input"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-bt-progress"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-bt-discip"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-bt-change-disc"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);

	// ������������ ���������, �������������� ����� ������ � �����������
	HTMLayoutAttachEventHandlerEx(_discip, ElementEventProcForDiscip, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
	// ������������ ���������, �������������� ����� ������ � ������
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "prog-estim"), ElementEventProcForBall, this, HANDLE_ALL);

	// ��������� ������ ���������
	LoadListDiscip();
}

// ��������� ������ ���������
void SProgress::LoadListDiscip(void)
{
	long cur_didcip_id = GetIDSelectedDiscip();

	// ������� ��� ������
  t::ClearTable(_discip, 1);

	string_t query = string_t() +
		"SELECT id, fulltitle, shorttitle, idclass " 
		" FROM disciplines "
		" WHERE idspec = " + aux::itow(theApp.GetSpecIdForStudId(theApp.GetCurrentStudentID())) +
		" AND deleted = 0 "
		" ORDER BY fulltitle";


  string_t buf;
  string_t buf2;

	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
	mybase::MYFASTROW	row;

  // ������������ ���������� �������� � ����� ��� �������
	while ((row = res.fetch_row()))	
    if (row["idclass"] == string_t(L"0"))
      buf2 += "<tr value=" + row["id"] + " style='color:red'><td>" + row["id"] + "</td>"
              "<td>" + row["fulltitle"] + " (" + row["shorttitle"] + ")" + "</td>"
              "</tr>";
    else
      buf += "<tr value=" + row["id"] + "><td>" + row["id"] + "</td>"
			       "<td>" + row["fulltitle"] + " (" + row["shorttitle"] + ")" + "</td>"
			       "</tr>";

  buf += buf2;
	assert(_mbslen(buf));
	if (_mbslen(buf))
		_discip.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
	
	_discip.update();

	// ���� � ������� ���� ���� �� ���� ����������, �� ������ �������
	// ��� ����� ��������� ��� ������� � ������
	if (_discip.children_count() > 1)
	{
		string_t selector = string_t() + "tr[value=" + aux::itow(cur_didcip_id) + "]";
		htmlayout::dom::element find = _discip.find_first(selector.c_str());
		if (!find.is_valid())
			find = _discip.child(1);
		find.set_state(STATE_CURRENT);
		find.scroll_to_view();
	}
	
	_discip.update();
}

// ������������ ������
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
	if (aux::wcseq(id, L"prog-bt-change-disc"))
	{	
		dlg->ChangeDiscip();
		return TRUE;
	}

	MessageBox(::GetActiveWindow(), L"��� �� ��������", L"��������������", 0);
	return FALSE;
}

// ��������� �� ������ ������������ ��������
void SProgress::UpdateView(void)
{
	UpdateViewEstimForDiscip();
}

// �������� ��������� ����������
long SProgress::GetIDSelectedDiscip(void)
{
	// ���� ���������� ��������
	assert(_discip.is_valid());
	htmlayout::dom::element row;
	for (UINT i = 1; i < _discip.children_count(); ++i)
	{
		htmlayout::dom::element t = _discip.child(i);
		if (t.get_state(STATE_CURRENT))
			return t.get_attribute_int("value", -1);
	}	

	return -1;
}

// ��������� ������� ������ ��� ���������� ��������
void SProgress::UpdateViewEstimForDiscip()
{
	htmlayout::dom::element ball_table	= LiteWnd::link_element(_root, "prog-estim"); 

  // ������� ��� ������
  while (ball_table.children_count() > 2)
    HTMLayoutDetachElement(ball_table.child(2));
  //t::ClearTable(ball_table, 2);

	long discip_id = GetIDSelectedDiscip();
	if (discip_id == -1)
		return;

	string_t query = string_t() +
		"SELECT id, numplansemestr, numgraphsemestr, estimation, ball, from_disc "
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
		+  "<td>" + row["numplansemestr"]	+ "</td>"
		+  "<td>" + row["numgraphsemestr"]	+ "</td>"
		+  "<td estim=" + type_estim + ">" + t::cod2text(row["estimation"])	+ "</td>"
		+  "<td>" + row["ball"]				+ "</td>"
		+  "<td>" + row["from_disc"] + "<\td>"
		+  "</tr>";
	}

	if (_mbslen(buf))
		ball_table.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
	
	ball_table.update();

	if (ball_table.children_count() > 2)
	{
		htmlayout::dom::element(ball_table.child(2)).set_state(STATE_CURRENT);
		UpdateEditFromBall(ball_table.child(2));
	}

	ball_table.update();
}

// ��������� �������� ��� �������������� ������
void SProgress::UpdateEditFromBall(htmlayout::dom::element row)
{
	htmlayout::dom::element plan   = row.child(0);
	htmlayout::dom::element graph  = row.child(1);
	htmlayout::dom::element estim  = row.child(2);
	htmlayout::dom::element ball   = row.child(3);

	json::t2v(LiteWnd::link_element(_root, "prog-plan-sem"),  string_t(plan.get_text()));
	json::t2v(LiteWnd::link_element(_root, "prog-graph-sem"), string_t(graph.get_text()));
	json::t2v(LiteWnd::link_element(_root, "prog-est"),       string_t(estim.get_attribute("estim")));
}

//
// ������������ ����� ����������
BOOL CALLBACK SProgress::ElementEventProcForDiscip(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != TABLE_ROW_CLICK)
		return FALSE;

	SProgress* dlg = static_cast<SProgress*>(tag);
	dlg->UpdateViewEstimForDiscip();

	return TRUE;
}
// ��������� ������ ��� ���������� �������� ��������
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

// ������� ��������� ����
void SProgress::DeleteBall(void)
{
	long ball_id = GetIDSelectedBall();
	if (ball_id == -1)
		return;

	if (IDNO == MessageBox(::GetActiveWindow(), L"������� ��������� ������?", L"���������", 
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

// ��������� �� ������������ ��������� ����
int SProgress::CheckValue(void)
{
	int		graph_sem	= aux::wtoi(json::v2t(LiteWnd::link_element(_root, "prog-graph-sem").get_value()), -1);
	int		plan_sem	= aux::wtoi(json::v2t(LiteWnd::link_element(_root, "prog-plan-sem").get_value()), -1);
	double	ball		= aux::strtod(json::v2t(LiteWnd::link_element(_root, "prog-ball").get_value()), -1);
	int		estim_type	= aux::wtoi(json::v2t(LiteWnd::link_element(_root, "prog-est").get_value()), -1);

	string_t msg;
	if (graph_sem < 1 || graph_sem > 12)
		msg = "�������� ����-�������� ������ ���� � ��������� [1-12]";
	if (plan_sem < 1 || plan_sem > 12)
		msg = "�������� ����-�������� ������ ���� � ��������� [1-12]";
	if (ball < 2.5 || ball > 5.0)
		msg = "�������� ����� ������ ���� � ��������� [2.5-5.0]";
	
	if (msg.empty())
		return 0;
	
	MessageBox(::GetActiveWindow(), msg, 
		L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	
	return 1;
}

// ���������� �� ���������� ����
long SProgress::GetIDSelectedBall(void)
{
	// ���� ��������� ����
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
	if (!row.is_valid()) // ���� ��� ��������� �����
		return -1;

	return row.get_attribute_int("value", -1);
}

// �������� ���� �������� ����� ������ ��� ���� ������ ��� ������ ����������
void SProgress::FastInput(void)
{
	CProgressFastInputBallDlg dlg;
	dlg.SetParam(GetIDSelectedDiscip(),
		json::v2t(LiteWnd::link_element(_root, "prog-plan-sem").get_value()),
		json::v2t(LiteWnd::link_element(_root, "prog-graph-sem").get_value()),
		json::v2t(LiteWnd::link_element(_root, "prog-est").get_value()));
	dlg.DoModal();
}

// ������������ ����� �����
BOOL CALLBACK SProgress::ElementEventProcForBall(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != TABLE_ROW_CLICK)
		return FALSE;

	SProgress* dlg = static_cast<SProgress*>(tag);

	// ������� �������� ���������, �������� ����� �� ��������� ������ � �����
	htmlayout::dom::element row = htmlayout::dom::element(he).child(pr->reason);
	dlg->UpdateEditFromBall(row);

	return FALSE;
}

// ��������� ���������� �� �������
void SProgress::ProgressForSem(void)
{
	CProgressViewBallForSemDlg dlg;
	dlg.SetParam(json::v2t(LiteWnd::link_element(_root, "prog-plan-sem").get_value()),
		json::v2t(LiteWnd::link_element(_root, "prog-graph-sem").get_value()));
	dlg.DoModal();
}

// ���������� ������ �������������� ���������
void SProgress::EditDiscip(void)
{
	CManagDisciplinesDlg dlg;
	dlg.DoModal();
}

// ���������� ������ �������� ������ �� ����� ���������� � ������
void SProgress::ChangeDiscip()
{
	ChangeDisc dlg;
	dlg.DoModal();
	UpdateViewEstimForDiscip();
}
