#include "stdafx.h"
#include "Updater.h"
#include "wss_exeption.h"
#include "SingeltonApp.h"
#include <direct.h>
#include "json-aux-ext.h"
#include "logger.h"


Updater::Updater(void):_hReadPipe(NULL),_hThread(NULL)
{
}

Updater::~Updater(void)
{
}

// проверяет наличие обновления
bool Updater::start_check_update(void)
{

	std::string cmd = theApp.GetModuleDir() + "rsync.exe --dry-run --delete --times --recursive --progress --human-readable \"" +
		t::path2rsync(theApp.GetSetting().path_update_dir) + "\" \"" +
		t::path2rsync(theApp.GetSetting().path_app_dir) + "\"";

	if (!create_thread((LPTHREAD_START_ROUTINE)read_check_update_thread) 
		|| !create_rsync_process(cmd))
		return false;
	// запускаем поток для обновления прогресса обновления
	ResumeThread(_pi.hThread);
	ResumeThread(_hThread);

	return true;
}

// запускает процесс обновления
bool Updater::start_update(void)
{
	std::string cmd = theApp.GetModuleDir() + "rsync.exe --delete --times --recursive --progress --human-readable "
		" --compare-dest=\"" + t::path2rsync(theApp.GetSetting().path_app_dir) + "\" \"" + 
		t::path2rsync(theApp.GetSetting().path_update_dir) + "\" \"" +
		t::path2rsync(theApp.GetSetting().path_temp_update_dir) + "\"" ;

	if (!create_thread((LPTHREAD_START_ROUTINE)read_update_progress_thread) 
		|| !create_rsync_process(cmd))
		return false;
	// запускаем поток для обновления прогресса обновления
	ResumeThread(_pi.hThread);
	ResumeThread(_hThread);

	return true;
}

// запускает процесс обновления  без перезагрузки 
bool Updater::start_silent_update(void)
{
	std::string cmd = theApp.GetModuleDir() + "rsync.exe --delete --times --recursive --progress --human-readable "
		+ theApp.GetSetting().exclude_silent_update +
		" \"" + t::path2rsync(theApp.GetSetting().path_update_dir) + "\" \"" +
		t::path2rsync(theApp.GetSetting().path_app_dir) + "\"";
	if (!create_thread((LPTHREAD_START_ROUTINE)read_update_progress_thread) 
		|| !create_rsync_process(cmd))
		return false;
	// запускаем поток для обновления прогресса обновления
	ResumeThread(_pi.hThread);
	ResumeThread(_hThread);

	return true;
}

// создает поток, читающий вывод запущенного процесса
bool Updater::create_thread(LPTHREAD_START_ROUTINE param)
{
	// проверям, может поток запущен
	DWORD ec;
	if (GetExitCodeThread(_hThread, &ec) && ec == STILL_ACTIVE)
	{
		MessageBox( NULL, L"Идет поиск обновления. \nПопробуйте еще раз.", L"Сообщение", 0);
		return false;
	}
	// создаем поток опроса вывода
	if (NULL == (_hThread = CreateThread(NULL, 0, param, this, CREATE_SUSPENDED, NULL)))
	{
		LOG_ERROR << FULL_LOCATION() << ":" << "не создался поток опроса пайпа";
		return false;
	}
	return true;
}

// запускает процесс обновления программы
bool Updater::create_rsync_process(const std::string &param_for_rsync)
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	HANDLE hWrite;
	if (!CreatePipe(&_hReadPipe, &hWrite, &sa, 0))
	{
		LOG_ERROR << FULL_LOCATION() << ":" << "не создался пайп";
		return false;
	}

	STARTUPINFOA si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = hWrite;

	if (!CreateProcessA(NULL, (LPSTR)param_for_rsync.c_str(), NULL, NULL, TRUE, 
		CREATE_NO_WINDOW | NORMAL_PRIORITY_CLASS | CREATE_SUSPENDED, NULL, NULL, &si, &_pi))
	{
		LOG_ERROR << FULL_LOCATION() << ":" << "не запустился процесс";
		DWORD err = GetLastError();
		CloseHandle(hWrite);
		DestroyHandle();
		return false;
	}

	CloseHandle(hWrite);	
	return true;
}

