#pragma once

#include <string>
#include <deque>

template<class T> 
class CSafeDeque
{
private:
	std::deque<T>		_deque;
	CRITICAL_SECTION	_cs;
public:    
	CSafeDeque(){InitializeCriticalSection(&_cs);}
	~CSafeDeque(){DeleteCriticalSection(&_cs);}
	void push_back(const T& elem)
	{
		EnterCriticalSection(&_cs);
		_deque.push_back(elem);
		LeaveCriticalSection(&_cs);
	}
	std::deque<T> get_content()
	{
		EnterCriticalSection(&_cs);
		std::deque<T> buf = _deque;
		_deque.clear();
		LeaveCriticalSection(&_cs);
		return buf;
	}
};

struct prog_info
{
	enum STATUS_PROGRESS {PROGRESS_START, PROGRESS_RUN, PROGRESS_END};
	std::string file_name;
	char		count[15];
	char		type_count[15];
	char		pec[15];
	char		speed[15];
	char		type_speed[15];
	char		tm[15];
	STATUS_PROGRESS	type;
};

class Updater
{
private:
	HANDLE					_hReadPipe; // ����� ��������
	HANDLE					_hThread;
	PROCESS_INFORMATION		_pi;
	prog_info				_info;
	CSafeDeque<prog_info>	_status_update;
	CSafeDeque<bool>		_status_check;
public:
	Updater(void);
	~Updater(void);
	// ��������� ������� ����������
	bool start_check_update(void);
	// ��������� ������� ����������  � ������������� 
	bool start_update(void);
	// ��������� ������� ����������  ��� ������������ 
	bool start_silent_update(void);
	// ������ ���������� �� ����������
	std::deque<prog_info> GetInfoProgress();
	// ������ ���������� �� ������� ����������
	std::deque<bool> GetInfoStatusCheck();
private:
	// ������� �����, �������� ����� ����������� ��������
	bool create_thread(LPTHREAD_START_ROUTINE param);
	// ������� ������� rcync
	bool create_rsync_process(const std::string &path_param);
	// ����� ������ ����� 
	static DWORD read_update_progress_thread(LPVOID param);
	// ����� ������ ����� 
	static DWORD read_check_update_thread(LPVOID param);
	// ��������� �������
	void DestroyHandle(void);
	// ������ �����
	void parse(std::string& output);
	// ������ ��� ������ � ������
	bool get_line(std::string& output, std::string& line, bool& prog_line, bool& end_line);
	// ������ ������ ��������� �����������
	void parse_progress(std::string& progress, bool end_progress);
	// ������ ������ �������� ��� �������� ����������� �����
	void parse_header(std::string& line);
	//
	void AddProgressInfo(prog_info::STATUS_PROGRESS type);
};
