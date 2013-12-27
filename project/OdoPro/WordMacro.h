#pragma once
#include "string_t.h"

class WordMacros
{
private:
	void IsLarge();
	void CreateRuningMacros();
public:
	enum EStyleBorder{StyleNone};
public:
	WordMacros();

	void LoadTamplate(string_t& name);
	void BeginMacros();
	void EndMacros();
	void RunMacros();
	//---------------------------------------------------------------------------
	void PageSetup(const string_t& param);
	void SelectionParagraphFormat(const string_t& param);
	void SelectionFont(const string_t& param);
	void SelectionText(const string_t& text);
	void SelectionTypeParagraph();
	void InsertLine(const string_t& line);
	//---------------------------------------------------------------------------
	void TablesAdd(int rowCount, int colCount);
	void TablesColumns(int tableNum, int colNum, const string_t& param);
	void TablesCell(int tableNum, int x, int y, const string_t& param);
	void TableStyle(const EStyleBorder& style);
	//---------------------------------------------------------------------------
	void FilterText(string_t& text);
	void Lock();    // реализуется для избежания некоторых глюков IsLarge
	void UnLock();  // 

public:
	string_t m_Macros;
	string_t m_CurMacros;
	string_t m_Tamplate;
	int      m_MacroCounter;
	bool     m_LargeLock;  // блокирует работу IsLarge

};