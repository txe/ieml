//---------------------------------------------------------------------------

#ifndef UnitListGroupsH
#define UnitListGroupsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <Buttons.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TFormListGroup : public TForm
{
__published:	// IDE-managed Components
        TCheckListBox *CheckListBox;
        TSpeedButton *SpeedBtnGo;
        void __fastcall SpeedBtnGoClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormListGroup(TComponent* Owner);
        AnsiString __fastcall GetWHEREStr(void);
        AnsiString __fastcall GetWHEREStrForVoc(void);
        std::vector<AnsiString> __fastcall GetGrpList();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormListGroup *FormListGroup;
//---------------------------------------------------------------------------
#endif
