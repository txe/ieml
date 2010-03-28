//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportStudsCandRedDiplom.h"
#include "UnitFuncs.h"
#include "UnitInfoOcenk.h"
#include <stdio.h>
#include "MacroWord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportStudsCandRedDiplom *FormReportStudsCandRedDiplom;

extern AnsiString arrOcenk[];

extern EDRegOpts opts;
//---------------------------------------------------------------------------
__fastcall TFormReportStudsCandRedDiplom::TFormReportStudsCandRedDiplom(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStudsCandRedDiplom::InitReportQuery(void)
{
  AnsiString IgnorePatternGroup="\%ОТЧИСЛ\%";
//  AnsiString query="select num,title from "+opts.DBVocTable+" where deleted=0 and vkey='grp' and title not like "+ToStr(IgnorePatternGroup)+" order by title";
  AnsiString query="select num,title from "+opts.DBVocTable+" where deleted=0 and vkey='grp'"+pFormList->GetWHEREStrForVoc()+" order by title";
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add(query);
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStudsCandRedDiplom::GetBallsCount(int& countUDOVL,double& percUDOVL,int& countHOR,double& percHOR,int& countOTL,double& percOTL,AnsiString& AvrgBallStr,AnsiString IDStudStr)
{
  ZMySqlQueryAvrgBall->SQL->Clear();
  ZMySqlQueryAvrgBall->SQL->Add("select ball,estimation from "+opts.DBProgress+" where deleted=0 and idstud="+ToStr(IDStudStr)+" and (estimation="+ToStr(AnsiString(OTL))+" or estimation="+ToStr(AnsiString(HOR))+" or estimation="+ToStr(AnsiString(UDOVL))+")");
  ZMySqlQueryAvrgBall->Active=true;
  double SumBall=0.0;
  countUDOVL=countHOR=countOTL=0;
  percUDOVL=percHOR=percOTL=0.0;
  for (int i=0;i<ZMySqlQueryAvrgBall->RecordCount;i++)
  {
    ZMySqlQueryAvrgBall->RecNo=i+1;
    SumBall += ZMySqlQueryAvrgBall->Fields->FieldByNumber(1)->AsString.ToDouble();
    if (ZMySqlQueryAvrgBall->Fields->FieldByNumber(2)->AsString.ToInt()==OTL) countOTL++;
    if (ZMySqlQueryAvrgBall->Fields->FieldByNumber(2)->AsString.ToInt()==HOR) countHOR++;
    if (ZMySqlQueryAvrgBall->Fields->FieldByNumber(2)->AsString.ToInt()==UDOVL) countUDOVL++;
  }
  int NumBalls=countOTL+countHOR+countUDOVL;
  double avrgball=0.0;
  AvrgBallStr="---";
  if (NumBalls>0)
  {
    avrgball = SumBall/double(NumBalls);
    AvrgBallStr = AnsiString(avrgball);
    char str[10];
    sprintf(str,"%4.2lf",AvrgBallStr.ToDouble());
    AvrgBallStr = AnsiString(str);
    percUDOVL=100.0*double(countUDOVL)/double(NumBalls);
    percHOR=100.0*double(countHOR)/double(NumBalls);
    percOTL=100.0*double(countOTL)/double(NumBalls);
  }
}
//---------------------------------------------------------------------------
int __fastcall TFormReportStudsCandRedDiplom::GetCountUsefullStudents(TStringList* listNumbers)
{
  listNumbers->Clear();
  AnsiString IDStudStr;
  int countUDOVL,countHOR,countOTL;
  double percUDOVL,percHOR,percOTL;
  int NumBalls;
  for (int i=0;i<ZMySqlQueryStudsOfGrp->RecordCount;i++)
  {
    ZMySqlQueryStudsOfGrp->RecNo=i+1;
    IDStudStr=ZMySqlQueryStudsOfGrp->Fields->FieldByNumber(1)->AsString;
    ZMySqlQueryCheckStuds->SQL->Clear();
    ZMySqlQueryCheckStuds->SQL->Add("select estimation from "+opts.DBProgress+" where deleted=0 and idstud="+ToStr(IDStudStr)+" and (estimation="+ToStr(AnsiString(OTL))+" or estimation="+ToStr(AnsiString(HOR))+" or estimation="+ToStr(AnsiString(UDOVL))+")");
    ZMySqlQueryCheckStuds->Active=true;
    countUDOVL=countHOR=countOTL=0;
    percUDOVL=percHOR=percOTL=0.0;
    for (int j=0;j<ZMySqlQueryCheckStuds->RecordCount;j++)
    {
      ZMySqlQueryCheckStuds->RecNo=j+1;
      if (ZMySqlQueryCheckStuds->Fields->FieldByNumber(1)->AsString.ToInt()==OTL) countOTL++;
      if (ZMySqlQueryCheckStuds->Fields->FieldByNumber(1)->AsString.ToInt()==HOR) countHOR++;
      if (ZMySqlQueryCheckStuds->Fields->FieldByNumber(1)->AsString.ToInt()==UDOVL) countUDOVL++;
    }
    NumBalls=countOTL+countHOR+countUDOVL;
    if (NumBalls>0)
    {
      percUDOVL=100.0*double(countUDOVL)/double(NumBalls);
      percHOR=100.0*double(countHOR)/double(NumBalls);
      percOTL=100.0*double(countOTL)/double(NumBalls);
      if (countUDOVL<=1 && percOTL>=75.00) listNumbers->Add(AnsiString(i+1));
    }
  }
  return listNumbers->Count;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportStudsCandRedDiplom::InitStudentsForGroup(int idgroup)
{
  ZMySqlQueryStudsOfGrp->SQL->Clear();
  ZMySqlQueryStudsOfGrp->SQL->Add("select id,CONCAT(secondname,\' \',firstname,\' \',thirdname),znum from "+opts.DBStudTable+" where deleted=0 and grpid="+ToStr(AnsiString(idgroup))+" order by secondname");
  ZMySqlQueryStudsOfGrp->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStudsCandRedDiplom::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStudsCandRedDiplom::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStudsCandRedDiplom::CreateWordDocument(void)
{
    int numgroup;

    InitReportQuery();

    if (ZMySqlQuery->RecordCount==0)
        return;

    WordMacros macros;
    macros.BeginMacros();

    macros.InsertLine("ActiveDocument.PageSetup.TopMargin=35");
    macros.InsertLine("ActiveDocument.PageSetup.BottomMargin=35");
    macros.InsertLine("ActiveDocument.PageSetup.LeftMargin=35");
    macros.InsertLine("ActiveDocument.PageSetup.RightMargin=35");

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionFont("Bold=true");
    macros.SelectionFont("Size=16");
    macros.SelectionText("Список кандидатов на получение красного диплома");
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Size=12");
    macros.SelectionFont("Bold=false");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");

    numgroup=-1;
    TStringList* listNumUsefullStuds=new TStringList;
    int CountTables=0;
    int CountRows;
    int CountUsefullStuds;
    AnsiString Str,ZnumStr;
    for (int i=0;i<ZMySqlQuery->RecordCount;i++)
    {
        ZMySqlQuery->RecNo=i+1;
        numgroup=ZMySqlQuery->Fields->FieldByNumber(1)->AsString.ToInt();

        InitStudentsForGroup(numgroup);
        CountUsefullStuds=GetCountUsefullStudents(listNumUsefullStuds);
        if (CountUsefullStuds==0)  continue;

        macros.SelectionTypeParagraph();
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
        macros.SelectionText("Группа ");
        macros.SelectionFont("Bold=true");
        macros.SelectionText(WCGetTitleForKeyNum(GROUPS,numgroup));
        macros.SelectionFont("Bold=false");
        macros.SelectionTypeParagraph();

        CountRows=CountUsefullStuds+1;
        macros.TablesAdd(CountRows,7);
        CountTables++;

        macros.TablesColumns(CountTables, 1, "Width = 20");
        macros.TablesColumns(CountTables, 2, "Width = 180");
        macros.TablesColumns(CountTables, 3, "Width = 60");
        macros.TablesColumns(CountTables, 4, "Width = 70");
        macros.TablesColumns(CountTables, 5, "Width = 70");
        macros.TablesColumns(CountTables, 6, "Width = 70");
        macros.TablesColumns(CountTables, 7, "Width = 50");

        macros.TablesColumns(CountTables, 1, "Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphRight");
        macros.TablesColumns(CountTables, 2, "Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
        macros.TablesColumns(CountTables, 3, "Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
        macros.TablesColumns(CountTables, 4, "Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
        macros.TablesColumns(CountTables, 5, "Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
        macros.TablesColumns(CountTables, 6, "Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
        macros.TablesColumns(CountTables, 7, "Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
        macros.SelectionFont("Bold=true");
        macros.InsertLine("ActiveDocument.Tables.Item(" + IntToStr(CountTables) + ").Range.Font.Size = 10");

        macros.InsertLine("ActiveDocument.Tables.Item(" + IntToStr(CountTables) + ").Rows.Item(1).Range.Font.Bold=true");
//    WordDocument->Tables->Item(CountTables)->Range->Select();

        macros.TablesCell(CountTables, 1, 1, "Range.Text = \"№\"");
        macros.TablesCell(CountTables, 1, 1, "Range.Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
        macros.TablesCell(CountTables, 1, 2, "Range.Text = \"Фамилия, имя, отчество\"");
        macros.TablesCell(CountTables, 1, 2, "Range.Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
        macros.TablesCell(CountTables, 1, 3, "Range.Text = \"№ зач. книжки\"");
        macros.TablesCell(CountTables, 1, 3, "Range.Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
        macros.TablesCell(CountTables, 1, 4, "Range.Text = \"Кол-во \"\"удовл.\"\" (%)\"");
        macros.TablesCell(CountTables, 1, 4, "Range.Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
        macros.TablesCell(CountTables, 1, 5, "Range.Text = \"Кол-во \"\"хор.\"\" (%)\"");
        macros.TablesCell(CountTables, 1, 5, "Range.Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
        macros.TablesCell(CountTables, 1, 6, "Range.Text = \"Кол-во \"\"отл.\"\" (%)\"");
        macros.TablesCell(CountTables, 1, 6, "Range.Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
        macros.TablesCell(CountTables, 1, 7, "Range.Text = \"Средний балл\"");
        macros.TablesCell(CountTables, 1, 7, "Range.Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");

        for (int j=0;j<listNumUsefullStuds->Count;j++)
        {
            ZMySqlQueryStudsOfGrp->RecNo=listNumUsefullStuds->Strings[j].ToInt();

            macros.TablesCell(CountTables, j+2,1, "Range.Text= \"" + AnsiString(j+1) + "\"");
            macros.TablesCell(CountTables, j+2,2, "Range.Text= \"" + ZMySqlQueryStudsOfGrp->Fields->FieldByNumber(2)->AsString + "\"");
            ZnumStr=ZMySqlQueryStudsOfGrp->Fields->FieldByNumber(3)->AsString;
            int CountZero=6-ZnumStr.Length();
            for (int it=0;it<CountZero;it++) ZnumStr="0"+ZnumStr;
            macros.TablesCell(CountTables, j+2,3, "Range.Text= \"" + ZnumStr + "\"");

            int countUDOVL,countHOR,countOTL;
            double percUDOVL,percHOR,percOTL;
            AnsiString AvrgBallStr;
            GetBallsCount(countUDOVL,percUDOVL,countHOR,percHOR,countOTL,percOTL,AvrgBallStr,ZMySqlQueryStudsOfGrp->Fields->FieldByNumber(1)->AsString);

            AnsiString percUDOVLStr=AnsiString(percUDOVL),
                 percHORStr=AnsiString(percHOR),
                 percOTLStr=AnsiString(percOTL);
            char str[10];
            sprintf(str,"%4.2lf",percUDOVLStr.ToDouble());
            percUDOVLStr = AnsiString(str);
            sprintf(str,"%4.2lf",percHORStr.ToDouble());
            percHORStr = AnsiString(str);
            sprintf(str,"%4.2lf",percOTLStr.ToDouble());
            percOTLStr = AnsiString(str);

            Str=AnsiString(countUDOVL)+" ("+percUDOVLStr+"%)";
            macros.TablesCell(CountTables, j+2,4, "Range.Text= \"" + Str + "\"");
            Str=AnsiString(countHOR)+" ("+percHORStr+"%)";
            macros.TablesCell(CountTables, j+2,5, "Range.Text= \"" + Str+ "\"");
            Str=AnsiString(countOTL)+" ("+percOTLStr+"%)";
            macros.TablesCell(CountTables, j+2,6, "Range.Text= \"" + Str+ "\"");
            macros.TablesCell(CountTables, j+2,7, "Range.Text= \"" + AvrgBallStr + "\"");
        }

        macros.TablesCell(CountTables, CountRows+1, 1, "Range.Select");
        macros.InsertLine("Selection.MoveDown Unit := wdLine");
    }
    delete listNumUsefullStuds;

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphRight");
    macros.SelectionFont("Size=8");
    macros.SelectionText("(*)В таблицах не учитываются оценки \"\"зачтено\\"", \"\"незачтено\"\" и \"\"неудовлетворительно\"\".");
    macros.SelectionTypeParagraph();
    macros.SelectionText("(*)Таблицы содержат студентов, имеющих не более одной \"\"удовл\"\" и не менее 75% \"\"отл\"\".");

    macros.EndMacros();
    macros.RunMacros();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStudsCandRedDiplom::ToolBtnPreviewClick(
      TObject *Sender)
{
  try
  {
    pFormList->ShowModal();
    CreateWordDocument();
  }
  catch(...)
  {
    Application->MessageBox("Работа с MS Word не возможна. Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStudsCandRedDiplom::FormCreate(TObject *Sender)
{
  pFormList = new TFormListGroup(this);

  bool NoError;
  try
  {
    ZMySqlDatabase->HostName=opts.DBHost;
    ZMySqlDatabase->User=opts.DBLogin;
    ZMySqlDatabase->Password=opts.DBPasswd;
    ZMySqlDatabase->Database=opts.DBName;
    ZMySqlDatabase->Connected=true;

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
void __fastcall TFormReportStudsCandRedDiplom::FormDestroy(TObject *Sender)
{
  pFormList->Free();
  WCDisconnect();
}
//---------------------------------------------------------------------------
