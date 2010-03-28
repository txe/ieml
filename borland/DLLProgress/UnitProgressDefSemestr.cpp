//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitProgressDefSemestr.h"
#include "UnitInfoOcenk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormProgressDefSemestr *FormProgressDefSemestr;

TStringList* listIDsOcenkDefSem;

extern EDRegOpts opts;
extern MYSQL *mysql;

extern AnsiString arrOcenk[];
//---------------------------------------------------------------------------
__fastcall TFormProgressDefSemestr::TFormProgressDefSemestr(TComponent* Owner)
        : TForm(Owner)
{
  listIDsOcenkDefSem=new TStringList;

  Constraints->MinWidth=Width;
  Constraints->MinHeight=Height;

  StrGridListOcenk->Cells[0][0]="№п/п";
  StrGridListOcenk->Cells[1][0]="Дисциплина";
  StrGridListOcenk->Cells[2][0]="Оценка";
  StrGridListOcenk->Cells[3][0]="Балл";
}
//---------------------------------------------------------------------------
void __fastcall TFormProgressDefSemestr::SetMyFormDLL(TFormDLLProgr* curFormDLL)
{
  MyFormDLL=curFormDLL;
}
//---------------------------------------------------------------------------
void __fastcall TFormProgressDefSemestr::FormResize(TObject *Sender)
{
  int size=StrGridListOcenk->ClientWidth-20,
      sizeindex=size*7/100,
      sizeDiscip=size*58/100,
      sizeEstim=size*25/100,
      sizeBall=size*10/100;
  StrGridListOcenk->ColWidths[0]=sizeindex;
  StrGridListOcenk->ColWidths[1]=sizeDiscip;
  StrGridListOcenk->ColWidths[2]=sizeEstim;
  StrGridListOcenk->ColWidths[3]=sizeBall;
}
//---------------------------------------------------------------------------
void __fastcall TFormProgressDefSemestr::RadioButtonPlanClick(
      TObject *Sender)
{
  if (Sender==RadioButtonPlan)
  {
    EditPlan->Enabled=true;
    UpDownPlan->Enabled=true;
    EditGraph->Enabled=false;
    UpDownGraph->Enabled=false;
  }
  else
  {
    EditPlan->Enabled=false;
    UpDownPlan->Enabled=false;
    EditGraph->Enabled=true;
    UpDownGraph->Enabled=true;
  }
  UpdateTable(StrGridListOcenk);
}
//---------------------------------------------------------------------------
void __fastcall TFormProgressDefSemestr::BtnCloseClick(TObject *Sender)
{
  Close();                      
}
//---------------------------------------------------------------------------
void __fastcall TFormProgressDefSemestr::FormCreate(TObject *Sender)
{
  UpdateTable(StrGridListOcenk);
}
//---------------------------------------------------------------------------
void __fastcall TFormProgressDefSemestr::SetIDStud(int _IDStud)
{
  idstud=_IDStud;
}
//---------------------------------------------------------------------------
void __fastcall TFormProgressDefSemestr::UpdateTable(TStringGrid* pTab)
{
  ClearTable(pTab);

  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString query;
  if (RadioButtonPlan->Checked)
        query="SELECT oc.id,disc.fulltitle,oc.estimation,oc.ball FROM "+opts.DBDisciplines+" as disc, "+opts.DBProgress+" as oc, "+opts.DBStudTable+" as stud WHERE oc.deleted=0 AND disc.deleted=0 AND stud.deleted=0 AND oc.idstud="+AnsiString(idstud)+" AND stud.id=oc.idstud AND disc.idspec=stud.specid AND oc.iddiscip=disc.id AND numplansemestr="+AnsiString(UpDownPlan->Position)+" ORDER BY fulltitle";
  else
        query="SELECT oc.id,disc.fulltitle,oc.estimation,oc.ball FROM "+opts.DBDisciplines+" as disc, "+opts.DBProgress+" as oc, "+opts.DBStudTable+" as stud WHERE oc.deleted=0 AND disc.deleted=0 AND stud.deleted=0 AND oc.idstud="+AnsiString(idstud)+" AND stud.id=oc.idstud AND disc.idspec=stud.specid AND oc.iddiscip=disc.id AND numgraphsemestr="+AnsiString(UpDownGraph->Position)+" ORDER BY fulltitle";

  mysql_query(mysql,query.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    int numCurRow;

    if (result && mysql_num_rows(result))
    {
      numCurRow=0;
      listIDsOcenkDefSem->Clear();
      listIDsOcenkDefSem->Add("NULL");

      while (row = mysql_fetch_row(result))
      {
        numCurRow++;
        pTab->RowCount=numCurRow+1;

        listIDsOcenkDefSem->Add(AnsiString(row[0]));
        pTab->Cells[0][numCurRow]=numCurRow;
        pTab->Cells[1][numCurRow]=AnsiString(row[1]);
        pTab->Cells[2][numCurRow]=arrOcenk[AnsiString(row[2]).ToInt()];
        pTab->Cells[3][numCurRow]=AnsiString(row[3]);
      }
    }
    mysql_free_result(result);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormProgressDefSemestr::ClearTable(TStringGrid* pTab)
{
  pTab->RowCount=2;
  for (int j=0;j<pTab->ColCount;j++) pTab->Cells[j][1]="";
}
//---------------------------------------------------------------------------
void __fastcall TFormProgressDefSemestr::FormDestroy(TObject *Sender)
{
  delete listIDsOcenkDefSem;
}
//---------------------------------------------------------------------------
void __fastcall TFormProgressDefSemestr::UpDownPlanClick(TObject *Sender,
      TUDBtnType Button)
{
  UpdateTable(StrGridListOcenk);
}
//---------------------------------------------------------------------------
void __fastcall TFormProgressDefSemestr::FormShow(TObject *Sender)
{
  RadioButtonPlanClick(RadioButtonPlan);
}
//---------------------------------------------------------------------------

