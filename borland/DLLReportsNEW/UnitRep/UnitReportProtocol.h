//---------------------------------------------------------------------------

#ifndef UnitReportProtocolH
#define UnitReportProtocolH
#include "ZAbstractDataset.hpp"
#include "ZAbstractRODataset.hpp"
#include "ZConnection.hpp"
#include "ZDataset.hpp"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <QRCtrls.hpp>
#include <QuickRpt.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TFormReportProtocol : public TForm
{
__published:	// IDE-managed Components
        TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolButtonClose;
        TQuickRep *QuickRep;
        TQRBand *TitleBand;
        TQRLabel *QRLabelspec;
        TQRLabel *QRLabelFIO;
        TQRLabel *QRLabelShifrSpec;
        TQRLabel *QRLabelIOFam;
        TQRLabel *QRLabelGroup;
        TQRLabel *QRLabelVUZ;
        TQRLabel *QRLabel1;
    TZConnection *ZMySqlDatabase;
    TZQuery *ZMySqlQuery;
    TZQuery *ZMySqlQueryOpts;
        void __fastcall ToolButtonCloseClick(TObject *Sender);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall InitReportQuery(void);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        int idstudent;
public:		// User declarations
        __fastcall TFormReportProtocol(TComponent* Owner);
        void __fastcall SetIDStudent(int _IDStudent);
        int __fastcall GetIDStudent(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportProtocol *FormReportProtocol;
//---------------------------------------------------------------------------
#endif
