// MainDlg.h : header file
//
#include "htmlayout.h"
#include "LiteWnd.h"
#include "string_t.h"
#include "ManagerPeports.h"
#include "ManagerOfActions.h"

#pragma once


// CMainDlg dialog
class CMainDlg : public LiteWnd, public IActionParent
{
	// ����������� ��� ������ ����
	enum {SB_HOST = 0, SB_LOGIN, SB_BD, SB_FILTR, SB_COUNT, SB_BUILD};
	// ��� ������
	enum FindType {F_FIO, F_ZNUM};
// Construction
public:
	CMainDlg(LiteWnd* pParent = NULL);		// standard constructor

public:
	virtual int		OnCreate();
	virtual BOOL	PreCreateWindow(CREATESTRUCT& cs); 

private:
	htmlayout::dom::element stud_grid_;		// ������� �� ������� ��������� ��� ��������� ������
	ManagerReports			manag_rep_;
	ManagerOfActions		manag_actions_;
  bool                show_dog_nums_;

public:
	void UpdateGrid(void);				// ��������� ������� �� ����������
	void ShowFindResult();    // ���������� ��������� ������ 
	void ShowFindResultEx();				// ���������� ��������� ������������ ������
	void SetStatusBar(const UINT& status,const string_t& value); // ������ �������� � ������-����
	void StudentProperty(void);			// �������� ������ �� ���������� ��������

private:
	void InitDomElement();			// ��������� �������� ��� � ������������
  void LoadRegParams();       // ��������� ��������� �� �������
	void UpdateGrpList(const string_t& dom_name, const string_t& grp_name_filter);	// ��������� ������ �����
	// ���������� ��� ������ ������
	static BOOL CALLBACK ElementEventProcForPanelGroup(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ���������� ��� ������ ��������
	static BOOL CALLBACK ElementEventProcForGrid(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ���������� ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ���������� ��������� ���� ������
	static BOOL CALLBACK ElementEventProcChangeFindType(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ���������� ����
	static BOOL CALLBACK ElementEventProcMenu(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������� ��������
	void CreateStudent(void);
	// ������� ��������
	void DeleteStudent(void);
	// ���������� �������� ������� ��� ������ � �������
	htmlayout::dom::element GetSelectedStudent(void);
	// ���������� id �������� ������� ��� ������ � ������� 
	long GetSelectedStudentID(void);

private:
  virtual void IActionParent_UpdateWindow();
};
