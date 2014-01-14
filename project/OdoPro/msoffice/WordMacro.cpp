#include "StdAfx.h"
#include "WordMacro.h"


//---------------------------------------------------------------------------
WordMacros::WordMacros(): m_MacroCounter(0), m_LargeLock(false)
{
}
//---------------------------------------------------------------------------
void WordMacros::RunMacros(string_t tempFile)
{
	// получаем текущее положение
	char path_buf[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];

	GetModuleFileNameA(NULL, path_buf, sizeof(path_buf)-1);
	_splitpath(path_buf, drive, dir, 0, 0);
	_makepath(path_buf, drive, dir, 0, 0);

	tempFile = string_t(drive) + string_t(dir) + tempFile;


	Word::_ApplicationPtr app;
	if (FAILED(app.CreateInstance("Word.Application")))
	{
		string_t msg = "Невозможно запустить Microsoft Word. Возможно он не установлен на компьютере.";
		MessageBox(::GetActiveWindow(), msg , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	try
	{
        _variant_t tamp = (const wchar_t*)tempFile;
	    Word::_DocumentPtr      doc  = app->Documents->Add(&tamp);
	    VBIDE::_VBProjectPtr    vbp  = doc->VBProject;
	    VBIDE::_VBComponentsPtr vbc  = vbp->VBComponents;
	    VBIDE::_VBComponentPtr  item = vbc->Item(1);
	    VBIDE::_CodeModulePtr   cm   = item->CodeModule;

	    CreateRuningMacros();
        string_t name = "M" + string_t(aux::itoa(m_MacroCounter));
    	
    	
	    cm->AddFromString((const wchar_t*)m_CurMacros);
	    app->Run((const wchar_t*)name);
    }
    catch (...)
    {
        string_t msg = "Невозможно открыть документ Microsoft Word:\n" + tempFile + "\nили проблема с макросами.";
        MessageBox(::GetActiveWindow(), msg , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    }
	app->Visible = VARIANT_TRUE;

	m_Macros = "";
	m_MacroCounter = 0;
	m_CurMacros = "";
}
//---------------------------------------------------------------------------
void WordMacros::BeginMacros()
{
	string_t name = string_t("M") + aux::itoa(++m_MacroCounter);
    m_Macros += "Sub " + name + "()                               \n";
    m_Macros += "' Johnny Txe                                     \n";
}
//---------------------------------------------------------------------------
void WordMacros::EndMacros()
{
    m_Macros += "End Sub                                          \n";

    m_CurMacros += m_Macros;
    m_Macros = "";
}
//---------------------------------------------------------------------------
void WordMacros::PageSetup(const string_t& param)
{
    m_Macros += "With ActiveDocument.Styles(wdStyleNormal).Font   \n";
    m_Macros += "    If .NameFarEast = .NameAscii Then            \n";
    m_Macros += "        .NameAscii = \"\"                        \n";
    m_Macros += "    End If                                       \n";
    m_Macros += "    .NameFarEast = \"\"                          \n";
    m_Macros += "End With                                         \n";
    m_Macros += "With ActiveDocument.PageSetup                    \n";
    m_Macros += "    .LineNumbering.Active = False                \n";
    m_Macros += "    .Orientation = wdOrientLandscape             \n";
    m_Macros += "    .TopMargin = CentimetersToPoints(1.5)        \n";
    m_Macros += "    .BottomMargin = CentimetersToPoints(3)       \n";
    m_Macros += "    .LeftMargin = CentimetersToPoints(2)         \n";
    m_Macros += "    .RightMargin = CentimetersToPoints(2)        \n";
    m_Macros += "    .Gutter = CentimetersToPoints(0)             \n";
    m_Macros += "    .HeaderDistance = CentimetersToPoints(1.25)  \n";
    m_Macros += "    .FooterDistance = CentimetersToPoints(1.25)  \n";
    m_Macros += "    .PageWidth = CentimetersToPoints(29.7)       \n";
    m_Macros += "    .PageHeight = CentimetersToPoints(21)        \n";
    m_Macros += "    .FirstPageTray = wdPrinterDefaultBin         \n";
    m_Macros += "    .OtherPagesTray = wdPrinterDefaultBin        \n";
    m_Macros += "    .SectionStart = wdSectionNewPage             \n";
    m_Macros += "    .OddAndEvenPagesHeaderFooter = False         \n";
    m_Macros += "    .DifferentFirstPageHeaderFooter = False      \n";
    m_Macros += "    .VerticalAlignment = wdAlignVerticalTop      \n";
    m_Macros += "    .SuppressEndnotes = False                    \n";
    m_Macros += "    .MirrorMargins = False                       \n";
    m_Macros += "    .TwoPagesOnOne = False                       \n";
    m_Macros += "    .BookFoldPrinting = False                    \n";
    m_Macros += "    .BookFoldRevPrinting = False                 \n";
    m_Macros += "    .BookFoldPrintingSheets = 1                  \n";
    m_Macros += "    .GutterPos = wdGutterPosLeft                 \n";
    m_Macros += "End With                                         \n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::SelectionParagraphFormat(const string_t& param)
{
    m_Macros += "Selection.ParagraphFormat." + param + "\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::SelectionFont(const string_t& param)
{
    m_Macros += "Selection.Font." + param + "\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::SelectionText(const string_t& text)
{
    if (text.empty())
        return;
    if (text == "vbTab")
    {
        m_Macros += "Selection.TypeText Text:=vbTab\n";
        return;
    }
    int end = text.indexOf(L"\n");
    if (end != 0)
    {
        if (end > 1)
            m_Macros += "Selection.TypeText Text:=\"" + text.subString(1, end-1) + "\"\n";
        SelectionTypeParagraph();
        SelectionText(text.subString(end + 1, text.size()));
    }
    else
        m_Macros += "Selection.TypeText Text:=\"" + text + "\"\n";

    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::SelectionTypeParagraph()
{
    m_Macros += "Selection.TypeParagraph\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::TablesAdd(int rowCount, int colCount)
{
	m_Macros += "ActiveDocument.Tables.Add Range:=Selection.Range, NumRows:=" + string_t(aux::itoa(rowCount)) + ", NumColumns:= _  \n";
	m_Macros += "    " + string_t(aux::itoa(colCount)) + ", DefaultTableBehavior:=wdWord9TableBehavior, AutoFitBehavior:= _   \n";
    m_Macros += "    wdAutoFitFixed\n";
    m_Macros += "With Selection.Tables(1)\n";
   /* 
    m_Macros += "    If .Style <> \"Сетка таблицы\" Then\n";
    m_Macros += "        .Style = \"Сетка таблицы\"\n";
    m_Macros += "    End If\n";
   */ 
	m_Macros += "    .ApplyStyleHeadingRows = True\n";
    m_Macros += "    .ApplyStyleLastRow = True\n";
    m_Macros += "    .ApplyStyleFirstColumn = True\n";
    m_Macros += "    .ApplyStyleLastColumn = True\n";
    m_Macros += "End With\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::TablesColumns(int tableNum, int colNum, const string_t& param)
{
	m_Macros += "ActiveDocument.Tables(" + string_t(aux::itoa(tableNum)) + ").Columns(" + aux::itoa(colNum) + ")." + param + "\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::InsertLine(const string_t& line)
{
    m_Macros += line + "\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::Cell(int tableNum, int x, int y, const string_t& param)
{
	m_Macros += "ActiveDocument.Tables(" + string_t(aux::itoa(tableNum)) + ").Cell(" + aux::itoa(x) + "," + aux::itoa(y) + ")." + param + "\n";
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::IsLarge()
{
    if (m_LargeLock)
        return;

    if (m_Macros.size() < 20000)
        return;

    EndMacros();
    BeginMacros();
}
//---------------------------------------------------------------------------
void WordMacros::CreateRuningMacros()
{
    BeginMacros();
    for (int i = 1; i < m_MacroCounter; i++)
		InsertLine("Application.Run \"M" + string_t(aux::itoa(i)) + "\"");
    EndMacros();

}
//---------------------------------------------------------------------------
void WordMacros::TableStyle(const EStyleBorder& style)
{
    if (style == StyleNone)
    {
        m_Macros += "With Selection.Tables(1)\n";
        m_Macros += "    .Borders(wdBorderLeft).LineStyle = wdLineStyleNone\n";
        m_Macros += "    .Borders(wdBorderRight).LineStyle = wdLineStyleNone\n";
        m_Macros += "    .Borders(wdBorderTop).LineStyle = wdLineStyleNone\n";
        m_Macros += "    .Borders(wdBorderBottom).LineStyle = wdLineStyleNone\n";
        m_Macros += "    .Borders(wdBorderHorizontal).LineStyle = wdLineStyleNone\n";
        m_Macros += "    .Borders(wdBorderVertical).LineStyle = wdLineStyleNone\n";
        m_Macros += "    .Borders(wdBorderDiagonalDown).LineStyle = wdLineStyleNone\n";
        m_Macros += "    .Borders(wdBorderDiagonalUp).LineStyle = wdLineStyleNone\n";
        m_Macros += "    .Borders.Shadow = False\n";
        m_Macros += "End With\n";
        m_Macros += "With Options\n";
        m_Macros += "    .DefaultBorderLineStyle = wdLineStyleSingle\n";
        m_Macros += "    .DefaultBorderLineWidth = wdLineWidth050pt\n";
        m_Macros += "    .DefaultBorderColor = wdColorAutomatic\n";
        m_Macros += "End With\n";
    }
    IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::FilterText(string_t& text)
{
//     TReplaceFlags flags;
//     flags << rfReplaceAll << rfIgnoreCase;
//     text = StringReplace(text, "\"", "\"\"", flags);
}
//---------------------------------------------------------------------------
void WordMacros::Lock()
{
    m_LargeLock = true;
}
//---------------------------------------------------------------------------
void WordMacros::UnLock()
{
    m_LargeLock = false;
    IsLarge();
}
