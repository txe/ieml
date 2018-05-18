//---------------------------------------------------------------------------

#ifndef UnitReportUchebKartStudH
#define UnitReportUchebKartStudH
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <vector>
//---------------------------------------------------------------------------

struct Progress
{
    AnsiString  dis_fulltitle;
    AnsiString  dis_shorttitle;
    int         numplansemestr;
    AnsiString  count_hours;
    int         idclass;
    int         estimation;
    //--------------------------
    AnsiString  title;
    int         count_line;
    AnsiString  hours[12];
};

struct PosPlanSemestr
{
    AnsiString  col;
    int         row;
};

//---------------------------------------------------------------------------
class TFormReportUchebKartStud : public TForm
{

__published:	// IDE-managed Components
    TToolBar *ToolBar;
    TToolButton *ToolBtnPreview;
    TToolButton *ToolBtnClose;
    TImageList *ImageList;
    void __fastcall ToolBtnCloseClick(TObject *Sender);
    void __fastcall ToolBtnPreviewClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
    int         idstudent;
    int         idgroup;
    std::vector<Progress*>  mas_ochenka;
    PosPlanSemestr          pos_semestr[12];
public:		// User declarations
    __fastcall TFormReportUchebKartStud(TComponent* Owner);
    void    __fastcall  SetIDStudent(int _IDGroup);
    int     __fastcall  GetIDStudent(void);
    void    __fastcall  SetIDGroup(int _IDGroup);
    int     __fastcall  GetIDGroup(void);
    void    __fastcall  GetOchenkiStudenta(void); // оценки по дисциплинам и тд
    // информац о студент и тп
    void    __fastcall  GetInfoStudent(AnsiString& NumZ, AnsiString& SN, AnsiString& FN, AnsiString& TN,
        AnsiString& Adres, AnsiString& NumPricas, AnsiString& InDate, AnsiString& spec,
        AnsiString& VipQualifWork, AnsiString& OutDate, AnsiString& Qualific,
        AnsiString& nGak, AnsiString perevodInfo[5], AnsiString& direct, AnsiString& profil);
    void    __fastcall  CreateWordDocument(void);
    int GetCountLine(const AnsiString& text, const int& max_len);
    AnsiString GetDateAsString(const AnsiString& str, bool isYear=true);
    void __fastcall UpdateView();
    bool __fastcall FileExists(const AnsiString& fname);
    void __fastcall StringToAudHours(AnsiString  h[21], AnsiString& str_hours);
    void __fastcall StringToPerevodInfo(AnsiString info[5], AnsiString str);
    static void GetDirectInfo(AnsiString& direct, AnsiString& profil, int specid, int directid);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormReportUchebKartStud *FormReportUchebKartStud;
//---------------------------------------------------------------------------
#endif
