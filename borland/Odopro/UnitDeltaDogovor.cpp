//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitDeltaDogovor.h"
#include "myregini.h" 
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDeltaDogovor *FormDeltaDogovor;
//---------------------------------------------------------------------------
__fastcall TFormDeltaDogovor::TFormDeltaDogovor(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormDeltaDogovor::FormShow(TObject *Sender)
{
    EDRegOpts opts;
    MaskEditVert->Text = opts.GetDeltaDogovor();
}
//---------------------------------------------------------------------------
void __fastcall TFormDeltaDogovor::Button1Click(TObject *Sender)
{
    EDRegOpts opts;
    opts.SetDeltaDogovor(MaskEditVert->Text.Trim());
}
//---------------------------------------------------------------------------
