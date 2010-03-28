//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportDolgsForGroup.h"
#include "UnitFuncs.h"
#include "MacroWord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportDolgsForGroup *FormReportDolgsForGroup;

extern EDRegOpts opts;
//---------------------------------------------------------------------------
__fastcall TFormReportDolgsForGroup::TFormReportDolgsForGroup(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDolgsForGroup::SetIDGroup(int _IDGroup)
{
    idgroup=_IDGroup;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportDolgsForGroup::GetIDGroup(void)
{
    return idgroup;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDolgsForGroup::InitReportQuery(void)
{
    ZMySqlQuery->SQL->Clear();
    ZMySqlQuery->SQL->Add("select CONCAT(secondname,\' \',firstname,\' \',thirdname) as fullname,specid,dogshifrid,dognum,dogyearid,dogfastid,id from "+opts.DBStudTable+" where deleted=0 and grpid="+ToStr(AnsiString(idgroup))+" order by specid,secondname");
    ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDolgsForGroup::InitOplataQuery(AnsiString IDStudStr)
{
    ZMySqlQueryOplata->SQL->Clear();
    AnsiString query="select moneypay,datepay from "+opts.DBPayFacts+" where idstud="+ToStr(IDStudStr)+" and (idopts="+ToStr(AnsiString(FormOpts->GetResultIDOpts()));
    for (int i=1; i<listIDs->Count; i++)
        query+=" or idopts="+ToStr(listIDs->Strings[i]);
    query+=") and deleted=0 order by datepay";
//  MessageBox(Handle,query.c_str(),"",MB_OK);
    ZMySqlQueryOplata->SQL->Add(query);
    ZMySqlQueryOplata->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDolgsForGroup::ToolBtnCloseClick(
      TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDolgsForGroup::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDolgsForGroup::CreateWordDocument(void)
{
    int numspec;

    InitReportQuery();

    if (ZMySqlQuery->RecordCount==0)
        return;

    AnsiString GroupStr,DatesStr,CountMoneyStr;
    FormOpts->GetSelectedFieldsOfOpts(GroupStr,DatesStr,CountMoneyStr);

    WordMacros macros;
    macros.BeginMacros();

    macros.PageSetup("");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionFont("Size=20");
    macros.SelectionFont("Bold=true");
    macros.SelectionText("Отчет по оплате");
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Size=12");
    macros.SelectionFont("Bold=false");
    TDateTime CurDate=Date();
    macros.SelectionText("на "+CurDate.DateString());
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
    macros.SelectionText("Отчетный период: ");
    macros.SelectionFont("Bold=true");
    macros.SelectionText(DatesStr);
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold=false");
    macros.SelectionText("Группа: ");
    macros.SelectionFont("Bold=true");
    macros.SelectionText(GroupStr);
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold=false");
    macros.SelectionText("Оплата за отчетный период составляет: ");
    macros.SelectionFont("Bold=true");
    macros.SelectionText(CountMoneyStr);
    macros.SelectionFont("Bold=false");
    macros.SelectionText(" рублей.");

    macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");

    numspec=-1;
    int CountTables=0;
    int CountRows;
    int CountPrevRows=0;
    int CountPrevPrevRows=0;
    AnsiString Str,Strtmp;
    for (int i=0;i<ZMySqlQuery->RecordCount;i++)
    {
        ZMySqlQuery->RecNo=i+1;
        if ( numspec!=ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt() )
        {
            numspec=ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt();

            macros.SelectionTypeParagraph();
            macros.SelectionTypeParagraph();
            macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
            macros.SelectionText("Cпециальность \"");
            macros.SelectionFont("Bold=true");
            AnsiString txt = WCGetTitleForKeyNum(SPECS, numspec);
            macros.FilterText(txt);
            macros.SelectionText(txt);
            macros.SelectionText("\"");
            macros.SelectionFont("Bold=false");
            macros.SelectionTypeParagraph();
            macros.SelectionTypeParagraph();

            CountRows=0;
            for (int j=i;j<ZMySqlQuery->RecordCount;j++)
            {
                ZMySqlQuery->RecNo=j+1;
                if (numspec!=ZMySqlQuery->Fields->FieldByNumber(2)->AsString.ToInt() )
                    break;
                CountRows++;
            }
            ZMySqlQuery->RecNo=i+1;
            macros.TablesAdd(CountRows+1,6);
            CountTables++;

            macros.TablesColumns(CountTables, 1, "Width=40");
            macros.TablesColumns(CountTables, 2, "Width=250");
            macros.TablesColumns(CountTables, 3, "Width=160");
            macros.TablesColumns(CountTables, 4, "Width=80");
            macros.TablesColumns(CountTables, 5, "Width=120");
            macros.TablesColumns(CountTables, 6, "Width=80");

            macros.TablesColumns(CountTables, 1, "Select");
            macros.SelectionParagraphFormat("Alignment=wdAlignParagraphRight");
            macros.TablesColumns(CountTables, 2, "Select");
            macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 3, "Select");
            macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
            macros.TablesColumns(CountTables, 4, "Select");
            macros.SelectionParagraphFormat("Alignment=wdAlignParagraphRight");
            macros.TablesColumns(CountTables, 5, "Select");
            macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
            macros.TablesColumns(CountTables, 6, "Select");
            macros.SelectionParagraphFormat("Alignment=wdAlignParagraphRight");
            macros.InsertLine("Selection.Tables.Item("+IntToStr(CountTables)+").Range.Font.Size=12");

            macros.InsertLine("Selection.Tables.Item("+IntToStr(CountTables)+").Rows.Item(1).Range.Font.Bold=true");
            macros.InsertLine("Selection.Tables.Item("+IntToStr(CountTables)+").Range.Select");

            macros.TablesCell(CountTables, 1, 1, "Range.Text = \"№\"");
            macros.TablesCell(CountTables, 1, 1, "VerticalAlignment = wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 1, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 2, "Range.Text = \"Фамилия, имя, отчество\"");
            macros.TablesCell(CountTables, 1, 2, "VerticalAlignment = wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 2, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 3, "Range.Text = \"Шифр договора\"");
            macros.TablesCell(CountTables, 1, 3, "VerticalAlignment = wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 3, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 4, "Range.Text = \"Сумма (руб.)\"");
            macros.TablesCell(CountTables, 1, 4, "VerticalAlignment = wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 4, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 5, "Range.Text = \"Дата платежа\"");
            macros.TablesCell(CountTables, 1, 5, "VerticalAlignment = wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 5, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
            macros.TablesCell(CountTables, 1, 6, "Range.Select");
            macros.InsertLine("Selection.TypeText Text:= \"Долг за текущий\"");
            macros.InsertLine("Selection.TypeParagraph");
            macros.InsertLine("Selection.TypeText Text:= \"уч. год (руб.)\"");
            macros.TablesCell(CountTables, 1, 6, "VerticalAlignment = wdCellAlignVerticalCenter");
            macros.TablesCell(CountTables, 1, 6, "Range.Select");
            macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");

            CountPrevPrevRows=CountPrevRows;
            CountPrevRows+=CountRows;
        }

        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 1, "VerticalAlignment = wdCellAlignVerticalCenter");
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 1, "Range.Text = \"" + AnsiString(i+1-CountPrevPrevRows) + "\"");
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 2, "VerticalAlignment = wdCellAlignVerticalCenter");
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 2, "Range.Text = \""+ZMySqlQuery->Fields->FieldByNumber(1)->AsString + "\"");
        Str=WCGetTitleForKeyNum(DOGSHIFRS,ZMySqlQuery->Fields->FieldByNumber(3)->AsString.ToInt())+"-";
        Str+=WCGetTitleForKeyNum(DOGYEARS,ZMySqlQuery->Fields->FieldByNumber(5)->AsString.ToInt());
        Strtmp=WCGetTitleForKeyNum(DOGFASTS,ZMySqlQuery->Fields->FieldByNumber(6)->AsString.ToInt());
        if (!Strtmp.IsEmpty())
            Str+="-"+Strtmp;
        Str+="-"+ZMySqlQuery->Fields->FieldByNumber(4)->AsString;
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 3, "VerticalAlignment = wdCellAlignVerticalCenter");
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 3, "Range.Text = \"" + Str + "\"");

        InitOplataQuery(ZMySqlQuery->Fields->FieldByNumber(7)->AsString);
        AnsiString Money="";
        AnsiString Dates="";
        int SumMoney=0;
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 4, "Range.Select");
        for (int i_op=0;i_op<ZMySqlQueryOplata->RecordCount;i_op++)
        {
            ZMySqlQueryOplata->RecNo=i_op+1;
            if (i_op!=0)
            {
                macros.InsertLine("Selection.Collapse Direction:=wdCollapseEnd");
                macros.InsertLine("Selection.TypeParagraph");
                macros.InsertLine("Selection.MoveLeft Unit:=wdCell");
                macros.InsertLine("Selection.Collapse Direction:=wdCollapseEnd");
                macros.InsertLine("Selection.TypeParagraph");
            }
            macros.InsertLine("Selection.TypeText Text:= \"" + ZMySqlQueryOplata->Fields->FieldByNumber(1)->AsString + "\"");
            macros.InsertLine("Selection.MoveRight Unit:=wdCell");
            macros.InsertLine("Selection.Collapse Direction:=wdCollapseEnd");
            macros.InsertLine("Selection.TypeText Text:= \"" + ZMySqlQueryOplata->Fields->FieldByNumber(2)->AsString + "\"");

            SumMoney += ZMySqlQueryOplata->Fields->FieldByNumber(1)->AsString.ToInt();
        }
        AnsiString DolgStr = AnsiString(CountMoneyStr.ToInt()-SumMoney);
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 6, "VerticalAlignment = wdCellAlignVerticalCenter");
        macros.TablesCell(CountTables, i+2-CountPrevPrevRows, 6, "Range.Text = \"" + DolgStr + "\"");

        if (i+1 == CountPrevRows)
        {
            macros.TablesCell(CountTables, CountRows+1, 1, "Range.Select");
            macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count := 3");
        }
    }
    macros.InsertLine("ActiveWindow.ActivePane.VerticalPercentScrolled = 0");
    macros.EndMacros();
    macros.RunMacros();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDolgsForGroup::ToolBtnPreviewClick(
      TObject *Sender)
{
  try
  {
    FormOpts->SetIDGroup(idgroup);
    FormOpts->Caption="Выбор категории оплаты по группе: "+WCGetTitleForKeyNum(GROUPS,idgroup);
    FormOpts->ShowModal();
    int idopts=FormOpts->GetResultIDOpts();
    if (idopts==ERROR_ID) return; // Не выбрана категория оплаты
    FormOpts->GetListOfOptsID(listIDs);
    
    CreateWordDocument();
  }
  catch(...)
  {
    Application->MessageBox("Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportDolgsForGroup::FormCreate(TObject *Sender)
{
  FormOpts=new TFormOptsOplata(Application);
  listIDs=new TStringList;


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
void __fastcall TFormReportDolgsForGroup::FormDestroy(TObject *Sender)
{
  delete listIDs;
  FormOpts->Free();
  WCDisconnect();      
}
//---------------------------------------------------------------------------

