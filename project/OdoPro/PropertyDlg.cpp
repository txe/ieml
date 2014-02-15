// PropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyDlg.h"
#include "SingeltonApp.h"


CPropertyDlg::CPropertyDlg(LiteWnd* pParent /*=NULL*/)
	: LiteWnd(pParent)
{
}

CPropertyDlg::~CPropertyDlg()
{
}

BOOL CPropertyDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"������ �� ��������";
	return TRUE;
}

int CPropertyDlg::OnCreate()
{
	SetWindowPos(m_hWnd, NULL, -1, -1, 640, 575, 
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	PBYTE pb; DWORD cb;
	if(GetHtmlResource(L"IDR_HTML_PROP", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	InitDomElement();
	InitStudIds();
	UpdateView();

	return 0;
}

// ������������ ������
BOOL CALLBACK CPropertyDlg::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CPropertyDlg* dlg = static_cast<CPropertyDlg*>(tag);
	const wchar_t* id = htmlayout::dom::element(he).get_attribute("id");
	
	if (aux::wcseq(id, L"bt-next"))
	{
		dlg->NextStudent();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-prev"))
	{
		dlg->PrevStudent();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-close"))
	{	
		//LOG_DEBUG << string_t(htmlayout::dom::element(htmlayout::dom::element::root_element(dlg->m_hWnd)).get_html());
		dlg->Close();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-save"))
	{	
		dlg->Save();
		return TRUE;
	}

	return FALSE;
}

// ��������� �������� ��� � ������������ � ����������
void CPropertyDlg::InitDomElement(void)
{
	header_ = link_element("sb-header"); 
	// ��������� � ��������� ������
	HTMLayoutAttachEventHandlerEx(link_element("bt-next"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-prev"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-save"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);

	s_data_.Init(htmlayout::dom::element::root_element(m_hWnd));
	s_progress_.Init(htmlayout::dom::element::root_element(m_hWnd));
	s_payment_.Init(htmlayout::dom::element::root_element(m_hWnd));
}

// ��������� � ����������� ������� ��������
bool CPropertyDlg::UpdateView(void)
{
	if ( -1 == theApp.GetCurrentStudentID())
		throw wss::exception(wss::reason_message(FULL_LOCATION()));

	s_data_.UpdateView();
	s_progress_.UpdateView();
	s_payment_.UpdateView();
	
	// ��������� ���������
	string_t header =  theApp.GetFIO() + " (" + 
		string_t(aux::itoa(GetPosFromId(theApp.GetCurrentStudentID()))) + 
		"/" + string_t(aux::itoa((UINT)students_map_.size()/2)) + string_t(")");
	header_.set_text(json::value((const wchar_t*)header));
	header_.update(true);
	return true;
}

// �������������� ������ ���������
void CPropertyDlg::InitStudIds(void)
{
	string_t  query	= string_t() +
		" SELECT id "
		" FROM students "
		" WHERE grpid = " + string_t(aux::itoa(theApp.GetCurrentGroupID())) +
		" AND students.deleted = 0 "
		" ORDER BY secondname, firstname, thirdname, id;";

	mybase::MYFASTRESULT	res		= theApp.GetCon().Query(query);	
	long					count	= 0;
	string_t				buf;
	mybase::MYFASTROW		row;
	
	students_map_.clear();
	typedef std::pair<long, long> PairMap;
	while (row = res.fetch_row())
	{
		students_map_.insert(PairMap(aux::wtoi(row["id"]), ++count));
		students_map_.insert(PairMap(0 - count, aux::wtoi(row["id"])));
	}

}

// ���������� ������ �� ���������� �������� � ������
void CPropertyDlg::NextStudent(void)
{
	long pos = GetPosFromId(theApp.GetCurrentStudentID());
	if (pos == students_map_.size()/2)
	{
		MessageBeep(MB_OK);
		return;
	}
	theApp.SetCurrentStudentID(GetIdFromPos(pos + 1));
	UpdateView();
}

// ���������� ������ �� ����������� �������� � ������ 
void CPropertyDlg::PrevStudent(void)
{
	long pos = GetPosFromId(theApp.GetCurrentStudentID());
	if (pos == 1)
	{
		MessageBeep(MB_OK);
		return; 
	}
	theApp.SetCurrentStudentID(GetIdFromPos(pos - 1));
	UpdateView();
}

// c���������
void CPropertyDlg::Save(void)
{
	s_data_.SaveData();
}

// ������� ������ �������� ��� ������� ������
bool CPropertyDlg::CreateData(void)
{
	return s_data_.CreateData();
}

// ������� ������ � ��������
bool CPropertyDlg::DeleteData(void)
{
	InitStudIds();
	long id = -1;
	long pos = GetPosFromId(theApp.GetCurrentStudentID());
	if ((++pos <= (int)students_map_.size()/2) || ((pos -= 2) != 0))
		id = GetIdFromPos(pos);
	
	bool rez = s_data_.DeleteData();

	theApp.SetCurrentStudentID(id);
	
	return rez;
}

// ��������� ������� �������� � ������ �� �������������
long CPropertyDlg::GetPosFromId(const long& stud_id)
{
	std::map<long, long>::const_iterator pos = students_map_.find(stud_id);
	if (pos == students_map_.end())
	{
		string_t msg = string_t("��� ������ id = '") + aux::itow(stud_id) + "' �������� ��� ��� ������� � ������ ������";
		assert(FALSE);
		throw wss::exception(wss::reason_message(FULL_LOCATION(), msg.c_str()));

	}
	return pos->second; 
}

// ���������� �� �������� �� ������� � ������
long CPropertyDlg::GetIdFromPos(const long& stud_pos)
{
	return GetPosFromId(-stud_pos);
}
