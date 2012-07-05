//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <fstream.h>
#include "UnitReportForma3.h"
#include "MacroExcel.h"
#include "UnitSelectCurrDate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "UnitFuncs.h"
#include "mysql.h"

TFormReportForma3 *FormReportForma3;

extern EDRegOpts opts;
extern MYSQL *mysql;
//---------------------------------------------------------------------------
__fastcall TFormReportForma3::TFormReportForma3(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportForma3::ToolBtnPreviewClick(TObject *Sender)
{
  TFormSelectDate* form = new TFormSelectDate(this);
  try
  {
    form->ShowModal();
    Word Year, Month, Day;
    TDateTime dtPresent = form->GetSelectedDate();
    DecodeDate(dtPresent, Year, Month, Day);
    curr_year = AnsiString(Year);

    CreateData();
    CreateWordDocument();
    DestroyData();
  }
  catch(...)
  {
    Application->MessageBox("Работа с MS Excel не возможна. Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
  delete form;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportForma3::ToolBtnCloseClick(TObject *Sender)
{
  Close();    
}
//---------------------------------------------------------------------------
void __fastcall TFormReportForma3::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;    
}
//---------------------------------------------------------------------------
void __fastcall TFormReportForma3::FormCreate(TObject *Sender)
{
  if ( !WCConnect())
  {
    AnsiString msg="Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportForma3::FormDestroy(TObject *Sender)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportForma3::CreateWordDocument(void)
{
    ExcelMacros macros;
    macros.BeginMacros();

// шапка
    AnsiString sheet;
    sheet += "Индекс книги;Регистрационный номер;Фамилия, имя, отчество;Наименование специальности;"
        "Код специальности;Код квалификации;Наименование квалификации;Год окончания вуза;"
        "Диплом с отличием да/нет;Серия диплома;№ диплома;Дата выдачи;Решение ГАК от;"
        "Номер протокола ГАК;Номер приказа об отчислении;Дата приказа об отчислении;Номер группы\n";


// делаем бордюр
    macros.Cells(1, 1, data.size() + 1, 17, "Select");
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
// задали размеры столбцов
    macros.InsertLine("Columns(\"A:A\").ColumnWidth = 5.86");
    macros.InsertLine("Columns(\"B:B\").ColumnWidth = 13");
    macros.InsertLine("Columns(\"C:C\").ColumnWidth = 13.14");
    macros.InsertLine("Columns(\"D:D\").ColumnWidth = 11.43");
    macros.InsertLine("Columns(\"E:E\").ColumnWidth = 11.71");
    macros.InsertLine("Columns(\"F:F\").ColumnWidth = 10.8");
    macros.InsertLine("Columns(\"G:G\").ColumnWidth = 11");
    macros.InsertLine("Columns(\"H:H\").ColumnWidth = 7.71");
    macros.InsertLine("Columns(\"I:I\").ColumnWidth = 7.3");
    macros.InsertLine("Columns(\"J:J\").ColumnWidth = 7.57");
    macros.InsertLine("Columns(\"P:P\").ColumnWidth = 9");
    macros.InsertLine("Columns(\"K:K\").ColumnWidth = 6.57");
    macros.InsertLine("Columns(\"O:O\").ColumnWidth = 9.29");
// задаем размер шрифта
    macros.Cells(1, 1, data.size() + 1, 17, "Select");
    macros.InsertLine("With Selection.Font");
    macros.InsertLine("    .Size = 8");
    macros.InsertLine("End With");
// задаем выравнивание текста
    macros.Cells(1, 1, data.size() + 1, 17, "Select");
    macros.InsertLine("With Selection");
    macros.InsertLine("    .HorizontalAlignment = xlCenter");
    macros.InsertLine("    .VerticalAlignment = xlCenter");
    macros.InsertLine("    .WrapText = True");
    macros.InsertLine("End With");
// задаем для столбца тип текст
    macros.Cells(2, 2, data.size() + 1, 2, "Select");
    macros.InsertLine("Selection.NumberFormat = \"@\"");

    macros.Cells(2, 5, data.size() + 1, 5, "Select");
    macros.InsertLine("Selection.NumberFormat = \"@\"");

    macros.Cells(2, 11, data.size() + 1, 11, "Select");
    macros.InsertLine("Selection.NumberFormat = \"@\"");
//
    macros.Cells(2, 3, data.size() + 1, 3, "Select");
    macros.InsertLine("Selection.HorizontalAlignment = xlLeft");
// выводим данные
    for (int i = 0; i < data.size(); i++)
    {
       sheet +=
            data[i]->index + ";" +
            /* data[i]->reg_num + */ ";" +
            data[i]->fio + ";" +
            data[i]->namespec + ";" +
            /* data[i]->kod_spec + */ ";" +
            data[i]->kod_kvalif + ";" +
            data[i]->namekvalif + ";" +
            curr_year + ";" +
            data[i]->otl + ";" +
            data[i]->serdip + ";" +
            /* data[i]->numdip + */ ";" +
            data[i]->datavid + ";" +
            data[i]->datagak + ";" +
            data[i]->numprotgak + ";" +
            data[i]->numotch + ";" +
            data[i]->dataotch + ";" +
            data[i]->grp + "\n";

        AnsiString pos_row = IntToStr(i+2);
        macros.Select("B" + pos_row);
        macros.Formula(data[i]->reg_num);
        macros.Select("E" + pos_row);
        macros.Formula(data[i]->kod_spec);
        macros.Select("K" + pos_row);
        macros.Formula(data[i]->numdip);
    }
    macros.InsertLine("ActiveWindow.ScrollRow = 1");
    macros.InsertLine("ActiveWindow.ScrollColumn = 1");

    // получаем текущее положение
    char path_buf[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];

    GetModuleFileName(NULL,path_buf,sizeof(path_buf)-1);
    _splitpath(path_buf,drive,dir,0,0);
    _makepath(path_buf,drive,dir,0,0);

    AnsiString filename = AnsiString(drive) + AnsiString(dir) + "forma3.csv";
    ofstream output_file(filename.c_str(), ios::in|ios::trunc);
    output_file.write(sheet.c_str(), sheet.Length());
    output_file.close();

    macros.EndMacros();
    macros.RunMacros(filename);

}
//---------------------------------------------------------------------------
void __fastcall TFormReportForma3::CreateData(void)
{
    AnsiString query;

    query = "set @year = '" + curr_year + "'";
    mysql_query(mysql, query.c_str());

    mysql_query(mysql, "CREATE TEMPORARY TABLE temp        "
        "(                                  "
        "  specid       int(11),            "
        "  name_sp      char(100),          "
        "  kod_spec     char(10),           "
        "  name_kval    char(100),          "
        "  INDEX (specid)                   "
        ") TYPE = HEAP;                     ");

    mysql_query(mysql, "INSERT temp (specid,name_sp,kod_spec,name_kval)    "
        "SELECT voc.num,voc.title, voc2.title,voc1.title    "
        "FROM voc, voc as voc1, voc as voc2                 "
        "WHERE                                              "
        "voc.vkey = 'spec' AND voc.deleted = 0 AND          "
        "voc1.num=voc.num AND voc1.vkey = 'qualific' AND voc1.deleted = 0 AND   "
        "voc2.num=voc.num AND voc2.vkey = 'shifrspec' AND voc2.deleted = 0      ");


    mysql_query(mysql, "SELECT st.`edunumreg`, CONCAT(st.`secondname`, ' ', st.`firstname`, ' ', st.`thirdname`) as fio,   "
        "temp.name_sp,temp.kod_spec,temp.name_kval,`edudiplomotl`, `edunumdiplom`,`edudatediplom`,           "
        "`edudatequalif`,`edunumprotgak`,`exitnum`,`exitdate`,voc.title                                                      "
        "FROM students as st, temp,voc                                                                       "
        "WHERE st.deleted = 0 AND YEAR(st.`exitdate`) = @year AND st.cityid != 0 AND temp.specid = st.specid "
        "AND voc.deleted = 0 AND voc.vkey='grp' AND voc.num = st.grpid ORDER BY voc.title, fio ");

    MYSQL_RES *result;
    MYSQL_ROW row;

    result = mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
        while (row = mysql_fetch_row(result))
        {
            st_forma3* d = new st_forma3;
            d->index = "19-65";
            d->reg_num = row[0];
            d->fio = row[1];
            d->namespec = row[2];
            AnsiString kod_spec = row[3];
            try {
              d->kod_spec = (kod_spec.Pos(".65")==0)?kod_spec:kod_spec.SubString(1, kod_spec.Length() - 3);
            } catch (...) { d->kod_spec = kod_spec; }
            d->kod_kvalif = "65";
            d->namekvalif = row[4];
            d->namekvalif = d->namekvalif.LowerCase();
            d->otl =  (AnsiString(row[5]) == "0")?"нет":"да";
            try {
              d->serdip = AnsiString(row[6]).SubString(1, 3);
            } catch (...) { d->serdip = row[6]; }
            try {
              d->numdip = AnsiString(row[6]).SubString(5, AnsiString(row[6]).Length()-4);
            } catch (...) { d->numdip = row[6]; }
            d->datavid = row[7];
            d->datagak = row[8];
            d->numprotgak = row[9];
            d->numotch = row[10];
            d->dataotch = row[11];
            d->grp = row[12];

            data.push_back(d);
        }
    }
    mysql_free_result(result);

    mysql_query(mysql, "drop table temp");
}
//---------------------------------------------------------------------------
void __fastcall TFormReportForma3::DestroyData(void)
{
    for (int i = 0; i < data.size(); i++)
        delete data[i];

    data.clear();
}
//---------------------------------------------------------------------------
