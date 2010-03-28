//---------------------------------------------------------------------------

#ifndef UnitFilterMainH
#define UnitFilterMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormFilter : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TComboBox *ComboBoxSubStr;
        TLabel *LabSubStr;
        TSpeedButton *BtnFind;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BtnFindClick(TObject *Sender);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
        __fastcall TFormFilter(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormFilter *FormFilter;
//---------------------------------------------------------------------------
#endif
