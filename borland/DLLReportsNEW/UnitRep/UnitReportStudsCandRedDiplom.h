//---------------------------------------------------------------------------

#ifndef UnitReportStudsCandRedDiplomH
#define UnitReportStudsCandRedDiplomH
//---------------------------------------------------------------------------
#include "UnitListGroups.h"
#include "ZAbstractDataset.hpp"
#include "ZAbstractRODataset.hpp"
#include "ZConnection.hpp"
#include "ZDataset.hpp"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>

//---------------------------------------------------------------------------
class TFormReportStudsCandRedDiplom : public TForm
{
__published:	// IDE-managed Components

        TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
    TZConnection *ZMySqlDatabase;
    TZQuery *ZMySqlQueryStudsOfGrp;
    TZQuery *ZMySqlQuery;
    TZQuery *ZMySqlQueryAvrgBall;
    TZQuery *ZMySqlQueryCheckStuds;

        void __fastcall InitReportQuery(void);
        void __fastcall CreateWordDocument(void);
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall GetBallsCount(int& countUDOVL,double& percUDOVL,int& countHOR,double& percHOR,
                                      int& countOTL,double& percOTL,AnsiString& AvrgBallStr,AnsiString IDStudStr);
        int __fastcall InitStudentsForGroup(int idgroup);
        int __fastcall GetCountUsefullStudents(TStringList* listNumbers);
private:	// User declarations
        TFormListGroup *pFormList;
public:		// User declarations
        __fastcall TFormReportStudsCandRedDiplom(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportStudsCandRedDiplom *FormReportStudsCandRedDiplom;
//---------------------------------------------------------------------------
#endif
