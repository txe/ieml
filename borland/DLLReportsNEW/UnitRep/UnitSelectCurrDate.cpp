//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitSelectCurrDate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSelectDate *FormSelectDate;
//---------------------------------------------------------------------------
__fastcall TFormSelectDate::TFormSelectDate(TComponent* Owner)
        : TForm(Owner)
{
  TDateTime dt;
  MonthCalendar->Date=dt.CurrentDate();
}
//---------------------------------------------------------------------------
TDateTime  __fastcall TFormSelectDate::GetSelectedDate(void)
{
  return MonthCalendar->Date;
}
//---------------------------------------------------------------------------
void __fastcall TFormSelectDate::BtnReportGoClick(TObject *Sender)
{
  Close();        
}
//---------------------------------------------------------------------------
