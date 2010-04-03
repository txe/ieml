#pragma once
#include <string>

class WordMacros
{
public:
	WordMacros(void);
	~WordMacros(void);
private:
	std::string	macros;
    std::string	cur_mac;
    std::string	tamplate;
    int			count_macros;
    bool		lock;  // блокирует использование ф-ии IsLarge
public:
	private:
    void IsLarge();
    void CreateRuningMacros();
public:
    enum EStyleBorder{StyleNone};

    void LoadTamplate(std::string& name);
    void BeginMacros();
    void EndMacros();
    void RunMacros();
//---------------------------------------------------------------------------
    void PageSetup(const std::string& param);
    void SelectionParagraphFormat(const std::string& param);
    void SelectionFont(const std::string& param);
    void SelectionText(const std::string& text);
    void SelectionTypeParagraph();
    void InsertLine(const std::string& line);
//---------------------------------------------------------------------------
    void TablesAdd(const int& NumRow, const int& NumColumns);
    void TablesColumns(const int& NumTab, const int& NumCol,const std::string& param);
    void TablesCell(const int& NumTab, const int& x, const int& y,const std::string& param);
    void TableStyle(const EStyleBorder& style);
//---------------------------------------------------------------------------
    void FilterText(std::string& text);
    void Lock();    // реализуется для блокировки функции IsLarge, что бы избежать
    void UnLock();  // некотрых глюков
};
