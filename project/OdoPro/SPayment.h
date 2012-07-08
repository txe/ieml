#pragma once

#include <htmlayout.h>
#include "string_t.h"

using namespace htmlayout::dom;

class SPayment
{
	
public:
	SPayment(void);
	~SPayment(void);
private:
	element	_root;
	element	_payment_table; // список дисциплин
	element _cat_month;
	element _cat_year;
	element _cat_money;
	element _cat_half_year;

public:
	// инициализирует дом объекты и все такое
	void Init(htmlayout::dom::element root);
private:
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает выбор дисциплины
	static BOOL CALLBACK ElementEventProcForPayTable(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
public:
	// обновляет на экране успеваимость студента
	void UpdateView(void);
	void UpdateViewPayment();
private:
	// переводит дату из формата sql в формат  требуемый в оплатах
	string_t DateToPayFormat(const string_t& pay_date);
	// сохраняет оплату студента
	void AddPay(void);
	// обновляет строку состояния
	void UpdateViewStatus(void);
	// проверяет введенную оплату на корректность
	bool CheckInputPay(void);
	// возвращает текущиую категорию оплаты
	element GetCurrentCat(void);
	// удалить оплату
	void DeletePay(void);
	// обновляет инфе о выбранной категории
	void UpdateViewCat(void);
	// добавить категорию оплаты
	void AddCat(void);
	// проверяет существует ли такая категория оплаты
	bool IsExistCat(bool include_cur_cat = true);
	// возвращает даты начала и конца периода оплаты
	void GetDateCat(string_t& startdate, string_t& enddate);
	// сохраняет изменения категории оплаты
	void SaveUpdateCat(void);
	// удаляет выбранную категорию
	void DeleteCat(void);
	// задает для выбранной категории собственную оплату студента
	void PersonalCat();
	// заполняет поле оплаты разными вариантами оплаты
	void InitPayPay(void);
	// показать только оплаты
	void ShowOnlyPay(void);
};
