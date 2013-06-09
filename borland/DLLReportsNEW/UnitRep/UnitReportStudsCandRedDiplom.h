//---------------------------------------------------------------------------

#ifndef UnitReportStudsCandRedDiplomH
#define UnitReportStudsCandRedDiplomH
//---------------------------------------------------------------------------

#include "UnitListGroups.h"
#include "ZAbstractDataset.hpp"
#include "ZAbstractRODataset.hpp"
#include "ZConnection.hpp"
#include "ZDataset.hpp"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <vector>

struct red_student
{
    AnsiString name;
    int    count_otl;
    double perc_otl;
    int    count_hor;
    double perc_hor;
    int    count_ud;
    double perc_ud;

    AnsiString avrBall;
    AnsiString znum;
};
typedef std::vector<red_student> red_students;

//---------------------------------------------------------------------------
class TFormReportStudsCandRedDiplom : public TForm
{
__published:	// IDE-managed Components

        TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
    TZConnection *ZMySqlDatabase;
    TZQuery *ZMySqlQueryStudsOfGrp;
    TZQuery *ZMySqlQuery;
    TZQuery *ZMySqlQueryAvrgBall;
    TZQuery *ZMySqlQueryCheckStuds;

        void __fastcall InitReportQuery(void);
        void __fastcall CreateWordDocument(void);
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall GetBallsCount(int& countUDOVL,double& percUDOVL,int& countHOR,double& percHOR,
                                      int& countOTL,double& percOTL,AnsiString& AvrgBallStr,AnsiString IDStudStr);
        int __fastcall InitStudentsForGroup(int idgroup);
        int __fastcall GetCountUsefullStudents(TStringList* listNumbers);

        red_students __fastcall  GetRedStudents(int idGroup);
        void __fastcall  ClearRedStudentsQuery();

private:	// User declarations
        TFormListGroup *pFormList;
public:		// User declarations
        __fastcall TFormReportStudsCandRedDiplom(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportStudsCandRedDiplom *FormReportStudsCandRedDiplom;
//---------------------------------------------------------------------------
#endif
