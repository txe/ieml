//---------------------------------------------------------------------------

#ifndef UnitSelectedH
#define UnitSelectedH
//---------------------------------------------------------------------------
#include "mysql.h"
#include "UnitInsertVoc.h"
#include "myregini.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>


enum TTypeQuery{UPDATE=1,INSERT=2, VIEW=3};

enum TTypeKeyVoc{
                  GROUPS=0,
                  CITIES=1,
                  SPECS=2,
                  SPEZIALIZS=3,
                  QUALIFIC=4,
                  DOGSHIFRS=5,
                  DOGFASTS=6,
                  DOGYEARS=7,
                  EDUCATIONS=8,
                  EDUDOCS=9,
                  EDUFORMS=10,
                  LANGUAGES=11,
                  FALSEEDUDOCINFOS=12,
                  SHIFRSPECS=13,
                  DISCIPCLASSIFIC=14
                };

AnsiString ReturnParsedDateInSQL(AnsiString);
//---------------------------------------------------------------------------
class TFormSelected : public TForm
{
__published:	// IDE-managed Components
        TSpeedButton *BtnOk;
        TSpeedButton *BtnCancel;
        TMemo *Memo1;
        TSpeedButton *BtnWorkVoc;
        TPageControl *PageControlSelected;
        TTabSheet *TabSheetLich;
        TTabSheet *TabSheetEducation;
        TTabSheet *TabSheetStudy;
        TLabel *LabSecondName;
        TLabel *LabFirstName;
        TLabel *LabThirdName;
        TLabel *LabSex;
        TLabel *Labbdate;
        TLabel *Labpassseries;
        TLabel *Labpassnum;
        TLabel *Labpassdate;
        TLabel *Labpassplace;
        TLabel *Labpass;
        TLabel *Labemail;
        TLabel *Labaddr;
        TLabel *Labliveaddr;
        TLabel *Labphones;
        TEdit *EditSecondName;
        TEdit *EditFirstName;
        TEdit *EditThirdName;
        TComboBox *ComboBoxSex;
        TMaskEdit *MaskEditbdate;
        TEdit *Editpassseries;
        TEdit *Editpassnum;
        TEdit *Editpassplace;
        TMaskEdit *MaskEditpassdate;
        TEdit *Editemail;
        TEdit *Editaddr;
        TEdit *Editliveaddr;
        TEdit *Editphones;
        TLabel *Labeducation;
        TLabel *Labedudoc;
        TLabel *Labeduplace;
        TLabel *Lablanguage;
        TLabel *Labeduenddate;
        TLabel *Labedudocinfo;
        TLabel *Labeduspec;
        TComboBox *ComboBoxeducation;
        TEdit *Editeduplace;
        TComboBox *ComboBoxlanguage;
        TComboBox *ComboBoxedudoc;
        TMaskEdit *MaskEditeduenddate;
        TEdit *Editedudocinfo;
        TEdit *Editeduspec;
        TLabel *Labenternum;
        TLabel *Labznum;
        TLabel *Labgrp;
        TLabel *Labcity;
        TLabel *Labspec;
        TLabel *Labdognum;
        TLabel *Labdogshifr;
        TLabel *Labdogyear;
        TLabel *Label1;
        TLabel *Labzach;
        TLabel *Labeduform;
        TEdit *Editenternum;
        TEdit *Editznum;
        TComboBox *ComboBoxgrp;
        TComboBox *ComboBoxcity;
        TComboBox *ComboBoxspec;
        TEdit *Editdognum;
        TComboBox *ComboBoxdogyear;
        TComboBox *ComboBoxdogshifr;
        TMaskEdit *MaskEditenterdate;
        TComboBox *ComboBoxeduform;
        TBevel *Bevel1;
        TBevel *Bevel2;
        TBevel *Bevel4;
        TComboBox *ComboBoxdogfast;
        TUpDown *UpDownStudent;
        TSpeedButton *BtnOkCancel;
        TTabSheet *TabSheetRelease;
        TLabel *LabOtch;
        TLabel *Labexitnum;
        TLabel *Label2;
        TLabel *Labmisc;
        TBevel *Bevel6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label3;
        TEdit *Editexitnum;
        TMaskEdit *MaskEditexitdate;
        TMemo *Memomisc;
        TMaskEdit *MaskEditedunumdiplom;
        TMaskEdit *MaskEditedunumreg;
        TMaskEdit *MaskEditedudatediplom;
        TMaskEdit *MaskEditedudatequalif;
        TCheckBox *CheckBoxedudiplomotl;
        TButton *ButtonAutoEntering;
        TButton *ButtonSaveDafaultDate;
    TMaskEdit *MaskEditedunumprotgak;
    TLabel *Label4;
        void __fastcall BtnOkClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BtnCancelClick(TObject *Sender);
        void __fastcall UpdateComboBox(TComboBox* cmb,TTypeKeyVoc tkey);
        AnsiString __fastcall GetTitleForKeyNum(TTypeKeyVoc tkey,int num);
        int __fastcall GetNumForKeyTitle(TTypeKeyVoc tkey, AnsiString title);
        void __fastcall BtnWorkVocClick(TObject *Sender);
        void __fastcall MaskEditbdateExit(TObject *Sender);
        bool __fastcall CompareMaskDate(AnsiString DateStr);
        void __fastcall SaveUpdateLoadComboBox(TComboBox* cmb,TTypeKeyVoc tkey);
        void __fastcall ComboBoxeduformChange(TObject *Sender);
        void __fastcall EditemailExit(TObject *Sender);
        bool __fastcall IsFalseEduDocInfo(void);
        void __fastcall UpDownStudentClick(TObject *Sender,
          TUDBtnType Button);
        void __fastcall BtnOkCancelClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall ComboBoxgrpChange(TObject *Sender);
        AnsiString __fastcall GetCityName(AnsiString GroupStr);
        bool __fastcall IsExistsDogovorNumber(AnsiString& _FIOStr,int& _groupid);
        bool __fastcall IsExistsUserNuke(AnsiString znumStr);
        void __fastcall InsertUserNuke(AnsiString user_id,AnsiString username,AnsiString user_email,AnsiString user_password,AnsiString user_avatar);
        void __fastcall DeleteUserNuke(AnsiString znumStr);
        void __fastcall ChangeUserNuke(AnsiString znumoldStr,AnsiString znumnewStr);
        AnsiString __fastcall GetZNumForStudent(AnsiString IDStudStr);
        int __fastcall GetIDGroupForStudent(AnsiString IDStudStr);
        int __fastcall GetIDSpecForStudent(AnsiString IDStudStr); 
        void __fastcall UpdateAllPayFactsForStudents(int IDStud,int IDGroupOld,int IDGroupNew);
        void __fastcall UpdateAllProgressForStudents(int IDStud,int IDSpecOld,int IDSpecNew);
     void __fastcall ButtonAutoEnteringClick(TObject *Sender);
  void __fastcall ButtonSaveDafaultDateClick(TObject *Sender);
private:	// User declarations
     bool __fastcall IsExistsZnum(AnsiString& _FIOStr,int& _groupid);
public:		// User declarations
        __fastcall TFormSelected(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSelected *FormSelected;
//---------------------------------------------------------------------------
#endif
