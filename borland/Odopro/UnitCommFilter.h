//---------------------------------------------------------------------------

#ifndef UnitCommFilterH
#define UnitCommFilterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormFilterComm : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel;
        TGroupBox *GroupBoxStr;
        TRadioButton *RadioButtonStr;
        TComboBox *ComboBoxSubStr;
        TSpeedButton *BtnFindStr;
        TCheckBox *CheckBox1;
        TRadioButton *RadioButtonData;
        TGroupBox *GroupBoxData;
        TCheckBox *CheckBox2;
        TSpeedButton *BtnFindData;
        TDateTimePicker *DateTimePicker;
        TGroupBox *GroupBoxDig;
        TSpeedButton *BtnFindDig;
        TCheckBox *CheckBox3;
        TRadioButton *RadioButtonDig;
        TComboBox *ComboBoxDig;
        void __fastcall BtnFindStrClick(TObject *Sender);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall RadioButtonStrClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormFilterComm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormFilterComm *FormFilterComm;
//---------------------------------------------------------------------------
#endif
