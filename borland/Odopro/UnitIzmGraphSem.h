//---------------------------------------------------------------------------

#ifndef UnitIzmGraphSemH
#define UnitIzmGraphSemH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Mask.hpp>

#include "myregini.h"            //For working with ini-files
//---------------------------------------------------------------------------
class TFormIzmGraphSem : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TLabel *LabelCurPlanSem;
    TLabel *Label2;
    TStringGrid *StrGridDB;
    TMaskEdit *MaskEditPredmet;
    TMaskEdit *MaskEditOldGraphSem;
    TMaskEdit *MaskEditNewGraphSem;
    TButton *Button1;
    TButton *Button2;
    void __fastcall StrGridDBSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
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
    __fastcall TFormIzmGraphSem(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormIzmGraphSem *FormIzmGraphSem;
//---------------------------------------------------------------------------
#endif
