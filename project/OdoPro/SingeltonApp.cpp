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
	последовательность действий:
		1. инициировать лог
		2. загрузить параметры из конфига, проверить валидность
		3. перезагрузить лог согласно новым параметрам
		4. подключиться к базе
		5. проверить параметры базы
		6. загрузить конфиг пользователя с базы
		7. выйти на рабочий режим
		8. завершение работы, отключение от базы
*/
//  инициализируем логгер
	LOG_PREFIX("log-odopro");

	#if defined(_DEBUG)
		LOG_LEVEL(logger::Debug); // уровень лога в рантайме (например, берём из командной строки)
	#else
		LOG_LEVEL(logger::Warning);
	#endif
	
	LOG_DEBUG << "Запуск программы";
	// устанавливаем правильную локаль, что бы работал aux
	if(setlocale(LC_CTYPE,"Russian_Russia.1251") == 0) 
		throw wss::exception(wss::reason_message(FULL_LOCATION(), "setlocale(LC_CTYPE,Russian_Russia.1251) failed"));

//  подключение к базе
	if (!_con.Connect())
	{
		MessageBox(::GetActiveWindow(), L"Ошибка подключения к базе данных", 
			L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return FALSE;
	}
//  получение установок пользователя
	_setting.load(theApp.GetCon(), "setting");
	UpgradeLocalParam();

	//_updater.RunUpdate();
	//return false;

//  запуск главного окна 
	LOG_DEBUG << "Отображение окна приложения ...";
	try
	{
		CMainDlg dlg;
	//	m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		
	}
	catch (...)
	{
		theApp.ExceptionManage();
		LOG_ERROR << "Обрушение программы";
	}
	_setting.save();
// закрытие подключения
	_con.Close();
	LOG_DEBUG << "Завершение программы";
	//exit(0); // ускоряем процесс заверешения
	return FALSE;
}

// сохраняет индификатор текущей группы
void SingeltonApp::SetCurrentGroupID(long group_id)
{
	_group_id = group_id;
}

// возвращает индентификатор текущего студента 
long SingeltonApp::GetCurrentGroupID(void)
{
	if (_group_id == -1)
	{
		assert(FALSE);
		throw wss::exception(wss::reason_message(FULL_LOCATION(), "Ошибка в логике программы, id группы равен -1"));
	}
	return _group_id;
}

// сохраняет индификатор текущего студента
void SingeltonApp::SetCurrentStudentID(long student_id)
{
	_student_id = student_id;
}

// возвращает индентификатор текущей группы
// check управляет режимом проверки
long SingeltonApp::GetCurrentStudentID(bool check /* = true */)
{
	if (!check)
		return _student_id;

	if (_student_id == -1)
	{
		assert(FALSE);
		throw wss::exception(wss::reason_message(FULL_LOCATION(), "Ошибка в логике программы, id студента равен -1"));
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
		LOG_ERROR << "Ошибка в логике программы, студент не принадлежит текущей группе";
		assert(FALSE);
		throw std::logic_error();
	}
*/	
	return _student_id;
}

// возвращает базу данных
mybase::MyBase& SingeltonApp::GetCon(void) 
{ 
	return _con;
};

// возвращает ФИО текущего студента
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

// возвращает описание для индефикатора и ключа из таблицы VOC (UTF)
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

// возвращает для индификатора имя группы (UTF)
string_t SingeltonApp::GetGroupName(int grpid)
{
	return GetTitleForKeyFromVoc(VK_GRP, grpid);
}

// возвращает ид специальности для данного студента
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



// управляет исключениями
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
		LOG_ERROR << "неизвестное исключение";
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
