#include "stdafx.h"
#include "ManadUpdate.h"
#include "LiteWnd.h"
#include "json-aux-ext.h"
#include <fstream>
#include "logger.h"

UpdateManager::UpdateManager(void)
{
}

UpdateManager::~UpdateManager(void)
{    
	RestartApp();
}

// запускает обновление программы
void UpdateManager::RunUpdate(bool silent_mode /* = true */)
{
	_silent_mode = silent_mode;
	if (!(_silent_mode?_updater.start_silent_update():_updater.start_update()))
		return;
	download_update_panel.start_timer(1);
}

// инициализирует параметры
void UpdateManager::Init(HWND hWnd)
{
	_hWnd					= hWnd;
	element root			= element::root_element(hWnd);
	check_update_panel		= LiteWnd::link_element(root, "check-update-panel");
	download_update_panel	= LiteWnd::link_element(root, "download-update-panel");

	download_status		= LiteWnd::link_element(root, "download-status");
	download_progress	= LiteWnd::link_element(root, "download-progress");
	//
	HTMLayoutAttachEventHandlerEx(download_update_panel, ElementEventProcTimerProgressUpdate, this, HANDLE_TIMER | DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(check_update_panel, ElementEventProcTimerCheckUpdate, this, HANDLE_TIMER | DISABLE_INITIALIZATION);

	check_update_panel.start_timer(500); // 0.5 сек

	char path[MAX_PATH];
	GetTempPathA(MAX_PATH, path);
	_script_name = path;
	_script_name += "update.cmd";

	create_wait_terminate_thread();
}

BOOL CALLBACK UpdateManager::ElementEventProcTimerProgressUpdate(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_TIMER)
		return FALSE;

	UpdateManager* manager = static_cast<UpdateManager*>(tag);
	manager->update_progress();
	return TRUE;
}

void UpdateManager::update_progress()
{
	std::deque<prog_info> _info = _updater.GetInfoProgress();
	while(_info.size())
	{
		prog_info info = _info.front();
		_info.pop_front();
		if (info.type == prog_info::PROGRESS_START)
		{
			check_update_panel.set_style_attribute("visibility", L"collapse");
			download_update_panel.set_style_attribute("visibility", L"visible");
			download_status.set_text(L"");
			download_progress.set_value(json::value(0.0));
			element(check_update_panel.parent()).update(true);
			continue;
		}
		if (info.type == prog_info::PROGRESS_END)
		{
			download_update_panel.set_style_attribute("visibility", L"collapse");
			check_update_panel.set_style_attribute("visibility", L"collapse");
			element(check_update_panel.parent()).update(true);
			download_update_panel.stop_timer();
			if (_silent_mode)
				RunUpdate(false);
			else
				GoToRestart();
			continue;
		}
		if (info.type == prog_info::PROGRESS_RUN)
		{
			string_t text = info.file_name + " : " + info.pec + "%";
			download_status.set_text(text);
			json::t2v(download_progress, info.pec);
			download_update_panel.update(true);
			continue;
		}
	}
}

BOOL CALLBACK UpdateManager::ElementEventProcTimerCheckUpdate(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_TIMER)
		return FALSE;

	UpdateManager* manager = static_cast<UpdateManager*>(tag);
	manager->manager_check_update();

	return TRUE;
}

void UpdateManager::manager_check_update()
{
	enum STATUS_CHECK {WAIT_CHECK = 0x2, RUN_CHECK = 0x4, SHOW_MESSAGE = 0x16};
	static DWORD status_check = WAIT_CHECK; 

	if (status_check == WAIT_CHECK)
		if (_updater.start_check_update())
		{
			check_update_panel.set_style_attribute("visibility", L"visible");
			check_update_panel.set_text(L"поиск обновления");
			check_update_panel.update(true);
			status_check = RUN_CHECK;
			check_update_panel.start_timer(1);
			return;
		}

	std::deque<bool> info = _updater.GetInfoStatusCheck();
	if (status_check == RUN_CHECK && info.size() > 0) // если есть результат
	{
		if (info.front()) 
		{
			check_update_panel.set_text(L"найдено обновление");
			//check_update_panel.start_timer(1000*30);// 30 сек
			check_update_panel.stop_timer();
			MessageBeep(MB_ICONASTERISK);
		}
		else
		{
			check_update_panel.set_text(L"обновление не требуется");
			check_update_panel.start_timer(1000*5);// 5 сек
		}
		check_update_panel.update(true);
		status_check = SHOW_MESSAGE;
		return;	
	}
	if (status_check == SHOW_MESSAGE)
	{
		status_check = WAIT_CHECK;
		check_update_panel.start_timer(20*60*1000); // 10 мин
		check_update_panel.set_style_attribute("visibility", L"collapse");
		check_update_panel.update(true);
		return;
	}	
}

