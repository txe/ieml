//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitDataForDiplom.h"
#include "UnitFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

extern EDRegOpts opts;

TFormDataForDiplom *FormDataForDiplom;
//---------------------------------------------------------------------------
__fastcall TFormDataForDiplom::TFormDataForDiplom(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormDataForDiplom::BtnOKClick(TObject *Sender)
{
   opts.DateDiplom = DateTimePickerDateVidachi->Date.DateString();
   opts.DateQualif = DateTimePickerDatePrisQualif->Date.DateString();
 
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormDataForDiplom::MaskEditRegNumEnter(TObject *Sender)
{
   MaskEditRegNum->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormDataForDiplom::FormCreate(TObject *Sender)
{
   DateTimePickerDateVidachi->Date = StrToDate(opts.DateDiplom);
   DateTimePickerDatePrisQualif->Date = StrToDate(opts.DateQualif);
}
//---------------------------------------------------------------------------

