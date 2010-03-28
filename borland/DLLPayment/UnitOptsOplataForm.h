//---------------------------------------------------------------------------

#ifndef UnitOptsOplataFormH
#define UnitOptsOplataFormH
#define ERROR_ID -1
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "UnitFuncs.h"
#include "mysql.h"
#include "UnitFormDLL.h"
#include <Db.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormOptsOplata : public TForm
{
__published:	// IDE-managed Components
        TLabel *LabGroup;
        TComboBox *ComboBoxGroup;
        TGroupBox *GroupBoxPeriod;
        TPanel *Panel1;
        TLabel *LabMounth1;
        TLabel *LabYear1;
        TComboBox *ComboBoxMounth1;
        TComboBox *ComboBoxYear1;
        TPanel *Panel2;
        TLabel *LabMounth2;
        TLabel *LabYear2;
        TComboBox *ComboBoxMounth2;
        TComboBox *ComboBoxYear2;
        TLabel *LabMines;
        TEdit *EditCommonCountMoney;
        TLabel *Label1;
        TStringGrid *StrGridOpts;
        TSpeedButton *BtnAddRecord;
        TSpeedButton *BtnDelRecord;
        TSpeedButton *BtnEditRecord;
        TStatusBar *StatusBar;
        TSpeedButton *BtnSelectClose;
    TRadioGroup *RadioGroupFilter;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall UpdateDate2(void);
        void __fastcall UpdateDatesInStatusBar(int selrow);
        void __fastcall ComboBoxMounth1Change(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall UpdateTable(TStringGrid* pTab);
        void __fastcall ClearTable(TStringGrid* pTab);
        void __fastcall ComboBoxGroupChange(TObject *Sender);
        void __fastcall StrGridOptsClick(TObject *Sender);
        void __fastcall BtnAddRecordClick(TObject *Sender);
        AnsiString __fastcall GetSQLDate(int numDate);
        void __fastcall BtnDelRecordClick(TObject *Sender);
        void __fastcall SetDate(int numdate,int idselected);
        void __fastcall BtnEditRecordClick(TObject *Sender);
        void __fastcall BtnSelectCloseClick(TObject *Sender);
        void __fastcall StrGridOptsDblClick(TObject *Sender);
        bool __fastcall IsExistsRecordOpts(int &id,AnsiString &group,AnsiString &date1,AnsiString &date2);
        AnsiString __fastcall GetGroupForStudID(int _idstud);
        void __fastcall SetMyFormDLL(TFormDLL* curFormDLL);
    void __fastcall RadioGroupFilterClick(TObject *Sender);
private:	// User declarations
        int ResultIDOpts;
        AnsiString GroupStr,
                   DatesStr,
                   CountMoney;
        TFormDLL* MyFormDLL;
public:		// User declarations
        __fastcall TFormOptsOplata(TComponent* Owner);
        int __fastcall GetResultIDOpts(void);
        void __fastcall GetSelectedFieldsOfOpts(AnsiString& _groupstr,AnsiString& _datesstr,AnsiString& _countmoney);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOptsOplata *FormOptsOplata;
//---------------------------------------------------------------------------
#endif
