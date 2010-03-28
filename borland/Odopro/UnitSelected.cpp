//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSelected.h"
#include "MainUnit.h"
#include "UnitWorkDLL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSelected *FormSelected;

MYSQL* mysql;
AnsiString query;
TTypeQuery tquery=VIEW;
AnsiString keys[15]={ "grp",
                      "city",
                      "spec",
                      "specializ",
                      "qualific",
                      "dogshifr",
                      "dogfast",
                      "dogyear",
                      "education",
                      "edudoc",
                      "eduform",
                      "language",
                      "falseedudocinfo",
                      "shifrspec",
                      "discipclassific"
                     };
AnsiString Hintkeys[15]={"группа (grp)",
                         "город (city)",
                         "специальность (spec)",
                         "специализация (specializ)",
                         "квалификация (qualific)",
                         "шифр договора (dogshifr)",
                         "скорость обучения (dogfast)",
                         "год закл. договора (dogyear)",
                         "образование (education)",
                         "документ об образовании (edudoc)",
                         "форма обучения (eduform)",
                         "язык (language)",
                         "номер фальшивого документа (falseedudocinfo)",
                         "шифр специальности (shifrspec)",
                         "классификация дисциплин (discipclassific)",
                        };
AnsiString TabStudName;
AnsiString TabVocName;
AnsiString idselected;
TStringList* listIDs;

int IndexOfCurActivePage=0;

struct TSavedData
{
  AnsiString PassPlace;
  AnsiString Addr,LiveAddr;
  AnsiString EduSpec,EduPlace;
  AnsiString EnterNum;
} SavedData;

