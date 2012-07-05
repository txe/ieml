#pragma once
#include "litewnd.h"
#include <htmlayout.h>
#include "string_t.h"

using namespace htmlayout::dom;

class CActionBuhReport2 :
	public LiteWnd
{ 
public:
	CActionBuhReport2(LiteWnd* parent = NULL);
	~CActionBuhReport2(void);

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();

private:
	void InitDomElement(void);	// ��������� �������� ��� � ������������ � ����������
	void FullGrpInLst();
	void FullSpecInLst();
	void CreateBuhData(int dKoef, int sKoef);
	void ProcessPlan(int dKoef, int sKoef);
	void ProcessPay();
	string_t GetSpecLst(string_t name);
	string_t GetGrpLst(string_t name);
	void GetYearMonthDay(std::wstring date1, int& year, int& month, int& day);
	
	// 
	void Report(void);
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ �����
	static BOOL CALLBACK ElementEventProcFor(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
};
