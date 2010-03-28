//---------------------------------------------------------------------------

#ifndef UnitReportDocDopuskVstupIspitH
#define UnitReportDocDopuskVstupIspitH
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
#include "UnitListGroups.h"
//---------------------------------------------------------------------------
class TFormReportDocDopuskVstupIspit : public TForm
{
__published:	// IDE-managed Components

        TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
        TZConnection *ZMySqlDatabase;
        TZQuery *ZMySqlQuery;
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall CreateWordDocument(void);
        void __fastcall InitReportQuery(void);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        TFormListGroup *pFormList;
public:		// User declarations
        __fastcall TFormReportDocDopuskVstupIspit(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportDocDopuskVstupIspit *FormReportDocDopuskVstupIspit;
//---------------------------------------------------------------------------
#endif
