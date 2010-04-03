#pragma once

#include "LiteWnd.h"
#include "SData.h"
#include "htmlayout.h"
#include "SProgress.h"
#include "SPayment.h"
#include <map>
#include "string_t.h"

// CPropertyDlg dialog

class CPropertyDlg : public LiteWnd
{
private:
	/*
	�������� � ���� ����: ("id"  "�������") � ("-�������" "id")
	*/
	std::map<long, long>	students_map_;

	htmlayout::dom::element	header_;
	SData					s_data_; // ������ �� ��������
	SProgress				s_progress_; // ������������ ��������
	SPayment				s_payment_;

public:
	CPropertyDlg(LiteWnd* pParent = NULL);   // standard constructor
	virtual ~CPropertyDlg();
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:
	void InitDomElement(void);	// ��������� �������� ��� � ������������ � ����������
	// ��������� � ����������� ������� ��������
	bool UpdateView(void);
	// �������������� ������ ���������
	void InitStudIds(void);
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms );
	// ���������� ������ �� ���������� �������� � ������
	void NextStudent(void);
	// ���������� ������ �� ����������� �������� � ������ 
	void PrevStudent(void);
	// ����������
	void Save(void);
	// ���������� �� �������� �� ������� � ������
	long GetIdFromPos(const long& stud_pos);
	// ��������� ������� �������� � ������ �� �������������
	long GetPosFromId(const long& stud_id);
public:
	// ������� ������ �������� ��� ������� ������
	bool CreateData(void);
	// ������� ������ � ��������
	bool DeleteData(void);
};
