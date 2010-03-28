//---------------------------------------------------------------------------

#ifndef UnitStatisticH
#define UnitStatisticH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormStatistic : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TComboBox *ComboBoxgroup;
        TComboBox *ComboBoxspec;
        TComboBox *ComboBoxeduform;
        TComboBox *ComboBoxdogfast;
        TComboBox *ComboBoxdogyear;
        TComboBox *ComboBoxSex;
        TMaskEdit *MaskEditbyear;
        TComboBox *ComboBoxeducation;
        TComboBox *ComboBoxlanguage;
        TSpeedButton *BtnClose;
        TSpeedButton *BtnStatistic;
        TCheckBox *CheckBoxSex;
        TCheckBox *CheckBoxbyear;
        TCheckBox *CheckBoxeducation;
        TCheckBox *CheckBoxlanguage;
        TCheckBox *CheckBoxdogyear;
        TCheckBox *CheckBoxgroup;
        TCheckBox *CheckBoxspec;
        TCheckBox *CheckBoxeduform;
        TCheckBox *CheckBoxdogfast;
        TMemo *Memo;
        TCheckBox *CheckBoxIgnore;
        void __fastcall BtnCloseClick(TObject *Sender);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall CheckBoxSexClick(TObject *Sender);
        void __fastcall BtnStatisticClick(TObject *Sender);
        void __fastcall MaskEditbyearExit(TObject *Sender);
        bool __fastcall IsTrueYear(AnsiString YearStr);
private:	// User declarations
public:		// User declarations
        __fastcall TFormStatistic(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormStatistic *FormStatistic;
//---------------------------------------------------------------------------
#endif
