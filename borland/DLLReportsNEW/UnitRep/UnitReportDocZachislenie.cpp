//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportDocZachislenie.h"
#include "UnitFuncs.h"
#include "MacroWord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportDocZachislenie *FormReportDocZachislenie;

extern EDRegOpts opts;
//---------------------------------------------------------------------------
__fastcall TFormReportDocZachislenie::TFormReportDocZachislenie(TComponent* Owner): TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocZachislenie::InitReportQuery(void)
{
  ZMySqlQuery->SQL->Clear();
  AnsiString WhereStr=pFormList->GetWHEREStr();
  ZMySqlQuery->SQL->Add("select secondname,firstname,thirdname,specid,eduformid,dogshifrid,dognum,dogyearid,dogfastid from "+opts.DBStudTable+" where deleted=0 "+WhereStr+" order by specid,eduformid,secondname,firstname,thirdname");
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocZachislenie::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocZachislenie::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocZachislenie::CreateWordDocument(void)
{
    int numspec,numform;

    InitReportQuery();

    WordMacros macros;
    macros.BeginMacros();

    macros.InsertLine("Selection.SetRange Start:=0, End:=0");

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphJustify");
    macros.SelectionFont("Bold = true");
    macros.SelectionText("Слушали: ");
    macros.SelectionFont("Bold = false");
    macros.SelectionText("о зачислении студентами и слушателями ННГАСУ на места с оплатой стоимости обучения на договорной основе.");
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold = true");
    macros.SelectionText("Постановили: ");
    macros.SelectionFont("Bold = false");
    macros.SelectionText("зачислить студентами и слушателями ННГАСУ на места с оплатой стоимости обучения на договорной основе.");
    macros.SelectionTypeParagraph();
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold = false");
    macros.SelectionText("Зачислить студентами 1-го курса с 01 сентября 20__ года на места с оплатой стоимости обучения на договорной основе следующих абитуриентов:");
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold = true");
    macros.SelectionText(" в институт экономики, управления и права");
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold = false");

    if (ZMySqlQuery->RecordCount==0)
        return;

    numspec=-1;
    numform=-1;
    int CountTables=0;
    int CountRows;
    int CountPrevRows=0;
    int CountPrevPrevRows=0;
    AnsiString Str,Strtmp;
    for (int i = 0; i < ZMySqlQuery->RecordCount; i++)
    {
        ZMySqlQuery->RecNo = i+1;
        if ( numspec!=ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt() ||
                numform!=ZMySqlQuery->Fields->FieldByNumber(5)->AsString.ToInt() )
        {
            numspec=ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt();
            numform=ZMySqlQuery->Fields->FieldByNumber(5)->AsString.ToInt();
            macros.SelectionTypeParagraph();
            macros.SelectionTypeParagraph();
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.SelectionText("на 1 курс по специальности \"");
            macros.SelectionFont("Bold = true");
            macros.SelectionText(WCGetTitleForKeyNum(SPECS,numspec));
            macros.SelectionText("\"");
            macros.SelectionFont("Bold = false");
            macros.SelectionText(", с формой обучения - ");
            macros.SelectionText(WCGetTitleForKeyNum(EDUFORMS,numform));
            macros.SelectionTypeParagraph();
            macros.SelectionTypeParagraph();
            CountRows=0;
            for (int j=i;j<ZMySqlQuery->RecordCount;j++)
            {
                ZMySqlQuery->RecNo=j+1;
                if (numspec!=ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt() ||
                    numform!=ZMySqlQuery->Fields->FieldByNumber(5)->AsString.ToInt()) break;
                CountRows++;
            }
            ZMySqlQuery->RecNo=i+1;
            macros.TablesAdd(CountRows, 6);
            macros.TableStyle(WordMacros::StyleNone);
            CountTables++;

            macros.TablesColumns(CountTables, 1, "Width = 30");
            macros.TablesColumns(CountTables, 2, "Width = 90");
            macros.TablesColumns(CountTables, 3, "Width = 80");
            macros.TablesColumns(CountTables, 4, "Width = 90");
            macros.TablesColumns(CountTables, 5, "Width = 100");
            macros.TablesColumns(CountTables, 6, "Width = 50");

            macros.TablesColumns(CountTables, 1, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphRight");
            macros.TablesColumns(CountTables, 2, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 3, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 4, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 5, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphRight");
            macros.TablesColumns(CountTables, 6, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.InsertLine("ActiveDocument.Tables.Item(" + IntToStr(CountTables) + ").Range.Font.Size = 10");

            CountPrevPrevRows=CountPrevRows;
            CountPrevRows+=CountRows;
        }
        macros.TablesCell(CountTables, i+1-CountPrevPrevRows, 1, "Range.Text = \"" + AnsiString(i+1-CountPrevPrevRows) + "\"");
        macros.TablesCell(CountTables, i+1-CountPrevPrevRows, 2, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(1)->AsString + "\"");
        macros.TablesCell(CountTables, i+1-CountPrevPrevRows, 3, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(2)->AsString + "\"");
        macros.TablesCell(CountTables, i+1-CountPrevPrevRows, 4, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(3)->AsString + "\"");

        Str=WCGetTitleForKeyNum(DOGSHIFRS,ZMySqlQuery->Fields->FieldByNumber(6)->AsString.ToInt())+"-";
        Str+=WCGetTitleForKeyNum(DOGYEARS,ZMySqlQuery->Fields->FieldByNumber(8)->AsString.ToInt());
        Strtmp=WCGetTitleForKeyNum(DOGFASTS,ZMySqlQuery->Fields->FieldByNumber(9)->AsString.ToInt());
        if (!Strtmp.IsEmpty())
            Str+="-"+Strtmp;
        Str+=" -";
        macros.TablesCell(CountTables, i+1-CountPrevPrevRows, 5, "Range.Text = \"" + Str + "\"");
        macros.TablesCell(CountTables, i+1-CountPrevPrevRows, 6, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(7)->AsString + "\"");
        if (i+1 == CountPrevRows)
        {
           macros.TablesCell(CountTables, CountRows, 1, "Range.Select");
           macros.InsertLine("Selection.MoveDown Unit := wdLine, Count := 2");
        }
    }
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionText("Председатель приемной комиссии");
    macros.SelectionTypeParagraph();
    macros.SelectionText("ректор, академик                                                                       Е.В. Копосов");
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionText("Ответственный секретарь");
    macros.SelectionTypeParagraph();
    macros.SelectionText("приемной комиссии, доцент                                                   Н.Д. Жилина");
    macros.InsertLine("ActiveWindow.ActivePane.VerticalPercentScrolled = 0");
    
    macros.EndMacros();
    macros.RunMacros();

}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocZachislenie::ToolBtnPreviewClick(
      TObject *Sender)
{
  try
  {
    pFormList->ShowModal();

    CreateWordDocument();
  }
  catch(...)
  {
    Application->MessageBox("Работа с MS Word не возможна. Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocZachislenie::FormCreate(TObject *Sender)
{
  pFormList = new TFormListGroup(this);

  bool NoError;
  try
  {
    ZMySqlDatabase->HostName = opts.DBHost;
    ZMySqlDatabase->User = opts.DBLogin;
    ZMySqlDatabase->Password = opts.DBPasswd;
    ZMySqlDatabase->Database = opts.DBName;
    ZMySqlDatabase->Connected = true;

    NoError = ZMySqlDatabase->Connected;
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
void __fastcall TFormReportDocZachislenie::FormDestroy(
      TObject *Sender)
{
  pFormList->Free();
  WCDisconnect();
}
//---------------------------------------------------------------------------
