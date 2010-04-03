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
public:
	SData(void);
	~SData(void);
public:
	// инициализирует дом елементы и все такое
	void Init(htmlayout::dom::element root);
	// обновляет  дом элементы для текущего студента
	void UpdateView(void);
private:
	// ищет все элементы на вкладке и добавляет их в список
	static BOOL CALLBACK CallbackAtachElement(HELEMENT he, LPVOID param);
	// выбирает город при изменении группы
	static BOOL CALLBACK ElementEventProcChangedGroup(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
private:
	// проверяет наличие существующего номера договора
	bool IsExistsDorovorNumber(void);
	// проверяет на наличие номера зачетки
	bool IsExistsZnum(void);
	// проверяет не фальшивый ли это договор
	bool IsFalseEduDocInfo(void);
	// изменяет все факты оплаты если изменилась группа
	void UpdateAllPayFactsForStudent(void);
	// изменяет все оценки если изменилась специальность
	void UpdateAllProgressForStudent(void);
	// возвращает город для группы
	long GetCityIdForGroup(long GroupId);
	// проверяет корректность введенных данных, корректирует их
	void get_current_value(std::map<string_t, string_t>& value);
public:
	// создает в  базе информацию о новом студенте
	bool CreateData(void);
	// ставит метку, что студент удален
	bool DeleteData(void);
	// сохраняет данные на студента
	bool SaveData(void);
};
