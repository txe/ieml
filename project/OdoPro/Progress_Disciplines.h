#pragma once

#include "string_t.h"
#include "litewnd.h"
#include <vector>

class CManagDisciplinesDlg :
	public LiteWnd
{
public:
	CManagDisciplinesDlg(LiteWnd* pParent = NULL);
	~CManagDisciplinesDlg(void);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:
	htmlayout::dom::element	discip_table_;
	htmlayout::dom::element	fulltitle_;
	htmlayout::dom::element	shorttitle_;
	htmlayout::dom::element	discip_class_;
	htmlayout::dom::element	scan_namber_;
	htmlayout::dom::element	num_hours_;
	htmlayout::dom::element	spec_;
  htmlayout::dom::element	zach_ed_;


	std::vector<htmlayout::dom::element> sem_hours_;
private:
	void InitDomElement(void);	// ��������� �������� ��� � ������������ � ����������
	// ��������� � ����������� ������� ������
	bool UpdateView(int showDisc);
	// �������� ��������� ������
	bool Check(void);
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ ����� ����������
	static BOOL CALLBACK ElementEventProcDiscip(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms );
	// ��������� ������� ���������
	void UpdateViewDiscipTable(int showDisc);
	// ���������� ������ �� ��������� ����������
	void UpdateViewDiscipData(void);
	// ������� ���������� �� ����
	void DeleteDiscip(void);
	// ��������� ���������� �� ���������� � ������ �����������
	bool IsExistDiscip(bool include_cur_discip = true);
	// ������� ����� ����������
	void AddDiscip(void);
	// ��������� ��������� ����������
	void SaveUpdateDiscip(void);
	// ��������� ���������� ���� � ������
	string_t AudHoursToStr(void);
};
