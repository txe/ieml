#pragma once
#include "litewnd.h"
#include <htmlayout.h>

using namespace htmlayout::dom;

class CActionChangeSem :
	public LiteWnd
{
public:
	enum TypeSem {PLAN_SEM, GRAPH_SEM};
private:
	TypeSem _type_sem;
private:
	element _table;
	element _cur_sem;
	element _new_sem;
public:
	CActionChangeSem(LiteWnd* parent = NULL);
	~CActionChangeSem(void);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
	void		 SetTypeSem(TypeSem type_sem) { _type_sem = type_sem; };
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
	// ������������ ����� ��������/��������
	static BOOL CALLBACK ElementEventProcForTable(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
};
