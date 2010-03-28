//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportCharacteristic.h"
#include "UnitFuncs.h"
#include "UnitInfoOcenk.h"
#include <stdio.h>
#include "MacroWord.h"
#include <Math.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)

#include "UnitFuncs.h"
#include "mysql.h"

#pragma resource "*.dfm"
TFormReportCharacteristic *FormReportCharacteristic;

extern AnsiString arrOcenk[];

extern AnsiString keys[]; 

extern EDRegOpts opts;
extern MYSQL *mysql;
//---------------------------------------------------------------------------
AnsiString GetYear_cur(const AnsiString& str)
{
  AnsiString year = str.SubString(7,4);
  return year;
}
AnsiString GetEst_cur(float valBall)
{
  AnsiString res = "";
      int index;
      if (valBall>=4.50 && valBall<=5.0) index = 0;
      else
        if (valBall>=3.50 && valBall<=4.49) index = 1;
        else
          if (valBall>=2.50 && valBall<=3.49) index = 2;
          else
            if (valBall>=1.0) index = 3;
            else  res="???";
  return ((res.IsEmpty()) ? arrOcenk[index] : res);
}
//---------------------------------------------------------------------------
__fastcall TFormReportCharacteristic::TFormReportCharacteristic(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportCharacteristic::SetIDStudent(int _IDStudent)
{
  idstudent=_IDStudent;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportCharacteristic::GetIDStudent(void)
{
  return idstudent;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportCharacteristic::InitReportQuery(void)
{
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("select id,CONCAT(secondname,\' \',firstname,\' \',thirdname),specid,znum,enterdate,bdate,educationid,eduenddate,eduplace,enterdate,sex,grpid from "+opts.DBStudTable+" where deleted=0 and id="+ToStr(AnsiString(idstudent)));
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportCharacteristic::GetBallsCount(int& countUDOVL,double& percUDOVL,int& countHOR,double& percHOR,int& countOTL,double& percOTL,AnsiString& AvrgBallStr,AnsiString IDStudStr)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    AnsiString query = "set @id = " + IDStudStr;
    mysql_query(mysql, query.c_str());
    mysql_query(mysql,"CREATE TEMPORARY TABLE temp_stud TYPE = HEAP "
        "SELECT st.id, st.firstname, st.secondname, st.thirdname, st.znum "
        "FROM students as st "
        "WHERE  st.id = @id AND st.deleted = 0 ");

    mysql_query(mysql,"CREATE TEMPORARY TABLE temp_progress1 TYPE = HEAP "
        "SELECT pr.*, st.firstname, st.secondname, st.thirdname, st.znum "
        "FROM progress as pr, temp_stud  as st "
        "WHERE  pr.idstud = st.id AND pr.deleted = 0 AND pr.estimation < 3 ");

    mysql_query(mysql,"CREATE TEMPORARY TABLE temp_progress2 TYPE = HEAP "
        "SELECT idstud, iddiscip, MAX(numplansemestr) as numplansemestr "
        "FROM temp_progress1 "
        "GROUP BY idstud, iddiscip ");

    mysql_query(mysql,"CREATE TEMPORARY TABLE temp_progress3 TYPE = HEAP "
        "SELECT pr1.* "
        "FROM temp_progress1 as pr1, temp_progress2 as pr2 "
        "WHERE pr1.idstud = pr2.idstud AND pr1.iddiscip = pr2.iddiscip "
        "AND pr1.numplansemestr = pr2.numplansemestr ");

    mysql_query(mysql,"select idstud,secondname,firstname, thirdname, znum, "
        "sum(case  when estimation=0 then 1 else 0 end) as otl, "
        "sum(case  when estimation=1 then 1 else 0 end) as hor, "
        "sum(case  when estimation=2 then 1 else 0 end) as ud, "
        "AVG(ball) as sr "
        "from temp_progress3 "
        "group by idstud "
        "order by secondname ");

    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
        row = mysql_fetch_row(result);

        countOTL = AnsiString(row[5]).ToInt();
        countHOR = AnsiString(row[6]).ToInt();
        countUDOVL = AnsiString(row[7]).ToInt();

        TReplaceFlags flags;
        flags << rfReplaceAll << rfIgnoreCase;
        AvrgBallStr = StringReplace(row[8], ".", ",", flags);

    }
    mysql_free_result(result);
    int count = countOTL + countHOR + countUDOVL;
    // percUDOVL = SimpleRoundTo(100.0*(double)countUDOVL/(double)count, -2);
    percHOR = SimpleRoundTo(100.0*(double)countHOR/(double)count, 0);
    percOTL = SimpleRoundTo(100.0*(double)countOTL/(double)count, 0);
    percUDOVL = 100.0 - percHOR - percOTL;
/*
  ZMySqlQueryAvrgBall->SQL->Clear();
  ZMySqlQueryAvrgBall->SQL->Add("select ball,estimation from "+opts.DBProgress+" where deleted=0 and idstud="+ToStr(IDStudStr)+" and (estimation="+ToStr(AnsiString(OTL))+" or estimation="+ToStr(AnsiString(HOR))+" or estimation="+ToStr(AnsiString(UDOVL))+")");
  ZMySqlQueryAvrgBall->Active=true;
  double SumBall=0.0;
  countUDOVL=countHOR=countOTL=0;
  percUDOVL=percHOR=percOTL=0.0;
  for (int i=0;i<ZMySqlQueryAvrgBall->RecordCount;i++)
  {
    ZMySqlQueryAvrgBall->RecNo=i+1;
    SumBall += ZMySqlQueryAvrgBall->Fields->FieldByNumber(1)->AsString.ToDouble();
    if (ZMySqlQueryAvrgBall->Fields->FieldByNumber(2)->AsString.ToInt()==OTL) countOTL++;
    if (ZMySqlQueryAvrgBall->Fields->FieldByNumber(2)->AsString.ToInt()==HOR) countHOR++;
    if (ZMySqlQueryAvrgBall->Fields->FieldByNumber(2)->AsString.ToInt()==UDOVL) countUDOVL++;
  }
  int NumBalls=countOTL+countHOR+countUDOVL;
  double avrgball=0.0;
  AvrgBallStr="---";
  if (NumBalls>0)
  {
    avrgball = SumBall/double(NumBalls);
    AvrgBallStr = AnsiString(avrgball);
    char str[10];
    sprintf(str,"%4.2lf",AvrgBallStr.ToDouble());
    AvrgBallStr = AnsiString(str);
    percUDOVL=(double)(int)(100.0*double(countUDOVL)/double(NumBalls));
    percHOR=(double)(int)(100.0*double(countHOR)/double(NumBalls));
    percOTL=(double)(100 - (int)percUDOVL - (int)percHOR);//100.0*double(countOTL)/double(NumBalls);
  }
  */
}
//---------------------------------------------------------------------------
void __fastcall TFormReportCharacteristic::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportCharacteristic::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportCharacteristic::CreateWordDocument(void)
{
  int numspec;

  InitReportQuery();

    if (ZMySqlQuery->RecordCount==0)
        return;

    WordMacros macros;
    macros.BeginMacros();

    macros.InsertLine("ActiveDocument.PageSetup.TopMargin=70");
    macros.InsertLine("ActiveDocument.PageSetup.BottomMargin=55");
    macros.InsertLine("ActiveDocument.PageSetup.LeftMargin=55");
    macros.InsertLine("ActiveDocument.PageSetup.RightMargin=55");

    macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");

    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
    macros.SelectionFont("Bold=false");
    macros.SelectionFont("Size=12");
    macros.SelectionText("Федеральное агентство по образованию\n");
    macros.SelectionText("Государственное образовательное учреждение высшего профессионального образования\n");
    macros.SelectionText("\"\"Нижегородский государственный архитектурно-строительный университет\"\" (ННГАСУ)\n\n");
    macros.InsertLine("Selection.MoveUp Unit:=wdLine, Count:=2");
    macros.SelectionParagraphFormat("Borders.Item(wdBorderBottom).LineStyle = wdLineStyleSingle");
    macros.SelectionParagraphFormat("Borders.Item(wdBorderBottom).LineWidth = wdLineWidth050pt");
    macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=2");
    macros.SelectionText("ИНСТИТУТ ЭКОНОМИКИ, УПРАВЛЕНИЯ И ПРАВА\n\n");
    macros.SelectionFont("Bold=true");
    macros.SelectionText("ХАРАКТЕРИСТИКА\n\n");
    macros.SelectionFont("Bold=false");
    macros.SelectionFont("Size=12");
    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");

    ZMySqlQuery->RecNo=1;

    AnsiString Sex = ZMySqlQuery->Fields->FieldByNumber(11)->AsString;
    bool isMan = (Sex[1]=='М') ? true : false;
    AnsiString FIO = ZMySqlQuery->Fields->FieldByNumber(2)->AsString;
    AnsiString byear = GetYear_cur(ZMySqlQuery->Fields->FieldByNumber(6)->AsString);
    AnsiString educ = WCGetTitleForKeyNum(EDUCATIONS,ZMySqlQuery->Fields->FieldByNumber(7)->AsString.ToInt());
    AnsiString eduendyear = GetYear_cur(ZMySqlQuery->Fields->FieldByNumber(8)->AsString);
    AnsiString eduplace = ZMySqlQuery->Fields->FieldByNumber(9)->AsString;
    AnsiString enteryear = GetYear_cur(ZMySqlQuery->Fields->FieldByNumber(10)->AsString);
    AnsiString spec = WCGetTitleForKeyNum(SPECS, ZMySqlQuery->Fields->FieldByNumber(3)->AsString.ToInt());
    AnsiString grpName = WCGetTitleForKeyNum(GROUPS, ZMySqlQuery->Fields->FieldByNumber(12)->AsString.ToInt());

    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphJustify");
    macros.SelectionText("vbTab");
    macros.SelectionText(FIO+", "+byear+" года рождения, образование "+educ+".\n");
    macros.SelectionText("vbTab");
    macros.FilterText(eduplace);
    macros.SelectionText("Закончил"+AnsiString(isMan?"":"а")+" в "+eduendyear+" году "+ eduplace + ".\n");
    macros.SelectionText("vbTab");
    macros.SelectionText("В "+enteryear+" году поступил"+AnsiString(isMan?"":"а")+" в ННГАСУ на места, не финансируемые из федерального бюджета, для обучения по специальности \"\""+spec+"\"\" по заочной форме с применением дистанционных технологий.\n");
    macros.SelectionText("vbTab");
    macros.SelectionText("За время обучения в университете показал"+AnsiString(isMan?"":"а")+" следующую успеваемость:\n");
    int countUDOVL,countHOR,countOTL;
    double percUDOVL,percHOR,percOTL;
    AnsiString AvrgBallStr;
    GetBallsCount(countUDOVL,percUDOVL,countHOR,percHOR,countOTL,percOTL,AvrgBallStr,AnsiString(idstudent));

    AnsiString percUDOVLStr=AnsiString(percUDOVL),
               percHORStr=AnsiString(percHOR),
               percOTLStr=AnsiString(percOTL);
    AnsiString Str;
    char str[10];
    sprintf(str,"%4.0lf",percUDOVLStr.ToDouble());
    percUDOVLStr = AnsiString(str);
    sprintf(str,"%4.0lf",percHORStr.ToDouble());
    percHORStr = AnsiString(str);
    sprintf(str,"%4.0lf",percOTLStr.ToDouble());
    percOTLStr = AnsiString(str);

    Str="- \"\"отлично\"\" - "+percOTLStr+" %;\n";
    macros.SelectionText(Str);
    Str="- \"\"хорошо\"\" - "+percHORStr+" %;\n";
    macros.SelectionText(Str);
    Str="- \"\"удовлетворительно\"\" - "+percUDOVLStr+" %.\n";
    macros.SelectionText(Str);

    AnsiString caf = "???";
    if (grpName.SubString(1,2) == "БУ" || grpName.SubString(1,3) == "ЭУН") caf = "экономического анализа и управления недвижимостью";
    if (grpName.SubString(1,2) == "МО") caf = "инвестиционного менеджмента";
    if (grpName.SubString(1,2) == "МР") caf = "стратегического маркетинга";
    if (grpName.SubString(1,3) == "ПГС") caf = "архитектуры/технологии строительного производства";
    if (grpName.SubString(1,3) == "ТГВ") caf = "теплогазоснабжения";
    if (grpName.SubString(1,1) == "Ю") caf = "права";

    float itog_oc;
    InitItog(itog_oc);
//    MessageBox(0, AnsiString(itog_oc).c_str(),"",0);
    macros.SelectionText("vbTab");
    macros.SelectionText("Сдал"+AnsiString(isMan?"":"а")+" государственный экзамен по специальности с итоговой оценкой \"\""+GetEst_cur(itog_oc)+"\"\".\n");
    macros.SelectionText("vbTab");
    macros.SelectionText(AnsiString(isMan?"Прошел":"Прошла")+" специализацию и выполнил"+AnsiString(isMan?"":"а")+" дипломную работу по кафедре " + caf + ".\n");
    macros.SelectionText("vbTab");
    macros.SelectionText(FIO+" административных взысканий за время обучения не имеет, пользуется уважением студентов и преподавателей.\n");
    macros.SelectionText("vbTab");
    macros.SelectionText("Характеристика дана для предоставления в Государственную аттестационную комиссию по защите выпускной квалификационной работы по специальности \"\""+spec+"\"\".\n");
    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
    macros.SelectionText("\n");
    macros.SelectionText("vbTab");
    macros.SelectionText("Директор ИЭУП, профессор");
    macros.SelectionText("vbTab");
    macros.SelectionText("vbTab");
    macros.SelectionText("vbTab");
    macros.SelectionText("vbTab");
    macros.SelectionText("vbTab");
    macros.SelectionText("vbTab");
    macros.SelectionText("Д.В. Хавин\n");
    AnsiString curdate = TDateTime::CurrentDate().DateString();
    macros.SelectionText("\n");
    macros.SelectionText("vbTab");
    macros.SelectionText(curdate+"\n");

    macros.EndMacros();
    macros.RunMacros();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportCharacteristic::ToolBtnPreviewClick(
      TObject *Sender)
{
  try
  {
    CreateWordDocument();
  }
  catch(...)
  {
    Application->MessageBox("Работа с MS Word не возможна. Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportCharacteristic::FormCreate(TObject *Sender)
{
  bool NoError;
  try
  {
    ZMySqlDatabase->HostName=opts.DBHost;
    ZMySqlDatabase->User=opts.DBLogin;
    ZMySqlDatabase->Password=opts.DBPasswd;
    ZMySqlDatabase->Database=opts.DBName;
    ZMySqlDatabase->Connected=true;

    NoError=ZMySqlDatabase->Connected;
  }
  catch(...)
  {
    NoError=ZMySqlDatabase->Connected;
  }

  NoError=NoError && WCConnect();
  if ( !NoError)
  {
    AnsiString msg="Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportCharacteristic::FormDestroy(TObject *Sender)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportCharacteristic::InitItog(float& itog_oc)
{
  itog_oc = 0.0;

  ZMySqlQuery->SQL->Clear();
  AnsiString query = "select di.idclass,pr.ball from "+opts.DBDisciplines+" as di, "+opts.DBProgress+" as pr where di.deleted=0 and pr.deleted=0 and pr.idstud="+ToStr(AnsiString(idstudent))+" and pr.iddiscip=di.id";
  ZMySqlQuery->SQL->Add(query);
  ZMySqlQuery->Active=true;

  if (ZMySqlQuery->RecordCount==0) return;

  int i;
  int d_class;
  int num_itog=0;
//  AnsiString s;
  for (i = 0; i < ZMySqlQuery->RecordCount; ++i)
  {
    ZMySqlQuery->RecNo = i+1;
    d_class = ZMySqlQuery->Fields->FieldByNumber(1)->AsString.ToInt();
    if (d_class == 5)   // Итоговая аттестация
    {
//      s += "\n" + AnsiString(ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToDouble());

      num_itog++;
      itog_oc += ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToDouble();
    }
  }
//  MessageBox(0,s.c_str(),"",0);
  if (num_itog > 0)
  {
    itog_oc = itog_oc / (float)num_itog;
    itog_oc = (float(int(itog_oc*10)) + (((itog_oc*10 - float((int(itog_oc*10)))) >= 0.5f) ? 1.0 : 0.0)) / 10.0f;
    itog_oc = float(itog_oc);
  }

}
//---------------------------------------------------------------------------

