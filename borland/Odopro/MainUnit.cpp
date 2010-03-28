//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnit.h"
#include "UnitSelected.h"
#include "UnitFilterMain.h"
#include "UnitCommFilter.h"
#include "UnitProgress.h"
#include "UnitSplash.h"
#include "UnitAbout.h"
#include "UnitStatistic.h"
#include "myregini.h"            //For working with ini-files
#include "UnitWorkRepDLL.h"
#include "UnitAccount.h"
#include "UnitClearDB.h"
#include "UnitIzmSem.h"
#include "UnitIzmGraphSem.h"
#include "UnitIzmOchenka.h"
#include "UnitDeltaDogovor.h"

#include <windows.h>
#include <fstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

extern MYSQL* mysql;
extern enum TTypeQuery tquery;
extern enum TTypeKeyVoc tkey;
extern AnsiString query;
extern AnsiString idselected;
extern TStringList* listIDs;

/* AnsiString host = "194.190.191.43",
           user="test",
           password="test";
AnsiString DBName="odoportal";*/

AnsiString host,user,password,DBName;
extern AnsiString TabStudName;
extern AnsiString TabVocName;

extern TStringList *AssocItemsRep;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{
  InitTable();

  Constraints->MinWidth=Width;
  Constraints->MinHeight=Height;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::InitTable(void)
{
  StrGridDB->Cells[0][0]="";                      StrGridDB->ColWidths[0]=10;
  StrGridDB->Cells[1][0]="№";                     StrGridDB->ColWidths[1]=50;
  StrGridDB->Cells[2][0]="Фамилия";                StrGridDB->ColWidths[2]=150;
  StrGridDB->Cells[3][0]="Имя";                    StrGridDB->ColWidths[3]=100;
  StrGridDB->Cells[4][0]="Отчество";               StrGridDB->ColWidths[4]=100;
//  StrGridDB->Cells[4][0]="Пол";                    StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[5][0]="Дата рожд.";             StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[6][0]="Адрес прописки";         StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[7][0]="Адрес прожив.";          StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[8][0]="Телефоны";               StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[9][0]="E-mail";                 StrGridDB->ColWidths[0]=40;
  StrGridDB->Cells[5][0]="№ зачетки";             StrGridDB->ColWidths[5]=80;
  StrGridDB->Cells[6][0]="Группа";                StrGridDB->ColWidths[6]=80;
  StrGridDB->Cells[7][0]="Город";                 StrGridDB->ColWidths[7]=170;
//  StrGridDB->Cells[13][0]="№ лич. дела";           StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[14][0]="Специал.";              StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[15][0]="Дата под. док.";        StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[16][0]="Дата оконч.";           StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[17][0]="№ договора";            StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[18][0]="Срок обучения";         StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[19][0]="Форма обучения";        StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[20][0]="Образование";           StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[21][0]="Док. об образ.";        StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[22][0]="Учебное завед.";        StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[23][0]="№ приказа о зачисл.";   StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[24][0]="№ приказа об отчисл.";  StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[25][0]="Язык";                  StrGridDB->ColWidths[0]=40;
//  StrGridDB->Cells[26][0]="Примеч.";               StrGridDB->ColWidths[0]=40;

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::InsertRecord(TStringGrid *pTab)
{
  pTab->RowCount++;
  int numCurRow=pTab->RowCount-1;

  pTab->Cells[1][numCurRow]="?";
  pTab->Cells[2][numCurRow]=FormSelected->EditSecondName->Text;
  pTab->Cells[3][numCurRow]=FormSelected->EditFirstName->Text;
  pTab->Cells[4][numCurRow]=FormSelected->EditThirdName->Text;
  pTab->Cells[5][numCurRow]=FormSelected->Editznum->Text;
  AnsiString GroupStr=FormSelected->GetTitleForKeyNum(GROUPS,FormSelected->ComboBoxgrp->ItemIndex);
  pTab->Cells[6][numCurRow]=GroupStr;//???
  pTab->Cells[7][numCurRow]=FormSelected->GetCityName(GroupStr);
  //pTab->Cells[7][numCurRow]=FormSelected->GetTitleForKeyNum(CITIES,FormSelected->ComboBoxcity->ItemIndex);//???
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BtnInsertClick(TObject *Sender)
{
  tquery=INSERT;
  FormSelected->Caption="Вставка новой записи в базу данных.";
  FormSelected->ShowModal();
  ComboBoxGroup->ItemIndex=ComboBoxGroup->Items->IndexOf(FormSelected->ComboBoxgrp->Text);
  ComboBoxGroup->OnChange(ComboBoxGroup);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExitItemClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetStatusBar(void)
{
   StatusBar->Panels->Items[1]->Text=host;
   StatusBar->Panels->Items[4]->Text=user;
   StatusBar->Panels->Items[7]->Text=DBName;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ConnectWithDB(void)
{
   InitConnectionData();
   if (mysql_connect(mysql,host.c_str(),user.c_str(),password.c_str()))
   {
     mysql_select_db(mysql,DBName.c_str());
     if (0!=mysql_errno(mysql))
     {
       AnsiString msg="Не найдена база данных: "+DBName;
       MessageBox(Handle,msg.c_str(),"Ошибка:",MB_OK|MB_ICONERROR);
       ChangeEnableItems(false);
       return;
     }
     Memo->Lines->Add(mysql_error(mysql));

     listIDs=new TStringList;
     listIDs->Clear();
     listIDs->Add("NULL");

     query="CREATE TABLE IF NOT EXISTS "+TabStudName+" (id INT PRIMARY KEY AUTO_INCREMENT NOT NULL, secondname CHAR(20) NULL";
     query=query+", firstname CHAR(20) NULL, thirdname CHAR(20) NULL, sex CHAR(1) NULL, bdate DATE NULL";
     query=query+", grpid INT(2) NULL, znum CHAR(10) NULL, cityid INT(1) NULL, specid INT(1) NULL";
     query=query+", dogshifrid INT(1) NULL, dogfastid BIT NULL, dogyearid INT(1) NULL, dognum INT(2) NULL";
     query=query+", passseries CHAR(8) NULL, passnum CHAR(6) NULL, passdate DATE NULL, passplace TEXT NULL";
     query=query+", email CHAR(50) NULL, addr TEXT NULL, liveaddr TEXT NULL, phones TEXT NULL";
     query=query+", educationid INT(1) NULL, edudocid INT(1) NULL, edudocinfo TEXT NULL, eduplace TEXT NULL, eduenddate DATE NULL, eduformid INT(1) NULL, eduspec TEXT NULL, languageid int(1) NULL";
     query=query+", misc TEXT NULL, enternum INT(1) NULL, enterdate DATE NULL, exitnum INT(1) NULL, exitdate DATE NULL";
     query=query+", deleted BIT NOT NULL DEFAULT 0, vkr_title TEXT NULL)";

     mysql_query(mysql,query.c_str());
     Memo->Lines->Add(mysql_error(mysql));

     FormSelected->UpdateComboBox(ComboBoxGroup,GROUPS);
     ComboBoxGroup->ItemIndex=0;
     BtnUpdateClick(LoadItem);
     StrGridDBClick(LoadItem);
     ChangeEnableItems(true);
     SetStatusBar();

     query="CREATE TABLE IF NOT EXISTS "+TabVocName+" (id INT PRIMARY KEY AUTO_INCREMENT NOT NULL, num INT(1) NOT NULL, vkey CHAR(20) NOT NULL, title CHAR(255) NOT NULL, deleted BIT NOT NULL DEFAULT 0)";
     mysql_query(mysql,query.c_str());
     Memo->Lines->Add(mysql_error(mysql));
   }
   else
   {
     AnsiString msg="Неудачное подключение пользователя "+user+" к host= \'"+host+"\'.";
     MessageBox(Handle,msg.c_str(),"Сообщение:",MB_OK|MB_ICONINFORMATION);

     ChangeEnableItems(false);
     //ExitItemClick(MainForm);
   }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ChangeEnableItems(bool Enable)
{
  BtnInsert->Enabled=Enable;
  BtnDelete->Enabled=Enable;
  BtnEdit->Enabled=Enable;
  BtnUpdate->Enabled=Enable;
  ComboBoxGroup->Enabled=Enable;
  StrGridDB->Enabled=Enable;

  ItemAdd->Enabled=Enable;
  ItemDel->Enabled=Enable;
  ItemEdit->Enabled=Enable;
  UpdateItem->Enabled=Enable;
  FilterItem->Enabled=Enable;

  ReportItem->Enabled=Enable;
  StatItem->Enabled=Enable;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::UpdateRecord(TStringGrid *pTab)
{
  MYSQL_RES *result;
  MYSQL_ROW row;

  query="SELECT id,secondname,firstname,thirdname,grpid,znum,cityid FROM "+TabStudName+" WHERE id="+AnsiString(idselected)+" AND deleted=0";
  mysql_query(mysql,query.c_str());
  Memo->Lines->Add(mysql_error(mysql));

  if (mysql_field_count(mysql))
  {
    Memo->Lines->Add(mysql_error(mysql));
    result=mysql_store_result(mysql);
    Memo->Lines->Add(mysql_error(mysql));
    int numCurRow;
    if (result && mysql_num_rows(result)==1) // could be one record
    {
      Memo->Lines->Add(mysql_error(mysql));

      numCurRow=pTab->Selection.Top;
      row = mysql_fetch_row(result);
      Memo->Lines->Add(mysql_error(mysql));

      listIDs->Add(AnsiString(row[0]));
      StrGridDB->Cells[1][numCurRow]=numCurRow;
      StrGridDB->Cells[2][numCurRow]=row[1];
      StrGridDB->Cells[3][numCurRow]=row[2];
      StrGridDB->Cells[4][numCurRow]=row[3];
      StrGridDB->Cells[5][numCurRow]=row[5];
      AnsiString GroupStr=FormSelected->GetTitleForKeyNum(GROUPS,AnsiString(row[4]).ToInt());
      StrGridDB->Cells[6][numCurRow]=GroupStr;
      StrGridDB->Cells[7][numCurRow]=FormSelected->GetCityName(GroupStr);
      //StrGridDB->Cells[7][numCurRow]=FormSelected->GetTitleForKeyNum(CITIES,AnsiString(row[6]).ToInt());
    }
    mysql_free_result(result);
    Memo->Lines->Add(mysql_error(mysql));
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BtnEditClick(TObject *Sender)
{
  FormSelected->UpDownStudent->Min=1;
  FormSelected->UpDownStudent->Max=StrGridDB->RowCount-1;
  FormSelected->UpDownStudent->Position=StrGridDB->Selection.Top;

  // ID locate in  the first position(field) into table
  int selrow=StrGridDB->Selection.Top;
  if (selrow!=StrGridDB->Selection.Bottom || StrGridDB->Cells[1][selrow].IsEmpty())
  {
    MessageBox(Handle,"Таблица или выбранная запись пустая.\nВыберите непустую запись.","Сообщение:",MB_OK|MB_ICONINFORMATION);
    return;
  }

  tquery=UPDATE;
  MYSQL_RES *result;
  MYSQL_ROW row;
  FormSelected->Caption="Редактирование записи: "+StrGridDB->Cells[2][selrow]+" "+ StrGridDB->Cells[3][selrow]+" "+StrGridDB->Cells[4][selrow];

  FormSelected->ShowModal();
  ComboBoxGroup->ItemIndex=ComboBoxGroup->Items->IndexOf(FormSelected->ComboBoxgrp->Text);
  ComboBoxGroup->OnChange(ComboBoxGroup);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ClearTable(TStringGrid *pTab)
{
  pTab->RowCount=2;
  for (int i=0;i<pTab->ColCount;i++)
      pTab->Cells[i][1]="";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::UpdateTable(TStringGrid *pTab)
{
  // longest waiting with ProgressBar

  MYSQL_RES *result;
  MYSQL_ROW row;

  StatusBar->Panels->Items[13]->Text="0";
  StatusBar->Refresh();

  //------------ Waiting -----------------
  FormProgress->Show();
  FormProgress->Increment(3);
  //------------ Waiting -----------------

  query="SELECT id,secondname,firstname,thirdname,grpid,znum,cityid FROM "+TabStudName+" WHERE deleted=0 AND grpid="+FormSelected->GetNumForKeyTitle(GROUPS,ComboBoxGroup->Text)+" ORDER BY secondname,firstname,thirdname,id";
  mysql_query(mysql,query.c_str());
  Memo->Lines->Add(mysql_error(mysql));

  //------------ Waiting -----------------
  FormProgress->Increment(10);
  //------------ Waiting -----------------

  if (mysql_field_count(mysql))
  {
    Memo->Lines->Add(mysql_error(mysql));
    result=mysql_store_result(mysql);
    Memo->Lines->Add(mysql_error(mysql));
    int numCurRow;

    if (result && mysql_num_rows(result))
    {
      Memo->Lines->Add(mysql_error(mysql));

      numCurRow=0;
      listIDs->Clear();
      listIDs->Add("NULL");

      while (row = mysql_fetch_row(result))
      {
        Memo->Lines->Add(mysql_error(mysql));

        //------------ Waiting -----------------
        FormProgress->Increment((FormProgress->GetMax()-FormProgress->GetPos())/(mysql_num_rows(result)-numCurRow));
        //------------ Waiting -----------------

        numCurRow++;
        pTab->RowCount=numCurRow+1;

        listIDs->Add(AnsiString(row[0]));
        pTab->Cells[1][numCurRow]=numCurRow;
        pTab->Cells[2][numCurRow]=row[1];
        pTab->Cells[3][numCurRow]=row[2];
        pTab->Cells[4][numCurRow]=row[3];
        pTab->Cells[5][numCurRow]=row[5];
        //pTab->Cells[6][numCurRow]=ComboBoxGroup->Text;
        AnsiString GroupStr=FormSelected->GetTitleForKeyNum(GROUPS,AnsiString(row[4]).ToInt());
        pTab->Cells[6][numCurRow]=GroupStr;
        pTab->Cells[7][numCurRow]=FormSelected->GetCityName(GroupStr);
        //pTab->Cells[7][numCurRow]=FormSelected->GetTitleForKeyNum(CITIES,AnsiString(row[6]).ToInt());

        pTab->Refresh();
        StatusBar->Panels->Items[13]->Text=AnsiString(pTab->RowCount-1);
        StatusBar->Refresh();
      }
    }
    mysql_free_result(result);
    Memo->Lines->Add(mysql_error(mysql));
  }

  //------------ Waiting -----------------
  FormProgress->Hide();
  //------------ Waiting -----------------


  EDRegOpts opts;
  BtnVoc->Enabled=opts.ACCESSEditVoc;
  ItemVoc->Enabled=opts.ACCESSEditVoc;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BtnUpdateClick(TObject *Sender)
{
  AnsiString Strtmp=ComboBoxGroup->Text;
  FormSelected->UpdateComboBox(ComboBoxGroup,GROUPS);
  ComboBoxGroup->ItemIndex=ComboBoxGroup->Items->IndexOf(Strtmp);
  ClearTable(StrGridDB);
  UpdateTable(StrGridDB);
  StrGridDB->OnClick(StrGridDB);
  StatusBar->Panels->Items[10]->Text="";
  ComboBoxGroup->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BtnDeleteClick(TObject *Sender)
{
  // ID locate in  the first position(field) into table
  int selrow=StrGridDB->Selection.Top;
  if (selrow!=StrGridDB->Selection.Bottom || StrGridDB->Cells[1][selrow].IsEmpty())
  {
    MessageBox(Handle,"Таблица или выбранная запись пустая.\nВыберите непустую запись.","Сообщение:",MB_OK|MB_ICONINFORMATION);
    return;
  }

  AnsiString msg="Вы действительно хотите удалить эту запись?";
  if (MessageBox(Handle,msg.c_str(),"Сообщение:",MB_YESNO|MB_ICONQUESTION)==ID_YES)
  {
    idselected=listIDs->Strings[StrGridDB->Selection.Top].ToInt();
    query="UPDATE "+TabStudName+" SET deleted=1 WHERE id="+AnsiString(idselected);

    mysql_query(mysql,query.c_str());
    Memo->Lines->Add(mysql_error(mysql));
    BtnUpdateClick(BtnDelete);

    FormSelected->DeleteUserNuke(FormSelected->GetZNumForStudent(idselected));
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::StrGridDBClick(TObject *Sender)
{
  for (int i=1;i<StrGridDB->RowCount;i++) StrGridDB->Cells[0][i]="";
  StrGridDB->Cells[0][StrGridDB->Selection.Top]=">";
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  mysql_close(mysql);
  Memo->Lines->Add(mysql_error(mysql));
  AssocItemsRep->Free();

  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ComboBoxGroupChange(TObject *Sender)
{
  BtnUpdateClick(ComboBoxGroup);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BtnVocClick(TObject *Sender)
{
  FormInsertVoc->ShowModal();
  BtnUpdateClick(BtnVoc);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormResize(TObject *Sender)
{
  int size=StrGridDB->ClientWidth-20,
      sizeindex=size*8/100,
      sizeFN=size*10/100,
      sizeSN=size*20/100,
      sizeTN=size*15/100,
      sizeznum=size*12/100,
      sizegroup=size*12/100,
      sizecity=size*20/100;
  StrGridDB->ColWidths[1]=sizeindex;
  StrGridDB->ColWidths[2]=sizeSN;
  StrGridDB->ColWidths[3]=sizeFN;
  StrGridDB->ColWidths[4]=sizeTN;
  StrGridDB->ColWidths[5]=sizeznum;
  StrGridDB->ColWidths[6]=sizegroup;
  StrGridDB->ColWidths[7]=sizecity;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender)
{
   SplashForm->ShowModal();

   mysql=new MYSQL;
   ConnectWithDB();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadItemClick(TObject *Sender)
{
  delete mysql;
  mysql=new MYSQL;

  ConnectWithDB();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::UpdateTableWithFilter(TStringGrid* pTab,AnsiString SubStrSN)
{
  // longest waiting with ProgressBar

  if (SubStrSN.Length()<=3)
    if (MessageBox(Handle,"Длина введенной подстроки не превышает 3-х символов, а база студентов велика.\nПрограмма может не получить результата выполнения запроса за приемлемое время ожидания.\nПродолжить?","Вопрос:",MB_YESNO|MB_ICONQUESTION)==ID_NO) return;

  ClearTable(pTab);

  MYSQL_RES *result;
  MYSQL_ROW row;

  StatusBar->Panels->Items[13]->Text="0";
  StatusBar->Refresh();

  AnsiString Strtmp=SubStrSN;
  if (Strtmp.Length()>=1) SubStrSN=Strtmp.SubString(1,1).UpperCase();
  if (Strtmp.Length()>=2) SubStrSN+=Strtmp.SubString(2,Strtmp.Length()-1).LowerCase();

  //------------ Waiting -----------------
  FormProgress->Show();
  FormProgress->Increment(3);
  //------------ Waiting -----------------

  query="SELECT id,secondname,firstname,thirdname,grpid,znum,cityid FROM "+TabStudName+" WHERE deleted=0 and secondname LIKE \'"+SubStrSN+"%\' ORDER BY secondname,firstname,thirdname";
  mysql_query(mysql,query.c_str());
  Memo->Lines->Add(mysql_error(mysql));

  //------------ Waiting -----------------
  FormProgress->Increment(10);
  //------------ Waiting -----------------

  if (mysql_field_count(mysql))
  {
    Memo->Lines->Add(mysql_error(mysql));
    result=mysql_store_result(mysql);
    Memo->Lines->Add(mysql_error(mysql));
    int numCurRow;
    if (result && mysql_num_rows(result))
    {
      Memo->Lines->Add(mysql_error(mysql));

      numCurRow=0;
      listIDs->Clear();
      listIDs->Add("NULL");

      while (row = mysql_fetch_row(result))
      {
        Memo->Lines->Add(mysql_error(mysql));

        //------------ Waiting -----------------
        FormProgress->Increment((FormProgress->GetMax()-FormProgress->GetPos())/(mysql_num_rows(result)-numCurRow));
        //------------ Waiting -----------------

        numCurRow++;
        pTab->RowCount=numCurRow+1;

        listIDs->Add(AnsiString(row[0]));
        pTab->Cells[1][numCurRow]=numCurRow;
        pTab->Cells[2][numCurRow]=row[1];
        pTab->Cells[3][numCurRow]=row[2];
        pTab->Cells[4][numCurRow]=row[3];
        pTab->Cells[5][numCurRow]=row[5];
        AnsiString GroupStr=FormSelected->GetTitleForKeyNum(GROUPS,AnsiString(row[4]).ToInt());
        pTab->Cells[6][numCurRow]=GroupStr;
        pTab->Cells[7][numCurRow]=FormSelected->GetCityName(GroupStr);
        //pTab->Cells[7][numCurRow]=FormSelected->GetTitleForKeyNum(CITIES,AnsiString(row[6]).ToInt());

        pTab->Refresh();
        StatusBar->Panels->Items[13]->Text=AnsiString(pTab->RowCount-1);
        StatusBar->Refresh();
      }

      ComboBoxGroup->Enabled=false;
    }
    mysql_free_result(result);
    Memo->Lines->Add(mysql_error(mysql));
  }

  StrGridDB->OnClick(pTab);
  StatusBar->Panels->Items[10]->Text=SubStrSN.UpperCase();

  //------------ Waiting -----------------
  FormProgress->Hide();
  //------------ Waiting -----------------

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FilterItemClick(TObject *Sender)
{
  if (FormFilter->ShowModal()==7) UpdateTableWithFilter(StrGridDB,FormFilter->ComboBoxSubStr->Text);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::InitConnectionData(void)
{
  EDRegOpts opts;
  host = opts.DBHost;
  user = opts.DBLogin;
  password = opts.DBPasswd;
  DBName = opts.DBName;
  TabStudName = opts.DBStudTable;
  TabVocName = opts.DBVocTable;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AboutItemClick(TObject *Sender)
{
  FormAbout->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ItemExecReportClick(TObject *Sender)
{
/*  EDOpts opts;
  AnsiString EXEFileName=opts.EXEGenReport;
  AnsiString FullPath=ExtractFilePath(Application->ExeName)+EXEFileName;
  if ( !FileExists(FullPath) )
  {
    AnsiString msg="Не найден файл программы "+EXEFileName+".\nЭтот файл должен располагаться в текущей дирректории.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONWARNING);
  }
  else
  {
    WinExec(FullPath.c_str(),1);
  } */
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::StatItemClick(TObject *Sender)
{
  FormStatistic->ShowModal();      
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
// Load all titles in MaInMenu...
  AnsiString DLLName=".\\ReportsDLL.dll";
  if (!WCLoadDLL(DLLName)) return;

  AssocItemsRep=new TStringList;
  TMenuItem *NewItemRep;
  TStringList* listRepTitles=WCGetListTitlesOfReportsInDLL();
  for (int indexRep=0;indexRep<listRepTitles->Count;indexRep++)
  {
     NewItemRep = new TMenuItem(MainForm);
     ReportItem->Add(NewItemRep);
     NewItemRep->Caption=listRepTitles->Strings[indexRep];
     AssocItemsRep->AddObject(AnsiString(indexRep),NewItemRep);
     NewItemRep->OnClick = MyEventDoingReport; // Назначаем процедуру обработки кликов
  }
  delete listRepTitles;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MyEventDoingReport(TObject *Sender)
{
  // Обработка выбора пункта меню, ассоциированного с некоторым отчетом
  for (int indexRep=0;indexRep<AssocItemsRep->Count;indexRep++)
  {
    if (AssocItemsRep->Objects[indexRep]==Sender)
    {
      int IDGroup=FormSelected->GetNumForKeyTitle(GROUPS,ComboBoxGroup->Text);
      int IDStudent=listIDs->Strings[StrGridDB->Selection.Top].ToInt();
      WCPreviewReportByIndex(indexRep,IDGroup,IDStudent);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CommFilterItemClick(TObject *Sender)
{
  if (FormFilterComm->ShowModal()==7)
  {
    AnsiString FindStr;
    if (FormFilterComm->RadioButtonStr->Checked)
      FindStr=FormFilterComm->ComboBoxSubStr->Text;
    if (FormFilterComm->RadioButtonDig->Checked)
      FindStr=FormFilterComm->ComboBoxDig->Text;
    if (FormFilterComm->RadioButtonData->Checked)
    {
      FindStr=ReturnParsedDateInSQL(FormFilterComm->DateTimePicker->Date);
    }
    UpdateTableWithCommFilter(StrGridDB,FindStr);
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::UpdateTableWithCommFilter(TStringGrid* pTab,AnsiString FindStr)
{
  // longest waiting with ProgressBar

  if (FindStr.Length()<=3)
    if (MessageBox(Handle,"Длина введенной подстроки не превышает 3-х символов, а база студентов велика.\nПрограмма может не получить результата выполнения запроса за приемлемое время ожидания.\nПродолжить?","Вопрос:",MB_YESNO|MB_ICONQUESTION)==ID_NO) return;

  ClearTable(pTab);

  MYSQL_RES *result;
  MYSQL_ROW row;

  StatusBar->Panels->Items[13]->Text="0";
  StatusBar->Refresh();

  //------------ Waiting -----------------
  FormProgress->Show();
  FormProgress->Increment(3);
  //------------ Waiting -----------------

  if (FormFilterComm->RadioButtonStr->Checked)
  {
   FindStr=FindStr.UpperCase();
   query="SELECT id,secondname,firstname,thirdname,grpid,znum,cityid FROM "+TabStudName+" WHERE deleted=0 and (";
   query+="UCASE(secondname) LIKE \'\%"+FindStr+"\%\'";
   query+=" OR UCASE(firstname) LIKE \'\%"+FindStr+"\%\'";
   query+=" OR UCASE(thirdname) LIKE \'\%"+FindStr+"\%\'";
   query+=" OR UCASE(passplace) LIKE \'\%"+FindStr+"\%\'";
   query+=" OR UCASE(email) LIKE \'\%"+FindStr+"\%\'";
   query+=" OR UCASE(addr) LIKE \'\%"+FindStr+"\%\'";
   query+=" OR UCASE(liveaddr) LIKE \'\%"+FindStr+"\%\'";
   query+=" OR UCASE(edudocinfo) LIKE \'\%"+FindStr+"\%\'";
   query+=" OR UCASE(eduplace) LIKE \'\%"+FindStr+"\%\'";
   query+=" OR UCASE(eduspec) LIKE \'\%"+FindStr+"\%\'";
   query+=") ORDER BY secondname,firstname,thirdname";
  }
  if (FormFilterComm->RadioButtonDig->Checked)
  {
   query="SELECT id,secondname,firstname,thirdname,grpid,znum,cityid FROM "+TabStudName+" WHERE deleted=0 and (";
   query+="znum=\'"+FindStr+"\'";
   query+=" OR passnum=\'"+FindStr+"\'";
   query+=" OR UCASE(edudocinfo) LIKE \'\%"+FindStr+"\%\'";
   query+=" OR dognum=\'"+FindStr+"\'";
   query+=") ORDER BY secondname,firstname,thirdname";
  }
  if (FormFilterComm->RadioButtonData->Checked)
  {
   query="SELECT id,secondname,firstname,thirdname,grpid,znum,cityid FROM "+TabStudName+" WHERE deleted=0 and (";
   query+="bdate=\'"+FindStr+"\'";
   query+=" OR passdate=\'"+FindStr+"\'";
   query+=" OR eduenddate=\'"+FindStr+"\'";
   query+=" OR enterdate=\'"+FindStr+"\'";
   query+=" OR exitdate=\'"+FindStr+"\'";
   query+=") ORDER BY secondname,firstname,thirdname";
  }
  mysql_query(mysql,query.c_str());
  Memo->Lines->Add(mysql_error(mysql));

  //------------ Waiting -----------------
  FormProgress->Increment(10);
  //------------ Waiting -----------------

  if (mysql_field_count(mysql))
  {
    Memo->Lines->Add(mysql_error(mysql));
    result=mysql_store_result(mysql);
    Memo->Lines->Add(mysql_error(mysql));
    int numCurRow;
    if (result && mysql_num_rows(result))
    {
      Memo->Lines->Add(mysql_error(mysql));

      numCurRow=0;
      listIDs->Clear();
      listIDs->Add("NULL");

      while (row = mysql_fetch_row(result))
      {
        Memo->Lines->Add(mysql_error(mysql));

        //------------ Waiting -----------------
        FormProgress->Increment((FormProgress->GetMax()-FormProgress->GetPos())/(mysql_num_rows(result)-numCurRow));
        //------------ Waiting -----------------

        numCurRow++;
        pTab->RowCount=numCurRow+1;

        listIDs->Add(AnsiString(row[0]));
        pTab->Cells[1][numCurRow]=numCurRow;
        pTab->Cells[2][numCurRow]=row[1];
        pTab->Cells[3][numCurRow]=row[2];
        pTab->Cells[4][numCurRow]=row[3];
        pTab->Cells[5][numCurRow]=row[5];
        AnsiString GroupStr=FormSelected->GetTitleForKeyNum(GROUPS,AnsiString(row[4]).ToInt());
        pTab->Cells[6][numCurRow]=GroupStr;
        pTab->Cells[7][numCurRow]=FormSelected->GetCityName(GroupStr);
        //pTab->Cells[7][numCurRow]=FormSelected->GetTitleForKeyNum(CITIES,AnsiString(row[6]).ToInt());

        pTab->Refresh();
        StatusBar->Panels->Items[13]->Text=AnsiString(pTab->RowCount-1);
        StatusBar->Refresh();
      }

      ComboBoxGroup->Enabled=false;
    }
    mysql_free_result(result);
    Memo->Lines->Add(mysql_error(mysql));
  }

  StrGridDB->OnClick(pTab);
  StatusBar->Panels->Items[10]->Text=FindStr;

  //------------ Waiting -----------------
  FormProgress->Hide();
  //------------ Waiting -----------------
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ClearDBItemClick(TObject *Sender)
{
  FormClearDB->ShowModal();        
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::PlanSemItemClick(TObject *Sender)
{
      int IDGroup=FormSelected->GetNumForKeyTitle(GROUPS,ComboBoxGroup->Text);
      int IDStudent=listIDs->Strings[StrGridDB->Selection.Top].ToInt();

      FormIzmSem->SetIDStudent(IDStudent);
      FormIzmSem->SetIDGroup(IDGroup);

      FormIzmSem->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GraphSemItemClick(TObject *Sender)
{
      int IDGroup=FormSelected->GetNumForKeyTitle(GROUPS,ComboBoxGroup->Text);
      int IDStudent=listIDs->Strings[StrGridDB->Selection.Top].ToInt();

      FormIzmGraphSem->SetIDStudent(IDStudent);
      FormIzmGraphSem->SetIDGroup(IDGroup);

      FormIzmGraphSem->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::UpdateVerItemClick(TObject *Sender)
{
    char path_buf[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];

    GetModuleFileName(NULL,path_buf,sizeof(path_buf)-1);
    _splitpath(path_buf,drive,dir,0,0);
    //_makepath(path_buf,drive,dir,0,0);

    //AnsiString path_host = AnsiString(drive) + AnsiString(dir);

    AnsiString prm = "rsync.exe -ar -P //server-odo/Exchange/ODOPro/ /cygdrive/c" + AnsiString(dir);
    TReplaceFlags flags;
    flags << rfReplaceAll << rfIgnoreCase;
    prm = StringReplace(prm, "\\", "/", flags);
    prm += "\nstart odopro.exe";

    std::ofstream ifs("update.cmd", ios_base::in |
              ios_base::out | ios_base::trunc);
    ifs.write(prm.c_str(), prm.Length());
    ifs.close();
    ShellExecute( NULL, "open", "update.cmd", NULL, NULL, SW_SHOWNORMAL);
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::IzmOchenkaItemClick(TObject *Sender)
{
      int IDGroup=FormSelected->GetNumForKeyTitle(GROUPS,ComboBoxGroup->Text);
      int IDStudent=listIDs->Strings[StrGridDB->Selection.Top].ToInt();

      FormIzmOchenka->SetIDStudent(IDStudent);
      FormIzmOchenka->SetIDGroup(IDGroup);

      FormIzmOchenka->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::N1Click(TObject *Sender)
{
    FormDeltaDogovor->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::N2Click(TObject *Sender)
{
// загружает в буфер обмена список фамилий вида Фамилия И.О. номер зачетки
    AnsiString list = "";

    for (int i = 1; i < StrGridDB->RowCount; ++i)
        list += StrGridDB->Cells[2][i] + " " + (StrGridDB->Cells[3][i])[1] + "." +
            (StrGridDB->Cells[4][i])[1] + ".\t" + StrGridDB->Cells[5][i] + "\n";
    // Open the clipboard, and empty it.

    if (!OpenClipboard(Handle))
        return ;
    EmptyClipboard();
    char* buf = (char*) LocalAlloc(LMEM_FIXED, list.Length()*sizeof(char));
    if (buf == NULL)
    {
        CloseClipboard();
        return ;
    }
    memcpy(buf, list.c_str(), list.Length());
    SetClipboardData(CF_TEXT, buf);
    CloseClipboard();
}
//---------------------------------------------------------------------------


