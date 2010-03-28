//---------------------------------------------------------------------------

#ifndef UnitReportBUPlanDolgssemestrH
#define UnitReportBUPlanDolgsSemestrH
//---------------------------------------------------------------------------
#include "UnitSelectCurrDate.h"
#include "UnitFuncs.h"
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
//---------------------------------------------------------------------------
class TFormReportBUPlanDolgsSemestr : public TForm
{
__published:	// IDE-managed Components
        
        TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
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
        void __fastcall InsertNewDolgPrevYearOfCity(AnsiString CityIDStr, AnsiString NumYearStr, int sumdolg);
        void __fastcall CreateWordDocument(void);
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall CalcPlanYearForCity(AnsiString CityIDStr, AnsiString StartDateSQl,AnsiString StartDateOfPrevSemSQL, vecPlans* vec);
        void __fastcall CalcFactYearForGroup(AnsiString GrpIDStr, TPlan* plan, vecFacts* vec, AnsiString StartDateSQL, AnsiString EndDateSQL);
private:	// User declarations
        TFormSelectDate* pFormSelDate;
public:		// User declarations
        __fastcall TFormReportBUPlanDolgsSemestr(TComponent* Owner);
        AnsiString __fastcall GetPeriodStr(TDateTime dt);
        AnsiString __fastcall GetStartYear(TDateTime dt);
        void __fastcall GetStartEndDateSQL(TDateTime dt,AnsiString& StartDate,AnsiString& EndDate);
        void __fastcall GetPrevSemStartDateSQL(TDateTime dt, AnsiString& StartDate);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportBUPlanDolgsSemestr *FormReportBUPlanDolgsSemestr;
//---------------------------------------------------------------------------
#endif
