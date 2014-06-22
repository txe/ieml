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
	// инициализирует
	void Init(LiteWnd* parent, HELEMENT menu, IActionParent* actionParent);

private:
	// обрабатывает меню
	static BOOL CALLBACK ElementEventProcMenu(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает изменение смещения для договора 
	static BOOL CALLBACK ElementEventProc(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// загружает в буфер обмена список фамилий вида Фамилия И.О. номер зачетки
	void FioToClipBoard();

	void SerializeDelta(bool toSave);
  void SerializeShowDogNum(bool toSave);
};
