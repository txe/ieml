#pragma once
#include "litewnd.h"
#include <deque>


class CActionFastInputGakDlg :	public LiteWnd
{
public:
	CActionFastInputGakDlg(LiteWnd* pParent = NULL);
	~CActionFastInputGakDlg(void);

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();

private:
	void SetParam(long discip_id, string_t plan_sem, string_t graph_sem, string_t estim);

	// связывает элементы дом с отображением и инициирует
	void InitDomElement(void);	
	// получение и отображение таблицы оценок
	bool UpdateView(void);
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает изменение семестров
	static BOOL CALLBACK ElementEventProcSem(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// сохранение
	void Save(void);
	// задать все гак
	void SetGak();
};
