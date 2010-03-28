//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitInsertVoc.h"
#include "UnitSelected.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma resource "*.dfm"
TFormInsertVoc *FormInsertVoc;

extern AnsiString TabVocName;
extern MYSQL* mysql;
extern AnsiString keys[15];
extern AnsiString Hintkeys[15];
#define DEFAULT_INDEX_SORT -1
AnsiString VocKeys[3]={"vkey","num","title"};
AnsiString titles[3]={"Ключ","Номер","Название"};
int SortIndex;
bool flagSortUp;
TStringList* listIDsVoc;

AnsiString ToStr(AnsiString str);


//---------------------------------------------------------------------------
AnsiString GetKeyForHint(AnsiString hint)
{
  int i=0;
  while (i<15)
  {
    if (hint==Hintkeys[i]) break;
    i++;
  }
  i=(i==15) ? 0 : i;
  return keys[i];
}
//---------------------------------------------------------------------------
AnsiString GetHintForKey(AnsiString key)
{
  int i=0;
  while (i<15)
  {
    if (key==keys[i]) break;
    i++;
  }
  i=(i==15) ? 0 : i;
  return Hintkeys[i];
}
//---------------------------------------------------------------------------
void InitTitleGrid(TStringGrid* grid,int indCol,int indRow,AnsiString Str)
{
  grid->Cells[indCol][indRow]=Str;
}
//---------------------------------------------------------------------------
void InitTitlesGrid(TStringGrid* grid,int count,int indRow,int startindCol,AnsiString* masStr)
{
  for (int i=0;i<count;i++) grid->Cells[startindCol+i][indRow]=masStr[i];
}
//---------------------------------------------------------------------------
__fastcall TFormInsertVoc::TFormInsertVoc(TComponent* Owner)
        : TForm(Owner)
{
  listIDsVoc=new TStringList;
  listIDsVoc->Clear();
  listIDsVoc->Add("NULL");

  InitTitleGrid(StrGridVoc,0,0,titles[0]);     StrGridVoc->ColWidths[0]=100;
  InitTitleGrid(StrGridVoc,1,0,titles[1]);     StrGridVoc->ColWidths[1]=50;
  InitTitleGrid(StrGridVoc,2,0,titles[2]);     StrGridVoc->ColWidths[2]=250;

  ComboBoxKey->Clear();
  ComboBoxKey->Items->Add("все");
  for (int i=0;i<15;i++) ComboBoxKey->Items->Add(Hintkeys[i]);

  Constraints->MinWidth=Width;
  Constraints->MinHeight=Height;
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::InitTable(AnsiString WHEREStr,AnsiString KeyOrder)
{
       MYSQL_RES *result;
       MYSQL_ROW row;

       AnsiString myquery="SELECT id,vkey,num,title FROM "+TabVocName+" "+WHEREStr+" ORDER BY "+KeyOrder;
       mysql_query(mysql,myquery.c_str());
       Memo1->Lines->Add(mysql_error(mysql));
       int numCurRow;
       if (mysql_field_count(mysql))
       {
         Memo1->Lines->Add(mysql_error(mysql));
         result=mysql_store_result(mysql);
         Memo1->Lines->Add(mysql_error(mysql));
         if (result && mysql_num_rows(result))
         {
           Memo1->Lines->Add(mysql_error(mysql));

           numCurRow=0;
           listIDsVoc->Clear();
           listIDsVoc->Add("NULL");
           while (row = mysql_fetch_row(result))
           {
                Memo1->Lines->Add(mysql_error(mysql));

                numCurRow++;
                StrGridVoc->RowCount=numCurRow+1;

                listIDsVoc->Add(AnsiString(row[0]));
                StrGridVoc->Cells[0][numCurRow]=row[1];
                StrGridVoc->Cells[1][numCurRow]=row[2];
                StrGridVoc->Cells[2][numCurRow]=row[3];
           }
         }
         mysql_free_result(result);
         Memo1->Lines->Add(mysql_error(mysql));
       }
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::ClearTable(void)
{
  StrGridVoc->RowCount=2;
  for (int i=0;i<3;i++) StrGridVoc->Cells[i][1]="";
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::ComboBoxKeyChange(TObject *Sender)
{
  ClearTable();
  AnsiString vkey;
  vkey=(ComboBoxKey->ItemIndex==0)? AnsiString("WHERE deleted=0") : AnsiString("WHERE vkey=\'"+GetKeyForHint(ComboBoxKey->Text)+"\' AND deleted=0");
  if (SortIndex==DEFAULT_INDEX_SORT) SortIndex=1;
  AnsiString Order=VocKeys[SortIndex];
  if (!flagSortUp) // По убыванию
     Order=Order+" DESC";
  InitTable(vkey,Order);
  AnsiString arrowStr;
  if (flagSortUp) arrowStr="\\/";else arrowStr="/\\";
  InitTitlesGrid(StrGridVoc,3,0,0,titles);
  InitTitleGrid(StrGridVoc,SortIndex,0,titles[SortIndex]+"  "+arrowStr);
  StrGridVoc->OnClick(StrGridVoc);
  if (ComboBoxKey->ItemIndex==0)
  {
    Edittitle->Enabled=false;
    BtnAdd->Enabled=false;
    //BtnDel->Enabled=false;
    BtnEdit->Enabled=false;
  }
  else
  {
    Edittitle->Enabled=true;
    BtnAdd->Enabled=true;
    //BtnDel->Enabled=true;
    BtnEdit->Enabled=true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::FormShow(TObject *Sender)
{
  SortIndex=DEFAULT_INDEX_SORT;
  flagSortUp=true;
  ComboBoxKey->ItemIndex=ComboBoxKey->Items->IndexOf(Hintkeys[CITIES]);
  ComboBoxKeyChange(FormInsertVoc);
  FormInsertVoc->OnResize(FormInsertVoc);
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::BtnCloseClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
bool __fastcall TFormInsertVoc::IsExistsTitleForKey(AnsiString vkey,AnsiString title)
{
       bool status=false;

       MYSQL_RES *result;
       MYSQL_ROW row;

       AnsiString myquery="SELECT title FROM "+TabVocName+" WHERE vkey="+ToStr(vkey)+" AND title="+ToStr(title)+" AND deleted=0";
       mysql_query(mysql,myquery.c_str());
       Memo1->Lines->Add(mysql_error(mysql));
       if (mysql_field_count(mysql))
       {
         Memo1->Lines->Add(mysql_error(mysql));
         result=mysql_store_result(mysql);
         Memo1->Lines->Add(mysql_error(mysql));
         if (result && mysql_num_rows(result))
         {
           Memo1->Lines->Add(mysql_error(mysql));

           status=true;
         }
         mysql_free_result(result);
         Memo1->Lines->Add(mysql_error(mysql));
       }
       return status;
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::BtnAddClick(TObject *Sender)
{
  if (IsExistsTitleForKey(GetKeyForHint(ComboBoxKey->Text),Edittitle->Text))
  {
    AnsiString msg="Запись по ключу \""+GetKeyForHint(ComboBoxKey->Text)+"\" с названием \""+Edittitle->Text+"\" уже существует в словаре.\nДобавления не произошло.";
    MessageBox(Handle,msg.c_str(),"предупреждение:",MB_OK|MB_ICONINFORMATION);
    return;
  }

  int nextNum=GetCountRecForKey(GetKeyForHint(ComboBoxKey->Text))+1;
  AnsiString myquery="INSERT INTO "+TabVocName+" (num,vkey,title) VALUES("+ToStr(AnsiString(nextNum))+","+ToStr(GetKeyForHint(ComboBoxKey->Text))+","+ToStr(Edittitle->Text)+")";

  mysql_query(mysql,myquery.c_str());
  Memo1->Lines->Add(mysql_error(mysql));
  ComboBoxKeyChange(BtnAdd);
}
//---------------------------------------------------------------------------
int __fastcall TFormInsertVoc::GetCountRecForKey(AnsiString vkey)
{
  int resCount=0;

  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString myquery="SELECT MAX(num) FROM "+TabVocName+" WHERE vkey="+ToStr(vkey)+" AND deleted=0";
  mysql_query(mysql,myquery.c_str());
  Memo1->Lines->Add(mysql_error(mysql));
  if (mysql_field_count(mysql))
  {
    Memo1->Lines->Add(mysql_error(mysql));
    result=mysql_store_result(mysql);
    Memo1->Lines->Add(mysql_error(mysql));
    if (result && mysql_num_rows(result))
    {
      Memo1->Lines->Add(mysql_error(mysql));

      row = mysql_fetch_row(result);
      if (row[0]) resCount=AnsiString(row[0]).ToInt();
      else resCount=0;
      Memo1->Lines->Add(mysql_error(mysql));
    }
    mysql_free_result(result);
    Memo1->Lines->Add(mysql_error(mysql));
  }
  return resCount;
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::BtnDelClick(TObject *Sender)
{
  int selrow=StrGridVoc->Selection.Top;
  if (selrow!=StrGridVoc->Selection.Bottom || StrGridVoc->Cells[1][selrow].IsEmpty())
  {
    MessageBox(Handle,"Таблица или выбранная запись пустая.\nВыберите непустую запись.","Сообщение:",MB_OK|MB_ICONINFORMATION);
    return;
  }

  int idselected;
  AnsiString myquery;
  AnsiString msg="Вы действительно хотите удалить эту запись?";
  if (MessageBox(Handle,msg.c_str(),"Сообщение:",MB_YESNO|MB_ICONQUESTION)==ID_YES)
  {
    idselected=listIDsVoc->Strings[StrGridVoc->Selection.Top].ToInt();
    myquery="UPDATE "+TabVocName+" SET deleted=1 WHERE id="+AnsiString(idselected);

    mysql_query(mysql,myquery.c_str());
    Memo1->Lines->Add(mysql_error(mysql));
    ComboBoxKeyChange(BtnDel);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::FormKeyPress(TObject *Sender, char &Key)
{
/*  if (Key==VK_INSERT) BtnAdd->OnClick(BtnAdd);
  if (Key==VK_DELETE) BtnDel->OnClick(BtnDel);    */
  if (Key==VK_ESCAPE) BtnClose->OnClick(BtnClose);
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::FormResize(TObject *Sender)
{
  int size=StrGridVoc->ClientWidth-20,
      sizekey=size*15/100,
      sizenum=size*10/100,
      sizetitle=size-sizekey-sizenum;
  StrGridVoc->ColWidths[0]=sizekey;
  StrGridVoc->ColWidths[1]=sizenum;
  StrGridVoc->ColWidths[2]=sizetitle;
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::StrGridVocDblClick(TObject *Sender)
{
  int selrow=StrGridVoc->Selection.Top;
  AnsiString msg="Запись словаря:\n\nКлюч: "+StrGridVoc->Cells[0][selrow]+"\nНомер: "+StrGridVoc->Cells[1][selrow]+"\nНазвание: "+StrGridVoc->Cells[2][selrow];
  MessageBox(Handle,msg.c_str(),"Сообщение:",MB_OK/*|MB_ICONINFORMATION*/);
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::StrGridVocClick(TObject *Sender)
{
  int selrow=StrGridVoc->Selection.Top;
  if (selrow==StrGridVoc->Selection.Bottom)
  {
    Edittitle->Text=StrGridVoc->Cells[2][selrow];
    if (Edittitle->Enabled) Edittitle->SetFocus();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::BtnEditClick(TObject *Sender)
{
  int selrow=StrGridVoc->Selection.Top;
  if (selrow!=StrGridVoc->Selection.Bottom || StrGridVoc->Cells[1][selrow].IsEmpty())
  {
    MessageBox(Handle,"Таблица или выбранная запись пустая.\nВыберите непустую запись.","Сообщение:",MB_OK|MB_ICONINFORMATION);
    return;
  }

  int idselected;
  AnsiString myquery;
  AnsiString msg="Все записи, содержащие данное значение в своих полях будут изменены!\nВы действительно хотите редактировать эту запись словаря?";
  if (MessageBox(Handle,msg.c_str(),"Сообщение:",MB_YESNO|MB_ICONQUESTION)==ID_YES)
  {
    idselected=listIDsVoc->Strings[StrGridVoc->Selection.Top].ToInt();
    myquery="UPDATE "+TabVocName+" SET title="+ToStr(Edittitle->Text)+" WHERE id="+AnsiString(idselected);

    mysql_query(mysql,myquery.c_str());
    Memo1->Lines->Add(mysql_error(mysql));
    ComboBoxKeyChange(BtnDel);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormInsertVoc::StrGridVocMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  // Sorting
  TGridCoord Coord=StrGridVoc->MouseCoord(X,Y);
  if(Coord.Y==0)
  {
    SortIndex=Coord.X;
    flagSortUp=!flagSortUp;
    ComboBoxKeyChange(StrGridVoc);
  }
}
//---------------------------------------------------------------------------

