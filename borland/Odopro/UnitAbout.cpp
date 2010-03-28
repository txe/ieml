//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitAbout.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma resource "*.dfm"
TFormAbout *FormAbout;
//---------------------------------------------------------------------------
__fastcall TFormAbout::TFormAbout(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormAbout::SpeedButton1Click(TObject *Sender)
{
  Close();      
}
//---------------------------------------------------------------------------
void __fastcall TFormAbout::FormKeyPress(TObject *Sender, char &Key)
{
  if (Key==VK_ESCAPE || Key==VK_RETURN) Close();
}
//---------------------------------------------------------------------------


