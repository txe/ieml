#pragma once
#include "litewnd.h"
#include <htmlayout.h>
#include "string_t.h"

using namespace htmlayout::dom;

class CDictionary :
	public LiteWnd
{
public:
	CDictionary(LiteWnd* parent = NULL);
	~CDictionary(void);
public:
	element _table;
	element _title;
	element _num;
	element _filter;
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual int	 OnCreate();
private:
	void InitDomElement(void);	// связывает элементы дом с отображением и инициирует
	// получение и отображение таблицы
	void UpdateView(void);
	// отображает выбор
	void ShowSelection();
	// обрабатывает кнопки
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// обрабатывает выбор
	static BOOL CALLBACK ElementEventProcForTable(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	static BOOL CALLBACK ElementEventProcChangedFilter(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);

	//
	void Add(void);
	void Delete(void);
	void Save(void);
	// вовращает следующий индиф. номер для записи раздела vkey, проверяет и возвращает предпочтительный номер
	string_t GetNextNumForKey( const string_t& vkey, const string_t default_num = "-1" );
	// 
	//
	bool IsExistsTitleForKey(bool exclude_cur = false);
	bool IsExistsNumForKey(bool exclude_cur = false);
	string_t GetRecordId(bool throw_ = true);
};
