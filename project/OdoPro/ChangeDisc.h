#pragma once
#include "litewnd.h"

class ChangeDisc : public LiteWnd
{
public:
	ChangeDisc(LiteWnd* pParent = NULL);
	~ChangeDisc(void);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:

	htmlayout::dom::element table1_;
	htmlayout::dom::element table2_;
	long spec_;
private:
	// ��������� �������� ��� � ������������ � ����������
	void InitDomElement(void);	
	// ��������� � ����������� ������ ���������
	void UpdateTable(void);
	// ���������� ��������� ����������
	long GetDisc(htmlayout::dom::element table);	
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ��������� ������� ������
	void Run();
};
