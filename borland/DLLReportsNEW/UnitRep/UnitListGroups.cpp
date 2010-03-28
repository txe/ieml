//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitListGroups.h"
#include "UnitFuncs.h"
//#include "mysql.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormListGroup *FormListGroup;

//extern MYSQL *mysql;
//extern EDRegOpts opts;
//extern AnsiString keys[];
//---------------------------------------------------------------------------
__fastcall TFormListGroup::TFormListGroup(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormListGroup::GetWHEREStr(void)
{
  AnsiString WhereStr="";
  for (int i=0;i<CheckListBox->Items->Count;i++)
  {
    if (CheckListBox->Checked[i])
    {
        if (!WhereStr.IsEmpty()) WhereStr+=" or";
        WhereStr+=" grpid="+AnsiString(WCGetNumForKeyTitle(GROUPS,CheckListBox->Items->Strings[i]));
    }
  }
  if (!WhereStr.IsEmpty()) WhereStr=" and ("+WhereStr+")";
  else WhereStr=" and grpid=-1";
  return WhereStr;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormListGroup::GetWHEREStrForVoc(void)
{
  AnsiString WhereStr="";
  for (int i=0;i<CheckListBox->Items->Count;i++)
  {
    if (CheckListBox->Checked[i])
    {
        if (!WhereStr.IsEmpty()) WhereStr+=" or";
        WhereStr+=" num="+AnsiString(WCGetNumForKeyTitle(GROUPS,CheckListBox->Items->Strings[i]));
    }
  }
  if (!WhereStr.IsEmpty()) WhereStr=" and ("+WhereStr+")";
  else WhereStr=" and num=-1";
  return WhereStr;
}
//---------------------------------------------------------------------------
void __fastcall TFormListGroup::SpeedBtnGoClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormListGroup::FormShow(TObject *Sender)
{
  CheckListBox->Clear();
  TStringList* ListGroupTitles=WCGetAllGroupTitles();
  for (int i=0;i<ListGroupTitles->Count;i++)
  {
    CheckListBox->Items->Add(ListGroupTitles->Strings[i]);
  }
  delete ListGroupTitles;
}
//---------------------------------------------------------------------------

