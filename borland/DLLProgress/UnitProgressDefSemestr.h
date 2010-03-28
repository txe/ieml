//---------------------------------------------------------------------------

#ifndef UnitProgressDefSemestrH
#define UnitProgressDefSemestrH
//---------------------------------------------------------------------------
#include "UnitFormDLL.h"
#include "UnitFuncs.h"
#include "mysql.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFormProgressDefSemestr : public TForm
{
__published:	// IDE-managed Components
        TRadioButton *RadioButtonPlan;
        TRadioButton *RadioButtonGraph;
        TStringGrid *StrGridListOcenk;
        TUpDown *UpDownPlan;
        TUpDown *UpDownGraph;
        TEdit *EditGraph;
        TEdit *EditPlan;
        TSpeedButton *BtnClose;
        void __fastcall SetMyFormDLL(TFormDLLProgr* curFormDLL);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall RadioButtonPlanClick(TObject *Sender);
        void __fastcall BtnCloseClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall ClearTable(TStringGrid* pTab);
        void __fastcall UpdateTable(TStringGrid* pTab);
        void __fastcall SetIDStud(int _IDStud);
        void __fastcall UpDownPlanClick(TObject *Sender,
          TUDBtnType Button);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
        TFormDLLProgr* MyFormDLL;
        int idstud;
public:		// User declarations
        __fastcall TFormProgressDefSemestr(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormProgressDefSemestr *FormProgressDefSemestr;
//---------------------------------------------------------------------------
#endif
