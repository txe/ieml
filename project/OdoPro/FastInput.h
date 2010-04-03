#pragma once
#include "litewnd.h"
#include <deque>


class CProgressFastInputBallDlg :
	public LiteWnd
{
public:
	CProgressFastInputBallDlg(LiteWnd* pParent = NULL);
	~CProgressFastInputBallDlg(void);
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
public:
	void SetParam(long discip_id, string_t plan_sem, string_t graph_sem, string_t estim);
private:
	long discip_id_;
	// используется хранения оценок при копирования и вставке
	static std::deque<json::value>	_buf_ball;
	// используются как промежуточные значения
	string_t plan_sem_;
	string_t graph_sem_;  
	string_t estim_;
private:
	// связывает элементы дом с отображением и инициирует
	void InitDomElement(void);	
	// получение и отображение таблицы оценок
	bool UpdateView(void);
	// проверка введенных данных
	bool Check(void);
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает изменение семестров
	static BOOL CALLBACK ElementEventProcSem(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// сохранение
	void Save(void);
	// копирует оценки в буфер
	void CopyToBuf();
	// втавляет оценки из буфера
	void InsertFromBuf();
};
