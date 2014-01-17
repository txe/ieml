#pragma once

#include "string_t.h"
#include "litewnd.h"
#include <vector>

class CManagDisciplinesDlg :
	public LiteWnd
{
public:
	CManagDisciplinesDlg(LiteWnd* pParent = NULL);
	~CManagDisciplinesDlg(void);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:
	htmlayout::dom::element	discip_table_;
	htmlayout::dom::element	fulltitle_;
	htmlayout::dom::element	shorttitle_;
	htmlayout::dom::element	discip_class_;
	htmlayout::dom::element	scan_namber_;
	htmlayout::dom::element	num_hours_;
	htmlayout::dom::element	spec_;
  htmlayout::dom::element	zach_ed_;


	std::vector<htmlayout::dom::element> sem_hours_;
private:
	void InitDomElement(void);	// связывает элементы дом с отображением и инициирует
	// получение и отображение таблицы оценок
	bool UpdateView(int showDisc);
	// проверка введенных данных
	bool Check(void);
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает выбор дисциплины
	static BOOL CALLBACK ElementEventProcDiscip(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms );
	// обновляет таблицу дисциплин
	void UpdateViewDiscipTable(int showDisc);
	// отображает данные на выбранную дисциплину
	void UpdateViewDiscipData(void);
	// удаляет дисциплину из базы
	void DeleteDiscip(void);
	// проверяет существует ли дисциплина с такими параметрами
	bool IsExistDiscip(bool include_cur_discip = true);
	// создает новую дисциплину
	void AddDiscip(void);
	// сохраняет изменение дисциплины
	void SaveUpdateDiscip(void);
	// переводит аудиторные часы в строку
	string_t AudHoursToStr(void);
};
