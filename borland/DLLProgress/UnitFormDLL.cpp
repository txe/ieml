//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitFormDLL.h"
#include "UnitEditDisciplines.h"
#include "UnitProgressDefSemestr.h"
#include "UnitInfoOcenk.h"
#include "mysql.h"
#include "UnitFast.h"

#define min(a,b) (a<b)?a:b

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDLLProgr *FormDLLProgr;

extern EDRegOpts opts;
extern MYSQL *mysql;

extern AnsiString arrOcenk[];

TStringList* listIDsOfDiscip;
TStringList* listClassOfDiscip;
TStringList* listIDsOfOcenk;

int indexDiscipForEditOneGroup=0;

const int index_class_VKR = 6;
const int index_class_Pract = 4;
#define IS_PERIOD(class) \
    ((class == index_class_VKR) || (class == index_class_Pract)) ? true : false


//---------------------------------------------------------------------------
AnsiString GetPointStr()
{
  AnsiString PointStr;
  AnsiString DblStr="3,7";
  double val;
  try
  {
    val=DblStr.ToDouble();
    PointStr=",";
  }
  catch(...)
  {
    PointStr=".";
  }
  return PointStr;
}
//---------------------------------------------------------------------------
AnsiString GetStrWithReplacedPoints(AnsiString Str, AnsiString PointStr=GetPointStr())
{
  AnsiString RetStr;
  int posPoint=Str.Pos(",");
  if (posPoint>0) RetStr=Str.SubString(1,posPoint-1)+PointStr+Str.SubString(posPoint+1,Str.Length()-posPoint);
  posPoint=Str.Pos(".");
  if (posPoint>0) RetStr=Str.SubString(1,posPoint-1)+PointStr+Str.SubString(posPoint+1,Str.Length()-posPoint);
  return RetStr;
}
//---------------------------------------------------------------------------
__fastcall TFormDLLProgr::TFormDLLProgr(TComponent* Owner)
        : TForm(Owner)
{
  listIDsOfDiscip=new TStringList;
  listClassOfDiscip=new TStringList;
  listIDsOfOcenk=new TStringList;

  OCUpdateOcenk(ComboBoxOcenka);
  OCUpdateBalls(ComboBoxBall);
}
//---------------------------------------------------------------------------
void __fastcall TFormDLLProgr::LoadData(char* IniFileName,int _idstud)
{
  opts.SetFileName(AnsiString(IniFileName));
  BtnEditDiscip->Enabled=opts.ACCESSEditDisciplines;
  idstud=_idstud;

  AnsiString Msg="Неудачное подключение к "+opts.DBHost+" пользователя "+opts.DBLogin+"!";
  if (!WCConnect())
  {
    //MessageBox(Handle,Msg.c_str(),"Error",MB_OK);
    WCDisconnect();
  }
  else
  {
    AnsiString query="CREATE TABLE IF NOT EXISTS "+opts.DBDisciplines+" (id INT PRIMARY KEY AUTO_INCREMENT NOT NULL, idspec INT NOT NULL, fulltitle TEXT NULL, shorttitle TEXT NULL, deleted BIT NOT NULL DEFAULT 0, num_hours INT NULL, idclass INT NOT NULL)";
    mysql_query(mysql,query.c_str());
    query="CREATE TABLE IF NOT EXISTS "+opts.DBProgress+" (id INT PRIMARY KEY AUTO_INCREMENT NOT NULL, idstud INT NOT NULL, iddiscip INT NOT NULL, numplansemestr INT NOT NULL, numgraphsemestr INT NOT NULL, estimation INT NULL, ball FLOAT(4,2) NULL, deleted BIT NOT NULL DEFAULT 0)";
    mysql_query(mysql,query.c_str());
  }

  UpdateDisciplinesInfo();
  ComboBoxDiscip->ItemIndex=min(indexDiscipForEditOneGroup,ComboBoxDiscip->Items->Count-1);
  ComboBoxDiscipChange(ComboBoxDiscip);
  UpdateVKR();
  
/*  ComboBoxOcenka->ItemIndex=-1;
  ComboBoxBall->ItemIndex=-1;*/
}
//---------------------------------------------------------------------------
void __fastcall TFormDLLProgr::BtnEditDiscipClick(TObject *Sender)
{
  TFormDisciplines* FormDisciplines=new TFormDisciplines(this);
  FormDisciplines->SetMyFormDLL(this);
  FormDisciplines->ShowModal();
  delete FormDisciplines;

  UpdateDisciplinesInfo();
  ComboBoxDiscip->ItemIndex=0;
  ComboBoxDiscipChange(ComboBoxDiscip);
}
//---------------------------------------------------------------------------
int __fastcall TFormDLLProgr::GetIDCurStud(void)
{
  return idstud;
}
//---------------------------------------------------------------------------
void __fastcall TFormDLLProgr::BtnCheckOcenkClick(TObject *Sender)
{
  if (ComboBoxOcenka->ItemIndex==-1)
  {
    MessageBox(Handle,"Выберите оценку пожалуйста.","Ошибка",MB_OK|MB_ICONERROR);
    //ComboBoxOcenka->SetFocus();
    return;
  }
  AnsiString EstimationStr=ComboBoxOcenka->ItemIndex;
  AnsiString BallStr=GetStrWithReplacedPoints(ComboBoxBall->Text);
  double valBall;

  try
  {
    valBall=BallStr.ToDouble();
    if (valBall<0 || valBall>5.0) throw 1;
  }
  catch(...)
  {
     MessageBox(Handle,"Балл это вещественное неотрицательное число из диапазона [0..5].\nИсправьте пожалуйста.","Ошибка",MB_OK|MB_ICONERROR);
     //ComboBoxBall->SetFocus();
     ComboBoxBall->SelectAll();
     return;
  }
  BallStr=GetStrWithReplacedPoints(BallStr,"."); //для MySql нужна точка

  AnsiString Msg="";
  switch (EstimationStr.ToInt())
  {
    case OTL:
      if (valBall<4.50 || valBall>5.0) Msg="Ошибка! При оценке \'отлично\' балл выбирается из дипазаона [4.50 .. 5.00].\nИсправьте пожалуйста.";
      break;
    case HOR:
      if (valBall<3.50 || valBall>4.49) Msg="Ошибка! При оценке \'хорошо\' балл выбирается из дипазаона [3.50 .. 4.49].\nИсправьте пожалуйста.";
      break;
    case UDOVL:
      if (valBall<2.50 || valBall>3.49) Msg="Ошибка! При оценке \'удовлетворительно\' балл выбирается из дипазаона [2.50 .. 3.49].\nИсправьте пожалуйста.";
      break;
    case NEUD:
      Msg="Ошибка! Оценка \'неудовлетворительно\' не может быть проставлена, поскольку о наличии такой оценки часто забывают, что может отразиться в отчетах.\nПроставьте оценку после пересдачи студентом экзамена.";
      break;
    case ZACH:
      if (valBall<2.50 || valBall>5.0) Msg="Ошибка! При оценке \'зачтено\' балл выбирается из дипазаона [2.50 .. 5.00].\nИсправьте пожалуйста.";
      break;
    case NEZACH:
      Msg="Ошибка! Оценка \'незачтено\' не может быть проставлена, поскольку о наличии такой оценки часто забывают, что может отразиться в отчетах.\nПроставьте оценку после пересдачи студентом зачета.";
      break;
  }
  if (!Msg.IsEmpty())
  {
    MessageBox(Handle,Msg.c_str(),"предупреждение",MB_OK|MB_ICONINFORMATION);
    ComboBoxBall->SelectAll();
    return;
  }

  int indexsel=ComboBoxDiscip->ItemIndex;
  if (indexsel < 0 || indexsel >= listIDsOfDiscip->Count) return;
  AnsiString IDDiscipStr=listIDsOfDiscip->Strings[indexsel];
  int classDiscip=listClassOfDiscip->Strings[indexsel].ToInt();
  bool isPeriod = IS_PERIOD(classDiscip);
  AnsiString IDStudStr=AnsiString(idstud);
  AnsiString PlanSemestrStr=UpDownPlan->Position;
  AnsiString GraphSemestrStr=UpDownGraphic->Position;

  AnsiString myquery="INSERT INTO "+opts.DBProgress+" (idstud,iddiscip,numplansemestr,numgraphsemestr,estimation,ball) VALUES("+ToStr(IDStudStr)+","+ToStr(IDDiscipStr)+","+ToStr(PlanSemestrStr)+","+ToStr(GraphSemestrStr)+","+ToStr(EstimationStr)+","+ToStr(BallStr)+")";
  mysql_query(mysql,myquery.c_str());
  //MessageBox(Handle,"Оценка зафиксирована!","Сообщение:",MB_OK|MB_ICONINFORMATION);

  ComboBoxDiscipChange(ComboBoxDiscip);

  // устанавливаем экзамен или зачет

  if ( ComboBoxOcenka->ItemIndex < 4 )
  {
    estim = 1;
    EstimLabel->Caption = "экзамен";
  }
  else
  {
    estim = 2;
    EstimLabel->Caption = "зачет";
  }

}
//---------------------------------------------------------------------------
void __fastcall TFormDLLProgr::UpdateDisciplinesInfo(void)
{
  AnsiString Str;
  ComboBoxDiscip->Clear();
  listIDsOfDiscip->Clear();
  listClassOfDiscip->Clear();

  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString myquery;
  myquery="SELECT id,fulltitle,shorttitle,idclass FROM "+opts.DBDisciplines+" WHERE idspec="+ToStr(GetSpecIDForStudID(idstud))+" AND deleted=0 ORDER BY fulltitle";
  mysql_query(mysql,myquery.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {

      while (row = mysql_fetch_row(result))
      {
        listIDsOfDiscip->Add(AnsiString(row[0]));
        listClassOfDiscip->Add(AnsiString(row[3]));
        Str=AnsiString(row[1]);
        if (!AnsiString(row[2]).IsEmpty()) Str+=" ( "+AnsiString(row[2])+" )";
        ComboBoxDiscip->Items->Add(Str);
      }
    }
    mysql_free_result(result);
  }
}
//---------------------------------------------------------------------------
int __fastcall TFormDLLProgr::GetSpecIDForStudID(int _idstud)
{
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
  return myidspec;
}
//---------------------------------------------------------------------------
void __fastcall TFormDLLProgr::FormDestroy(TObject *Sender)
{
  delete listIDsOfDiscip;
  delete listClassOfDiscip;
  delete listIDsOfOcenk;
}
//---------------------------------------------------------------------------
void __fastcall TFormDLLProgr::ComboBoxDiscipChange(TObject *Sender)
{
  indexDiscipForEditOneGroup=ComboBoxDiscip->ItemIndex;

  ListBoxOcenk->Clear();
  listIDsOfOcenk->Clear();

  ListBoxOcenk->Items->Add("Сем. по плану          Сем. по графику                 Оценка                      Балл");
  listIDsOfOcenk->Add("");
  ListBoxOcenk->Items->Add("");
  listIDsOfOcenk->Add("");

  if (ComboBoxDiscip->Items->Count==0)
  {
      BtnCheckOcenk->Enabled=false;
      return;
  }
  else BtnCheckOcenk->Enabled=true;
  int indexsel=ComboBoxDiscip->ItemIndex;
  if (indexsel < 0 || indexsel >= listIDsOfDiscip->Count) return;

  AnsiString idselDiscip=listIDsOfDiscip->Strings[indexsel];
  int classDiscip=listClassOfDiscip->Strings[indexsel].ToInt();

  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString numplansem,numgraphsem,est,ball;
  int estindex;

  AnsiString SpaceStr;
  AnsiString Str;
  AnsiString myquery;
  myquery="SELECT id,numplansemestr,numgraphsemestr,estimation,ball FROM "+opts.DBProgress+" WHERE idstud="+ToStr(AnsiString(idstud))+" AND iddiscip="+ToStr(idselDiscip)+" AND deleted=0 ORDER BY numplansemestr";
  mysql_query(mysql,myquery.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      while (row = mysql_fetch_row(result))
      {
        listIDsOfOcenk->Add(AnsiString(row[0]));
        numplansem=AnsiString(row[1]);
        numgraphsem=AnsiString(row[2]);
        estindex=AnsiString(row[3]).ToInt();
        est=arrOcenk[estindex];
        ball=GetStrWithReplacedPoints(AnsiString(row[4]),",");

        SpaceStr="      ";
        if (numplansem.Length()==1) SpaceStr+=" ";
        Str=SpaceStr+numplansem;
        SpaceStr="                                 ";
        if (numgraphsem.Length()==1) SpaceStr+=" ";
        Str+=SpaceStr+numgraphsem;
        SpaceStr="                       ";
        switch(estindex)
        {
         case 0: SpaceStr+="           ";break;
         case 1: SpaceStr+="           ";break;
         case 2: SpaceStr+="  ";break;
         case 4: SpaceStr+="           ";break;
         case 5: SpaceStr+="          ";break;
        }
        Str+=SpaceStr+est;
        switch(estindex)
        {
         case 0: SpaceStr="              ";break;
         case 1: SpaceStr="               ";break;
         case 2: SpaceStr="   ";break;
         case 3: SpaceStr=" ";break;
         case 4: SpaceStr="              ";break;
         case 5: SpaceStr="           ";break;
        }
        SpaceStr+="        ";
        Str+=SpaceStr+ball;
        SpaceStr="                 ";
        if (IS_PERIOD(classDiscip))
          Str+=SpaceStr;
        ListBoxOcenk->Items->Add(Str);
      }
    }
    mysql_free_result(result);
  }

  ListBoxOcenkClick(ListBoxOcenk);
}
//---------------------------------------------------------------------------
void __fastcall TFormDLLProgr::BtnDelClick(TObject *Sender)
{
  int rowsel=ListBoxOcenk->ItemIndex;
  if (rowsel==-1) return; // Запись не выбрана
  if (listIDsOfOcenk->Strings[rowsel].IsEmpty()) return; // это не запись оценки

  AnsiString IDOcenkStr=listIDsOfOcenk->Strings[rowsel];

  AnsiString myquery;
/*  AnsiString msg="Вы действительно хотите удалить эту запись?\nБудьте аккуратны!";
  if (MessageBox(Handle,msg.c_str(),"Сообщение:",MB_YESNO|MB_ICONQUESTION)==ID_YES)
  {*/
    myquery="UPDATE "+opts.DBProgress+" SET deleted=1 WHERE id="+IDOcenkStr;
    mysql_query(mysql,myquery.c_str());

    ComboBoxDiscipChange(ComboBoxDiscip);
//  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDLLProgr::ListBoxOcenkClick(TObject *Sender)
{
  int rowsel=ListBoxOcenk->ItemIndex;
  if (rowsel==-1) BtnDel->Enabled=false;
  else
    if (listIDsOfOcenk->Strings[rowsel].IsEmpty()) BtnDel->Enabled=false;
    else BtnDel->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormDLLProgr::BtnProgressDefSemestrClick(TObject *Sender)
{
  TFormProgressDefSemestr* FormProgressDefSemestr=new TFormProgressDefSemestr(this);
  FormProgressDefSemestr->SetMyFormDLL(this);
  FormProgressDefSemestr->SetIDStud(idstud);
  FormProgressDefSemestr->ShowModal();
  delete FormProgressDefSemestr;

  UpdateDisciplinesInfo();
  ComboBoxDiscip->ItemIndex=0;
  ComboBoxDiscipChange(ComboBoxDiscip);
}
//---------------------------------------------------------------------------
void __fastcall TFormDLLProgr::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------
void __fastcall TFormDLLProgr::BtnVKRClick(TObject *Sender)
{
// назначение/удаление названия ВКР
  AnsiString myquery;
  myquery="UPDATE "+opts.DBStudTable+" SET vkr_title="+ToStr(EditVKRTitle->Text)+" WHERE id="+ToStr(AnsiString(GetIDCurStud()));
  mysql_query(mysql,myquery.c_str());
  UpdateVKR();
}
//---------------------------------------------------------------------------
void __fastcall TFormDLLProgr::UpdateVKR(void)
{
  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString VKRTitle="";
  AnsiString myquery;
  myquery="SELECT vkr_title FROM "+opts.DBStudTable+" WHERE id="+ToStr(AnsiString(GetIDCurStud()));
  mysql_query(mysql,myquery.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      row = mysql_fetch_row(result);
      VKRTitle = AnsiString(row[0]);
    }
    mysql_free_result(result);
  }

  EditVKRTitle->Text = VKRTitle;
}
//---------------------------------------------------------------------------

void __fastcall TFormDLLProgr::ComboBoxBallChange(TObject *Sender)
{
    AnsiString ball = ComboBoxBall->Text;

    if (estim == 0 || ball.Length() == 0)
        return;

    if ( ball.Length() == 3 && ball[2] == ',')
        ball[2] = '.';

    char*  end_ptr;
    double val = strtod(ball.c_str(), &end_ptr);
    if (*end_ptr)
    {
        // Сигнализируем об ошибке в строке
            return;
    }
    if ((val == LONG_MAX || val == LONG_MIN) && errno == ERANGE)
    {
        // Сигнализируем о переполнении
        return;
    }
    // Продолжаем штатную работу.
    if (estim == 1) // экзамен
    {
        // отлично
        if ( val >= 4.5)
        {
            ComboBoxOcenka->ItemIndex = 0;
            return;
        }
        // хорошо
        if ( val >= 3.5 && val < 4.5 )
        {
            ComboBoxOcenka->ItemIndex = 1;
            return;
        }
        // уд
        if ( val >= 2.5 && val < 3.5 )
        {
            ComboBoxOcenka->ItemIndex = 2;
            return;
        }
        // неуд
        if ( val <= 2.5)
        {
            ComboBoxOcenka->ItemIndex = 3;
            return;
        }
    }
    if (estim == 2) // зачет
    {
        // зачтено
        if ( val >= 2.5)
        {
            ComboBoxOcenka->ItemIndex = 4;
        }
        // незачтено
        else
        {
            ComboBoxOcenka->ItemIndex = 5;
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormDLLProgr::FormShow(TObject *Sender)
{
    estim = 0;
    EstimLabel->Caption = "неизвестно";
}
//---------------------------------------------------------------------------

void __fastcall TFormDLLProgr::ButtonFastClick(TObject *Sender)
{
    AnsiString query = "SELECT grpid FROM students \
        WHERE id = " + AnsiString(idstud);
    MYSQL_RES *result;
    MYSQL_ROW row;
    mysql_query(mysql,query.c_str());
    result = mysql_store_result(mysql);
    row = mysql_fetch_row(result);
    AnsiString grpid = row[0];
    mysql_free_result(result);

    int indexsel = ComboBoxDiscip->ItemIndex;
    if (indexsel < 0 || indexsel >= listIDsOfDiscip->Count)
         return;
    AnsiString iddiscip = listIDsOfDiscip->Strings[indexsel];

    if ( ComboBoxOcenka->ItemIndex < 4 )
    {
        estim = 1;
        EstimLabel->Caption = "экзамен";
    }
    else
    {
        estim = 2;
        EstimLabel->Caption = "зачет";
    }

    TFormFast* fastform = new TFormFast(this);

    fastform->name_pred = ComboBoxDiscip->Text;
    fastform->idgroup = grpid;
    fastform->iddiscip = iddiscip;
    fastform->plan_sem = EditSemestrPlan->Text;
    fastform->graf_sem = EditSemestrGraphic->Text;
    fastform->estim = estim;
    fastform->CreateTable();
    
    fastform->ShowModal();

    delete fastform;
    
    ComboBoxDiscipChange(ComboBoxDiscip);
}
//---------------------------------------------------------------------------

