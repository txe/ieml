//---------------------------------------------------------------------------

#ifndef UnitSelectSemestrH
#define UnitSelectSemestrH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormSelectSemestr : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *EditSemestrGraphic;
        TUpDown *UpDownGraphic;
    TButton *Button1;
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
        int semestr_gr;
        int semestr_pl;
public:		// User declarations
        __fastcall TFormSelectSemestr(TComponent* Owner);
        int __fastcall GetSemestrPlan(void);
        int __fastcall GetSemestrGraphic(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSelectSemestr *FormSelectSemestr;
//---------------------------------------------------------------------------
#endif
