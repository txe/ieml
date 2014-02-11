#pragma once

#include <htmlayout.h>
#include "string_t.h"
#include "reports/Report_Abstract.h"

using namespace htmlayout::dom;

class ManagerReports
{
public:
	ManagerReports(void);
	~ManagerReports(void);

private:
	element		_menu;
	HINSTANCE	_hinstRepLib;
	HANDLE      hRead, hWrite;
	std::vector<ReportAbstract*> _reports;

public:
	// инициализация работы менеджера
	void Init(element menu);
private:
	// загружает библиотеки отчетов
	void LoadReports(void);
	// выгружает библиотеки отчетов
	void UnLoadReports(void);
	// создание меню
	void CreateMenu(void);
	// обрабатывает меню
	static BOOL CALLBACK ElementEventProcMenu(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// вызывает отчет
	void CallNewReport(string_t title);
	void CallOldReport(string_t id);
};
