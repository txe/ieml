//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSelectSemestr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSelectSemestr *FormSelectSemestr;
//---------------------------------------------------------------------------
__fastcall TFormSelectSemestr::TFormSelectSemestr(TComponent* Owner)
        : TForm(Owner)
{
  semestr_gr=-1;
}
//---------------------------------------------------------------------------
int __fastcall TFormSelectSemestr::GetSemestrPlan(void)
{
  return semestr_pl;
}
//---------------------------------------------------------------------------
int __fastcall TFormSelectSemestr::GetSemestrGraphic(void)
{
  return semestr_gr;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelectSemestr::FormKeyPress(TObject *Sender, char &Key)
{
  if (Key==VK_RETURN)
    Close();
  if (Key==VK_ESCAPE)
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSelectSemestr::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  semestr_gr=UpDownGraphic->Position;
  Action=caFree;
}
//---------------------------------------------------------------------------


