//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnitInfoOcenk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

const CountOcenk=6;
AnsiString arrOcenk[CountOcenk]={ "отлично",
                                  "хорошо",
                                  "удовлетворительно",
                                  "неудовлетворительно",
                                  "зачтено",
                                  "незачтено"};
AnsiString arrShortOcenk[CountOcenk]={ "отл","хор","удовл","неуд","зач","незач"};
const CountBall=6;
AnsiString arrBall[CountBall]={ "0,0","1,0","2,0","3,0","4,0","5,0"};

//---------------------------------------------------------------------------
void OCUpdateOcenk(TComboBox* cmb)
{
  cmb->Clear();
  for (int i=0;i<CountOcenk;i++) cmb->Items->Add(arrOcenk[i]);
}
void OCUpdateBalls(TComboBox* cmb)
{
  cmb->Clear();
  for (int i=0;i<CountBall;i++) cmb->Items->Add(arrBall[i]);
}
