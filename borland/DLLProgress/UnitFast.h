//---------------------------------------------------------------------------

#ifndef UnitFastH
#define UnitFastH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>

#include <vector>

struct pr_data{
    AnsiString  idstud;
    AnsiString  fio;
    AnsiString  ball;
    AnsiString  estim;
    AnsiString  bla;
};
//---------------------------------------------------------------------------
class TFormFast : public TForm
{
__published:	// IDE-managed Components
    TStringGrid *StrGridListOcenk;
    TButton *ButtonSave;
    TButton *ButtonClose;
    TLabel *LabelStuff;
        TButton *ButtonCopy;
        TButton *ButtonInsert;
    void __fastcall StrGridListOcenkSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall StrGridListOcenkKeyPress(TObject *Sender, char &Key);
    void __fastcall ButtonSaveClick(TObject *Sender);
    void __fastcall StrGridListOcenkDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall FormResize(TObject *Sender);
        void __fastcall ButtonCopyClick(TObject *Sender);
    void __fastcall ButtonInsertClick(TObject *Sender);
public:	// User declarations
    void CreateTable();
    void UpdateTable();
    AnsiString GetEstByBall(const double &val);
public:
    AnsiString name_pred;
    AnsiString plan_sem;
    AnsiString graf_sem;
    AnsiString iddiscip;
    AnsiString idgroup;
    int estim;  // 0 - невключен
                // 1 - экзамен
                // 2 - зачет
    std::vector<pr_data*> data;
public:		// User declarations
    __fastcall TFormFast(TComponent* Owner);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
