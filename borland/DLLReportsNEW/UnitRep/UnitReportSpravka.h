//---------------------------------------------------------------------------

#ifndef UnitReportSpravkaH
#define UnitReportSpravkaH
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <QRCtrls.hpp>
#include <QuickRpt.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TFormReportSpravka : public TForm
{
__published:	// IDE-managed Components
        TQuickRep *QuickRep;
        TQRBand *PageFooterBand;
        TQRLabel *QRLabel1;
        TQRLabel *QRLabel2;
        TQRLabel *QRLabel3;
        TQRLabel *QRLabel4;
        TQRBand *TitleBand;
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
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolButtonClose;
        TImageList *ImageList;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall ToolButtonCloseClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
        int idstudent;
public:		// User declarations
        __fastcall TFormReportSpravka(TComponent* Owner);
        void __fastcall SetIDStudent(int _IDStudent);
        int __fastcall GetIDStudent(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportSpravka *FormReportSpravka;
//---------------------------------------------------------------------------
#endif
