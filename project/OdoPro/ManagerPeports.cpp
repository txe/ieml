#include "stdafx.h"
#include "ManagerPeports.h"
#include "SingeltonApp.h"
#include "json-aux-ext.h"
#include "logger.h"
#include "reports/Report_StudyingSpravka.h"
#include "reports/Report_PeriodSpravka.h"
#include "reports/Report_Characteristic.h"
#include "reports/Report_Diplom.h"
#include "reports/Report_Dogovor2014.h"
#include "reports/Report_Soglasovanie.h"
#include "reports/Report_Diplom2015.h"
#include "reports/Report_Module.h"
#include "reports/Report_ReleaseDoc.h"
#include "reports/Report_Pril4.h"

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

// инициализация работы менеджера
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

// загружает библиотеки отчетов
void ManagerReports::LoadReports(void)
{
	std::string path = theApp.GetModuleDir() + "bin\\ReportsDLL.dll";
	if (!(_hinstRepLib = LoadLibraryA(path.c_str())))
	{
		assert(FALSE);
		LOG_ERROR << "не найдено ни одной библиотеки с отчетами (ReportsDLL.dll)";
		return;
	}
	TGetCountOfReportsInDLL		GetCount = (TGetCountOfReportsInDLL)GetProcAddress(_hinstRepLib, "_GetCountOfReportsInDLL");
	TGetTitleOfReportByIndex	GetTitle = (TGetTitleOfReportByIndex)GetProcAddress(_hinstRepLib, "_GetTitleOfReportByIndex");

	if (!GetCount && !GetTitle)
	{
		assert(FALSE);
		throw wss::exception(wss::reason_message(FULL_LOCATION(), "неверная структура библиотеки"));
	}
	
  _reports.push_back(new ReportStudyingSpravka("Справка об обучении (MS Word)"));
  _reports.push_back(new ReportPeriodSpravka("Справка об периоде обучении (MS Word)"));
  _reports.push_back(new ReportCharacteristic("Характеристика студента (MS Word)"));
  _reports.push_back(new ReportDiplom("Приложение к диплому 2014 (MS Word)"));
  _reports.push_back(new ReportDogovor("Договор 2014 (MS Word)", false));
  _reports.push_back(new ReportDogovor("Договор 2014 Доп. соглашение (MS Word)", true));
  _reports.push_back(new ReportSoglasovanie("Лист согласования (MS Word)"));
  _reports.push_back(new ReportModule("Модуль (MS Excel)"));
  _reports.push_back(new ReportDiplom2015("Диплом 2015 (MS Word)"));
  _reports.push_back(new ReportReleaseDoc("Докладная на выпуск (MS Word)"));
  _reports.push_back(new ReportPril4("Приложение 4 к акту 1 (MS Word)"));
}

// выгружает библиотеки отчетов
void ManagerReports::UnLoadReports(void)
{
	FreeLibrary(_hinstRepLib); 
	for (size_t i = 0; i < _reports.size(); ++i)
		delete _reports[i];
	_reports.clear();
}

// создание меню
void ManagerReports::CreateMenu(void)
{
	TGetCountOfReportsInDLL		GetCount = (TGetCountOfReportsInDLL)GetProcAddress(_hinstRepLib, "_GetCountOfReportsInDLL");
	TGetTitleOfReportByIndex	GetTitle = (TGetTitleOfReportByIndex)GetProcAddress(_hinstRepLib, "_GetTitleOfReportByIndex");

	if (!GetCount && !GetTitle)
		return;

	// делаем структуру меню
	string_t buf	= "<li reports>Отчет<menu class=\"cool\">";
	int		 count	= GetCount();
  // старые отчеты
	for (int i = 0; i < count; ++i)
	{
		string_t title	= GetTitle(i);
		string_t id		= aux::itow(i);

    if (title == "Ведомость на группу (Quick)"
      || title == "Договор new (Quick)"
      || title == "Договор old (Quick)"
      || title == "Экзаменационный (зачетный) лист (Quick)"
      || title == "Протокол заседания экз. комиссии - гос. экзамены (Quick)"
      || title == "Справка (Quick)"
      || title == "Справочная карточка (Quick)"
      || title == "Документ о допуске к вступительным испытаниям (MS Word)"
      || title == "Академическая справка (MS Word)"
      || title == "Приложение к диплому (MS Word)"
      || title == "Статистика (old)")
        continue;

    if (title == "Характеристика студента (MS Word)")
      id = "-1";

		buf += string_t() +
			"<li report id=" + id + " title=\"" + title + "\">" + title + "</li>"; 

	}
  // новые отчеты
	for (size_t i = 0; i < _reports.size(); ++i)
	{
		string_t title	= _reports[i]->GetName();
    // вставлено в коде выше, чтобы позиция отчета оставалось прежней
    if (title == "Характеристика студента (MS Word)")
      continue;

		buf += string_t() +
			"<li report id=-1 title=\"" + title + "\">" + title + "</li>"; 
	}

	buf += "</menu></li>";
	if (_mbslen(buf))
		_menu.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);

	// присоединяем процедуру, отвлавливающую выбор строки с группой
	HTMLayoutAttachEventHandlerEx(_menu, ElementEventProcMenu, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

	_menu.update();
}

// обрабатывает меню
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
// вызывает отчет
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
    MessageBox(::GetActiveWindow(), L"Произошло падение в процессе создания отчета", L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
  }
}
// вызывает отчет
void ManagerReports::CallOldReport(string_t id)
{
	std::string com_line = theApp.GetModuleDir() + "bin\\RunReport.exe .\\ReportsDLL.dll " + id + 
		" " + aux::itow(theApp.GetCurrentGroupID()) + " " + aux::itow(theApp.GetCurrentStudentID());

  com_line += " \"" + theApp.GetCon().GetParam(mybase::PR_HOST) + "\"";
  com_line += " \"" + theApp.GetCon().GetParam(mybase::PR_BD) + "\"";
	com_line += " \"" + theApp.GetCon().GetParam(mybase::PR_LOGIN) + "\"";
  com_line += " \"" + theApp.GetCon().GetParam(mybase::PR_PASS) + "\"";

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