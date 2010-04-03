#pragma once

#include "MYBASE.h"
#include <json-aux.h>
#include <Winsock2.h>

#define BEGIN_MAN_MAP(CLASS_NAME) \
	CLASS_NAME() \
	{ \
		InternalSaveLoad(false, true); \
	} \
	virtual bool InternalSaveLoad(bool bSave, bool bOnlyInit) \
	{ \
		bool result = false; \
		if(bSave && !bOnlyInit) \
			OnBeforeSave();

#define END_MAN_MAP() \
		if (!bSave && !bOnlyInit) \
			OnAfterLoad(); \
		return result; \
	}

#define LOC_ITEM(variable, DEFAULT_VALUE) \
		if (bOnlyInit) \
			variable = DEFAULT_VALUE; \
		else \
		{ \
			result = SaveLoadItem((#variable), variable, true, bSave); \
			if (!result) \
				if(!bSave) \
					variable = DEFAULT_VALUE; \
		}

#define GLOB_ITEM(variable, DEFAULT_VALUE) \
		if (bOnlyInit) \
			variable = DEFAULT_VALUE; \
		else \
		{ \
			result = SaveLoadItem((#variable), variable, false, bSave); \
			if (!result) \
				if(!bSave) \
					variable = DEFAULT_VALUE; \
		}


class ManagerSettingImpl
{
public:
	mybase::MyBase		_con;   // подключение к базе данных
	std::string			_table; // таблица где хранятся данные
	std::string			_local_tag;	// метка, для локальных настроек
	const std::string	_global_tag;
public:
	ManagerSettingImpl(void):_global_tag("global") {};
	~ManagerSettingImpl(void) { };
public:
	virtual void OnBeforeSave(void) {};
	virtual void OnAfterLoad(void) {};
	virtual bool InternalSaveLoad(bool bSave, bool bInit){ return false;};
public:
	bool load(mybase::MyBase& con, std::string table)
	{ 
		_con	= con; 
		_table	= table;
		struct hostent *host = gethostbyname("localhost"); // char * name - имя сервера
		_local_tag	= host->h_name; 
		return InternalSaveLoad(false, false); 
	}

	bool save()
	{  
		return InternalSaveLoad(true, false); 
	}
public:
//	заготовки  
	inline bool LoadItem(const std::string name, std::string& variable,const bool tag)
	{
		std::string query = "SELECT variable FROM " + _table + 
			" WHERE name = '" + name + "' AND tag = '" + 
			(tag?_local_tag:_global_tag) + "' LIMIT 1";
		mybase::MYFASTRESULT res = _con.Query(query);
		if (!res.size())
			return false;
		variable = res.fetch_row()["variable"];
		return true;
	}

	inline bool SaveItem(const std::string name, std::string& variable, const bool tag)
	{
		if (!tag)
			return true;

		std::string var;
		std::string query;
		if (LoadItem(name, var, tag))
			query = "UPDATE " +  _table + " SET variable = '" + _con.escape_string(variable) + "' WHERE name = '" + name + 
			"' AND tag = '" + (tag?_local_tag:_global_tag) + "' LIMIT 1";
		else
			query = "INSERT INTO  " +  _table + "(name, variable, tag) VALUES('" + name + "', "
			"'" +  _con.escape_string(variable) + "', '" + (tag?_local_tag:_global_tag) + "')";
		_con.Query(query);
		return _con.AffectedRows() > 0;
	}
public:
	// реализации
//  INT
	inline bool SaveLoadItem(LPCSTR name, int& variable, bool tag, bool bSave)
	{
		std::string var;
		bool		res;
		if (bSave)
		{
			var = aux::itoa(variable);
			res = SaveItem(name, var, tag);
		}
		else
		{
			res = LoadItem(name, var, tag);
			variable = aux::atoi(var.c_str());
		}
		return res;
	}
//  std::string
	inline bool SaveLoadItem(LPCSTR name, std::string& variable, bool tag, bool bSave)
	{
		if (bSave)
			return SaveItem(name, variable, tag);
		else
			return LoadItem(name, variable, tag);
	}
//  string_t
	inline bool SaveLoadItem(LPCSTR name, string_t& variable, bool tag, bool bSave)
	{
		std::string var = variable;
		bool res = SaveLoadItem(name, var, tag, bSave);
		variable = var;
		return res;
	}
//  std::wstring
	inline bool SaveLoadItem(LPCSTR name, std::wstring& variable, bool tag, bool bSave)
	{
		string_t var = variable;
		bool res = SaveLoadItem(name, var, tag, bSave);
		variable = var;
		return res;
	}

};


