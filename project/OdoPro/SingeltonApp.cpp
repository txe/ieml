// ODOPro.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SingeltonApp.h"
#include "MainDlg.h"
#include <locale.h>
#include "wss_exeption.h"
#include "json-aux-ext.h"
#include "logger.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CODOApp

// CODOApp construction

SingeltonApp::SingeltonApp():_group_id(-1),_student_id(-1)
{

}

SingeltonApp::~SingeltonApp()
{

}

// The one and only CODOApp object

SingeltonApp theApp;

// CODOApp initialization

BOOL SingeltonApp::InitInstance()
{
/*
	������������������ ��������:
		1. ������������ ���
		2. ��������� ��������� �� �������, ��������� ����������
		3. ������������� ��� �������� ����� ����������
		4. ������������ � ����
		5. ��������� ��������� ����
		6. ��������� ������ ������������ � ����
		7. ����� �� ������� �����
		8. ���������� ������, ���������� �� ����
*/
//  �������������� ������
	LOG_PREFIX("log-odopro");

	#if defined(_DEBUG)
		LOG_LEVEL(logger::Debug); // ������� ���� � �������� (��������, ���� �� ��������� ������)
	#else
		LOG_LEVEL(logger::Warning);
	#endif
	
	LOG_DEBUG << "������ ���������";
	// ������������� ���������� ������, ��� �� ������� aux
	if(setlocale(LC_CTYPE,"Russian_Russia.1251") == 0) 
		throw wss::exception(wss::reason_message(FULL_LOCATION(), "setlocale(LC_CTYPE,Russian_Russia.1251) failed"));

//  ����������� � ����
	if (!_con.Connect())
	{
		MessageBox(::GetActiveWindow(), L"������ ����������� � ���� ������", 
			L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return FALSE;
	}
//  ��������� ��������� ������������
	_setting.load(theApp.GetCon(), "setting");
	UpgradeLocalParam();

	//_updater.RunUpdate();
	//return false;

//  ������ �������� ���� 
	LOG_DEBUG << "����������� ���� ���������� ...";
	try
	{
		CMainDlg dlg;
	//	m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		
	}
	catch (...)
	{
		theApp.ExceptionManage();
		LOG_ERROR << "��������� ���������";
	}
	_setting.save();
// �������� �����������
	_con.Close();
	LOG_DEBUG << "���������� ���������";
	//exit(0); // �������� ������� �����������
	return FALSE;
}

// ��������� ����������� ������� ������
void SingeltonApp::SetCurrentGroupID(long group_id)
{
	_group_id = group_id;
}

// ���������� �������������� �������� �������� 
long SingeltonApp::GetCurrentGroupID(void)
{
	if (_group_id == -1)
	{
		assert(FALSE);
		throw wss::exception(wss::reason_message(FULL_LOCATION(), "������ � ������ ���������, id ������ ����� -1"));
	}
	return _group_id;
}

// ��������� ����������� �������� ��������
void SingeltonApp::SetCurrentStudentID(long student_id)
{
	_student_id = student_id;
}

// ���������� �������������� ������� ������
// check ��������� ������� ��������
long SingeltonApp::GetCurrentStudentID(bool check /* = true */)
{
	if (!check)
		return _student_id;

	if (_student_id == -1)
	{
		assert(FALSE);
		throw wss::exception(wss::reason_message(FULL_LOCATION(), "������ � ������ ���������, id �������� ����� -1"));
	}

/*	string_t query;
	query	+= string_t()
		+ " SELECT id FROM students "
		+ " WHERE `deleted` = 0 AND "
		+ " `id` = " + string_t(aux::itoa(_student_id)) + " AND "
		+ " `grpid` = " + string_t(aux::itoa(theApp.GetCurrentGroupID()));
	mybase::MYFASTRESULT res = GetCon().Query(query);
	if (!res.Size())
	{
		LOG_ERROR << "������ � ������ ���������, ������� �� ����������� ������� ������";
		assert(FALSE);
		throw std::logic_error();
	}
*/	
	return _student_id;
}

// ���������� ���� ������
mybase::MyBase& SingeltonApp::GetCon(void) 
{ 
	return _con;
};

// ���������� ��� �������� ��������
string_t SingeltonApp::GetFIO(void)
{
	string_t query = string_t() + " SELECT secondname, firstname, thirdname FROM students "
		+ " WHERE `deleted` = 0 AND "
		+ " `id` = " + aux::itow(_student_id) + " AND "
		+ " `grpid` = " + aux::itow(theApp.GetCurrentGroupID());
	
	mybase::MYFASTRESULT res = GetCon().Query(query);
	mybase::MYFASTROW	 row = res.fetch_row();
	
	return row["secondname"] + " " + 
		row["firstname"] + " " + row["thirdname"];
}

// ���������� �������� ��� ������������ � ����� �� ������� VOC (UTF)
string_t SingeltonApp::GetTitleForKeyFromVoc(VOK_KEY vkey, int num, bool no_throw /* = false */)
{
	string_t key;
	switch (vkey)
	{
	case VK_GRP:        key = "grp";        break;
	case VK_EDUDOC:     key = "edudoc";     break;
	case VK_SPECS:      key = "spec";       break;
	case VK_SPEZIALIZ:  key = "specializ";  break;
	case VK_QUALIFIC:   key = "qualific";   break;
	case VK_DIRECT:     key = "directs";    break;
	case VK_DISCIPCLASSIFIC: key = "discipclassific"; break;
	}

	string_t query = string_t() + " SELECT title FROM voc "
		+ " WHERE `deleted` = 0 AND "
		+ " `num` = '" + aux::itow(num) + "' AND "
		+ " `vkey` = '" + key + "' LIMIT 1";

	mybase::MYFASTRESULT res = GetCon().Query(query);
	mybase::MYFASTROW	 row;

	if (!(row = res.fetch_row()))
	{
		if (no_throw)
			return "";
		assert(false);
		throw wss::exception(wss::reason_message(FULL_LOCATION()));
	}

	return row["title"];
}

// ���������� ��� ������������ ��� ������ (UTF)
string_t SingeltonApp::GetGroupName(int grpid)
{
	return GetTitleForKeyFromVoc(VK_GRP, grpid);
}

// ���������� �� ������������� ��� ������� ��������
long int SingeltonApp::GetSpecIdForStudId(long studid)
{
	string_t query = string_t() + " SELECT specid "
		" FROM students "
		" WHERE `deleted` = 0 AND "
		" `id` = " + aux::itow(studid);

	mybase::MYFASTRESULT res = GetCon().Query(query);
	mybase::MYFASTROW	 row = res.fetch_row();

	return aux::wtoi(string_t(row["specid"]));	
}



// ��������� ������������
int SingeltonApp::ExceptionManage(void)
{
	try
	{
		throw;
	}
	catch (wss::exception& e)
	{
		LOG_ERROR << e.what();
	}
	catch (std::exception& e)
	{
		LOG_ERROR << e.what();
	}
	catch (_com_error& e)
	{
		LOG_ERROR << (LPCSTR)aux::w2a(e.ErrorMessage());
	}
	catch (...)
	{
		LOG_ERROR << "����������� ����������";
		return -1;
	}
	return 0;
}

std::string SingeltonApp::GetModuleDir()
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[ _MAX_EXT];
	char path_buf[_MAX_PATH];

	GetModuleFileNameA(NULL, path_buf, sizeof(path_buf)-1);
	_splitpath(path_buf, drive, dir, fname, ext);

	return std::string(drive) + dir;
}
void SingeltonApp::UpgradeLocalParam(void)
{
	_setting.path_app_dir = GetModuleDir();
	_setting.path_temp_update_dir = _setting.path_app_dir + "{A65DEBAD-22C9-4b32-BEEE-BDBD8CF07BC9}\\";
}
