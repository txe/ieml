//---------------------------------------------------------------------------

#ifndef MacroWordH
#define MacroWordH

#include <vcl.h>
#include <windows.h>
#include <deque>

class WordMacros
{
private:
    AnsiString macros;
    AnsiString cur_mac;
    AnsiString tamplate;
 //   std::deque<AnsiString> names_macros;
    int count_macros;
    bool lock;  // блокирует использование ф-ии IsLarge
private:
    void IsLarge();
    void CreateRuningMacros();
public:
    enum EStyleBorder{StyleNone};
public:
    WordMacros();

    void LoadTamplate(AnsiString& name);
    void BeginMacros();
    void EndMacros();
    void RunMacros();
//---------------------------------------------------------------------------
    void PageSetup(const AnsiString& param);
    void SelectionParagraphFormat(const AnsiString& param);
    void SelectionFont(const AnsiString& param);
    void SelectionText(const AnsiString& text);
    void SelectionTypeParagraph();
    void InsertLine(const AnsiString& line);
//---------------------------------------------------------------------------
    void TablesAdd(const int& NumRow, const int& NumColumns);
    void TablesColumns(const int& NumTab, const int& NumCol,const AnsiString& param);
    void TablesCell(const int& NumTab, const int& x, const int& y,const AnsiString& param);
    void TableStyle(const EStyleBorder& style);
//---------------------------------------------------------------------------
    void FilterText(AnsiString& text);
    void Lock();    // реализуется для блокировки функции IsLarge, что бы избежать
    void UnLock();  // некотрых глюков
};
//---------------------------------------------------------------------------
#endif
