//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportSpravCard.h"
#include "UnitFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportSpravCard *FormReportSpravCard;

extern EDRegOpts opts;
//---------------------------------------------------------------------------
__fastcall TFormReportSpravCard::TFormReportSpravCard(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSpravCard::SetIDStudent(int _IDStudent)
{
  idstudent=_IDStudent;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportSpravCard::GetIDStudent(void)
{
  return idstudent;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSpravCard::FormClose(TObject *Sender, TCloseAction &Action)
{
  Action=caFree;      
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSpravCard::ToolButtonCloseClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSpravCard::ToolBtnPreviewClick(
      TObject *Sender)
{
  InitReportQuery();
  if (ZMySqlQuery->RecordCount)
  {
    TDateField* pF=(TDateField*)ZMySqlQuery->Fields->FieldByNumber(1);
    AnsiString StrDate="";
    if (pF->AsString!="01.01.0001") StrDate=pF->AsString;
    QRLabelbdate->Caption=StrDate;
    QRLabelsecondname->Caption=ZMySqlQuery->Fields->FieldByNumber(2)->AsString;
    QRLabelfirstname->Caption=ZMySqlQuery->Fields->FieldByNumber(3)->AsString;
    QRLabelthirdname->Caption=ZMySqlQuery->Fields->FieldByNumber(4)->AsString;
    QRLabeldogyear->Caption=WCGetTitleForKeyNum(DOGYEARS,ZMySqlQuery->Fields->FieldByNumber(5)->AsString.ToInt());
    QRLabelspec->Caption=WCGetTitleForKeyNum(SPECS,ZMySqlQuery->Fields->FieldByNumber(6)->AsString.ToInt());
    AnsiString AddrStr=ZMySqlQuery->Fields->FieldByNumber(7)->AsString;
    QRLabeladdr1->Caption=AddrStr.SubString(1,70);
    AddrStr.Delete(1,70);
    QRLabeladdr2->Caption=AddrStr.SubString(1,95);
    AddrStr.Delete(1,95);
    QRLabeladdr3->Caption=AddrStr;

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
void __fastcall TFormReportSpravCard::InitReportQuery(void)
{
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("select bdate,secondname,firstname,thirdname,dogyearid,specid,addr from "+opts.DBStudTable+" where deleted=0 and id="+ToStr(AnsiString(idstudent)));
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSpravCard::FormCreate(TObject *Sender)
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
void __fastcall TFormReportSpravCard::FormDestroy(TObject *Sender)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------

