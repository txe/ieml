//---------------------------------------------------------------------------

#ifndef UnitSelectCurrDateH
#define UnitSelectCurrDateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormSelectDate : public TForm
{
__published:	// IDE-managed Components
        TMonthCalendar *MonthCalendar;
        TLabel *Label1;
        TSpeedButton *BtnReportGo;
        void __fastcall BtnReportGoClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormSelectDate(TComponent* Owner);
        TDateTime  __fastcall GetSelectedDate(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSelectDate *FormSelectDate;
//---------------------------------------------------------------------------
#endif
