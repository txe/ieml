#include "stdafx.h"
#include "ManagerPeports.h"
#include "SingeltonApp.h"
#include "json-aux-ext.h"
#include "logger.h"
#include "reports/ReportStudyingSpravka.h"
#include "reports/ReportCharacteristic.h"

typedef int   (__cdecl *TGetCountOfReportsInDLL)(void);
typedef char* (__cdecl *TGetTitleOfReportByIndex)(int);
typedef char* (__cdecl *TGetHintOfReportByIndex)(int);
typedef void  (__cdecl *TPreviewReportByIndex)(int, int, int);

ManagerReports::ManagerReports(void)
{
}

ManagerReports::~ManagerReports(void)
{
	UnLoadReports();
}

// ������������� ������ ���������
void ManagerReports::Init(element menu)
{
	assert(menu.is_valid());
	_menu = menu;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	
	CreatePipe(&hRead, &hWrite, &sa, 0);

	UnLoadReports();
	LoadReports();
	CreateMenu();
}

// ��������� ���������� �������
void ManagerReports::LoadReports(void)
{
	std::string path = theApp.GetModuleDir() + "bin\\ReportsDLL.dll";
	if (!(_hinstRepLib = LoadLibraryA(path.c_str())))
	{
		assert(FALSE);
		LOG_ERROR << "�� ������� �� ����� ���������� � �������� (ReportsDLL.dll)";
		return;
	}
	TGetCountOfReportsInDLL		GetCount = (TGetCountOfReportsInDLL)GetProcAddress(_hinstRepLib, "_GetCountOfReportsInDLL");
	TGetTitleOfReportByIndex	GetTitle = (TGetTitleOfReportByIndex)GetProcAddress(_hinstRepLib, "_GetTitleOfReportByIndex");

	if (!GetCount && !GetTitle)
	{
		assert(FALSE);
		throw wss::exception(wss::reason_message(FULL_LOCATION(), "�������� ��������� ����������"));
	}
	
	_reports.push_back(new ReportStudyingSpravka("��������� ������� (MS Word)"));
  _reports.push_back(new ReportCharacteristic("�������������� �������� (MS Word)"));
}

// ��������� ���������� �������
void ManagerReports::UnLoadReports(void)
{
	FreeLibrary(_hinstRepLib); 
	for (size_t i = 0; i < _reports.size(); ++i)
		delete _reports[i];
	_reports.size();
}

// �������� ����
void ManagerReports::CreateMenu(void)
{
	TGetCountOfReportsInDLL		GetCount = (TGetCountOfReportsInDLL)GetProcAddress(_hinstRepLib, "_GetCountOfReportsInDLL");
	TGetTitleOfReportByIndex	GetTitle = (TGetTitleOfReportByIndex)GetProcAddress(_hinstRepLib, "_GetTitleOfReportByIndex");

	if (!GetCount && !GetTitle)
		return;

	// ������ ��������� ����
	string_t buf	= "<li reports>�����<menu class=\"cool\">";
	int		 count	= GetCount();
  // ������ ������
	for (int i = 0; i < count; ++i)
	{
		string_t title	= GetTitle(i);
		string_t id		= aux::itow(i);

    if (title == "�������������� �������� (MS Word)")
      id = "-1";

		buf += string_t() +
			"<li report id=" + id + " title=\"" + title + "\">" + title + "</li>"; 

	}
  // ����� ������
	for (size_t i = 0; i < _reports.size(); ++i)
	{
		string_t title	= _reports[i]->GetName();
    if (title == "�������������� �������� (MS Word)")
      continue;

		buf += string_t() +
			"<li report id=-1 title=\"" + title + "\">" + title + "</li>"; 
	}

	buf += "</menu></li>";
	if (_mbslen(buf))
		_menu.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);

	// ������������ ���������, �������������� ����� ������ � �������
	HTMLayoutAttachEventHandlerEx(_menu, ElementEventProcMenu, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

	_menu.update();
}

// ������������ ����
BOOL CALLBACK ManagerReports::ElementEventProcMenu(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != MENU_ITEM_CLICK)
		return FALSE;

	element el = pr->heTarget;
	if (!el.get_attribute("report"))
		return FALSE;

	ManagerReports* main = static_cast<ManagerReports*>(tag);
	if (-1 == el.get_attribute_int("id"))
		main->CallNewReport(el.get_attribute("title"));
	else
		main->CallOldReport(el.get_attribute("id"));

	return TRUE;
}
// �������� �����
void ManagerReports::CallNewReport(string_t title)
{
	ReportAbstract* report = NULL;
	for (size_t i = 0; i < _reports.size(); ++i)
		if (_reports[i]->GetName() == title)
			report = _reports[i];
	if (!report)
		return;
	try
  {
    report->Run(theApp.GetCurrentGroupID(), theApp.GetCurrentStudentID());
  }
  catch (...)
  {
    theApp.ExceptionManage();
    MessageBox(::GetActiveWindow(), L"��������� ������� � �������� �������� ������", L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
  }
}
// �������� �����
void ManagerReports::CallOldReport(string_t id)
{
	std::string com_line = theApp.GetModuleDir() + "bin\\RunReport.exe .\\ReportsDLL.dll " + id + 
		" " + aux::itow(theApp.GetCurrentGroupID()) + " " + aux::itow(theApp.GetCurrentStudentID());
	
	HANDLE hFile;

	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;    
	sa.lpSecurityDescriptor = NULL;

	hFile = CreateFile(
		L"stdout.txt",
		GENERIC_WRITE | GENERIC_READ, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		&sa,
		OPEN_ALWAYS,
		0,
		NULL);

	SetFilePointer(hFile, 0, NULL, FILE_END);

	STARTUPINFOA si = {sizeof(STARTUPINFOA)};

	si.hStdOutput = hFile;
	si.dwFlags    = STARTF_USESTDHANDLES;

	PROCESS_INFORMATION pi;

	CreateProcessA(NULL , (LPSTR)com_line.c_str(), NULL, NULL, TRUE ,CREATE_NO_WINDOW, 0, 0, &si, &pi);

	CloseHandle(hFile);  // <--
}