#pragma once
#include "litewnd.h"
#include <deque>


class CActionFastInputGakDlg :	public LiteWnd
{
public:
	CActionFastInputGakDlg(LiteWnd* pParent = NULL);
	~CActionFastInputGakDlg(void);

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();

private:
	void SetParam(long discip_id, string_t plan_sem, string_t graph_sem, string_t estim);

	// ��������� �������� ��� � ������������ � ����������
	void InitDomElement(void);	
	// ��������� � ����������� ������� ������
	bool UpdateView(void);
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ ��������� ���������
	static BOOL CALLBACK ElementEventProcSem(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ����������
	void Save(void);
	// ������ ��� ���
	void SetGak();
};