void UpdateManager::create_wait_terminate_thread()
{
	if (NULL == CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&wait_terminate_thread, this, 0, NULL))
	{
		LOG_ERROR << FULL_LOCATION() << ":" << "не создался поток ожидания сообщений";
		return;
	}	
}

DWORD UpdateManager::wait_terminate_thread(LPVOID param)
{
	UpdateManager* manager = static_cast<UpdateManager*>(param);
	// устанавливаем флаг
	manager->_my_send_event_terminate = false;
	// создаем событие
	HANDLE hEventTerminate = CreateEvent(NULL, TRUE, FALSE, L"Global\\{2BD672C4-8070-490a-949E-48F68A91BE8B}");
	::WaitForSingleObject(hEventTerminate, INFINITE); // ожидания события об убиении процесса
	CloseHandle(hEventTerminate);
	if (manager->_my_send_event_terminate) // если мы заказали музыку
	{
		HANDLE hSemaphoreExistProcess = CreateSemaphore(NULL, 0, 1, L"Global\\{34DEF936-37D9-4409-8843-BD988E5FBAF0}");
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(hSemaphoreExistProcess, 500)) // ожидание есть ли еще процессы
		{
			HANDLE hMutexAllProcessExit = CreateMutex(NULL, true, L"Global\\{5C03AEB8-309F-42a8-B253-F225F5E45F4B}");
			::WaitForSingleObject(hMutexAllProcessExit, INFINITE);
			CloseHandle(hMutexAllProcessExit);
		}
		CloseHandle(hSemaphoreExistProcess);
		manager->create_restart_script();
		// завершаем коректно программу
		PostMessage(manager->_hWnd, WM_CLOSE, 0, 0);
	}
	else
	{// иначе грохаем весь процесс
		// тормозим процесс, который нас убивает, разблокируется после выхода этой программы
		CreateMutex(NULL, true, L"Global\\{5C03AEB8-309F-42a8-B253-F225F5E45F4B}"); 
		CreateSemaphore(NULL, 0, 1, L"Global\\{34DEF936-37D9-4409-8843-BD988E5FBAF0}"); // говорим процессу что мы есть
		exit(-1);
	}
	return 0;
}

void UpdateManager::GoToRestart()
{
	_my_send_event_terminate = true;
	HANDLE hGoKillAll = CreateEvent(NULL, TRUE, FALSE, L"Global\\{2BD672C4-8070-490a-949E-48F68A91BE8B}");
	PulseEvent(hGoKillAll);
}

void UpdateManager::create_restart_script()
{
	std::string body;
	// временная задержка n секунд
	body += theApp.GetSetting().command_wait + "\n";
	// копирование
	body += "\"" + theApp.GetSetting().path_app_dir + "rsync.exe\" --recursive --progress --times --human-readable "
		" \"" + t::path2rsync(theApp.GetSetting().path_temp_update_dir) + "\" \"" +
		t::path2rsync(theApp.GetSetting().path_app_dir) + "\"\n";
	// удаляет удаляет временный каталог
	body += "RMDIR /S /Q \"" + theApp.GetSetting().path_temp_update_dir + "\"\n";
	// запускает заного приложение
	body += "START /D\"" + theApp.GetSetting().path_app_dir + "\" " + theApp.GetSetting().app_before_restart;
	char buf[1024];
	CharToOemA(body.c_str(), buf);
	std::ofstream ifs(_script_name.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
	ifs.write(buf, strlen(buf));
	ifs.close();
}

void UpdateManager::RestartApp()
{
	if (!_my_send_event_terminate)
		return;
	
	PROCESS_INFORMATION pi;
	STARTUPINFOA si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_HIDE;
	_script_name = "\"" + _script_name;
	CreateProcessA(NULL, (LPSTR)_script_name.c_str(), NULL, NULL, TRUE, 
		CREATE_NO_WINDOW | NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
}
