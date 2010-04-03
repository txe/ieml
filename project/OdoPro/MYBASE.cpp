#include "stdafx.h"

#include "MYBASE.h"
#include "SingeltonApp.h"
#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include "json-aux.h"
#include "logger.h"

namespace po = boost::program_options;


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

	if (!LoadSetting(string_t("odopro.cfg")))
		return false;

	if ((m_con = mysql_init(NULL)) == NULL)
	{
		LOG_ERROR <<  "������ ��� �������� ������� MySQL";
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
bool mybase::MyBase::LoadSetting( const string_t& cfg_name )
{
	LOG_DEBUG << "������ ���������� �� ������. �����";
	// ���������� �������� �������� �� ����������
	try {
		po::options_description generic("Generic options");
		generic.add_options()
			("host", po::value<std::string>(&sHost)->default_value("127.0.0.1"), "")
			("login", po::value<std::string>(&sLogin)->default_value("app_user"), "")
			("password", po::value<std::string>(&sPassword)->default_value("-Nu(q$j0Xnxk"), "")
			("dbname", po::value<std::string>(&sBaseName)->default_value("test"), "")
			("port", po::value<std::string>(&sPort)->default_value("3306"), "")
			;
		po::variables_map vm;        
		std::ifstream ifs(cfg_name.c_str());
		if (ifs.fail())
		{
			LOG_WARNING << "������. ���� '" << cfg_name << "'�� ������, ������������ ��������� �� ���������";
			assert(FALSE);
		}
		store(parse_config_file(ifs, generic), vm);
		notify(vm);
	}
	catch (...)
	{
		if ( -1 == theApp.ExceptionManage())
			LOG_ERROR << "����������� ������ ��� ������ ������. �����";
		return false;
	}
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
//	LOG_WARNING << query;
	assert(m_con != NULL);
	if (0 != mysql_query(m_con, query.c_str()))
		if (check)
		{
			string_t msg = "������������ ������:" + query + "/n" + mysql_error(m_con);
			LOG_ERROR << msg;
			assert(FALSE);
			throw wss::exception(wss::reason_message(FULL_LOCATION(), ""));
		}
		else
			assert(FALSE);
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
