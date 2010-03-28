//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportPrilDiplomNepol.h"
#include "UnitFuncs.h"
//#include "UnitInfoOcenk.h"
#include "MacroWord.h"
#include "mysql.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma resource "*.dfm"
TFormReportPrilDiplomNepol *FormReportPrilDiplom;

extern AnsiString arrOcenk[];
extern const CountOcenk=6;

extern EDRegOpts opts;
extern MYSQL *mysql;
enum ETypeDiscip {
        tdCommon = 1,
        tdCourseWork = 2,
        tdCourseProject = 3,
        tdPraktica = 4,
        tdItogAttest = 5,
        tdVipQualifWork = 6,
        tdAdditional = 7
};

enum ETypeEstimation {
        te5 = 0,
        te4 = 1,
        te3 = 2,
        te2 = 3,
        teZach = 4,
        teNeZach = 5
};

AnsiString MounthNames[12]={ "января",
                             "февраля",
                             "марта",
                             "апреля",
                             "мая",
                             "июня",
                             "июля",
                             "августа",
                             "сентября",
                             "октября",
                             "ноября",
                             "декабря"};
//---------------------------------------------------------------------------
AnsiString GetWithUpperFirst(AnsiString str)
{
  if (!str.IsEmpty())
    str[1] = AnsiString(str[1]).UpperCase().operator[](1);
  return str;
}
AnsiString GetWithLowerFirst(AnsiString str)
{
  if (!str.IsEmpty())
    str[1] = AnsiString(str[1]).LowerCase().operator[](1);
  return str;
}
//---------------------------------------------------------------------------
AnsiString GetDateAsString(const AnsiString& str,bool isYear=true)
{
  AnsiString res = "";
  res += str.SubString(9,2);
  if (res.Length() == 1) res = "0" + res;
  res += " ";
  res += MounthNames[(str.SubString(6,2).ToInt()-1) % 12] + " ";
  res += str.SubString(1,4);
  if (isYear) res += " года";
  return res;
}
AnsiString GetYear(const AnsiString& str)
{
  AnsiString year = str.SubString(1,4);
  return year;
}
AnsiString GetMonth(const AnsiString& str)
{
  AnsiString m = str.SubString(6,2);
  return m;
}
AnsiString GetDay(const AnsiString& str)
{
  AnsiString d = str.SubString(9,2);
  return d;
}
AnsiString GetWeekAsString(int weeks)
{
  AnsiString res;
  int w10 = weeks%10;
  if (weeks>=10 && weeks <=20)
    res = "недель";
  else
    if (w10 >= 2 && w10 <= 4)
      res = "недели";
    else
      if (w10 == 1)
        res = "неделя";
      else
        res = "недель";
  return res;
}
AnsiString GetEst(float valBall)
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
void ClearVectorDiscips(m_vecDiscip& vec)
{
  for (int i=0;i<vec.size();++i)
    delete vec[i];
  vec.clear();
}
//---------------------------------------------------------------------------
__fastcall TFormReportPrilDiplomNepol::TFormReportPrilDiplomNepol(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportPrilDiplomNepol::SetIDStudent(int _IDStudent)
{
  idstudent=_IDStudent;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportPrilDiplomNepol::GetIDStudent(void)
{
  return idstudent;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportPrilDiplomNepol::ToolBtnPreviewClick(TObject *Sender)
{
  try
  {
 //   pFormData->ShowModal();
     CreateWordDocument();
//   bla();
  }
  catch(...)
  {
    Application->MessageBox("Работа с MS Word не возможна. Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportPrilDiplomNepol::ToolBtnCloseClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportPrilDiplomNepol::FormClose(TObject *Sender,
      TCloseAction &Action)
{

  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportPrilDiplomNepol::FormCreate(TObject *Sender)
{
  pFormData = new TFormDataForDiplom(this);


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
void __fastcall TFormReportPrilDiplomNepol::FormDestroy(TObject *Sender)
{
  pFormData->Free();
  WCDisconnect();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportPrilDiplomNepol::CreateWordDocument(void)
{
  int i;

  AnsiString SecNameS, FirstNameS, ThirdNameS;
  AnsiString BirstDateS;
  AnsiString PrevDocS, PrevDocYearS;
  AnsiString InS = " году в государственное образовательное учреждение высшего профессионального образования \"\"Нижегородский государственный архитектурно-строительный университет\"\"\n(заочная форма)";
  AnsiString OutS = " году в государственном образовательном учреждении высшего профессионального образования \"\"Нижегородский государственный архитектурно-строительный университет\"\"\n(заочная форма)";
  AnsiString NormPeriodStudyS = "       5 лет";
  AnsiString NapravSpecS, SpecializS;
  AnsiString CursWorks = "\nприведены в продолжении приложения к диплому";
  AnsiString MiscS = "приведены в продолжении приложения к диплому";
  AnsiString PracticaS, ItogGosEkzS;
  AnsiString VipQualificWorkS, ContVIP;
  AnsiString CityS = "г. Нижний Новгород";
  AnsiString VUZS = "Государственное\nобразовательное учреждение\nвысшего профессионального\nобразования \"\"Нижегородский\nгосударственный\nархитектурно-строительный\nуниверситет\"\"";
// изменил -------------------------------------------------------------
  AnsiString NumDiplomS, RegNumS, DataVidachiS, DataQualificS;

  AnsiString QualificS = "квалификация", QualificTitleS, PrevSpecS = "по специальности", SpecS, SexS;

  AnsiString Comment1 = "Государственное образовательное учреждение \"\"Нижегородский государственный архитектурно-строительный университет\"\" изменило свое наименование на государственное образовательное учреждение высшего профессионального образования \"\"Нижегородский государственный архитектурно-строительный университет\"\" 14 июня 2002 года.",
             Comment2 = "Образовательная программа освоена по сокращенной + ускоренной программе.";

  AnsiString InYear, InMonth, InDay, OutYear;
  AnsiString lang;
  InitPrivateData(SecNameS, FirstNameS, ThirdNameS, BirstDateS, VipQualificWorkS, PrevDocS,
        PrevDocYearS, InYear, InMonth, InDay, OutYear, NapravSpecS, SpecializS, QualificTitleS, SexS, lang,
        NumDiplomS, RegNumS, DataVidachiS, DataQualificS);

  WordMacros macros;
  macros.BeginMacros();

  //----------------------------------------------------------
  //              THE FIRST PAGE
  //----------------------------------------------------------
  int CountRows1 = 22;
  macros.InsertLine("ActiveDocument.PageSetup.Orientation=wdOrientLandscape");
  macros.InsertLine("ActiveDocument.PageSetup.TopMargin=25");
  macros.InsertLine("ActiveDocument.PageSetup.BottomMargin=25");
  macros.InsertLine("ActiveDocument.PageSetup.LeftMargin=25");
  macros.InsertLine("ActiveDocument.PageSetup.RightMargin=25");

  macros.InsertLine("ActiveDocument.Select");
  macros.TablesAdd(CountRows1, 2);
  macros.TableStyle(WordMacros::StyleNone);

  macros.InsertLine("ActiveDocument.Tables.Item(1).Range.Font.Name = \"Arial\"");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Range.Font.Spacing = 1.0");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Range.Font.Size = 12");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Range.Font.Italic = true");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Range.Font.Bold = false");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Columns.Item(2).Select");
   macros.SelectionParagraphFormat("LeftIndent = 25");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(1).SetHeight RowHeight:=72.00, HeightRule:=wdRowHeightAtLeast");
  for (i = 0; i < 6; ++i)
    macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(" + IntToStr(2+i) + ").SetHeight RowHeight:=14.00, HeightRule:=wdRowHeightAtLeast");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(8).SetHeight RowHeight:=50.00, HeightRule:=wdRowHeightAtLeast");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(9).SetHeight RowHeight:=20.00, HeightRule:=wdRowHeightAtLeast");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(10).SetHeight RowHeight:=45.00, HeightRule:=wdRowHeightAtLeast");
  for (i = 0; i < 3; ++i)
    macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(" + IntToStr(11+i) + ").SetHeight RowHeight:=14.00, HeightRule:=wdRowHeightAtLeast");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(14).SetHeight RowHeight:=63.00, HeightRule:=wdRowHeightAtLeast");
  for (i = 0; i < 5; ++i)
    macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(" + IntToStr(15+i) + ").SetHeight RowHeight:=14.00, HeightRule:=wdRowHeightAtLeast");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(20).SetHeight RowHeight:=56.00, HeightRule:=wdRowHeightAtLeast");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(21).SetHeight RowHeight:=14.00, HeightRule:=wdRowHeightAtLeast");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(22).SetHeight RowHeight:=14.00, HeightRule:=wdRowHeightAtLeast");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Columns.Item(1).Select");
  macros.InsertLine("ActiveDocument.Paragraphs.Format.Alignment=wdAlignParagraphLeft");
  macros.InsertLine("Selection.Font.Size = 12");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Columns.Item(2).Select");
  macros.InsertLine("ActiveDocument.Paragraphs.Format.Alignment=wdAlignParagraphCenter");
  macros.InsertLine("Selection.Font.Size = 12");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(2,2).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(2,2).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(2,2).Range.Text= \"г. Нижний Новгород\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(3,2).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(3,2).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(3,2).Range.Text=\"Государственное образовательное учреждение\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(4,2).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(4,2).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(4,2).Range.Text=\"высшего профессионального образования\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(5,2).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(5,2).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(5,2).Range.Text=\"\"\"Нижегородский государственный\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(6,2).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(6,2).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(6,2).Range.Text=\"архитектурно-строительный университет\"\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(9,2).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(9,2).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("Selection.Font.Bold=false");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(9,2).Range.Text=\"\"");/*NumDiplomS*/;

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(11,2).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(11,2).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(11,2).Range.Text=\""+ DataQualificS + "\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(12,2).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(12,2).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("Selection.Font.Size=14");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(12,2).Range.Text=\"" + SecNameS + "\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(13,2).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(13,2).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("Selection.Font.Size=14");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(13,2).Range.Text=\""+FirstNameS+" "+ThirdNameS+"\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(15,2).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(15,2).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(15,2).Range.Text=\"" + QualificTitleS.UpperCase()+ "\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(16,2).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(16,2).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(16,2).Range.Text=\"" + PrevSpecS + "\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(17,2).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(17,2).VerticalAlignment=wdCellAlignVerticalTop");
  macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=2, Extend:=wdExtend");
  macros.InsertLine("Selection.Cells.Merge");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(17,2).Range.Text=\"\"\""+NapravSpecS+"\"\"\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(21,1).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(21,1).Range.Paragraphs.Format.Alignment = wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(21,1).VerticalAlignment=wdCellAlignVerticalCenter");
  AnsiString mystr="                                  ";
  mystr+=RegNumS;
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(21,1).Range.Text=\"" + mystr + "\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(22,1).Range.Select");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(22,1).Range.Paragraphs.Format.Alignment = wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(22,1).VerticalAlignment=wdCellAlignVerticalCenter");
  mystr = "Дата выдачи "+DataVidachiS+" года";
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(22,1).Range.Text=\"" + mystr + "\"");

  macros.EndMacros();
  macros.RunMacros();

  //----------------------------------------------------------
  //              THE SECOND PAGE
  //----------------------------------------------------------
  macros.BeginMacros();
  macros.LoadTamplate("nepol_diplom.dot");