//---------------------------------------------------------------------------
__fastcall TFormSelected::TFormSelected(TComponent* Owner)
        : TForm(Owner)
{
   Constraints->MinWidth=Width;
   Constraints->MinHeight=Height;

   PageControlSelected->ActivePageIndex=0;
}
//---------------------------------------------------------------------------
//----------my functions-----------------------------------------------------
AnsiString ReturnParsedDateInSQL(AnsiString Str)
{
   // Date in format dd.mm.yyyy convert to yyyy-mm-dd
   AnsiString y=Str.SubString(7,4),
              m=Str.SubString(4,2),
              d=Str.SubString(1,2),
              date;
   date=y+"-"+m+"-"+d;
   return date;
}
//---------------------------------------------------------------------------
AnsiString ReturnParsedDateInMask(AnsiString Str)
{
   // Date in format yyyy-mm-dd convert to dd.mm.yyyy
   AnsiString y=Str.SubString(1,4),
              m=Str.SubString(6,2),
              d=Str.SubString(9,2),
              date;
   date=d+"."+m+"."+y;
   return date;
}
//---------------------------------------------------------------------------
AnsiString ReturnDateInSQLForYear(AnsiString yearStr)
{
   // Date in format yyyy convert to yyyy-01-01
   AnsiString date;
   date=yearStr.SubString(1,4)+"-01-01";
   return date;
}
//---------------------------------------------------------------------------
AnsiString ReturnYearInMaskForDate(AnsiString Str)
{
   // Date in format yyyy-mm-dd convert to yyyy
   AnsiString y=Str.SubString(1,4);
   return y;
}
//---------------------------------------------------------------------------
AnsiString ToStr(AnsiString str)
{
  return "\'"+str+"\'";
}
//---------------------------------------------------------------------------
bool __fastcall TFormSelected::IsExistsDogovorNumber(AnsiString& _FIOStr,int& _groupid)
{
  MYSQL_RES *result;
  MYSQL_ROW row;

  bool IsExists=false;
  _FIOStr="??? ??? ???";
  _groupid=-1;
  AnsiString myquery;
  myquery="SELECT id,CONCAT(secondname,\' \',firstname,\' \',thirdname),grpid FROM "+TabStudName+" WHERE dogshifrid="+ToStr(AnsiString(GetNumForKeyTitle(DOGSHIFRS,ComboBoxdogshifr->Text)))+" AND dogyearid="+ToStr(AnsiString(GetNumForKeyTitle(DOGYEARS,ComboBoxdogyear->Text)))+" AND dogfastid="+ToStr(AnsiString(GetNumForKeyTitle(DOGFASTS,ComboBoxdogfast->Text)))+" AND dognum="+ToStr(Editdognum->Text)+" AND id<>"+ToStr(AnsiString(idselected))+" AND deleted=0";

  mysql_query(mysql,myquery.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      row = mysql_fetch_row(result);
      IsExists=true;
      _FIOStr=AnsiString(row[1]);
      _groupid=AnsiString(row[2]).ToInt();
    }
    mysql_free_result(result);
  }

  return IsExists;
}
//---------------------------------------------------------------------------
bool __fastcall TFormSelected::IsExistsUserNuke(AnsiString znumStr)
{
  MYSQL_RES *result;
  MYSQL_ROW row;

  bool IsExists=false;;
  AnsiString myquery;
  myquery="SELECT username FROM nuke_users WHERE user_password="+ToStr(znumStr);

  mysql_query(mysql,myquery.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result)) IsExists=true;
    else IsExists=false;
    mysql_free_result(result);
  }

  return IsExists;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::DeleteUserNuke(AnsiString znumStr)
{
  AnsiString myquery;
  myquery="DELETE FROM nuke_users WHERE user_password="+ToStr(znumStr);
  mysql_query(mysql,myquery.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::InsertUserNuke(AnsiString user_id,AnsiString username,AnsiString user_email,AnsiString user_password,AnsiString user_avatar)
{
  AnsiString myquery;
  myquery="INSERT INTO nuke_users (user_id,username,user_email,user_password,user_avatar) VALUES("+ToStr(user_id)+","+ToStr(username)+","+ToStr(user_email)+","+ToStr(user_password)+","+ToStr(user_avatar)+")";
  if (!IsExistsUserNuke(user_password)) mysql_query(mysql,myquery.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::ChangeUserNuke(AnsiString znumoldStr,AnsiString znumnewStr)
{
  AnsiString myquery;
  myquery="UPDATE nuke_users SET user_password="+ToStr(znumnewStr)+" WHERE user_password="+ToStr(znumoldStr);
  if (!IsExistsUserNuke(znumnewStr)) mysql_query(mysql,myquery.c_str());
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormSelected::GetZNumForStudent(AnsiString IDStudStr)
{
  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString resStr="";
  AnsiString myquery;
  myquery="SELECT znum FROM "+TabStudName+" WHERE id="+ToStr(IDStudStr);

  mysql_query(mysql,myquery.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      row = mysql_fetch_row(result);
      resStr=AnsiString(row[0]);
    }
    mysql_free_result(result);
  }
  return resStr;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormSelected::GetTitleForKeyNum(TTypeKeyVoc tkey,int num)
{
  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString resStr="";
  AnsiString myquery;
  myquery="SELECT title FROM "+TabVocName+" WHERE deleted=0 AND vkey="+ToStr(keys[tkey])+" AND num="+ToStr(AnsiString(num));

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
      Memo1->Lines->Add(mysql_error(mysql));

      resStr=AnsiString(row[0]);
    }
    mysql_free_result(result);
    Memo1->Lines->Add(mysql_error(mysql));
  }

  return resStr;
}
//---------------------------------------------------------------------------
int __fastcall TFormSelected::GetNumForKeyTitle(TTypeKeyVoc tkey, AnsiString title)
{
  MYSQL_RES *result;
  MYSQL_ROW row;

  int resNum=0;
  AnsiString myquery;
  myquery="SELECT num FROM "+TabVocName+" WHERE deleted=0 AND vkey="+ToStr(keys[tkey])+" AND title="+ToStr(title);

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
      Memo1->Lines->Add(mysql_error(mysql));

      resNum=AnsiString(row[0]).ToInt();
    }
    mysql_free_result(result);
    Memo1->Lines->Add(mysql_error(mysql));
  }

  return resNum;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::UpdateComboBox(TComboBox* cmb,TTypeKeyVoc tkey)
{
  cmb->Clear();

  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString myquery;
  if (tkey==GROUPS)
        myquery="SELECT title, num FROM "+TabVocName+" WHERE vkey="+ToStr(keys[tkey])+" AND deleted=0 ORDER BY title";
  else
        myquery="SELECT title, num FROM "+TabVocName+" WHERE vkey="+ToStr(keys[tkey])+" AND deleted=0 ORDER BY num";
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
      while (row = mysql_fetch_row(result))
      {
        Memo1->Lines->Add(mysql_error(mysql));

        cmb->Items->Add(AnsiString(row[0]));
      }
    }
    mysql_free_result(result);
    Memo1->Lines->Add(mysql_error(mysql));
  }
}
//---------------------------------------------------------------------------
bool __fastcall TFormSelected::IsFalseEduDocInfo(void)
{
  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString myquery;
  myquery="SELECT title FROM "+TabVocName+" WHERE vkey="+ToStr(keys[FALSEEDUDOCINFOS])+" AND deleted=0 AND title="+ToStr(Editedudocinfo->Text);
  mysql_query(mysql,myquery.c_str());
  Memo1->Lines->Add(mysql_error(mysql));

  bool status=false;
  if (mysql_field_count(mysql))
  {
    Memo1->Lines->Add(mysql_error(mysql));
    result=mysql_store_result(mysql);
    Memo1->Lines->Add(mysql_error(mysql));
    if (result && mysql_num_rows(result))
    {
      status=true;
      Memo1->Lines->Add(mysql_error(mysql));
    }

    mysql_free_result(result);
    Memo1->Lines->Add(mysql_error(mysql));
  }

  return status;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::BtnOkClick(TObject *Sender)
{
   if (UpDownStudent->Enabled)
     idselected=listIDs->Strings[UpDownStudent->Position]; // изменение сущ-ей записи
   else
     idselected=-1; // вставка новой записи

   int IDGroupBefore=-1,
       IDGroupAfter=-1;
   int IDSpecBefore=-1,
       IDSpecAfter=-1;

   AnsiString FIOStr;
   int groupid;
   if (IsExistsDogovorNumber(FIOStr,groupid))
   {
     AnsiString Msg,Str;
     Str=ComboBoxdogshifr->Text+"-"+ComboBoxdogyear->Text;
     if (!ComboBoxdogfast->Text.IsEmpty()) Str+=ComboBoxdogfast->Text;
     Str+="-"+Editdognum->Text;
     Msg="Студент с шифром договора :  "+Str+"  уже существует в базе данных: \n"+FIOStr+" (группа: "+GetTitleForKeyNum(GROUPS,groupid)+").\nНеобходима уникальность этого значения.\nИсправьте пожалуйста.";
     MessageBox(Handle,Msg.c_str(),"Сообщение",MB_OK|MB_ICONINFORMATION);
     return;
   }
   if (IsExistsZnum(FIOStr,groupid))
   {
     AnsiString Msg,Str;
     Str = Editznum->Text;
     Msg="Студент с номером зачетки :  "+Str+"  уже существует в базе данных: \n"+FIOStr+" (группа: "+GetTitleForKeyNum(GROUPS,groupid)+").\nВы хотите продолжать дальше ?";
     if (IDNO == MessageBox(Handle,Msg.c_str(),"Сообщение",MB_YESNO|MB_ICONINFORMATION))
        return;
   }


   bool flagReturn=false;
   AnsiString msg;
   if (tquery==INSERT)
   {
// изменил
     query="INSERT INTO "+TabStudName+" (secondname,firstname,thirdname,sex,bdate,grpid,znum,cityid";
     query=query+",specid,dogshifrid,dogfastid,dogyearid,dognum,passseries,passnum,passdate,passplace,email,addr,liveaddr,phones";
     query=query+",educationid,edudocid,edudocinfo,eduplace,eduenddate,eduformid,eduspec,languageid,misc";
     query=query+",enternum,enterdate,exitnum,exitdate,edunumdiplom,edunumreg,edudatediplom,edudatequalif,edudiplomotl,edunumprotgak) ";

     query=query+"VALUES("+ToStr(EditSecondName->Text)+","+ToStr(EditFirstName->Text)+","+ToStr(EditThirdName->Text)+",";
     query=query+ToStr(ComboBoxSex->Text)+","+ToStr(ReturnParsedDateInSQL(MaskEditbdate->Text))+","+ToStr(GetNumForKeyTitle(GROUPS,ComboBoxgrp->Text))+",";
     query=query+ToStr(Editznum->Text)+","+ToStr(GetNumForKeyTitle(CITIES,ComboBoxcity->Text))+","+ToStr(GetNumForKeyTitle(SPECS,ComboBoxspec->Text))+",";
     query=query+ToStr(GetNumForKeyTitle(DOGSHIFRS,ComboBoxdogshifr->Text))+","+ToStr(GetNumForKeyTitle(DOGFASTS,ComboBoxdogfast->Text))+",";
     query=query+ToStr(GetNumForKeyTitle(DOGYEARS,ComboBoxdogyear->Text))+","+ToStr(Editdognum->Text)+","+ToStr(Editpassseries->Text)+",";
     query=query+ToStr(Editpassnum->Text)+","+ToStr(ReturnParsedDateInSQL(MaskEditpassdate->Text))+","+ToStr(Editpassplace->Text)+",";
     query=query+ToStr(Editemail->Text)+","+ToStr(Editaddr->Text)+","+ToStr(Editliveaddr->Text)+","+ToStr(Editphones->Text)+",";
     query=query+ToStr(GetNumForKeyTitle(EDUCATIONS,ComboBoxeducation->Text))+","+ToStr(GetNumForKeyTitle(EDUDOCS,ComboBoxedudoc->Text))+","+ToStr(Editedudocinfo->Text)+",";
     query=query+ToStr(Editeduplace->Text)+","+ToStr(ReturnDateInSQLForYear(MaskEditeduenddate->Text))+","+ToStr(GetNumForKeyTitle(EDUFORMS,ComboBoxeduform->Text))+",";
     query=query+ToStr(Editeduspec->Text)+","+ToStr(GetNumForKeyTitle(LANGUAGES,ComboBoxlanguage->Text))+","+ToStr(Memomisc->Text)+",";
     query=query+ToStr(Editenternum->Text)+","+ToStr(ReturnParsedDateInSQL(MaskEditenterdate->Text))+","+ToStr(Editexitnum->Text)+","+ToStr(ReturnParsedDateInSQL(MaskEditexitdate->Text))+",";
// добавил
     query=query+ToStr(MaskEditedunumdiplom->Text)+","+ToStr(MaskEditedunumreg->Text)+","+ToStr(ReturnParsedDateInSQL(MaskEditedudatediplom->Text))+","+ToStr(ReturnParsedDateInSQL(MaskEditedudatequalif->Text))+",";
     AnsiString chk;
     if(CheckBoxedudiplomotl->Checked)
        chk = "1";
     else
        chk = "0";

     query=query + chk + "," + ToStr(MaskEditedunumprotgak->Text) + ")";

     if (IsFalseEduDocInfo())
     {
       msg="Обратите внимание:\nвведенные данные ( "+Editedudocinfo->Text+" ) документа об образовании студента "+EditSecondName->Text+" совпадают с заложенными ранее в базу данных фальшивыми данными.";
       msg=msg+"\nВы действительно хотите добавить студента "+EditSecondName->Text+" в вашу базу данных?";
       if (MessageBox(Handle,msg.c_str(),"Предупреждение!!!",MB_YESNO|MB_ICONINFORMATION)==ID_YES)
       {
         flagReturn=true;  // to use query
       }
       else flagReturn=false;                // not to use query
     }
     else
       flagReturn=true;    // to use query
   }
   else
     if (tquery==UPDATE)
     {
       IDGroupBefore=GetIDGroupForStudent(AnsiString(idselected));
       IDSpecBefore=GetIDSpecForStudent(AnsiString(idselected));

       query="UPDATE "+TabStudName+" SET secondname="+ToStr(EditSecondName->Text)+",firstname="+ToStr(EditFirstName->Text)+",thirdname="+ToStr(EditThirdName->Text)+",";
       query=query+"sex="+ToStr(ComboBoxSex->Text)+",bdate="+ToStr(ReturnParsedDateInSQL(MaskEditbdate->Text))+",grpid="+ToStr(GetNumForKeyTitle(GROUPS,ComboBoxgrp->Text))+",";
       query=query+"znum="+ToStr(Editznum->Text)+",cityid="+ToStr(GetNumForKeyTitle(CITIES,ComboBoxcity->Text))+",specid="+ToStr(GetNumForKeyTitle(SPECS,ComboBoxspec->Text))+",";
       query=query+"dogshifrid="+ToStr(GetNumForKeyTitle(DOGSHIFRS,ComboBoxdogshifr->Text))+",dogfastid="+ToStr(GetNumForKeyTitle(DOGFASTS,ComboBoxdogfast->Text))+",";
       query=query+"dogyearid="+ToStr(GetNumForKeyTitle(DOGYEARS,ComboBoxdogyear->Text))+",dognum="+ToStr(Editdognum->Text)+",passseries="+ToStr(Editpassseries->Text)+",";
       query=query+"passnum="+ToStr(Editpassnum->Text)+",passdate="+ToStr(ReturnParsedDateInSQL(MaskEditpassdate->Text))+",passplace="+ToStr(Editpassplace->Text)+",";
       query=query+"email="+ToStr(Editemail->Text)+",addr="+ToStr(Editaddr->Text)+",liveaddr="+ToStr(Editliveaddr->Text)+",phones="+ToStr(Editphones->Text)+",";
       query=query+"educationid="+ToStr(GetNumForKeyTitle(EDUCATIONS,ComboBoxeducation->Text))+",edudocid="+ToStr(GetNumForKeyTitle(EDUDOCS,ComboBoxedudoc->Text))+",edudocinfo="+ToStr(Editedudocinfo->Text)+",";
       query=query+"eduplace="+ToStr(Editeduplace->Text)+",eduenddate="+ToStr(ReturnDateInSQLForYear(MaskEditeduenddate->Text))+",eduformid="+ToStr(GetNumForKeyTitle(EDUFORMS,ComboBoxeduform->Text))+",";
       query=query+"eduspec="+ToStr(Editeduspec->Text)+",languageid="+ToStr(GetNumForKeyTitle(LANGUAGES,ComboBoxlanguage->Text))+",misc="+ToStr(Memomisc->Text)+",";
       query=query+"enternum="+ToStr(Editenternum->Text)+",enterdate="+ToStr(ReturnParsedDateInSQL(MaskEditenterdate->Text))+",exitnum="+ToStr(Editexitnum->Text)+",exitdate="+ToStr(ReturnParsedDateInSQL(MaskEditexitdate->Text))+",";
// добавил   --------------------------------------------------------------------------------
       query=query+"edunumdiplom="+ToStr(MaskEditedunumdiplom->Text)+",edunumreg="+ToStr(MaskEditedunumreg->Text)+",edudatediplom="+ToStr(ReturnParsedDateInSQL(MaskEditedudatediplom->Text))+",edudatequalif="+ToStr(ReturnParsedDateInSQL(MaskEditedudatequalif->Text))+",";
       AnsiString chk;
       if(CheckBoxedudiplomotl->Checked)
          chk = "1";
       else
          chk = "0";
       query=query+"edudiplomotl="+chk+",edunumprotgak="+ToStr(MaskEditedunumprotgak->Text)+" ";

       query=query+"WHERE id="+AnsiString(idselected);

       if (IsFalseEduDocInfo())
       {
         msg="Обратите внимание:\nизмененные данные ( "+Editedudocinfo->Text+" ) документа об образовании студента "+EditSecondName->Text+" совпадают с заложенными ранее в базу данных фальшивыми данными.";
         msg=msg+"\nВы действительно хотите изменить данные студента "+EditSecondName->Text+" в вашей базе данных?";
         if (MessageBox(Handle,msg.c_str(),"Предупреждение!!!",MB_YESNO|MB_ICONINFORMATION)==ID_YES)
         {
           flagReturn=true;  // to use query
         }
         else flagReturn=false;                // not to  use query
       }
       else
         flagReturn=true;    // to use query
     }
     else
       flagReturn=false;   // not to  use query

   if (flagReturn)
   {
        AnsiString user_avatar="gallery/blank.gif";
        if (tquery==INSERT)
        {
          InsertUserNuke("NULL",Editznum->Text,Editemail->Text,Editznum->Text,user_avatar);
        }
        else
        {
          if (tquery==UPDATE)
          {
            AnsiString OldZNum=GetZNumForStudent(AnsiString(idselected));
            if (OldZNum.IsEmpty() || OldZNum=="000000")
            {
              if (!IsExistsUserNuke(Editznum->Text)) InsertUserNuke("NULL",Editznum->Text,Editemail->Text,Editznum->Text,user_avatar);
            }
            else ChangeUserNuke(OldZNum,Editznum->Text);
          }
        }

        mysql_query(mysql,query.c_str());
        Memo1->Lines->Add(mysql_error(mysql));

        if (tquery==UPDATE)
        {
          IDGroupAfter=GetIDGroupForStudent(AnsiString(idselected));
          if (IDGroupAfter!=IDGroupBefore)
                UpdateAllPayFactsForStudents(idselected.ToInt(),IDGroupBefore,IDGroupAfter);
          IDSpecAfter=GetIDSpecForStudent(AnsiString(idselected));
          if (IDSpecAfter!=IDSpecBefore)
                UpdateAllProgressForStudents(idselected.ToInt(),IDSpecBefore,IDSpecAfter);
        }
        if (tquery==INSERT)
        {
          SavedData.PassPlace=Editpassplace->Text+" (исправить)";
          SavedData.Addr=Editaddr->Text+" (исправить)";
          SavedData.LiveAddr=Editliveaddr->Text+" (исправить)";
          SavedData.EduSpec=Editeduspec->Text+" (исправить)";
          SavedData.EduPlace=Editeduplace->Text+" (исправить)";
          SavedData.EnterNum=Editenternum->Text+" (исправить)";
        }
   }
}
//---------------------------------------------------------------------------
AnsiString GetCurDateInStr(void)
{
  TDateTime dt,
            curdate=dt.CurrentDate();
  return curdate.DateString();
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::FormShow(TObject *Sender)
{
  EDRegOpts opts;
  BtnWorkVoc->Enabled=opts.ACCESSEditVoc;

//  IndexOfCurActivePage=PageControlSelected->ActivePageIndex;
//  PageControlSelected->ActivePageIndex=0;

  UpdateComboBox(ComboBoxeducation,EDUCATIONS);
  UpdateComboBox(ComboBoxedudoc,EDUDOCS);
  UpdateComboBox(ComboBoxeduform,EDUFORMS);
  UpdateComboBox(ComboBoxlanguage,LANGUAGES);

  UpdateComboBox(ComboBoxgrp,GROUPS);
  UpdateComboBox(ComboBoxspec,SPECS);
  UpdateComboBox(ComboBoxcity,CITIES);
  UpdateComboBox(ComboBoxdogshifr,DOGSHIFRS);
  UpdateComboBox(ComboBoxdogfast,DOGFASTS);
  UpdateComboBox(ComboBoxdogyear,DOGYEARS);

  if (tquery==INSERT)
  {
     UpDownStudent->Enabled=false;

     EditSecondName->Text="";
     EditFirstName->Text="";
     EditThirdName->Text="";
     ComboBoxSex->Text="";          ComboBoxSex->ItemIndex=0;
     MaskEditbdate->Text="";        MaskEditbdate->Text="00.00.0000";
     Editpassseries->Text="";
     Editpassnum->Text="";
     MaskEditpassdate->Text="";     MaskEditpassdate->Text="00.00.0000";
     Editpassplace->Text="";        Editpassplace->Text=SavedData.PassPlace;
     Editaddr->Text="";             Editaddr->Text=SavedData.Addr;
     Editliveaddr->Text="";         Editliveaddr->Text=SavedData.LiveAddr;
     Editphones->Text="";
     Editemail->Text="";

     Editznum->Text="";
     ComboBoxgrp->Text="";          //ComboBoxgrp->ItemIndex=0;
     ComboBoxgrp->ItemIndex=ComboBoxgrp->Items->IndexOf(MainForm->ComboBoxGroup->Text);
     ComboBoxcity->Text="";         ComboBoxcity->ItemIndex=0;
     ComboBoxspec->Text="";         ComboBoxspec->ItemIndex=0;
     //ComboBoxspec->Enabled=true;
     ComboBoxdogshifr->Text="";     ComboBoxdogshifr->ItemIndex=0;
     ComboBoxdogfast->Text="";      ComboBoxdogfast->ItemIndex=0;
     ComboBoxdogyear->Text="";      ComboBoxdogyear->ItemIndex=0;
     Editdognum->Text="";
     Editenternum->Text="";         Editenternum->Text=SavedData.EnterNum;
     MaskEditenterdate->Text="";    MaskEditenterdate->Text="00.00.0000";
     Editexitnum->Text="";
     MaskEditexitdate->Text="";     MaskEditexitdate->Text="00.00.0000";
     Memomisc->Text="";

     ComboBoxeducation->Text="";    ComboBoxeducation->ItemIndex=0;
     Editeduspec->Text="";          Editeduspec->Text=SavedData.EduSpec;
     ComboBoxedudoc->Text="";       ComboBoxedudoc->ItemIndex=0;
     Editedudocinfo->Text="";
     Editeduplace->Text="";         Editeduplace->Text=SavedData.EduPlace;
     ComboBoxeduform->Text="";      ComboBoxeduform->ItemIndex=0;
     MaskEditeduenddate->Text="";   MaskEditeduenddate->Text="0000";
     ComboBoxlanguage->Text="";     ComboBoxlanguage->ItemIndex=0;
  // добавил
     MaskEditedunumdiplom->Text = "";
     MaskEditedunumreg->Text = "";
     MaskEditedudatediplom->Text = "00.00.0000";//opts.DateDiplom;
     MaskEditedudatequalif->Text = "00.00.0000";//opts.DateQualif;
     CheckBoxedudiplomotl->Checked = false;
     MaskEditedunumprotgak->Text = "";

     ComboBoxgrpChange(ComboBoxgrp);
  }
  else
    if (tquery==UPDATE)
    {
       idselected=listIDs->Strings[UpDownStudent->Position];

       UpDownStudent->Enabled=true;

       MYSQL_RES *result;
       MYSQL_ROW row;
 // добавил
       query="SELECT secondname,firstname,thirdname,sex,bdate,grpid,znum,cityid";
       query=query+",specid,dogshifrid,dogfastid,dogyearid,dognum,passseries,passnum,passdate,passplace,email,addr,liveaddr,phones";
       query=query+",educationid,edudocid,edudocinfo,eduplace,eduenddate,eduformid,eduspec,languageid,misc,enternum,enterdate";
       query=query+",exitnum,exitdate,edunumdiplom,edunumreg,edudatediplom,edudatequalif,edudiplomotl,edunumprotgak FROM "+TabStudName+" WHERE id="+AnsiString(idselected);

       mysql_query(mysql,query.c_str());
       Memo1->Lines->Add(mysql_error(mysql));
       if (mysql_field_count(mysql))
       {
         Memo1->Lines->Add(mysql_error(mysql));
         result=mysql_store_result(mysql);
         Memo1->Lines->Add(mysql_error(mysql));
         if (result && mysql_num_rows(result))
         {
           Memo1->Lines->Add(mysql_error(mysql));
           while (row = mysql_fetch_row(result))
           {
                Memo1->Lines->Add(mysql_error(mysql));

                EditSecondName->Text=row[0];
                EditFirstName->Text=row[1];
                EditThirdName->Text=row[2];
                ComboBoxSex->ItemIndex=ComboBoxSex->Items->IndexOf(AnsiString(row[3]));
                MaskEditbdate->Text=ReturnParsedDateInMask(AnsiString(row[4]));
                Editpassseries->Text=row[13];
                Editpassnum->Text=row[14];
                MaskEditpassdate->Text=ReturnParsedDateInMask(AnsiString(row[15]));
                Editpassplace->Text=row[16];
                Editaddr->Text=row[18];
                Editliveaddr->Text=row[19];
                Editphones->Text=row[20];
                Editemail->Text=row[17];

                AnsiString Strtmp=GetTitleForKeyNum(GROUPS,AnsiString(row[5]).ToInt());
                ComboBoxgrp->ItemIndex=ComboBoxgrp->Items->IndexOf(GetTitleForKeyNum(GROUPS,AnsiString(row[5]).ToInt()));
                ComboBoxspec->ItemIndex=ComboBoxspec->Items->IndexOf(GetTitleForKeyNum(SPECS,AnsiString(row[8]).ToInt()));
                //ComboBoxspec->Enabled=false;
                Editznum->Text=row[6];
                ComboBoxcity->ItemIndex=ComboBoxcity->Items->IndexOf(GetTitleForKeyNum(CITIES,AnsiString(row[7]).ToInt()));
                ComboBoxdogshifr->ItemIndex=ComboBoxdogshifr->Items->IndexOf(GetTitleForKeyNum(DOGSHIFRS,AnsiString(row[9]).ToInt()));
                ComboBoxdogfast->ItemIndex=ComboBoxdogfast->Items->IndexOf(GetTitleForKeyNum(DOGFASTS,AnsiString(row[10]).ToInt()));
                ComboBoxdogyear->ItemIndex=ComboBoxdogyear->Items->IndexOf(GetTitleForKeyNum(DOGYEARS,AnsiString(row[11]).ToInt()));
                Editdognum->Text=row[12];
                Editenternum->Text=row[30];
                MaskEditenterdate->Text=ReturnParsedDateInMask(AnsiString(row[31]));
                Editexitnum->Text=row[32];
                MaskEditexitdate->Text=ReturnParsedDateInMask(AnsiString(row[33]));
                Memomisc->Text=row[29];

                ComboBoxeducation->ItemIndex=ComboBoxeducation->Items->IndexOf(GetTitleForKeyNum(EDUCATIONS,AnsiString(row[21]).ToInt()));
                Editeduspec->Text=row[27];
                ComboBoxedudoc->ItemIndex=ComboBoxedudoc->Items->IndexOf(GetTitleForKeyNum(EDUDOCS,AnsiString(row[22]).ToInt()));
                Editedudocinfo->Text=row[23];
                Editeduplace->Text=row[24];
                ComboBoxeduform->ItemIndex=ComboBoxeduform->Items->IndexOf(GetTitleForKeyNum(EDUFORMS,AnsiString(row[26]).ToInt()));
                MaskEditeduenddate->Text=ReturnYearInMaskForDate(AnsiString(row[25]));
                ComboBoxlanguage->ItemIndex=ComboBoxlanguage->Items->IndexOf(GetTitleForKeyNum(LANGUAGES,AnsiString(row[28]).ToInt()));
         // добавил
                MaskEditedunumdiplom->Text = row[34];
                MaskEditedunumreg->Text = row[35];
                MaskEditedudatediplom->Text = ReturnParsedDateInMask(AnsiString(row[36]));
                MaskEditedudatequalif->Text = ReturnParsedDateInMask(AnsiString(row[37]));
                CheckBoxedudiplomotl->Checked = StrToInt(AnsiString(row[38]));
                MaskEditedunumprotgak->Text = row[39];

                ComboBoxgrpChange(ComboBoxgrp);
           }
         }
         mysql_free_result(result);
         Memo1->Lines->Add(mysql_error(mysql));
       }

       LoadDataInAllPages(idselected.ToInt());
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::BtnCancelClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::SaveUpdateLoadComboBox(TComboBox* cmb,TTypeKeyVoc tkey)
{
  AnsiString Strtmp=cmb->Text;
  UpdateComboBox(cmb,tkey);
  cmb->ItemIndex=cmb->Items->IndexOf(Strtmp);
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::BtnWorkVocClick(TObject *Sender)
{
  FormInsertVoc->ShowModal();

  SaveUpdateLoadComboBox(ComboBoxeducation,EDUCATIONS);
  SaveUpdateLoadComboBox(ComboBoxedudoc,EDUDOCS);
  SaveUpdateLoadComboBox(ComboBoxeduform,EDUFORMS);
  SaveUpdateLoadComboBox(ComboBoxlanguage,LANGUAGES);
  SaveUpdateLoadComboBox(ComboBoxgrp,GROUPS);
  SaveUpdateLoadComboBox(ComboBoxspec,SPECS);
  SaveUpdateLoadComboBox(ComboBoxcity,CITIES);
  SaveUpdateLoadComboBox(ComboBoxdogshifr,DOGSHIFRS);
  SaveUpdateLoadComboBox(ComboBoxdogfast,DOGFASTS);
  SaveUpdateLoadComboBox(ComboBoxdogyear,DOGYEARS);

  ComboBoxgrpChange(ComboBoxgrp);

  AnsiString msg="После изменения данных в словаре рекомендуется проверить поля редактируемой записи.";
  MessageBox(Handle,msg.c_str(),"Сообщение:",MB_OK|MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::MaskEditbdateExit(TObject *Sender)
{
  TMaskEdit* mskEdit=(TMaskEdit*)Sender;
  if (!CompareMaskDate(mskEdit->Text))
  {
    AnsiString msg="Вводите дату в полном формате: dd.mm.yyyy .";
    MessageBox(Handle,msg.c_str(),"Сообщение:",MB_OK|MB_ICONERROR);
    mskEdit->SetFocus();
  }
}
//---------------------------------------------------------------------------
bool __fastcall TFormSelected::CompareMaskDate(AnsiString DateStr)
{
  int y,m,d;
  try
  {
    d=DateStr.SubString(1,2).ToInt();
    m=DateStr.SubString(4,2).ToInt();
    y=DateStr.SubString(7,4).ToInt();
  }
  catch (...)
  {
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::ComboBoxeduformChange(TObject *Sender)
{
  TComboBox* cmb=(TComboBox*)Sender;
  cmb->Hint=cmb->Hint+": "+cmb->Text;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::EditemailExit(TObject *Sender)
{
  PageControlSelected->ActivePageIndex++;
  if (PageControlSelected->ActivePageIndex==-1)
    PageControlSelected->ActivePageIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::UpDownStudentClick(TObject *Sender,
      TUDBtnType Button)
{
  int selrow=UpDownStudent->Position;
  Caption="Редактирование записи: "+MainForm->StrGridDB->Cells[2][selrow]+" "+MainForm->StrGridDB->Cells[3][selrow]+" "+MainForm->StrGridDB->Cells[4][selrow];

  FormShow(UpDownStudent);
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::BtnOkCancelClick(TObject *Sender)
{
  BtnOkClick(BtnOk);
  BtnCancelClick(BtnCancel);
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::FormDestroy(TObject *Sender)
{
  DestroyPages();
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::FormCreate(TObject *Sender)
{
   //------------Выгрузка DLL----------
  DestroyPages();
   //------------Загрузка DLL----------
  EDRegOpts myopts;
  int CountMyDLLs=myopts.CountDLLs;
  for (int i=1;i<=CountMyDLLs;i++) SetNewPageFromDLL(myopts.GetLibraryName(i));
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormSelected::GetCityName(AnsiString GroupStr)
{
  AnsiString Digits="0123456789";
  int MinPosDig=100; //Я думаю длиннее группы не будет
  int CurPos;
  for (int i=1;i<=Digits.Length();i++)
  {
    CurPos=GroupStr.Pos(AnsiString(Digits[i]));
    if (CurPos>0 && CurPos<MinPosDig) MinPosDig=CurPos;
  }
  int PosSlash=GroupStr.Pos("/");
  if (PosSlash==0 || MinPosDig==100 || PosSlash<=MinPosDig)
  {
    //MessageBox(Handle,"Неправильное задание группы:нет символа \'/\' или ещё что-либо (смотрите таблицу словаря voc или обратитесь за помощью к разработчику).","Ошибка:",MB_OK|MB_ICONERROR);
    return "???";
  }
  AnsiString NumCityStr=GroupStr.SubString(MinPosDig+1,PosSlash-2-MinPosDig);
  int NumCity;
  try
  {
    NumCity=NumCityStr.ToInt();
  }
  catch(...)
  {
    //MessageBox(Handle,"Неправильное задание группы:нет символа \'/\' или ещё что-либо (смотрите таблицу словаря voc или обратитесь за помощью к разработчику).","Ошибка:",MB_OK|MB_ICONERROR);
    return "???";
  }

  AnsiString CityStr=GetTitleForKeyNum(CITIES,NumCity);
  return CityStr;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::ComboBoxgrpChange(TObject *Sender)
{
  ComboBoxcity->ItemIndex=ComboBoxcity->Items->IndexOf(GetCityName(ComboBoxgrp->Text));
}
//---------------------------------------------------------------------------
int __fastcall TFormSelected::GetIDSpecForStudent(AnsiString(IDStudStr))
{
  EDRegOpts opts;
  int SpecID=-1;

  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString query="SELECT specid FROM "+opts.DBStudTable+" WHERE deleted=0 AND id="+ToStr(IDStudStr);
  mysql_query(mysql,query.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      row = mysql_fetch_row(result);
      SpecID=AnsiString(row[0]).ToInt();
    }
    mysql_free_result(result);
  }
  return SpecID;
}
//---------------------------------------------------------------------------
int __fastcall TFormSelected::GetIDGroupForStudent(AnsiString IDStudStr)
{
  EDRegOpts opts;
  int GroupID=-1;

  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString query="SELECT grpid FROM "+opts.DBStudTable+" WHERE deleted=0 AND id="+ToStr(IDStudStr);
  mysql_query(mysql,query.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      row = mysql_fetch_row(result);
      GroupID=AnsiString(row[0]).ToInt();
    }
    mysql_free_result(result);
  }
  return GroupID;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::UpdateAllPayFactsForStudents(int IDStud,int IDGroupOld,int IDGroupNew)
{
  EDRegOpts opts;

  MYSQL_RES *result;
  MYSQL_ROW row;

  TStringList* listIDFacts=new TStringList;
  TStringList* listOldIDOpts=new TStringList;
  TStringList* listDatesStart=new TStringList;
  TStringList* listDatesEnd=new TStringList;
  TStringList* listCommonMoney=new TStringList;
  AnsiString query;
  query="select pf.id,pf.idopts,po.datestart,po.dateend,po.commoncountmoney from "+opts.DBPayFacts+" as pf,"+opts.DBPayOpts+" as po where pf.deleted=0 and po.deleted=0 and pf.idstud="+ToStr(AnsiString(IDStud))+" and pf.idopts=po.id and po.idgroup="+ToStr(AnsiString(IDGroupOld));
  mysql_query(mysql,query.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      while (row = mysql_fetch_row(result))
      {
        listIDFacts->Add(AnsiString(row[0]));
        listOldIDOpts->Add(AnsiString(row[1]));
        listDatesStart->Add(AnsiString(row[2]));
        listDatesEnd->Add(AnsiString(row[3]));
        listCommonMoney->Add(AnsiString(row[4]));
      }
    }
    mysql_free_result(result);
  }

  AnsiString IDFactStr;
  AnsiString OldIDOptsStr;
  AnsiString NewIDOptsStr;

  AnsiString DateStartStr;
  AnsiString DateEndStr;
  AnsiString CommonMoneyStr;
  for (int i=0;i<listIDFacts->Count;i++)
  {
    IDFactStr=listIDFacts->Strings[i];
    OldIDOptsStr=listOldIDOpts->Strings[i];
    NewIDOptsStr="-1";
    DateStartStr=listDatesStart->Strings[i];
    DateEndStr=listDatesEnd->Strings[i];
    CommonMoneyStr=listCommonMoney->Strings[i];
    query="select pon.id from "+opts.DBPayOpts+" as pon, "+opts.DBPayOpts+" as po where pon.deleted=0 and po.deleted=0 and po.id="+ToStr(OldIDOptsStr)+" and po.id<>pon.id and pon.idgroup="+ToStr(AnsiString(IDGroupNew))+" and pon.datestart=po.datestart and pon.dateend=po.dateend and pon.commoncountmoney=po.commoncountmoney";
    mysql_query(mysql,query.c_str());
    if (mysql_field_count(mysql))
    {
      result=mysql_store_result(mysql);
      if (result && mysql_num_rows(result))
      {
        row = mysql_fetch_row(result);
        NewIDOptsStr=AnsiString(row[0]);
      }
      mysql_free_result(result);
    }
    if (NewIDOptsStr=="-1")// нет подходящей категории в другой группе
    {
      query="insert into "+opts.DBPayOpts+" (idgroup,datestart,dateend,commoncountmoney,deleted) values("+ToStr(AnsiString(IDGroupNew))+","+ToStr(DateStartStr)+","+ToStr(DateEndStr)+","+ToStr(CommonMoneyStr)+",0)";
      mysql_query(mysql,query.c_str());

      query="select id from "+opts.DBPayOpts+" where deleted=0 and idgroup="+ToStr(AnsiString(IDGroupNew))+" and datestart="+ToStr(DateStartStr)+" and dateend="+ToStr(DateEndStr)+" and commoncountmoney="+ToStr(CommonMoneyStr);
      mysql_query(mysql,query.c_str());
      if (mysql_field_count(mysql))
      {
        result=mysql_store_result(mysql);
        if (result && mysql_num_rows(result))
        {
          row = mysql_fetch_row(result);
          NewIDOptsStr=AnsiString(row[0]);
        }
        mysql_free_result(result);
      }
      if (NewIDOptsStr=="-1")
      {
        MessageBox(Handle,"Не удается обновить данные об оплате студента.","Предупреждение",MB_OK|MB_ICONINFORMATION);
        return;
      }
      else
      {
        query="update "+opts.DBPayFacts+" set idopts="+ToStr(NewIDOptsStr)+" where deleted=0 and id="+ToStr(IDFactStr);
        mysql_query(mysql,query.c_str());
      }
    }
    else // надо просто изменить поле idopts в факте оплаты
    {
      query="update "+opts.DBPayFacts+" set idopts="+ToStr(NewIDOptsStr)+" where deleted=0 and id="+ToStr(IDFactStr);
      mysql_query(mysql,query.c_str());
    }
  }

  delete listIDFacts;
  delete listOldIDOpts;
  delete listDatesStart;
  delete listDatesEnd;
  delete listCommonMoney;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelected::UpdateAllProgressForStudents(int IDStud,int IDSpecOld,int IDSpecNew)
{
  EDRegOpts opts;

  MYSQL_RES *result;
  MYSQL_ROW row;

  TStringList* listIDProgress=new TStringList;
  TStringList* listOldIDDiscip=new TStringList;
  TStringList* listFullTitles=new TStringList;
  TStringList* listShortTitles=new TStringList;
  AnsiString query;
  query="select p.id,p.iddiscip,d.fulltitle,d.shorttitle from "+opts.DBProgress+" as p,"+opts.DBDisciplines+" as d where p.deleted=0 and d.deleted=0 and p.idstud="+ToStr(AnsiString(IDStud))+" and p.iddiscip=d.id and d.idspec="+ToStr(AnsiString(IDSpecOld));
  mysql_query(mysql,query.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      while (row = mysql_fetch_row(result))
      {
        listIDProgress->Add(AnsiString(row[0]));
        listOldIDDiscip->Add(AnsiString(row[1]));
        listFullTitles->Add(AnsiString(row[2]));
        listShortTitles->Add(AnsiString(row[3]));
      }
    }
    mysql_free_result(result);
  }

  AnsiString IDProgressStr;
  AnsiString OldIDDiscipStr;
  AnsiString NewIDDiscipStr;

  AnsiString FullTitleStr;
  AnsiString ShortTitleStr;
  for (int i=0;i<listIDProgress->Count;i++)
  {
    IDProgressStr=listIDProgress->Strings[i];
    OldIDDiscipStr=listOldIDDiscip->Strings[i];
    NewIDDiscipStr="-1";
    FullTitleStr=listFullTitles->Strings[i];
    ShortTitleStr=listShortTitles->Strings[i];
    query="select dn.id from "+opts.DBDisciplines+" as dn, "+opts.DBDisciplines+" as d where dn.deleted=0 and d.deleted=0 and d.id="+ToStr(OldIDDiscipStr)+" and d.id<>dn.id and dn.idspec="+ToStr(AnsiString(IDSpecNew))+" and (dn.fulltitle=d.fulltitle or dn.shorttitle=d.shorttitle)";
    mysql_query(mysql,query.c_str());
    if (mysql_field_count(mysql))
    {
      result=mysql_store_result(mysql);
      if (result && mysql_num_rows(result))
      {
        row = mysql_fetch_row(result);
        NewIDDiscipStr=AnsiString(row[0]);
      }
      mysql_free_result(result);
    }
    if (NewIDDiscipStr=="-1")// нет подходящей дисциплины в другой специальности
    {
      query="insert into "+opts.DBDisciplines+" (idspec,fulltitle,shorttitle,deleted) values("+ToStr(AnsiString(IDSpecNew))+","+ToStr(FullTitleStr)+","+ToStr(ShortTitleStr)+",0)";
      mysql_query(mysql,query.c_str());

      query="select id from "+opts.DBDisciplines+" where deleted=0 and idspec="+ToStr(AnsiString(IDSpecNew))+" and fulltitle="+ToStr(FullTitleStr)+" and shorttitle="+ToStr(ShortTitleStr);
      mysql_query(mysql,query.c_str());
      if (mysql_field_count(mysql))
      {
        result=mysql_store_result(mysql);
        if (result && mysql_num_rows(result))
        {
          row = mysql_fetch_row(result);
          NewIDDiscipStr=AnsiString(row[0]);
        }
        mysql_free_result(result);
      }
      if (NewIDDiscipStr=="-1")
      {
        MessageBox(Handle,"Не удается обновить данные успеваемости студента.","Предупреждение",MB_OK|MB_ICONINFORMATION);
        return;
      }
      else
      {
        query="update "+opts.DBProgress+" set iddiscip="+ToStr(NewIDDiscipStr)+" where deleted=0 and id="+ToStr(IDProgressStr);
        mysql_query(mysql,query.c_str());
      }
    }
    else // надо просто изменить поле iddiscip в успеваемости
    {
      query="update "+opts.DBProgress+" set iddiscip="+ToStr(NewIDDiscipStr)+" where deleted=0 and id="+ToStr(IDProgressStr);
      mysql_query(mysql,query.c_str());
    }
  }

  delete listIDProgress;
  delete listOldIDDiscip;
  delete listFullTitles;
  delete listShortTitles;
}
//---------------------------------------------------------------------------

void __fastcall TFormSelected::ButtonAutoEnteringClick(TObject *Sender)
{
  EDRegOpts opts;

  MaskEditedudatediplom->Text = opts.DateDiplom;
  MaskEditedudatequalif->Text = opts.DateQualif;
  MaskEditexitdate->Text =  opts.DateOtch;
}
//---------------------------------------------------------------------------

void __fastcall TFormSelected::ButtonSaveDafaultDateClick(TObject *Sender)
{
  EDRegOpts opts;

  opts.DateDiplom = MaskEditedudatediplom->Text;
  opts.DateQualif = MaskEditedudatequalif->Text;
  opts.DateOtch = MaskEditexitdate->Text;
}
//---------------------------------------------------------------------------
bool __fastcall TFormSelected::IsExistsZnum(AnsiString& _FIOStr,int& _groupid)
{
  MYSQL_RES *result;
  MYSQL_ROW row;

  bool IsExists=false;
  _FIOStr="??? ??? ???";
  _groupid=-1;
  AnsiString myquery;
  myquery="SELECT id,CONCAT(secondname,\' \',firstname,\' \',thirdname),grpid FROM "+TabStudName+" WHERE znum ="+ToStr(Editznum->Text)+" AND id<>"+ToStr(AnsiString(idselected))+" AND deleted=0";

  mysql_query(mysql,myquery.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      row = mysql_fetch_row(result);
      IsExists=true;
      _FIOStr=AnsiString(row[1]);
      _groupid=AnsiString(row[2]).ToInt();
    }
    mysql_free_result(result);
  }

  return IsExists;
}

//---------------------------------------------------------------------------

