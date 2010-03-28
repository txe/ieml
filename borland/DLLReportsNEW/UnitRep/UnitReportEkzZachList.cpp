//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportEkzZachList.h"
#include "UnitFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportEkzZachList *FormReportEkzZachList;

extern EDRegOpts opts;
//---------------------------------------------------------------------------
void __fastcall TFormReportEkzZachList::SetIDStudent(int _IDStudent)
{
  idstudent=_IDStudent;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportEkzZachList::GetIDStudent(void)
{
  return idstudent;
}
//---------------------------------------------------------------------------
__fastcall TFormReportEkzZachList::TFormReportEkzZachList(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportEkzZachList::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;      
}
//---------------------------------------------------------------------------
void __fastcall TFormReportEkzZachList::ToolButtonCloseClick(
      TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportEkzZachList::ToolBtnPreviewClick(
      TObject *Sender)
{
  InitReportQuery();
  if (ZMySqlQuery->RecordCount)
  {
    AnsiString FIOStr=ZMySqlQuery->Fields->FieldByNumber(1)->AsString; //secondname
    FIOStr=FIOStr+" "+ZMySqlQuery->Fields->FieldByNumber(2)->AsString.SubString(1,1)+"."; //firstname
    FIOStr=FIOStr+ZMySqlQuery->Fields->FieldByNumber(3)->AsString.SubString(1,1)+"."; //thirdname
    QRLabelFIO->Caption=FIOStr;
    QRLabelznum->Caption=ZMySqlQuery->Fields->FieldByNumber(4)->AsString;
    int CountZero=6-QRLabelznum->Caption.Length(); // 6 - это кол-во цифр в номере зач. книжки
    AnsiString ZeroStr="";
    for (int i=0;i<CountZero;i++) ZeroStr=ZeroStr+"0";
    QRLabelznum->Caption=ZeroStr+QRLabelznum->Caption;
    QRLabelgroup->Caption=WCGetTitleForKeyNum(GROUPS,ZMySqlQuery->Fields->FieldByNumber(5)->AsString.ToInt());

    QuickRep->Preview();
  }
  else
  {
    AnsiString msg="Сгененрированный отчет пуст.\nПоказать его?";
    if (MessageBox(Handle,msg.c_str(),"Question",MB_YESNO|MB_ICONQUESTION)==ID_YES)
      QuickRep->Preview();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportEkzZachList::InitReportQuery(void)
{
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("select secondname,firstname,thirdname,znum,grpid from "+opts.DBStudTable+" where deleted=0 and id="+ToStr(AnsiString(idstudent)));
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportEkzZachList::FormCreate(TObject *Sender)
{
  QuickRep->Visible=false;

  bool NoError;
  try
  {
    ZMySqlDatabase->HostName = opts.DBHost;
    ZMySqlDatabase->User = opts.DBLogin;
    ZMySqlDatabase->Password = opts.DBPasswd;
    ZMySqlDatabase->Database = opts.DBName;
    ZMySqlDatabase->Connected = true;

    NoError=ZMySqlDatabase->Connected;
  }
  catch(...)
  {
    NoError=ZMySqlDatabase->Connected;
  }

  NoError=NoError && WCConnect();
  if ( !NoError)
  {
    AnsiString msg="Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportEkzZachList::FormDestroy(TObject *Sender)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------

