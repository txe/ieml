//---------------------------------------------------------------------------

#ifndef UnitDeltaDogovorH
#define UnitDeltaDogovorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TFormDeltaDogovor : public TForm
{
__published:	// IDE-managed Components
    TMaskEdit *MaskEditVert;
    TMaskEdit *MaskEditGor;
    TButton *Button1;
    TButton *Button2;
    TLabel *Label1;
    TLabel *Label2;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormDeltaDogovor(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDeltaDogovor *FormDeltaDogovor;
//---------------------------------------------------------------------------
#endif
