//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportBUCityMounth.h"
#include "UnitFuncs.h"
#include "MacroWord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportBUCityMounth *FormReportBUCityMounth;

extern EDRegOpts opts;

//---------------------------------------------------------------------------
AnsiString __fastcall TFormReportBUCityMounth::GetPeriodStr(TDateTime dt)
{
  AnsiString Str="";
  unsigned short year,mounth,day;
  dt.DecodeDate(&year,&mounth,&day);            
  switch (mounth)
  {
    case 1: Str+="январь ";break;
    case 2: Str+="ферваль ";break;
    case 3: Str+="март ";break;
    case 4: Str+="апрель ";break;
    case 5: Str+="май ";break;
    case 6: Str+="июнь ";break;
    case 7: Str+="июль ";break;
    case 8: Str+="август ";break;
    case 9: Str+="сентябрь ";break;
    case 10: Str+="октябрь ";break;
    case 11: Str+="ноябрь ";break;
    case 12: Str+="декабрь ";break;
    default: Str+="??? ";break;
  }
  Str+=AnsiString(year);
  Str+="г.";
  return Str;
}
//---------------------------------------------------------------------------
__fastcall TFormReportBUCityMounth::TFormReportBUCityMounth(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUCityMounth::InitReportQuery(void)
{
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("select title,num from "+opts.DBVocTable+" where deleted=0 and vkey=\'city\' order by num");
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormReportBUCityMounth::GetSummCity(AnsiString CityIDStr,AnsiString NumMounthStr,AnsiString NumYearStr)
{
  int Summ=0;
  AnsiString Pattern1Str="%4"+CityIDStr+"_/_%";
  AnsiString Pattern2Str="%6"+CityIDStr+"_/_%";
//  MessageBox(Handle,PatternStr.c_str(),"",MB_OK);
  ZMySqlQuerySumm->SQL->Clear();
  ZMySqlQuerySumm->SQL->Add("select pf.moneypay,pf.datepay,voc.title from "+opts.DBPayFacts+" as pf,"+opts.DBVocTable+" as voc,"+opts.DBStudTable+" as st where pf.deleted=0 and st.deleted=0 and voc.deleted=0 and voc.vkey=\'grp\' and (voc.title like "+ToStr(Pattern1Str)+" or voc.title like "+ToStr(Pattern2Str)+") and st.grpid=voc.num and pf.idstud=st.id and YEAR(pf.datepay)="+ToStr(NumYearStr)+" and MONTH(pf.datepay)="+ToStr(NumMounthStr)+" order by pf.datepay");
  ZMySqlQuerySumm->Active=true;
//AnsiString Str=Pattern1Str+"\t"+Pattern2Str+"\n";
  for (int i=0;i<ZMySqlQuerySumm->RecordCount;i++)
  {
    ZMySqlQuerySumm->RecNo=i+1;
//Str+="; "+ZMySqlQuerySumm->Fields->FieldByNumber(3)->AsString;
    Summ+=ZMySqlQuerySumm->Fields->FieldByNumber(1)->AsString.ToInt();
  }
//MessageBox(Handle,Str.c_str(),"",MB_OK);
  return AnsiString(Summ);
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUCityMounth::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUCityMounth::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUCityMounth::CreateWordDocument(void)
{
    InitReportQuery();

    WordMacros macros;
    macros.BeginMacros();

    if (ZMySqlQuery->RecordCount == 0)
        return;

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionFont("Size=20");
    macros.SelectionFont("Bold=true");
    macros.SelectionText("Отчет по оплате представительств");
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Size=12");
    macros.SelectionFont("Bold=false");
    TDateTime DateRep=pFormSelDate->GetSelectedDate();
    macros.SelectionText("на "+DateRep.DateString());
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.SelectionText("Отчетный период: ");
    macros.SelectionFont("Bold=true");
    macros.SelectionText(GetPeriodStr(DateRep));
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold=false");

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");

  // Формирование таблицы
    AnsiString Str,Strtmp;
    int CountTables=0;
    int CountRows=ZMySqlQuery->RecordCount;

    macros.TablesAdd(CountRows+1, 3);
    CountTables++;

    macros.TablesColumns(CountTables, 1, "Width = 50");
    macros.TablesColumns(CountTables, 2, "Width = 300");
    macros.TablesColumns(CountTables, 3, "Width = 80");

    macros.TablesColumns(CountTables, 1, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.TablesColumns(CountTables, 2, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.TablesColumns(CountTables, 3, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphRight");
    macros.InsertLine("ActiveDocument.Tables.Item(" + IntToStr(CountTables) + ").Range.Font.Size = 10");

    macros.InsertLine("ActiveDocument.Tables.Item(" + IntToStr(CountTables) + ").Rows.Item(1).Range.Font.Bold=true");
//  WordDocument->Tables->Item(CountTables)->Range->Select();

    macros.TablesCell(CountTables, 1, 1, "Range.Text = \"№п/п\"");
    macros.TablesCell(CountTables, 1, 1, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(CountTables, 1, 2, "Range.Text = \"Представительство\"");
    macros.TablesCell(CountTables, 1, 2, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(CountTables, 1, 3, "Range.Text = \"Сумма (руб.)\"");
    macros.TablesCell(CountTables, 1, 3, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");

    AnsiString SummStr;
    unsigned short year,month,day;
    DateRep.DecodeDate(&year,&month,&day);
    long commsumm=0;
    for (int i=0;i<ZMySqlQuery->RecordCount;i++)
    {
        ZMySqlQuery->RecNo=i+1;

        macros.TablesCell(CountTables, i+2, 1, "Range.Text = \"" + AnsiString(i+1) + "\"");
        macros.TablesCell(CountTables, i+2, 2, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(1)->AsString);
        SummStr=GetSummCity(ZMySqlQuery->Fields->FieldByNumber(2)->AsString,AnsiString(month),AnsiString(year) + "\"");
        macros.TablesCell(CountTables, i+2, 3, "Range.Text = \"" + SummStr + "\"");
        commsumm+=SummStr.ToInt();
    }

    macros.TablesCell(CountTables, CountRows+1, 1, "Range.Select");
    macros.InsertLine("Selection.MoveDown Unit := wdLine");
    macros.SelectionTypeParagraph();
    macros.SelectionText("Итого: общая сумма зафиксированных выплат со стороны всех представительств составляет "+AnsiString(commsumm)+" рублей.");

    macros.EndMacros();
    macros.RunMacros();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUCityMounth::ToolBtnPreviewClick(
      TObject *Sender)
{
  try
  {
    pFormSelDate->ShowModal();
    CreateWordDocument();
  }
  catch(...)
  {
    Application->MessageBox("Работа с MS Word не возможна. Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUCityMounth::FormCreate(TObject *Sender)
{
  pFormSelDate = new TFormSelectDate(this);

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
void __fastcall TFormReportBUCityMounth::FormDestroy(TObject *Sender)
{
  pFormSelDate->Free();
  WCDisconnect();
}
//---------------------------------------------------------------------------
