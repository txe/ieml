#pragma once
#include "litewnd.h"


class CSelectYearDlg :	public LiteWnd
{
public:
	CSelectYearDlg(LiteWnd* pParent, string_t year);
	~CSelectYearDlg(void);

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();

private:
	void SetParam(long discip_id, string_t plan_sem, string_t graph_sem, string_t estim);

	// связывает элементы дом с отображением и инициирует
	void InitDomElement(void);	

	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);

public:
  string_t _year;
};

