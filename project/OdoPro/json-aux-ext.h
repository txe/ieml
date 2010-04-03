#pragma once

#include <json-aux.h>
#include "string_t.h"
#include <boost\algorithm\string.hpp>
#include <vector>
#include <float.h>
#include "SingeltonApp.h"
#include <mbstring.h>


enum { CHECK_ALL = (FIRST_APPLICATION_EVENT_CODE + 1) };


namespace t
{
	/*	таблица кодов и переводов:
		код			описание		балл
		0			отл.(экз.)		= 5.0 - = 4.5
		1			хор.(экз.)		< 4.5 - = 3.5
		2			уд.(экз.)		< 3.5 - = 2.5
		3			неуд.(экз.)     < 2.5
		4			зачет			= 5.0 - = 2.5
		5			незачет			< 2.5
		также используется короткий вариант кода (/ тип сданного испытания: экз\зачет)
		type		описание
		0			эзамен
		1			зачет
	
	*/
	 

	//	переводит из код оценки в описание оценки
	inline string_t cod2text(const string_t& cod)
	{
		string_t types[6] = {L"отл.", L"хор.", L"уд.", L"неуд.", L"зач.", L"незач."};
		int cod_ = aux::wtoi(cod, -1);
		if (cod_ < 0 || cod_ > 5)
		{
			assert(FALSE);
			string_t msg =	"Пожалуйста, сообщите немедленно разработчику об этой ошибке.\n"
							"Приложение не закрываейте.\n";
			MessageBox(::GetActiveWindow(), msg , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			msg = "ошибка перевода кода в тип. cod = " + cod;
			throw wss::exception(wss::reason_message(FULL_LOCATION(), msg.c_str()));
		}

		return types[cod_];
	}


	// переводит код описания в тип испытания
	inline int cod2type(const int cod_est)
	{
		if (cod_est >= 0 && cod_est <= 3) // экзамен
			return 0;
		if (cod_est >= 4 && cod_est <= 5) // зачет
			return 1;

		assert(FALSE);
		string_t msg =	"Пожалуйста, сообщите немедленно разработчику об этой ошибке.\n"
			"Приложение не закрываейте.\n";
		MessageBox(::GetActiveWindow(), msg , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		msg = string_t("ошибка перевода кода описаняи в тип испытания. cod_est = ") + aux::itow(cod_est); 
		throw wss::exception(wss::reason_message(FULL_LOCATION(), msg.c_str()));
	}
	// переводит тип описания испытания + оценка в код
	inline int type2cod(const int type_est,const double ball)
	{
		if (type_est == 0)  // экзамен
		{
			if ( ball <= 5.0 && ball >= 4.5)
				return 0; // отл
			if ( ball < 4.5 && ball >= 3.5)
				return 1; // хор
			if ( ball <= 3.5 && ball >= 2.5)
				return 2; // уд
			if ( ball <= 2.5 && ball >= 0)
				return 3; // неуд
			assert(FALSE);
			throw wss::exception(wss::reason_message(FULL_LOCATION()));
		}
		if (type_est == 1)
		{
			if ( ball <= 5.0 && ball >= 2.5)
				return 4; // зачет
			if ( ball < 2.5 && ball >= 0)
				return 4; // незачет
			assert(FALSE);
			throw wss::exception(wss::reason_message(FULL_LOCATION()));
		}
		assert(FALSE);
		throw wss::exception(wss::reason_message(FULL_LOCATION()));
	}



	// формат даты в SQL уууу-mm-dd
	// проверяет валидность даты и коректирует ее
	inline bool date2t(const string_t& date, string_t& rez)
	{
		std::vector<std::wstring> result;
		std::wstring buf = date;
		boost::split(result, buf, boost::is_any_of(L"-"));
		// год
		int year = aux::wtoi((result.size() > 0)?result[0].c_str():L"0", 0);
		int month = aux::wtoi((result.size() > 1)?result[1].c_str():L"0", 0);
		int day = aux::wtoi((result.size() > 2)?result[2].c_str():L"0", 0);

		if (year > 2015)
			year = 0;
		if (month > 12)
			month = 0;
		if (day > 31)
			day = 0;
		
		rez = string_t(aux::itow(year)) + "-" + aux::itow(month) + "-" + aux::itow(day);
		return date == rez;
	}
	// получает год
	inline string_t get_year(const string_t& date)
	{
		std::vector<std::wstring> result;
		std::wstring buf = date;
		boost::split(result, buf, boost::is_any_of(L"-"));
		return result[0];
	}
	// переводит балл в формат из x,yz  в x.y0 для совместимости с SQL
	inline string_t ball2sql(const wchar_t* ball_)
	{
	//	wchar_t buf[40];
	//	::wmemset(buf, '0', sizeof(buf));
	//	wcscpy(ball_, buf);
		return string_t() + wchar_t(ball_[0]) + L"." + wchar_t(ball_[2]) + L"0";
	}
	// возвращает полное название дисциплины по его индентификаитору
	inline string_t GetFullDiscipTitle(const long& discip_id)
	{
		string_t query = string_t() + " SELECT fulltitle FROM disciplines "
			" WHERE `deleted` = 0 AND "
			" `id` = " + aux::itow(discip_id) + " LIMIT 1 ";

		mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
		mybase::MYFASTROW	 row = res.fetch_row();

		return row["fulltitle"];
	}

	// загружает в список из таблицы voc согласно спецификатору 
	inline void LoadContentFromVocForList(htmlayout::dom::element &el, const string_t &vkey)
	{
		htmlayout::dom::element option	= el.find_first("popup");
		assert(option.is_valid());
		// удаляем все строки
		while (option.children_count())
			HTMLayoutDetachElement(el.child(0));

		string_t query = string_t() +
			" SELECT num , title FROM voc " +
			" WHERE vkey= '" + vkey + "' AND deleted = 0 " +
			" ORDER BY title, id";
		mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

		string_t			buf;
		mybase::MYFASTROW	row;

		while ((row = res.fetch_row()))
		{
			string_t title = row["title"];
			if (title.empty())
				title = "&nbsp;";
			buf += "<option value=" + row["num"] + ">" + title + "</option>";
		}
		assert(_mbslen(buf));
		if (_mbslen(buf))
			option.set_html(buf, _mbslen(buf), SIH_REPLACE_CONTENT);
		option.update(false);
		//LOG_DEBUG << string_t(el.get_html());
	}

	// получает выбранную строку в таблице
	inline htmlayout::dom::element GetSelectedRow(htmlayout::dom::element table)
	{
		assert(table.is_valid());// && aux::streq(table.get_element_type(), "table"));

		htmlayout::dom::element row;
		for (UINT i = 0; i < table.children_count(); ++i)
		{
			htmlayout::dom::element t = table.child(i);
			if (t.get_state(STATE_CURRENT))
			{
				row = t;
				break;
			}
		}	
		if (!row.is_valid())
		{
			assert(FALSE);
			throw wss::exception(wss::reason_message(FULL_LOCATION()));
		}
		return row;
	}

	// переводит в rsync формат
	inline std::string path2rsync(const std::string& path)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];

		_splitpath(path.c_str(), drive, dir, 0, 0);
		std::string rsync_path = (!drive[0]?std::string():(std::string("/cygdrive/") + drive[0])) + dir;
		std::replace(rsync_path.begin(), rsync_path.end(), '\\', '/');

		return rsync_path;
	}
}

