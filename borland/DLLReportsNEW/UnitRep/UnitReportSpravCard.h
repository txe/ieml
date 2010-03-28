//---------------------------------------------------------------------------

#ifndef UnitReportSpravCardH
#define UnitReportSpravCardH
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
class TFormReportSpravCard : public TForm
{
__published:	// IDE-managed Components
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolButtonClose;
        TImageList *ImageList;
        TQuickRep *QuickRep;

        TQRBand *TitleBand;
        TQRLabel *QRLabel1;
        TQRLabel *QRLabel2;
        TQRLabel *QRLabel3;
        TQRLabel *QRLabel4;
        TQRLabel *QRLabel5;
        TQRLabel *QRLabel6;
        TQRLabel *QRLabel7;
        TQRLabel *QRLabel8;
        TQRLabel *QRLabel9;
        TQRLabel *QRLabel10;
        TQRLabel *QRLabel11;
        TQRLabel *QRLabel12;
        TQRLabel *QRLabel13;
        TQRLabel *QRLabel14;
        TQRLabel *QRLabel15;
        TQRLabel *QRLabel16;
        TQRLabel *QRLabel17;
        TQRLabel *QRLabel18;
        TQRLabel *QRLabel19;
        TQRLabel *QRLabel20;
        TQRLabel *QRLabel21;
        TQRLabel *QRLabel22;
        TQRLabel *QRLabel23;
        TQRLabel *QRLabel24;
        TQRLabel *QRLabel25;
        TQRLabel *QRLabel26;
        TQRLabel *QRLabel27;
        TQRLabel *QRLabel28;
        TQRLabel *QRLabel29;
        TQRLabel *QRLabel30;
        TQRLabel *QRLabel31;
        TQRLabel *QRLabel32;
        TQRLabel *QRLabel33;
        TQRLabel *QRLabelFacVuz;
        TQRLabel *QRLabelbdate;
        TQRLabel *QRLabelsecondname;
        TQRLabel *QRLabelfirstname;
        TQRLabel *QRLabelthirdname;
        TQRLabel *QRLabeldogyear;
        TQRLabel *QRLabelspec;
        TQRLabel *QRLabel34;
        TQRLabel *QRLabeladdr1;
        TQRLabel *QRLabeladdr2;
        TQRLabel *QRLabeladdr3;
    TZConnection *ZMySqlDatabase;
    TZQuery *ZMySqlQuery;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolButtonCloseClick(TObject *Sender);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall InitReportQuery(void);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        int idstudent;
public:		// User declarations
        __fastcall TFormReportSpravCard(TComponent* Owner);
        void __fastcall SetIDStudent(int _IDStudent);
        int __fastcall GetIDStudent(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportSpravCard *FormReportSpravCard;
//---------------------------------------------------------------------------
#endif
