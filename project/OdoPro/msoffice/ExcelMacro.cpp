#include "StdAfx.h"
#include "ExcelMacro.h"

//-------------------------------------------------------------------------
ExcelMacro::ExcelMacro(void) : m_MacroCounter(0)
{
}
//-------------------------------------------------------------------------
ExcelMacro::~ExcelMacro(void)
{
}
//-------------------------------------------------------------------------
void ExcelMacro::RunMacros(string_t tempFile)
{
  Excel::_ApplicationPtr app;
  if (FAILED(app.CreateInstance("Excel.Application")))
  {
    string_t msg = "Невозможно запустить Microsoft Excel. Возможно он не установлен на компьютере.";
    MessageBox(::GetActiveWindow(), msg , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    return;
  }
  try
  {
    _bstr_t tamp = (const wchar_t*)tempFile;
    Excel::_WorkbookPtr     doc  = app->Workbooks->Open(tamp);
    VBIDE::_VBProjectPtr    vbp  = doc->VBProject;
    VBIDE::_VBComponentsPtr vbc  = vbp->VBComponents;
    VBIDE::_VBComponentPtr  item = vbc->Item(2);
    VBIDE::_CodeModulePtr   cm   = item->CodeModule;

    string_t sheetName = (const wchar_t*)item->Name;
    CreateRunningMacros(sheetName);
    string_t name = sheetName + ".M" + string_t(aux::itoa(m_MacroCounter));

    cm->AddFromString((const wchar_t*)m_CurMacros);
    app->Run((const wchar_t*)name);
  }
  catch (_com_error & e)
  {
    _bstr_t desc = e.Description();
    string_t msg = "Невозможно открыть документ Microsoft Excel:\n" + tempFile + "\n";
    if (desc.length())
      msg += std::wstring(desc);
    MessageBox(::GetActiveWindow(), msg , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL); 
  }
  catch (...)
  {
    string_t msg = "Невозможно открыть документ Microsoft Excel:\n" + tempFile + "\nили проблема с макросами или не смогли открыть шаблон файла";
    MessageBox(::GetActiveWindow(), msg , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
  }
  app->PutVisible(0, VARIANT_TRUE);

  m_Macros = "";
  m_MacroCounter = 0;
  m_CurMacros = "";
}
//-------------------------------------------------------------------------
void ExcelMacro::BeginMacros()
{
  string_t name = string_t("M") + aux::itoa(++m_MacroCounter);
  m_Macros += "Sub " + name + "()                               \n";
  m_Macros += "' Johnny Txe                                     \n";
}
//-------------------------------------------------------------------------
void ExcelMacro::EndMacros()
{
  m_Macros += "End Sub                                          \n";

  m_CurMacros += m_Macros;
  m_Macros = "";
}
//---------------------------------------------------------------------------
void ExcelMacro::InsertLine(const string_t& line)
{
  m_Macros += line + "\n";
  IsLarge();
}
//-------------------------------------------------------------------------
void ExcelMacro::Select(const string_t& range)
{
  m_Macros += "Range(\"" + range +"\").Select\n";
  IsLarge();
}
//-------------------------------------------------------------------------
void ExcelMacro::Formula(const string_t& text)
{
  m_Macros += "ActiveCell.FormulaR1C1 = \"" + text + "\"\n";
  IsLarge();
}
//-------------------------------------------------------------------------
void ExcelMacro::Text(const string_t& text)
{
  m_Macros += "ActiveCell.Text = \"" + text + "\"\n";
  IsLarge();
}
//---------------------------------------------------------------------------
void ExcelMacro::MergeCell()
{
  m_Macros += "   With Selection                         \n";
  //    m_Macros += "    .HorizontalAlignment = xlGeneral      \n";
  m_Macros += "    .VerticalAlignment = xlCenter         \n";
  m_Macros += "    .WrapText = True                      \n";
  //    m_Macros += "    .Orientation = 0                      \n";
  //    m_Macros += "    .AddIndent = False                    \n";
  //    m_Macros += "    .IndentLevel = 0                      \n";
  //    m_Macros += "    .ShrinkToFit = False                  \n";
  //    m_Macros += "    .ReadingOrder = xlContext             \n";
  m_Macros += "    .MergeCells = True                    \n";
  m_Macros += "End With                                  \n";

  IsLarge();
}
//---------------------------------------------------------------------------
void ExcelMacro::IsLarge()
{
  if (m_Macros.size() < 20000)
    return;

  EndMacros();
  BeginMacros();
}
//---------------------------------------------------------------------------
void ExcelMacro::CreateRunningMacros(string_t sheetName)
{
  BeginMacros();
  for (int i = 1; i < m_MacroCounter; i++)
    InsertLine("Application.Run \"" + sheetName + ".M" + string_t(aux::itoa(i)) + "\"");
  EndMacros();

}
//---------------------------------------------------------------------------
void ExcelMacro::FilterText(string_t& text)
{
  text.replaceAll("\"", "\"\"");    
}
//---------------------------------------------------------------------------
void ExcelMacro::Cells(int row1, int col1, int row2, int col2, const string_t& text)
{
  m_Macros += "Range(Cells(" + string_t(aux::itow(row1)) + ", " + string_t(aux::itow(col1))+ "), "
    "Cells(" + string_t(aux::itow(row2)) + ", " + string_t(aux::itow(col2)) + "))." + text + "\n";
  IsLarge();
}
//-------------------------------------------------------------------------
void ExcelMacro::Border(string_t& range)
{
  m_Macros += "Range(\"" + range + "\").Select\n"
              "Selection.Borders(xlDiagonalDown).LineStyle = xlNone\n"
              "Selection.Borders(xlDiagonalUp).LineStyle = xlNone\n"
              "With Selection.Borders(xlEdgeLeft)\n"
              "   .LineStyle = xlContinuous\n"
              "   .Weight = xlThin\n"
              "   .ColorIndex = xlAutomatic\n"
              "End With\n"
              "With Selection.Borders(xlEdgeTop)\n"
              "    .LineStyle = xlContinuous\n"
              "    .Weight = xlThin\n"
              "    .ColorIndex = xlAutomatic\n"
              "End With\n"
              "With Selection.Borders(xlEdgeBottom)\n"
              "    .LineStyle = xlContinuous\n"
              "    .Weight = xlThin\n"
              "    .ColorIndex = xlAutomatic\n"
              "End With\n"
              "With Selection.Borders(xlEdgeRight)\n"
              "    .LineStyle = xlContinuous\n"
              "    .Weight = xlThin\n"
              "    .ColorIndex = xlAutomatic\n"
              "End With\n"
              "With Selection.Borders(xlInsideVertical)\n"
              "    .LineStyle = xlContinuous\n"
              "    .Weight = xlThin\n"
              "    .ColorIndex = xlAutomatic\n"
              "End With\n"
              "With Selection.Borders(xlInsideHorizontal)\n"
              "    .LineStyle = xlContinuous\n"
              "    .Weight = xlThin\n"
              "    .ColorIndex = xlAutomatic\n"
              "End With\n";
  IsLarge();
}