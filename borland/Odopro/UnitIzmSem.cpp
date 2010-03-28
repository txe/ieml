//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitIzmSem.h"
#include "UnitSelected.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormIzmSem *FormIzmSem;

extern MYSQL* mysql;

//---------------------------------------------------------------------------
__fastcall TFormIzmSem::TFormIzmSem(TComponent* Owner)
        : TForm(Owner),IDStudent(-1),IDGroup(-1)
{
    InitTable();

    Constraints->MinWidth=Width;
    Constraints->MinHeight=Height;
    Constraints->MaxWidth=Width;
    Constraints->MaxHeight=Height;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmSem::SetIDStudent(const int& _IDStudent)
{
    IDStudent = _IDStudent;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmSem::SetIDGroup(const int& _IDGroup)
{
    IDGroup = _IDGroup;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmSem::UpdateTable()
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    AnsiString query;
    query = " SELECT DISTINCT pr.numplansemestr, dis.fulltitle, dis.id, dis.shorttitle" \
            " FROM progress as pr,disciplines AS dis" \
            " WHERE pr.idstud = " + IntToStr(IDStudent) + " AND pr.deleted = 0 AND dis.id = pr.iddiscip AND dis.deleted = 0 " \
            " ORDER BY pr.numplansemestr, LEFT(dis.fulltitle,20)";

    mysql_query(mysql,query.c_str());
    if (!mysql_field_count(mysql))
        return;
    result = mysql_store_result(mysql);
    if (!(result && mysql_num_rows(result)))
        return;

    StrGridDB->RowCount = 1;
    while (row = mysql_fetch_row(result))
    {
        StrGridDB->Cells[0][StrGridDB->RowCount] = StrGridDB->RowCount;
        StrGridDB->Cells[1][StrGridDB->RowCount] = row[0];
        StrGridDB->Cells[2][StrGridDB->RowCount] = row[3];
        StrGridDB->Cells[3][StrGridDB->RowCount] = row[1];
        StrGridDB->Objects[0][StrGridDB->RowCount] = (TObject*)AnsiString(row[2]).ToInt();

        StrGridDB->RowCount++;
    }
    StrGridDB->FixedRows = 1;
    StrGridDBSelectCell(StrGridDB, 1, 1, false);
    mysql_free_result(result);
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmSem::InitTable()
{
    StrGridDB->Cells[0][0] = "№";                     StrGridDB->ColWidths[0] = 50;
    StrGridDB->Cells[1][0] = "План-семестр";          StrGridDB->ColWidths[1] = 80;
    StrGridDB->Cells[2][0] = "Предмет";               StrGridDB->ColWidths[2] = 160;
    StrGridDB->Cells[3][0] = "Предмет";               StrGridDB->ColWidths[3] = 250;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmSem::FormShow(TObject *Sender)
{
    InitTable();
    UpdateTable();
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmSem::StrGridDBSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
    MaskEditPredmet->Text = StrGridDB->Cells[3][ARow];
    MaskEditOldPlanSem->Text = StrGridDB->Cells[1][ARow];
    MaskEditNewPlanSem->Text = StrGridDB->Cells[1][ARow];
}
//---------------------------------------------------------------------------

void __fastcall TFormIzmSem::Button2Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmSem::Button1Click(TObject *Sender)
{

    if (MaskEditOldPlanSem->Text == MaskEditNewPlanSem->Text)
        return;

    int selrow = StrGridDB->Selection.Top;
    if (selrow != StrGridDB->Selection.Bottom || StrGridDB->Cells[1][selrow].IsEmpty())
    {
        MessageBox(Handle,"Таблица или выбранная запись пустая.\nВыберите непустую запись.","Сообщение:",MB_OK|MB_ICONINFORMATION);
        return;
    }

    int dis_id = (int)StrGridDB->Objects[0][selrow];
    AnsiString query;

    query = " UPDATE students AS st,progress as pr" \
    " SET pr.numplansemestr = " + MaskEditNewPlanSem->Text + \
    " WHERE st.deleted = 0 AND st.grpid = " + IntToStr(IDGroup) + " AND" \
    " pr.idstud = st.id AND pr.numplansemestr = " + MaskEditOldPlanSem->Text + " AND pr.deleted = 0" \
    " AND pr.iddiscip = " + IntToStr(dis_id);

    mysql_query(mysql,query.c_str());
    UpdateTable();
}
//---------------------------------------------------------------------------
