//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportDocDopuskVstupIspit.h"
#include "UnitFuncs.h"
#include "MacroWord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportDocDopuskVstupIspit *FormReportDocDopuskVstupIspit;

extern EDRegOpts opts;
//---------------------------------------------------------------------------
__fastcall TFormReportDocDopuskVstupIspit::TFormReportDocDopuskVstupIspit(TComponent* Owner): TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocDopuskVstupIspit::InitReportQuery(void)
{
  ZMySqlQuery->SQL->Clear();
  AnsiString WhereStr=pFormList->GetWHEREStr();
  ZMySqlQuery->SQL->Add("select secondname,firstname,thirdname,specid,eduformid from "+opts.DBStudTable+" where deleted=0 "+WhereStr+" order by specid,eduformid,secondname");
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocDopuskVstupIspit::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();      
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocDopuskVstupIspit::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocDopuskVstupIspit::CreateWordDocument(void)
{
    int numspec,numform;

    InitReportQuery();
    WordMacros macros;
    macros.BeginMacros();

    macros.InsertLine("Selection.SetRange Start:=0, End:=0");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphJustify");
    macros.SelectionFont("Bold = true");
    macros.SelectionText("�������: ");
    macros.SelectionFont("Bold = false");
    macros.SelectionText("� ������� ������������ � ������������� ���������� �� �����, � ������� ��������� �������� �� ���������� ������");
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold = true");
    macros.SelectionText("�����������: ");
    macros.SelectionFont("Bold = false");
    macros.SelectionText("��������� � ������������� ���������� ������������ �� �����, � ������� ��������� �������� �� ���������� ������");
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionFont("Bold = true");
    macros.SelectionText("� �������� ���������, ���������� � �����");
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold = false");

    if (ZMySqlQuery->RecordCount==0) return;

    numspec=-1;
    numform=-1;
    int CountTables=0;
    int CountRows;
    int CountPrevRows=0;
    int CountPrevPrevRows=0;
    AnsiString Str,Strtmp;
    for (int i = 0; i < ZMySqlQuery->RecordCount; i++)
    {
        ZMySqlQuery->RecNo=i+1;
        if ( numspec!=ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt() ||
            numform!=ZMySqlQuery->Fields->FieldByNumber(5)->AsString.ToInt() )
        {
            numspec = ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt();
            numform = ZMySqlQuery->Fields->FieldByNumber(5)->AsString.ToInt();

            macros.SelectionTypeParagraph();
            macros.SelectionTypeParagraph();
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.SelectionText("�� 1 ���� �� ������������� \"");
            macros.SelectionFont("Bold = true");
            macros.SelectionText(WCGetTitleForKeyNum(SPECS,numspec));
            macros.SelectionText("\"");
            macros.SelectionFont("Bold = false");
            macros.SelectionText(", � ������ �������� - ");
            macros.SelectionText(WCGetTitleForKeyNum(EDUFORMS,numform));
            macros.SelectionTypeParagraph();
            macros.SelectionTypeParagraph();

            CountRows=0;
            for (int j = i; j < ZMySqlQuery->RecordCount; j++)
            {
                ZMySqlQuery->RecNo = j + 1;
                if (numspec != ZMySqlQuery->Fields->FieldByNumber(4)->AsString.ToInt() ||
                    numform != ZMySqlQuery->Fields->FieldByNumber(5)->AsString.ToInt()) break;
                CountRows++;
            }
            ZMySqlQuery->RecNo=i+1;
            macros.TablesAdd(CountRows, 4);
            macros.TableStyle(WordMacros::StyleNone);
            CountTables++;

            macros.TablesColumns(CountTables, 1, "Width = 30");
            macros.TablesColumns(CountTables, 2, "Width = 140");
            macros.TablesColumns(CountTables, 3, "Width = 100");
            macros.TablesColumns(CountTables, 4, "Width = 150");

            macros.TablesColumns(CountTables, 1, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphRight");
            macros.TablesColumns(CountTables, 2, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 3, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 4, "Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
            macros.InsertLine("ActiveDocument.Tables.Item(" + IntToStr(CountTables) + ").Range.Font.Size = 10");

            CountPrevPrevRows=CountPrevRows;
            CountPrevRows+=CountRows;
        }

        macros.TablesCell(CountTables, i+1-CountPrevPrevRows, 1, "Range.Text = \"" + AnsiString(i+1-CountPrevPrevRows) + "\"");
        macros.TablesCell(CountTables, i+1-CountPrevPrevRows, 2, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(1)->AsString + "\"");
        macros.TablesCell(CountTables, i+1-CountPrevPrevRows, 3, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(2)->AsString + "\"");
        macros.TablesCell(CountTables, i+1-CountPrevPrevRows, 4, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(3)->AsString + "\"");

        if (i+1 == CountPrevRows)
        {
            macros.TablesCell(CountTables, CountRows, 1, "Range.Select");
            macros.InsertLine("Selection.MoveDown Unit := wdLine");
        }
    }

    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.SelectionText("������������ �������� ��������");
    macros.SelectionTypeParagraph();
    macros.SelectionText("������, ��������                                                                       �.�. �������");
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionText("������������� ���������");
    macros.SelectionTypeParagraph();
    macros.SelectionText("�������� ��������, ������                                                    �.�. ������");


    macros.EndMacros();
    macros.RunMacros();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocDopuskVstupIspit::ToolBtnPreviewClick(
      TObject *Sender)
{
  try
  {
    pFormList->ShowModal();
    CreateWordDocument();
  }
  catch(...)
  {
    Application->MessageBox("������ � MS Word �� ��������. �������� ������ ����������.","������.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocDopuskVstupIspit::FormCreate(TObject *Sender)
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
    AnsiString msg="��������� ��������� � ������� "+opts.DBHost+" ��� � ���� ������ "+opts.DBName+".\n���������� � ���������� ��������������.";
    MessageBox(Handle,msg.c_str(),"������",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDocDopuskVstupIspit::FormDestroy(
      TObject *Sender)
{
  pFormList->Free();
  WCDisconnect();
}
//---------------------------------------------------------------------------

