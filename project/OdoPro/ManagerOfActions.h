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
	// инициализирует
	void Init(LiteWnd* parent, HELEMENT menu);
private:
	// обрабатывает меню
	static BOOL CALLBACK ElementEventProcMenu(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает изменение смещения для договора 
	static BOOL CALLBACK ElementEventProcDelta(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// загружает в буфер обмена список фамилий вида Фамилия И.О. номер зачетки
	void FioToClipBoard();

	// чтение ключа
	void LoadDelta();
	// сохранение ключа
	void SaveDelta();
};
