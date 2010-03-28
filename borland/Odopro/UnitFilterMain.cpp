//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitFilterMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormFilter *FormFilter;
//---------------------------------------------------------------------------
__fastcall TFormFilter::TFormFilter(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormFilter::FormShow(TObject *Sender)
{
  ComboBoxSubStr->SetFocus();
  ComboBoxSubStr->SelectAll();      
}
//---------------------------------------------------------------------------
void __fastcall TFormFilter::BtnFindClick(TObject *Sender)
{
  char Key=VK_RETURN;
  FormKeyPress(BtnFind,Key);
}
//---------------------------------------------------------------------------
void __fastcall TFormFilter::FormKeyPress(TObject *Sender, char &Key)
{
  int MAX_COUNT_ITEMS=10;
  TComboBox* cmb=ComboBoxSubStr;
  if (Key==VK_RETURN)
  {
    if (cmb->Items->Count==MAX_COUNT_ITEMS) cmb->Items->Delete(cmb->Items->Count-1);
    cmb->Items->Insert(0,cmb->Text);

    ModalResult=7;  // Cancel
  }
  else
    if (Key==VK_ESCAPE) ModalResult=6;  // to find
}
//---------------------------------------------------------------------------

