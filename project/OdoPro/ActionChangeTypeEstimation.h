#pragma once
#include "litewnd.h"
#include <htmlayout.h>

using namespace htmlayout::dom;

class CActionChangeTypeEstimation :
	public LiteWnd
{
public:
	CActionChangeTypeEstimation(LiteWnd* parent = NULL);
	~CActionChangeTypeEstimation(void);
private:
	element _table;
	element _exam;
	element _test;
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:
	void InitDomElement(void);	// связывает элементы дом с отображением и инициирует
	// получение и отображение таблицы
	void UpdateView(void);
	// отображает выбор
	void ShowSelection();
	// 
	void Save(void);
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает выбор
	static BOOL CALLBACK ElementEventProcForTable(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
};
