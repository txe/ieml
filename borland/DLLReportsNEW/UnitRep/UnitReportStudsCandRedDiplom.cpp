//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportStudsCandRedDiplom.h"
#include "UnitFuncs.h"
#include "UnitInfoOcenk.h"
#include <stdio.h>
#include "MacroWord.h"
#include "UnitFuncs.h"
#include "mysql.h"
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
extern MYSQL *mysql;
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

    int CountTables=0;
    int CountRows;
    for (int i=0;i<ZMySqlQuery->RecordCount;i++)
    {
        ZMySqlQuery->RecNo=i+1;
        int numgroup=ZMySqlQuery->Fields->FieldByNumber(1)->AsString.ToInt();

        // подготавливает запрос на список студенов
        //InitStudentsForGroup(numgroup);
        // получает список студентов на красный диплом
        //CountUsefullStuds=GetCountUsefullStudents(listNumUsefullStuds);
        //if (CountUsefullStuds==0)  continue;
        std::vector<red_student> redStudents = GetRedStudents(numgroup);
        if (redStudents.empty())
          continue;
        int CountUsefullStuds = redStudents.size();

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

        // проходим по списку на красный диплом
        for (int j = 0; j < redStudents.size(); ++j)
        {
            red_student& redStudent = redStudents[j];
            // номер позиции
            macros.TablesCell(CountTables, j+2,1, "Range.Text= \"" + AnsiString(j+1) + "\"");
            // фио
            macros.TablesCell(CountTables, j+2,2, "Range.Text= \"" + redStudent.name + "\"");
            // номер зачетки
            macros.TablesCell(CountTables, j+2,3, "Range.Text= \"" + redStudent.znum + "\"");

            // вычисляет баллы для студента
            //int countUDOVL,countHOR,countOTL;
            //double percUDOVL,percHOR,percOTL;
            //AnsiString AvrgBallStr;
            //GetBallsCount(countUDOVL,percUDOVL,countHOR,percHOR,countOTL,percOTL,AvrgBallStr,ZMySqlQueryStudsOfGrp->Fields->FieldByNumber(1)->AsString);

            AnsiString percUDOVLStr = AnsiString(redStudent.perc_ud);
            AnsiString percHORStr   = AnsiString(redStudent.perc_hor);
            AnsiString percOTLStr   = AnsiString(redStudent.perc_otl);
            char str[10];
            sprintf(str,"%4.2lf",percUDOVLStr.ToDouble());
            percUDOVLStr = AnsiString(str);
            sprintf(str,"%4.2lf",percHORStr.ToDouble());
            percHORStr = AnsiString(str);
            sprintf(str,"%4.2lf",percOTLStr.ToDouble());
            percOTLStr = AnsiString(str);

            AnsiString Str=AnsiString(redStudent.count_ud)+" ("+percUDOVLStr+"%)";
            macros.TablesCell(CountTables, j+2,4, "Range.Text= \"" + Str + "\"");
            Str=AnsiString(redStudent.count_hor)+" ("+percHORStr+"%)";
            macros.TablesCell(CountTables, j+2,5, "Range.Text= \"" + Str+ "\"");
            Str=AnsiString(redStudent.count_otl)+" ("+percOTLStr+"%)";
            macros.TablesCell(CountTables, j+2,6, "Range.Text= \"" + Str+ "\"");
            macros.TablesCell(CountTables, j+2,7, "Range.Text= \"" + redStudent.avrBall + "\"");
        }

        macros.TablesCell(CountTables, CountRows+1, 1, "Range.Select");
        macros.InsertLine("Selection.MoveDown Unit := wdLine");
    }
    //delete listNumUsefullStuds;

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
red_students __fastcall  TFormReportStudsCandRedDiplom::GetRedStudents(int idGroup)
{
  ClearRedStudentsQuery();

    MYSQL_RES *result;
    MYSQL_ROW row;

    AnsiString query = "set @grpid = " + IntToStr(idGroup);
    mysql_query(mysql, query.c_str());
    mysql_query(mysql,"CREATE TEMPORARY TABLE temp_stud TYPE = HEAP "
        "SELECT st.id, st.firstname, st.secondname, st.thirdname, st.znum "
        "FROM students as st "
        "WHERE  st.grpid = @grpid AND st.deleted = 0 ");

    mysql_query(mysql,"CREATE TEMPORARY TABLE temp_progress1 TYPE = HEAP "
        "SELECT pr.*, st.firstname, st.secondname, st.thirdname, st.znum "
        "FROM progress as pr, temp_stud  as st "
        "WHERE  pr.idstud = st.id AND pr.deleted = 0 AND pr.estimation < 3 ");

    mysql_query(mysql,"CREATE TEMPORARY TABLE temp_progress2 TYPE = HEAP "
        "SELECT idstud, iddiscip, MAX(numplansemestr) as numplansemestr "
        "FROM temp_progress1 "
        "GROUP BY idstud, iddiscip ");

    mysql_query(mysql,"CREATE TEMPORARY TABLE temp_progress3 TYPE = HEAP "
        "SELECT pr1.* "
        "FROM temp_progress1 as pr1, temp_progress2 as pr2 "
        "WHERE pr1.idstud = pr2.idstud AND pr1.iddiscip = pr2.iddiscip "
        "AND pr1.numplansemestr = pr2.numplansemestr ");

    mysql_query(mysql,"select idstud,secondname,firstname, thirdname, znum, "
        "sum(case  when estimation=0 then 1 else 0 end) as otl, "
        "sum(case  when estimation=1 then 1 else 0 end) as hor, "
        "sum(case  when estimation=2 then 1 else 0 end) as ud, "
        "AVG(ball) as sr "
        "from temp_progress3 "
        "group by idstud "
        "order by secondname ");

    std::vector<red_student> redStudents;

    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
        while (row = mysql_fetch_row(result))
        {
            red_student redStudent;
            redStudent.name = row[1] + AnsiString(" ") + row[2] + AnsiString(" ") + row[3];
            redStudent.znum = row[4];

            int CountZero = 6 - redStudent.znum.Length();
            for (int it = 0;it < CountZero; it++)
              redStudent.znum = "0" + redStudent.znum;

            redStudent.count_otl = AnsiString(row[5]).ToInt();
            redStudent.count_hor = AnsiString(row[6]).ToInt();
            redStudent.count_ud  = AnsiString(row[7]).ToInt();

            int ballCount = redStudent.count_otl + redStudent.count_hor + redStudent.count_ud;
            if (ballCount == 0)
              continue;

            redStudent.perc_otl = 100.0 * double(redStudent.count_otl) / double(ballCount);
            redStudent.perc_hor = 100.0 * double(redStudent.count_hor) / double(ballCount);
            redStudent.perc_ud  = 100.0 * double(redStudent.count_ud) / double(ballCount);

            TReplaceFlags flags;
            flags << rfReplaceAll << rfIgnoreCase;
            redStudent.avrBall = StringReplace(row[8], ".", ",", flags);
            char str[10];
            sprintf(str,"%4.2lf", redStudent.avrBall.ToDouble());
            redStudent.avrBall = AnsiString(str);

            if (redStudent.count_ud <= 1 && redStudent.perc_otl >= 75.00)
              redStudents.push_back(redStudent);
        }
    }
    mysql_free_result(result);

  ClearRedStudentsQuery();
  return redStudents;
}
//---------------------------------------------------------------------------
void __fastcall  TFormReportStudsCandRedDiplom::ClearRedStudentsQuery()
{
    mysql_query(mysql,"drop temporary table if exists temp_stud");
    mysql_query(mysql,"drop temporary table if exists temp_progress1");
    mysql_query(mysql,"drop temporary table if exists temp_progress2");
    mysql_query(mysql,"drop temporary table if exists temp_progress3");
}

