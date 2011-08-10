#pragma once

#include <htmlayout.h>
#include "string_t.h"
#include "LiteWnd.h"

using namespace htmlayout::dom;

class ManagerOfActions
{
public:
	ManagerOfActions(void);
	~ManagerOfActions(void);

private:
	element		_menu;
	LiteWnd*	_parent;

public:
	// ��������������
	void Init(LiteWnd* parent, HELEMENT menu);
private:
	// ������������ ����
	static BOOL CALLBACK ElementEventProcMenu(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ ��������� �������� ��� �������� 
	static BOOL CALLBACK ElementEventProcDelta(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ��������� � ����� ������ ������ ������� ���� ������� �.�. ����� �������
	void FioToClipBoard();

	// ������ �����
	void LoadDelta();
	// ���������� �����
	void SaveDelta();
};
