//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportBULast2.h"
#include "MacroWord.h"
#include "UnitSelectYear.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "UnitFuncs.h"
#include "mysql.h"
#include <stdio>

TFormReportBULast2 *FormReportBULast2;

extern EDRegOpts opts;
extern MYSQL *mysql;
//---------------------------------------------------------------------------
__fastcall TFormReportBULast2::TFormReportBULast2(TComponent* Owner)
    : TForm(Owner)
{
}

long GetCountRows(AnsiString table)
{
  MYSQL_RES *result;
  MYSQL_ROW row;
  table = " SELECT COUNT(*) FROM " + table;
  mysql_query(mysql, table.c_str());

  AnsiString err = mysql_error(mysql);
  if (err.Length())
     Application->MessageBox(err.c_str(),"Ошибка.",MB_OK|MB_ICONEXCLAMATION);

  long count = 0;
  if (result = mysql_store_result(mysql))
     if (row = mysql_fetch_row(result))
       count = AnsiString(row[0]).ToInt();
  mysql_free_result(result);

  return count;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast2::ToolBtnPreviewClick(TObject *Sender)
{
  TFormSelectYear* form = new TFormSelectYear(this);
  try
  {
    if ( mrOk == form->ShowModal())
    {
        firstyear = form->ComboBoxFirstYear->Text;
        secondyear = AnsiString(firstyear.ToInt() + 1);

        // создадим таблицу с данными
        CreateBuhData();

        // определим кол-во строк в таблице
        MYSQL_RES *result;
        MYSQL_ROW row;
        mysql_query(mysql, " SELECT COUNT(*) FROM full_table");

        long count = 0;
        if (result = mysql_store_result(mysql))
            if (row = mysql_fetch_row(result))
                count = AnsiString(row[0]).ToInt();
        mysql_free_result(result);


        if (count)
          CreateWordDocument(count);
        else
          MessageBox(Handle, "Произошла ошибка. Обратитесь к разработчику.","Ошибка",MB_OK|MB_ICONERROR);

        mysql_query(mysql, "drop temporary table if exists full_table");
    }
  }
  catch(...)
  {
    Application->MessageBox("Работа с MS Word не возможна. Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
  delete form;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast2::ToolBtnCloseClick(TObject *Sender)
{
  Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast2::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;    
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast2::FormCreate(TObject *Sender)
{
  if ( !WCConnect())
  {
    AnsiString msg="Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast2::FormDestroy(TObject *Sender)
{
    WCDisconnect();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast2::CreateWordDocument(long rows_count)
{
    WordMacros macros;
    macros.BeginMacros();

    macros.InsertLine("ActiveDocument.PageSetup.Orientation = wdOrientLandscape");
    macros.InsertLine("ActiveDocument.PageSetup.TopMargin = 25");
    macros.InsertLine("ActiveDocument.PageSetup.BottomMargin = 25");
    macros.InsertLine("ActiveDocument.PageSetup.LeftMargin = 25");
    macros.InsertLine("ActiveDocument.PageSetup.RightMargin = 25");

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionFont("Size=20");
    macros.SelectionFont("Bold=true");
    macros.SelectionText("2010/2011 гг.");
    macros.SelectionFont("Size=10");
    macros.SelectionFont("Bold=false");

    macros.TablesAdd(2 + rows_count, 7);

    // размеры
    macros.TablesColumns(1, 1, "Width = 200");
    macros.TablesColumns(1, 2, "Width = 70");
    macros.TablesColumns(1, 3, "Width = 100");
    macros.TablesColumns(1, 4, "Width = 100");
    macros.TablesColumns(1, 5, "Width = 100");
    macros.TablesColumns(1, 6, "Width = 100");
    macros.TablesColumns(1, 7, "Width = 100");

    macros.InsertLine("ActiveDocument.Tables.Item(1).Range.Font.Size = 9");
    macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(1).Range.Font.Bold = true");
    macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(2).Range.Font.Bold = true");

    macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(1,1).Range.Select");
    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
    macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=1, Extend:=wdExtend");
    macros.InsertLine("Selection.Cells.Merge");
    macros.TablesCell(1, 1, 1, "VerticalAlignment=wdCellAlignVerticalCenter");
    macros.TablesCell(1, 1, 1, "Range.Text = \"Ф.И.О.\"");

    macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(1,2).Range.Select");
    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
    macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=1, Extend:=wdExtend");
    macros.InsertLine("Selection.Cells.Merge");
    macros.TablesCell(1, 1, 2, "VerticalAlignment=wdCellAlignVerticalCenter");
    macros.TablesCell(1, 1, 2, "Range.Text = \"Группа\"");

    macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(1,3).Range.Select");
    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
    macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=1, Extend:=wdExtend");
    macros.InsertLine("Selection.Cells.Merge");
    macros.TablesCell(1, 1, 3, "VerticalAlignment=wdCellAlignVerticalCenter");
    macros.TablesCell(1, 1, 3, "Range.Text = \"Долг за предыдущ. период на 01.09.2010\"");

    macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(1,4).Range.Select");
    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
    macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=1, Extend:=wdExtend");
    macros.InsertLine("Selection.Cells.Merge");
    macros.TablesCell(1, 1, 4, "VerticalAlignment=wdCellAlignVerticalCenter");
    macros.TablesCell(1, 1, 4, "Range.Text = \"Стоимость обучения 2010/2011\"");

    macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(1,5).Range.Select");
    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
    macros.InsertLine("Selection.MoveRight Unit:=wdCharacter, Count:=2, Extend:=wdExtend");
    macros.InsertLine("Selection.Cells.Merge");
    macros.TablesCell(1, 1, 5, "VerticalAlignment=wdCellAlignVerticalCenter");
    macros.TablesCell(1, 1, 5, "Range.Text = \"Оплачено\"");

    macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(2,5).Range.Select");
    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
    macros.TablesCell(1, 2, 5, "VerticalAlignment=wdCellAlignVerticalCenter");
    macros.TablesCell(1, 2, 5, "Range.Text = \"на 01.09.2010 за 2010/2011\"");

    macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(2,6).Range.Select");
    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
    macros.TablesCell(1, 2, 6, "VerticalAlignment=wdCellAlignVerticalCenter");
    macros.TablesCell(1, 2, 6, "Range.Text = \"на 01.09.2010 за 2010/2011\"");

    macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(2,7).Range.Select");
    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
    macros.TablesCell(1, 2, 7, "VerticalAlignment=wdCellAlignVerticalCenter");
    macros.TablesCell(1, 2, 7, "Range.Text = \"на 01.09.2010 за 2010/2011\"");

    AnsiString text;
    text += "Ф.И.О,Группа,Долг за предыдущ.,Стоимость,Оплачено,Оплачено,Долг на\n";
    text += ",, период на,обучения,на 01.09."+firstyear+",на 01.12."+firstyear+",01.09."+firstyear+"\n";
    text += ",,01.09."+firstyear+","+firstyear+"/"+secondyear+" г.,за "+firstyear+"/"+secondyear+ "г.,за "+firstyear+"/"+secondyear+" г.\n\n";



    mysql_query(mysql, " SELECT CONCAT_WS(' ', s.secondname, s.firstname, s.thirdname) as name, v.title, f.pre_dolg, f.plan, f.pay_09, f.pay_12, f.dolg_09 "
                       " FROM full_table as f, students as s, voc as v "
                       " WHERE s.id = f.idstud AND v.vkey='grp' AND v.num = f.grpid AND v.deleted = 0 "
                       " ORDER BY v.title, name ");
  //  Application->MessageBox(AnsiString(rows_count).c_str(),"Ошибка.",MB_OK|MB_ICONEXCLAMATION);

    MYSQL_RES* result;
    MYSQL_ROW  row;
    int        cur_row = 3;
    if (result = mysql_store_result(mysql))
      while (row = mysql_fetch_row(result))
      {
        /*macros.InsertLine("ActiveDocument.Tables.Item(1).Cell("+IntToStr(cur_row)+",1).Range.Text= \"" + AnsiString(row[0]) + "\"");
        macros.InsertLine("ActiveDocument.Tables.Item(1).Cell("+IntToStr(cur_row)+",2).Range.Text= \"" + AnsiString(row[1]) + "\"");
        macros.InsertLine("ActiveDocument.Tables.Item(1).Cell("+IntToStr(cur_row)+",3).Range.Text= \"" + AnsiString(row[2]) + "\"");
        macros.InsertLine("ActiveDocument.Tables.Item(1).Cell("+IntToStr(cur_row)+",4).Range.Text= \"" + AnsiString(row[3]) + "\"");
        macros.InsertLine("ActiveDocument.Tables.Item(1).Cell("+IntToStr(cur_row)+",5).Range.Text= \"" + AnsiString(row[4]) + "\"");
        macros.InsertLine("ActiveDocument.Tables.Item(1).Cell("+IntToStr(cur_row)+",6).Range.Text= \"" + AnsiString(row[5]) + "\"");
        macros.InsertLine("ActiveDocument.Tables.Item(1).Cell("+IntToStr(cur_row)+",7).Range.Text= \"" + AnsiString(row[6]) + "\"");
        */
        text += AnsiString(row[0]) + "," + AnsiString(row[1]) + ",";
        text += AnsiString(row[2]) + "," + AnsiString(row[3]) + ",";
        text += AnsiString(row[4]) + "," + AnsiString(row[5]) + ",";
        text += AnsiString(row[6]) + "\n";
        ++cur_row;

      }
    mysql_free_result(result);

    macros.EndMacros();
   // macros.RunMacros();

   HANDLE  hfile = CreateFile("Account.csv", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

   DWORD dwBytesWritten;
   WriteFile(hfile, text.c_str(), text.Length(),&dwBytesWritten,NULL);
   CloseHandle(hfile);

   Application->MessageBox("Данные по оплате сохранены в Account.csv","Информация",MB_OK|MB_ICONINFORMATION);

}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast2::CreateBuhData(void)
{
    mysql_query(mysql, "drop temporary table if exists full_table");
    mysql_query(mysql,
        "CREATE TEMPORARY TABLE full_table "
        " ( "
        "  idstud    int(11), "
        "  grpid     int(11), "
        "  pre_dolg  int(11) DEFAULT '0', "
        "  plan      int(11) DEFAULT '0', "
        "  pay_09    int(11) DEFAULT '0', "
        "  pay_12    int(11) DEFAULT '0', "
        "  dolg_09   int(11) DEFAULT '0', "
        "  INDEX (idstud), "
        "  INDEX (grpid) "
        " ) TYPE = HEAP ");

       AnsiString query;

    // зададим даты для периода 2009-2011

    query = "SET @date_a1 = '" + AnsiString(firstyear.ToInt()-1) + "-02-01'"; // 2009-02-01
    mysql_query(mysql, query.c_str());
    query = "SET @date_a2 = '" + AnsiString(firstyear.ToInt()-1) + "-09-01'"; // 2009-09-01
    mysql_query(mysql, query.c_str());
    query = "SET @date_a3 = '" + firstyear + "-09-01'";                       // 2010-02-01
    mysql_query(mysql, query.c_str());
    query = "SET @date_a4 = '" + firstyear + "-09-01'";                       // 2010-09-01
    mysql_query(mysql, query.c_str());
    query = "SET @date_a5 = '" + secondyear + "-02-01'";                      // 2011-02-01
    mysql_query(mysql, query.c_str());

    // найдем студентов, которые имеют план оплаты за этот или предыдущий (для долга) периоды
    // сделаем группировки т.к. может быть несколько план за один период
    mysql_query(mysql,"INSERT full_table (idstud,grpid) "
        " SELECT studs.id, studs.grpid"
        " FROM `payoptstest` AS opts, students AS studs "
        " WHERE studs.grpid = opts.idgroup"
        " AND (opts.datestart = @date_a1 OR opts.datestart = @date_a2 OR opts.datestart = @date_a3 "
        " OR opts.datestart = @date_a4 OR opts.datestart = @date_a5) "
        " AND opts.deleted = 0 AND studs.deleted = 0 AND studs.cityid != 0 "
        " GROUP BY studs.id ");

    ProcessPreDolg();    // расчитаем долг за предыдущий период
    ProcessCurPlan();    // расчитаем план для текущего периода
    ProcessPay(true);    // оплата на 01.09 за текущий год
    ProcessPay(false);   // оплата на 01.12 за текущий год

    // расчитаем долг на текущий период на 01.09
    mysql_query(mysql,  " UPDATE full_table SET dolg_09 = plan - pay_09");

    // удалим тех, кто не учиться в этом году
    mysql_query(mysql,  " DELETE FROM full_table WHERE plan = 0");
}

void __fastcall TFormReportBULast2::ProcessPlan(AnsiString year)
{
    AnsiString query;

    // зададим даты для расчета долга в предыдущем периоде 2009-2010
    query = "SET @date_pre1 = '" + year + "-02-01'";                        // 2009-02-01
    mysql_query(mysql, query.c_str());
    query = "SET @date_pre2 = '" + year + "-09-01'";                        // 2009-09-01
    mysql_query(mysql, query.c_str());
    query = "SET @date_pre3 = '" + AnsiString(year.ToInt()+1) + "-02-01'";  // 2010-02-01

    // соберем оплаты по каждой категории
    mysql_query(mysql, "drop temporary table if exists old_pay");
    mysql_query(mysql,
        "CREATE TEMPORARY TABLE old_pay "
        " ( "
        "  id        int(11) NOT NULL AUTO_INCREMENT, "
        "  idstud    int(11) NOT NULL, "
        "  idopt     int(11) NOT NULL, "
        "  plan      int(11) NOT NULL, "
        "  pay       int(11) NOT NULL, "
        "  INDEX (id),     "
        "  INDEX (idstud)  "
        " ) TYPE = HEAP ");

    //# это обычная сентябрьская оплата
    mysql_query(mysql,"INSERT old_pay (idstud, idopt, plan, pay)                                    "
          " SELECT s.idstud, s.idopt, s.commoncountmoney, SUM(COALESCE(fact.moneypay, 0))           "
          " FROM (                                                                                  "
          "      SELECT st.id as idstud, opts.id as idopt, opts.commoncountmoney                    "
          "      FROM students AS st, payoptstest as opts, voc as v                                 "
          "      WHERE st.deleted = 0 AND opts.deleted = 0 AND v.deleted = 0                        "
          "      AND v.vkey = 'grp'  AND st.grpid = v.num AND opts.idgroup = st.grpid               "
          "      AND opts.datestart=@date_pre2 AND st.cityid != 0                                   "
          " ) as s                                                                                  "
          " LEFT JOIN payfactstest AS fact                                                          "
          " ON s.idopt = fact.idopts AND s.idstud = fact.idstud AND fact.deleted = 0                "
          " GROUP BY s.idstud, s.idopt");

    //# это февральские предыдущие
    mysql_query(mysql,"INSERT old_pay (idstud, idopt, plan, pay)                                    "
          " SELECT s.idstud, s.idopt, s.commoncountmoney/2, SUM(COALESCE(fact.moneypay, 0)) - s.commoncountmoney/2  "
          " FROM (                                                                                  "
          "      SELECT st.id as idstud, opts.id as idopt, opts.commoncountmoney                    "
          "      FROM students AS st, payoptstest as opts, voc as v                                 "
          "	     WHERE st.deleted = 0 AND opts.deleted = 0 AND v.deleted = 0                        "
          "      AND v.vkey = 'grp'  AND st.grpid = v.num AND opts.idgroup = st.grpid               "
          "      AND opts.datestart=@data_pre1 AND st.cityid != 0                                   "
          " ) as s                                                                                  "
          " LEFT JOIN payfactstest AS fact                                                          "
          " ON s.idopt = fact.idopts AND s.idstud = fact.idstud AND fact.deleted = 0                "
          " GROUP BY s.idstud, s.idopt");
    //# сделаеим для них проверку что бы не было отрицательных оплат
    mysql_query(mysql, "UPDATE old_pay SET old_pay.pay = 0 WHERE old_pay.pay < 0 ");

     //# это февральские последующие
    mysql_query(mysql,"INSERT old_pay (idstud, idopt, plan, pay)                                    "
          " SELECT s.idstud, s.idopt, s.commoncountmoney/2, SUM(COALESCE(fact.moneypay, 0))                  "
          " FROM (                                                                                  "
          "      SELECT st.id as idstud, opts.id as idopt, opts.commoncountmoney                    "
          "      FROM students AS st, payoptstest as opts, voc as v                                 "
          "	     WHERE st.deleted = 0 AND opts.deleted = 0 AND v.deleted = 0                        "
          "      AND v.vkey = 'grp'  AND st.grpid = v.num AND opts.idgroup = st.grpid               "
          "      AND opts.datestart=@data_pre3 AND st.cityid != 0                                   "
          " ) as s                                                                                  "
          " LEFT JOIN payfactstest AS fact                                                          "
          " ON s.idopt = fact.idopts AND s.idstud = fact.idstud AND fact.deleted = 0                "
          " GROUP BY s.idstud, s.idopt");
    //# сделаеим для них проверку что бы не было оплаты больше плана
    mysql_query(mysql,"UPDATE old_pay SET old_pay.pay = old_pay.plan WHERE old_pay.pay > old_pay.plan ");

    // этап 2
    // у стедента могут несколько категорий оплат за текущий период, то есть два сценария поведения
    // 1 если хотя бы по одной было заплачено(может быть несколько), то удалить те которые без оплаты
    // 2 если ни по одной не заплачено, то оставить только ту которая основная в группе

    // найдем студентов у которых проблемы
    mysql_query(mysql, "drop temporary table if exists bad_stud");
    mysql_query(mysql,
        " CREATE TEMPORARY TABLE bad_stud                  "
        " (                                                "
        "  id        int(11) NOT NULL AUTO_INCREMENT,      "
        "  idstud    int(11) NOT NULL,                     "
        "  _count    int(11) NOT NULL,                     "
        "  pay       int(11) NOT NULL,                     "
        "  INDEX (id),                                     "
        "  INDEX (idstud)                                  "
        " ) TYPE = HEAP ");
    mysql_query(mysql,"INSERT bad_stud (idstud, _count, pay) "
          " SELECT idstud, COUNT(0) as c, SUM(pay)           "
          " FROM old_pay                                     "
          " GROUP BY idstud "
          " HAVING c > 1");

    // найдем котагории которые являются для проблемных студентов основными
    mysql_query(mysql, "drop temporary table if exists main_opt");
    mysql_query(mysql,
        " CREATE TEMPORARY TABLE main_opt                  "
        " (                                                "
        "  id        int(11) NOT NULL AUTO_INCREMENT,      "
        "  grpid     int(11) NOT NULL,                     "
        "  idopt     int(11) NOT NULL,                     "
        "  count     int(11) NOT NULL,"
        "  INDEX (id),                                     "
        "  INDEX (grpid)                                   "
        " ) TYPE = HEAP ");
    mysql_query(mysql,"INSERT main_opt (grpid, idopt, count)  "
            " SELECT m.grpid, m.idopt, MAX(m.c)               "
            " FROM (SELECT s.grpid, pay.idopt, COUNT(*) as c  "
            "       FROM old_pay as pay, students as s        "
            "       WHERE pay.idstud = s.id AND pay.pay > 0   "
            "       GROUP BY s.grpid, pay.idopt               "
            "      ) as m                                     "
            " GROUP BY m.grpid");

    // сценарий 1
    mysql_query(mysql, " DELETE old_pay                          "
                       " FROM old_pay, bad_stud                  "
                       " WHERE old_pay.idstud = bad_stud.idstud  "
                       " AND bad_stud.pay > 0 AND old_pay.pay = 0");
    // сценарий 2
    mysql_query(mysql, " DELETE old_pay                                   "
                       " FROM old_pay, bad_stud, students as s, main_opt  "
                       " WHERE old_pay.idstud = bad_stud.idstud           "
                       " AND bad_stud.pay = 0 AND s.id = bad_stud.idstud  "
                       " AND main_opt.grpid = s.grpid                     "
                       " AND old_pay.idopt != main_opt.idopt");


    mysql_query(mysql, "drop temporary table if exists bad_stud");
    mysql_query(mysql, "drop temporary table if exists main_opt");
}
// расчитаем долг за предыдущий период
void __fastcall TFormReportBULast2::ProcessPreDolg(void)
{
    ProcessPlan(AnsiString(firstyear.ToInt()-1));

    // посчитаем долг по каждой оплате и объединим их
    mysql_query(mysql, "drop temporary table if exists old_dolg");
    mysql_query(mysql,
        " CREATE TEMPORARY TABLE old_dolg                  "
        " (                                                "
        "  id        int(11) NOT NULL AUTO_INCREMENT,      "
        "  idstud    int(11) NOT NULL,                     "
        "  dolg      int(11) NOT NULL,                     "
        "  INDEX (id),                                     "
        "  INDEX (idstud)                                  "
        " ) TYPE = HEAP ");
    mysql_query(mysql,"INSERT old_dolg  (idstud, dolg)     "
          " SELECT pay.idstud, SUM(pay.plan - pay.pay)     "
          " FROM old_pay AS pay                            "
          " GROUP BY pay.idstud ");

//    Application->MessageBox(AnsiString(GetCountRows("old_dolg")).c_str(),"Ошибка.",MB_OK|MB_ICONEXCLAMATION);
//    Application->MessageBox(AnsiString(GetCountRows("old_pay")).c_str(),"Ошибка.",MB_OK|MB_ICONEXCLAMATION);

    // перенесем в основную таблицу
    mysql_query(mysql,  " UPDATE full_table, old_dolg             "
                        " SET full_table.pre_dolg = old_dolg.dolg "
                        " WHERE full_table.idstud = old_dolg.idstud");


    mysql_query(mysql, "drop temporary table if exists old_dolg");
    mysql_query(mysql, "drop temporary table if exists old_pay");
}

// расчитаем план за текущий период
void __fastcall TFormReportBULast2::ProcessCurPlan(void)
{
    ProcessPlan(firstyear);

    // у студента может быть несколько категорий оплат
    mysql_query(mysql, "drop temporary table if exists plan");
    mysql_query(mysql,
        "CREATE TEMPORARY TABLE plan "
        " ( "
        "  id        int(11) NOT NULL AUTO_INCREMENT, "
        "  idstud    int(11) NOT NULL, "
        "  plan      int(11) NOT NULL, "
        "  INDEX (id),     "
        "  INDEX (idstud)  "
        " ) TYPE = HEAP ");
     mysql_query(mysql," INSERT plan (idstud, plan)        "
                       " SELECT pay.idstud, SUM(pay.plan - 0)  "
                       " FROM old_pay AS pay               "
                       " GROUP BY pay.idstud ");
    // перенесем в основную таблицу
    mysql_query(mysql,  " UPDATE full_table, plan         "
                        " SET full_table.plan = plan.plan "
                        " WHERE full_table.idstud = plan.idstud");

    mysql_query(mysql, "drop temporary table if exists plan");
    mysql_query(mysql, "drop temporary table if exists old_pay");
}

void __fastcall TFormReportBULast2::ProcessPay(bool is_09)
{
    AnsiString query;

    // зададим даты для текущего периода 2010-2011
    query = "SET @date1 = '" + firstyear + "-02-01'";                       // 2010-02-01
    mysql_query(mysql, query.c_str());
    query = "SET @date2 = '" + firstyear + "-09-01'";                       // 2010-09-01
    mysql_query(mysql, query.c_str());
    query = "SET @date3 = '" + secondyear + "-02-01'";                      // 2011-02-01
    mysql_query(mysql, query.c_str());

    if (is_09)
        query = "SET @date_for = '" + firstyear + "-09-01'";
    else
        query = "SET @date_for = '" + firstyear + "-12-01'";
    mysql_query(mysql, query.c_str());


    // т.к. у студентов может оказаться несколько категорий оплат на один период
    // то сделаем в два этапа
    mysql_query(mysql, "drop temporary table if exists pay1");
    mysql_query(mysql,
        "CREATE TEMPORARY TABLE pay1 "
        " ( "
        "  id        int(11) NOT NULL AUTO_INCREMENT, "
        "  idstud    int(11) NOT NULL, "
        "  plan      int(11) NOT NULL, "
        "  pay       int(11) NOT NULL, "
        "  INDEX (id),     "
        "  INDEX (idstud)  "
        " ) TYPE = HEAP ");

    //# это обычная сентябрьская оплата
    mysql_query(mysql,"INSERT pay1 (idstud, plan, pay)                                              "
          " SELECT s.idstud, s.commoncountmoney, SUM(COALESCE(fact.moneypay, 0))                    "
          " FROM (                                                                                  "
          "      SELECT st.id as idstud, opts.id as idopt, opts.commoncountmoney                    "
          "      FROM students AS st, payoptstest as opts                                           "
          "      WHERE st.deleted = 0 AND opts.deleted = 0 AND opts.idgroup = st.grpid              "
          "      AND opts.datestart=@date2 AND st.cityid != 0                                       "
          " ) as s                                                                                  "
          " LEFT JOIN payfactstest AS fact                                                          "
          " ON s.idopt = fact.idopts AND s.idstud = fact.idstud AND fact.deleted = 0 AND fact.datepay <= @date_for "
          " GROUP BY s.idstud, s.idopt");

    //# это февральские предыдущие
    mysql_query(mysql,"INSERT pay1 (idstud, plan, pay)                                              "
          " SELECT s.idstud, s.commoncountmoney/2, SUM(COALESCE(fact.moneypay, 0)) - s.commoncountmoney/2  "
          " FROM (                                                                                  "
          "      SELECT st.id as idstud, opts.id as idopt, opts.commoncountmoney                    "
          "      FROM students AS st, payoptstest as opts                                           "
          "	     WHERE st.deleted = 0 AND opts.deleted = 0 AND opts.idgroup = st.grpid              "
          "      AND opts.datestart=@data1 AND st.cityid != 0                                       "
          " ) as s                                                                                  "
          " LEFT JOIN payfactstest AS fact                                                          "
          " ON s.idopt = fact.idopts AND s.idstud = fact.idstud AND fact.deleted = 0 AND fact.datepay <= @date_for "
          " GROUP BY s.idstud, s.idopt");
    //# сделаеим для них проверку что бы не было отрицательных оплат
    mysql_query(mysql, "UPDATE pay1 SET pay1.pay = 0 WHERE pay1.pay < 0 ");

     //# это февральские последующие
    mysql_query(mysql,"INSERT pay1 (idstud, plan, pay)                                              "
          " SELECT s.idstud, s.commoncountmoney/2, SUM(COALESCE(fact.moneypay, 0))                  "
          " FROM (                                                                                  "
          "      SELECT st.id as idstud, opts.id as idopt, opts.commoncountmoney                    "
          "      FROM students AS st, payoptstest as opts                                           "
          "	     WHERE st.deleted = 0 AND opts.deleted = 0 AND opts.idgroup = st.grpid              "
          "      AND opts.datestart=@data3 AND st.cityid != 0                                       "
          " ) as s                                                                                  "
          " LEFT JOIN payfactstest AS fact                                                          "
          " ON s.idopt = fact.idopts AND s.idstud = fact.idstud AND fact.deleted = 0 AND fact.datepay <= @date_for "
          " GROUP BY s.idstud, s.idopt");
    //# сделаеим для них проверку что бы не было оплаты больше плана
    mysql_query(mysql,"UPDATE pay1 SET pay1.pay = pay1.plan WHERE pay1.pay > pay1.plan ");


    // может быть несколько оплат
    mysql_query(mysql, "drop temporary table if exists pay2");
    mysql_query(mysql,
        " CREATE TEMPORARY TABLE pay2                 "
        " (                                           "
        "  id        int(11) NOT NULL AUTO_INCREMENT, "
        "  idstud    int(11) NOT NULL,                "
        "  pay       int(11) NOT NULL,                "
        "  INDEX (id),                                "
        "  INDEX (idstud)                             "
        " ) TYPE = HEAP ");
    mysql_query(mysql,"INSERT pay2  (idstud, pay)     "
          " SELECT pay1.idstud, SUM(pay1.pay)         "
          " FROM pay1                                 "
          " GROUP BY pay1.idstud ");

   // Application->MessageBox(AnsiString(GetCountRows("pay1")).c_str(),"Ошибка.",MB_OK|MB_ICONEXCLAMATION);
   // Application->MessageBox(AnsiString(GetCountRows("pay2")).c_str(),"Ошибка.",MB_OK|MB_ICONEXCLAMATION);

    // перенесем в основную таблицу
    if (is_09)
        mysql_query(mysql,  " UPDATE full_table, pay2          "
                            " SET full_table.pay_09 = pay2.pay "
                            " WHERE full_table.idstud = pay2.idstud");
    else
        mysql_query(mysql,  " UPDATE full_table, pay2          "
                            " SET full_table.pay_12 = pay2.pay "
                            " WHERE full_table.idstud = pay2.idstud");

    mysql_query(mysql, "drop temporary table if exists pay1");
    mysql_query(mysql, "drop temporary table if exists pay2");
}

