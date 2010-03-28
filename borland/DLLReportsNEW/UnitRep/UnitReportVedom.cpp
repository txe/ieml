//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportVedom.h"
#include "UnitFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportVedom *FormReportVedom;

extern EDRegOpts opts;
//---------------------------------------------------------------------------
void __fastcall TFormReportVedom::SetIDGroup(int _IDGroup)
{
  idgroup=_IDGroup;      
}
//---------------------------------------------------------------------------
int __fastcall TFormReportVedom::GetIDGroup(void)
{
  return idgroup;      
}
//---------------------------------------------------------------------------
__fastcall TFormReportVedom::TFormReportVedom(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedom::Connect(void)
{
  try
  {
    ZMySqlDatabase->HostName = opts.DBHost;
    ZMySqlDatabase->User = opts.DBLogin;
    ZMySqlDatabase->Password = opts.DBPasswd;
    ZMySqlDatabase->Database = opts.DBName;
    ZMySqlDatabase->Connected = true;
  }
  catch(...)
  {
    return;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedom::InitReportQuery(void)
{
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("select secondname,firstname,thirdname,znum,specid from "+opts.DBStudTable+" where deleted=0 and grpid="+ToStr(idgroup)+" order by specid,secondname");
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedom::InitReportQueryForSpec(int specid)
{
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("select secondname,firstname,thirdname,znum,specid from "+opts.DBStudTable+" where deleted=0 and grpid="+ToStr(idgroup)+"and specid="+ToStr(specid)+" order by secondname");
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedom::FormClose(TObject *Sender,TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedom::ToolBtnCloseClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedom::ToolBtnPreviewClick(TObject *Sender)
{
  InitReportQuery();
  QRLabelGroup->Caption=WCGetTitleForKeyNum(GROUPS,idgroup);

  int CountSpec=0;
  TStringList* list=new TStringList;
  AnsiString Str;
  int i;
  for (i=0;i<ZMySqlQuery->RecordCount;i++)
  {
    ZMySqlQuery->RecNo=i+1;
    Str=ZMySqlQuery->Fields->FieldByNumber(5)->AsString;
    if (list->IndexOf(Str)==-1) list->Add(Str);
  }
  CountSpec=list->Count;

  int specid;
  for (i=0;i<CountSpec;i++)
  {
    specid=list->Strings[i].ToInt();
    InitReportQueryForSpec(specid);
    QRLabelSpec->Caption=WCGetTitleForKeyNum(SPECS,specid);
    if (ZMySqlQuery->RecordCount) QuickRep->Preview();
    else
    {
      AnsiString msg="Сгененрированный отчет пуст.\nПоказать его?";
      if (MessageBox(Handle,msg.c_str(),"Question",MB_YESNO|MB_ICONQUESTION)==ID_YES)
        QuickRep->Preview();
    }
  }

  delete list;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedom::FormCreate(TObject *Sender)
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
void __fastcall TFormReportVedom::FormDestroy(TObject *Sender)
{
  WCDisconnect();      
}
//---------------------------------------------------------------------------

