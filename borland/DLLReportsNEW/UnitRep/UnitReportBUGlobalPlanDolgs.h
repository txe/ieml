//---------------------------------------------------------------------------

#ifndef UnitReportBUGlobalPlanDolgsH
#define UnitReportBUGlobalPlanDolgsH
//---------------------------------------------------------------------------
#include "UnitSelectCurrDate.h"
#include "UnitFuncs.h"
#include "ZAbstractDataset.hpp"
#include "ZAbstractRODataset.hpp"
#include "ZConnection.hpp"
#include "ZDataset.hpp"
#include <Classes.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <ImgList.hpp>
#include "ZSqlUpdate.hpp"
#include <vector>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TFormReportBUGlobalPlanDolgs : public TForm
{
__published:	// IDE-managed Components
        TImageList *ImageList;
    TZConnection *ZMySqlDatabase;
    TZQuery *ZMySqlQuery;
    TZQuery *ZMySqlQuerySumm;
    TZQuery *ZMySqlQueryCountStuds;
    TZQuery *ZMySqlQuerySumOplata;
    TZQuery *ZMySqlQueryPrevDolg;
        void __fastcall InitReportQuery(void);
        int __fastcall GetSummPlanCity(AnsiString CityIDStr,int NumSemestr,AnsiString NumYearStr);
        int __fastcall GetSummOplats1(AnsiString CityIDStr,AnsiString NumYearStr);
        int __fastcall GetSummOplats2(AnsiString CityIDStr,AnsiString NumYearStr);
        int __fastcall GetDolgPrevYearOfCity(AnsiString CityIDStr, AnsiString NumYearStr);
        void __fastcall InsertNewDolgPrevYearOfCity(AnsiString CityIDStr, AnsiString NumYearStr, int sumdolg);
        void __fastcall CreateWordDocument(void);
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall CalcPlanYearForCity(AnsiString CityIDStr, AnsiString NumYearStr, vecPlans* vec);
        void __fastcall CalcFactYearForGroup(AnsiString GrpIDStr, TPlan* plan, vecFacts* vec);
private:	// User declarations
        TFormSelectDate* pFormSelDate;
public:		// User declarations
        __fastcall TFormReportBUGlobalPlanDolgs(TComponent* Owner);
        AnsiString __fastcall GetPeriodStr(TDateTime dt);
        AnsiString __fastcall GetStartYear(TDateTime dt);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportBUGlobalPlanDolgs *FormReportBUGlobalPlanDolgs;
//---------------------------------------------------------------------------
#endif
