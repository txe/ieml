#ifndef UNITMAINQREPDLL_H
#define UNITMAINQREPDLL_H
//---------------------------------------------------------------------------
#ifdef __DLL__ 
# define DLL_EI __declspec(dllexport) 
#else 
# define DLL_EI __declspec(dllimport) 
#endif 

//---------------------------------------------------------------------------
#include "UnitConfigReports.h"

//---------------------------------------------------------------------------
extern "C" int   DLL_EI GetCountOfReportsInDLL(void);
extern "C" char* DLL_EI GetTitleOfReportByIndex(int index);
extern "C" char* DLL_EI GetHintOfReportByIndex(int index);
extern "C" void  DLL_EI PreviewReportByIndex(int index, int _IDGroup, int _IDStudent);
//---------------------------------------------------------------------------
#endif

 