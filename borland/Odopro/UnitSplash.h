//---------------------------------------------------------------------------

#ifndef UnitSplashH
#define UnitSplashH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TSplashForm : public TForm
{
__published:	// IDE-managed Components
        TTimer *Timer;
        TPanel *Panel;      
        TImage *Image;
        TLabel *Label1;
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall TimerTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TSplashForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSplashForm *SplashForm;
//---------------------------------------------------------------------------
#endif
