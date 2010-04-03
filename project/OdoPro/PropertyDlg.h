#pragma once

#include "LiteWnd.h"
#include "SData.h"
#include "htmlayout.h"
#include "SProgress.h"
#include "SPayment.h"
#include <map>
#include "string_t.h"

// CPropertyDlg dialog

class CPropertyDlg : public LiteWnd
{
private:
	/*
	Содержит в себе пары: ("id"  "позиция") и ("-позиция" "id")
	*/
	std::map<long, long>	students_map_;

	htmlayout::dom::element	header_;
	SData					s_data_; // данные на студента
	SProgress				s_progress_; // успеваемость студента
	SPayment				s_payment_;

public:
	CPropertyDlg(LiteWnd* pParent = NULL);   // standard constructor
	virtual ~CPropertyDlg();
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:
	void InitDomElement(void);	// связывает элементы дом с отображением и инициирует
	// получение и отображение свойств студента
	bool UpdateView(void);
	// инициализирует список студентов
	void InitStudIds(void);
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms );
	// перемещает курсор на следующего студента в списке
	void NextStudent(void);
	// перемещает курсор на предыдущего студента в списке 
	void PrevStudent(void);
	// сохранение
	void Save(void);
	// возвращает ид студента по позиции в группе
	long GetIdFromPos(const long& stud_pos);
	// возращает позицию студента в группе по ииндификатору
	long GetPosFromId(const long& stud_id);
public:
	// создает нового студента для текущей группы
	bool CreateData(void);
	// удаляет данные о студенте
	bool DeleteData(void);
};
