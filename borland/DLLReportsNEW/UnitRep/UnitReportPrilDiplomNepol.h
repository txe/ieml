//---------------------------------------------------------------------------

#ifndef UnitReportPrilDiplomNepolH
#define UnitReportPrilDiplomNepolH

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

struct m_SDiscip
{
  int iddiscip;
  AnsiString title;
  int time;
  int estimation;
  int num_semestr;
};
typedef std::vector<m_SDiscip*> m_vecDiscip;

//---------------------------------------------------------------------------
class TFormReportPrilDiplomNepol : public TForm
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
                AnsiString& OutYear, AnsiString& spec, AnsiString& specializ, AnsiString& Qualific, AnsiString& Sex, AnsiString& lang,
                AnsiString&  NumDiplomS, AnsiString& RegNumS, AnsiString& DataVidachiS, AnsiString& DataQualificS);
        void __fastcall InitPracticAndItog(AnsiString& pract, float& itog_oc, AnsiString& VIP);
        void __fastcall InitVectorsOfDisciplines(m_vecDiscip& vecComm, m_vecDiscip& vecCurWorks, m_vecDiscip& vecCurProj,
                m_vecDiscip& vecOther);
private:	// User declarations
        int idstudent;
        TFormDataForDiplom* pFormData;
public:		// User declarations
        __fastcall TFormReportPrilDiplomNepol(TComponent* Owner);
        void bla();
        void __fastcall SetIDStudent(int _IDStudent);
        int __fastcall GetIDStudent(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportPrilDiplomNepol *FormReportPrilDiplomNepol;
//---------------------------------------------------------------------------
#endif
