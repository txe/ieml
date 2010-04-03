#pragma once

#include <list>
#include <map>
#include <vector>
#include <mysql.h>
#include <memory>
#include "string_t.h"
#include "wss_exeption.h"

namespace mybase
{

typedef std::map<string_t, int>		        COLUMS;
typedef std::vector<string_t>				ROW;
typedef std::tr1::shared_ptr<ROW>				PtrRow;
typedef std::vector<PtrRow>					TABLE;
typedef std::pair<string_t, int>			Col_Pair;
typedef std::tr1::shared_ptr<COLUMS>			PtrCol;
typedef std::tr1::shared_ptr<TABLE>			PtrTab;
class WRAPPER_RES;
typedef std::tr1::shared_ptr<WRAPPER_RES>		PtrRes;

//////////////////////////////////////////////////////////////////////////

// строчка для работы с MYFASTRESULT
class MYFASTROW
{
private:
	PtrCol		colums_;
	MYSQL_ROW	row_;
public:
	MYFASTROW():row_(NULL) {;};
	MYFASTROW(PtrCol& colums, MYSQL_ROW row):colums_(colums),row_(row){ ;};
	operator bool () const
	{
		return NULL != row_;
	};
	string_t operator[](const char* col_name)
	{ 
		assert(row_);
		COLUMS::const_iterator col =  colums_->find(col_name);
		if (col == colums_->end())
		{
			assert(FALSE);
			string_t msg = "нет такого поля '" + string_t(col_name) + "'";
			throw wss::exception(wss::reason_message(FULL_LOCATION(), msg.c_str()));
		}
		return (const unsigned char*)(row_[col->second]?row_[col->second]:""); 
	};
};
// является враппером MYSQL_RES
class WRAPPER_RES
{
	friend class MYFASTRESULT;
	friend class MYFASTROW;
private:
	PtrCol		columns_;  // именнованные колонки таблицы
	MYSQL_RES*  result_;   // содержимое запроса
public:
	WRAPPER_RES():result_(NULL){};
	WRAPPER_RES(MYSQL_RES* result):result_(result) 
	{
		if (!result)
			return;
		int num_fields = mysql_num_fields(result);
		if (!num_fields)
			return;
		columns_ = PtrCol(new COLUMS);
		for (int i = 0; i < num_fields; ++i)
			columns_->insert(Col_Pair((const unsigned char*)mysql_fetch_field_direct(result, i)->name, i));
	}
	~WRAPPER_RES()
	{
		if (result_)
			 mysql_free_result(result_);
	}
};
// работа с результатом напрямую

class MYFASTRESULT 
{
private:
	PtrRes res_;
public:
	MYFASTRESULT():res_(new WRAPPER_RES) {};
	~MYFASTRESULT()	{ };
	MYFASTRESULT(const MYFASTRESULT& myresult):res_(myresult.res_) {};
	MYFASTRESULT(MYSQL_RES* result):res_(new WRAPPER_RES(result)) {};
	
	const MYFASTRESULT& operator=(const MYFASTRESULT& myresult)
	{
		if (&myresult != this) 
			res_ = myresult.res_; 
		return *this; 
	}

	my_ulonglong size() 
	{ 
		if (NULL == res_->result_)
		{
			assert(res_->result_); 
			return 0;
		}
		return mysql_num_rows(res_->result_); 
	};
	operator bool () const { return NULL != res_->result_; };
	MYFASTROW fetch_row(void) 
	{ 
		return MYFASTROW(res_->columns_, mysql_fetch_row(res_->result_));	
	};
};


//////////////////////////////////////////////////////////////////////////
// различные парамметры
enum {PR_HOST = 0, PR_PORT, PR_LOGIN, PR_PASS, PR_BD};

// работа с базой данных
class MyBase
{
public:
	MyBase(void);
	~MyBase(void);
private:
	MYSQL*  m_con;

	std::string sBaseName;
	std::string sHost;
	std::string sLogin;
	std::string sPassword;
	std::string sPort;
public:
	bool Connect(void);		// соединение с базой
	void Close(void);		// закрывает соединение с базой
	bool IsError(void);		// проверяет, есть ли ошибка
	MYSQL_RES*	Query(const string_t& query, bool check = true);	// выполняет запрос, возвращает результат
	MYSQL*		GetCon(void) { return m_con; };		// возвращает дескриптор соединения
	string_t	GetParam(UINT param);        // возвращает параметры, как хост, логтн и т.д.
private:
	bool LoadSetting(const string_t& cfg_name);	// чтение параметров подключения
	bool CheckBase(void);							// проверка состояния базы	
public:
	// возвращает количество строк которые были затронуты измнениями и т.д. см. mysql_affected_rows
	long AffectedRows(void);
	std::string escape_string(const std::string& str)
	{
		char buf[1024];
		mysql_real_escape_string(m_con, buf, str.c_str(), str.size());
		return buf;
	}
};
}
