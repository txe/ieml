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
#include "reports/ReportAbstract.h"

enum VOK_KEY {VK_GRP, VK_EDUDOC, VK_SPECS, VK_SPEZIALIZ, VK_QUALIFIC, VK_DIRECT, VK_DISCIPCLASSIFIC};

class SingeltonApp
{
public:
	SingeltonApp();
	~SingeltonApp();
private:
	mybase::MyBase	_con;			// база данных
	long			_group_id;		// id текущей группы
	long			_student_id;	// id текущего студента
	ManagerSetting	_setting;		// различные установки
	UpdateManager	_updater;		// управляет обновлениями
    std::vector<ReportAbstract*> _reposts;
// Overrides
public:
	virtual BOOL InitInstance();
public:
	void SetCurrentGroupID(long group_id);			// сохраняет индификатор текущей группы
	long GetCurrentGroupID(void);					// возвращает индентификатор текущей группы
	void SetCurrentStudentID(long group_id);		// сохраняет индификатор текущего студента
	long GetCurrentStudentID(bool check = true);	// возвращает индентификатор текущего студента
	mybase::MyBase& GetCon(void);					// возвращает базу данных
	const ManagerSetting&	GetSetting() { return _setting; }
	UpdateManager&			GetUpdater() { return _updater; }
	// возвращает ФИО текущего студента
	string_t GetFIO(void);
	// возвращает описание для индефикатора и ключа из таблицы VOC
	string_t GetTitleForKeyFromVoc(VOK_KEY vkey, int num, bool no_throw = false);
	// возвращает для индификатора имя группы
	string_t GetGroupName(int grpid);
	// возвращает ид специальности для данного студента
	long int GetSpecIdForStudId(long studid);
	// управляет исключениями
	int ExceptionManage(void);
	// возвращает положение файла программы
	std::string GetModuleDir();

private:
	void UpgradeLocalParam(void);
};

extern SingeltonApp theApp;
