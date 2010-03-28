//---------------------------------------------------------------------------

#ifndef UnitReportCharacteristicH
#define UnitReportCharacteristicH
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
class TFormReportCharacteristic : public TForm
{
__published:	// IDE-managed Components

        TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
    TZConnection *ZMySqlDatabase;
    TZQuery *ZMySqlQuery;
    TZQuery *ZMySqlQueryAvrgBall;

        void __fastcall InitReportQuery(void);
        void __fastcall CreateWordDocument(void);
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall GetBallsCount(int& countUDOVL,double& percUDOVL,int& countHOR,double& percHOR,int& countOTL,double& percOTL,AnsiString& AvrgBallStr,AnsiString IDStudStr);
        void __fastcall InitItog(float& itog_oc);
        AnsiString __fastcall ItogForUr(bool IsMan);
private:	// User declarations
        int idstudent;
public:		// User declarations
        __fastcall TFormReportCharacteristic(TComponent* Owner);
        void __fastcall SetIDStudent(int _IDStudent);
        int __fastcall GetIDStudent(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportCharacteristic *FormReportCharacteristic;
//---------------------------------------------------------------------------
#endif
