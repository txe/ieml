//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitProgress.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormProgress *FormProgress;
//---------------------------------------------------------------------------
__fastcall TFormProgress::TFormProgress(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormProgress::FormShow(TObject *Sender)
{
  ProgressBar->Position=0;
}
//---------------------------------------------------------------------------
void __fastcall TFormProgress::FormHide(TObject *Sender)
{
  while (ProgressBar->Position<ProgressBar->Max)
  {
    ProgressBar->StepIt();
    ProgressBar->Refresh();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormProgress::Increment(int dPersent)
{
  int NextPos=ProgressBar->Position + dPersent;
  if (NextPos>ProgressBar->Max) NextPos=ProgressBar->Max;
  while (ProgressBar->Position<NextPos)
  {
    ProgressBar->StepIt();
    ProgressBar->Refresh();
  }
}
//---------------------------------------------------------------------------
int __fastcall TFormProgress::GetMax(void) { return ProgressBar->Max; }
//---------------------------------------------------------------------------
int __fastcall TFormProgress::GetPos(void) { return ProgressBar->Position; }
//---------------------------------------------------------------------------

