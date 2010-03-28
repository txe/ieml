//---------------------------------------------------------------------------

#ifndef UnitSelDrobH
#define UnitSelDrobH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormSelDrob : public TForm
{
__published:	// IDE-managed Components
        TComboBox *ComboBox1;
        TBitBtn *BitBtn1;
        void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormSelDrob(TComponent* Owner);

        int __fastcall GetIndex() { return ComboBox1->ItemIndex; }
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSelDrob *FormSelDrob;
//---------------------------------------------------------------------------
#endif
