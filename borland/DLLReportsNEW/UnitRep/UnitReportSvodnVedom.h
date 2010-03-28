//---------------------------------------------------------------------------

#ifndef UnitReportSvodnVedomH
#define UnitReportSvodnVedomH
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

struct ochenka
{
    int estimat;
    AnsiString ball;
    int idstud;
    int iddiscip;
};
//---------------------------------------------------------------------------
class TFormReportSvodnVedom : public TForm
{
__published:	// IDE-managed Components
        TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
        void __fastcall CreateWordDocument(void);
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        int idgroup;
        TFormSelectSemestr *pFormSemestr;

        vector<AnsiString> fio;
        vector<AnsiString> dis;
        vector<ochenka*> data_ochenka;
        map<int, int> pos_fio;
        map<int, int> pos_dis;
        void __fastcall CreateData();
        void __fastcall DestroyData();

        AnsiString __fastcall GetSpec();
public:		// User declarations
        __fastcall TFormReportSvodnVedom(TComponent* Owner);
        void __fastcall SetIDGroup(int _IDGroup);
        int __fastcall GetIDGroup(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportSvodnVedom *FormReportSvodnVedom;
//---------------------------------------------------------------------------
#endif
