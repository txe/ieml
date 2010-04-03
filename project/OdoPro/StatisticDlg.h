#pragma once
#include "litewnd.h"

class CStatisticDlg :
	public LiteWnd
{
public:
	CStatisticDlg(LiteWnd* pParent = NULL);   // standard constructor
	virtual ~CStatisticDlg(void);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:
	void InitDomElement(void);	// связывает элементы дом с отображением и инициирует
	void ShowStatistic(void);	// показывает статистику по выбранным критериям
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms );
};
