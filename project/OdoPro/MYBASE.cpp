#include "stdafx.h"

#include "MYBASE.h"
#include "SingeltonApp.h"
#include <iostream>
#include <fstream>
#include "json-aux.h"
#include "logger.h"


mybase::MyBase::MyBase(void):m_con(NULL)
{
}

mybase::MyBase::~MyBase(void)
{
	//if (this->m_con != NULL)
	//	Close();
}

// ���������� � �����
bool mybase::MyBase::Connect(void)
{
	LOG_DEBUG << "����������� � ���� ������";

	if (!LoadSetting())
		return false;

	if ((m_con = mysql_init(NULL)) == NULL)
	{
		LOG_ERROR <<  "������ ��� �������� ������� MySQL";
		LOG_ERROR <<  " host  = " << sHost;
		LOG_ERROR <<  " login = " << sLogin;
		LOG_ERROR <<  " base  = " << sBaseName;
		return false;
	}

	mysql_options(m_con, MYSQL_SET_CHARSET_NAME, "utf8");
	mysql_options(m_con, MYSQL_INIT_COMMAND, "set character_set_client = cp1251");
	mysql_options(m_con, MYSQL_OPT_COMPRESS, NULL);

	if (!mysql_real_connect(m_con, sHost.c_str(), sLogin.c_str(), 
		sPassword.c_str(), sBaseName.c_str(), aux::atoi(sPort.c_str(), 0), NULL, CLIENT_MULTI_STATEMENTS | CLIENT_COMPRESS))
	{
		LOG_ERROR <<  mysql_error(m_con);
		return false;
	}


	if (!CheckBase())
		return false;

	return true;
}

// ������ ���������� �����������
bool mybase::MyBase::LoadSetting()
{
	LOG_DEBUG << "������ ���������� �� ������. �����";
	struct local
	{
		static std::string get(std::string fileName, std::string secName, std::string keyName, std::string defVal)
		{
			char buf[255];
			DWORD count = GetPrivateProfileStringA(secName.c_str(), keyName.c_str(), defVal.c_str(), buf, 245, fileName.c_str());
			if (count != 0)
				return buf;
			return "";
		}	
	};

	std::string cfg_name = theApp.GetModuleDir() + "config.ini";
    sHost     = local::get(cfg_name, "link", "host",     "127.0.0.1");
	sLogin    = local::get(cfg_name, "link", "login",    "app_user");
	sPassword = local::get(cfg_name, "link", "password", "-Nu(q$j0Xnxk");
	sBaseName = local::get(cfg_name, "link", "dbname",   "test");
	sPort     = local::get(cfg_name, "link", "port",     "3306");

	return true;
}

// ��������� ���������� � �����
void mybase::MyBase::Close(void)
{
	LOG_DEBUG << "�������� ����������� � ����";
	mysql_close(m_con);
	//mysql_server_end();
	m_con = NULL;
}

// �������� ��������� ����, ��������� �����������
bool mybase::MyBase::CheckBase(void)
{
	LOG_DEBUG << "�������� ���������� ����";
	string_t tables[] ={
		"disciplines",
		"payfactstest",
		"payoptstest",
		"progress",
		"students",
		"voc"
	};

	mybase::MYFASTRESULT res = Query("SELECT * FROM `students` LIMIT 1");
		
	return true;
}

// ���������, ���� �� ������ � ������������ ��
bool mybase::MyBase::IsError(void)
{
	assert(NULL != m_con);
	if (mysql_errno(m_con))
	{
		LOG_ERROR << mysql_error(m_con);
		return true;
	}
	return false;
}

// ��������� ������, ���������� ���������
// �������� check ����� ��������, ��� ������ exception
MYSQL_RES* mybase::MyBase::Query( const string_t& query, bool check /*= true*/ )
{
	//LOG_WARNING << query;
	//LOG_WARNING << "\n ---------------------------------------- \n";
	assert(m_con != NULL);
	if (0 != mysql_query(m_con, query.c_str()))
	{
		string_t msg = "������������ ������:" + query + "\n" + mysql_error(m_con);
		LOG_ERROR << msg;
		assert(FALSE);

		if (check)
			throw wss::exception(wss::reason_message(FULL_LOCATION(), ""));
	}
	return mysql_store_result(m_con);
}

// ���������� ���������, ��� ����, ����� � �.�.
string_t mybase::MyBase::GetParam(UINT param)
{
	// enum {PR_HOST = 0, PR_PORT, PR_LOGIN, PR_PASS, PR_BD};
	switch(param)
	{
		case PR_HOST:	return sHost;
		case PR_PORT:	return sPort;
		case PR_LOGIN:	return sLogin;
		case PR_PASS:	return sPassword;
		case PR_BD:		return sBaseName;
	}
	assert(FALSE);
	return string_t();
}

// ���������� ���������� ����� ������� ���� ��������� ���������� � �.�. ��. mysql_affected_rows
long mybase::MyBase::AffectedRows(void)
{
	return (long)mysql_affected_rows(GetCon());
}
