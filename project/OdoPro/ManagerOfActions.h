#pragma once

#include <htmlayout.h>
#include "string_t.h"
#include "LiteWnd.h"

using namespace htmlayout::dom;

struct IActionParent
{
  virtual void IActionParent_UpdateWindow() = 0;
};

class ManagerOfActions
{
private:
	element		     _menu;
	LiteWnd*	     _parent;
  IActionParent* _actionParent;

public:
	// ��������������
	void Init(LiteWnd* parent, HELEMENT menu, IActionParent* actionParent);

private:
	// ������������ ����
	static BOOL CALLBACK ElementEventProcMenu(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ ��������� �������� ��� �������� 
	static BOOL CALLBACK ElementEventProc(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ��������� � ����� ������ ������ ������� ���� ������� �.�. ����� �������
	void FioToClipBoard();

	void SerializeDelta(bool toSave);
  void SerializeShowDogNum(bool toSave);
};
