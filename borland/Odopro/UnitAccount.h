//---------------------------------------------------------------------------

#ifndef UnitAccountH
#define UnitAccountH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormAccount : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TEdit *EditLogin;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *EditPassword;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
        int NumFalseAccounts;
        bool __fastcall IsTrueAccount(void);
public:		// User declarations
        __fastcall TFormAccount(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAccount *FormAccount;
//---------------------------------------------------------------------------
#endif
