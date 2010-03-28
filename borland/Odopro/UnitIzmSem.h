//---------------------------------------------------------------------------

#ifndef UnitIzmSemH
#define UnitIzmSemH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Mask.hpp>

#include "myregini.h"            //For working with ini-files

//---------------------------------------------------------------------------
class TFormIzmSem : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *StrGridDB;
    TLabel *Label1;
    TLabel *LabelCurPlanSem;
    TMaskEdit *MaskEditPredmet;
    TMaskEdit *MaskEditOldPlanSem;
    TLabel *Label2;
    TMaskEdit *MaskEditNewPlanSem;
    TButton *Button1;
    TButton *Button2;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall StrGridDBSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
        int IDStudent;
        int IDGroup;
        EDRegOpts opt_connect;
private:
        void __fastcall InitTable(void);
        void __fastcall UpdateTable(void);
public:
        void __fastcall SetIDStudent(const int& _IDStudent);
        void __fastcall SetIDGroup(const int& _IDGroup);
public:		// User declarations
        __fastcall TFormIzmSem(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormIzmSem *FormIzmSem;
//---------------------------------------------------------------------------
#endif
