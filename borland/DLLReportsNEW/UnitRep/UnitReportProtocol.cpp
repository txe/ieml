//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportProtocol.h"
#include "UnitFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportProtocol *FormReportProtocol;

extern EDRegOpts opts;
//---------------------------------------------------------------------------
AnsiString GetFamIO(AnsiString SecondName,AnsiString FirstName,AnsiString ThirdName)
{
  AnsiString FamIO="";
  FamIO+=SecondName+" ";
  if (FirstName.Length()>1 && ThirdName.Length()>1)
    FamIO+=FirstName.SubString(1,1)+"."+ThirdName.SubString(1,1)+".";
  return FamIO;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportProtocol::SetIDStudent(int _IDStudent)
{
  idstudent=_IDStudent;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportProtocol::GetIDStudent(void)
{
  return idstudent;
}
//---------------------------------------------------------------------------
__fastcall TFormReportProtocol::TFormReportProtocol(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportProtocol::ToolButtonCloseClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportProtocol::ToolBtnPreviewClick(
      TObject *Sender)
{
  InitReportQuery();
  if (ZMySqlQuery->RecordCount)
  {
    /*TDateField* pF=(TDateField*)ZMySqlQuery->Fields->FieldByNumber(1);
    AnsiString StrDate="";
    if (pF->AsString!="01.01.0001")
    {
      StrDate=pF->AsString;
      QRLabelbdateDay->Caption=GetNumDay(StrDate);
      QRLabelbdateMounthYear->Caption=GetNameMounth(GetNumMounth(StrDate).ToInt());
      QRLabelbdateMounthYear->Caption=QRLabelbdateMounthYear->Caption+"  "+GetYear(StrDate)+"г.";
    }
    else
    {
      QRLabelbdateDay->Caption="";
      QRLabelbdateMounthYear->Caption="";
    } */

    QRLabelShifrSpec->Caption=WCGetTitleForKeyNum(SHIFRSPECS,ZMySqlQuery->Fields->FieldByNumber(3)->AsString.ToInt());
    QRLabelspec->Caption=WCGetTitleForKeyNum(SPECS,ZMySqlQuery->Fields->FieldByNumber(3)->AsString.ToInt());

    QRLabelFIO->Caption=ZMySqlQuery->Fields->FieldByNumber(1)->AsString;

    QRLabelGroup->Caption=WCGetTitleForKeyNum(GROUPS,ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt());

    AnsiString I=ZMySqlQuery->Fields->FieldByNumber(6)->AsString;
    AnsiString O=ZMySqlQuery->Fields->FieldByNumber(7)->AsString;
    AnsiString F=ZMySqlQuery->Fields->FieldByNumber(5)->AsString;
    QRLabelIOFam->Caption=GetFamIO(F,I,O);
    QRLabelIOFam->Caption="";

    QuickRep->Preview();
  }
  else
  {
    AnsiString msg="Сгененрированный отчет пуст.\nПоказать его?";
    if (MessageBox(Handle,msg.c_str(),"Question",MB_YESNO|MB_ICONQUESTION)==ID_YES)
    {
      QuickRep->Preview();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportProtocol::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportProtocol::InitReportQuery(void)
{
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("select concat(secondname,\' \',firstname,\' \',thirdname) as fullname,dogyearid,specid,grpid,secondname,firstname,thirdname from "+opts.DBStudTable+" where deleted=0 and id="+ToStr(AnsiString(idstudent)));
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportProtocol::FormCreate(TObject *Sender)
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
void __fastcall TFormReportProtocol::FormDestroy(TObject *Sender)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------

