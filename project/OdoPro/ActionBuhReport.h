#pragma once
#include "litewnd.h"
#include <htmlayout.h>

using namespace htmlayout::dom;

class CActionBuhReport :
	public LiteWnd
{
public:
	CActionBuhReport(LiteWnd* parent = NULL);
	~CActionBuhReport(void);
private:
	element _o_all;
	element _o_spec;
	element _o_grp;
	element _field_grp;
	element _field_fio;
	element _field_pas;
	element _pay_year_1;
	element _pay_year_2;
	element _cat_month_1;
	element _cat_month_2;
	element _cat_year_1;
	element _cat_year_2;
	element _date_box;
	element _date_1;
	element _date_2;

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:
	void InitDomElement(void);	// ��������� �������� ��� � ������������ � ����������
	// ���������� �����
	void StateChange();
	// 
	void Report(void);
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ �����
	static BOOL CALLBACK ElementEventProcFor(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
};
