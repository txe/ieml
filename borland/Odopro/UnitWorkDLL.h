//---------------------------------------------------------------------------

#ifndef UnitWorkDLLH
#define UnitWorkDLLH
//---------------------------------------------------------------------------
typedef TForm (__import *TCreate(int,TForm*));
typedef char *(__import TGetTitle(void));
typedef void (__import TLoadData(char *, int,TForm*));

//---------------------------------------------------------------------------
void SetNewPageFromDLL(AnsiString DLLName);
void DestroyPages(void);
void LoadDataInAllPages(int idstud);
//---------------------------------------------------------------------------
#endif
