//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSelDrob.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSelDrob *FormSelDrob;
//---------------------------------------------------------------------------
__fastcall TFormSelDrob::TFormSelDrob(TComponent* Owner)
        : TForm(Owner)
{
  ComboBox1->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelDrob::BitBtn1Click(TObject *Sender)
{
  Close();        
}
//---------------------------------------------------------------------------
