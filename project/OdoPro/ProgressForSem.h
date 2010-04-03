#pragma once

#include "string_t.h"
#include "litewnd.h"

class CProgressViewBallForSemDlg :	public LiteWnd
{
public:
	CProgressViewBallForSemDlg(LiteWnd* pParent = NULL);
	~CProgressViewBallForSemDlg(void);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
	void SetParam(string_t plan_sem, string_t graph_sem);
private:
	// используются как промежуточные значения
	string_t plan_sem_;
	string_t graph_sem_;
private:
	void InitDomElement(void);	// связывает элементы дом с отображением и инициирует
	// получение и отображение таблицы оценок
	bool UpdateView(void);
	// проверка введенных данных
	bool Check(void);
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает изменение семестров
	static BOOL CALLBACK ElementEventProcSem(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
};
