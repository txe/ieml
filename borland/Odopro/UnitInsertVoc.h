//---------------------------------------------------------------------------

#ifndef UnitInsertVocH
#define UnitInsertVocH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TFormInsertVoc : public TForm
{
__published:	// IDE-managed Components
        TLabel *LabKey;
        TComboBox *ComboBoxKey;
        TStringGrid *StrGridVoc;
        TMemo *Memo1;
        TSpeedButton *BtnClose;
        TEdit *Edittitle;
        TLabel *Labtitle;
        TSpeedButton *BtnAdd;
        TSpeedButton *BtnDel;
        TSpeedButton *BtnEdit;
        void __fastcall InitTable(AnsiString WHEREStr,AnsiString KeyOrder);
        void __fastcall ClearTable(void);
        void __fastcall ComboBoxKeyChange(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BtnCloseClick(TObject *Sender);
        void __fastcall BtnAddClick(TObject *Sender);
        int __fastcall GetCountRecForKey(AnsiString vkey);
        void __fastcall BtnDelClick(TObject *Sender);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall StrGridVocDblClick(TObject *Sender);
        void __fastcall StrGridVocClick(TObject *Sender);
        void __fastcall BtnEditClick(TObject *Sender);
        void __fastcall StrGridVocMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        bool __fastcall IsExistsTitleForKey(AnsiString vkey,AnsiString title);

private:	// User declarations
public:		// User declarations
        __fastcall TFormInsertVoc(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormInsertVoc *FormInsertVoc;
//---------------------------------------------------------------------------
#endif
