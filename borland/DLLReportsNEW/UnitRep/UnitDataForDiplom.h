//---------------------------------------------------------------------------

#ifndef UnitDataForDiplomH
#define UnitDataForDiplomH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormDataForDiplom : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *EditNumberDiplom;
        TLabel *Label2;
        TLabel *Label3;
        TDateTimePicker *DateTimePickerDateVidachi;
        TLabel *Label4;
        TDateTimePicker *DateTimePickerDatePrisQualif;
        TSpeedButton *BtnOK;
        TBevel *Bevel1;
        TEdit *MaskEditRegNum;
        void __fastcall BtnOKClick(TObject *Sender);
        void __fastcall MaskEditRegNumEnter(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormDataForDiplom(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDataForDiplom *FormDataForDiplom;
//---------------------------------------------------------------------------
#endif
