//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitIzmOchenka.h"
#include "UnitSelected.h"
#include <vector>

extern MYSQL* mysql;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormIzmOchenka *FormIzmOchenka;

AnsiString type_ochenka[] = {"Отл",
                            "Хор",
                            "Уд",
                            "Неуд",
                            "Зачет",
                            "Незач"};
//---------------------------------------------------------------------------
void __fastcall  TFormIzmOchenka::delete_data(void)
{
    for (int i = 0; i< progress.size(); i++)
        if ( NULL != progress[i])
            delete progress[i];
    progress.clear();
}
//---------------------------------------------------------------------------
__fastcall TFormIzmOchenka::TFormIzmOchenka(TComponent* Owner)
    : TForm(Owner),IDStudent(-1),IDGroup(-1)
{
    InitTable();

    Constraints->MinWidth=Width;
    Constraints->MinHeight=Height;
    Constraints->MaxWidth=Width;
    Constraints->MaxHeight=Height;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmOchenka::SetIDStudent(const int& _IDStudent)
{
    IDStudent = _IDStudent;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmOchenka::SetIDGroup(const int& _IDGroup)
{
    IDGroup = _IDGroup;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmOchenka::UpdateTable()
{
    delete_data();

    MYSQL_RES *result;
    MYSQL_ROW row;
    AnsiString query;
    query = " SELECT DISTINCT pr.numplansemestr, dis.fulltitle, pr.id, dis.shorttitle, pr.estimation, pr.ball,pr.iddiscip " \
            " FROM progress as pr,disciplines AS dis " \
            " WHERE pr.idstud = " + IntToStr(IDStudent) + " AND pr.deleted = 0 AND dis.id = pr.iddiscip AND dis.deleted = 0  " \
            " ORDER BY pr.numplansemestr, LEFT(dis.fulltitle,20) ";

    mysql_query(mysql,query.c_str());
    if (!mysql_field_count(mysql))
        return;
    result = mysql_store_result(mysql);
    if (!(result && mysql_num_rows(result)))
        return;

    StrGridDB->RowCount = 1;

    progress.push_back(NULL);

    while (row = mysql_fetch_row(result))
    {
        StrGridDB->Cells[0][StrGridDB->RowCount] = StrGridDB->RowCount;
        StrGridDB->Cells[1][StrGridDB->RowCount] = row[0];
        StrGridDB->Cells[2][StrGridDB->RowCount] = type_ochenka[AnsiString(row[4]).ToInt()];
        StrGridDB->Cells[3][StrGridDB->RowCount] = row[5];
        StrGridDB->Cells[4][StrGridDB->RowCount] = row[3];
        StrGridDB->Cells[5][StrGridDB->RowCount] = row[1];

        StrGridDB->RowCount++;

        data_progress *data = new data_progress;

        data->id_prog = AnsiString(row[2]).ToInt();
        data->estimation = AnsiString(row[4]).ToInt();
        data->iddisc = AnsiString(row[6]).ToInt();
        data->plansem = AnsiString(row[0]).ToInt();
        progress.push_back(data);
    }
    StrGridDB->FixedRows = 1;
    StrGridDBSelectCell(StrGridDB, 1, 1, false);
    mysql_free_result(result);
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmOchenka::InitTable()
{
    StrGridDB->Cells[0][0] = "№";                     StrGridDB->ColWidths[0] = 50;
    StrGridDB->Cells[1][0] = "План-семестр";          StrGridDB->ColWidths[1] = 80;

    StrGridDB->Cells[2][0] = "Тип";                   StrGridDB->ColWidths[2] = 80;
    StrGridDB->Cells[3][0] = "Оценка";                StrGridDB->ColWidths[3] = 80;

    StrGridDB->Cells[4][0] = "Предмет";               StrGridDB->ColWidths[4] = 160;
    StrGridDB->Cells[5][0] = "Предмет";               StrGridDB->ColWidths[5] = 250;

}
//---------------------------------------------------------------------------
void __fastcall TFormIzmOchenka::FormShow(TObject *Sender)
{
    InitTable();
    UpdateTable();
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmOchenka::StrGridDBSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    MaskEditPredmet->Text = StrGridDB->Cells[5][ARow];
  //  if (ARow < progress.size() && ARow >= 1)
  //      RadioGroupType->ItemIndex = progress[ARow]->estimation;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmOchenka::Button2Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmOchenka::Button1Click(TObject *Sender)
{
    int selrow = StrGridDB->Selection.Top;
    if (selrow != StrGridDB->Selection.Bottom || StrGridDB->Cells[1][selrow].IsEmpty())
    {
        MessageBox(Handle,"Таблица или выбранная запись пустая.\nВыберите непустую запись.","Сообщение:",MB_OK|MB_ICONINFORMATION);
        return;
    }

    AnsiString query;

    if (RadioGroupType->ItemIndex == -1)
    {
        MessageBox(Handle,"Это наглость","Сообщение:",MB_OK|MB_ICONINFORMATION);
        return;
    }
    if (RadioGroupType->ItemIndex == 0)
        query = " UPDATE students AS st,progress as pr " \
            " SET pr.estimation = CASE  " \
                " WHEN ball >= 2.5 AND ball < 3.5 then 2 " \
                " WHEN ball >= 3.5 AND ball < 4.5 then 1 " \
                " WHEN ball >= 4.5 AND ball <= 5.0 then 0 END " \
            " WHERE st.deleted = 0 AND st.grpid = " + IntToStr(IDGroup) + " AND " \
            " pr.idstud = st.id AND pr.numplansemestr = " + progress[selrow]->plansem + " AND pr.deleted = 0 " \
            " AND pr.iddiscip = " + progress[selrow]->iddisc + " AND pr.ball >= 2.5 ";
    else                           // у зачетов всегда зачет (они не ставят незачет)
        query = " UPDATE students AS st,progress as pr " \
            " SET pr.estimation = 4 " \
            " WHERE st.deleted = 0 AND st.grpid = " + IntToStr(IDGroup) + " AND " \
            " pr.idstud = st.id AND pr.numplansemestr = " + progress[selrow]->plansem + " AND pr.deleted = 0 " \
            " AND pr.iddiscip = " + progress[selrow]->iddisc + " AND pr.ball >= 2.5 ";

    mysql_query(mysql,query.c_str());
    UpdateTable();
    RadioGroupType->ItemIndex = -1;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmOchenka::FormDestroy(TObject *Sender)
{
    delete_data();
}
//---------------------------------------------------------------------------

