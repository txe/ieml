//---------------------------------------------------------------------------

#ifndef MacroExcelH
#define MacroExcelH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
//---------------------------------------------------------------------------

class ExcelMacros
{
private:
    AnsiString  macros;
    AnsiString  mas_macros;
    int         count_macros;
private:
    void __fastcall IsLarge();
    void __fastcall CreateRuningMacros(const AnsiString& name_item);
public:
    ExcelMacros();
    void __fastcall BeginMacros();
    void __fastcall EndMacros();
    void __fastcall RunMacros(const AnsiString& tamplate);

    void __fastcall InsertLine(const AnsiString& line);
    void __fastcall Select(const AnsiString& range);
    void __fastcall Formula(const AnsiString& text);
    void __fastcall Text(const AnsiString& text);
    void __fastcall MergeCell();
    void __fastcall Cells(const int& row1, const int& col1,const int& row2, const int& col2, const AnsiString& text);

    void __fastcall FilterText(AnsiString& text);
};
//---------------------------------------------------------------------------
#endif
