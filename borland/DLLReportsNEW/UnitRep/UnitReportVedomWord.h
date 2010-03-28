//---------------------------------------------------------------------------

#ifndef UnitReportVedomWordH
#define UnitReportVedomWordH
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

//---------------------------------------------------------------------------
class TFormReportVedomWord : public TForm
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
        int idgroup;
public:		// User declarations
        __fastcall TFormReportVedomWord(TComponent* Owner);
        void __fastcall SetIDGroup(int _IDGroup);
        int __fastcall GetIDGroup(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportVedomWord *FormReportVedomWord;
//---------------------------------------------------------------------------
#endif
