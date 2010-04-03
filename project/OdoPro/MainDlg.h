// MainDlg.h : header file
//
#include "htmlayout.h"
#include "LiteWnd.h"
#include "string_t.h"
#include "ManagerPeports.h"
#include "ManagerOfActions.h"

#pragma once


// CMainDlg dialog
class CMainDlg : public LiteWnd
{
	// используюс€ дл€ статус бара
	enum {SB_HOST = 0, SB_LOGIN, SB_BD, SB_FILTR, SB_COUNT, SB_BUILD};
	// тип поиска
	enum FindType {F_FIO, F_ZNUM};
// Construction
public:
	CMainDlg(LiteWnd* pParent = NULL);		// standard constructor
public:
	virtual int		OnCreate();
	virtual BOOL	PreCreateWindow(CREATESTRUCT& cs); 
private:
	htmlayout::dom::element stud_grid_;		// таблица со списком студентов дл€ выбранной группы
	ManagerReports			manag_rep_;
	ManagerOfActions		manag_actions_;
public:
	void UpdateGrid(void);				// обновл€ет таблицу со студентами
	void ShowFindResult();    // показывает результат поиска 
	void ShowFindResultEx();				// показывает результат расширенного поиска
	void SetStatusBar(const UINT& status,const string_t& value); // мен€ет значени€ в статус-баре
	void StudentProperty(void);			// вызывает диалог со свойствами студента
private:
	void InitDomElement(void);			// св€зывает элементы дом с отображением
	void UpdateGrpList(const string_t& dom_name, const string_t& grp_name_filter);	// обновл€ет список групп
	// вызываетс€ при выборе группы
	static BOOL CALLBACK ElementEventProcForPanelGroup(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// вызываетс€ при выборе студента
	static BOOL CALLBACK ElementEventProcForGrid(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обработчик кнопок
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обработчик изменени€ типа поиска
	static BOOL CALLBACK ElementEventProcChangeFindType(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обработчик меню
	static BOOL CALLBACK ElementEventProcMenu(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// создает студента
	void CreateStudent(void);
	// удал€ет студента
	void DeleteStudent(void);
	// возвращает студента который был выбран в таблице
	htmlayout::dom::element GetSelectedStudent(void);
	// возвращает id студента который был выбран в таблице 
	long GetSelectedStudentID(void);
};
