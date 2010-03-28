//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnitWorkDLL.h"
#include "UnitSelected.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// Подразумевается, что в dll-ке обязательно есть функции с прототипами:
//  1) TForm* CreatePage(int n,TForm* OwnerForm);
//  2) char *GetTitlePage(void);
//  3) void LoadData(char* IniFileName, int idstud, TForm* WorkForm);
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

int CountPagesBefore;
HINSTANCE dllInstance;
TList *dllInstances=NULL;
TList *Pages=NULL;
TCreate *CreatePage; //function CreatePage
TGetTitle *GetTitlePage; //function GetTitlePage
TLoadData *LoadData; //function LoadData

//---------------------------------------------------------------------------
void SetNewPageFromDLL(AnsiString DLLName)
{
 if (!FileExists(DLLName)) return;

 TPageControl *myPages=FormSelected->PageControlSelected;

 if (!Pages) CountPagesBefore=myPages->PageCount;

 TForm *Form;
 dllInstance = LoadLibrary(DLLName.c_str());

 if (!Pages) Pages = new TList();
 if (!dllInstances) dllInstances=new TList();
 dllInstances->Add(dllInstance);

 CreatePage = NULL;
 CreatePage = (TCreate *)GetProcAddress(dllInstance, "_CreatePage");
 GetTitlePage = (TGetTitle *)GetProcAddress(dllInstance, "_GetTitlePage");
 if (CreatePage)
 {
  Form = CreatePage(myPages->PageCount,FormSelected);
  Pages->Add(Form);
  Form->ManualDock(myPages, NULL, alClient);
  Form->Visible = true;
//  myPages->ActivePageIndex = myPages->PageCount - 1;
 }
 if (GetTitlePage)
   myPages->Pages[myPages->PageCount-1]->Caption=AnsiString(GetTitlePage());
}
//---------------------------------------------------------------------------
void DestroyPages(void)
{
 TForm *Form;
 int i;
 if (Pages)
 {
  while(Pages->Count > 0)
  {
    i=Pages->Count-1;
    Form = (TForm*)(Pages->Items[i]);
    if (Form) delete Form;
    Pages->Items[i] = 0;
    Pages->Delete(i);
  }
  Pages->Clear();
  delete Pages;
  Pages=NULL;
 }

 CreatePage = NULL;

 if (dllInstances)
 {
   while(dllInstances->Count > 0)
   {
    i=dllInstances->Count-1;
    FreeLibrary((HINSTANCE)dllInstances->Items[i]);
    dllInstances->Items[i]=0;
    dllInstances->Delete(i);
   }
   dllInstances->Clear();
   delete dllInstances;
   dllInstances=NULL;
 }
}
//---------------------------------------------------------------------------
void LoadDataInAllPages(int idstud)
{
 if (dllInstances)
 {
   for (int i=0;i<dllInstances->Count;i++)
   {
    dllInstance=dllInstances->Items[i];
    LoadData = (TLoadData *)GetProcAddress(dllInstance, "_LoadData");
    if (LoadData) LoadData(DefIniFileName,idstud,(TForm*)(Pages->Items[i]));
   }
 }
}
//---------------------------------------------------------------------------

