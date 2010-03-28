//---------------------------------------------------------------------------
#ifndef UnitWorkRepDLLH
#define UnitWorkRepDLLH
//---------------------------------------------------------------------------
typedef int   (__import TGetCountOfReportsInDLL(void));
typedef char* (__import TGetTitleOfReportByIndex(int));
typedef char* (__import TGetHintOfReportByIndex(int));
typedef void  (__import TPreviewReportByIndex(int, int, int));
//---------------------------------------------------------------------------
bool WCLoadDLL(AnsiString);
int WCGetCountReportsInDLL(void);
void WCFreeDLL(void);
TStringList* WCGetListTitlesOfReportsInDLL(void);
void WCPreviewReportByIndex(int,int,int);
//---------------------------------------------------------------------------
#endif
