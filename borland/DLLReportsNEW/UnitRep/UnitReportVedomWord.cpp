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

            //���������
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.InsertLine("Selection.Font.Size=12");
            if (!isFirst)
                macros.InsertLine("Selection.InsertBreak Type:= wdPageBreak");
            isFirst=false;

            macros.SelectionFont("Bold=false");
            macros.SelectionText("������������� ��������������� ������������-������������ �����������");
            macros.SelectionTypeParagraph();
            macros.SelectionText("��������������� �������� ���������");
            macros.SelectionTypeParagraph();
            macros.SelectionFont("Size=12");
            macros.SelectionFont("Bold=false");
            macros.SelectionText("�������� ���������, ���������� � �����");
            macros.SelectionTypeParagraph();
            macros.SelectionTypeParagraph();
            macros.SelectionFont("Size=10");
            macros.SelectionFont("Bold=false");
            macros.SelectionText("������������� ");
            macros.SelectionFont("Bold=true");
            macros.SelectionFont("Underline=wdUnderlineSingle");
            macros.SelectionText("  "+WCGetTitleForKeyNum(SPECS,numspec)+"  ");
            macros.SelectionTypeParagraph();
            macros.SelectionFont("Underline=wdUnderlineNone");
            macros.SelectionFont("Bold=false");
            macros.SelectionText("������ ");
            macros.SelectionFont("Underline=wdUnderlineSingle");
            macros.SelectionFont("Bold=true");
            macros.SelectionText("___"+GroupStr+"__");
            macros.SelectionFont("Underline=wdUnderlineNone");
            macros.SelectionFont("Bold=false");
            macros.SelectionText(" ������� __________ ���� _______ ������� _______ ����������� _______");
            macros.SelectionTypeParagraph();
            macros.SelectionText("���������� _____________________________________________________________________");
            macros.SelectionTypeParagraph();
            macros.SelectionText("������������� ___________________________________________ ���� ________________");
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

            macros.TablesCell(CountTables, 1, 1, "Range.Text=\"��/�\"");
            macros.TablesCell(CountTables, 1, 1, "VerticalAlignment=wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 1, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 2, "Range.Text=\"�������\"");
            macros.TablesCell(CountTables, 1, 2, "VerticalAlignment=wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 2, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = dAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 3, "Range.Text=\"���\"");
            macros.TablesCell(CountTables, 1, 3, "VerticalAlignment=wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 3, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 4, "Range.Text=\"��������\"");
            macros.TablesCell(CountTables, 1, 4, "VerticalAlignment=wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 4, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 5, "Range.Text=\"� ���.��.\"");
            macros.TablesCell(CountTables, 1, 5, "VerticalAlignment=wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 5, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 6, "Range.Text=\"����\"");
            macros.TablesCell(CountTables, 1, 6, "VerticalAlignment = wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 6, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 7, "Range.Text=\"������\"");
            macros.TablesCell(CountTables, 1, 7, "VerticalAlignment = wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 7, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCente");
            macros.TablesCell(CountTables, 1, 8, "Range.Text = \"�������\"");
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
            macros.SelectionText("                                                                                                    �����:");
            macros.SelectionFont("Bold=false");
            macros.SelectionText("                         \"\"�������\"\" _______ ���.");
            macros.SelectionTypeParagraph();
            macros.SelectionFont("Bold=true");
            macros.SelectionText("�������� ��������� _________________________");
            macros.SelectionFont("Bold=false");
            macros.SelectionText("                                                  \"\"������\"\" _______ ���.");
            macros.SelectionTypeParagraph();
            macros.SelectionText("                                                                                                                      \"\"�����������������\"\" _______ ���.");
            macros.SelectionTypeParagraph();
            macros.SelectionFont("Bold=true");
            macros.SelectionText("\"\"_____\"\"________________________ 200__�.");
            macros.SelectionFont("Bold=false");
            macros.SelectionText("                                     \"\"�� �����������������\"\" _______ ���.");
            macros.SelectionTypeParagraph();
            macros.SelectionText("                                                                                                                              \"\"�� �����������\"\" _______ ���.");
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
    Application->MessageBox("������ � MS Word �� ��������. �������� ������ ����������.","������.",MB_OK|MB_ICONEXCLAMATION);
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
    AnsiString msg="��������� ��������� � ������� "+opts.DBHost+" ��� � ���� ������ "+opts.DBName+".\n���������� � ���������� ��������������.";
    MessageBox(Handle,msg.c_str(),"������",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportVedomWord::FormDestroy(TObject *Sender)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------

