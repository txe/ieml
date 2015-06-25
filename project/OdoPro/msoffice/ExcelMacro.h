#pragma once
#include "..\string_t.h"


class ExcelMacro
{
public:
	ExcelMacro(void);
	~ExcelMacro(void);

private:
  void IsLarge();
  void CreateRunningMacros(string_t sheetName);

public:
  void BeginMacros();
  void EndMacros();
  void RunMacros(string_t tempFile);

  void InsertLine(const string_t& line);
  void Select(const string_t& range);
  void Formula(const string_t& text);
  void Text(const string_t& text);
  void MergeCell();
  void Cells(int row1, int col1, int row2, int col2, const string_t& text);
  void Border(string_t& range);

  void FilterText(string_t& text);

private:
  string_t m_Macros;
  string_t m_CurMacros;
  int      m_MacroCounter;
};
