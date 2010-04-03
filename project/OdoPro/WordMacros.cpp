#include "stdafx.h"
#include "WordMacros.h"
#include "json-aux.h"
#include <string>

WordMacros::WordMacros(void):
	macros(""), count_macros(0), cur_mac(""), lock(false), tamplate("")
{
}

WordMacros::~WordMacros(void)
{
}

void WordMacros::RunMacros()
{
}

void WordMacros::BeginMacros()
{
	std::string name = std::string("M") + std::string(aux::itoa(++count_macros));
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
void WordMacros::PageSetup(const std::string& param)
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
void WordMacros::SelectionParagraphFormat(const std::string& param)
{
	macros += "Selection.ParagraphFormat." + param + "\n";
	IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::SelectionFont(const std::string& param)
{
	macros += "Selection.Font." + param + "\n";
	IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::SelectionText(const std::string& text)
{
	if (text.empty())
		return;
	if (text == "vbTab")
	{
		macros += "Selection.TypeText Text:=vbTab\n";
		return;
	}
	int end = text.find("\n");
	if (end != std::string::npos)
	{
		if (end > 1)
			macros += "Selection.TypeText Text:=\"" + text.substr(1, end-1) + "\"\n";
		SelectionTypeParagraph();
		SelectionText(text.substr(end + 1, text.Length()));
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
	macros += "ActiveDocument.Tables.Add Range:=Selection.Range, NumRows:=" + aux::itoa(NumRow) + ", NumColumns:= _  \n";
	macros += "    " +  aux::itoa(NumColumns) + ", DefaultTableBehavior:=wdWord9TableBehavior, AutoFitBehavior:= _   \n";
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
void WordMacros::TablesColumns(const int& NumTab, const int& NumCol,const std::string& param)
{
	macros += "ActiveDocument.Tables(" + aux::itoa(NumTab) + ").Columns(" + aux::itoa(NumCol) + ")." + param + "\n";
	IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::InsertLine(const std::string& line)
{
	macros += line + "\n";
	IsLarge();
}
//---------------------------------------------------------------------------
void WordMacros::TablesCell(const int& NumTab, const int& x, const int& y,const std::string& param)
{
	macros += "ActiveDocument.Tables(" + aux::itoa(NumTab) + ").Cell(" + aux::itoa(x) + "," + aux::itoa(y) + ")." + param + "\n";
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
		InsertLine("Application.Run \"M" + aux::itoa(i) +"\"");
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
void WordMacros::FilterText(std::string& text)
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
void WordMacros::LoadTamplate(std::string& name)
{
	tamplate = name;
	// получаем текущее положение
	char path_buf[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];

	GetModuleFileName(NULL,path_buf,sizeof(path_buf)-1);
	_splitpath(path_buf,drive,dir,0,0);
	_makepath(path_buf,drive,dir,0,0);

	std::string path_host_template = std::string(drive) + std::string(dir) + tamplate;
	if (FileExists(path_host_template))
		tamplate = path_host_template;
	else
		tamplate = "c:\\" + tamplate;
}
