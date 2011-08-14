//---------------------------------------------------------------------------

#ifndef UnitReportPrilDiplomH
#define UnitReportPrilDiplomH

#include "ZAbstractDataset.hpp"
#include "ZAbstractRODataset.hpp"
#include "ZConnection.hpp"
#include "ZDataset.hpp"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <ImgList.hpp>
#include <OleServer.hpp>
#include <ToolWin.hpp>
#include "Word_2K_SRVR.h"
#include "UnitDataForDiplom.h"
#include <vector>
//--------------------------------------------------------------------------
struct SDiscip
{
  int iddiscip;
  AnsiString title;
  int time;
  int estimation;
  int num_semestr;
};
typedef std::vector<SDiscip*> vecDiscip;

//---------------------------------------------------------------------------
class TFormReportPrilDiplom : public TForm
{
__published:	// IDE-managed Components
        TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
    TZConnection *ZMySqlDatabase;
    TZQuery *ZMySqlQuery;
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall CreateWordDocument(void);
        void __fastcall InitPrivateData(AnsiString& SN, AnsiString& FN, AnsiString& TN, AnsiString& BirstDate,
                AnsiString& VipQualifWork, AnsiString& PrevDoc, AnsiString& PrevDocYear, AnsiString& InYear, AnsiString& InMonth, AnsiString& InDay,
                AnsiString& OutYear, AnsiString& OutMonth, AnsiString& OutDay, AnsiString& spec, AnsiString& specializ, AnsiString& Qualific, AnsiString& Sex, AnsiString& lang,
                AnsiString&  NumDiplomS, AnsiString& RegNumS, AnsiString& DataVidachiS, AnsiString& DataQualificS, AnsiString& Direct);
        void __fastcall InitPracticAndItog(AnsiString& pract, float& itog_oc, AnsiString& VIP);
        void __fastcall InitVectorsOfDisciplines(vecDiscip& vecComm, vecDiscip& vecCurWorks, vecDiscip& vecCurProj,
                vecDiscip& vecOther);
private:	// User declarations
        int idstudent;
        TFormDataForDiplom* pFormData;
public:		// User declarations
        __fastcall TFormReportPrilDiplom(TComponent* Owner);
        void __fastcall SetIDStudent(int _IDStudent);
        int __fastcall GetIDStudent(void);
        AnsiString __fastcall ExamForUr() ;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportPrilDiplom *FormReportPrilDiplom;
//---------------------------------------------------------------------------
#endif
