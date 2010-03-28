//---------------------------------------------------------------------------

#ifndef UnitReportForma3H
#define UnitReportForma3H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <vector>

struct st_forma3
{
    AnsiString index;
    AnsiString reg_num;
    AnsiString fio;
    AnsiString namespec;
    AnsiString kod_spec;
    AnsiString kod_kvalif;
    AnsiString namekvalif;
    AnsiString otl;
    AnsiString serdip;
    AnsiString numdip;
    AnsiString datavid;
    AnsiString datagak;
    AnsiString numprotgak;
    AnsiString numotch;
    AnsiString dataotch;
    AnsiString grp; 
};

using namespace std;
//---------------------------------------------------------------------------
class TFormReportForma3 : public TForm
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
    void __fastcall CreateData(void);
    void __fastcall DestroyData(void);

private:	// User declarations
    AnsiString curr_year;
    vector<st_forma3*> data;
public:		// User declarations
    __fastcall TFormReportForma3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportForma3 *FormReportForma3;
//---------------------------------------------------------------------------
#endif
