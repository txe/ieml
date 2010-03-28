//---------------------------------------------------------------------------

#ifndef UnitClearDBH
#define UnitClearDBH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormClearDB : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TCheckListBox *CheckListBoxTabs;
        TBevel *Bevel1;
        TSpeedButton *BtnDel;
        TBevel *Bevel2;
        TGroupBox *GroupBox2;
        void __fastcall BtnDelClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormClearDB(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormClearDB *FormClearDB;
//---------------------------------------------------------------------------
#endif
