//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitOptsOplataForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormOptsOplata *FormOptsOplata;

extern EDRegOpts opts;

TStringList* listIDs;
TStringList* listRealIDs;

AnsiString arrMounth[2]={"Сентябрь","Февраль"}; // don't edit  - to support order of mounths (0 - september;1 - february)

//---------------------------------------------------------------------------
AnsiString GetPeriodStr(AnsiString Date1Str,AnsiString Date2Str)
{
  // Convert date1 yyyy-mm-dd and date2 yyyy-mm-dd to "(September/February) yyyy - (September/February) yyyy"
  AnsiString Res="";
  try
  {
    int y1=Date1Str.SubString(7,4).ToInt(),
        m1=Date1Str.SubString(4,2).ToInt(),
        d1=Date1Str.SubString(1,2).ToInt(),
        y2=Date2Str.SubString(7,4).ToInt(),
        m2=Date2Str.SubString(4,2).ToInt(),
        d2=Date2Str.SubString(1,2).ToInt();
    switch (m1)
    {
      case 2: Res+="Февраль ";break;
      case 9: Res+="Сентябрь ";break;
      default: Res+="Unsupport ";break;
    }
    Res+=AnsiString(y1)+"г. -- ";
    switch (m2)
    {
      case 2: Res+="Февраль ";break;
      case 9: Res+="Сентябрь ";break;
      default: Res+="Unsupport ";break;
    }
    Res+=AnsiString(y2)+"г.";
    return Res;
  }
  catch(...)
  {
    Res="Unknown format of date";
    return Res;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::SetIDGroup(int _id)
{
  IDGroup=_id;
}
//---------------------------------------------------------------------------
int __fastcall TFormOptsOplata::GetIDGroup(void)
{
  return IDGroup;
}
//---------------------------------------------------------------------------
__fastcall TFormOptsOplata::TFormOptsOplata(TComponent* Owner)
        : TForm(Owner)
{
  ResultIDOpts=ERROR_ID; // can't been
  GroupStr="???";
  DatesStr="??? - ???";
  CountMoney="???";
  IDGroup=ERROR_ID;

  Constraints->MinWidth=Width;
  Constraints->MinHeight=Height;

  listIDs=new TStringList;
  listRealIDs=new TStringList;

  StrGridOpts->Cells[0][0]="№п/п";
  StrGridOpts->Cells[1][0]="Группа";
  StrGridOpts->Cells[2][0]="Период оплаты";
  StrGridOpts->Cells[3][0]="Сумма оплаты за год";
  StringGridRealOpts->Cells[0][0]="№п/п";
  StringGridRealOpts->Cells[1][0]="Группа";
  StringGridRealOpts->Cells[2][0]="Период оплаты";
  StringGridRealOpts->Cells[3][0]="Сумма оплаты за год";
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::UpdateDatesInStatusBar(int selrow)
{
  AnsiString Str;
  Str=StrGridOpts->Cells[1][selrow];
  if (Str.IsEmpty()) Str="???";
  StatusBar->Panels->Items[1]->Text=Str;
  Str=StrGridOpts->Cells[2][selrow];
  if (Str.IsEmpty()) Str="??? - ???";
  StatusBar->Panels->Items[4]->Text=Str;
  Str=StrGridOpts->Cells[3][selrow];
  if (Str.IsEmpty()) Str="???";
  StatusBar->Panels->Items[7]->Text=Str;
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::FormResize(TObject *Sender)
{
  int size=StrGridOpts->ClientWidth-20,
      sizeindex=size*10/100,
      sizegroup=size*25/100,
      sizePeriod=size*45/100,
      sizeCommonMoney=size*20/100;
  StrGridOpts->ColWidths[0]=sizeindex;
  StrGridOpts->ColWidths[1]=sizegroup;
  StrGridOpts->ColWidths[2]=sizePeriod;
  StrGridOpts->ColWidths[3]=sizeCommonMoney;
  StringGridRealOpts->ColWidths[0]=sizeindex;
  StringGridRealOpts->ColWidths[1]=sizegroup;
  StringGridRealOpts->ColWidths[2]=sizePeriod;
  StringGridRealOpts->ColWidths[3]=sizeCommonMoney;
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::ClearTable(TStringGrid* pTab)
{
  pTab->RowCount=2;
  for (int j=0;j<pTab->ColCount;j++) pTab->Cells[j][1]="";
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::UpdateTable(TStringGrid* pTab)
{
  ClearTable(pTab);

  int i,j;
  TStringList* listIDStud = new TStringList;
  TStringList* listIDOpts = new TStringList;
  listIDOpts->Clear();
  listIDStud->Clear();

  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("SELECT id FROM "+opts.DBStudTable+" WHERE deleted=0 AND grpid="+ToStr(AnsiString(IDGroup)));
  ZMySqlQuery->Active=true;
  for (i=1;i<=ZMySqlQuery->RecordCount;i++)
  {
    ZMySqlQuery->RecNo=i;
    listIDStud->Add(AnsiString(ZMySqlQuery->Fields->FieldByNumber(1)->AsString.ToInt()));
  }

  AnsiString IDOptsStr;
  for (i=0; i<listIDStud->Count; i++)
  {
    ZMySqlQuery->SQL->Clear();
    ZMySqlQuery->SQL->Add("SELECT idopts FROM "+opts.DBPayFacts+" WHERE deleted=0 AND idstud="+ToStr(listIDStud->Strings[i]));
    ZMySqlQuery->Active=true;

    for (j=1; j<=ZMySqlQuery->RecordCount;j++)
    {
      ZMySqlQuery->RecNo=j;
      IDOptsStr = ZMySqlQuery->Fields->FieldByNumber(1)->AsString;
      if (listIDOpts->IndexOf(IDOptsStr)==-1)
        listIDOpts->Add(IDOptsStr);
    }
  }


  int numCurRow=0;
  listIDs->Clear();
  listIDs->Add("NULL");
  listRealIDs->Clear();
  listRealIDs->Add("NULL");

for (j=0; j<listIDOpts->Count; j++)
{
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("SELECT id,idgroup,datestart,dateend,commoncountmoney FROM "+opts.DBPayOpts+" WHERE deleted=0 AND id="+ToStr(listIDOpts->Strings[j])+" ORDER BY datestart");
  ZMySqlQuery->Active=true;

  for (i=1;i<=ZMySqlQuery->RecordCount;i++)
  {
    ZMySqlQuery->RecNo=i;
    numCurRow++;
    pTab->RowCount=numCurRow+1;

    listIDs->Add(AnsiString(ZMySqlQuery->Fields->FieldByNumber(1)->AsString.ToInt()));
    pTab->Cells[0][numCurRow]=numCurRow;
    pTab->Cells[1][numCurRow]=WCGetTitleForKeyNum(GROUPS,ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt());
    pTab->Cells[2][numCurRow]=GetPeriodStr(ZMySqlQuery->Fields->FieldByNumber(3)->AsString,ZMySqlQuery->Fields->FieldByNumber(4)->AsString);
    pTab->Cells[3][numCurRow]=ZMySqlQuery->Fields->FieldByNumber(5)->AsString;

    pTab->Refresh();
  }
}
  delete listIDOpts;
  delete listIDStud;

  StrGridOptsClick(StrGridOpts);
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::StrGridOptsClick(TObject *Sender)
{
  // здесь перенос данных на ComboBoxes
  int selrow=StrGridOpts->Selection.Top;
  int idselected;
  if (selrow==StrGridOpts->Selection.Bottom && !StrGridOpts->Cells[0][selrow].IsEmpty())
  {
    idselected=listIDs->Strings[selrow].ToInt();
  }
  UpdateDatesInStatusBar(selrow);
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::BtnSelectCloseClick(TObject *Sender)
{
  if (listRealIDs->Count<=1)
  {
     ResultIDOpts=ERROR_ID;
     GroupStr=StringGridRealOpts->Cells[1][1];
     DatesStr=StringGridRealOpts->Cells[2][1];
     CountMoney=StringGridRealOpts->Cells[3][1];
     if (MessageBox(Handle,"Вы не выбрали ни одной категории оплаты. Отчет не будет создан.\nЗакрыть окно категорий оплаты?", "вопрос",MB_YESNO|MB_ICONQUESTION)==ID_YES)
       Close();
  }
  else
  {
     ResultIDOpts=listRealIDs->Strings[1].ToInt();
     GroupStr=StringGridRealOpts->Cells[1][1];
     DatesStr=StringGridRealOpts->Cells[2][1];
     CountMoney=StringGridRealOpts->Cells[3][1];
     Close();
  }
/*
  // Выбор ID записи и закрытие окна
  int selrow=StrGridOpts->Selection.Top;

  if (selrow==StrGridOpts->Selection.Bottom && !StrGridOpts->Cells[0][selrow].IsEmpty())
  {
    ResultIDOpts=listIDs->Strings[selrow].ToInt();
    GroupStr=StatusBar->Panels->Items[1]->Text;
    DatesStr=StatusBar->Panels->Items[4]->Text;
    CountMoney=StatusBar->Panels->Items[7]->Text;

    Close();
  }
  else
  {
    ResultIDOpts=ERROR_ID;
    GroupStr=StatusBar->Panels->Items[1]->Text;
    DatesStr=StatusBar->Panels->Items[4]->Text;
    CountMoney=StatusBar->Panels->Items[7]->Text;

    AnsiString msg="Выбрано более одной записи или Вы выбрали пустую запись,\nкоторая не может быть использована как категория оплата,\nтак как она не содержит никакой информационной нагрузки!\nСначала создайте новую запись!\nЗакрыть окно редактирования категорий оплаты?";
    if (MessageBox(Handle,msg.c_str(),"Сообщение:",MB_YESNO|MB_ICONINFORMATION)==ID_YES) Close();
  }*/
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::StrGridOptsDblClick(TObject *Sender)
{
  BtnSelectCloseClick(StrGridOpts);
}
//---------------------------------------------------------------------------
int __fastcall TFormOptsOplata::GetResultIDOpts(void)
{
  return ResultIDOpts;
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::GetSelectedFieldsOfOpts(AnsiString& _groupstr,AnsiString& _datesstr,AnsiString& _countmoney)
{
  _groupstr=GroupStr;
  _datesstr=DatesStr;
  _countmoney=CountMoney;
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::FormDestroy(TObject *Sender)
{
//  WCDisconnect();
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::FormShow(TObject *Sender)
{
  bool NoError;
  try
  {
    ZMySqlDatabase->HostName = opts.DBHost;
    ZMySqlDatabase->User = opts.DBLogin;
    ZMySqlDatabase->Password = opts.DBPasswd;
    ZMySqlDatabase->Database = opts.DBName;
    ZMySqlDatabase->Connected = true;

    NoError = ZMySqlDatabase->Connected;
  }
  catch(...)
  {
    NoError = ZMySqlDatabase->Connected;
  }

  NoError=NoError;// && WCConnect();
  if ( !NoError)
  {
    AnsiString msg="Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
  
  UpdateTable(StrGridOpts);
  ResultIDOpts=ERROR_ID;
  GroupStr=StatusBar->Panels->Items[1]->Text;
  DatesStr=StatusBar->Panels->Items[4]->Text;
  CountMoney=StatusBar->Panels->Items[7]->Text;
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::BtnAddClick(TObject *Sender)
{
  int selrow=StrGridOpts->Selection.Top;
  int i;

  if (selrow==StrGridOpts->Selection.Bottom && !StrGridOpts->Cells[0][selrow].IsEmpty())
  {
    AnsiString IDStr=listIDs->Strings[selrow];
    bool is_exists=false;
    if (listRealIDs->Count==1)
    {
      is_exists=true;
    }
    else
    {
      for (i=1;i<StringGridRealOpts->RowCount;i++)
        if (StringGridRealOpts->Cells[2][i]==StrGridOpts->Cells[2][selrow])
          is_exists=true;
    }
    if (is_exists)
    {
      if (listRealIDs->IndexOf(IDStr)==-1)
      {
        if (listRealIDs->Count>1)
          StringGridRealOpts->RowCount++;
        listRealIDs->Add(IDStr);
        for (i=0;i<StrGridOpts->ColCount;i++)
          StringGridRealOpts->Cells[i][StringGridRealOpts->RowCount-1]=StrGridOpts->Cells[i][selrow];
      }
    }
    else
    {
      MessageBox(Handle,"Первая категория, которая была добавлена Вами является ключевой. Остальные добавляемые категории должны относится к тому же периоду, что и первая категория.", "Уведомление",MB_OK|MB_ICONINFORMATION);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::GetListOfOptsID(TStringList* list)
{
  list->Clear();
  for (int i=1;i<listRealIDs->Count;i++)
    list->Add(listRealIDs->Strings[i]);
}
//---------------------------------------------------------------------------

