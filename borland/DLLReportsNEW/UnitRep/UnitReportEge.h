//---------------------------------------------------------------------------

#ifndef UnitReportEgeH
#define UnitReportEgeH
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
#include <map>
//---------------------------------------------------------------------------
class TFormReportEge : public TForm
{
__published:	// IDE-managed Components

        TImageList *ImageList;
        TToolBar *ToolBar;
        TToolButton *ToolBtnPreview;
        TToolButton *ToolBtnClose;
        TZConnection *ZMySqlDatabase;
        TZQuery *ZMySqlQuery;
      //  void __fastcall InitReportQuery(void);
        void __fastcall CreateWordDocument(void);
        void __fastcall ToolBtnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ToolBtnPreviewClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
        TFormListGroup *pFormList;
public:		// User declarations
        __fastcall TFormReportEge(TComponent* Owner);

public:
    struct name_id
    {
      AnsiString name;
      AnsiString id;
      name_id(AnsiString _name, AnsiString _id) : name(_name), id(_id) {}
    };
    struct stud_data
    {
      AnsiString name;
      AnsiString id;
      AnsiString orig;   // оригинал свидетельства
      AnsiString number; // номер свидетельства
      stud_data(AnsiString _name, AnsiString _id, AnsiString _orig, AnsiString _number)
      : name(_name), id(_id), orig(_orig), number(_number) {}
    };

    // получаем предметы еге для группы
    std::vector<name_id> __fastcall GetDiscip(AnsiString grp_id);
    // получаем студентов группы
    std::vector<stud_data> __fastcall GetStudent(AnsiString grp_id);
    // заполняем реальными оценками
    void __fastcall SetRealBall(AnsiString grp_id, std::map<AnsiString, std::map<AnsiString, AnsiString> >& stud_dis_ball);


};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportEge *FormReportEge;
//---------------------------------------------------------------------------
#endif
