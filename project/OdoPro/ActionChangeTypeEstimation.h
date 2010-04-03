#pragma once
#include "litewnd.h"
#include <htmlayout.h>

using namespace htmlayout::dom;

class CActionChangeTypeEstimation :
	public LiteWnd
{
public:
	CActionChangeTypeEstimation(LiteWnd* parent = NULL);
	~CActionChangeTypeEstimation(void);
private:
	element _table;
	element _exam;
	element _test;
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:
	void InitDomElement(void);	// ��������� �������� ��� � ������������ � ����������
	// ��������� � ����������� �������
	void UpdateView(void);
	// ���������� �����
	void ShowSelection();
	// 
	void Save(void);
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ �����
	static BOOL CALLBACK ElementEventProcForTable(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
};
