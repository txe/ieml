//---------------------------------------------------------------------------

#ifndef UnitIzmOchenkaH
#define UnitIzmOchenkaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>
#include <vector>
#include "myregini.h"            //For working with ini-files
struct data_progress
{
    AnsiString fulltitle;
    int id_prog;
    int estimation;
    int iddisc;
    int plansem;
};
using namespace std;
//---------------------------------------------------------------------------
class TFormIzmOchenka : public TForm
{
__published:	// IDE-managed Components
    TStringGrid *StrGridDB;
    TButton *Button1;
    TButton *Button2;
    TLabel *Label1;
    TMaskEdit *MaskEditPredmet;
    TRadioGroup *RadioGroupType;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall StrGridDBSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormIzmOchenka(TComponent* Owner);
private:	// User declarations
        int IDStudent;
        int IDGroup;
        EDRegOpts opt_connect;
        vector<data_progress*> progress;
private:
        void __fastcall InitTable(void);
        void __fastcall UpdateTable(void);
        void __fastcall delete_data(void);
public:
        void __fastcall SetIDStudent(const int& _IDStudent);
        void __fastcall SetIDGroup(const int& _IDGroup);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormIzmOchenka *FormIzmOchenka;
//---------------------------------------------------------------------------
#endif
