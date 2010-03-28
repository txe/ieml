//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportBULast.h"
#include "MacroExcel.h"
#include "UnitSelectYear.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "UnitFuncs.h"
#include "mysql.h"

TFormReportBULast *FormReportBULast;

extern EDRegOpts opts;
extern MYSQL *mysql;
//---------------------------------------------------------------------------
__fastcall TFormReportBULast::TFormReportBULast(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast::ToolBtnPreviewClick(TObject *Sender)
{
  TFormSelectYear* form = new TFormSelectYear(this);
  try
  {
    if ( mrOk == form->ShowModal())
    {
        firstyear = form->ComboBoxFirstYear->Text;
        secondyear = AnsiString(firstyear.ToInt() + 1);
        CreateBuhData();
        CreateWordDocument();
        DestroyBuhData();
    }
  }
  catch(...)
  {
    Application->MessageBox("Работа с MS Excel не возможна. Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
  delete form;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast::ToolBtnCloseClick(TObject *Sender)
{
  Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;    
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast::FormCreate(TObject *Sender)
{
  if ( !WCConnect())
  {
    AnsiString msg="Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast::FormDestroy(TObject *Sender)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast::CreateWordDocument(void)
{
    AnsiString month[] = {"Сентябрь", "Октябрь", "Ноябрь",
        "Декабрь", "Январь", "Февраль",
        "Март", "Апрель", "Май",
        "Июнь", "Июль", "Август"};

    ExcelMacros macros;
    macros.BeginMacros();

// шапка
    macros.Select("A1");
    macros.Formula("Города");
    macros.Select("B1");
    macros.Formula("План");
    macros.Select("C1");
    macros.Formula("Факт");

    for (int i = 1; i <= 3; i++)
    {
        macros.Cells( 1, i, 2, i, "Select");
        macros.MergeCell();

         macros.Cells( 1, buh_data.size(), 1, buh_data.size(), "ColumnWidth = 10");
    }
    for (int i = 4; i <= 26; i+=2)
    {
        macros.Cells(1, i , 1, i+1, "Select");
        macros.MergeCell();


        macros.Formula(month[(i-4)/2]);

        macros.Cells(2, i, 2, i, "Select");
        macros.Formula("Реализ.");

        macros.Cells(2, i+1, 2, i+1, "Select");
        macros.Formula("Остаток");

    }
    macros.Cells(1, 1, 2, 26, "Select");
    macros.InsertLine("With Selection");
    macros.InsertLine("    .HorizontalAlignment = xlCenter");
    macros.InsertLine("End With");
// размер колонок
    macros.InsertLine("Columns(\"A:A\").ColumnWidth = 34.14");
    for(int i = 2; i <= 26; i++)
        macros.InsertLine("Columns(" + IntToStr(i) + ").ColumnWidth =12.5");
// цветовые полосы
    for (int i = 0; i < buh_data.size() - 1; i+=2)
    {
        macros.Cells(i+4, 1, i+4, 27, "Select");
        macros.InsertLine("With Selection.Interior");
        macros.InsertLine("    .ColorIndex = 15");
        macros.InsertLine("    .Pattern = xlSolid");
        macros.InsertLine("End With");
    }
// делаем бордюр
    macros.Cells(1, 1, buh_data.size() + 2, 27, "Select");
    macros.InsertLine("Selection.Borders(xlDiagonalDown).LineStyle = xlNone");
    macros.InsertLine("Selection.Borders(xlDiagonalUp).LineStyle = xlNone");
    macros.InsertLine("With Selection.Borders(xlEdgeLeft)");
    macros.InsertLine("    .LineStyle = xlContinuous");
    macros.InsertLine("    .Weight = xlThin");
    macros.InsertLine("    .ColorIndex = xlAutomatic");
    macros.InsertLine("End With");
    macros.InsertLine("With Selection.Borders(xlEdgeTop)");
    macros.InsertLine("    .LineStyle = xlContinuous");
    macros.InsertLine("    .Weight = xlThin");
    macros.InsertLine("    .ColorIndex = xlAutomatic");
    macros.InsertLine("End With");
    macros.InsertLine("With Selection.Borders(xlEdgeBottom)");
    macros.InsertLine("    .LineStyle = xlContinuous");
    macros.InsertLine("    .Weight = xlThin");
    macros.InsertLine("    .ColorIndex = xlAutomatic");
    macros.InsertLine("End With");
    macros.InsertLine("With Selection.Borders(xlEdgeRight)");
    macros.InsertLine("    .LineStyle = xlContinuous");
    macros.InsertLine("    .Weight = xlThin");
    macros.InsertLine("    .ColorIndex = xlAutomatic");
    macros.InsertLine("End With");
    macros.InsertLine("With Selection.Borders(xlInsideVertical)");
    macros.InsertLine("    .LineStyle = xlContinuous");
    macros.InsertLine("    .Weight = xlThin");
    macros.InsertLine("    .ColorIndex = xlAutomatic");
    macros.InsertLine("End With");
    macros.InsertLine("With Selection.Borders(xlInsideHorizontal)");
    macros.InsertLine("    .LineStyle = xlContinuous");
    macros.InsertLine("    .Weight = xlThin");
    macros.InsertLine("    .ColorIndex = xlAutomatic");
    macros.InsertLine("End With");

// выводим данные
    for (int i = 0; i < buh_data.size(); i++)
    {
        AnsiString pos_row = IntToStr(i+3);
        macros.Select("A" + pos_row);
        macros.Formula(buh_data[i]->title);
        macros.Select("B" + pos_row);
        macros.Formula(AnsiString(buh_data[i]->plan));
        macros.Select("C" + pos_row);
        macros.Formula(AnsiString(buh_data[i]->pay));

        for (int j = 4; j <= 26; j+=2)
        {
            macros.Cells(i+3, j, i+3, j, "Select");
            macros.Formula("=RC2/12" );

            macros.Cells(i+3, j+1, i+3, j+1, "Select");
            macros.Formula("=RC[-2]-RC[-1]");
        }
    }

    macros.InsertLine("ActiveWindow.ScrollRow = 1");
    macros.InsertLine("ActiveWindow.ScrollColumn = 1");

    macros.EndMacros();
    macros.RunMacros("");

}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast::CreateQuery(void)
{

//# бух для 2006 - 2007 г.г.

    AnsiString query;

    query = "SET @date1 = '" + firstyear + "-09-01'";
    mysql_query(mysql, query.c_str());
    query = "SET @date2 = '" + firstyear + "-02-01'";
    mysql_query(mysql, query.c_str());
    query = "SET @date3 = '" + secondyear + "-02-01'";
    mysql_query(mysql, query.c_str());
//    mysql_query(mysql,"SET @date1 = '2006-09-01' # это обычная сентябрьская оплата ");
//    mysql_query(mysql,"SET @date2 = '2006-02-01' # это февральские предыдущие ");
//    mysql_query(mysql,"SET @date3 = '2007-02-01' # это февральские последующие ");
//# все ясно
    mysql_query(mysql,"CREATE TEMPORARY TABLE _group TYPE = HEAP SELECT num , title FROM voc "
        " WHERE voc.vkey = 'grp' AND voc.deleted = 0 ");

    mysql_query(mysql,"CREATE TEMPORARY TABLE city TYPE = HEAP SELECT num , title FROM voc "
        " WHERE voc.vkey = 'city' AND voc.deleted = 0 ");
    mysql_query(mysql,"INSERT city VALUES(0, 'Отчисленные') ");

    mysql_query(mysql,"CREATE TEMPORARY TABLE temp "
        " ( "
        "  opl_idgrp int(2), "
        "  idstud int(11), "
        "  name   char(20), "
        "  grpid  int(2), "
        "  znum 	int(6), "
        "  pay    int(11), "
        "  plan   int(11), "
        "  id_opl int(11), "
        "  datestart  date, "
        "  vid_plan 	char(1), "
        "  real_pay   int(11), "
        "  real_plan  int(11), "
        "  cityid int(1), "
        "  INDEX (idstud), "
        "  INDEX (grpid), "
        "  INDEX (cityid) "
        " ) TYPE = HEAP ");

//# это обычная сентябрьская оплата
    mysql_query(mysql,"INSERT temp (opl_idgrp,id_opl,idstud,name,grpid,znum,pay,plan,datestart,vid_plan,real_pay,real_plan,cityid) "
        " SELECT  pot.idgroup,pot.id,pft.idstud, sts.secondname, sts.grpid,sts.znum, SUM(pft.moneypay), pot.commoncountmoney,pot.datestart, 'F',SUM(pft.moneypay),pot.commoncountmoney,sts.cityid "
        " FROM `payfactstest` AS pft ,`payoptstest` AS pot, students AS sts "
        " WHERE pft.idopts = pot.id AND pot.datestart = @date1 AND sts.id = pft.idstud "
        " AND pft.deleted = 0 AND pot.deleted = 0 AND sts.deleted = 0 "
        " GROUP BY idstud,pft.idopts ");

//# это февральские предыдущие
    mysql_query(mysql,"INSERT temp (opl_idgrp,id_opl,idstud,name,grpid,znum,pay,plan,datestart,vid_plan,real_pay,real_plan,cityid) "
        " SELECT pot.idgroup,pot.id,pft.idstud, sts.secondname,sts.grpid,sts.znum, (SUM(pft.moneypay) - (pot.commoncountmoney/2)), pot.commoncountmoney/2,pot.datestart, 'R',SUM(pft.moneypay),pot.commoncountmoney,sts.cityid "
        " FROM `payfactstest` AS pft ,`payoptstest` AS pot, students AS sts "
        " WHERE pft.idopts = pot.id AND pot.datestart = @date2 AND sts.id = pft.idstud "
        " AND pft.deleted = 0 AND pot.deleted = 0 AND sts.deleted = 0 "
        " GROUP BY pft.idstud,pft.idopts ");
//# сделаеим для них проверку что бы не было отрицательных оплат
    mysql_query(mysql,"UPDATE temp SET temp.pay = 0 WHERE temp.pay < 0 ");

//# это февральские последующие
    mysql_query(mysql,"INSERT temp (opl_idgrp,id_opl,idstud,name,grpid,znum,pay,plan,datestart,vid_plan,real_pay,real_plan,cityid) "
        " SELECT pot.idgroup,pot.id,pft.idstud, sts.secondname,sts.grpid,sts.znum, SUM(pft.moneypay), pot.commoncountmoney/2,pot.datestart, 'L',SUM(pft.moneypay),pot.commoncountmoney, sts.cityid "
        " FROM `payfactstest` AS pft ,`payoptstest` AS pot, students AS sts "
        " WHERE pft.idopts = pot.id AND pot.datestart = @date3 AND sts.id = pft.idstud "
        " AND pft.deleted = 0 AND pot.deleted = 0 AND sts.deleted = 0 "
        " GROUP BY pft.idstud,pft.idopts ");
//# сделаеим для них проверку что бы не было оплаты больше плана
    mysql_query(mysql,"UPDATE temp SET temp.pay = temp.plan WHERE temp.pay > temp.plan ");
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast::DestroyQuery(void)
{
    mysql_query(mysql,"drop temporary table if exists _group");
    mysql_query(mysql,"drop temporary table if exists city");
    mysql_query(mysql,"drop temporary table if exists temp");
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast::CreateBuhData(void)
{
    DestroyBuhData();

    CreateQuery();

    MYSQL_RES *result;
    MYSQL_ROW row;

//# делаем выборку по городам
    mysql_query(mysql," (SELECT city.title, SUM(pay), SUM(plan) "
        " FROM temp, city "
        " WHERE city.num = temp.cityid AND city.num != 0 "
        " GROUP BY temp.cityid "
        " ORDER BY city.title) "
        " UNION "
        " (SELECT city.title, SUM(pay), SUM(plan) "
        " FROM temp, city "
        " WHERE city.num = temp.cityid AND city.num = 0"
        " GROUP BY temp.cityid "
        " ORDER BY city.title) ");

    result = mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
        while (row = mysql_fetch_row(result))
        {
            opl_data* data = new opl_data;
            data->title = row[0];
            data->plan = AnsiString(row[2]).ToInt();
            data->pay = AnsiString(row[1]).ToInt();
            buh_data.push_back(data);
        }
    }
    mysql_free_result(result);

    DestroyQuery();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBULast::DestroyBuhData(void)
{
    for (int i = 0; i < buh_data.size(); i++)
        delete buh_data[i];

    buh_data.clear();
}
//---------------------------------------------------------------------------
