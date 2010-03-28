//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitEditDisciplines.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDisciplines *FormDisciplines;

TStringList* listIDs;

extern EDRegOpts opts;
extern MYSQL *mysql;

const int idclass_common = 1;
const int idclass_kurwork = 2;
const int idclass_kurproj = 3;
const int idclass_praktica = 4;
const int idclass_itogattest = 5;
const int idclass_VKR = 6;
const int idclass_addition = 7;

#define IS_WEEKS(idclass) \
  ((idclass == idclass_VKR) || (idclass == idclass_praktica))

//---------------------------------------------------------------------------
__fastcall TFormDisciplines::TFormDisciplines(TComponent* Owner)
        : TForm(Owner)
{
  listIDs=new TStringList;

  Constraints->MinWidth=Width;
  Constraints->MinHeight=Height;

  StrGridDiscip->Cells[0][0]="№п/п";
  StrGridDiscip->Cells[1][0]="Полное наименование";
  StrGridDiscip->Cells[2][0]="Абривиатура";
  StrGridDiscip->Cells[3][0]="Часов/недель";
  StrGridDiscip->Cells[4][0]="Назначение";
  StrGridDiscip->Cells[5][0]="Пор. №";

  ComboBoxDiscipline->Clear();
  ComboBoxAbr->Clear();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormDisciplines::GetSpecForStudID(int _idstud)
{
  AnsiString SpecStr="???";
  int myidspec=-1;

  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString query="SELECT specid FROM "+opts.DBStudTable+" WHERE deleted=0 AND id="+AnsiString(_idstud);
  mysql_query(mysql,query.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      row = mysql_fetch_row(result);
      myidspec=AnsiString(row[0]).ToInt();
    }
    mysql_free_result(result);
  }
  if (myidspec!=-1) SpecStr=WCGetTitleForKeyNum(SPECS,myidspec);
  return SpecStr;
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::FormResize(TObject *Sender)
{
  int size=StrGridDiscip->ClientWidth-20,
      sizeindex=size*7/100,
      sizeFull=size*35/100,
      sizeShort=size*15/100,
      sizeNumHours=size*13/100,
      sizeClass=size*20/100,
      sizeScanNum=size*10/100;
  StrGridDiscip->ColWidths[0]=sizeindex;
  StrGridDiscip->ColWidths[1]=sizeFull;
  StrGridDiscip->ColWidths[2]=sizeShort;
  StrGridDiscip->ColWidths[3]=sizeNumHours;
  StrGridDiscip->ColWidths[4]=sizeClass;
  StrGridDiscip->ColWidths[5]=sizeScanNum;
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::BtnCloseClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::BtnAddClick(TObject *Sender)
{
  EditNumHoursExit(BtnAdd);
  EditScanNumberExit(BtnAdd);

  AnsiString idspec=AnsiString(WCGetNumForKeyTitle(SPECS,ComboBoxSpec->Text));
  AnsiString ft=ComboBoxDiscipline->Text;
  AnsiString st=ComboBoxAbr->Text;

  int findid;
  AnsiString findspec,findfulltitle,findshorttitle;
  if (IsExistsRecordDiscip(findid,findspec,findfulltitle,findshorttitle))
  {
    AnsiString msg="Запись с данными:\n\n\tспециальность:  "+findspec+"\n\tдисциплина:  "+findfulltitle+"\n\tсокращенное название: "+findshorttitle+"\n\nуже существует в базе данных.\nДобавления не произошло!";
    MessageBox(Handle,msg.c_str(),"Сообщение:",MB_OK|MB_ICONINFORMATION);
    return;
  }

  AnsiString myquery="INSERT INTO "+opts.DBDisciplines+" (idspec,fulltitle,shorttitle,idclass,num_hours,scan_number,sem_hours) VALUES("+ToStr(idspec)+","+ToStr(ft)+","+ToStr(st)+","+ToStr(AnsiString(WCGetNumForKeyTitle(DISCIPCLASSIFIC,ComboBoxClassDiscip->Text)))+","+ToStr(AnsiString(UpDownNumHours->Position))+","+ToStr(AnsiString(UpDownScanNumber->Position))+","+ToStr(AudHoursToString())+")";
  mysql_query(mysql,myquery.c_str());

  AnsiString DiscipStr=ComboBoxDiscipline->Text;
  AnsiString AbrStr=ComboBoxAbr->Text;
  if (!DiscipStr.IsEmpty() && ComboBoxDiscipline->Items->IndexOf(DiscipStr)==-1)
  {
    ComboBoxDiscipline->Items->Insert(0,DiscipStr);
    ComboBoxDiscipline->ItemIndex=0;
  }
  if (!AbrStr.IsEmpty() && ComboBoxAbr->Items->IndexOf(AbrStr)==-1)
  {
    ComboBoxAbr->Items->Insert(0,AbrStr);
    ComboBoxAbr->ItemIndex=0;
  }

  ComboBoxSpecChange(ComboBoxSpec);
  StrGridDiscipClick(StrGridDiscip);
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::BtnDelClick(TObject *Sender)
{
  EditNumHoursExit(BtnDel);
  EditScanNumberExit(BtnDel);

  int selrow=StrGridDiscip->Selection.Top;
  if (selrow!=StrGridDiscip->Selection.Bottom || StrGridDiscip->Cells[1][selrow].IsEmpty())
  {
    MessageBox(Handle,"Таблица или выбранная запись пустая либо выбрано более одной записи.\nВыберите непустую запись.","Сообщение:",MB_OK|MB_ICONINFORMATION);
    return;
  }

  AnsiString myquery;
  AnsiString msg="Вы действительно хотите удалить эту запись?\nВедь при её удалении пропадут некоторые данные о студентах,\nа при попытке исправить ситуацию и внести запись с теми же данными структура базы не восстановится!\nБудьте аккуратны!";
  int idselected;
  if (MessageBox(Handle,msg.c_str(),"Сообщение:",MB_YESNO|MB_ICONQUESTION)==ID_YES)
  {
    idselected=listIDs->Strings[StrGridDiscip->Selection.Top].ToInt();
    myquery="UPDATE "+opts.DBDisciplines+" SET deleted=1 WHERE id="+AnsiString(idselected);
    mysql_query(mysql,myquery.c_str());

    ComboBoxSpecChange(ComboBoxSpec);
    StrGridDiscipClick(StrGridDiscip);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::BtnEditClick(TObject *Sender)
{
  EditNumHoursExit(BtnEdit);
  EditScanNumberExit(BtnEdit);

  int selrow=StrGridDiscip->Selection.Top;
  int idselected;
  if (selrow!=StrGridDiscip->Selection.Bottom || StrGridDiscip->Cells[0][selrow].IsEmpty())
  {
    AnsiString msg="Запись не выбрана либо выбрано более 1 записи.\nИсправление не произошло!\nЕсли таблица пуста, то воспользуйтесь кнопкой \"Добавить\" для добавления записи.";
    MessageBox(Handle,msg.c_str(),"Сообщение:",MB_OK|MB_ICONINFORMATION);
    return;
  }

  idselected=listIDs->Strings[selrow].ToInt();
  AnsiString idspec=AnsiString(WCGetNumForKeyTitle(SPECS,ComboBoxSpec->Text));
  AnsiString fulltitle=ComboBoxDiscipline->Text;
  AnsiString shorttitle=ComboBoxAbr->Text;

  int findid;
  AnsiString findspec,findfulltitle,findshorttitle;
  bool IsExists=IsExistsRecordDiscip(findid,findspec,findfulltitle,findshorttitle);
  bool neededit=(!IsExists || (IsExists && idselected==findid));
  if (!neededit)
  {
    AnsiString msg="Запись с данными:\n\n\tспециальность:  "+findspec+"\n\tдисциплина:  "+findfulltitle+"\n\tсокращенное название:  "+findshorttitle+"\n\nуже существует в базе данных.\nИзменения не произошло!";
    MessageBox(Handle,msg.c_str(),"Сообщение:",MB_OK|MB_ICONINFORMATION);
    return;
  }

  // neededit=true -- в изменении нуждается
  AnsiString myquery="UPDATE "+opts.DBDisciplines+" SET idspec="+ToStr(idspec)+",fulltitle="+ToStr(fulltitle)+",shorttitle="+ToStr(shorttitle)+",idclass="+ToStr(AnsiString(WCGetNumForKeyTitle(DISCIPCLASSIFIC,ComboBoxClassDiscip->Text)))+",num_hours="+ToStr(AnsiString(UpDownNumHours->Position))+", scan_number="+ToStr(AnsiString(UpDownScanNumber->Position))+", sem_hours="+ToStr(AudHoursToString())+" WHERE id="+AnsiString(idselected);
  mysql_query(mysql,myquery.c_str());

  AnsiString DiscipStr=ComboBoxDiscipline->Text;
  AnsiString AbrStr=ComboBoxAbr->Text;
  if (!DiscipStr.IsEmpty() && ComboBoxDiscipline->Items->IndexOf(DiscipStr)==-1)
  {
    ComboBoxDiscipline->Items->Insert(0,DiscipStr);
    ComboBoxDiscipline->ItemIndex=0;
  }
  if (!AbrStr.IsEmpty() && ComboBoxAbr->Items->IndexOf(AbrStr)==-1)
  {
    ComboBoxAbr->Items->Insert(0,AbrStr);
    ComboBoxAbr->ItemIndex=0;
  }

  UpdateTable(StrGridDiscip);

  int selrownew=listIDs->IndexOf(AnsiString(idselected));
  StrGridDiscip->Selection.Top=selrownew;

  StrGridDiscipClick(StrGridDiscip);
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::FormShow(TObject *Sender)
{
  ComboBoxDiscipline->Text="";
  ComboBoxAbr->Text="";
  WCUpdateComboBox(ComboBoxSpec,SPECS);
  WCUpdateComboBox(ComboBoxClassDiscip,DISCIPCLASSIFIC);
  ComboBoxSpec->ItemIndex=ComboBoxSpec->Items->IndexOf(GetSpecForStudID(MyFormDLL->GetIDCurStud()));

  ComboBoxSpecChange(ComboBoxSpec);
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::ClearTable(TStringGrid* pTab)
{
  pTab->RowCount=2;
  for (int j=0;j<pTab->ColCount;j++) pTab->Cells[j][1]="";

}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::UpdateTable(TStringGrid* pTab)
{
  ClearTable(pTab);
  delete_data();

  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString query="SELECT id,fulltitle,shorttitle,idspec,idclass,num_hours,scan_number,sem_hours FROM "+opts.DBDisciplines+" WHERE deleted=0 AND idspec="+WCGetNumForKeyTitle(SPECS,ComboBoxSpec->Text)+" ORDER BY idclass,fulltitle";
  mysql_query(mysql,query.c_str());

  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    int numCurRow;

    if (result && mysql_num_rows(result))
    {
      numCurRow=0;
      listIDs->Clear();
      listIDs->Add("NULL");
      audhoursdiscip.push_back(NULL);

      while (row = mysql_fetch_row(result))
      {
        numCurRow++;
        pTab->RowCount=numCurRow+1;

        listIDs->Add(AnsiString(row[0]));
        pTab->Cells[0][numCurRow]=numCurRow;
        pTab->Cells[1][numCurRow]=row[1];
        pTab->Cells[2][numCurRow]=row[2];
        pTab->Cells[3][numCurRow]=AnsiString(row[5]);
        pTab->Cells[4][numCurRow]=WCGetTitleForKeyNum(DISCIPCLASSIFIC,AnsiString(row[4]).ToInt());
        pTab->Cells[5][numCurRow]=AnsiString(row[6]);

        AudHours *data = new AudHours;
        StringToAudHours(data, AnsiString(row[7]));
        audhoursdiscip.push_back(data);

        pTab->Refresh();
      }
    }
    mysql_free_result(result);
  }
  StrGridDiscipClick(StrGridDiscip);
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::FormDestroy(TObject *Sender)
{
  delete listIDs;
  delete_data();
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::StrGridDiscipClick(TObject *Sender)
{
  int selrow=StrGridDiscip->Selection.Top;
  int idselected;
  if (selrow==StrGridDiscip->Selection.Bottom && !StrGridDiscip->Cells[0][selrow].IsEmpty())
  {
//  idselected=listIDs->Strings[selrow].ToInt();
    ComboBoxDiscipline->Text=StrGridDiscip->Cells[1][selrow];
    ComboBoxAbr->Text=StrGridDiscip->Cells[2][selrow];
    int num=ComboBoxClassDiscip->Items->IndexOf(StrGridDiscip->Cells[4][selrow]);
    ComboBoxClassDiscip->ItemIndex=num;
    ComboBoxClassDiscipChange(Sender);
    try
    {
      UpDownNumHours->Position = StrGridDiscip->Cells[3][selrow].ToInt();
      UpDownScanNumber->Position = StrGridDiscip->Cells[5][selrow].ToInt();
    }
    catch(...)
    {
      UpDownNumHours->Position = 0;
      UpDownScanNumber->Position = 0;
    }

    MaskEditAudHours1->Text = audhoursdiscip[selrow]->hours[0];
    MaskEditAudHours2->Text = audhoursdiscip[selrow]->hours[1];
    MaskEditAudHours3->Text = audhoursdiscip[selrow]->hours[2];
    MaskEditAudHours4->Text = audhoursdiscip[selrow]->hours[3];
    MaskEditAudHours5->Text = audhoursdiscip[selrow]->hours[4];
    MaskEditAudHours6->Text = audhoursdiscip[selrow]->hours[5];
    MaskEditAudHours7->Text = audhoursdiscip[selrow]->hours[6];
    MaskEditAudHours8->Text = audhoursdiscip[selrow]->hours[7];
    MaskEditAudHours9->Text = audhoursdiscip[selrow]->hours[8];
    MaskEditAudHours10->Text = audhoursdiscip[selrow]->hours[9];
    MaskEditAudHours11->Text = audhoursdiscip[selrow]->hours[10];
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::ComboBoxSpecChange(TObject *Sender)
{
  UpdateTable(StrGridDiscip);
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::SetMyFormDLL(TFormDLLProgr* curFormDLL)
{
  MyFormDLL=curFormDLL;
}
//---------------------------------------------------------------------------
bool __fastcall TFormDisciplines::IsExistsRecordDiscip(int &id,AnsiString &spec,AnsiString &fulltitle,AnsiString &shorttitle)
{
  id=-1;
  spec="???";
  fulltitle=shorttitle="???";

  bool IsExists=false;
  int CountRec=0;

  AnsiString idspec=AnsiString(WCGetNumForKeyTitle(SPECS,ComboBoxSpec->Text));
  AnsiString ft=ComboBoxDiscipline->Text;
  AnsiString st=ComboBoxAbr->Text;

  MYSQL_RES *result;
  MYSQL_ROW row;
  AnsiString myquery="SELECT fulltitle,shorttitle,id FROM "+opts.DBDisciplines+" WHERE idspec="+idspec+" AND fulltitle="+ToStr(ft)+" AND shorttitle="+ToStr(st)+" AND deleted=0";
  mysql_query(mysql,myquery.c_str());

  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      row = mysql_fetch_row(result);
      CountRec=mysql_num_rows(result);
      if (CountRec==0)
        IsExists=false; // запись не найдена
      else
        IsExists=true;  // запись найдена
      spec=ComboBoxSpec->Text;
      id=AnsiString(row[2]).ToInt();
      fulltitle=AnsiString(row[0]);
      shorttitle=AnsiString(row[1]);
    }
    mysql_free_result(result);
  }
  return IsExists;
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::EditNumHoursExit(TObject *Sender)
{
  EditNumHours->Text = UpDownNumHours->Position;
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::ComboBoxClassDiscipChange(
      TObject *Sender)
{
  int self_index = ComboBoxClassDiscip->ItemIndex;
  int self_idclass = WCGetNumForKeyTitle(DISCIPCLASSIFIC,ComboBoxClassDiscip->Text);
  if (IS_WEEKS(self_idclass))
  {
    Label4->Caption = "недель";
  }
  else
  {
    Label4->Caption = "часов";
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::EditScanNumberExit(TObject *Sender)
{
  EditScanNumber->Text = UpDownScanNumber->Position;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormDisciplines::AudHoursToString()
{
    AnsiString str =
        MaskEditAudHours1->Text + ";" +
        MaskEditAudHours2->Text + ";" +
        MaskEditAudHours3->Text + ";" +
        MaskEditAudHours4->Text + ";" +
        MaskEditAudHours5->Text + ";" +
        MaskEditAudHours6->Text + ";" +
        MaskEditAudHours7->Text + ";" +
        MaskEditAudHours8->Text + ";" +
        MaskEditAudHours9->Text + ";" +
        MaskEditAudHours10->Text + ";" +
        MaskEditAudHours11->Text + ";";
    return str;
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::StringToAudHours(AudHours* h, AnsiString &str_hours)
{
    int cur_sem = 0;
    int begin = 1;
    for(int pos = 1; pos <= str_hours.Length(); pos++)
    {
        if (str_hours[pos] == ';')
        {
            h->hours[cur_sem++] = str_hours.SubString(begin, pos - begin);
            begin = pos + 1;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall  TFormDisciplines::delete_data(void)
{
    for (int i = 0; i< audhoursdiscip.size(); i++)
        if ( NULL != audhoursdiscip[i])
            delete audhoursdiscip[i];
    audhoursdiscip.clear();
}
//---------------------------------------------------------------------------
void __fastcall TFormDisciplines::MaskEditAudHoursExit(TObject *Sender)
{
    TMaskEdit * edit = (TMaskEdit *)Sender;
    try
    {
        Trim(edit->Text).ToInt();
    }
    catch(...)
    {
        //   edit->Text = "0";
        edit->SetFocus();
        MessageBox(Handle, "Текстовое поле \"Общие часы\" не должно быть пустым", "Сообщение:", MB_OK | MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------

