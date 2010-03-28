//---------------------------------------------------------------------------

#ifndef UnitEditDisciplinesH
#define UnitEditDisciplinesH
//---------------------------------------------------------------------------
#include "UnitFuncs.h"
#include "mysql.h"
#include "UnitFormDLL.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Mask.hpp>
#include <vector>

using namespace std;
struct AudHours
{
    AnsiString hours[11];
};
//---------------------------------------------------------------------------
class TFormDisciplines : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *StrGridDiscip;
        TComboBox *ComboBoxSpec;
        TLabel *LabSpec;
        TComboBox *ComboBoxDiscipline;
        TLabel *LabDiscipline;
        TSpeedButton *BtnAdd;
        TSpeedButton *BtnDel;
        TSpeedButton *BtnEdit;
        TSpeedButton *BtnClose;
        TLabel *Label1;
        TComboBox *ComboBoxAbr;
        TLabel *Label2;
        TComboBox *ComboBoxClassDiscip;
        TLabel *Label3;
        TEdit *EditNumHours;
        TUpDown *UpDownNumHours;
        TLabel *Label4;
        TLabel *Label5;
        TEdit *EditScanNumber;
        TUpDown *UpDownScanNumber;
    TGroupBox *GroupBox5;
    TMaskEdit *MaskEditAudHours1;
    TMaskEdit *MaskEditAudHours2;
    TMaskEdit *MaskEditAudHours3;
    TMaskEdit *MaskEditAudHours4;
    TMaskEdit *MaskEditAudHours5;
    TMaskEdit *MaskEditAudHours6;
    TMaskEdit *MaskEditAudHours7;
    TMaskEdit *MaskEditAudHours8;
    TMaskEdit *MaskEditAudHours9;
    TMaskEdit *MaskEditAudHours10;
    TMaskEdit *MaskEditAudHours11;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
        void __fastcall FormResize(TObject *Sender);
        void __fastcall BtnCloseClick(TObject *Sender);
        void __fastcall BtnAddClick(TObject *Sender);
        void __fastcall BtnDelClick(TObject *Sender);
        void __fastcall BtnEditClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ClearTable(TStringGrid* pTab);
        void __fastcall UpdateTable(TStringGrid* pTab);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall StrGridDiscipClick(TObject *Sender);
        void __fastcall ComboBoxSpecChange(TObject *Sender);
        void __fastcall SetMyFormDLL(TFormDLLProgr* curFormDLL);
        bool __fastcall IsExistsRecordDiscip(int &id,AnsiString &spec,AnsiString &fulltitle,AnsiString &shorttitle);
        AnsiString __fastcall GetSpecForStudID(int _idstud);
        void __fastcall EditNumHoursExit(TObject *Sender);
        void __fastcall ComboBoxClassDiscipChange(TObject *Sender);
        void __fastcall EditScanNumberExit(TObject *Sender);
    void __fastcall MaskEditAudHoursExit(TObject *Sender);
private:	// User declarations
        TFormDLLProgr* MyFormDLL;
        vector<AudHours*> audhoursdiscip;
private:
        AnsiString __fastcall AudHoursToString();
        void __fastcall StringToAudHours(AudHours* h, AnsiString &str_hours);
        void __fastcall delete_data(void);
public:		// User declarations
        __fastcall TFormDisciplines(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDisciplines *FormDisciplines;
//---------------------------------------------------------------------------
#endif
