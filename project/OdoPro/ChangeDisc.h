#pragma once
#include "litewnd.h"

class ChangeDisc : public LiteWnd
{
public:
	ChangeDisc(LiteWnd* pParent = NULL);
	~ChangeDisc(void);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:

	htmlayout::dom::element table1_;
	htmlayout::dom::element table2_;
	long spec_;
private:
	// связывает элементы дом с отображением и инициирует
	void InitDomElement(void);	
	// получение и отображение таблиц дисциплин
	void UpdateTable(void);
	// возвращает выбранную дисциплину
	long GetDisc(htmlayout::dom::element table);	
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// запускает перенос оценок
	void Run();
};
