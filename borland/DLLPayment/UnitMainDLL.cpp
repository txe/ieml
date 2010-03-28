//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#pragma hdrstop
//---------------------------------------------------------------------------

#pragma argsused

#include "UnitMainDLL.h"
#include "UnitFormDLL.h"
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------
// Variables


//---------------------------------------------------------------------------
TForm *CreatePage(int n,TForm* OwnerForm)
{
  TFormDLL* newFormDLL=new TFormDLL(OwnerForm);
  return newFormDLL;
}
//---------------------------------------------------------------------------
char *GetTitlePage(void)
{
  char* Title="Оплата за обучение";
  return Title;
}
//---------------------------------------------------------------------------
void LoadData(char* IniFileName, int idstud, TForm* WorkForm)
{
  ((TFormDLL*)WorkForm)->LoadData(IniFileName,idstud);
}
//---------------------------------------------------------------------------

