#pragma once
#include "litewnd.h"
#include <deque>


class CProgressFastInputBallDlg :
	public LiteWnd
{
public:
	CProgressFastInputBallDlg(LiteWnd* pParent = NULL);
	~CProgressFastInputBallDlg(void);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
public:
	void SetParam(long discip_id, string_t plan_sem, string_t graph_sem, string_t estim);
private:
	long discip_id_;
	// ������������ �������� ������ ��� ����������� � �������
	static std::deque<json::value>	_buf_ball;
	// ������������ ��� ������������� ��������
	string_t plan_sem_;
	string_t graph_sem_;  
	string_t estim_;
private:
	// ��������� �������� ��� � ������������ � ����������
	void InitDomElement(void);	
	// ��������� � ����������� ������� ������
	bool UpdateView(void);
	// �������� ��������� ������
	bool Check(void);
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ ��������� ���������
	static BOOL CALLBACK ElementEventProcSem(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ����������
	void Save(void);
	// �������� ������ � �����
	void CopyToBuf();
	// �������� ������ �� ������
	void InsertFromBuf();
};
