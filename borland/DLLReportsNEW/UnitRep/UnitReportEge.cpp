//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportEge.h"
#include "UnitFuncs.h"
#include "MacroWord.h"
#include "mysql.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportEge *FormReportEge;

extern EDRegOpts opts;
extern MYSQL*    mysql;
//---------------------------------------------------------------------------
__fastcall TFormReportEge::TFormReportEge(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportEge::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportEge::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
// получаем предметы еге для группы
std::vector<TFormReportEge::name_id> __fastcall TFormReportEge::GetDiscip(AnsiString grp_id)
{
    std::vector<TFormReportEge::name_id> discip;

    AnsiString query = " SELECT v.title, gr.iddiscip FROM ege_for_group AS gr, voc AS v "
                       " WHERE gr.deleted = 0 AND gr.grpid = " + grp_id + " AND v.deleted = 0 "
                       " AND v.num = gr.iddiscip AND v.vkey = 'egediscip' "
                       " ORDER BY v.title ";

    mysql_query(mysql, query.c_str());
    if (MYSQL_RES* result = mysql_store_result(mysql))
    {
      while (MYSQL_ROW row = mysql_fetch_row(result))
        discip.push_back(TFormReportEge::name_id(row[0], row[1]));
      mysql_free_result(result);
    }

    return discip;
}
//---------------------------------------------------------------------------
// получаем студентов группы
std::vector<TFormReportEge::stud_data> __fastcall TFormReportEge::GetStudent(AnsiString grp_id)
{
    std::vector<TFormReportEge::stud_data> student;

    AnsiString query = " SELECT CONCAT(st.secondname,\' \',st.firstname,\' \',st.thirdname), st.id, st.egedoc, st.egereg FROM students AS st "
                       " WHERE st.deleted = 0 AND st.grpid = " + grp_id +
                       " ORDER BY st.secondname ";

    mysql_query(mysql, query.c_str());
    if (MYSQL_RES* result = mysql_store_result(mysql))
    {
      while (MYSQL_ROW row = mysql_fetch_row(result))
        student.push_back(TFormReportEge::stud_data(row[0], row[1], AnsiString(row[2]) != "0"? "" : "+", AnsiString(row[3]) == "00-00000000-00" ? "" : row[3]));
      mysql_free_result(result);
    }
    return student;
}
//---------------------------------------------------------------------------
// заполняем реальными оценками
void __fastcall TFormReportEge::SetRealBall(AnsiString grp_id, std::map<AnsiString, std::map<AnsiString, AnsiString> >& stud_dis_ball)
{
    AnsiString query = " SELECT st.id, gr.iddiscip, pr.ball  FROM students AS st, ege_for_group AS gr "
                       " LEFT JOIN ege_progress AS pr ON gr.iddiscip = pr.iddiscip AND pr.deleted = 0 "
                       " LEFT JOIN voc AS v ON gr.iddiscip = v.num AND v.deleted = 0 AND v.vkey = 'egediscip' "
                       " WHERE gr.deleted = 0 AND gr.grpid = " + grp_id + " AND st.grpid = gr.grpid AND st.deleted = 0 AND pr.idstud = st.id ";

    mysql_query(mysql, query.c_str());
    if (MYSQL_RES* result = mysql_store_result(mysql))
    {
      while (MYSQL_ROW row = mysql_fetch_row(result))
      {
        std::map<AnsiString, std::map<AnsiString, AnsiString> >::iterator dis_ball = stud_dis_ball.find(row[0]);
        std::map<AnsiString, AnsiString>::iterator ball =  dis_ball->second.find(row[1]);
        if (ball != dis_ball->second.end()) // если такая дисциплина есть
          ball->second = row[2];
      }
      mysql_free_result(result);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportEge::CreateWordDocument(void)
{
  std::vector<AnsiString> grp_lst = pFormList->GetGrpList();
  if (grp_lst.empty())
    return;

  WordMacros macros;
  macros.BeginMacros();

  // заголовок
  macros.InsertLine("ActiveDocument.PageSetup.Orientation=wdOrientLandscape");
  macros.InsertLine("ActiveDocument.PageSetup.TopMargin=25");
  macros.InsertLine("ActiveDocument.PageSetup.BottomMargin=25");
  macros.InsertLine("ActiveDocument.PageSetup.LeftMargin=25");
  macros.InsertLine("ActiveDocument.PageSetup.RightMargin=25");

  macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
  macros.SelectionFont("Bold = true");
  macros.SelectionFont("Size = 16");
  macros.SelectionText("Отчет по ЕГЭ\n");
  macros.SelectionFont("Size = 12");
  macros.SelectionFont("Bold = false");
  macros.SelectionParagraphFormat("Alignment = dAlignParagraphLeft");

  //  пройдемся по списку групп
  for (size_t i = 0; i < grp_lst.size(); ++i)
  {
    AnsiString grp_id = grp_lst[i];

    // получим предметы для группы и студентов в группе
    std::vector<TFormReportEge::name_id>   discip  = GetDiscip(grp_id);
    std::vector<TFormReportEge::stud_data> student = GetStudent(grp_id);

    // сделаем заготовки оценок предметов
    std::map<AnsiString, AnsiString> dis_ball;
    for (size_t i = 0; i < discip.size(); ++i)
      dis_ball.insert(std::make_pair(discip[i].id, "0"));

    // сделаем заготовки оценок по студентам
    std::map<AnsiString, std::map<AnsiString, AnsiString> > stud_dis_ball;
    for (size_t i = 0; i < student.size(); ++i)
      stud_dis_ball.insert(std::make_pair(student[i].id, dis_ball));

    // заполним настоящими оценками
    SetRealBall(grp_id, stud_dis_ball);

    // заголовок таблицы
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionText("Группа " + WCGetTitleForKeyNum(GROUPS, grp_id.ToInt()));
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();

    // создадим таблицу
    int CountTables = i + 1;
    macros.TablesAdd(student.size()+2, 4 + discip.size() + 1);

    macros.TablesColumns(CountTables, 1, "Width = 30");
    macros.TablesColumns(CountTables, 2, "Width = 190");
    macros.TablesColumns(CountTables, 3, "Width = 40");
    macros.TablesColumns(CountTables, 4, "Width = 80");
    macros.TablesColumns(CountTables, 5+discip.size(), "Width = 40");

    macros.TablesColumns(CountTables, 1, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.TablesColumns(CountTables, 2, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.TablesColumns(CountTables, 3, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesColumns(CountTables, 4, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");

    macros.InsertLine("ActiveDocument.Tables.Item("+IntToStr(CountTables)+").Range.Font.Size = 10");
    macros.InsertLine("ActiveDocument.Tables.Item("+IntToStr(CountTables)+").Rows.Item(1).Range.Font.Bold = true");
    macros.InsertLine("ActiveDocument.Tables.Item("+IntToStr(CountTables)+").Rows.Item(2).Range.Font.Bold = true");

    macros.TablesCell(CountTables, 1, 1, "Range.Select");
    macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=1, Extend:=wdExtend");
    macros.InsertLine("Selection.Cells.Merge");
    macros.TablesCell(CountTables, 1, 1, "Range.Text = \"№\"");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(CountTables, 1, 2, "Range.Select");
    macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=1, Extend:=wdExtend");
    macros.InsertLine("Selection.Cells.Merge");
    macros.TablesCell(CountTables, 1, 2, "Range.Text = \"ФИО\"");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(CountTables, 1, 3, "Range.Select");
    macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=1, Extend:=wdExtend");
    macros.InsertLine("Selection.Cells.Merge");
    macros.TablesCell(CountTables, 1, 3, "Range.Text = \"Ориг.\"");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(CountTables, 1, 4, "Range.Select");
    macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=1, Extend:=wdExtend");
    macros.InsertLine("Selection.Cells.Merge");
    macros.TablesCell(CountTables, 1, 4, "Range.Text = \"№ св-ва ЕГЭ\"");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(CountTables, 1, 5+discip.size(), "Range.Select");
    macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=1, Extend:=wdExtend");
    macros.InsertLine("Selection.Cells.Merge");
    macros.TablesCell(CountTables, 1, 5+discip.size(), "Range.Text = \"Сумма\"");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");

    if (discip.size())
    {
      macros.TablesCell(CountTables, 1, 5, "Range.Select");
      macros.InsertLine("Selection.MoveRight Unit:=wdCharacter, Count:=" + AnsiString(discip.size()-1)+ ", Extend:=wdExtend");
      macros.InsertLine("Selection.Cells.Merge");
      macros.TablesCell(CountTables, 1, 5, "Range.Text = \"Баллы\"");
      macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");

      for (size_t j = 0; j < discip.size(); ++j)
        macros.TablesCell(CountTables, 2, 5+j, "Range.Text = \"" + discip[j].name +"\"");
    }


    // заполняем фио и номера
    for (size_t i = 0; i < student.size(); ++i)
    {
      int row = i + 3;
      macros.TablesCell(CountTables, row, 1, "Range.Text = \"" + AnsiString(i+1) + "\"");
      macros.TablesCell(CountTables, row, 2, "Range.Text = \"" + student[i].name + "\"");
      macros.TablesCell(CountTables, row, 3, "Range.Text = \"" + student[i].orig + "\"");
      macros.TablesCell(CountTables, row, 4, "Range.Text = \"" + student[i].number + "\"");

      int ball_sum = 0;
      for (size_t j = 0; j < discip.size(); ++j)
      {
        std::map<AnsiString, std::map<AnsiString, AnsiString> >::iterator dis_ball = stud_dis_ball.find(student[i].id);
        std::map<AnsiString, AnsiString>::iterator ball =  dis_ball->second.find(discip[j].id);

        if (ball->second != "0")
          macros.TablesCell(CountTables, row, 4+1+j, "Range.Text = \"" + ball->second + "\"");
        ball_sum += ball->second.ToInt();
      }
      if (ball_sum)
        macros.TablesCell(CountTables, row, 4+1+discip.size(), "Range.Text = \"" + AnsiString(ball_sum) + "\"");
    }

    // перейдем в низ
    macros.InsertLine("ActiveDocument.Tables.Item("+IntToStr(CountTables)+").Cell("+IntToStr(student.size()+2) +",1).Range.Select");
    macros.InsertLine("Selection.MoveDown Unit:=wdLine");

  }

  macros.EndMacros();
  macros.RunMacros();

}
//---------------------------------------------------------------------------
void __fastcall TFormReportEge::ToolBtnPreviewClick(
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
void __fastcall TFormReportEge::FormCreate(TObject *Sender)
{
  pFormList = new TFormListGroup(this);

 if (!WCConnect())
  {
    AnsiString msg="Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportEge::FormDestroy(TObject *Sender)
{
  pFormList->Free();
  WCDisconnect();
}
//---------------------------------------------------------------------------
