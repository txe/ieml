// RunReport.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include "..\dev\HTMLayoutSDK\include\json-aux.h"

using namespace std;

typedef void  (__cdecl *TPreviewReportByIndex)(int, int, int);
typedef void  (__cdecl *TSetupDB)(const char*, const char*, const char*, const char*);

int main(int argc,char*argv[],char*envp[])
{
	if (argc < 5)
		return 0;
	
  char  lib_path[MAX_PATH];	// путь до библиотеки с отчетом
  int	  report_id;        	// номер отчета
  long	group_id;
  long	stud_id;

  std::string db_host;
  std::string db_port;
  std::string db_name;
  std::string db_user;
  std::string db_pass;

	strcpy(lib_path, argv[1]);
	report_id	= aux::atoi(argv[2]);
	group_id	= aux::atoi(argv[3]);
	stud_id		= aux::atoi(argv[4]);

  db_host = argv[5];
  db_name = argv[6];
  db_user = argv[7];
  db_pass = argv[8];

  /*
  cout << db_host.c_str() << "\n";
  cout << db_name.c_str() << "\n";
  cout << db_user.c_str() << "\n";
  cout << db_pass.c_str() << "\n";
  */
  HINSTANCE	hinstRepLib = LoadLibraryA(lib_path); // библиотека отчетов
  TSetupDB setup_db = (TSetupDB)GetProcAddress(hinstRepLib, "_SetupDB");
  TPreviewReportByIndex call_report = (TPreviewReportByIndex)GetProcAddress(hinstRepLib, "_PreviewReportByIndex");
 
	try
	{    
		if (setup_db && call_report) 
    {
      setup_db(db_host.c_str(), db_name.c_str(), db_user.c_str(), db_pass.c_str());
      call_report(report_id, group_id, stud_id);
    }
		else
			cout << "fail";
	}
	catch(...)
	{
	}
  FreeLibrary(hinstRepLib);

	return 0;
}
