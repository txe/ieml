//---------------------------------------------------------------------------

#ifndef UnitFormDLLH
#define UnitFormDLLH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
#include <Grids.hpp>

//---------------------------------------------------------------------------
class TFormDLL : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBoxOplata;
        TLabel *LabDateOplata;
        TDateTimePicker *DateTimePickerOplata;
        TSpeedButton *BtnOplataFact;
        TComboBox *ComboBoxMoney;
        TLabel *LabMoneyOplata;
        TGroupBox *GroupBoxOpts;
        TSpeedButton *BtnOptsOplata;
        TLabel *LabOpts;
        TLabel *LabGroup;
        TLabel *LabDates;
        TLabel *LabMoney;
        TLabel *LabGroupVal;
        TLabel *LabDatesVal;
        TLabel *LabMoneyVal;
        TGroupBox *GroupBoxPrevOplata;
        TListBox *ListBoxPrevOplata;
        TLabel *LabDolg;
        TEdit *EditDolg;
        TSpeedButton *BtnDel;
        TLabel *LabSumMoney;
        TEdit *EditSumMoney;
        TLabel *Label1;
        TLabel *Label2;
        TSpeedButton *BtnAllFacts;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BtnOplataFactClick(TObject *Sender);
        void __fastcall LoadData(char* IniFileName,int _idstud);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall BtnOptsOplataClick(TObject *Sender);
        AnsiString __fastcall GetFIOForID(int _idstud);
        void __fastcall InitPrevOplata(void);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall BtnDelClick(TObject *Sender);
        void __fastcall ListBoxPrevOplataClick(TObject *Sender);
        void __fastcall BtnAllFactsClick(TObject *Sender);
private:	// User declarations
        int CurIDOpts;
        int idstud;
        int CommonMoneyForPeriod;
        TStringList* listIDFacts;
        int filter_checker;  // переключатель с отоб-я грыппы на студента и наоборот
public:

public:		// User declarations
        __fastcall TFormDLL(TComponent* Owner);
        void __fastcall SetFilterCheck(const int& flag);
        int __fastcall GetFilterCheck();
        int __fastcall GetIDCurStud(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDLL *FormDLL;
//---------------------------------------------------------------------------
#endif
