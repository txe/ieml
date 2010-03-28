//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitCommFilter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormFilterComm *FormFilterComm;
//---------------------------------------------------------------------------
__fastcall TFormFilterComm::TFormFilterComm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormFilterComm::BtnFindStrClick(TObject *Sender)
{
  char Key=VK_RETURN;
  FormKeyPress(Sender,Key);        
}
//---------------------------------------------------------------------------
void __fastcall TFormFilterComm::FormKeyPress(TObject *Sender, char &Key)
{
  int MAX_COUNT_ITEMS=10;
  TComboBox* cmb;
 if (!RadioButtonData->Checked)
 {
  if (RadioButtonStr->Checked) cmb=ComboBoxSubStr;
  if (RadioButtonDig->Checked) cmb=ComboBoxDig;
  if (Key==VK_RETURN)
  {
    if (cmb->Items->Count==MAX_COUNT_ITEMS) cmb->Items->Delete(cmb->Items->Count-1);
    cmb->Items->Insert(0,cmb->Text);

    ModalResult=7;  // Cancel
  }
  else
    if (Key==VK_ESCAPE) ModalResult=6;  // to find
 }
 else
 {
  if (Key==VK_RETURN) ModalResult=7;
  if (Key==VK_ESCAPE) ModalResult=6;  
 }
}
//---------------------------------------------------------------------------
void __fastcall TFormFilterComm::FormShow(TObject *Sender)
{
  RadioButtonStr->Checked=true;
  RadioButtonStrClick(RadioButtonStr);
  ComboBoxSubStr->SetFocus();
  ComboBoxSubStr->SelectAll();            
}
//---------------------------------------------------------------------------
void __fastcall TFormFilterComm::RadioButtonStrClick(TObject *Sender)
{
  if (RadioButtonStr->Checked)
  {
    GroupBoxStr->Enabled=true;
/*    GroupBoxStr->Color=clSilver;
    RadioButtonStr->Color=clSilver;*/
  }
  else
  {
    GroupBoxStr->Enabled=false;
/*    GroupBoxStr->Color=clBtnFace;
    RadioButtonStr->Color=clBtnFace;*/
  }
  if (RadioButtonData->Checked)
  {
    GroupBoxData->Enabled=true;
/*    GroupBoxData->Color=clSilver;
    RadioButtonData->Color=clSilver;*/
  }
  else
  {
    GroupBoxData->Enabled=false;
/*    GroupBoxData->Color=clBtnFace;
    RadioButtonData->Color=clBtnFace;*/
  }
  if (RadioButtonDig->Checked)
  {
    GroupBoxDig->Enabled=true;
/*    GroupBoxDig->Color=clSilver;
    RadioButtonDig->Color=clSilver;*/
  }
  else
  {
    GroupBoxDig->Enabled=false;
/*    GroupBoxDig->Color=clBtnFace;
    RadioButtonDig->Color=clBtnFace;*/
  }
}
//---------------------------------------------------------------------------

