//---------------------------------------------------------------------------

#ifndef UnitReportVedomH
#define UnitReportVedomH
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
class TFormReportVedom : public TForm
{
__published:	// IDE-managed Components
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
        TImageList *ImageList;
        TQuickRep *QuickRep;
        TQRBand *TitleBand;
        TQRLabel *QRLabel;
        TQRLabel *QRLabel1;
        TQRLabel *QRLabel2;
        TQRLabel *QRLabel3;
        TQRLabel *QRLabel4;
        TQRLabel *QRLabel5;
        TQRLabel *QRLabelGroup;
        TQRBand *DetailBand;
        TQRDBText *QRDBTextSecName;
        TQRSysData *QRSysDataCurRec;
        TQRShape *QRShape2;
        TQRShape *QRShape3;
        TQRDBText *QRDBTextFirstName;
        TQRShape *QRShape6;
        TQRDBText *QRDBTextThirdName;
        TQRShape *QRShape8;
        TQRDBText *QRDBTextznum;
        TQRShape *QRShape10;
        TQRShape *QRShape11;
        TQRShape *QRShape12;
        TQRBand *ColumnHeaderBand;
        TQRLabel *QRLabel6;
        TQRLabel *QRLabel7;
        TQRLabel *QRLabel8;
        TQRShape *QRShape1;
        TQRShape *QRShape4;
        TQRLabel *QRLabel9;
        TQRShape *QRShape5;
        TQRLabel *QRLabel10;
        TQRShape *QRShape7;
        TQRShape *QRShape9;
        TQRLabel *QRLabel11;
        TQRLabel *QRLabel12;
        TQRLabel *QRLabel13;
        TQRShape *QRShape13;
        TQRShape *QRShape14;
        TQRBand *SummaryBand;
        TQRLabel *QRLabel14;
        TQRLabel *QRLabel15;
        TQRLabel *QRLabel16;
        TQRLabel *QRLabel17;
        TQRLabel *QRLabel18;
        TQRLabel *QRLabel19;
        TQRLabel *QRLabel20;
        TQRLabel *QRLabel21;
        TQRBand *PageFooterBand;
        TQRSysData *QRSysDataNumPage;
        TQRLabel *QRLabel22;
        TQRLabel *QRLabel23;
        TQRLabel *QRLabelSpec;
    TZConnection *ZMySqlDatabase;
    TZQuery *ZMySqlQuery;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall Connect(void);
        void __fastcall InitReportQuery(void);
        void __fastcall InitReportQueryForSpec(int specid);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        int idgroup;
public:		// User declarations
        __fastcall TFormReportVedom(TComponent* Owner);
        void __fastcall SetIDGroup(int _IDGroup);
        int __fastcall GetIDGroup(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportVedom *FormReportVedom;
//---------------------------------------------------------------------------
#endif
