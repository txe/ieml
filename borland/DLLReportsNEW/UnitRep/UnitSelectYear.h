//---------------------------------------------------------------------------

#ifndef UnitSelectYearH
#define UnitSelectYearH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormSelectYear : public TForm
{
__published:	// IDE-managed Components
    TComboBox *ComboBoxFirstYear;
    TEdit *EditSecondYear;
    TLabel *Label1;
    TButton *Button1;
    TButton *Button2;
    void __fastcall ComboBoxFirstYearChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormSelectYear(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSelectYear *FormSelectYear;
//---------------------------------------------------------------------------
#endif
