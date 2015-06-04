// SingeltonApp.h : main header file for the PROJECT_NAME application
//

#pragma once

#include "resource.h"		// main symbols
#include "MYBASE.h"
#include <vector>
#include "string_t.h"
#include "wss_exeption.h"
#include "ManagerSetting.h"
#include "ManadUpdate.h"
#include "reports/Report_Abstract.h"

namespace vok_key
{
  enum VOK_KEY {VK_GRP, VK_EDUCATIONS, VK_EDUDOC, VK_EDUFORM, VK_SPECS, VK_SPEZIALIZ, VK_QUALIFIC, VK_DIRECT, VK_DISCIPCLASSIFIC, VK_SHIFRSPEC, VK_DOG_YEAR, VK_DOG_SHIFR, VK_DOG_FAST, VK_DOG_EXTRA, VK_FACULTY};
}

class LiteWnd;
class SingeltonApp
{
public:
	SingeltonApp();
	~SingeltonApp();
private:
	mybase::MyBase	_con;			// ���� ������
	long			_group_id;		// id ������� ������
	long			_student_id;	// id �������� ��������
	ManagerSetting	_setting;		// ��������� ���������
	UpdateManager	_updater;		// ��������� ������������
    std::vector<ReportAbstract*> _reposts;
  LiteWnd*     _mainWnd;
// Overrides
public:
	virtual BOOL InitInstance();
public:
	void SetCurrentGroupID(long group_id);			// ��������� ����������� ������� ������
	long GetCurrentGroupID(void);					// ���������� �������������� ������� ������
	void SetCurrentStudentID(long group_id);		// ��������� ����������� �������� ��������
	long GetCurrentStudentID(bool check = true);	// ���������� �������������� �������� ��������
	mybase::MyBase& GetCon(void);					// ���������� ���� ������
	const ManagerSetting&	GetSetting() { return _setting; }
	UpdateManager&			GetUpdater() { return _updater; }

  void SetMainWnd(LiteWnd* wnd) { _mainWnd = wnd; }
  LiteWnd* GetMainWnd()         { return _mainWnd; }
  
  // ���������� ��� �������� ��������
	string_t GetFIO(void);
	// ���������� �������� ��� ������������ � ����� �� ������� VOC
  string_t GetTitleForKeyFromVoc(vok_key::VOK_KEY vkey, int num, bool no_throw = false, string_t* get_tag = NULL);
	// ���������� ��� ������������ ��� ������
	string_t GetGroupName(int grpid);
	// ���������� �� ������������� ��� ������� ��������
	long int GetSpecIdForStudId(long studid);
	// ��������� ������������
	int ExceptionManage(void);
	// ���������� ��������� ����� ���������
	std::string GetModuleDir();

private:
	void UpgradeLocalParam(void);
};

extern SingeltonApp theApp;
