//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportAvrgBallInGroup.h"
#include "UnitFuncs.h"
#include "UnitInfoOcenk.h"
#include <stdio.h>
#include "MacroWord.h"
#include <Math.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)

#include "UnitFuncs.h"
#include "mysql.h"

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma link "ZStoredProcedure"
#pragma resource "*.dfm"
TFormReportAvrgBallInGroup *FormReportAvrgBallInGroup;

extern AnsiString arrOcenk[];

extern EDRegOpts opts;
extern MYSQL *mysql;
//---------------------------------------------------------------------------
__fastcall TFormReportAvrgBallInGroup::TFormReportAvrgBallInGroup(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportAvrgBallInGroup::SetIDGroup(int _IDGroup)
{
  idgroup=_IDGroup;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportAvrgBallInGroup::GetIDGroup(void)
{
  return idgroup;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportAvrgBallInGroup::GetBallsCount(int& countUDOVL,double& percUDOVL,int& countHOR,double& percHOR,int& countOTL,double& percOTL,AnsiString& AvrgBallStr,AnsiString IDStudStr)
{
/*
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
  */
}
//---------------------------------------------------------------------------
void __fastcall TFormReportAvrgBallInGroup::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportAvrgBallInGroup::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportAvrgBallInGroup::CreateWordDocument(void)
{
    WordMacros macros;
    macros.BeginMacros();

    macros.InsertLine("ActiveDocument.PageSetup.TopMargin=35");
    macros.InsertLine("ActiveDocument.PageSetup.BottomMargin=35");
    macros.InsertLine("ActiveDocument.PageSetup.LeftMargin=35");
    macros.InsertLine("ActiveDocument.PageSetup.RightMargin=35");

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionFont("Bold=true");
    macros.SelectionFont("Size=16");
    macros.SelectionText("Общая успеваемость студентов группы "+AnsiString(WCGetTitleForKeyNum(GROUPS,idgroup)));
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Size=12");
    macros.SelectionFont("Bold=false");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");

    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionText("Cпециальность \"\"");
    macros.SelectionFont("Bold=true");

    macros.SelectionText(GetSpec());
    macros.SelectionText("\"\"");
    macros.SelectionFont("Bold=false");
    macros.SelectionTypeParagraph();

                             // создаем таблицу ...

    macros.TablesAdd(avg_progress_data.size() + 1, 7);

    macros.TablesColumns(1, 1, "Width = 20");
    macros.TablesColumns(1, 2, "Width = 180");
    macros.TablesColumns(1, 3, "Width = 60");
    macros.TablesColumns(1, 4, "Width = 70");
    macros.TablesColumns(1, 5, "Width = 70");
    macros.TablesColumns(1, 6, "Width = 70");
    macros.TablesColumns(1, 7, "Width = 50");

    macros.TablesColumns(1, 1, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphRight");
    macros.TablesColumns(1, 2, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.TablesColumns(1, 3, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesColumns(1, 4, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesColumns(1, 5, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesColumns(1, 6, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesColumns(1, 7, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionFont("Bold=true");
    macros.InsertLine("ActiveDocument.Tables.Item(1).Range.Font.Size = 10");

    macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(1).Range.Font.Bold=true");
//  WordDocument->Tables->Item(1)->Range->Select();

    macros.TablesCell(1, 1, 1, "Range.Text = \"№\"");
    macros.TablesCell(1, 1, 1, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(1, 1, 2, "Range.Text = \"Фамилия, имя, отчество\"");
    macros.TablesCell(1, 1, 2, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(1, 1, 3, "Range.Text = \"№ зач. книжки\"");
    macros.TablesCell(1, 1, 3, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(1, 1, 4, "Range.Text = \"Кол-во \"\"удовл.\"\" (%)");
    macros.TablesCell(1, 1, 4, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(1, 1, 5, "Range.Text = \"Кол-во \"\"хор.\"\" (%)\"");
    macros.TablesCell(1, 1, 5, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(1, 1, 6, "Range.Text = \"Кол-во \"\"отл.\"\" (%)\"");
    macros.TablesCell(1, 1, 6, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(1, 1, 7, "Range.Text = \"Средний балл\"");
    macros.TablesCell(1, 1, 7, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");


//    macros.TablesCell(1, i+2-CountPrevPrevRows, 1, "Range.Text= \"" + AnsiString(i+1-CountPrevPrevRows) + "\"");
//    macros.TablesCell(1, i+2-CountPrevPrevRows, 2, "Range.Text= \"" + ZMySqlQuery->Fields->FieldByNumber(2)->AsString + "\"");

/*    ZnumStr=ZMySqlQuery->Fields->FieldByNumber(4)->AsString;
    int CountZero=6-ZnumStr.Length();
    for (int it=0;it<CountZero;it++)
        ZnumStr="0"+ZnumStr;
  */
    AnsiString text;
    for (int i = 0; i < avg_progress_data.size(); i++)
    {
        int count = avg_progress_data[i]->count_otl + avg_progress_data[i]->count_hor + \
            avg_progress_data[i]->count_ud;

        macros.TablesCell(1, i+2, 1, "Range.Text= \"" + AnsiString(i+1) + "\"");
        macros.TablesCell(1, i+2, 2, "Range.Text= \"" + avg_progress_data[i]->name + "\"");
        macros.TablesCell(1, i+2, 3, "Range.Text= \"" + avg_progress_data[i]->znum + "\"");
        text = AnsiString(avg_progress_data[i]->count_ud) + " (" +
            FloatToStrF(SimpleRoundTo(100.0*(double)avg_progress_data[i]->count_ud/(double)count, -2), ffNumber, 15, 2)
            + "%)";
        macros.TablesCell(1, i+2, 4, "Range.Text= \"" + text + "\"");
        text = AnsiString(avg_progress_data[i]->count_hor) + " (" +
            FloatToStrF(SimpleRoundTo(100.0*(double)avg_progress_data[i]->count_hor/(double)count, -2), ffNumber, 15, 2)
            + "%)";
        macros.TablesCell(1, i+2, 5, "Range.Text= \"" + text + "\"");
        text = AnsiString(avg_progress_data[i]->count_otl) + " (" +
            FloatToStrF(SimpleRoundTo(100.0*(double)avg_progress_data[i]->count_otl/(double)count, -2), ffNumber, 15, 2)
            + "%)";
        macros.TablesCell(1, i+2, 6, "Range.Text= \"" + text + "\"");
        text = FloatToStrF(avg_progress_data[i]->sred.ToDouble(), ffNumber, 15, 2);
        macros.TablesCell(1, i+2, 7, "Range.Text= \"" +  text + "\"");
    }

    macros.TablesCell(1, avg_progress_data.size()+1, 1, "Range.Select");
    macros.InsertLine("Selection.MoveDown Unit := wdLine");

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphRight");
    macros.SelectionFont("Size=8");
    macros.SelectionText("(*)В таблицах не учитываются оценки \"\"зачтено\"\", \"\"незачтено\"\" и \"\"неудовлетворительно\"\".");

   /*
    int countUDOVL,countHOR,countOTL;
    double percUDOVL,percHOR,percOTL;
    AnsiString AvrgBallStr;
    GetBallsCount(countUDOVL,percUDOVL,countHOR,percHOR,countOTL,percOTL,AvrgBallStr,ZMySqlQuery->Fields->FieldByNumber(1)->AsString);

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
        macros.TablesCell(1, i+2-CountPrevPrevRows,4, "Range.Text= \"" + Str + "\"");
        Str=AnsiString(countHOR)+" ("+percHORStr+"%)";
        macros.TablesCell(1, i+2-CountPrevPrevRows,5, "Range.Text= \"" + Str + "\"");
        Str=AnsiString(countOTL)+" ("+percOTLStr+"%)";
        macros.TablesCell(1, i+2-CountPrevPrevRows,6, "Range.Text= \"" + Str + "\"");
        macros.TablesCell(1, i+2-CountPrevPrevRows,7, "Range.Text=\"" + AvrgBallStr + "\"");

        if (i+1 == CountPrevRows)
        {
            macros.TablesCell(1, CountRows+1, 1, "Range.Select");
            macros.InsertLine("Selection.MoveDown Unit := wdLine");
        }
    }
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphRight");
    macros.SelectionFont("Size=8");
    macros.SelectionText("(*)В таблицах не учитываются оценки \"\"зачтено\"\", \"\"незачтено\"\" и \"\"неудовлетворительно\"\".");
                        */
    macros.EndMacros();
    macros.RunMacros();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportAvrgBallInGroup::ToolBtnPreviewClick(
      TObject *Sender)
{
  try
  {
    OpenAvgQuery();
    CreateWordDocument();
    CloseAvgQuery();
  }
  catch(...)
  {
    Application->MessageBox("Работа с MS Word не возможна. Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportAvrgBallInGroup::FormCreate(TObject *Sender)
{
  if (!WCConnect())
  {
    AnsiString msg="Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportAvrgBallInGroup::FormDestroy(TObject *Sender)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportAvrgBallInGroup::OpenAvgQuery()
{
    CloseAvgQuery();

    MYSQL_RES *result;
    MYSQL_ROW row;

    AnsiString query = "set @grpid = " + IntToStr(GetIDGroup());
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

    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
        while (row = mysql_fetch_row(result))
        {
            avg_progess * data = new avg_progess;
            data->name = row[1] + AnsiString(" ") + row[2] + AnsiString(" ") + row[3];
            data->znum = row[4];
            data->count_otl = AnsiString(row[5]).ToInt();
            data->count_hor = AnsiString(row[6]).ToInt();
            data->count_ud = AnsiString(row[7]).ToInt();
           // data->sred = row[8];
            TReplaceFlags flags;
            flags << rfReplaceAll << rfIgnoreCase;
            data->sred = StringReplace(row[8], ".", ",", flags);

            avg_progress_data.push_back(data);
        }
    }
    mysql_free_result(result);
}
//---------------------------------------------------------------------------
void __fastcall TFormReportAvrgBallInGroup::CloseAvgQuery()
{
    mysql_query(mysql,"drop temporary table if exists temp_stud");
    mysql_query(mysql,"drop temporary table if exists temp_progress1");
    mysql_query(mysql,"drop temporary table if exists temp_progress2");
    mysql_query(mysql,"drop temporary table if exists temp_progress3");

    for (int i = 0; i < avg_progress_data.size(); i++)
        delete avg_progress_data[i];

    avg_progress_data.clear();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormReportAvrgBallInGroup::GetSpec()
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    int numspec;
    AnsiString query ="select specid "
        "from students "
        "where grpid = " + AnsiString(GetIDGroup())+ " AND deleted = 0 "
        "limit 1";
    mysql_query(mysql, query.c_str());

    result = mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
        row = mysql_fetch_row(result);
        numspec = AnsiString(row[0]).ToInt();
    }
    mysql_free_result(result);

    return WCGetTitleForKeyNum(SPECS, numspec);
}
//---------------------------------------------------------------------------
