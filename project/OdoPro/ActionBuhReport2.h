#pragma once
#include "litewnd.h"
#include <htmlayout.h>
#include "string_t.h"

using namespace htmlayout::dom;

class CActionBuhReport2 :
	public LiteWnd
{
public:
	CActionBuhReport2(LiteWnd* parent = NULL);
	~CActionBuhReport2(void);

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();

private:
	void InitDomElement(void);	// связывает элементы дом с отображением и инициирует
	void FullGrpInLst();
	void FullSpecInLst();
	void CreateBuhData();
	void ProcessPlan();
	void ProcessPay();
	string_t GetSpecLst(string_t name);
	string_t GetGrpLst(string_t name);
	string_t GetRange(string_t name);

	// 
	void Report(void);
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает выбор
	static BOOL CALLBACK ElementEventProcFor(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
};
