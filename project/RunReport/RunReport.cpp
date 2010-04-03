// RunReport.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include "..\\ODOPro\logger.h"
#include <json-aux.h>

using namespace std;

typedef void  (__cdecl *TPreviewReportByIndex)(int, int, int);

char		command[MAX_PATH];	// путь до библиотеки с отчетом
int			report_id;	// номер отчета
long		group_id;
long		stud_id;
HINSTANCE	hinstRepLib; // библиотека отчетов



bool load()
{
	return (hinstRepLib = LoadLibraryA(command)) != NULL;
}

void unload()
{
	FreeLibrary(hinstRepLib);
}

void view()
{
	TPreviewReportByIndex call = (TPreviewReportByIndex)GetProcAddress(hinstRepLib, "_PreviewReportByIndex");
	try
	{
		if (call) 
			call(report_id, group_id, stud_id);
		else
			cout << "fail";
	}
	catch(...)
	{
		//theApp.ExceptionManage();
	}
}

int run_from_pipe(void)
{
	// делаем цикл запроса
	
	while(1)
	{
		cout << "enter path report or 'exit':";
		cin >> command;
		// выход
		if (aux::streq(command, "exit"))
			break;
		// загрузка отчета
		if (!load())
		{
			cout << "fail";
			continue;
		}
		cout << "enter report id:";
		cin >> report_id;
		cout << "enter group id:";
		cin >> group_id;
		cout << "enter student id:";
		cin >> stud_id;

		view();
		// выгружаем библиотеку
		unload();
	}
	return -1;
}

void run_from_command_line(int argc, char* argv[])
{
	
}

int main(int argc,char*argv[],char*envp[])
{
	LOG_PREFIX("log-runreport");

#if defined(_DEBUG)
	LOG_LEVEL(logger::Debug); // уровень лога в рантайме (например, берём из командной строки)
#else
	LOG_LEVEL(logger::Warning);
#endif

	LOG_DEBUG << "argc = " << argc;

	if (argc < 5)
		return 0;
	
	strcpy(command, argv[1]);
	report_id	= aux::atoi(argv[2]);
	LOG_DEBUG << "report = " << report_id;
	group_id	= aux::atoi(argv[3]);
	LOG_DEBUG << "group = " << group_id;
	stud_id		= aux::atoi(argv[4]);
	LOG_DEBUG << "stud = " << stud_id;
	LOG_DEBUG << "good = " << argc;
	load();
	view();
	unload();
	
	cin >> group_id;
	return 0;
}
