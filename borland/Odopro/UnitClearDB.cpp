//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitClearDB.h"
#include "UnitSelected.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormClearDB *FormClearDB;

extern MYSQL* mysql;
//---------------------------------------------------------------------------
__fastcall TFormClearDB::TFormClearDB(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormClearDB::BtnDelClick(TObject *Sender)
{
  MYSQL_RES *result;
  MYSQL_ROW row;

  EDRegOpts opts;
  TStringList* listTabs=new TStringList;
  if (CheckListBoxTabs->Checked[0]) listTabs->Add(opts.DBStudTable);
  if (CheckListBoxTabs->Checked[1]) listTabs->Add(opts.DBVocTable);
  if (CheckListBoxTabs->Checked[2]) listTabs->Add(opts.DBPayOpts);
  if (CheckListBoxTabs->Checked[3]) listTabs->Add(opts.DBPayFacts);
  if (CheckListBoxTabs->Checked[4]) listTabs->Add(opts.DBDisciplines);
  if (CheckListBoxTabs->Checked[5]) listTabs->Add(opts.DBProgress);
  AnsiString Info="В выбранных таблицах следующие записи, помечены на удаление:\n";
  AnsiString query="";
  int i;
  for (i=0;i<listTabs->Count;i++)
  {
    query="SELECT COUNT(*) FROM "+listTabs->Strings[i]+" WHERE deleted=1";
    mysql_query(mysql,query.c_str());
    if (mysql_field_count(mysql))
    {
      result=mysql_store_result(mysql);
      if (result && mysql_num_rows(result)==1) // could be one record
      {
        row = mysql_fetch_row(result);
        Info+="\t\`"+listTabs->Strings[i]+"\` ( "+AnsiString(row[0])+" зап. )\n";
      }
      mysql_free_result(result);
    }
  }
  Info+="\nВы действительно хотите удалить эти записи?";
  if (MessageBox(Handle,Info.c_str(),"удаление",MB_YESNO|MB_ICONQUESTION)==ID_YES)
  {
    for (i=0;i<listTabs->Count;i++)
    {
      query="DELETE FROM "+listTabs->Strings[i]+" WHERE deleted=1";
      mysql_query(mysql,query.c_str());
    }
    MessageBox(Handle,"Готово!","ok",MB_OK);
  }

  delete listTabs;
}
//---------------------------------------------------------------------------
void __fastcall TFormClearDB::FormShow(TObject *Sender)
{
  CheckListBoxTabs->Clear();
  CheckListBoxTabs->Items->Add("Личные данные студентов");
  CheckListBoxTabs->Items->Add("Словарь терминов");
  CheckListBoxTabs->Items->Add("Категории оплаты");
  CheckListBoxTabs->Items->Add("Факты оплаты");
  CheckListBoxTabs->Items->Add("Дисциплины");
  CheckListBoxTabs->Items->Add("Успеваемость студентов");
}
//---------------------------------------------------------------------------
