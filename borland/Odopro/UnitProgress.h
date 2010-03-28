//---------------------------------------------------------------------------

#ifndef UnitProgressH
#define UnitProgressH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormProgress : public TForm
{
__published:	// IDE-managed Components
        TProgressBar *ProgressBar;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormHide(TObject *Sender);
        void __fastcall Increment(int dPersent);
        int __fastcall GetMax(void);
        int __fastcall GetPos(void);
private:	// User declarations
public:		// User declarations
        __fastcall TFormProgress(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormProgress *FormProgress;
//---------------------------------------------------------------------------
#endif
