//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitStatistic.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormStatistic *FormStatistic;

#include "mysql.h"
#include "UnitFuncs.h"

extern MYSQL *mysql;
extern EDRegOpts opts;

//---------------------------------------------------------------------------
AnsiString GetStartDateForYearInSQL(AnsiString YearStr)
{
   // Date in format yyyy convert to yyyy-01-01
   AnsiString date=YearStr+"-01-01";
   return date;
}
//---------------------------------------------------------------------------
AnsiString GetEndDateForYearInSQL(AnsiString YearStr)
{
   // Date in format yyyy convert to yyyy-12-31
   AnsiString date=YearStr+"-12-31";
   return date;
}
//---------------------------------------------------------------------------
__fastcall TFormStatistic::TFormStatistic(TComponent* Owner)
        : TForm(Owner)
{
  if ( !WCConnect())
  {
    AnsiString msg="Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormStatistic::BtnCloseClick(TObject *Sender)
{
  Close();      
}
//---------------------------------------------------------------------------
void __fastcall TFormStatistic::FormKeyPress(TObject *Sender, char &Key)
{
  if (Key==VK_ESCAPE) BtnCloseClick(BtnClose);
  if (Key==VK_RETURN) BtnStatisticClick(BtnStatistic);      
}
//---------------------------------------------------------------------------
void __fastcall TFormStatistic::FormShow(TObject *Sender)
{
  ComboBoxSex->Clear();
  ComboBoxSex->Items->Add("М");
  ComboBoxSex->Items->Add("Ж");
  ComboBoxSex->ItemIndex=0;
  MaskEditbyear->Text="1985";
  WCUpdateComboBox(ComboBoxeducation,EDUCATIONS);
  ComboBoxeducation->ItemIndex=0;
  WCUpdateComboBox(ComboBoxlanguage,LANGUAGES);
  ComboBoxlanguage->ItemIndex=0;
  WCUpdateComboBox(ComboBoxdogyear,DOGYEARS);
  ComboBoxdogyear->ItemIndex=0;
  WCUpdateComboBox(ComboBoxgroup,GROUPS);
  ComboBoxgroup->ItemIndex=0;
  WCUpdateComboBox(ComboBoxspec,SPECS);
  ComboBoxspec->ItemIndex=0;
  WCUpdateComboBox(ComboBoxeduform,EDUFORMS);
  ComboBoxeduform->ItemIndex=0;
  WCUpdateComboBox(ComboBoxdogfast,DOGFASTS);
  ComboBoxdogfast->ItemIndex=0;

  CheckBoxSexClick(CheckBoxSex);
}
//---------------------------------------------------------------------------
void __fastcall TFormStatistic::CheckBoxSexClick(TObject *Sender)
{
  if (CheckBoxSex->Checked) ComboBoxSex->Enabled=true; else ComboBoxSex->Enabled=false;
  if (CheckBoxbyear->Checked) MaskEditbyear->Enabled=true; else MaskEditbyear->Enabled=false;
  if (CheckBoxeducation->Checked) ComboBoxeducation->Enabled=true; else ComboBoxeducation->Enabled=false;
  if (CheckBoxlanguage->Checked) ComboBoxlanguage->Enabled=true; else ComboBoxlanguage->Enabled=false;
  if (CheckBoxdogyear->Checked) ComboBoxdogyear->Enabled=true; else ComboBoxdogyear->Enabled=false;
  if (CheckBoxgroup->Checked) ComboBoxgroup->Enabled=true; else ComboBoxgroup->Enabled=false;
  if (CheckBoxspec->Checked) ComboBoxspec->Enabled=true; else ComboBoxspec->Enabled=false;
  if (CheckBoxeduform->Checked) ComboBoxeduform->Enabled=true; else ComboBoxeduform->Enabled=false;
  if (CheckBoxdogfast->Checked) ComboBoxdogfast->Enabled=true; else ComboBoxdogfast->Enabled=false;
}
//---------------------------------------------------------------------------
AnsiString ToStr(AnsiString Str)
{
 AnsiString NewStr="\'"+Str+"\'";
 return NewStr;
}
//---------------------------------------------------------------------------
void __fastcall TFormStatistic::BtnStatisticClick(TObject *Sender)
{
  EDRegOpts myopts;

  AnsiString FromStr=" FROM "+myopts.DBStudTable+" AS tst";
  AnsiString WhereStr=" WHERE tst.deleted=0 ";
  AnsiString MsgStr="";
  AnsiString Strtmp;
  if (CheckBoxSex->Checked)
  {
   WhereStr+="and tst.sex=\'"+ComboBoxSex->Text+"\' ";
   MsgStr+="\n\tпол: "+ToStr(ComboBoxSex->Text);
  }
  if (CheckBoxbyear->Checked)
  {
   Strtmp=MaskEditbyear->Text;
   WhereStr+="and tst.bdate>="+ToStr(GetStartDateForYearInSQL(Strtmp))+" and tst.bdate<="+ToStr(GetEndDateForYearInSQL(Strtmp))+" ";
   MsgStr+="\n\tгод рождения: "+ToStr(Strtmp);
  }
  if (CheckBoxeducation->Checked)
  {
   Strtmp=IntToStr(WCGetNumForKeyTitle(EDUCATIONS,ComboBoxeducation->Text));//GetNumForKeyTitle(EDUCATIONS,ComboBoxeducation->Text);
   WhereStr+="and tvoceduc.deleted=0 and tvoceduc.vkey=\'education\' and tst.educationid=tvoceduc.num and tst.educationid="+ToStr(Strtmp)+" ";
   FromStr+=","+myopts.DBVocTable+" AS tvoceduc";
   MsgStr+="\n\tобразование: "+ToStr(ComboBoxeducation->Text);
  }
  if (CheckBoxlanguage->Checked)
  {
   Strtmp=IntToStr(WCGetNumForKeyTitle(LANGUAGES,ComboBoxlanguage->Text));
   WhereStr+="and tvoclang.deleted=0 and tvoclang.vkey=\'language\' and tst.languageid=tvoclang.num and tst.languageid="+ToStr(Strtmp)+" ";
   FromStr+=","+myopts.DBVocTable+" AS tvoclang";
   MsgStr+="\n\tязык: "+ToStr(ComboBoxlanguage->Text);
  }
  if (CheckBoxdogyear->Checked)
  {
   Strtmp=IntToStr(WCGetNumForKeyTitle(DOGYEARS,ComboBoxdogyear->Text));
   WhereStr+="and tvocdogyear.deleted=0 and tvocdogyear.vkey=\'dogyear\' and tst.dogyearid=tvocdogyear.num and tst.dogyearid="+ToStr(Strtmp)+" ";
   FromStr+=","+myopts.DBVocTable+" AS tvocdogyear";
   MsgStr+="\n\tгод поступления: "+ToStr(ComboBoxdogyear->Text);
  }
  if (CheckBoxgroup->Checked)
  {
   Strtmp=IntToStr(WCGetNumForKeyTitle(GROUPS,ComboBoxgroup->Text));
   WhereStr+="and tvocgroup.deleted=0 and tvocgroup.vkey=\'grp\' and tst.grpid=tvocgroup.num and tst.grpid="+ToStr(Strtmp)+" ";
   FromStr+=","+myopts.DBVocTable+" AS tvocgroup";
   MsgStr+="\n\tгруппа: "+ToStr(ComboBoxgroup->Text);
  }
  if (CheckBoxspec->Checked)
  {
   Strtmp=IntToStr(WCGetNumForKeyTitle(SPECS,ComboBoxspec->Text));
   WhereStr+="and tvocspec.deleted=0 and tvocspec.vkey=\'spec\' and tst.specid=tvocspec.num and tst.specid="+ToStr(Strtmp)+" ";
   FromStr+=","+myopts.DBVocTable+" AS tvocspec";
   MsgStr+="\n\tспециальность: "+ToStr(ComboBoxspec->Text);
  }
  if (CheckBoxeduform->Checked)
  {
   Strtmp=IntToStr(WCGetNumForKeyTitle(EDUFORMS,ComboBoxeduform->Text));
   WhereStr+="and tvoceduform.deleted=0 and tvoceduform.vkey=\'eduform\' and tst.eduformid=tvoceduform.num and tst.eduformid="+ToStr(Strtmp)+" ";
   FromStr+=","+myopts.DBVocTable+" AS tvoceduform";
   MsgStr+="\n\tформа обучения: "+ToStr(ComboBoxeduform->Text);
  }
  if (CheckBoxdogfast->Checked)
  {
   Strtmp=IntToStr(WCGetNumForKeyTitle(DOGFASTS,ComboBoxdogfast->Text));
   WhereStr+="and tvocdogfast.deleted=0 and tvocdogfast.vkey=\'dogfast\' and tst.dogfastid=tvocdogfast.num and tst.dogfastid="+ToStr(Strtmp);
   FromStr+=","+myopts.DBVocTable+" AS tvocdogfast";
   MsgStr+="\n\tскорость обучения: "+ToStr(ComboBoxdogfast->Text);
  }
  if (CheckBoxIgnore->Checked)
  {
   AnsiString IgnorePatternGroup="\%ОТЧИСЛ\%";
   WhereStr+="and tvocigngrp.deleted=0 and tvocigngrp.vkey=\'grp\' and tst.grpid=tvocigngrp.num and tvocigngrp.title not like "+ToStr(IgnorePatternGroup)+" ";
   FromStr+=","+myopts.DBVocTable+" AS tvocigngrp";
   MsgStr+="\n\t(без учета отчисленных студентов)";
  }
  else
  {
    MsgStr+="\n\t(с учетом отчисленных студентов)";
  }



  MYSQL_RES *result;
  MYSQL_ROW row;
  long CountStudents=0;

  AnsiString myquery;
  myquery="SELECT COUNT(*)";// FROM "+myopts.DBStudTable+" AS tst,"+myopts.DBVocTable+" AS tvoceduc,"+myopts.DBVocTable+" AS tvoclang,"+myopts.DBVocTable+" AS tvocdogyear,"+myopts.DBVocTable+" AS tvocgroup,"+myopts.DBVocTable+" AS tvocspec,"+myopts.DBVocTable+" AS tvoceduform,"+myopts.DBVocTable+" AS tvocdogfast";
  myquery+=FromStr;
  myquery+=WhereStr;
  mysql_query(mysql,myquery.c_str());
  Memo->Lines->Add(mysql_error(mysql));

  if (mysql_field_count(mysql))
  {
    Memo->Lines->Add(mysql_error(mysql));
    result=mysql_store_result(mysql);
    Memo->Lines->Add(mysql_error(mysql));
    if (result && mysql_num_rows(result))
    {
      Memo->Lines->Add(mysql_error(mysql));
      row = mysql_fetch_row(result);
      Memo->Lines->Add(mysql_error(mysql));
      CountStudents=AnsiString(row[0]).ToInt();
    }
    mysql_free_result(result);
    Memo->Lines->Add(mysql_error(mysql));
  }
  if (MsgStr.IsEmpty()) MsgStr="\n\t(все студенты, занесенные в базу данных)";
  MsgStr="Количество студентов, данные которых удовлетворяют следующим условиям\n"+MsgStr+"\n\nравно "+AnsiString(CountStudents)+".";
  MessageBox(Handle,MsgStr.c_str(),"статистика:",MB_OK|MB_ICONINFORMATION);


}
//---------------------------------------------------------------------------
void __fastcall TFormStatistic::MaskEditbyearExit(TObject *Sender)
{
  if (!IsTrueYear(MaskEditbyear->Text))
  {
    MessageBox(Handle,"Задайте правильно год (год - это число, состоящее из 4-х цифр).\nБудьте внимательны.","Ошибка:",MB_OK|MB_ICONERROR);
    MaskEditbyear->SetFocus();
  }
}
//---------------------------------------------------------------------------
bool __fastcall TFormStatistic::IsTrueYear(AnsiString YearStr)
{
  bool Status=true;
  try
  {
    int YearInt=YearStr.ToInt();
    if (YearInt<1000 && YearInt>9999) Status=false;
  }
  catch(...)
  {
    Status=false;
  }
  return Status;
}
//---------------------------------------------------------------------------

void __fastcall TFormStatistic::FormDestroy(TObject *Sender)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------

