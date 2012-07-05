//---------------------------------------------------------------------------
#pragma hdrstop
#include "MacroExcel.h"
#include <OleServer.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)

Variant ExcelProxy;

ExcelMacros::ExcelMacros():macros(""),mas_macros(""),count_macros(0)
{
 // проверяем есть ли загруженный ексель, если нет то создаем
 // необходимо для ускорения вывода отчетов
/*
    try
    {
        if (ExcelProxy.IsEmpty())
            Variant::GetActiveObject("Excel.Application");
    }
    catch(...)
    {
        try
        {
            ExcelProxy = Variant::CreateObject("Excel.Application");
        }
        catch (...)
        {
            Application->MessageBox("Невозможно открыть Microsoft Excel!"  \
                "Возможно Excel не установлен на компьютере.", "Ошибка", MB_OK + MB_ICONERROR);
        }
    }
    */
}
//---------------------------------------------------------------------------
void __fastcall ExcelMacros::RunMacros(const AnsiString& tamplate)
{
    Variant ExcelApp;
    Variant ExcelBook;
    Variant VBP;

    try {
        ExcelApp = Variant::CreateObject("Excel.Application");
    } catch(...) {
        Application->MessageBox("Невозможно открыть Microsoft Excel!"  \
            "Возможно Excel не установлен на компьютере.", "Ошибка", MB_OK + MB_ICONERROR);
        return;
    }

    try {

        ExcelApp.OlePropertyGet("WorkBooks").OleProcedure("Add", tamplate.c_str());
        ExcelBook = ExcelApp.OlePropertyGet("WorkBooks", 1);
        VBP = ExcelBook.OlePropertyGet("VBProject");
        Variant vbc = VBP.OlePropertyGet("VBComponents");
        Variant item = vbc.OleFunction("Item",2);
        Variant cm = item.OlePropertyGet("CodeModule");
        CreateRuningMacros(item.OlePropertyGet("Name"));
        AnsiString name = item.OlePropertyGet("Name") + AnsiString(".E") + IntToStr(count_macros);
        cm.OleProcedure("AddFromString",mas_macros.c_str());
        ExcelApp.OlePropertySet("Visible",true);
        ExcelApp.OleProcedure("Run",name.c_str());
        macros = "";
        count_macros = 0;
        mas_macros = "";
    } catch(...) {
        Application->MessageBox("Ошибка открытия документа Microsoft Excel!\n"
                                "Возможно в макросах Excel не установлены следующие параметры:\n"
                                " - низкий уровень безопасности\n",
                                         "Ошибка",MB_OK+MB_ICONERROR);
        ExcelApp.OlePropertySet("Visible",true);
    }
    return;
}
//---------------------------------------------------------------------------
void __fastcall ExcelMacros::BeginMacros()
{
    AnsiString name = "E" + IntToStr(++count_macros);
    macros += "Sub " + name + "()                               \n";
    macros += "'                                                \n";
    macros += "'                                                \n";
    macros += "' Макрос записан 03.03.2007     Morfey           \n";
    macros += "'                                                \n";
    IsLarge();
}
//---------------------------------------------------------------------------
void __fastcall ExcelMacros::EndMacros()
{
    macros += "End Sub                                          \n";
    mas_macros += macros;
    macros = "";
}
//---------------------------------------------------------------------------
void __fastcall ExcelMacros::InsertLine(const AnsiString& line)
{
    macros += line + "\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void __fastcall ExcelMacros::Select(const AnsiString& range)
{
    macros += "Range(\"" + range +"\").Select\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void __fastcall ExcelMacros::Formula(const AnsiString& text)
{
    macros += "ActiveCell.FormulaR1C1 = \"" + text + "\"\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void __fastcall ExcelMacros::Text(const AnsiString& text)
{
    macros += "ActiveCell.Text = \"" + text + "\"\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void __fastcall ExcelMacros::MergeCell()
{
    macros += "   With Selection                         \n";
//    macros += "    .HorizontalAlignment = xlGeneral      \n";
    macros += "    .VerticalAlignment = xlCenter         \n";
    macros += "    .WrapText = True                      \n";
//    macros += "    .Orientation = 0                      \n";
//    macros += "    .AddIndent = False                    \n";
//    macros += "    .IndentLevel = 0                      \n";
//    macros += "    .ShrinkToFit = False                  \n";
//    macros += "    .ReadingOrder = xlContext             \n";
    macros += "    .MergeCells = True                    \n";
    macros += "End With                                  \n";

    IsLarge();
}

//---------------------------------------------------------------------------
void __fastcall ExcelMacros::IsLarge()
{
    if (macros.Length() < 20000)
        return;

    EndMacros();
    BeginMacros();
}
//---------------------------------------------------------------------------
void __fastcall ExcelMacros::CreateRuningMacros(const AnsiString& name_item)
{
    BeginMacros();
    for (int i = 1; i < count_macros; i++)
        InsertLine("Application.Run \"" + name_item +".E" + IntToStr(i) +"\"");
    EndMacros();

}
//---------------------------------------------------------------------------
void __fastcall ExcelMacros::FilterText(AnsiString& text)
{
    TReplaceFlags flags;
    flags << rfReplaceAll << rfIgnoreCase;
    text = StringReplace(text, "\"", "\"\"", flags);    
}
//---------------------------------------------------------------------------
void __fastcall ExcelMacros::Cells(const int& row1, const int& col1,const int& row2, const int& col2, const AnsiString& text)
{
    macros += "Range(Cells(" + IntToStr(row1) + ", " + IntToStr(col1)+ "), "
        "Cells(" + IntToStr(row2) + ", " + IntToStr(col2)+ "))." + text + "\n";
    IsLarge();
}

//---------------------------------------------------------------------------



