#pragma once
#include "litewnd.h"
#include <htmlayout.h>
#include "string_t.h"

using namespace htmlayout::dom;

class CDictionary :
	public LiteWnd
{
public:
	CDictionary(LiteWnd* parent = NULL);
	~CDictionary(void);
public:
	element _table;
	element _title;
	element _num;
	element _filter;
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:
	void InitDomElement(void);	// ��������� �������� ��� � ������������ � ����������
	// ��������� � ����������� �������
	void UpdateView(void);
	// ���������� �����
	void ShowSelection();
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ �����
	static BOOL CALLBACK ElementEventProcForTable(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	static BOOL CALLBACK ElementEventProcChangedFilter(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);

	//
	void Add(void);
	void Delete(void);
	void Save(void);
	// ��������� ��������� �����. ����� ��� ������ ������� vkey, ��������� � ���������� ���������������� �����
	string_t GetNextNumForKey( const string_t& vkey, const string_t default_num = "-1" );
	// 
	//
	bool IsExistsTitleForKey(bool exclude_cur = false);
	bool IsExistsNumForKey(bool exclude_cur = false);
	string_t GetRecordId(bool throw_ = true);
};
