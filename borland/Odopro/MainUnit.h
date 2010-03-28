//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *StrGridDB;
        TSpeedButton *BtnInsert;
        TMainMenu *MainMenu;
        TMenuItem *FileItem;
        TMenuItem *ExitItem;
        TMenuItem *DataBaseItem;
        TMenuItem *LoadItem;
        TMenuItem *HelpItem;
        TMenuItem *AboutItem;
        TMemo *Memo;
        TSpeedButton *BtnEdit;
        TSpeedButton *BtnUpdate;
        TMenuItem *UpdateItem;
        TSpeedButton *BtnDelete;
        TComboBox *ComboBoxGroup;
        TLabel *LabGroup;
        TSpeedButton *BtnVoc;
        TMenuItem *FilterItem;
        TStatusBar *StatusBar;
        TMenuItem *ItemLine1;
        TMenuItem *ItemAdd;
        TMenuItem *ItemDel;
        TMenuItem *ItemEdit;
        TMenuItem *ItemVoc;
        TImageList *ImageList;
        TMenuItem *ItemLine2;
        TMenuItem *ItemExecReport;
        TMenuItem *StatItem;
        TMenuItem *ReportItem;
        TMenuItem *CommFilterItem;
        TMenuItem *ClearDBItem;
        TMenuItem *PlanSemItem;
    TMenuItem *GraphSemItem;
    TMenuItem *UpdateVerItem;
    TMenuItem *IzmOchenkaItem;
    TMenuItem *N1;
    TMenuItem *N2;
        void __fastcall BtnInsertClick(TObject *Sender);
        void __fastcall ExitItemClick(TObject *Sender);
        void __fastcall BtnEditClick(TObject *Sender);
        void __fastcall BtnUpdateClick(TObject *Sender);
        void __fastcall InitTable(void);
        void __fastcall ClearTable(TStringGrid *pTab);
        void __fastcall UpdateTable(TStringGrid *pTab);
        void __fastcall UpdateRecord(TStringGrid *pTab);
        void __fastcall InsertRecord(TStringGrid *pTab);
        void __fastcall BtnDeleteClick(TObject *Sender);
        void __fastcall StrGridDBClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ComboBoxGroupChange(TObject *Sender);
        void __fastcall BtnVocClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall LoadItemClick(TObject *Sender);
        void __fastcall ConnectWithDB(void);
        void __fastcall FilterItemClick(TObject *Sender);
        void __fastcall UpdateTableWithFilter(TStringGrid* pTab,AnsiString SubStrSN);
        void __fastcall UpdateTableWithCommFilter(TStringGrid* pTab,AnsiString FindStr);
        void __fastcall InitConnectionData(void);
        void __fastcall ChangeEnableItems(bool Enable);
        void __fastcall SetStatusBar(void);
        void __fastcall AboutItemClick(TObject *Sender);
        void __fastcall ItemExecReportClick(TObject *Sender);
        void __fastcall StatItemClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall MyEventDoingReport(TObject *Sender);
        void __fastcall CommFilterItemClick(TObject *Sender);
        void __fastcall ClearDBItemClick(TObject *Sender);
    void __fastcall PlanSemItemClick(TObject *Sender);
    void __fastcall GraphSemItemClick(TObject *Sender);
    void __fastcall UpdateVerItemClick(TObject *Sender);
    void __fastcall IzmOchenkaItemClick(TObject *Sender);
    void __fastcall N1Click(TObject *Sender);
    void __fastcall N2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
