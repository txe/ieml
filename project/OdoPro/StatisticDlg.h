#pragma once
#include "litewnd.h"

class CStatisticDlg :
	public LiteWnd
{
public:
	CStatisticDlg(LiteWnd* pParent = NULL);   // standard constructor
	virtual ~CStatisticDlg(void);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:
	void InitDomElement(void);	// ��������� �������� ��� � ������������ � ����������
	void ShowStatistic(void);	// ���������� ���������� �� ��������� ���������
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms );
};
