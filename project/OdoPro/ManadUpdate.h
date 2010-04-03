#pragma once
#include <htmlayout.h>
#include "Updater.h"
#include <deque>

using namespace htmlayout::dom;

class UpdateManager
{
private:
	Updater					_updater;
	bool					_silent_mode;
private:
	volatile bool			_my_send_event_terminate;
	HWND					_hWnd;
	std::string				_script_name;
	HANDLE					hThread;
private:
	void create_wait_terminate_thread();
	static DWORD wait_terminate_thread(LPVOID param);
	void GoToRestart();
	void RestartApp();
	void create_restart_script();
private:
	element check_update_panel;
	element download_update_panel;

	element download_status;
	element download_progress;
private:
	static BOOL CALLBACK ElementEventProcTimerProgressUpdate(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	static BOOL CALLBACK ElementEventProcTimerCheckUpdate(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	//
	void update_progress();
	void manager_check_update();
public:
	UpdateManager(void);
	~UpdateManager(void);
	// запускает обновление программы
	void RunUpdate(bool silent_mode = true);
	// инициализирует параметры
	void Init(HWND hWnd);
};
