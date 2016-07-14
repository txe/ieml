#pragma once
#include "litewnd.h"


class CSelectGroupsDlg :	public LiteWnd
{
public:
	CSelectGroupsDlg(LiteWnd* pParent);
	~CSelectGroupsDlg(void);

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();

private:
  void SetGroups();

  // связывает элементы дом с отображением и инициирует
	void InitDomElement(void);	

	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);

public:
  std::vector<string_t> groups;
};

