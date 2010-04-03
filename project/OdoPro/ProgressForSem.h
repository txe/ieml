#pragma once

#include "string_t.h"
#include "litewnd.h"

class CProgressViewBallForSemDlg :	public LiteWnd
{
public:
	CProgressViewBallForSemDlg(LiteWnd* pParent = NULL);
	~CProgressViewBallForSemDlg(void);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
	void SetParam(string_t plan_sem, string_t graph_sem);
private:
	// ������������ ��� ������������� ��������
	string_t plan_sem_;
	string_t graph_sem_;
private:
	void InitDomElement(void);	// ��������� �������� ��� � ������������ � ����������
	// ��������� � ����������� ������� ������
	bool UpdateView(void);
	// �������� ��������� ������
	bool Check(void);
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ ��������� ���������
	static BOOL CALLBACK ElementEventProcSem(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
};
