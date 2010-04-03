#pragma once

#include <htmlayout.h>

class SProgress
{
public:
	SProgress(void);
	~SProgress(void);
private:
	htmlayout::dom::element	_root;
	htmlayout::dom::element	_discip; // список дисциплин
public:
	// инициализирует дом объекты и все такое
	void Init(htmlayout::dom::element root);
private:
	// заполняем список дисциплин
	void LoadListDiscip(void);
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает выбор дисциплины
	static BOOL CALLBACK ElementEventProcForDiscip(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает выбор балла
	static BOOL CALLBACK ElementEventProcForBall(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
public:
	// обновляет на экране успеваимость студента
	void UpdateView(void);
private:
	// получает выбранную дисциплину
	long GetIDSelectedDiscip(void);
	// обновляет таблицу оценок для выбранного предмета
	void UpdateViewEstimForDiscip();
	// сохраняет оценку для дисциплины текущего студента
	void SaveBall(void);
	// удаляет выбранный балл
	void DeleteBall(void);
public:
	// проверяет на корректность введенный балл
	int CheckValue(void);
private:
	// возвращает ид выбранного бала
	long GetIDSelectedBall(void);
public:
	// вызывает окно быстрого ввода оценок для всей группы для данной дисциплины
	void FastInput(void);
	// показвает дисциплины за семестр
	void ProgressForSem(void);
	// показывает диалог редактирования дисциплин
	void EditDiscip(void);
};
