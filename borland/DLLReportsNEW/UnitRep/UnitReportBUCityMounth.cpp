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
    case 1: Str+="������ ";break;
    case 2: Str+="������� ";break;
    case 3: Str+="���� ";break;
    case 4: Str+="������ ";break;
    case 5: Str+="��� ";break;
    case 6: Str+="���� ";break;
    case 7: Str+="���� ";break;
    case 8: Str+="������ ";break;
    case 9: Str+="�������� ";break;
    case 10: Str+="������� ";break;
    case 11: Str+="������ ";break;
    case 12: Str+="������� ";break;
    default: Str+="??? ";break;
  }
  Str+=AnsiString(year);
  Str+="�.";
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
    macros.SelectionText("����� �� ������ ����������������");
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Size=12");
    macros.SelectionFont("Bold=false");
    TDateTime DateRep=pFormSelDate->GetSelectedDate();
    macros.SelectionText("�� "+DateRep.DateString());
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.SelectionText("�������� ������: ");
    macros.SelectionFont("Bold=true");
    macros.SelectionText(GetPeriodStr(DateRep));
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold=false");

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");

  // ������������ �������
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

    macros.TablesCell(CountTables, 1, 1, "Range.Text = \"��/�\"");
    macros.TablesCell(CountTables, 1, 1, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(CountTables, 1, 2, "Range.Text = \"�����������������\"");
    macros.TablesCell(CountTables, 1, 2, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(CountTables, 1, 3, "Range.Text = \"����� (���.)\"");
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
    macros.SelectionText("�����: ����� ����� ��������������� ������ �� ������� ���� ���������������� ���������� "+AnsiString(commsumm)+" ������.");

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
    Application->MessageBox("������ � MS Word �� ��������. �������� ������ ����������.","������.",MB_OK|MB_ICONEXCLAMATION);
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
    AnsiString msg="��������� ��������� � ������� "+opts.DBHost+" ��� � ���� ������ "+opts.DBName+".\n���������� � ���������� ��������������.";
    MessageBox(Handle,msg.c_str(),"������",MB_OK|MB_ICONERROR);
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