//#if 0   // нужно убрать
  bool isChangingNameVIZ = false;

  try {
    if ( (InYear.ToInt()<2002) ||
           ((InYear.ToInt()==2002) && ((InMonth.ToInt()<6) || ((InMonth.ToInt()==6) && (InDay.ToInt()<14)))))
    {
         InS = " году в государственное образовательное учреждение\n\"\"Нижегородский государственный архитектурно-строительный университет\"\"\n(заочная форма)";
         isChangingNameVIZ = true;
    }
  }
  catch (...)
  {
        InYear = "0";
        InMonth = "0";
        InDay = "0";
  }


  float itog_oc;
  InitPracticAndItog(PracticaS, itog_oc, ContVIP);
  ItogGosEkzS = "\n\nМеждисциплинарный экзамен по специальности \"\""+NapravSpecS+"\"\", "+GetEst(itog_oc);
  VipQualificWorkS = "\"" + VipQualificWorkS + "\"" + ContVIP;

  //macros.InsertLine("Selection.Range->InsertAfter(TVariant("\n\n"));
  //macros.SelectionTypeParagraph();
  //macros.SelectionTypeParagraph();

  int ns = 9;
  // Создание 1-й таблицы
  int CountTables = 0;
  int CountRows = 27;
  CountTables++;


  // Заполнение 1-й таблицы
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(4,2).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(4,2).VerticalAlignment=wdCellAlignVerticalTop");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size=9");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(4,2).Range.Text= \"" + CityS + "\"");
         /*
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(5,2).Range.Select");
  macros.InsertLine("Selection.ParagraphFormat.LineSpacingRule = wdLineSpace1pt5");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(5,2).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size=9");
  macros.InsertLine("Selection.MoveDown Unit:= wdLine, Count:=4, Extend:=wdExtend");
  macros.InsertLine("Selection.Cells.Merge");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(5,2).Range.Select");
  macros.SelectionText(VUZS);
           */

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(16,2).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(16,2).VerticalAlignment=wdCellAlignVerticalTop");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Size=9");
  macros.SelectionTypeParagraph();
  macros.SelectionTypeParagraph();
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(16,2).Range.Text= \"" + DataVidachiS + "\"");

 //---------------------------------------------

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(2,1).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(2,1).VerticalAlignment=wdCellAlignVerticalTop");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size=14");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(2,1).Range.Text= \"" + SecNameS+" "+FirstNameS+" "+ThirdNameS + "\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(6,1).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(6,1).VerticalAlignment=wdCellAlignVerticalTop");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size= "+IntToStr(ns));
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(6,1).Range.Text= \"" + PrevDocS+", "+PrevDocYearS + "\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(7,1).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size="+IntToStr(ns));
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(7,1).Range.Text= \"" + ((SexS.UpperCase() == "Ж") ? AnsiString("прошла") : AnsiString("прошел")) + "\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(9,1).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(9,1).VerticalAlignment=wdCellAlignVerticalTop");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size="+IntToStr(ns));
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(9,1).Range.Select");
  macros.SelectionText(InYear+InS);

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(12,1).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(12,1).VerticalAlignment=wdCellAlignVerticalTop");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size="+IntToStr(ns));
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(12,1).Range.Select");
  macros.SelectionText(OutYear + OutS);

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(13,1).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(13,1).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size="+IntToStr(ns));
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(13,1).Range.Text= \"" + NormPeriodStudyS + "\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(15,1).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(15,1).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size="+IntToStr(ns));
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(15,1).Range.Text= \"" + GetWithLowerFirst(NapravSpecS) + "\"");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(16,1).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(16,1).VerticalAlignment=wdCellAlignVerticalTop");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size="+IntToStr(ns));
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(16,1).Range.Select");
  macros.SelectionText(CursWorks);



  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(17,1).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(17,1).VerticalAlignment=wdCellAlignVerticalTop");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size="+IntToStr(ns));
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(17,1).Range.Select");
  macros.SelectionText(PracticaS);


  // Заполнение 2-й таблицы

  macros.InsertLine("ActiveDocument.Tables.Item(2).Cell(1,1).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(2).Cell(1,1).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size=14");
  macros.InsertLine("ActiveDocument.Tables.Item(2).Cell(1,1).Range.Text= \"" + SecNameS+" "+FirstNameS+" "+ThirdNameS + "\"");

  macros.InsertLine("ActiveDocument.Tables.Item(2).Cell(2,1).Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(2).Cell(2,1).VerticalAlignment=wdCellAlignVerticalCenter");
  macros.InsertLine("Selection.Font.Bold=true");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size=10");
  macros.InsertLine("ActiveDocument.Tables.Item(2).Cell(2,1).Range.Text= \"" + BirstDateS + "\"");

 
  // Заполнение 3-й таблицы

  m_vecDiscip vecComm, vecCurWork, vecCurProj, vecOther;
  InitVectorsOfDisciplines(vecComm, vecCurWork, vecCurProj, vecOther);

  CountRows = vecComm.size() + ((vecCurProj.size()) ? (1 + vecCurProj.size()) : 0) + 1;

  macros.InsertLine("ActiveDocument.Tables.Item(3).Rows.Item(1).Range.Select");
  macros.InsertLine("Selection.InsertRowsBelow " + AnsiString(CountRows - 1));


  macros.InsertLine("ActiveDocument.Tables.Item(3).Select");
  macros.InsertLine("Selection.Font.Italic=true");
  macros.InsertLine("Selection.Font.Size=8.5");

  macros.InsertLine("ActiveDocument.Tables.Item(3).Columns.Item(1).Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphRight");
  macros.InsertLine("ActiveDocument.Tables.Item(3).Columns.Item(2).Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.SelectionParagraphFormat("LeftIndent = -5");
  macros.InsertLine("ActiveDocument.Tables.Item(3).Columns.Item(3).Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.InsertLine("ActiveDocument.Tables.Item(3).Columns.Item(4).Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");

 // macros.InsertLine("ActiveDocument.Tables.Item(3).Rows.Item(1).SetHeight RowHeight:=50.00, HeightRule:=wdRowHeightAtLeast");

  int num_all_hours=0;
  int start_row = 1;
  for (i = 0; i < vecComm.size(); ++i)
  {
    num_all_hours+=vecComm[i]->time;
    macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row+i)+",1).VerticalAlignment=wdCellAlignVerticalTop");
    macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row+i)+",2).VerticalAlignment=wdCellAlignVerticalTop");
    macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row+i)+",3).VerticalAlignment=wdCellAlignVerticalBottom");
    macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row+i)+",4).VerticalAlignment=wdCellAlignVerticalBottom");

    macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row+i)+",1).Range.Text= \"" + AnsiString(i+1)+"." + "\"");
    if (vecComm[i]->title.UpperCase().Trim() == "ИНОСТРАННЫЙ ЯЗЫК")
    {
      vecComm[i]->title = vecComm[i]->title + " (" + lang + ")";
    }
    macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row+i)+",2).Range.Text= \"" + vecComm[i]->title + "\"");
    macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row+i)+",3).Range.Text= \"" + AnsiString(vecComm[i]->time) + "\"");
    macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row+i)+",4).Range.Text= \"" + arrOcenk[vecComm[i]->estimation] + "\"");
  }
  start_row+=vecComm.size();

  if (vecCurProj.size()>0)
  {
    macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row)+",2).Range.Select");
    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
    macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row)+",2).Range.Text=\"Курсовые проекты\"");
    start_row++;
    for (i = 0; i < vecCurProj.size(); ++i)
    {
        macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row+i)+",1).Range.Text= \"" + AnsiString(i+1) + "\"");
        macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row+i)+",2).Range.Text= \"" + vecCurProj[i]->title + "\"");
        macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row+i)+",4).Range.Text= \"" + arrOcenk[vecCurProj[i]->estimation] + "\"");
    }
    start_row+=vecCurProj.size();
  }

  macros.InsertLine("ActiveDocument.Tables.Item(3).Rows.Item("+IntToStr(start_row)+").Range.Borders.Item(wdBorderTop).LineStyle = wdLineStyleSingle");
  macros.InsertLine("ActiveDocument.Tables.Item(3).Rows.Item("+IntToStr(start_row)+").Range.Borders.Item(wdBorderTop).LineWidth = wdLineWidth050pt");
  macros.InsertLine("ActiveDocument.Tables.Item(3).Rows.Item("+IntToStr(start_row)+").Range.Borders.Item(wdBorderTop).Color = wdColorBlack");

  // изменил
  macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row)+",2).Range.Select");
  macros.SelectionText("Всего\nВ том числе аудиторных");
  macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row)+",3).Range.Select");
  macros.SelectionText(AnsiString(num_all_hours)+"\n????");

  macros.InsertLine("ActiveDocument.Tables.Item(3).Cell("+IntToStr(start_row)+",1).Range.Select");
  macros.InsertLine("Selection.MoveDown Unit:=wdLine");
  macros.InsertLine("Selection.Font.Bold = false");
  macros.InsertLine("Selection.Font.Italic = false");
  macros.InsertLine("Selection.Font.Size = 8");
  int delta = -1000;
  try
  {
    int out = OutYear.ToInt();
    int in = InYear.ToInt();
    delta = out - in;
  }
  catch(...)
  {}
  if (delta != -1000 && delta < 5)
  {
    macros.InsertLine("Selection.TypeText \"" + Comment2 + "\"");
    macros.SelectionTypeParagraph();
  }
  if (isChangingNameVIZ)
  {
    macros.InsertLine("Selection.TypeText \"" + Comment1 + "\"");
    macros.SelectionTypeParagraph();
  }
  macros.InsertLine("Selection.Font.Bold = true");
  macros.InsertLine("Selection.TypeText \"Конец документа\"");


  AnsiString ms="";
  for (i=0;i<vecCurWork.size();++i)
  {
    if (i>0)
    {
        ms += ";";
        ms += "\n";
    }
    ms += vecCurWork[i]->title+", "+arrOcenk[vecCurWork[i]->estimation];
  }
  // добавил
  macros.InsertLine("ActiveDocument.Tables.Item(1).Cell(16,1).Range.Select");
  macros.SelectionText(ms);
  ClearVectorDiscips(vecComm);
  ClearVectorDiscips(vecCurWork);
  ClearVectorDiscips(vecCurProj);
  ClearVectorDiscips(vecOther);


  macros.EndMacros();
  macros.RunMacros();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportPrilDiplomNepol::InitPrivateData(AnsiString& SN, AnsiString& FN, AnsiString& TN,
        AnsiString& BirstDate, AnsiString& VipQualifWork, AnsiString& PrevDoc, AnsiString& PrevDocYear,
        AnsiString& InYear, AnsiString& InMonth, AnsiString& InDay, AnsiString& OutYear, AnsiString& spec, AnsiString& specializ,
        AnsiString& Qualific, AnsiString& Sex, AnsiString& lang,
        AnsiString&  NumDiplomS, AnsiString& RegNumS, AnsiString& DataVidachiS, AnsiString& DataQualificS)
{
    SN = "???";
    FN = "???";
    TN = "???";
    BirstDate = "???";
    VipQualifWork = "???";
    PrevDoc = "???";
    PrevDocYear = "???";
    InYear = "???";
    InMonth = "0";
    InDay = "0";
    OutYear = "???";
    spec = "???";
    specializ = "???";
    Qualific = "???";
    Sex = "Средний пол";
    lang = "???";
// добавил
    NumDiplomS = "xxx 0000000";
    RegNumS = "00000";
    DataVidachiS = "00.00.0000";
    DataQualificS = "00.00.0000";

    // из-за ошибки с неверной датой переписал код работы с БД
    // ошибка связяна с Правильной работой ZMySqlQuery
    MYSQL_RES *result;
    MYSQL_ROW row;

    AnsiString myquery = "select s.secondname,s.firstname,s.thirdname,s.bdate,s.vkr_title," \
        "s.edudocid,s.eduenddate,s.specid,s.enterdate,s.exitdate,s.sex,v.title,s.edunumdiplom,"\
        "s.edunumreg,s.edudatediplom,s.edudatequalif from "+opts.DBStudTable+" as s, "\
        +opts.DBVocTable+" as v where s.deleted=0 and v.deleted=0 and s.id="\
        +ToStr(AnsiString(idstudent))+" and s.languageid=v.num and v.vkey="+ToStr("language");

    mysql_query(mysql,myquery.c_str());
    if (mysql_field_count(mysql))
    {
        result=mysql_store_result(mysql);
        if (result && mysql_num_rows(result))
        {
             while (row = mysql_fetch_row(result))
            {
                SN = AnsiString(row[0]);
                FN = AnsiString(row[1]);
                TN = AnsiString(row[2]);
                BirstDate = GetDateAsString(AnsiString(row[3]));
                VipQualifWork = AnsiString(row[4]);
                PrevDoc = WCGetTitleForKeyNum(EDUDOCS, AnsiString(row[5]).ToInt());
                PrevDocYear = "выданный в " + GetYear(AnsiString(row[6])) + " году";
                spec = WCGetTitleForKeyNum(SPECS, AnsiString(row[7]).ToInt());
                specializ = WCGetTitleForKeyNum(SPEZIALIZS, AnsiString(row[7]).ToInt());
                Qualific = WCGetTitleForKeyNum(QUALIFIC, AnsiString(row[7]).ToInt());
                InYear = GetYear(AnsiString(row[8]));
                InMonth = GetMonth(AnsiString(row[8]));
                InDay = GetDay(AnsiString(row[8]));
                OutYear = GetYear(AnsiString(row[9]));
                Sex = AnsiString(row[10]);
                lang = AnsiString(row[11]);
// добавил ------------------------------
                try{
                    NumDiplomS = AnsiString(row[12]);
                    RegNumS = AnsiString(row[13]);
                    DataVidachiS = GetDateAsString(AnsiString(row[14]), false);
                    DataQualificS = GetDateAsString(AnsiString(row[15]), false);
                }
                catch(...)
                {
                    NumDiplomS = "xxx 0000000";
                    RegNumS = "00000";
                    DataVidachiS = "00.00.0000";
                    DataQualificS = "00.00.0000";
                }
            }
        }
        mysql_free_result(result);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportPrilDiplomNepol::InitPracticAndItog(AnsiString& pract, float& itog_oc, AnsiString& VIP)
{
  pract = "???";
  itog_oc = 0.0;
  VIP = "";

  ZMySqlQuery->SQL->Clear();
  AnsiString query = "select di.fulltitle,di.num_hours,di.idclass,pr.estimation,pr.ball from "+opts.DBDisciplines+" as di, "+opts.DBProgress+" as pr where di.deleted=0 and pr.deleted=0 and pr.idstud="+ToStr(AnsiString(idstudent))+" and pr.iddiscip=di.id order by di.scan_number";
  ZMySqlQuery->SQL->Add(query);
  ZMySqlQuery->Active=true;

  if (ZMySqlQuery->RecordCount==0) return;

  pract = "";
  itog_oc = 0.0;
  VIP = ",\n";
  int i;
  int weeks;
  int d_class;
  int num_itog=0;
  for (i = 0; i < ZMySqlQuery->RecordCount; ++i)
  {
    ZMySqlQuery->RecNo = i+1;
    d_class = ZMySqlQuery->Fields->FieldByNumber(3)->AsString.ToInt();
    if (d_class == 4)
    {
      if (!pract.IsEmpty()) pract += ";\n";
      pract += ZMySqlQuery->Fields->FieldByNumber(1)->AsString;
      pract += ", ";
      weeks = ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt();
      pract += AnsiString(weeks);
      pract += " ";
      pract += GetWeekAsString(weeks);
      pract += ", ";
      pract += arrOcenk[ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt()];
    }
    if (d_class == 5)
    {
      num_itog++;
      itog_oc += ZMySqlQuery->Fields->FieldByNumber(5)->AsString.ToDouble();
    }
    if (d_class == 6)
    {
      weeks = ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt();
      VIP += AnsiString(weeks);
      VIP += " ";
      VIP += GetWeekAsString(weeks);
      VIP += ", ";
      VIP += arrOcenk[ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt()];
    }
  }
  if (num_itog > 0)
  {
    itog_oc = itog_oc / (float)num_itog;
    itog_oc = (float(int(itog_oc*10)) + (((itog_oc*10 - float((int(itog_oc*10)))) >= 0.5f) ? 1.0 : 0.0)) / 10.0f;
    itog_oc = float(itog_oc);
  }

}
//---------------------------------------------------------------------------
bool IsEqualDiscips(m_SDiscip& one, m_SDiscip& two)
{
  bool equal=false;
  if ((one.iddiscip == two.iddiscip) && (one.title == two.title)
      /*&&
      ((one.estimation>=te5 && one.estimation<=te2 && two.estimation>=te5 && two.estimation<=te2)
       ||
       (one.estimation>=teZach && one.estimation<=teNeZach && two.estimation>=teZach && two.estimation<=teNeZach))*/)
           equal=true;
  return equal;
}
//---------------------------------------------------------------------------
void InsertIntoVectorDiscips(m_vecDiscip& vec, m_SDiscip* discip)
{
  int i;
  m_SDiscip* vDiscip;
  for (i=vec.size()-1; i>=0; --i)
  {
    vDiscip = vec[i];
    if (IsEqualDiscips(*discip, *vDiscip))
        break;
  }
  if (i>=0)
  {
    vDiscip = vec[i];
    if (discip->num_semestr >= vDiscip->num_semestr)
    {
      delete vDiscip;
      vec[i] = discip;
    }
    else
    {
      delete discip;
      discip=0;
    }
  }
  else
  {
    vec.push_back(discip);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportPrilDiplomNepol::InitVectorsOfDisciplines(m_vecDiscip& vecComm,
        m_vecDiscip& vecCurWorks, m_vecDiscip& vecCurProj, m_vecDiscip& vecOther)
{
  int i;
  ClearVectorDiscips(vecComm);
  ClearVectorDiscips(vecCurWorks);
  ClearVectorDiscips(vecCurProj);
  ClearVectorDiscips(vecOther);

  ZMySqlQuery->SQL->Clear();
  AnsiString query = "select di.fulltitle,di.num_hours,di.idclass,pr.estimation,di.scan_number,pr.iddiscip,pr.numplansemestr from "+opts.DBDisciplines+" as di, "+opts.DBProgress+" as pr where di.deleted=0 and pr.deleted=0 and pr.idstud="+ToStr(AnsiString(idstudent))+" and pr.iddiscip=di.id order by di.scan_number, pr.numplansemestr";
  ZMySqlQuery->SQL->Add(query);
  ZMySqlQuery->Active=true;

  if (ZMySqlQuery->RecordCount==0) return;
  m_SDiscip* discip;
  int d_class;

  for (i = 0; i < ZMySqlQuery->RecordCount; ++i)
  {
    ZMySqlQuery->RecNo = i+1;
    d_class = ZMySqlQuery->Fields->FieldByNumber(3)->AsString.ToInt();

    discip = new m_SDiscip;
    discip->iddiscip = ZMySqlQuery->Fields->FieldByNumber(6)->AsString.ToInt();
    discip->title = ZMySqlQuery->Fields->FieldByNumber(1)->AsString;
    discip->time = ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt();
    discip->estimation = ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt();
    discip->num_semestr = ZMySqlQuery->Fields->FieldByNumber(7)->AsString.ToInt();

    switch (d_class)
    {
    case tdCommon: InsertIntoVectorDiscips(vecComm, discip);break;
    case tdCourseWork: InsertIntoVectorDiscips(vecCurWorks, discip);break;
    case tdCourseProject: InsertIntoVectorDiscips(vecCurProj, discip);break;
    default: InsertIntoVectorDiscips(vecOther, discip);break;
    }                      
  }
}

void TFormReportPrilDiplomNepol::bla()
{
  WordMacros macros;
  macros.BeginMacros();

  macros.EndMacros();
  macros.RunMacros();
}

