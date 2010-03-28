//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitIzmGraphSem.h"
#include "UnitSelected.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormIzmGraphSem *FormIzmGraphSem;
extern MYSQL* mysql;

//---------------------------------------------------------------------------
__fastcall TFormIzmGraphSem::TFormIzmGraphSem(TComponent* Owner)
        : TForm(Owner),IDStudent(-1),IDGroup(-1)
{
    InitTable();

    Constraints->MinWidth=Width;
    Constraints->MinHeight=Height;
    Constraints->MaxWidth=Width;
    Constraints->MaxHeight=Height;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmGraphSem::SetIDStudent(const int& _IDStudent)
{
    IDStudent = _IDStudent;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmGraphSem::SetIDGroup(const int& _IDGroup)
{
    IDGroup = _IDGroup;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmGraphSem::UpdateTable()
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    AnsiString query;
    query = " SELECT DISTINCT pr.numgraphsemestr, dis.fulltitle, dis.id, dis.shorttitle" \
            " FROM progress as pr,disciplines AS dis" \
            " WHERE pr.idstud = " + IntToStr(IDStudent) + " AND pr.deleted = 0 AND dis.id = pr.iddiscip AND dis.deleted = 0 " \
            " ORDER BY pr.numgraphsemestr, LEFT(dis.fulltitle,20)";

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
void __fastcall TFormIzmGraphSem::InitTable()
{
    StrGridDB->Cells[0][0] = "№";                     StrGridDB->ColWidths[0] = 50;
    StrGridDB->Cells[1][0] = "Граф-семестр";          StrGridDB->ColWidths[1] = 80;
    StrGridDB->Cells[2][0] = "Предмет";               StrGridDB->ColWidths[2] = 160;
    StrGridDB->Cells[3][0] = "Предмет";               StrGridDB->ColWidths[3] = 250;
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmGraphSem::StrGridDBSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    MaskEditPredmet->Text = StrGridDB->Cells[3][ARow];
    MaskEditOldGraphSem->Text = StrGridDB->Cells[1][ARow];
    MaskEditNewGraphSem->Text = StrGridDB->Cells[1][ARow];
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmGraphSem::Button2Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormIzmGraphSem::Button1Click(TObject *Sender)
{

    if (MaskEditOldGraphSem->Text == MaskEditNewGraphSem->Text)
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
    " SET pr.numgraphsemestr = " + MaskEditNewGraphSem->Text + \
    " WHERE st.deleted = 0 AND st.grpid = " + IntToStr(IDGroup) + " AND" \
    " pr.idstud = st.id AND pr.numgraphsemestr = " + MaskEditOldGraphSem->Text + " AND pr.deleted = 0" \
    " AND pr.iddiscip = " + IntToStr(dis_id);

    mysql_query(mysql,query.c_str());
    UpdateTable();
}
//---------------------------------------------------------------------------

void __fastcall TFormIzmGraphSem::FormShow(TObject *Sender)
{
    InitTable();
    UpdateTable();    
}
//---------------------------------------------------------------------------

