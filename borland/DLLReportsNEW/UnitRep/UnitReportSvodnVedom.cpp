//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportSvodnVedom.h"
#include "UnitInfoOcenk.h" 
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

TFormReportSvodnVedom *FormReportSvodnVedom;

extern EDRegOpts opts;

extern AnsiString arrOcenk[];
extern AnsiString arrShortOcenk[];
extern const CountOcenk=6;

extern EDRegOpts opts;
extern MYSQL *mysql;

//---------------------------------------------------------------------------
__fastcall TFormReportSvodnVedom::TFormReportSvodnVedom(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSvodnVedom::SetIDGroup(int _IDGroup)
{
  idgroup=_IDGroup;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportSvodnVedom::GetIDGroup(void)
{
  return idgroup;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSvodnVedom::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSvodnVedom::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSvodnVedom::CreateWordDocument(void)
{
    WordMacros macros;
    macros.BeginMacros();

    macros.InsertLine("ActiveDocument.PageSetup.Orientation = wdOrientLandscape");
    macros.InsertLine("ActiveDocument.PageSetup.TopMargin = 25");
    macros.InsertLine("ActiveDocument.PageSetup.BottomMargin = 25");
    macros.InsertLine("ActiveDocument.PageSetup.LeftMargin = 25");
    macros.InsertLine("ActiveDocument.PageSetup.RightMargin = 25");

    AnsiString GroupStr,SemestrGraphicStr;
    GroupStr = WCGetTitleForKeyNum(GROUPS,idgroup);
    SemestrGraphicStr = AnsiString(pFormSemestr->GetSemestrGraphic());

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionFont("Size=20");
    macros.SelectionFont("Bold=true");
    macros.SelectionText("Сводная ведомость ");
    macros.SelectionFont("Size=10");
    macros.SelectionFont("Bold=false");

    TDateTime CurDate=Date();

    macros.SelectionText("(на "+CurDate.DateString()+")");
    macros.SelectionTypeParagraph();
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.SelectionText("Семестр по графику: ");
    macros.SelectionFont("Bold=true");
    macros.SelectionText(SemestrGraphicStr);
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold=false");
    macros.SelectionText("Группа: ");
    macros.SelectionFont("Bold=true");
    macros.SelectionText(GroupStr);
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold=false");

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");

    AnsiString Str,Strtmp;

    macros.SelectionText("");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionText("Cпециальность \"");
    macros.SelectionFont("Bold=true");
    macros.SelectionText(GetSpec());
    macros.SelectionText("\"");
    macros.SelectionFont("Bold=false");
    macros.SelectionTypeParagraph();

    macros.TablesAdd(fio.size() + 1, dis.size() + 2);

    // размеры
    macros.TablesColumns( 1, 1, "Width = 20");
    macros.TablesColumns( 1, 2, "Width = 190");

    int othersize = 590;
    AnsiString sizeDiscip = AnsiString(othersize/dis.size());

    for (int i = 0; i < dis.size(); i++)
        macros.TablesColumns( 1, 3 + i, "Width = " + sizeDiscip);
    macros.TablesColumns( 1, 2 + dis.size(), "Width = " + IntToStr(othersize-sizeDiscip.ToInt()*(dis.size()-1)));
    // вид

    macros.InsertLine("ActiveDocument.Tables.Item(1).Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.InsertLine("ActiveDocument.Tables.Item(1).Range.Cells.VerticalAlignment = wdCellAlignVerticalCenter");
  
    macros.TablesColumns( 1, 1, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.TablesColumns( 1, 2, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");

  //  macros.InsertLine("ActiveDocument.Tables.Item(1).Range.Colonus

 /*   for (int i = 0; i < dis.size(); i++)
    {
        macros.TablesColumns( 1, 3 + i, "Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    }
   */
    macros.InsertLine("ActiveDocument.Tables.Item(1).Range.Font.Size = 9");
    macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(1).Range.Font.Bold = true");
//          WordDocument->Tables->Item(CountTables)->Range->Select();

    macros.TablesCell( 1, 1, 1, "Range.Text = \"№\"");
    macros.TablesCell( 1, 1, 1, "VerticalAlignment = wdCellAlignVerticalCenter");
    macros.TablesCell( 1, 1, 1, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell( 1, 1, 2, "Range.Text = \"Фамилия, имя, отчество\"");
    macros.TablesCell( 1, 1, 2, "VerticalAlignment = wdCellAlignVerticalCenter");
    macros.TablesCell( 1, 1, 2, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");

    for (int i = 0; i < dis.size(); i++)
            macros.TablesCell( 1, 1, 3 + i, "Range.Text = \"" + dis[i] + "\"");

    AnsiString type[] = {"отл" ,"хор",
        "уд", "неуд", "зач", "незач", "???"};

    for (int i = 0; i < fio.size(); i++)
    {
        macros.TablesCell( 1, i + 2, 1, "Range.Text = \"" + AnsiString(i + 1) + "\"");
        macros.TablesCell( 1, i + 2, 2, "Range.Text = \"" + fio[i] + "\"");
    }
    for (int pos = 0; pos < data_ochenka.size(); pos++)
    {
            AnsiString text = type[data_ochenka[pos]->estimat] + "(" + data_ochenka[pos]->ball+ ")";

         //   macros.TablesCell( 1, i + 2, 3 + j, "Range.Text = \"" +  text + "\"");
            macros.TablesCell( 1, 2 + pos_fio[data_ochenka[pos]->idstud], 3 + pos_dis[data_ochenka[pos]->iddiscip], "Range.Text = \"" +  text + "\"");

      /*      AnsiString t = AnsiString(fio[i]->idstud) + " = " + AnsiString(data_ochenka[pos]->idstud);
            if (fio[i]->idstud != data_ochenka[pos]->idstud) // неправильный вывод
                (--pos)++;//throw -1;
            */
    }

    macros.TablesCell( 1, 1, 1, "Range.Select");
    macros.InsertLine("Selection.MoveDown Unit := wdLine");

    macros.EndMacros();
    macros.RunMacros();

}
//---------------------------------------------------------------------------
void __fastcall TFormReportSvodnVedom::ToolBtnPreviewClick(
      TObject *Sender)
{
  try
  {
    pFormSemestr->ShowModal();
    int semestr_gr=pFormSemestr->GetSemestrGraphic();
    if (semestr_gr==-1)
        return; // Не выбрана категория оплаты

    CreateData();
    CreateWordDocument();
    DestroyData();

  }
  catch(...)
  {
    Application->MessageBox("Работа с MS Word не возможна. Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSvodnVedom::FormCreate(TObject *Sender)
{
  pFormSemestr=new TFormSelectSemestr(Application);

  if ( !WCConnect())
  {
    AnsiString msg="Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSvodnVedom::FormDestroy(TObject *Sender)
{
  pFormSemestr->Free();
  WCDisconnect();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSvodnVedom::CreateData()
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    AnsiString query;
    query = " SET @grpid = " + AnsiString(idgroup)+ " ";
    mysql_query(mysql, query.c_str());
    query = " SET @graphsem = " + AnsiString(pFormSemestr->GetSemestrGraphic()) + " ";
    mysql_query(mysql, query.c_str());

    mysql_query(mysql, " CREATE TEMPORARY TABLE temp_stud TYPE = HEAP "
        " SELECT st.id, st.firstname, st.secondname, st.thirdname "
        " FROM students as st "
        " WHERE  st.grpid = @grpid AND st.deleted = 0 ");

    mysql_query(mysql, " CREATE TEMPORARY TABLE temp_progress TYPE = HEAP "
        " SELECT pr.*, st.secondname "
        " FROM progress as pr, temp_stud  as st, disciplines as dis "
        " WHERE  pr.idstud = st.id AND pr.deleted = 0 AND pr.numgraphsemestr = @graphsem "
        " AND dis.id = pr.iddiscip AND dis.deleted = 0 ");
// считываем дисциплины

    mysql_query(mysql, " SELECT LEFT(shorttitle, 50), temp_progress.iddiscip"
        " FROM temp_progress, disciplines "
        " WHERE disciplines.id = temp_progress.iddiscip AND disciplines.deleted = 0 "
        " GROUP BY temp_progress.iddiscip "
        " ORDER BY temp_progress.iddiscip");
    int pos;
    pos = 0;
    result = mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
        while (row = mysql_fetch_row(result))
        {
            pos_dis[AnsiString(row[1]).ToInt()] = pos;
            pos++;
            dis.push_back(row[0]);
        }

    mysql_free_result(result);
// считываем фамилии
    mysql_query(mysql, " SELECT CONCAT(secondname,\' \',firstname,\' \',thirdname) as fullname, id "
        " FROM temp_stud "
        " ORDER BY secondname, id ");

    pos = 0;
    result = mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
        while (row = mysql_fetch_row(result))
        {
            pos_fio[AnsiString(row[1]).ToInt()] = pos;
            pos++;
            fio.push_back(row[0]);
        }
    mysql_free_result(result);
// считываем оценки
    mysql_query(mysql, " SELECT estimation, ball, idstud, iddiscip "
        " FROM temp_progress "
        " "
        " ORDER BY secondname, idstud, iddiscip ");

    result = mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
        while (row = mysql_fetch_row(result))
        {
            ochenka* data = new ochenka;
            try{
                data->estimat = AnsiString(row[0]).ToInt();
            }
            catch(...)
            {
                 data->estimat = 6;
            }
           // data->ball = ;
            TReplaceFlags flags;
            flags << rfReplaceAll << rfIgnoreCase;
            data->ball = StringReplace(AnsiString(row[1]).SubString(1,3), ".", ",", flags);
            data->idstud = AnsiString(row[2]).ToInt();
            data->iddiscip = AnsiString(row[3]).ToInt();

            data_ochenka.push_back(data);
        }
    }
    mysql_free_result(result);
// удалим таблицы
    mysql_query(mysql, " drop temporary table if exists temp_stud ");
    mysql_query(mysql, " drop temporary table if exists temp_progress ");
    mysql_query(mysql, " drop temporary table if exists temp_dis ");
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSvodnVedom::DestroyData()
{
    dis.clear();
    fio.clear();
    for (int i = 0; i< data_ochenka.size(); i++)
        delete data_ochenka[i];
    data_ochenka.clear();

    pos_fio.clear();
    pos_dis.clear();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormReportSvodnVedom::GetSpec()
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    AnsiString query;
    AnsiString spec;

    query = "SELECT specid FROM students WHERE deleted = 0 AND grpid = " + IntToStr(GetIDGroup())
        + " LIMIT 1";
    mysql_query(mysql, query.c_str());
    result = mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
        row = mysql_fetch_row(result);
        spec = WCGetTitleForKeyNum(SPECS, AnsiString(row[0]).ToInt());
    }
    mysql_free_result(result);
    return spec;
}
//---------------------------------------------------------------------------

