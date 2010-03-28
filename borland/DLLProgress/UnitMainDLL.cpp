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
TForm* CreatePage(int n,TForm* OwnerForm)
{
  TFormDLLProgr* newFormDLL=new TFormDLLProgr(OwnerForm);
  return newFormDLL;
}
//---------------------------------------------------------------------------
char* GetTitlePage(void)
{
  char* Title="Успеваемость";
  return Title;
}
//---------------------------------------------------------------------------
void LoadData(char* IniFileName, int idstud, TForm* WorkForm)
{
  ((TFormDLLProgr*)WorkForm)->LoadData(IniFileName,idstud);
}
//---------------------------------------------------------------------------

