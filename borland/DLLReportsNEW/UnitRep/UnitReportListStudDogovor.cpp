//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportListStudDogovor.h"
#include "UnitFuncs.h"
#include "MacroWord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportListStudDogovor *FormReportListStudDogovor;

extern EDRegOpts opts;
//---------------------------------------------------------------------------
__fastcall TFormReportListStudDogovor::TFormReportListStudDogovor(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportListStudDogovor::InitReportQuery(void)
{
  ZMySqlQuery->SQL->Clear();
  AnsiString WhereStr=pFormList->GetWHEREStr();
  ZMySqlQuery->SQL->Add("select CONCAT(secondname,\' \',firstname,\' \',thirdname) as fullname,specid,dogshifrid,dognum,dogyearid,dogfastid,grpid from "+opts.DBStudTable+" where deleted=0 "+WhereStr+" order by specid,dogfastid,fullname");
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportListStudDogovor::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();      
}
//---------------------------------------------------------------------------
void __fastcall TFormReportListStudDogovor::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportListStudDogovor::CreateWordDocument(void)
{
    InitReportQuery();

    if (ZMySqlQuery->RecordCount == 0)
        return;

    WordMacros macros;
    macros.BeginMacros();

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionFont("Bold = true");
    macros.SelectionFont("Size = 16");
    macros.SelectionText("Договора абитуриентов");
    macros.SelectionFont("Size = 12");
    macros.SelectionFont("Bold = false");
    macros.SelectionParagraphFormat("Alignment = dAlignParagraphLeft");

    int numspec = -1;
    int CountTables = 0;   // кол-во таблиц
    int CountRows;         //  строк в текущ таблице
    int CountPrevRows = 0;  // сколько будет использовано
    int CountPrevPrevRows = 0; // строк использовано
    AnsiString Str,Strtmp;
    // проходим по всем записям последовательно
    for (int i = 0; i < ZMySqlQuery->RecordCount; i++)
    {
        ZMySqlQuery->RecNo = i + 1;
        // если такой спец нету то делаем новую таблицу
        if (numspec != ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt())
        {
            numspec = ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt();

            macros.SelectionTypeParagraph();
            macros.SelectionTypeParagraph();
            macros.SelectionTypeParagraph();
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.SelectionText("Cпециальность \"");
            macros.SelectionFont("Bold = true");
            macros.SelectionText(WCGetTitleForKeyNum(SPECS,numspec));
            macros.SelectionText("\"");
            macros.SelectionFont("Bold = false");
            macros.SelectionTypeParagraph();
            macros.SelectionTypeParagraph();
            // узнаем кол-во записей
            CountRows=0;
            for (int j = i; j < ZMySqlQuery->RecordCount; j++)
            {
                ZMySqlQuery->RecNo = j + 1;
                if (numspec != ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt() ) break;
                    CountRows++;
            }
            //формируем таблицу
            ZMySqlQuery->RecNo=i+1;
            macros.TablesAdd(CountRows+1, 5);
            CountTables++;

            macros.TablesColumns(CountTables, 1, "Width = 30");
            macros.TablesColumns(CountTables, 2, "Width = 190");
            macros.TablesColumns(CountTables, 3, "Width = 70");
            macros.TablesColumns(CountTables, 4, "Width = 100");
            macros.TablesColumns(CountTables, 5, "Width = 60");

            macros.TablesColumns(CountTables, 1, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 2, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 3, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesColumns(CountTables, 4, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphRight");
            macros.TablesColumns(CountTables, 5, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.InsertLine("ActiveDocument.Tables.Item("+IntToStr(CountTables)+").Range.Font.Size = 10");

            macros.InsertLine("ActiveDocument.Tables.Item("+IntToStr(CountTables)+").Rows.Item(1).Range.Font.Bold = true");
//          WordDocument->Tables->Item(CountTables)->Range->Select();

            macros.TablesCell(CountTables, 1, 1, "Range.Text = \"№\"");
            macros.TablesCell(CountTables, 1, 1, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 2, "Range.Text = \"Фамилия, имя, отчество\"");
            macros.TablesCell(CountTables, 1, 2, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 3, "Range.Text = \"Группа\"");
            macros.TablesCell(CountTables, 1, 3, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 4, "Range.Text = \"Шифр договора\"");
            macros.TablesCell(CountTables, 1, 4, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 5, "Range.Text = \"Номер договора\"");
            macros.TablesCell(CountTables, 1, 5, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");

            CountPrevPrevRows = CountPrevRows;
            CountPrevRows += CountRows;
        }
        // вводим данные в ячейки
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 1, "Range.Text = \"" + AnsiString(i+1-CountPrevPrevRows) + "\"");
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 2, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(1)->AsString + "\"");
        AnsiString GroupStr=WCGetTitleForKeyNum(GROUPS,ZMySqlQuery->Fields->FieldByNumber(7)->AsString.ToInt());
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 3, "Range.Text = \"" + GroupStr + "\"");
        Str = WCGetTitleForKeyNum(DOGSHIFRS, ZMySqlQuery->Fields->FieldByNumber(3)->AsString.ToInt())+"-";
        Str += WCGetTitleForKeyNum(DOGYEARS, ZMySqlQuery->Fields->FieldByNumber(5)->AsString.ToInt());
        Strtmp = WCGetTitleForKeyNum(DOGFASTS, ZMySqlQuery->Fields->FieldByNumber(6)->AsString.ToInt());
        if (!Strtmp.IsEmpty())
            Str += "-" + Strtmp;
        Str += "-";
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 4, "Range.Text = \"" + Str + "\"");
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 5, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(4)->AsString + "\"");
        // проверка перехода на новую строку
        if (i+1 == CountPrevRows)
        {
             macros.TablesCell(CountTables, CountRows + 1, 1, "Range.Select");
             macros.InsertLine("Selection.MoveDown Unit := wdLine, Count:= 2");
        }
    }
    macros.EndMacros();
    macros.RunMacros();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportListStudDogovor::ToolBtnPreviewClick(
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
void __fastcall TFormReportListStudDogovor::FormCreate(TObject *Sender)
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
    NoError = ZMySqlDatabase->Connected;
  }

  NoError=NoError && WCConnect();
  if ( !NoError)
  {
    AnsiString msg = "Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportListStudDogovor::FormDestroy(TObject *Sender)
{
  pFormList->Free();
  WCDisconnect();
}
//---------------------------------------------------------------------------
