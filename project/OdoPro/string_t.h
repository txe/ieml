#pragma once

#include <json-aux.h>
#include <ostream>
#include <string>

class string_t
{
private:
	std::wstring _str;
	mutable std::string  _buf;
public:
	string_t(void) {};
	~string_t(void){};
// конструкторы копирования
	string_t(const string_t& value)		 { _str = value._str;		}
	string_t(const std::string& value)	 { _str = std::wstring(aux::a2w(value.c_str()));  }
	string_t(const std::wstring& value)	 { _str = value;			}
	string_t(const char* value)			 { _str = std::wstring(aux::a2w(value));  }
	string_t(const wchar_t* value)  	 { _str = std::wstring(value);			}  
	string_t(const unsigned char* value) { _str = std::wstring(aux::utf2w(value));}

	bool					empty() const { return _str.empty(); }
	void					erase(int off, int count) { _str.erase(off, count); }
	std::wstring::size_type	size() const { return _str.size(); }
	//std::wstring::size_type length() { return _str.length(); }
	const char* c_str() const { return (const char*)(*this); }
//

//
	const string_t& operator=(const string_t& value) { if (&value != this) _str = value._str; return *this; }
	const string_t& operator=(const std::string& value) { _str = std::wstring(aux::a2w(value.c_str())); return *this; }
	const string_t& operator=(const std::wstring& value){ _str = value; return *this; }
	const string_t& operator=(const char* value) { _str = std::wstring(aux::a2w(value)); return *this; }
	const string_t& operator=(const wchar_t* value) { _str = std::wstring(value); return *this; }
	const string_t& operator=(const unsigned char* value) { _str = std::wstring(aux::utf2w(value)); return *this; }

	const string_t& operator+=(const string_t& value) { _str += value._str; return *this; }
	//const string_t& operator+=(const std::string& value) { _str += std::wstring(aux::a2w(value.c_str())); return *this; }
	//const string_t& operator+=(const std::wstring& value){ _str += value; return *this; }
	//const string_t& operator+=(const char* value) { _str += std::wstring(aux::a2w(value)); return *this; }
	//const string_t& operator+=(const wchar_t* value) { _str += std::wstring(value); return *this; }
	//const string_t& operator+=(const unsigned char* value) { _str += std::wstring(aux::utf2w(value)); return *this; }

	friend string_t operator+(const string_t& str1, const string_t& str2) { return string_t(str1._str + str2._str); }
	friend string_t operator+(const string_t& str1, const std::string& str2) { return string_t(str1._str + aux::a2w(str2.c_str())); }
	friend string_t operator+(const string_t& str1, const std::wstring& str2){ return string_t(str1._str + str2); }
	friend string_t operator+(const string_t& str1, const char* str2) { return string_t(str1._str + std::wstring(aux::a2w(str2))); }
	friend string_t operator+(const string_t& str1, const wchar_t* str2) { return string_t(str1._str + std::wstring(str2)); }
	friend string_t operator+(const string_t& str1, const wchar_t  str2) 
	{ 
		std::wstring t_;
		t_.push_back(str2);
		return string_t(str1._str + t_); 
	}
	friend string_t operator+(const string_t& str1, const unsigned char* str2) { return string_t(str1._str + std::wstring(aux::utf2w(str2))); }

	friend bool operator < (const string_t& str1, const string_t& str2) { return str1._str < str2._str; }
	friend bool operator == (const string_t& str1, const string_t& str2) { return str1._str == str2._str; }
	friend bool operator == (const string_t& str1, const char* str2) { return str1 == string_t(str2); }
	friend bool operator != (const string_t& str1, const string_t& str2) { return str1._str != str2._str; }
	friend std::ostream& operator << (std::ostream& stream, const string_t& value) { return stream << (const char*)value; };
	
	operator const char*() const 
	{
		_buf = aux::w2a(_str.c_str());
		return _buf.c_str();
	}
	operator const wchar_t*() const 
	{
		return _str.c_str();
	}
	operator const unsigned char*() const 
	{
		_buf = (char*)(const byte*)aux::w2utf(_str.c_str());
		return (unsigned char*)_buf.c_str();	
	}

	int indexOf(const std::wstring& str) const
	{
		return _str.find(str);
	}

	string_t subString(int begin, int count) const
	{
		return _str.substr(begin, count);
	}

};
