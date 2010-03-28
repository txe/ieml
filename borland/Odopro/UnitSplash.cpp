//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSplash.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma resource "*.dfm"
TSplashForm *SplashForm;
//---------------------------------------------------------------------------
__fastcall TSplashForm::TSplashForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSplashForm::FormKeyPress(TObject *Sender, char &Key)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TSplashForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Timer->Enabled=false;
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TSplashForm::TimerTimer(TObject *Sender)
{
  Close();      
}
//---------------------------------------------------------------------------

