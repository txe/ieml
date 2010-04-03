#pragma once

#include <string>
namespace wss
{

class location
{
public:
	static const char* file_basename(const char * fullFileName)
	{
		const char * baseName = fullFileName;
		if ((baseName = strrchr(fullFileName, '\\')) != NULL)
			return (const char*)(baseName + 1);
		return fullFileName;
	}

	explicit location(const char* file_location, const std::string& line,const std::string& function = "")
	{
		location_ = std::string(file_location) + "[" + line + "]:(" + function + "): ";
	}

	virtual ~location()
	{
	}
	friend std::ostream& operator << (std::ostream& stream, const location& value) { return stream << value.location_; };

public:
	std::string location_;
};

// преобразование числа в строку
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define CURR_LOCATION() wss::location(__FILE__, TOSTRING(__LINE__))
#define THIS_LOCATION() wss::location(wss::location::file_basename(__FILE__), TOSTRING(__LINE__))
#define FULL_LOCATION() wss::location(wss::location::file_basename(__FILE__), TOSTRING(__LINE__), __FUNCSIG__)

class reason_message
{
public:
	reason_message(const std::string& descrip):message_(descrip) {}
	
	explicit reason_message(const wss::location& loc, const std::string& descrip = "")
		:message_(loc.location_ + descrip) {}

	~reason_message() {}

	const std::string& get_message() const { return message_; }  
private:
	std::string message_;
};

class exception : public std::exception
{
public:
	exception() throw() {}

	explicit exception(const wss::reason_message& reason):std::exception(reason.get_message().c_str())
	{
	}

	virtual ~exception()  { }

};
}