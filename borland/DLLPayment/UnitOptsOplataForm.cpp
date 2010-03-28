//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitOptsOplataForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormOptsOplata *FormOptsOplata;

extern EDRegOpts opts;
extern MYSQL *mysql;

TStringList* listIDs;

AnsiString arrMounth[2]={"Сентябрь","Февраль"}; // don't edit  - to support order of mounths (0 - september;1 - february)

//---------------------------------------------------------------------------
AnsiString GetPeriodStr(AnsiString Date1Str,AnsiString Date2Str)
{
  // Convert date1 yyyy-mm-dd and date2 yyyy-mm-dd to "(September/February) yyyy - (September/February) yyyy"
  AnsiString Res="";
  try
  {
    int y1=Date1Str.SubString(1,4).ToInt(),
        m1=Date1Str.SubString(6,2).ToInt(),
        d1=Date1Str.SubString(9,2).ToInt(),
        y2=Date2Str.SubString(1,4).ToInt(),
        m2=Date2Str.SubString(6,2).ToInt(),
        d2=Date2Str.SubString(9,2).ToInt();
    switch (m1)
    {
      case 2: Res+="Февраль ";break;
      case 9: Res+="Сентябрь ";break;
      default: Res+="Unsupport ";break;
    }
    Res+=AnsiString(y1)+"г. -- ";
    switch (m2)
    {
      case 2: Res+="Февраль ";break;
      case 9: Res+="Сентябрь ";break;
      default: Res+="Unsupport ";break;
    }
    Res+=AnsiString(y2)+"г.";
    return Res;
  }
  catch(...)
  {
    Res="Unknown format of date";
    return Res;
  }
}
//---------------------------------------------------------------------------
__fastcall TFormOptsOplata::TFormOptsOplata(TComponent* Owner)
        : TForm(Owner)
{
  ResultIDOpts=ERROR_ID; // can't been
  GroupStr="???";
  DatesStr="??? - ???";
  CountMoney="???";

  Constraints->MinWidth=Width;
  Constraints->MinHeight=Height;

  listIDs=new TStringList;

  StrGridOpts->Cells[0][0]="№п/п";
  StrGridOpts->Cells[1][0]="Группа";
  StrGridOpts->Cells[2][0]="Период оплаты";
  StrGridOpts->Cells[3][0]="Сумма оплаты за год";
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::FormShow(TObject *Sender)
{
    // ставим переключатель
    RadioGroupFilter->ItemIndex = MyFormDLL->GetFilterCheck();

    ComboBoxMounth1->Clear();
    ComboBoxMounth2->Clear();
    for (int i=0;i<2;i++)
    {
        ComboBoxMounth1->Items->Add(arrMounth[i]);
        ComboBoxMounth2->Items->Add(arrMounth[i]);
    }

    WCUpdateComboBox(ComboBoxGroup,GROUPS);
    WCUpdateComboBox(ComboBoxYear1,DOGYEARS);
    WCUpdateComboBox(ComboBoxYear2,DOGYEARS);

    // поиск и установка группы студента в ComboBoxGroup
    ComboBoxGroup->ItemIndex=ComboBoxGroup->Items->IndexOf(GetGroupForStudID(MyFormDLL->GetIDCurStud()));
    ComboBoxMounth1->ItemIndex=0;
    ComboBoxYear1->ItemIndex=0;
    UpdateDate2();

    ComboBoxGroupChange(ComboBoxGroup);
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::UpdateDate2(void)
{
  ComboBoxMounth2->ItemIndex=ComboBoxMounth2->Items->IndexOf(ComboBoxMounth1->Text);
  ComboBoxYear2->ItemIndex=ComboBoxYear2->Items->IndexOf(AnsiString(ComboBoxYear1->Text.ToInt()+1));
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::UpdateDatesInStatusBar(int selrow)
{
  AnsiString Str;
  Str=StrGridOpts->Cells[1][selrow];
  if (Str.IsEmpty()) Str="???";
  StatusBar->Panels->Items[1]->Text=Str;
  Str=StrGridOpts->Cells[2][selrow];
  if (Str.IsEmpty()) Str="??? - ???";
  StatusBar->Panels->Items[4]->Text=Str;
  Str=StrGridOpts->Cells[3][selrow];
  if (Str.IsEmpty()) Str="???";
  StatusBar->Panels->Items[7]->Text=Str;
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::ComboBoxMounth1Change(TObject *Sender)
{
  UpdateDate2();
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::FormResize(TObject *Sender)
{
  int size=StrGridOpts->ClientWidth-20,
      sizeindex=size*10/100,
      sizegroup=size*25/100,
      sizePeriod=size*45/100,
      sizeCommonMoney=size*20/100;
  StrGridOpts->ColWidths[0]=sizeindex;
  StrGridOpts->ColWidths[1]=sizegroup;
  StrGridOpts->ColWidths[2]=sizePeriod;
  StrGridOpts->ColWidths[3]=sizeCommonMoney;
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::ClearTable(TStringGrid* pTab)
{
  pTab->RowCount=2;
  for (int j=0;j<pTab->ColCount;j++) pTab->Cells[j][1]="";
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::UpdateTable(TStringGrid* pTab)
{
    ClearTable(pTab);

    MYSQL_RES *result1;
    MYSQL_RES *result2;
    MYSQL_ROW row1,row2;

    int idstud=MyFormDLL->GetIDCurStud();
    int numCurRow;


    AnsiString query="SELECT op.id,op.idgroup,op.datestart,op.dateend,op.commoncountmoney FROM "+opts.DBPayOpts+" as op, "+opts.DBPayFacts+" as f WHERE op.deleted=0 AND f.deleted=0 AND f.idstud="+ToStr(AnsiString(idstud))+" AND f.idopts=op.id ORDER BY op.datestart";

    mysql_query(mysql,query.c_str());
    if (mysql_field_count(mysql))
    {
        result1=mysql_store_result(mysql);
        if (result1 && mysql_num_rows(result1))
        {
            numCurRow=0;
            listIDs->Clear();
            listIDs->Add("NULL");

            while (row1 = mysql_fetch_row(result1))
            {
                if (listIDs->IndexOf(AnsiString(row1[0])) != -1) continue;

                numCurRow++;
                pTab->RowCount=numCurRow+1;

                listIDs->Add(AnsiString(row1[0]));
                pTab->Cells[0][numCurRow]=numCurRow;
                pTab->Cells[1][numCurRow]=WCGetTitleForKeyNum(GROUPS,AnsiString(row1[1]).ToInt());
                pTab->Cells[2][numCurRow]=GetPeriodStr(AnsiString(row1[2]),AnsiString(row1[3]));
                pTab->Cells[3][numCurRow]=AnsiString(row1[4]);

                pTab->Refresh();
            }
        }
        mysql_free_result(result1);
    }

    if (listIDs->Count == 0)
    {
        numCurRow=0;
        listIDs->Clear();
        listIDs->Add("NULL");
    }

    if (RadioGroupFilter->ItemIndex == 0)
    {
        query="SELECT id,idgroup,datestart,dateend,commoncountmoney FROM "+opts.DBPayOpts+" WHERE deleted=0 AND idgroup="+WCGetNumForKeyTitle(GROUPS,ComboBoxGroup->Text)+" ORDER BY datestart";
        mysql_query(mysql,query.c_str());
        if (mysql_field_count(mysql))
        {
            result2=mysql_store_result(mysql);

            if (result2 && mysql_num_rows(result2))
            {
                while (row2 = mysql_fetch_row(result2))
                {
                    if (listIDs->IndexOf(AnsiString(row2[0])) != -1) continue;

                    numCurRow++;
                    pTab->RowCount=numCurRow+1;

                    listIDs->Add(AnsiString(row2[0]));
                    pTab->Cells[0][numCurRow]=numCurRow;
                    pTab->Cells[1][numCurRow]=WCGetTitleForKeyNum(GROUPS,AnsiString(row2[1]).ToInt());
                    pTab->Cells[2][numCurRow]=GetPeriodStr(AnsiString(row2[2]),AnsiString(row2[3]));
                    pTab->Cells[3][numCurRow]=AnsiString(row2[4]);

                    pTab->Refresh();
                }
            }
            mysql_free_result(result2);
        }
    }
    StrGridOptsClick(StrGridOpts);
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::ComboBoxGroupChange(TObject *Sender)
{
  UpdateTable(StrGridOpts);
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::SetDate(int numdate,int idselected)
{
    if (numdate!=1 && numdate!=2) return;

    MYSQL_RES *result;
    MYSQL_ROW row;
    AnsiString DateStr;

    AnsiString query;
    switch (numdate)
    {
      case 1: query="SELECT datestart FROM "+opts.DBPayOpts+" WHERE deleted=0 AND id="+ToStr(idselected);
              break;
      case 2: query="SELECT dateend FROM "+opts.DBPayOpts+" WHERE deleted=0 AND id="+ToStr(idselected);
              break;
    }
    mysql_query(mysql,query.c_str());
    if (mysql_field_count(mysql))
    {
      result=mysql_store_result(mysql);
      if (result && mysql_num_rows(result))
      {
        row = mysql_fetch_row(result);
        DateStr=AnsiString(row[0]);
      }
      mysql_free_result(result);
    }
    AnsiString year=DateStr.SubString(1,4),
               mounth=DateStr.SubString(6,2),
               day=DateStr.SubString(9,2);
    TComboBox *curBoxYear,*curBoxMounth;
    switch (numdate)
    {
      case 1: curBoxYear=ComboBoxYear1;
              curBoxMounth=ComboBoxMounth1;
              break;
      case 2: curBoxYear=ComboBoxYear2;
              curBoxMounth=ComboBoxMounth2;
              break;
      default: return;break;
    }
    curBoxYear->ItemIndex=curBoxYear->Items->IndexOf(year);
    switch (mounth.ToInt())
    {
      case 9: curBoxMounth->ItemIndex=0;break;
      case 2: curBoxMounth->ItemIndex=1;break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::StrGridOptsClick(TObject *Sender)
{
  // здесь перенос данных на ComboBoxes
  int selrow=StrGridOpts->Selection.Top;
  int idselected;
  if (selrow==StrGridOpts->Selection.Bottom && !StrGridOpts->Cells[0][selrow].IsEmpty())
  {
    EditCommonCountMoney->Text=StrGridOpts->Cells[3][selrow];
    idselected=listIDs->Strings[selrow].ToInt();
    SetDate(1,idselected);
    SetDate(2,idselected);
    EditCommonCountMoney->SetFocus();
    EditCommonCountMoney->SelectAll();
    ComboBoxMounth1Change(ComboBoxMounth1);
  }
  UpdateDatesInStatusBar(selrow);
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::BtnAddRecordClick(TObject *Sender)
{
  AnsiString idgroup=AnsiString(WCGetNumForKeyTitle(GROUPS,ComboBoxGroup->Text));
  AnsiString commoncountmoney=EditCommonCountMoney->Text;
  AnsiString Date1=GetSQLDate(1);
  AnsiString Date2=GetSQLDate(2);

  int findid;
  AnsiString findgroup,finddate1,finddate2;
  if (IsExistsRecordOpts(findid,findgroup,finddate1,finddate2))
  {
    AnsiString msg="Запись с данными:\n\n\tгруппа:  "+findgroup+"\n\tпериод оплаты:  "+GetPeriodStr(finddate1,finddate2)+"\n\nуже существует как категория оплаты.\nДобавить еще одну категорию оплаты ?";
    if (IDNO == MessageBox(Handle,msg.c_str(),"Сообщение:",MB_YESNO|MB_ICONINFORMATION))
        return;
  }

  try
  {
    int money=commoncountmoney.ToInt();
    if (money<=0) throw 1;
  }
  catch(...)
  {
    AnsiString msg="Сумма оплаты должна задаваться как целое положительное число!\nИсправьте пожалуйста!";
    MessageBox(Handle,msg.c_str(),"Ошибка:",MB_OK|MB_ICONERROR);
    EditCommonCountMoney->SetFocus();
    EditCommonCountMoney->SelectAll();
    return;
  }

  AnsiString myquery="INSERT INTO "+opts.DBPayOpts+" (idgroup,datestart,dateend,commoncountmoney) VALUES("+ToStr(idgroup)+","+ToStr(Date1)+","+ToStr(Date2)+","+ToStr(commoncountmoney)+")";
  mysql_query(mysql,myquery.c_str());

  ComboBoxGroupChange(ComboBoxGroup);
  StrGridOptsClick(StrGridOpts);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormOptsOplata::GetSQLDate(int numDate)
{
   TComboBox *curBoxYear,
             *curBoxMounth;
   switch (numDate)
   {
     case 1: curBoxYear=ComboBoxYear1;
             curBoxMounth=ComboBoxMounth1;
             break;
     case 2: curBoxYear=ComboBoxYear2;
             curBoxMounth=ComboBoxMounth2;
             break;
     default:
             return "0000-00-00";
             break;
   }
   AnsiString year,mounth,day;
   year=curBoxYear->Text;
   if (year.IsEmpty()) year="0000";
   if (curBoxMounth->ItemIndex==0) // Это СЕНТЯБРЬ
        mounth="09";
   else
        if (curBoxMounth->ItemIndex==1) // Это ФЕВРАЛЬ
             mounth="02";
        else mounth="00";
   day="01";
   AnsiString ResDate=year+"-"+mounth+"-"+day;
   return ResDate;
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::BtnDelRecordClick(TObject *Sender)
{
  int selrow=StrGridOpts->Selection.Top;
  if (selrow!=StrGridOpts->Selection.Bottom || StrGridOpts->Cells[1][selrow].IsEmpty())
  {
    MessageBox(Handle,"Таблица или выбранная запись пустая либо выбрано более одной записи.\nВыберите непустую запись.","Сообщение:",MB_OK|MB_ICONINFORMATION);
    return;
  }

  AnsiString myquery;
  AnsiString msg="Вы действительно хотите удалить эту запись?\nВедь при её удалении пропадут некоторые данные о студентах,\nа при попытке исправить ситуацию и внести запись с теми же данными структура базы не восстановится!\nБудьте аккуратны!";
  int idselected;
  if (MessageBox(Handle,msg.c_str(),"Сообщение:",MB_YESNO|MB_ICONQUESTION)==ID_YES)
  {
    idselected=listIDs->Strings[StrGridOpts->Selection.Top].ToInt();
    myquery="UPDATE "+opts.DBPayOpts+" SET deleted=1 WHERE id="+AnsiString(idselected);
    mysql_query(mysql,myquery.c_str());

    ComboBoxGroupChange(ComboBoxGroup);
    StrGridOptsClick(StrGridOpts);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::BtnEditRecordClick(TObject *Sender)
{
  int selrow=StrGridOpts->Selection.Top;
  int idselected;
  if (selrow!=StrGridOpts->Selection.Bottom || StrGridOpts->Cells[0][selrow].IsEmpty())
  {
    AnsiString msg="Запись не выбрана либо выбрано более 1 записи.\nИсправление не произошло!\nЕсли таблица пуста, то воспользуйтесь кнопкой \"Добавить\" для добавления записи.";
    MessageBox(Handle,msg.c_str(),"Сообщение:",MB_OK|MB_ICONINFORMATION);
    return;
  }

  idselected=listIDs->Strings[selrow].ToInt();
  AnsiString idgroup=AnsiString(WCGetNumForKeyTitle(GROUPS,ComboBoxGroup->Text));
  AnsiString commoncountmoney=EditCommonCountMoney->Text;
  AnsiString Date1=GetSQLDate(1);
  AnsiString Date2=GetSQLDate(2);

  try
  {
    int money=commoncountmoney.ToInt();
    if (money<=0) throw 1;
  }
  catch(...)
  {
    AnsiString msg="Сумма оплаты должна задаваться как целое положительное число!\nИсправьте пожалуйста!";
    MessageBox(Handle,msg.c_str(),"Ошибка:",MB_OK|MB_ICONERROR);
    EditCommonCountMoney->SetFocus();
    EditCommonCountMoney->SelectAll();
    return;
  }

  int findid;
  AnsiString findgroup,finddate1,finddate2;
  bool IsExists=IsExistsRecordOpts(findid,findgroup,finddate1,finddate2);
  bool neededit=(!IsExists || (IsExists && idselected==findid));
  if (!neededit)
  {
    AnsiString msg="Запись с данными:\n\n\tгруппа:  "+findgroup+"\n\tпериод оплаты:  "+GetPeriodStr(finddate1,finddate2)+"\n\nуже существует как категория оплаты.\nИзменения не произошло!";
    MessageBox(Handle,msg.c_str(),"Сообщение:",MB_OK|MB_ICONINFORMATION);
    return;
  }

  // neededit=true -- в изменении нуждается
  AnsiString myquery="UPDATE "+opts.DBPayOpts+" SET idgroup="+ToStr(idgroup)+",datestart="+ToStr(Date1)+",dateend="+ToStr(Date2)+",commoncountmoney="+ToStr(commoncountmoney)+" WHERE id="+AnsiString(idselected);
  mysql_query(mysql,myquery.c_str());

  UpdateTable(StrGridOpts);

  int selrownew=listIDs->IndexOf(AnsiString(idselected));
  StrGridOpts->Selection.Top=selrownew;
  
  StrGridOptsClick(StrGridOpts);
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::BtnSelectCloseClick(TObject *Sender)
{
  // Выбор ID записи и закрытие окна
  int selrow=StrGridOpts->Selection.Top;
  if (selrow==StrGridOpts->Selection.Bottom && !StrGridOpts->Cells[0][selrow].IsEmpty())
  {
    ResultIDOpts=listIDs->Strings[selrow].ToInt();
    GroupStr=StatusBar->Panels->Items[1]->Text;
    DatesStr=StatusBar->Panels->Items[4]->Text;
    CountMoney=StatusBar->Panels->Items[7]->Text;

    Close();
  }
  else
  {
    ResultIDOpts=ERROR_ID;
    GroupStr=StatusBar->Panels->Items[1]->Text;
    DatesStr=StatusBar->Panels->Items[4]->Text;
    CountMoney=StatusBar->Panels->Items[7]->Text;

    AnsiString msg="Выбрано более одной записи или Вы выбрали пустую запись,\nкоторая не может быть использована как категория оплата,\nтак как она не содержит никакой информационной нагрузки!\nСначала создайте новую запись!\nЗакрыть окно редактирования категорий оплаты?";
    if (MessageBox(Handle,msg.c_str(),"Сообщение:",MB_YESNO|MB_ICONINFORMATION)==ID_YES) Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::StrGridOptsDblClick(TObject *Sender)
{
  BtnSelectCloseClick(StrGridOpts);
}
//---------------------------------------------------------------------------
bool __fastcall TFormOptsOplata::IsExistsRecordOpts(int &id,AnsiString &group,AnsiString &date1,AnsiString &date2)
{
  id=-1;
  group="???";
  date1=date2="0000-00-00";

  bool IsExists=false;
  int CountRec=0;

  AnsiString idgroup=AnsiString(WCGetNumForKeyTitle(GROUPS,ComboBoxGroup->Text));
  AnsiString Date1=GetSQLDate(1);
  AnsiString Date2=GetSQLDate(2);

  MYSQL_RES *result;
  MYSQL_ROW row;
  AnsiString myquery="SELECT datestart,dateend,id FROM "+opts.DBPayOpts+" WHERE idgroup="+idgroup+" AND datestart="+ToStr(Date1)+" AND dateend="+ToStr(Date2)+" AND deleted=0";
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
      group=ComboBoxGroup->Text;
      id=AnsiString(row[2]).ToInt();
      date1=AnsiString(row[0]);
      date2=AnsiString(row[1]);
    }
    mysql_free_result(result);
  }
  return IsExists;
}
//---------------------------------------------------------------------------
int __fastcall TFormOptsOplata::GetResultIDOpts(void)
{
  return ResultIDOpts;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormOptsOplata::GetGroupForStudID(int _idstud)
{
  AnsiString GroupStr="???";
  int myidgroup=ERROR_ID;

  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString query="SELECT grpid FROM "+opts.DBStudTable+" WHERE deleted=0 AND id="+AnsiString(_idstud);
  mysql_query(mysql,query.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      row = mysql_fetch_row(result);
      myidgroup=AnsiString(row[0]).ToInt();
    }
    mysql_free_result(result);
  }
  if (myidgroup!=ERROR_ID) GroupStr=WCGetTitleForKeyNum(GROUPS,myidgroup);
  return GroupStr;
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::SetMyFormDLL(TFormDLL* curFormDLL)
{
  MyFormDLL=curFormDLL;
}
//---------------------------------------------------------------------------
void __fastcall TFormOptsOplata::GetSelectedFieldsOfOpts(AnsiString& _groupstr,AnsiString& _datesstr,AnsiString& _countmoney)
{
  _groupstr=GroupStr;
  _datesstr=DatesStr;
  _countmoney=CountMoney;
}
//---------------------------------------------------------------------------

void __fastcall TFormOptsOplata::RadioGroupFilterClick(TObject *Sender)
{
    if (RadioGroupFilter->ItemIndex == MyFormDLL->GetFilterCheck())
        return;
    UpdateTable(StrGridOpts);
    // сохраняем переключатель
    MyFormDLL->SetFilterCheck(RadioGroupFilter->ItemIndex);

}
//---------------------------------------------------------------------------

