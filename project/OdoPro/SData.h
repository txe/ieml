#pragma once

#include "htmlayout.h"
#include <map>
#include <map>
#include "string_t.h"

class SData
{
private:
	std::map<string_t, htmlayout::dom::element>	map_elements_;

	htmlayout::dom::element	root_;
	htmlayout::dom::element	sel_egediscip_;
	htmlayout::dom::element	table_ege_discip_;
	htmlayout::dom::element	ege_ball_;
public:
	SData(void);
	~SData(void);
public:
	// инициализирует дом елементы и все такое
	void Init(htmlayout::dom::element root);
	// обновл€ет  дом элементы дл€ текущего студента
	void UpdateView(void);
private:
	// ищет все элементы на вкладке и добавл€ет их в список
	static BOOL CALLBACK CallbackAtachElement(HELEMENT he, LPVOID param);
	// выбирает город при изменении группы
	static BOOL CALLBACK ElementEventProcChangedGroup(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает выбор предмета ≈√Ё
	static BOOL CALLBACK ElementEventProcEgeDiscip(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
private:
	// обновл€ет таблицу предметов по ≈√Ё
	void UpdateEgeTable();
	// добавл€ет\измен€ет оценку предмета по ≈√Ё
	void AddEgeDiscip();
	// возвращает текущую дисциплину по еге в таблице
	htmlayout::dom::element GetCurEgeDiscip();
	// удал€ет оценку предмета по ≈√Ё
	void DeleteEgeDiscip();
	// провер€ет наличие существующего номера договора
	bool IsExistsDorovorNumber(void);
	// провер€ет на наличие номера зачетки
	bool IsExistsZnum(void);
	// провер€ет не фальшивый ли это договор
	bool IsFalseEduDocInfo(void);
	// измен€ет все факты оплаты если изменилась группа
	void UpdateAllPayFactsForStudent(void);
	// измен€ет все оценки если изменилась специальность
	void UpdateAllProgressForStudent(void);
	// возвращает город дл€ группы
	long GetCityIdForGroup(long GroupId);
	// провер€ет корректность введенных данных, корректирует их
	void get_current_value(std::map<string_t, string_t>& value);
public:
	// создает в  базе информацию о новом студенте
	bool CreateData(void);
	// ставит метку, что студент удален
	bool DeleteData(void);
	// сохран€ет данные на студента
	bool SaveData(void);
};
