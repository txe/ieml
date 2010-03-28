//---------------------------------------------------------------------------

#ifndef UnitReportAvrgBallInGroupH
#define UnitReportAvrgBallInGroupH
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
#include "ZStoredProcedure.hpp"

#include <vector>
using  namespace std;
//---------------------------------------------------------------------------

struct avg_progess
{
    AnsiString name;
    int count_otl;
    int count_hor;
    int count_ud;
    AnsiString sred;
    AnsiString znum;
};

//---------------------------------------------------------------------------
class TFormReportAvrgBallInGroup : public TForm
{
__published:	// IDE-managed Components
         TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
        AnsiString __fastcall GetSpec();
        void __fastcall OpenAvgQuery();
        void __fastcall CloseAvgQuery();
        void __fastcall CreateWordDocument(void);
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall GetBallsCount(int& countUDOVL,double& percUDOVL,int& countHOR,double& percHOR,int& countOTL,double& percOTL,AnsiString& AvrgBallStr,AnsiString IDStudStr);
private:	// User declarations
        int idgroup;
        vector<avg_progess*> avg_progress_data;
public:		// User declarations
        __fastcall TFormReportAvrgBallInGroup(TComponent* Owner);

        void __fastcall SetIDGroup(int _IDGroup);
        int __fastcall GetIDGroup(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportAvrgBallInGroup *FormReportAvrgBallInGroup;
//---------------------------------------------------------------------------
#endif
