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
	// ���������� ��� �������� ��������
	string_t GetFIO(void);
	// ���������� �������� ��� ������������ � ����� �� ������� VOC
	string_t GetTitleForKeyFromVoc(string_t vkey, int num);
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
