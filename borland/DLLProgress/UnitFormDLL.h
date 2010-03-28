//---------------------------------------------------------------------------

#ifndef UnitFormDLLH
#define UnitFormDLLH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------

class TFormDLLProgr : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBoxOcenka;
        TLabel *Label1;
        TListBox *ListBoxOcenk;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *EditSemestrPlan;
        TEdit *EditSemestrGraphic;
        TUpDown *UpDownPlan;
        TUpDown *UpDownGraphic;
        TComboBox *ComboBoxOcenka;
        TLabel *Label4;
        TComboBox *ComboBoxBall;
        TLabel *Label5;
        TSpeedButton *BtnEditDiscip;
        TSpeedButton *BtnCheckOcenk;
        TComboBox *ComboBoxDiscip;
        TSpeedButton *BtnDel;
        TSpeedButton *BtnProgressDefSemestr;
        TGroupBox *GroupBox1;
        TEdit *EditVKRTitle;
        TLabel *Label6;
        TSpeedButton *BtnVKR;
        TLabel *Label7;
    TLabel *EstimLabel;
    TButton *ButtonFast;
        void __fastcall LoadData(char* IniFileName,int _idstud);
        void __fastcall BtnEditDiscipClick(TObject *Sender);
        void __fastcall BtnCheckOcenkClick(TObject *Sender);
        void __fastcall UpdateDisciplinesInfo(void);
        int __fastcall GetSpecIDForStudID(int _idstud);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall ComboBoxDiscipChange(TObject *Sender);
        void __fastcall BtnDelClick(TObject *Sender);
        void __fastcall ListBoxOcenkClick(TObject *Sender);
        void __fastcall BtnProgressDefSemestrClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall BtnVKRClick(TObject *Sender);
        void __fastcall UpdateVKR(void);
    void __fastcall ComboBoxBallChange(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ButtonFastClick(TObject *Sender);
private:	// User declarations
        int idstud;
        int estim;  // 0 - невключен
                    // 1 - экзамен
                    // 2 - зачет
public:		// User declarations
        __fastcall TFormDLLProgr(TComponent* Owner);
        int __fastcall GetIDCurStud(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDLLProgr *FormDLLProgr;
//---------------------------------------------------------------------------
#endif
