//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportDogovor.h"
#include "UnitFuncs.h"
#include "MacroWord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportDogovor *FormReportDogovor;

extern EDRegOpts opts;

AnsiString StrRazd=" ";
AnsiString MounthNames2[12]={ "€нвар€",
                             "феврал€",
                             "марта",
                             "апрел€",
                             "ма€",
                             "июн€",
                             "июл€",
                             "августа",
                             "сент€бр€",
                             "окт€бр€",
                             "но€бр€",
                             "декабр€"};

AnsiString hausends[20]={"одна","две","три","четыре","п€ть","шесть","семь","восемь","дев€ть","дес€ть","одиннадцать",
                         "двенадцать","тринадцать","четырнадцать","п€тнадцать","шестнадцать","семнадцать",
                         "восемнадцать","дев€тнадцать","двадцать"};
AnsiString hausendsdec[10]={"дес€ть","двадцать","тридцать","сорок","п€тьдес€т","шестьдес€т","семьдес€т","восемьдес€т","дев€носто","сто"};
AnsiString handred[10]={"сто","двести","триста","четыреста","п€тьсот","шестьсот","семьсот","восемьсот","дев€тьсот","одна тыс€ча"};
AnsiString eds[20]={"один","два","три","четыре","п€ть","шесть","семь","восемь","дев€ть","дес€ть","одиннадцать",
                    "двенадцать","тринадцать","четырнадцать","п€тнадцать","шестнадцать","семнадцать",
                    "восемнадцать","дев€тнадцать","двадцать"};
