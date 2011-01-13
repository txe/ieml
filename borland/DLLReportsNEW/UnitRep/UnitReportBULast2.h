//---------------------------------------------------------------------------

#ifndef UnitReportBULast2H
#define UnitReportBULast2H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <vector>

using namespace std;
//---------------------------------------------------------------------------

struct opl_data2
{
    AnsiString   fio;
    int          plan;
    int          pay;
};

class TFormReportBULast2 : public TForm
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
    void __fastcall CreateWordDocument(long rows_count);
    void __fastcall CreateBuhData(void);
    void __fastcall ProcessPlan(AnsiString year);
    void __fastcall ProcessPreDolg(void);
    void __fastcall ProcessCurPlan(void);
    void __fastcall ProcessPay(bool is_09);

private:	// User declarations
    vector<opl_data2*>   buh_data;
    AnsiString firstyear;
    AnsiString secondyear;
public:		// User declarations
    __fastcall TFormReportBULast2(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportBULast2 *FormReportBULast2;
//---------------------------------------------------------------------------
#endif
