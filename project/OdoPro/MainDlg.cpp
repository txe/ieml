// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SingeltonApp.h"
#include "MainDlg.h"
#include "PropertyDlg.h"
#include "Dictionary.h"
#include "json-aux-ext.h"
#include "logger.h"

using namespace htmlayout::dom;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

CMainDlg::CMainDlg(LiteWnd* pParent /*=NULL*/)
	: LiteWnd(pParent)
{
}

BOOL CMainDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;
	
	#if defined(_DEBUG)
		cs.lpszName = (LPCWSTR)"OdoPro Debug";
	#else
		cs.lpszName = (LPCWSTR)"OdoPro";
	#endif
	
	cs.cx = 800;
	cs.cy = 600;
	return TRUE;
}

int CMainDlg::OnCreate()
{
	CenterWindow();

	PBYTE pb; DWORD cb;
	if (GetHtmlResource(L"IDR_HTML_MAIN", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	else
		assert(FALSE);

	InitDomElement();
// ������� ������ �����
	UpdateGrpList("group-bu", "��%");
	UpdateGrpList("group-mo", "��%");
	UpdateGrpList("group-mr", "��%");
	UpdateGrpList("group-u",  "�%");
	UpdateGrpList("group-ep", "��%");
	UpdateGrpList("group-eun","���%");
	UpdateGrpList("group-pm", "��%");
	UpdateGrpList("group-pgs", "���%");
	UpdateGrpList("group-tgv", "���%");
	// ��������� ������� ���
	UpdateGrpList("group-otch", "%'"
		" AND title NOT like '��%'"
		" AND title NOT like '��%'"
		" AND title NOT like '��%'"
		" AND title NOT like '�%'"
		" AND title NOT like '��%'"
		" AND title NOT like '���%'"
		" AND title NOT like '��%'"
		" AND title NOT like '���%'"
		" AND title NOT like '���%");
	//  �������� ���������� ������ ���	
	SetStatusBar(SB_HOST, theApp.GetCon().GetParam(mybase::PR_HOST) + ":" + 
		theApp.GetCon().GetParam(mybase::PR_PORT));
	SetStatusBar(SB_LOGIN, theApp.GetCon().GetParam(mybase::PR_LOGIN));
	SetStatusBar(SB_BD, theApp.GetCon().GetParam(mybase::PR_BD));
	SetStatusBar(SB_BUILD,  string_t(__DATE__) + "/" + string_t(__TIME__));
	// ��������� ������
	manag_rep_.Init(link_element("menu-bar"));
	manag_actions_.Init(this, link_element("menu-bar"));
	theApp.GetUpdater().Init(m_hWnd);
	//  ��������� ������� ���������
	theApp.SetCurrentGroupID(1538);
	UpdateGrid();

	return 0;
}

// ��������� �������� ��� � ������������
void CMainDlg::InitDomElement(void)
{
	stud_grid_	= link_element("main-grid");
	// ������������ ���������, �������������� ����� ������ �� ���������
	HTMLayoutAttachEventHandlerEx(stud_grid_, ElementEventProcForGrid, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
	
	// ������������ ������������ � �������
	HTMLayoutAttachEventHandlerEx(link_element("bt-create"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-delete"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-dictionary"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);

	HTMLayoutAttachEventHandlerEx(link_element("bt-find"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	// ���������� ��� ���������� ���� ������
	//HTMLayoutAttachEventHandlerEx(link_element("filter_type"), ElementEventProcChangeFindType, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
	// ���������� ��� ����
	HTMLayoutAttachEventHandlerEx(link_element("menu-bar"), ElementEventProcMenu, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}

// ��������� ������� �� ����������
void CMainDlg::UpdateGrid(void)
{
	assert(stud_grid_.is_valid());

	string_t  query = string_t() +
			" SELECT students.id, secondname, firstname, thirdname, znum, grpid, "
			" voc.title AS city, voc1.title AS `group` "
			" FROM students, voc, voc AS voc1 "
			" WHERE grpid = " + aux::itow(theApp.GetCurrentGroupID()) +
			" AND students.deleted = 0 AND voc.vkey = 'city' AND voc.num = "
			" students.cityid AND voc.deleted = 0 AND voc1.vkey = 'grp' AND"
			" voc1.num = grpid AND voc1.deleted = 0 "
			" ORDER BY secondname, firstname, thirdname, id;";

	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
		
	string_t			buf;
	mybase::MYFASTROW	row;
	int					count = 0;

	SetStatusBar(SB_COUNT, "...");
	// ������� ������ ����� ��������� �������
	while (stud_grid_.children_count() > 1)
		HTMLayoutDetachElement(stud_grid_.child(1));
		
	while (row = res.fetch_row())	
	{
		buf += "<tr value=" + row["id"] + " grpid=" + row["grpid"] + ">";
		buf += string_t() + "<td>" + aux::itow(++count)	+ "</td>"
			"<td>" + row["secondname"]	+ "</td>"
			"<td>" + row["firstname"]	+ "</td>"
			"<td>" + row["thirdname"]	+ "</td>"
			"<td>" + row["znum"]		+ "</td>"
			"<td>" + row["group"]		+ "</td>"
			"<td>" + row["city"]		+ "</td>"
			"</tr>";
	}
	
	if (_mbslen(buf))
		stud_grid_.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
	stud_grid_.update();
	// ���� � ������� ���� ���� �� ���� �������, �� ������ �������
	// ��� ����� ���������� �������� ��� ������� � ������
	if (stud_grid_.children_count() > 1)
	{
		string_t selector = string_t() + "tr[value=" + 
			aux::itow(theApp.GetCurrentStudentID(false)) + "]";
		htmlayout::dom::element find = stud_grid_.find_first(selector.c_str());
		if (!find.is_valid())
			find = stud_grid_.child(1);
		find.set_state(STATE_CURRENT);
		find.scroll_to_view();
	}
	
	theApp.SetCurrentStudentID(GetSelectedStudentID());
	// � ������ ����� ���� ����� ����������� ���������, ������� ������ ��� ��������
	// ��� �� ��������� ����� � �� ����, ����� ������ �� ������
	int grp = GetSelectedStudent().get_attribute_int("grpid", -1);
	if (grp != -1)
		theApp.SetCurrentGroupID(grp);

	SetStatusBar(SB_COUNT, string_t(aux::itow(count)));

	stud_grid_.update();
}

// ��������� ������ ������
void CMainDlg::UpdateGrpList(const string_t& dom_name, const string_t& grp_name_filter)
{
	htmlayout::dom::element group_list = link_element(dom_name.c_str());
	// ������������ ���������, �������������� ����� ������ � �������
	HTMLayoutAttachEventHandlerEx(group_list, ElementEventProcForPanelGroup, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

	string_t query	= 
			" SELECT num , title FROM voc "
			" WHERE vkey='grp' AND deleted = 0 AND "
			" title like '" + grp_name_filter + "' "
			" ORDER BY title, id";
	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
	
	string_t			buf;
	mybase::MYFASTROW	row;

	while (row = res.fetch_row())	
		buf += "<option value=" + row["num"] + ">" +row["title"] + "</option>";
	
	assert(_mbslen(buf));
	if (_mbslen(buf))
		group_list.set_html(buf, _mbslen(buf), SIH_REPLACE_CONTENT);
}

// ������ �������� � ������-����
void CMainDlg::SetStatusBar(const UINT& status,const string_t& value)
{
//	enum {SB_HOST = 0, SB_LOGIN, SB_BD, SB_FILTR, SB_COUNT, SB_BUILD};
	htmlayout::dom::element r		 = htmlayout::dom::element::root_element(m_hWnd);
	htmlayout::dom::element sb;
	switch (status)
	{
		case SB_HOST:	sb = link_element("sb-host");
			break;
		case SB_LOGIN:	sb = link_element("sb-login");
			break;
		case SB_BD:		sb = link_element("sb-bd");
			break;
		case SB_FILTR:	sb = link_element("sb-filter");
			break;
		case SB_COUNT:	sb = link_element("sb-count");
			break;
		case SB_BUILD:	sb = link_element("sb-build");
			break;
		default:
			assert(false);
			return;
	}
	sb.set_text(aux::a2w(value.c_str()));
	sb.update(true);
}

// �������� ������ �� ���������� ��������
void CMainDlg::StudentProperty(void)
{
	// ���� ���������� ��������
	long id = GetSelectedStudentID();
	if (id < 0)
		return;
	
	theApp.SetCurrentStudentID(id);
	theApp.SetCurrentGroupID(GetSelectedStudent().get_attribute_int("grpid", -1));
	
	try
	{
		CPropertyDlg dlg(this);
		dlg.DoModal();
		UpdateGrid();
		return;
	}
	catch (...)
	{
		LOG_ERROR << "������ ��� ���������/�������������� ������� ��������";
		theApp.ExceptionManage();	
	}

}

//////////////////////////////////////////////////////////////////////////
// ���������� ��� ������ ������
BOOL CALLBACK CMainDlg::ElementEventProcForPanelGroup(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != SELECT_SELECTION_CHANGED)
		return FALSE;

	CMainDlg* main = static_cast<CMainDlg*>(tag);
	htmlayout::dom::element el = pr->he;
	theApp.SetCurrentGroupID(aux::wtoi(el.get_attribute("value"), -1));
	main->UpdateGrid();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// ���������� ��� ������ ��������
BOOL CALLBACK CMainDlg::ElementEventProcForGrid(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	CMainDlg* main = static_cast<CMainDlg*>(tag);

	if (pr->cmd == TABLE_ROW_CLICK)
	{
		long id = main->GetSelectedStudentID();
		if (id < 0)
			return FALSE;
		theApp.SetCurrentStudentID(id);
		theApp.SetCurrentGroupID(main->GetSelectedStudent().get_attribute_int("grpid", -1));
		return TRUE;
	}

	if (pr->cmd == TABLE_ROW_DBL_CLICK)
	{
		main->StudentProperty();
		return TRUE;
	}
	return FALSE;
}

// ������������ ������
BOOL CALLBACK CMainDlg::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CMainDlg*		main	= static_cast<CMainDlg*>(tag);
	const wchar_t*	id	= htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-create"))
	{
		main->CreateStudent();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-delete"))
	{
		main->DeleteStudent();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-find"))
	{
		main->ShowFindResult();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-dictionary"))
	{
		CDictionary dlg(main);
		dlg.DoModal();
		return TRUE;
	}
	return FALSE;
}

// ������� ��������
void CMainDlg::CreateStudent(void)
{
	if (theApp.GetCurrentGroupID() < 0)
		return;

	try
	{
		CPropertyDlg dlg(this);
		dlg.CreateData();
		dlg.DoModal();
		UpdateGrid();
		return;
	}
	catch (...)
	{
		theApp.ExceptionManage();
		LOG_ERROR << "������ ��� �������� ��������";
	}
	
}

// ������� ��������
void CMainDlg::DeleteStudent(void)
{
	// ���� ���������� ��������
	long id = GetSelectedStudentID();
	if (id < 0)
		return;

	theApp.SetCurrentStudentID(id);
	
	string_t msg = "�� �������� ������ � �������� \n" + theApp.GetFIO() + "\n"
		"�� ������ ����������?";
	if (IDNO == MessageBox(m_hWnd, msg, L"��������������", MB_YESNO |MB_ICONINFORMATION | MB_APPLMODAL))
		return;

	try
	{
		CPropertyDlg dlg(this);
		dlg.DeleteData();
		MessageBeep(0);
		UpdateGrid();
		return;
	}
	catch (...)
	{
		theApp.ExceptionManage();
		LOG_ERROR << "������ ��� �������� ��������";
	}
}

// ���������� �������� ������� ��� ������ � �������
htmlayout::dom::element CMainDlg::GetSelectedStudent(void)
{
	// ���� ���������� ��������
	assert(stud_grid_.is_valid());
	htmlayout::dom::element row;
	for (UINT i = 1; i < stud_grid_.children_count(); ++i)
	{
		htmlayout::dom::element t = stud_grid_.child(i);
		if (t.get_state(STATE_CURRENT))
		{
			row = t;
			break;
		}
	}	
	return row;
}

// ���������� id �������� ������� ��� ������ � �������
long CMainDlg::GetSelectedStudentID(void)
{
	element st = GetSelectedStudent();
	if (!st.is_valid()) // ���� ��� ��������� �����
		return -1;

	return st.get_attribute_int("value", -1);
}

// ���������� ��������� ���� ������
BOOL CALLBACK CMainDlg::ElementEventProcChangeFindType(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != SELECT_SELECTION_CHANGED)
		return FALSE;

	CMainDlg* dlg = static_cast<CMainDlg*>(tag);
	std::string find_type = json::v2t(htmlayout::dom::element(he).get_value());
	string_t html;
	if (find_type == "date")
		html = "<input type=\"date\" id=\"find\"/>";
	else
		if (find_type == "number")
			html = "<input type=\"number\" id=\"find\" value=\"0\" size=\"6\" step=\"1\" style=\"overflow: hidden; width: 150px;\"/>";
		else
			html = "<input type=\"text\" id=\"find\" style=\"overflow: hidden; width: 150px;\" />";
	
	dlg->link_element("find").set_html(html, _mbslen(html), SIH_REPLACE_CONTENT);
	dlg->link_element("find").update(true);
	return TRUE;

}

// ���������� ��������� ������ 
void CMainDlg::ShowFindResult()
{
	assert(stud_grid_.is_valid());

	string_t find = json::v2t(link_element("find").get_value());
	if (strlen(find) < 4)
	{
		string_t msg = "����� ��������� ��������� �� ��������� 3-� ��������, � ���� ��������� ������.\n"
			"��������� ����� �� �������� ���������� ���������� ������� �� ���������� ����� ��������.\n"
			"����������?";
		if (IDNO == MessageBox(m_hWnd, msg, L"��������������", MB_YESNO |MB_ICONINFORMATION | MB_APPLMODAL))
			return;
	}
	
	std::string find_type = json::v2t(link_element("find_type"));
	
	string_t wh = "0 != 0";
	
	if (find_type == "fio")
		wh = " (secondname LIKE '" + find + "%' OR znum LIKE '" + find + "' ) ";
	if (find_type == "text")
	{
		wh =	" (  secondname	LIKE '%" + find + "%'"
				" OR firstname	LIKE '%" + find + "%'"
				" OR thirdname	LIKE '%" + find + "%'"
				" OR passplace	LIKE '%" + find + "%'"
				" OR email		LIKE '%" + find + "%'"
				" OR addr		LIKE '%" + find + "%'"
				" OR liveaddr	LIKE '%" + find + "%'"
				" OR edudocinfo LIKE '%" + find + "%'"
				" OR eduplace	LIKE '%" + find + "%'"
				" OR eduspec	LIKE '%" + find + "%' ) ";
	}

	string_t  query	= string_t() 
		+	" SELECT students.id, secondname, firstname, thirdname, znum, grpid, "
			" voc.title AS city, voc1.title AS `group` "
			" FROM students, voc, voc AS voc1 "
			" WHERE "
			" students.deleted = 0 AND voc.vkey = 'city' AND voc.num = "
			" students.cityid AND voc.deleted = 0 AND voc1.vkey = 'grp' AND"
			" voc1.num = grpid AND voc1.deleted = 0 AND "
			+ wh +
			" ORDER BY secondname, firstname, thirdname, id;";

	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

	string_t			buf;
	mybase::MYFASTROW	row;
	int					count = 0;

	SetStatusBar(SB_COUNT, "...");
	// ������� ������ ����� ��������� �������
	while (stud_grid_.children_count() > 1)
		HTMLayoutDetachElement(stud_grid_.child(1));

	while (row = res.fetch_row())	
	{
		buf += "<tr value=" + row["id"] + " grpid = " + row["grpid"] + ">";
		buf += string_t() + "<td>" + aux::itow(++count)	+ "</td>"
			"<td>" + row["secondname"]	+ "</td>"
			"<td>" + row["firstname"]	+ "</td>"
			"<td>" + row["thirdname"]	+ "</td>"
			"<td>" + row["znum"]		+ "</td>"
			"<td>" + row["group"]		+ "</td>"
			"<td>" + row["city"]		+ "</td>"
			"</tr>";
	}

	if (_mbslen(buf))
		stud_grid_.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
	stud_grid_.update();
	// ���� � ������� ���� ���� �� ���� �������, �� ������ �������
	// ��� ����� ���������� �������� ��� ������� � ������
	if (stud_grid_.children_count() > 1)
	{
		htmlayout::dom::element first = stud_grid_.child(1);
		first.set_state(STATE_CURRENT);
	}

	theApp.SetCurrentStudentID(GetSelectedStudentID());
	theApp.SetCurrentGroupID(GetSelectedStudent().get_attribute_int("grpid", -1));

	SetStatusBar(SB_COUNT, string_t(aux::itow(count)));

	stud_grid_.update();
}

// ���������� ��������� ������������ ������
void CMainDlg::ShowFindResultEx()				
{
	
}

// ������������ ����
BOOL CALLBACK CMainDlg::ElementEventProcMenu(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != MENU_ITEM_CLICK)
		return FALSE;

	CMainDlg* main = static_cast<CMainDlg*>(tag);

	const wchar_t* id = element(pr->heTarget).get_attribute("id");
	if (aux::wcseq(id, L"run-update"))
	{
		theApp.GetUpdater().RunUpdate();
		return TRUE;
	}
	if (aux::wcseq(id, L"exit"))
	{
		main->Close();
		return TRUE;
	}
	return FALSE;
}