// поток чтения пайпа 
DWORD Updater::read_update_progress_thread(LPVOID param)
{
	Updater* updater = static_cast<Updater*>(param);
	updater->AddProgressInfo(prog_info::PROGRESS_START);
	for (;;) 
	{ 
		std::string output;
		const int	BUFSIZE = 1024;
		char		chBuf[BUFSIZE];
		DWORD		dwRead;
		DWORD		ec = 0; //exit code
		if (!GetExitCodeThread(updater->_pi.hThread, &ec) || ec != STILL_ACTIVE)
			break;
		while (ReadFile(updater->_hReadPipe, chBuf, BUFSIZE - 1, &dwRead,	NULL) || dwRead)
		{
			chBuf[dwRead] = '\0';
			output += chBuf;
			LOG_DEBUG << output;
			updater->parse(output);
		}
	} 
	updater->AddProgressInfo(prog_info::PROGRESS_END);
	updater->DestroyHandle();
	return 0;
}

// поток, проверка наличия обновления 
DWORD Updater::read_check_update_thread(LPVOID param)
{
	Updater*	updater = static_cast<Updater*>(param);
	std::string output;
	for (;;) 
	{ 
		const int	BUFSIZE = 1024;
		char		chBuf[BUFSIZE];
		DWORD		dwRead;
		DWORD		ec = 0; //exit code
		if (!GetExitCodeThread(updater->_pi.hThread, &ec) || ec != STILL_ACTIVE)
			break;
		while(ReadFile(updater->_hReadPipe, chBuf, BUFSIZE - 1, &dwRead,	NULL) || dwRead)
		{
			chBuf[dwRead] = '\0';
			output += chBuf;
		}
	} 
	
	LOG_DEBUG << output;
	int	count_line = 0;
	for (std::string::iterator it = output.begin(); it != output.end(); ++it)
	{
		if (*it != '\n')
			continue;
		std::string line = output.substr(0, std::distance(output.begin(), it));
		++it;
		output.erase(output.begin(), it);
		it = output.begin();
		if (0 != line.find("sending") &&
			(line.size()-1) != line.find("/") &&
			0 != line.find("deleting") &&
			1 != line.find("sent") &&
			0 != line.find("total size") &&
			0 != line.size())
				++count_line;			
		if (it == output.end())
			break;
	}

	bool exist_update = count_line > 0;

	updater->_status_check.get_content(); // удаляем предыдущую информацию
	updater->_status_check.push_back(exist_update);

	updater->DestroyHandle();
	return 0;
}

// закрывает ресурсы
void Updater::DestroyHandle(void)
{
	if (_hThread)
	{
		ResumeThread(_hThread);
		CloseHandle(_hThread);
	}
	if (_hReadPipe)
		CloseHandle(_hReadPipe);
	if (_pi.hProcess)
		CloseHandle(_pi.hProcess);
	if (_pi.hThread)
		CloseHandle(_pi.hThread);
	_hThread     = 0;
	_hReadPipe   = 0;
	_pi.hProcess = 0;
	_pi.hThread  = 0;		
}

// парсит вывод
void Updater::parse(std::string& output)
{
	std::string line;
	bool		prog_line;
	bool		end_line; // оканчание строки или возврат каретки
	while (get_line(output, line, prog_line, end_line))
		prog_line?parse_progress(line, end_line):parse_header(line);
}

// выдает тип строки, тип окончания строки и строку
bool Updater::get_line(std::string& output, std::string& line, bool& prog_line, bool& end_line)
{
	for (std::string::iterator it = output.begin(); it != output.end(); ++it)
	{
		if (*it == '\0')
			return false;
		if (*it == '\n' || *it == '\r')
		{
			end_line =  *it == '\n';

			line = output.substr(0, std::distance(output.begin(), it));
			++it;
			output.erase(output.begin(), it);

			if (!(prog_line = !end_line))
				if ( 0 == line.find("   "))
					prog_line = true;
			return true;
		}
	}
	return false;
}

// парсит строку прогресса копирования
void Updater::parse_progress(std::string& progress, bool end_progress)
{
	char none[20];
	sscanf(progress.c_str(),"%[^0-9]%[0-9.] %s %[^%%]%%%[^0-9]%[0-9.] %s %s", 
		none, _info.count, _info.type_count, _info.pec, none, _info.speed, _info.type_speed, _info.tm);
	
	AddProgressInfo(prog_info::PROGRESS_RUN);
}

// парсит строку возможно это назавние копируемого файла
void Updater::parse_header(std::string& line)
{
	_info.file_name = line;
}

void Updater::AddProgressInfo(prog_info::STATUS_PROGRESS type)
{
	_info.type = type;
	_status_update.push_back(_info);
}

// отдает информацию об обновлении
std::deque<prog_info> Updater::GetInfoProgress()
{
	return _status_update.get_content();
}

// отдает информацию об наличии обновления
std::deque<bool> Updater::GetInfoStatusCheck()
{
	return _status_check.get_content();
}