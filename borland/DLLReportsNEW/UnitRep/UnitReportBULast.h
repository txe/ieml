//---------------------------------------------------------------------------

#ifndef UnitReportBULastH
#define UnitReportBULastH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <vector>

using namespace std;
//---------------------------------------------------------------------------

struct opl_data
{
    AnsiString   title;
    int          plan;
    int          pay;
};

class TFormReportBULast : public TForm
{
__published:	// IDE-managed Components
    TToolBar *ToolBar;
    TToolButton *ToolBtnPreview;
    TToolButton *ToolBtnClose;
    TImageList *ImageList;
    void __fastcall ToolBtnPreviewClick(TObject *Sender);
    void __fastcall ToolBtnCloseClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
    void __fastcall CreateWordDocument(void);
    void __fastcall CreateBuhData(void);
    void __fastcall DestroyBuhData(void);
    void __fastcall CreateQuery(void);
    void __fastcall DestroyQuery(void);
private:	// User declarations
    vector<opl_data*>   buh_data;
    AnsiString firstyear;
    AnsiString secondyear;
public:		// User declarations
    __fastcall TFormReportBULast(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportBULast *FormReportBULast;
//---------------------------------------------------------------------------
#endif
