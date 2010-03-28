//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnitWorkRepDLL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// Подразумевается, что в dll-ке обязательно есть функции с прототипами:
//  1) int GetCountOfReportsInDll(void);
//  2) char* GetTitleOfReportByIndex(int index);
//  3) char* GetHintOfReportByIndex(int index);
//  4) void PreviewReportByIndex(int index, int _IDGroup, int _IDStudent);
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
HINSTANCE dllInstance;

TGetCountOfReportsInDLL  *GetCountOfReportsInDLL;  //function GetCountOfReportsInDLL
TGetTitleOfReportByIndex *GetTitleOfReportByIndex; //function GetTitleOfReportByIndex
TGetHintOfReportByIndex  *GetHintOfReportByIndex;  //function GetHintOfReportByIndex
TPreviewReportByIndex    *PreviewReportByIndex;    //function PreviewReportByIndex

TStringList *AssocItemsRep;
//---------------------------------------------------------------------------
bool WCLoadDLL(AnsiString DLLName)
{
  if (!FileExists(DLLName)) return false; // нет такой dllки
  dllInstance=LoadLibrary(DLLName.c_str());
  return true;
}
int WCGetCountReportsInDLL(void)
{
  GetCountOfReportsInDLL=(TGetCountOfReportsInDLL *)GetProcAddress(dllInstance, "_GetCountOfReportsInDLL");
  int Count=0;
  if (GetCountOfReportsInDLL) Count=GetCountOfReportsInDLL();
  return Count;
}
//---------------------------------------------------------------------------
void WCFreeDLL(void)
{
  FreeLibrary(dllInstance);
}
//---------------------------------------------------------------------------
TStringList* WCGetListTitlesOfReportsInDLL(void)
{
  TStringList* listTitles=new TStringList;
  int CountReportsInDLL=WCGetCountReportsInDLL();
  GetTitleOfReportByIndex = (TGetTitleOfReportByIndex *)GetProcAddress(dllInstance, "_GetTitleOfReportByIndex");
  if (GetTitleOfReportByIndex)
    for (int indexRep=0;indexRep<CountReportsInDLL;indexRep++)
        listTitles->Add(AnsiString(GetTitleOfReportByIndex(indexRep)));
  return listTitles;
}
//---------------------------------------------------------------------------
void WCPreviewReportByIndex(int index,int IDGroup,int IDStudent)
{
  PreviewReportByIndex=(TPreviewReportByIndex *)GetProcAddress(dllInstance, "_PreviewReportByIndex");
  if (PreviewReportByIndex) PreviewReportByIndex(index,IDGroup,IDStudent);
}
//---------------------------------------------------------------------------

