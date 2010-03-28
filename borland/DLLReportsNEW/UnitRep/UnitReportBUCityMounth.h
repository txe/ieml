//---------------------------------------------------------------------------

#ifndef UnitReportBUCityMounthH
#define UnitReportBUCityMounthH
//---------------------------------------------------------------------------
#include "UnitSelectCurrDate.h"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include "ZAbstractDataset.hpp"
#include "ZAbstractRODataset.hpp"
#include "ZConnection.hpp"
#include "ZDataset.hpp"
#include <DB.hpp>


//---------------------------------------------------------------------------
class TFormReportBUCityMounth : public TForm
{
__published:	// IDE-managed Components

        TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
    TZConnection *ZMySqlDatabase;
    TZQuery *ZMySqlQuerySumm;
    TZQuery *ZMySqlQuery;

        void __fastcall InitReportQuery(void);
        AnsiString __fastcall GetSummCity(AnsiString CityIDStr,AnsiString NumMounthStr,AnsiString NumYearStr);
        void __fastcall CreateWordDocument(void);
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        TFormSelectDate* pFormSelDate;
public:		// User declarations
        __fastcall TFormReportBUCityMounth(TComponent* Owner);
        AnsiString __fastcall GetPeriodStr(TDateTime dt);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportBUCityMounth *FormReportBUCityMounth;
//---------------------------------------------------------------------------
#endif
