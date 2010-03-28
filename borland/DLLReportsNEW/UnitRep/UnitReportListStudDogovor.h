//---------------------------------------------------------------------------

#ifndef UnitReportListStudDogovorH
#define UnitReportListStudDogovorH
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
class TFormReportListStudDogovor : public TForm
{
__published:	// IDE-managed Components

        TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
        TZConnection *ZMySqlDatabase;
        TZQuery *ZMySqlQuery;
        void __fastcall InitReportQuery(void);
        void __fastcall CreateWordDocument(void);
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        TFormListGroup *pFormList;
public:		// User declarations
        __fastcall TFormReportListStudDogovor(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportListStudDogovor *FormReportListStudDogovor;
//---------------------------------------------------------------------------
#endif