//---------------------------------------------------------------------------
AnsiString GetCountMoneyWords(int CountMoney)
{
  AnsiString CountMoneyStr=AnsiString(CountMoney);
  AnsiString Str="";

  int dig,dec,ed,hand;
  if (CountMoneyStr.Length()<=5)
  {
    if (CountMoneyStr.Length()==5)
    {
      dig=CountMoneyStr.SubString(1,2).ToInt();
      if (dig<20 && dig>0)
      {
        Str=Str+hausends[dig-1];
        if (dig==1) Str=Str+" тыс€ча";
        if (dig>=2 && dig<=4) Str=Str+" тыс€чи";
        if (dig>=5 && dig<=20) Str=Str+" тыс€ч";
      }
      if (dig>=20)
      {
        dec=dig/10;
        ed=dig%10;
        if (dec>0) Str=Str+hausendsdec[dec-1];
        if (ed>0)
        {
          Str=Str+" "+hausends[ed-1];
          if (ed==1) Str=Str+" тыс€ча";
          if (ed>=2 && ed<=4) Str=Str+" тыс€чи";
          if (ed>=5 && ed<=9) Str=Str+" тыс€ч";
        }
        else Str=Str+" тыс€ч";
      }

      CountMoneyStr.Delete(1,2);
    }
    if (CountMoneyStr.Length()==4)
    {
      dig=CountMoneyStr.SubString(1,1).ToInt();
      if (dig<20 && dig>0)
      {
        Str=Str+hausends[dig-1];
        if (dig==1) Str=Str+" тыс€ча";
        if (dig>=2 && dig<=4) Str=Str+" тыс€чи";
        if (dig>=5 && dig<=9) Str=Str+" тыс€ч";
      }

      CountMoneyStr.Delete(1,1);
    }
    if (CountMoneyStr.Length()==3)
    {
      hand=CountMoneyStr.SubString(1,1).ToInt();
      if (hand>0) Str=Str+" "+handred[hand-1];

      dig=CountMoneyStr.SubString(2,2).ToInt();
      if (dig<20 && dig>0) {Str=Str+" "+eds[dig-1];}
      if (dig>=20)
      {
        dec=dig/10;
        ed=dig%10;
        if (dec>0) {Str=Str+" "+hausendsdec[dec-1];}
        if (ed>0) {Str=Str+" "+eds[ed-1];}
      }
    }
  }
  if (Str.Length()>0)
  {
    AnsiString UpChar=Str.SubString(1,1);
    Str.Delete(1,1);
    Str=UpChar.UpperCase()+Str;
  }
  return Str;
}
//---------------------------------------------------------------------------
AnsiString GetIOFam(AnsiString SecondName,AnsiString FirstName,AnsiString ThirdName)
{
  AnsiString IOFam="";
  if (FirstName.Length()>1 && ThirdName.Length()>1)
    IOFam+=FirstName.SubString(1,1)+"."+ThirdName.SubString(1,1)+".";
  IOFam+=SecondName;
  return IOFam;
}
//---------------------------------------------------------------------------
AnsiString GetNumDay(AnsiString DateStr)
{
  AnsiString Str=DateStr.SubString(1,2);
  return Str;
}
//---------------------------------------------------------------------------
AnsiString GetNumMounth(AnsiString DateStr)
{
  AnsiString Str=DateStr.SubString(4,2);
  return Str;
}
//---------------------------------------------------------------------------
AnsiString GetYear(AnsiString DateStr)
{
  AnsiString Str=DateStr.SubString(7,4);
  return Str;
}
//---------------------------------------------------------------------------
AnsiString GetNameMounth(int numMounth)
{
  AnsiString Name="";
  if (numMounth>0 && numMounth<13) Name=MounthNames2[numMounth-1];
  return Name;
}
//---------------------------------------------------------------------------
int GetListWords(AnsiString Str,TStringList* listWords)
{
  AnsiString Strtmp=Str;
  int Count=0;
  listWords->Clear();

  while (Strtmp.SubString(1,1)==StrRazd) Strtmp.Delete(1,1);
  while (Strtmp.SubString(Strtmp.Length(),1)==StrRazd) Strtmp.Delete(Strtmp.Length(),1);
  Strtmp+=StrRazd;

  int posspace;
  while (!Strtmp.IsEmpty())
  {
    posspace=Strtmp.Pos(StrRazd);
    listWords->Add(Strtmp.SubString(1,posspace-1));
    Strtmp.Delete(1,posspace);
    Count++;
  }

  return Count;
}
//---------------------------------------------------------------------------
AnsiString GetStrWithWordsAndDefLenWithDel(TStringList* listWords,int CountSimb)
{
  AnsiString Str="";

  while (listWords->Count>0 && Str.Length()<=CountSimb)
  {
    Str+=listWords->Strings[0]+StrRazd;
    listWords->Delete(0);
  }
  while (Str.SubString(Str.Length(),1)==StrRazd) Str.Delete(Str.Length(),1);

  return Str;
}
//---------------------------------------------------------------------------
AnsiString GetEndWordsWithDel(TStringList* listWords)
{
  AnsiString Str="";

  while (listWords->Count>0)
  {
    Str+=listWords->Strings[0]+StrRazd;
    listWords->Delete(0);
  }
  while (Str.SubString(Str.Length(),1)==StrRazd) Str.Delete(Str.Length(),1);

  return Str;
}
//---------------------------------------------------------------------------
__fastcall TFormReportDogovor::TFormReportDogovor(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDogovor::SetIDStudent(int _IDStudent)
{
  idstudent=_IDStudent;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportDogovor::GetIDStudent(void)
{
  return idstudent;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDogovor::ToolButtonCloseClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDogovor::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDogovor::ToolBtnPreviewClick(TObject *Sender)
{
  InitReportQuery();

  QuickRep1->Page->TopMargin = QuickRep1->Page->TopMargin - opts.GetDeltaDogovor(true).ToInt();
  QuickRep2->Page->TopMargin = QuickRep2->Page->TopMargin - opts.GetDeltaDogovor(false).ToInt();

  if (ZMySqlQuery->RecordCount)
  {
    TStringList* listWords=new TStringList;

    QRLabeldognum->Caption=GetNumberOfDogovor();

    TDateField* pF=(TDateField*)ZMySqlQuery->Fields->FieldByNumber(1);
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
    }

    pF=(TDateField*)ZMySqlQuery->Fields->FieldByNumber(10);
    StrDate="";
    if (pF->AsString!="01.01.0001")
    {
      StrDate=pF->AsString;
      QRLabelpassdateDay->Caption=GetNumDay(StrDate);
      QRLabelpassdateMounthYear->Caption=GetNameMounth(GetNumMounth(StrDate).ToInt());
      QRLabelpassdateMounthYear->Caption=QRLabelpassdateMounthYear->Caption+"  "+GetYear(StrDate);
    }
    else
    {
      QRLabelpassdateDay->Caption="";
      QRLabelpassdateMounthYear->Caption="";
    }

    QRLabelFIO1->Caption=ZMySqlQuery->Fields->FieldByNumber(2)->AsString;
    QRLabelFIO2->Caption=ZMySqlQuery->Fields->FieldByNumber(2)->AsString;
    // специальность - направление
    AnsiString spec_direct = WCGetTitleForKeyNum(_IsNewType ? DIRECTS : SPECS,ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt());
    GetListWords(spec_direct,listWords);
    QRLabelspec11->Caption=GetStrWithWordsAndDefLenWithDel(listWords,70);
    QRLabelspec12->Caption=GetEndWordsWithDel(listWords);
     // шифр специальности - направлени€ направление
    QRLabelShifrSpec1->Caption=WCGetTitleForKeyNum(SHIFRSPECS,ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt());
    QRLabelShifrSpec2->Caption=WCGetTitleForKeyNum(SHIFRSPECS,ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt());
    // форма обучени€
    GetListWords(WCGetTitleForKeyNum(EDUFORMS,ZMySqlQuery->Fields->FieldByNumber(7)->AsString.ToInt()),listWords);
    QRLabelform11->Caption=GetStrWithWordsAndDefLenWithDel(listWords,80);
    QRLabelform12->Caption=GetEndWordsWithDel(listWords);

    int CountMoney=GetCountMoney();
    if (CountMoney!=-1)
    {
      QRLabelCountMoney->Caption=AnsiString(CountMoney);
      QRLabelCountMoneyStr->Caption=GetCountMoneyWords(CountMoney);
    }
    else
    {
      QRLabelCountMoney->Caption="";
      QRLabelCountMoneyStr->Caption="";
    }
    // специальность
    GetListWords(spec_direct,listWords);
    QRLabelspec21->Caption=GetStrWithWordsAndDefLenWithDel(listWords,70);
    QRLabelspec22->Caption=GetEndWordsWithDel(listWords);
    // форма обучени€
    GetListWords(WCGetTitleForKeyNum(EDUFORMS,ZMySqlQuery->Fields->FieldByNumber(7)->AsString.ToInt()),listWords);
    QRLabelform21->Caption=GetStrWithWordsAndDefLenWithDel(listWords,80);
    QRLabelform22->Caption=GetEndWordsWithDel(listWords);

    GetListWords(ZMySqlQuery->Fields->FieldByNumber(11)->AsString,listWords);
    QRLabelpassplace1->Caption=GetStrWithWordsAndDefLenWithDel(listWords,55);
    QRLabelpassplace2->Caption=GetEndWordsWithDel(listWords);

    QRLabelpassseries->Caption=ZMySqlQuery->Fields->FieldByNumber(8)->AsString;
    QRLabelpassnumber->Caption=ZMySqlQuery->Fields->FieldByNumber(9)->AsString;
    QRLabeladdr->Caption=ZMySqlQuery->Fields->FieldByNumber(5)->AsString;
    QRLabelliveaddr->Caption=ZMySqlQuery->Fields->FieldByNumber(6)->AsString;
    QRLabelphones->Caption=ZMySqlQuery->Fields->FieldByNumber(12)->AsString;
    AnsiString I=ZMySqlQuery->Fields->FieldByNumber(15)->AsString;
    AnsiString O=ZMySqlQuery->Fields->FieldByNumber(16)->AsString;
    AnsiString F=ZMySqlQuery->Fields->FieldByNumber(14)->AsString;
    QRLabelIOFam->Caption=GetIOFam(F,I,O);

    delete listWords;

    QuickRep1->Preview();
    QuickRep2->Preview();
  }
  else
  {
    AnsiString msg="—гененрированный отчет пуст.\nѕоказать его?";
    if (MessageBox(Handle,msg.c_str(),"Question",MB_YESNO|MB_ICONQUESTION)==ID_YES)
    {
      QuickRep1->Preview();
      QuickRep2->Preview();
    }
  }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormReportDogovor::GetNumberOfDogovor(void)
{
  AnsiString Str="";
  if (ZMySqlQuery->RecordCount!=0)
  {
    ZMySqlQuery->RecNo=1;
    Str+=WCGetTitleForKeyNum(DOGSHIFRS,ZMySqlQuery->Fields->FieldByNumber(17)->AsString.ToInt())+"-";
    Str+=WCGetTitleForKeyNum(DOGYEARS,ZMySqlQuery->Fields->FieldByNumber(3)->AsString.ToInt());
    AnsiString Strtmp;
    Strtmp+=WCGetTitleForKeyNum(DOGFASTS,ZMySqlQuery->Fields->FieldByNumber(18)->AsString.ToInt());
    if (!Strtmp.IsEmpty()) Str+=Strtmp;
    Str+="-";
    Str+=ZMySqlQuery->Fields->FieldByNumber(19)->AsString;
  }
  return Str;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDogovor::InitReportQuery(void)
{
  AnsiString spec_direct_id = _IsNewType ? "directid" : "specid";
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("select bdate,concat(secondname,\' \',firstname,\' \',thirdname) as fullname,dogyearid," + spec_direct_id + ",addr,liveaddr,eduformid,passseries,passnum,passdate,passplace,phones,grpid,secondname,firstname,thirdname,dogshifrid,dogfastid,dognum from "+opts.DBStudTable+" where deleted=0 and id="+ToStr(AnsiString(idstudent)));
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportDogovor::GetCountMoney(void)
{
  int CountMoney;
  try
  {
   ZMySqlQueryOpts->SQL->Clear();
   AnsiString GroupID=ZMySqlQuery->Fields->FieldByNumber(13)->AsString;
   AnsiString DogYearID=ZMySqlQuery->Fields->FieldByNumber(3)->AsString;
   ZMySqlQueryOpts->SQL->Add("select commoncountmoney from "+opts.DBPayOpts+" where deleted=0 and idgroup="+GroupID);
   ZMySqlQueryOpts->Active=true;
   if (ZMySqlQueryOpts->RecordCount>0)
     CountMoney=ZMySqlQueryOpts->Fields->FieldByNumber(1)->AsString.ToInt();
   else CountMoney=-1;
  }
  catch(...)
  {
   CountMoney=-1;
  }
  return CountMoney;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDogovor::FormCreate(TObject *Sender)
{
  QuickRep1->Visible=false;
  QuickRep2->Visible=false;

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
    AnsiString msg="ќшибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nќбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"ќшибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDogovor::FormDestroy(TObject *Sender)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------
void TFormReportDogovor::SetOfset(double& val)
{

}
//---------------------------------------------------------------------------

