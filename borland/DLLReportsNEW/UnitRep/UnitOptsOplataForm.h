//---------------------------------------------------------------------------

#ifndef UnitOptsOplataFormH
#define UnitOptsOplataFormH
#define ERROR_ID -1
//---------------------------------------------------------------------------
#include "UnitFuncs.h"
#include "ZAbstractDataset.hpp"
#include "ZAbstractRODataset.hpp"
#include "ZConnection.hpp"
#include "ZDataset.hpp"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TFormOptsOplata : public TForm
{
__published:	// IDE-managed Components
        TStatusBar *StatusBar;
        TSpeedButton *BtnSelectClose;
    
        TGroupBox *GroupBox1;
        TStringGrid *StrGridOpts;
        TSpeedButton *BtnAdd;
        TGroupBox *GroupBox2;
        TStringGrid *StringGridRealOpts;
    TZConnection *ZMySqlDatabase;
    TZQuery *ZMySqlQuery;
        void __fastcall UpdateDatesInStatusBar(int selrow);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall UpdateTable(TStringGrid* pTab);
        void __fastcall ClearTable(TStringGrid* pTab);
        void __fastcall StrGridOptsClick(TObject *Sender);
        void __fastcall BtnSelectCloseClick(TObject *Sender);
        void __fastcall StrGridOptsDblClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BtnAddClick(TObject *Sender);
private:	// User declarations
        int ResultIDOpts;
        AnsiString GroupStr,
                   DatesStr,
                   CountMoney;
        int IDGroup;
public:		// User declarations
        __fastcall TFormOptsOplata(TComponent* Owner);
        int __fastcall GetResultIDOpts(void);
        void __fastcall GetSelectedFieldsOfOpts(AnsiString& _groupstr,AnsiString& _datesstr,AnsiString& _countmoney);
        void __fastcall SetIDGroup(int _id);
        int __fastcall GetIDGroup(void);
        void __fastcall GetListOfOptsID(TStringList* list);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOptsOplata *FormOptsOplata;
//---------------------------------------------------------------------------
#endif
