//---------------------------------------------------------------------------


#pragma hdrstop

#include "MacroWord.h"
#include <ComObj.hpp>

Variant WordProxy;

WordMacros::WordMacros():macros(""),count_macros(0),cur_mac(""),lock(false),tamplate("")
{
 // проверяем есть ли загруженный ворд, если нет то создаем
 // необходимо для ускорения вывода отчетов

 /*   try
    {
        if (WordProxy.IsEmpty())
            Variant::GetActiveObject("Word.Application");
    }
    catch(...)
    {
        try
        {
            WordProxy = Variant::CreateObject("Word.Application");
        }
        catch (...)
        {
            Application->MessageBox("Невозможно открыть Microsoft Word!"  \
                "Возможно Word не установлен на компьютере.", "Ошибка", MB_OK + MB_ICONERROR);
        }
    }
    */
}
//---------------------------------------------------------------------------
void WordMacros::RunMacros()
{
    Variant WordApp;
    Variant WordDoc;
    Variant VBP;


    // если Word запущен - подключиться к нему
    try {
        WordApp = Variant::CreateObject("Word.Application");

    } catch (...) {
        Application->MessageBox("Невозможно открыть Microsoft Word!  \
            Возможно Word не установлен на компьютере.", "Ошибка", MB_OK + MB_ICONERROR);
    }
    
    try {

        WordDoc = WordApp.OlePropertyGet("Documents").OleFunction("Add", tamplate.c_str());
        //WordDoc = WordApp.OlePropertyGet("ActiveDocument");
        VBP = WordDoc.OlePropertyGet("VBProject");
        Variant vbc = VBP.OlePropertyGet("VBComponents");
        Variant item = vbc.OleFunction("Item",1);
        Variant cm = item.OlePropertyGet("CodeModule");
        CreateRuningMacros();
        cm.OleProcedure("AddFromString",cur_mac.c_str());
        AnsiString name = "M" + IntToStr(count_macros);
        WordApp.OleProcedure("Run",name.c_str());
        WordApp.OlePropertySet("Visible",true);
        macros = "";
        count_macros = 0;
        cur_mac = "";
        tamplate = "";
    } catch(...) {
        Application->MessageBox("Ошибка открытия документа Microsoft Word!",
                                         "Ошибка",MB_OK+MB_ICONERROR);
        WordApp.OlePropertySet("Visible",true);
    }
    return;
}
//---------------------------------------------------------------------------
void WordMacros::BeginMacros()
{
    AnsiString name = "M" + IntToStr(++count_macros);
    macros += "Sub " + name + "()                               \n";
    macros += "'                                                \n";
    macros += "'                                                \n";
    macros += "' Макрос записан 23.02.2007   Morfey             \n";
    macros += "'                                                \n";
}
//---------------------------------------------------------------------------
void WordMacros::EndMacros()
{
    macros += "End Sub                                          \n";

    cur_mac += macros;
    macros = "";
}
//---------------------------------------------------------------------------
void WordMacros::PageSetup(const AnsiString& param)
{
    macros += "With ActiveDocument.Styles(wdStyleNormal).Font   \n";
    macros += "    If .NameFarEast = .NameAscii Then            \n";
    macros += "        .NameAscii = \"\"                        \n";
    macros += "    End If                                       \n";
    macros += "    .NameFarEast = \"\"                          \n";
    macros += "End With                                         \n";
    macros += "With ActiveDocument.PageSetup                    \n";
    macros += "    .LineNumbering.Active = False                \n";
    macros += "    .Orientation = wdOrientLandscape             \n";
    macros += "    .TopMargin = CentimetersToPoints(1.5)        \n";
    macros += "    .BottomMargin = CentimetersToPoints(3)       \n";
    macros += "    .LeftMargin = CentimetersToPoints(2)         \n";
    macros += "    .RightMargin = CentimetersToPoints(2)        \n";
    macros += "    .Gutter = CentimetersToPoints(0)             \n";
    macros += "    .HeaderDistance = CentimetersToPoints(1.25)  \n";
    macros += "    .FooterDistance = CentimetersToPoints(1.25)  \n";
    macros += "    .PageWidth = CentimetersToPoints(29.7)       \n";
    macros += "    .PageHeight = CentimetersToPoints(21)        \n";
    macros += "    .FirstPageTray = wdPrinterDefaultBin         \n";
    macros += "    .OtherPagesTray = wdPrinterDefaultBin        \n";
    macros += "    .SectionStart = wdSectionNewPage             \n";
    macros += "    .OddAndEvenPagesHeaderFooter = False         \n";
    macros += "    .DifferentFirstPageHeaderFooter = False      \n";
    macros += "    .VerticalAlignment = wdAlignVerticalTop      \n";
    macros += "    .SuppressEndnotes = False                    \n";
    macros += "    .MirrorMargins = False                       \n";
    macros += "    .TwoPagesOnOne = False                       \n";
    macros += "    .BookFoldPrinting = False                    \n";
    macros += "    .BookFoldRevPrinting = False                 \n";
    macros += "    .BookFoldPrintingSheets = 1                  \n";
    macros += "    .GutterPos = wdGutterPosLeft                 \n";
    macros += "End With                                         \n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::SelectionParagraphFormat(const AnsiString& param)
{
    macros += "Selection.ParagraphFormat." + param + "\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::SelectionFont(const AnsiString& param)
{
    macros += "Selection.Font." + param + "\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::SelectionText(const AnsiString& text)
{
    if (text.IsEmpty())
        return;
    if (text == "vbTab")
    {
        macros += "Selection.TypeText Text:=vbTab\n";
        return;
    }
    int end = text.Pos("\n");
    if (end != 0)
    {
        if (end > 1)
            macros += "Selection.TypeText Text:=\"" + text.SubString(1, end-1) + "\"\n";
        SelectionTypeParagraph();
        SelectionText(text.SubString(end + 1, text.Length()));
    }
    else
        macros += "Selection.TypeText Text:=\"" + text + "\"\n";

    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::SelectionTypeParagraph()
{
    macros += "Selection.TypeParagraph\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::TablesAdd(const int& NumRow, const int& NumColumns)
{
    macros += "ActiveDocument.Tables.Add Range:=Selection.Range, NumRows:=" + IntToStr(NumRow) + ", NumColumns:= _  \n";
    macros += "    " +  IntToStr(NumColumns) + ", DefaultTableBehavior:=wdWord9TableBehavior, AutoFitBehavior:= _   \n";
    macros += "    wdAutoFitFixed\n";
    macros += "With Selection.Tables(1)\n";
   /* macros += "    If .Style <> \"Сетка таблицы\" Then\n";
    macros += "        .Style = \"Сетка таблицы\"\n";
    macros += "    End If\n";
   */ macros += "    .ApplyStyleHeadingRows = True\n";
    macros += "    .ApplyStyleLastRow = True\n";
    macros += "    .ApplyStyleFirstColumn = True\n";
    macros += "    .ApplyStyleLastColumn = True\n";
    macros += "End With\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::TablesColumns(const int& NumTab, const int& NumCol,const AnsiString& param)
{
    macros += "ActiveDocument.Tables(" + IntToStr(NumTab) + ").Columns(" + IntToStr(NumCol) + ")." + param + "\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::InsertLine(const AnsiString& line)
{
    macros += line + "\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::TablesCell(const int& NumTab, const int& x, const int& y,const AnsiString& param)
{
    macros += "ActiveDocument.Tables(" + IntToStr(NumTab) + ").Cell(" + IntToStr(x) + "," + IntToStr(y) + ")." + param + "\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::IsLarge()
{
    if (lock)
        return;

    if (macros.Length() < 20000)
        return;

    EndMacros();
    BeginMacros();
}
//---------------------------------------------------------------------------
void WordMacros::CreateRuningMacros()
{
    BeginMacros();
    for (int i = 1; i < count_macros; i++)
        InsertLine("Application.Run \"M" + IntToStr(i) +"\"");
    EndMacros();

}
//---------------------------------------------------------------------------
void WordMacros::TableStyle(const EStyleBorder& style)
{
    if (style == StyleNone)
    {
        macros += "With Selection.Tables(1)\n";
        macros += "    .Borders(wdBorderLeft).LineStyle = wdLineStyleNone\n";
        macros += "    .Borders(wdBorderRight).LineStyle = wdLineStyleNone\n";
        macros += "    .Borders(wdBorderTop).LineStyle = wdLineStyleNone\n";
        macros += "    .Borders(wdBorderBottom).LineStyle = wdLineStyleNone\n";
        macros += "    .Borders(wdBorderHorizontal).LineStyle = wdLineStyleNone\n";
        macros += "    .Borders(wdBorderVertical).LineStyle = wdLineStyleNone\n";
        macros += "    .Borders(wdBorderDiagonalDown).LineStyle = wdLineStyleNone\n";
        macros += "    .Borders(wdBorderDiagonalUp).LineStyle = wdLineStyleNone\n";
        macros += "    .Borders.Shadow = False\n";
        macros += "End With\n";
        macros += "With Options\n";
        macros += "    .DefaultBorderLineStyle = wdLineStyleSingle\n";
        macros += "    .DefaultBorderLineWidth = wdLineWidth050pt\n";
        macros += "    .DefaultBorderColor = wdColorAutomatic\n";
        macros += "End With\n";
    }
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::FilterText(AnsiString& text)
{
    TReplaceFlags flags;
    flags << rfReplaceAll << rfIgnoreCase;
    text = StringReplace(text, "\"", "\"\"", flags);
}
//---------------------------------------------------------------------------
void WordMacros::Lock()
{
    lock = true;
}
//---------------------------------------------------------------------------
void WordMacros::UnLock()
{
    lock = false;
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::LoadTamplate(AnsiString& name)
{
     tamplate = name;
         // получаем текущее положение
    char path_buf[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];

    GetModuleFileName(NULL,path_buf,sizeof(path_buf)-1);
    _splitpath(path_buf,drive,dir,0,0);
    _makepath(path_buf,drive,dir,0,0);

    AnsiString path_host_template = AnsiString(drive) + AnsiString(dir) + tamplate;
    if (FileExists(path_host_template))
       tamplate = path_host_template;
    else
       tamplate = "c:\\" + tamplate;
}
#pragma package(smart_init)
