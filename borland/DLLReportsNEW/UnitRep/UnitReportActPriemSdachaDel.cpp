//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportActPriemSdachaDel.h"
#include "UnitFuncs.h"
#include "MacroWord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportActPriemSdachaDel *FormReportActPriemSdachaDel;

extern EDRegOpts opts;
AnsiString GetDocumentStr(int edudocid,AnsiString edudocinfo)
{
  AnsiString Str;
  try
  {
    AnsiString Strtmp=WCGetTitleForKeyNum(EDUDOCS,edudocid);
    Str=Strtmp.SubString(1,Strtmp.Pos(" ")-1);
    AnsiString prilStr;
    if (Str.UpperCase()=="ДИПЛОМ")
        prilStr="приложение к диплому";
    else
      if (Str.UpperCase()=="АТТЕСТАТ")
        prilStr="приложение к аттестату";
      else
      {
        prilStr="приложение к \'"+Strtmp+"\'";
        Str=Strtmp;
      }
    Str+=" "+edudocinfo+",\nSelection.Collapse Direction:=wdCollapseEnd\nSelection.TypeParagraph\nSelection.TypeText Text:=\"";
    Str+=prilStr;
//    MessageBox(0,Str.c_str(),"",MB_OK);
  }
  catch(...)
  {
    Str=edudocinfo+"\nSelection.Collapse Direction:=wdCollapseEnd\nSelection.TypeParagraph\nSelection.TypeText Text:=\"Ни диплома, ни аттестата";
  }
  return Str;
}
//---------------------------------------------------------------------------
__fastcall TFormReportActPriemSdachaDel::TFormReportActPriemSdachaDel(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportActPriemSdachaDel::SetIDGroup(int _IDGroup)
{
  idgroup=_IDGroup;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportActPriemSdachaDel::GetIDGroup(void)
{
  return idgroup;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportActPriemSdachaDel::InitReportQuery(void)
{
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("select CONCAT(secondname,\' \',firstname,\' \',thirdname),specid,eduformid,edudocid,edudocinfo from "+opts.DBStudTable+" where deleted=0 and grpid="+ToStr(AnsiString(idgroup))+" order by specid,eduformid,secondname");
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportActPriemSdachaDel::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportActPriemSdachaDel::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportActPriemSdachaDel::CreateWordDocument(void)
{
    int numspec,numform;

    InitReportQuery();

    WordMacros macros;
    macros.BeginMacros();
    macros.PageSetup("");

    macros.InsertLine("Selection.SetRange Start:=0, End:=0");

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionText("АКТ ПРИЕМА-СДАЧИ");
    macros.SelectionTypeParagraph();
    macros.SelectionText("личных дел студентов 1 курса института экономики, управления и права");
    macros.SelectionTypeParagraph();
    macros.SelectionText("Мною, методистом института экономики, управления и права __________________________ , сданы специалисту по кадровой работе");
    macros.SelectionTypeParagraph();
    macros.SelectionText("1 категории __________________________ следующие дела вновь принятых на 1 курс студентов института экономики, управления и права.");
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionText("Прием 20__г.");
    macros.SelectionTypeParagraph();

    if (ZMySqlQuery->RecordCount==0) return;

    numspec=-1;
    numform=-1;
    int CountTables=0;
    int CountRows;
    int CountPrevRows=0;
    int CountPrevPrevRows=0;
    AnsiString Str,Strtmp;
    for (int i=0;i<ZMySqlQuery->RecordCount;i++)
    {
        ZMySqlQuery->RecNo=i+1;
        if ( numspec!=ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt() ||
            numform!=ZMySqlQuery->Fields->FieldByNumber(3)->AsString.ToInt() )
        {
            numspec=ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt();
            numform=ZMySqlQuery->Fields->FieldByNumber(3)->AsString.ToInt();

            macros.SelectionTypeParagraph();
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.SelectionText("Специальность \"");
            macros.SelectionText(WCGetTitleForKeyNum(SPECS,numspec));
            macros.SelectionText("\"\" - ");
            macros.SelectionText(WCGetTitleForKeyNum(EDUFORMS,numform));
            macros.SelectionTypeParagraph();
            CountRows=0;
            for (int j=i;j<ZMySqlQuery->RecordCount;j++)
            {
                ZMySqlQuery->RecNo=j+1;
                if (numspec!=ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt() ||
                    numform!=ZMySqlQuery->Fields->FieldByNumber(3)->AsString.ToInt()) break;
                CountRows++;
            }
            ZMySqlQuery->RecNo=i+1;
            macros.TablesAdd(CountRows+1, 6);
            CountTables++;
            macros.InsertLine("ActiveDocument.Tables.Item("+IntToStr(CountTables)+").Range.Font.Size = 10");

            macros.TablesColumns(CountTables, 1, "Width = 55");
            macros.TablesColumns(CountTables, 2, "Width = 240");
            macros.TablesColumns(CountTables, 3, "Width = 60");
            macros.TablesColumns(CountTables, 4, "Width = 150");
            macros.TablesColumns(CountTables, 5, "Width = 100");

            macros.TablesColumns(CountTables, 1, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesColumns(CountTables, 2, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 3, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesColumns(CountTables, 4, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 5, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesColumns(CountTables, 6, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");

            macros.InsertLine("ActiveDocument.Tables.Item("+IntToStr(CountTables)+").Rows.Item(1).Range.Font.Bold = true");

            macros.TablesCell(CountTables, 1, 1, "Range.Text = \"№п\\п\"");
            macros.TablesCell(CountTables, 1, 1, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 2, "Range.Text = \"Фамилия, имя, отчество\"");
            macros.TablesCell(CountTables, 1, 2, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 3, "Range.Text = \"Кол-во листов\"");
            macros.TablesCell(CountTables, 1, 3, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 4, "Range.Text = \"Примечание\"");
            macros.TablesCell(CountTables, 1, 4, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 5, "Range.Text = \"Номер зачетной книжки\"");
            macros.TablesCell(CountTables, 1, 5, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 6, "Range.Text = \"Номер группы\"");
            macros.TablesCell(CountTables, 1, 6, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");

            CountPrevPrevRows=CountPrevRows;
            CountPrevRows+=CountRows;
        }

        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 1, "Range.Text = \"" + AnsiString(i+1-CountPrevPrevRows) + "\"");
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 2, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(1)->AsString + "\"");
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 4, "Range.Select");
        macros.InsertLine("Selection.TypeText Text:= \"" + GetDocumentStr(ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt(),ZMySqlQuery->Fields->FieldByNumber(5)->AsString) + "\"");

        if (i+1 == CountPrevRows)
        {
            macros.TablesCell(CountTables, CountRows+1, 1, "Range.Select");
            macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count := 2");
        }
    }

    macros.SelectionTypeParagraph();
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.SelectionTypeParagraph();
    macros.SelectionText("Итого \"\"дел\"\" - "+AnsiString(ZMySqlQuery->RecordCount)+" - (___прописью_________________________)");
    macros.SelectionTypeParagraph();
    macros.SelectionText("Сдал: __________________________ ");
    macros.SelectionTypeParagraph();
    macros.SelectionText("Принял: __________________________ ");
    macros.SelectionTypeParagraph();
    macros.InsertLine("ActiveWindow.ActivePane.VerticalPercentScrolled = 0");

    macros.EndMacros();
    macros.RunMacros();

/*  for (int i=1;i<=ZMySqlQuery->RecordCount;i++)
  {
    ZMySqlQuery->RecNo=i;
    WordDocument->Tables->Item(1)->Cell(i+1,1)->Range->Text=TVariant(AnsiString(i));
    WordDocument->Tables->Item(1)->Cell(i+1,2)->Range->Text=TVariant(ZMySqlQuery->Fields->FieldByNumber(1)->AsString);
    WordDocument->Tables->Item(1)->Cell(i+1,4)->Range->Text=TVariant(GetDocumentStr(ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt(),ZMySqlQuery->Fields->FieldByNumber(5)->AsString));
    if (i==ZMySqlQuery->RecordCount)
      WordDocument->Tables->Item(1)->Cell(i+1,1)->Range->Select();
  }*/
}
//---------------------------------------------------------------------------
void __fastcall TFormReportActPriemSdachaDel::ToolBtnPreviewClick(
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
void __fastcall TFormReportActPriemSdachaDel::FormCreate(TObject *Sender)
{

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

  NoError = NoError && WCConnect();
  if ( !NoError)
  {
    AnsiString msg="Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportActPriemSdachaDel::FormDestroy(TObject *Sender)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------

