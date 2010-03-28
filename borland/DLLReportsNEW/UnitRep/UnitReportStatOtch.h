//---------------------------------------------------------------------------

#ifndef UnitReportStatOtchH
#define UnitReportStatOtchH
//---------------------------------------------------------------------------
#include "UnitSelectSemestr.h"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>

#include <vector>
#include <map>

using namespace std;
//---------------------------------------------------------------------------
class TFormReportStatOtch : public TForm
{
__published:	// IDE-managed Components
        TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        int idgroup;
        TFormSelectSemestr *pFormSemestr;

        void __fastcall CreateData();
        void __fastcall DestroyData();

        AnsiString __fastcall GetSpec();
public:		// User declarations
        __fastcall TFormReportStatOtch(TComponent* Owner);
        void __fastcall SetIDGroup(int _IDGroup);
        int __fastcall GetIDGroup(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportStatOtch *FormReportStatOtch;
//---------------------------------------------------------------------------
#endif
