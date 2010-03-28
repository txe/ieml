//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportSpravka.h"
#include "UnitFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormReportSpravka *FormReportSpravka;
//---------------------------------------------------------------------------
__fastcall TFormReportSpravka::TFormReportSpravka(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSpravka::SetIDStudent(int _IDStudent)
{
  idstudent=_IDStudent;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportSpravka::GetIDStudent(void)
{
  return idstudent;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSpravka::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSpravka::ToolBtnPreviewClick(TObject *Sender)
{
  QuickRep->Preview();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSpravka::ToolButtonCloseClick(TObject *Sender)
{
  Close();      
}
//---------------------------------------------------------------------------
void __fastcall TFormReportSpravka::FormCreate(TObject *Sender)
{
  QuickRep->Visible=false;
}
//---------------------------------------------------------------------------

