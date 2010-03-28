//---------------------------------------------------------------------------

#ifndef UnitReportEkzZachListH
#define UnitReportEkzZachListH
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
class TFormReportEkzZachList : public TForm
{
__published:	// IDE-managed Components
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolButtonClose;
        TImageList *ImageList;
        TQuickRep *QuickRep;
        TQRBand *TitleBand1;
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
        TQRShape *QRShape;
        TQRLabel *QRLabel29;
        TQRLabel *QRLabel30;
        TQRLabel *QRLabel31;
        TQRLabel *QRLabel32;
        TQRLabel *QRLabel33;
        TQRLabel *QRLabel34;
        TQRLabel *QRLabel35;
        TQRLabel *QRLabel36;
        TQRLabel *QRLabel37;
        TQRLabel *QRLabel38;
        TQRLabel *QRLabel39;
        TQRLabel *QRLabel40;
        TQRLabel *QRLabel41;
        TQRLabel *QRLabel42;
        TQRLabel *QRLabel43;
        TQRLabel *QRLabel44;
        TQRLabel *QRLabel45;
        TQRLabel *QRLabel46;
        TQRLabel *QRLabel47;
        TQRLabel *QRLabel48;
        TQRLabel *QRLabel49;
        TQRLabel *QRLabel50;
        TQRLabel *QRLabel51;
        TQRLabel *QRLabel52;
        TQRLabel *QRLabel53;
        TQRLabel *QRLabel54;
        TQRLabel *QRLabel55;
        TQRLabel *QRLabel56;
       
        TQRLabel *QRLabelFIO;
        TQRLabel *QRLabelznum;
        TQRLabel *QRLabelgroup;
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
        __fastcall TFormReportEkzZachList(TComponent* Owner);
        void __fastcall SetIDStudent(int _IDStudent);
        int __fastcall GetIDStudent(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportEkzZachList *FormReportEkzZachList;
//---------------------------------------------------------------------------
#endif