namespace aux
{

	// (c) Den Chetverikov
	inline bool Parse(const char* string, long double& var)
	{
		bool minus(false);
		bool sign_awaited(true);
		int fpt = -1;     // A number of digits after floating point or (-1).
		int exp = 0;
		int dig = 0;      // A number of decimal digits
		long double x = 0;
		char c;           // A current character

		while ((c = *string++),(c != 0))
		{
			if (c == ' ') continue;

			if (sign_awaited && (c == '+' || c == '-'))
			{
				minus = (c != '+');
				sign_awaited = false;
				continue;
			}
			if (fpt < 0 && (c == '.' || c == ','))
			{
				fpt = 0;
				sign_awaited = false;
				continue;
			}
			if (dig > 0 && (c == 'e' || c == 'E'))
			{
				int e = 0;     // An exponent
				int edig = 0;  // A number of decimal digits in exponent
				bool eminus = false;
				sign_awaited = true;
				while ((c = *string++),(c != 0))
				{
					if (c == ' ') continue;
					if (sign_awaited && (c == '+' || c == '-'))
					{
						eminus = (c != '+');
						sign_awaited = false;
						continue;
					}
					if (c < '0' || c > '9') return false;
					e = e * 10 + (c - '0');
					++edig;
				}
				if (edig == 0) return false;
				exp += (eminus) ? -e : e;
				break;
			}

			if (c < '0' || c > '9') return false;
			sign_awaited = false;
			if (x != 0 || dig == 0) ++dig;

			if (dig >= LDBL_DIG)
			{
				if (fpt < 0) ++exp;
			}
			else
			{
				x = x * 10 + (c - '0');
				if (fpt >= 0) ++fpt;
			}
		}

		if (dig == 0) return false;

		if (fpt >= 0) exp = exp - fpt;
		while (exp > 0)
		{
			if (x > LDBL_MAX / 10) return false;
			x *= 10;
			--exp;
		}
		while (exp < 0)
		{
			x /= 10;
			++exp;
		}
		var = minus ? -x : x;
		return true;
	}

	// переводит строку в double
	inline long double strtod(const char* str, long double def = 0.0)
	{
		long double value;
		if (!Parse(str, value))
			return def;
		return value;
	}
}

namespace json
{
	// возвращает для списков
	inline string_t get_caption(const htmlayout::dom::element& el)
	{
		htmlayout::dom::element find;
		find = el.find_first("caption");
		if (!find.is_valid())
			find = el.find_first("option:current");
		if (!find.is_valid())
		{
			assert(false);
			throw wss::exception(wss::reason_message(FULL_LOCATION()));
		}
		return string_t(find.get_text());
	}
	// 
	inline string_t v2t(const json::value& value)
	{
		switch (value.type())
		{
			case json::value::V_BOOL:      return value.get(false)?"1": "0";
			case json::value::V_INT:       return string_t(aux::itow(value.get(0)));
			case json::value::V_UNDEFINED: return value.get(L"");
			case json::value::V_STRING:    return value.get(L"");
			case json::value::V_REAL:      return string_t(aux::ftow(value.get(0.0)));
		}
		assert(false);
		return string_t();
	}

	inline string_t v2t(const htmlayout::dom::element& el)
	{
		return json::v2t(el.get_value());
	}


	inline void t2v(htmlayout::dom::element& el, const string_t& value)
	{
		switch (el.get_value().type())
		{
			case json::value::V_BOOL:     
				el.set_value(json::value(aux::streq(value.c_str(), "1")?true:false));
				break;
			case json::value::V_INT:       
				el.set_value(json::value(aux::wtoi(value)));
				break;
			case json::value::V_UNDEFINED: 
				el.set_value(json::value(aux::wtoi(value)));
				if (json::v2t(el.get_value()) != value)
					el.set_value(json::value((const wchar_t*)value));
				break;
			case json::value::V_STRING:    
				el.set_value(json::value((const wchar_t*)value));
				break;
			case json::value::V_REAL:    
				el.set_value(json::value((double)aux::strtod(value)));
				break;
		}
	}
}