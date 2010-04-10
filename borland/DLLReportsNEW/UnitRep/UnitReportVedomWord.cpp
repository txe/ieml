//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportVedomWord.h"
#include "UnitFuncs.h"
#include "MacroWord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportVedomWord *FormReportVedomWord;

extern EDRegOpts opts;

//---------------------------------------------------------------------------
__fastcall TFormReportVedomWord::TFormReportVedomWord(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedomWord::SetIDGroup(int _IDGroup)
{
  idgroup=_IDGroup;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportVedomWord::GetIDGroup(void)
{
  return idgroup;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedomWord::InitReportQuery(void)
{
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("select secondname,firstname,thirdname,specid,znum,id from "+opts.DBStudTable+" where deleted=0 and grpid="+ToStr(AnsiString(idgroup))+" order by specid,secondname,firstname,thirdname");
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedomWord::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedomWord::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedomWord::CreateWordDocument(void)
{
     int numspec;

    InitReportQuery();

    WordMacros macros;
    macros.BeginMacros();

    if (ZMySqlQuery->RecordCount==0)
        return;

    AnsiString GroupStr;
    GroupStr=WCGetTitleForKeyNum(GROUPS,idgroup);

    numspec=-1;
    int CountTables=0;
    int CountRows;
    int CountPrevRows=0;
    int CountPrevPrevRows=0;
    AnsiString Str,Strtmp;
    bool isFirst=true;
    for (int i=0;i<ZMySqlQuery->RecordCount;i++)
    {
        ZMySqlQuery->RecNo=i+1;
        if ( numspec != ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt() )
        {
            macros.SelectionTypeParagraph();
            macros.SelectionTypeParagraph();
            macros.InsertLine("Range.Select");
            macros.InsertLine("Selection.MoveUp Unit := wdLine, Count:=2");
            
            numspec=ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt();

            //Заголовок
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.InsertLine("Selection.Font.Size=12");
            if (!isFirst)
                macros.InsertLine("Selection.InsertBreak Type:= wdPageBreak");
            isFirst=false;

            macros.SelectionFont("Bold=false");
            macros.SelectionText("Нижегородский государственный архитектурно-строительный университет");
            macros.SelectionTypeParagraph();
            macros.SelectionText("ЭКЗАМЕНАЦИОННАЯ ЗАЧЕТНАЯ ВЕДОМОСТЬ");
            macros.SelectionTypeParagraph();
            macros.SelectionFont("Size=12");
            macros.SelectionFont("Bold=false");
            macros.SelectionText("Институт экономики, управления и права");
            macros.SelectionTypeParagraph();
            macros.SelectionTypeParagraph();
            macros.SelectionFont("Size=10");
            macros.SelectionFont("Bold=false");
            macros.SelectionText("Специальность ");
            macros.SelectionFont("Bold=true");
            macros.SelectionFont("Underline=wdUnderlineSingle");
            macros.SelectionText("  "+WCGetTitleForKeyNum(SPECS,numspec)+"  ");
            macros.SelectionTypeParagraph();
            macros.SelectionFont("Underline=wdUnderlineNone");
            macros.SelectionFont("Bold=false");
            macros.SelectionText("Группа ");
            macros.SelectionFont("Underline=wdUnderlineSingle");
            macros.SelectionFont("Bold=true");
            macros.SelectionText("___"+GroupStr+"__");
            macros.SelectionFont("Underline=wdUnderlineNone");
            macros.SelectionFont("Bold=false");
            macros.SelectionText(" Уровень __________ Курс _______ Семестр _______ Полусеместр _______");
            macros.SelectionTypeParagraph();
            macros.SelectionText("Дисциплина _____________________________________________________________________");
            macros.SelectionTypeParagraph();
            macros.SelectionText("Преподаватель ___________________________________________ Дата ________________");
            macros.SelectionTypeParagraph();

            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");

            macros.SelectionTypeParagraph();
            CountRows=0;
            for (int j=i;j<ZMySqlQuery->RecordCount;j++)
            {
                ZMySqlQuery->RecNo=j+1;
                if (numspec!=ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt() )
                    break;
                CountRows++;
            }
            ZMySqlQuery->RecNo=i+1;
            macros.TablesAdd(CountRows+1, 8);
            CountTables++;

            macros.TablesColumns(CountTables, 1, "Width = 30");
            macros.TablesColumns(CountTables, 2, "Width = 90");
            macros.TablesColumns(CountTables, 3, "Width = 70");
            macros.TablesColumns(CountTables, 4, "Width = 90");
            macros.TablesColumns(CountTables, 5, "Width = 50");
            macros.TablesColumns(CountTables, 6, "Width = 30");
            macros.TablesColumns(CountTables, 7, "Width = 40");
            macros.TablesColumns(CountTables, 8, "Width = 40");

            macros.TablesColumns(CountTables, 1, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphRight");
            macros.TablesColumns(CountTables, 2, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 3, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 4, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 5, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesColumns(CountTables, 6, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesColumns(CountTables, 7, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesColumns(CountTables, 8, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.InsertLine("ActiveDocument.Tables.Item(" + IntToStr(CountTables) + ").Range.Font.Size = 10");

            macros.InsertLine("ActiveDocument.Tables.Item(" + IntToStr(CountTables) + ").Rows.Item(1).Range.Font.Bold = true");
//          WordDocument->Tables->Item(CountTables)->Range->Select();

            macros.TablesCell(CountTables, 1, 1, "Range.Text=\"№п/п\"");
            macros.TablesCell(CountTables, 1, 1, "VerticalAlignment=wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 1, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 2, "Range.Text=\"Фамилия\"");
            macros.TablesCell(CountTables, 1, 2, "VerticalAlignment=wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 2, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = dAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 3, "Range.Text=\"Имя\"");
            macros.TablesCell(CountTables, 1, 3, "VerticalAlignment=wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 3, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 4, "Range.Text=\"Отчество\"");
            macros.TablesCell(CountTables, 1, 4, "VerticalAlignment=wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 4, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 5, "Range.Text=\"№ зач.кн.\"");
            macros.TablesCell(CountTables, 1, 5, "VerticalAlignment=wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 5, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 6, "Range.Text=\"Балл\"");
            macros.TablesCell(CountTables, 1, 6, "VerticalAlignment = wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 6, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 7, "Range.Text=\"Оценка\"");
            macros.TablesCell(CountTables, 1, 7, "VerticalAlignment = wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 7, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCente");
            macros.TablesCell(CountTables, 1, 8, "Range.Text = \"Подпись\"");
            macros.TablesCell(CountTables, 1, 8, "VerticalAlignment = wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 8, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");


            CountPrevPrevRows = CountPrevRows;
            CountPrevRows += CountRows;
        }

        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 1, "Range.Text = \"" + AnsiString(i+1-CountPrevPrevRows) + "\"");
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 2, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(1)->AsString + "\"");
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 3, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(2)->AsString + "\"");
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 4, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(3)->AsString + "\"");

        Strtmp=ZMySqlQuery->Fields->FieldByNumber(5)->AsString;
        int CountZero=6-Strtmp.Length();
        for (int it=0;it<CountZero;it++)
            Strtmp="0"+Strtmp;
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 5, "Range.Text = \"" + Strtmp + "\"");

        if (i+1 == CountPrevRows)
        {
            macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 1, "Range.Select");
            macros.InsertLine("Selection.MoveDown Unit := wdLine");

            macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 1, "Range.Select");
            macros.InsertLine("Selection.MoveDown Unit := wdLine");

            macros.SelectionTypeParagraph();
            macros.SelectionFont("Bold=true");
            macros.SelectionText("                                                                                                    Итого:");
            macros.SelectionFont("Bold=false");
            macros.SelectionText("                         \"\"отлично\"\" _______ чел.");
            macros.SelectionTypeParagraph();
            macros.SelectionFont("Bold=true");
            macros.SelectionText("Директор института _________________________");
            macros.SelectionFont("Bold=false");
            macros.SelectionText("                                                  \"\"хорошо\"\" _______ чел.");
            macros.SelectionTypeParagraph();
            macros.SelectionText("                                                                                                                      \"\"удовлетворительно\"\" _______ чел.");
            macros.SelectionTypeParagraph();
            macros.SelectionFont("Bold=true");
            macros.SelectionText("\"\"_____\"\"________________________ 200__г.");
            macros.SelectionFont("Bold=false");
            macros.SelectionText("                                     \"\"не удовлетворительно\"\" _______ чел.");
            macros.SelectionTypeParagraph();
            macros.SelectionText("                                                                                                                              \"\"не аттестовано\"\" _______ чел.");
            macros.SelectionTypeParagraph();
        }
    }

    macros.EndMacros();
    macros.RunMacros();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedomWord::ToolBtnPreviewClick(
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
void __fastcall TFormReportVedomWord::FormCreate(TObject *Sender)
{

  bool NoError;
  try
  {
    ZMySqlDatabase->HostName = opts.DBHost;
    ZMySqlDatabase->User = opts.DBLogin;
    ZMySqlDatabase->Password = opts.DBPasswd;
    ZMySqlDatabase->Database = opts.DBName;
    ZMySqlDatabase->Connected = true;

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
void __fastcall TFormReportVedomWord::FormDestroy(TObject *Sender)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------

