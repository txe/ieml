//---------------------------------------------------------------------------

#ifndef UnitReportDogovorH
#define UnitReportDogovorH
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
class TFormReportDogovor : public TForm
{
__published:	// IDE-managed Components
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolButtonClose;
        TQRCompositeReport *QRCompositeReport;
        TImageList *ImageList;
        TQuickRep *QuickRep1;
        TQRBand *TitleBand;
        TQuickRep *QuickRep2;
        TQRBand *QRBand1;
        TQRLabel *QRLabelspec11;
        TQRLabel *QRLabelform11;
        TQRLabel *QRLabelspec21;
        TQRLabel *QRLabelform21;
        TQRLabel *QRLabelbdateDay;
        TQRLabel *QRLabeladdr;
        TQRLabel *QRLabelliveaddr;
        TQRLabel *QRLabelFIO1;
        TQRLabel *QRLabelRF;
        TQRLabel *QRLabelShifrSpec1;
        TQRLabel *QRLabelspec12;
        TQRLabel *QRLabelspec22;
        TQRLabel *QRLabelform12;
        TQRLabel *QRLabelform22;
        TQRLabel *QRLabelShifrSpec2;
        TQRLabel *QRLabelFIO2;
        TQRLabel *QRLabelbdateMounthYear;
        TQRLabel *QRLabelphones;
        TQRLabel *QRLabelpassseries;
        TQRLabel *QRLabelpassnumber;
        TQRLabel *QRLabelpassdateDay;
        TQRLabel *QRLabelpassdateMounthYear;
        TQRLabel *QRLabelpassplace1;
        TQRLabel *QRLabelIOFam;
        TQRLabel *QRLabelpassplace2;
        TQRLabel *QRLabelCountMoney;
        TQRLabel *QRLabelCountMoneyStr;
        TQRLabel *QRLabeldognum;
    TZConnection *ZMySqlDatabase;
    TZQuery *ZMySqlQuery;
    TZReadOnlyQuery *ZMySqlQueryOpts;
        void __fastcall ToolButtonCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall InitReportQuery(void);
        int  __fastcall GetCountMoney(void);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        int  idstudent;
        bool _IsNewType;
public:		// User declarations
        void SetOfset(double& val);
        __fastcall TFormReportDogovor(TComponent* Owner);
        void __fastcall SetIDStudent(int _IDStudent);
        int __fastcall GetIDStudent(void);
        AnsiString __fastcall GetNumberOfDogovor(void);
        void IsNewType(bool yes) { _IsNewType = yes; }
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportDogovor *FormReportDogovor;
//---------------------------------------------------------------------------
#endif
