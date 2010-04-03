#pragma once
#include "litewnd.h"
#include <htmlayout.h>

using namespace htmlayout::dom;

class CActionChangeSem :
	public LiteWnd
{
public:
	enum TypeSem {PLAN_SEM, GRAPH_SEM};
private:
	TypeSem _type_sem;
private:
	element _table;
	element _cur_sem;
	element _new_sem;
public:
	CActionChangeSem(LiteWnd* parent = NULL);
	~CActionChangeSem(void);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
	void		 SetTypeSem(TypeSem type_sem) { _type_sem = type_sem; };
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
	// обрабатывает выбор предмета/семестра
	static BOOL CALLBACK ElementEventProcForTable(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
};
