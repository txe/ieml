#pragma once

#include <htmlayout.h>
#include "string_t.h"

using namespace htmlayout::dom;

class ManagerReports
{
public:
	ManagerReports(void);
	~ManagerReports(void);
private:
	element		_menu;
	HINSTANCE	_hinstRepLib;
	HANDLE hRead, hWrite;
public:
	// ������������� ������ ���������
	void Init(element menu);
private:
	// ��������� ���������� �������
	void LoadReports(void);
	// ��������� ���������� �������
	void UnLoadReports(void);
	// �������� ����
	void CreateMenu(void);
	// ������������ ����
	static BOOL CALLBACK ElementEventProcMenu(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// �������� �����
	void CallReport(string_t id);
};
