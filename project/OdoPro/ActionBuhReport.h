#pragma once
#include "litewnd.h"
#include <htmlayout.h>
#include "string_t.h"

using namespace htmlayout::dom;

class CActionBuhReport :
	public LiteWnd
{
public:
	CActionBuhReport(LiteWnd* parent = NULL);
	~CActionBuhReport(void);

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();

private:
	void InitDomElement(void);	// ��������� �������� ��� � ������������ � ����������
	void FullGrpInLst();
	void FullSpecInLst();
	void CreateBuhData();
	void ProcessPlan();
	void ProcessPay();
	string_t GetSpecLst(string_t name);
	string_t GetGrpLst(string_t name);
	string_t GetRange(string_t name);

	// ���������� �����
	void StateChange();
	// 
	void Report(void);
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ �����
	static BOOL CALLBACK ElementEventProcFor(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
};
