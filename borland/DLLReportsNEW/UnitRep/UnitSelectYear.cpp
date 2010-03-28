//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSelectYear.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSelectYear *FormSelectYear;
//---------------------------------------------------------------------------
__fastcall TFormSelectYear::TFormSelectYear(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSelectYear::ComboBoxFirstYearChange(TObject *Sender)
{
    EditSecondYear->Text = IntToStr(ComboBoxFirstYear->Text.ToInt() + 1);
}
//---------------------------------------------------------------------------
