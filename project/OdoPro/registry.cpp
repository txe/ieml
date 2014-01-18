#include "stdafx.h"
#include "registry.h"
#include "WinReg.h"
#include <assert.h>


#define CLASS_NAME_LENGTH 255;

Reg::Reg()
{
	m_bDelayedWrite = FALSE;
	m_hRootKey = HKEY_CURRENT_USER;
	m_nLastError = ERROR_SUCCESS;
}

Reg::~Reg()
{
	ClearKey();
}

BOOL Reg::ClearKey()
{
	m_strCurrentPath.clear();
	m_bDelayedWrite = FALSE;
	m_hRootKey = HKEY_CURRENT_USER;

	return TRUE;
}

BOOL Reg::SetRootKey(HKEY hRootKey)
{
	if((hRootKey != HKEY_CURRENT_USER) &&
		(hRootKey != HKEY_LOCAL_MACHINE) &&
		(hRootKey != HKEY_CLASSES_ROOT) &&
		(hRootKey != HKEY_USERS))
		return FALSE;

	m_hRootKey = hRootKey;

	return TRUE;
}

BOOL Reg::CreateKey(std::string strKey)
{
	if(strKey[0] == '\\')
		return FALSE;

	HKEY hKey;
	DWORD dwDisposition = 0;

	m_nLastError = ::RegCreateKeyExA(m_hRootKey, strKey.c_str(), 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey,
		&dwDisposition);

	if(m_nLastError != ERROR_SUCCESS)
		return FALSE;

	if(!m_bDelayedWrite)
		::RegFlushKey(hKey);
	::RegCloseKey(hKey);

	m_strCurrentPath = strKey;

	return TRUE;
}

BOOL Reg::DeleteKey(std::string strKey)
{
	if((strKey[0] == '\\') || (strKey.length() == 0))
		return FALSE;

	HKEY hKey;

	m_nLastError = ::RegOpenKeyExA(m_hRootKey, strKey.c_str(), 0,
		KEY_ALL_ACCESS, (PHKEY)&hKey);

	if(m_nLastError != ERROR_SUCCESS)
		return FALSE;

	m_nLastError = ::RegDeleteKeyA(hKey, strKey.c_str());

	return (m_nLastError == ERROR_SUCCESS);
}

BOOL Reg::DeleteValue(std::string strKey)
{
	if(m_strCurrentPath.length() < 0)
		return FALSE;

	HKEY hKey;

	if(::RegOpenKeyExA(m_hRootKey, m_strCurrentPath.c_str(), 0,
		KEY_SET_VALUE, (PHKEY)&hKey) != ERROR_SUCCESS)
		return FALSE;

	m_nLastError = ::RegDeleteValueA(hKey, strKey.c_str());
	::RegCloseKey(hKey);

	if(m_nLastError != ERROR_SUCCESS)
		return FALSE;
	else
		return TRUE;
}

BOOL Reg::KeyExists(std::string strKeyName, HKEY hRoot)
{
	if(strKeyName[0] == '\\')
		return FALSE;

	HKEY hKey;
	if(hRoot == NULL)
		hRoot = m_hRootKey;

	m_nLastError = ::RegOpenKeyExA(hRoot, strKeyName.c_str(), 0,
		KEY_ALL_ACCESS, (PHKEY)&hKey);
	::RegCloseKey(hKey);

	return (m_nLastError == ERROR_SUCCESS);
}

BOOL Reg::SetKey(std::string strKeyName, BOOL bCanCreate)
{
	if(strKeyName[0] == '\\')
		return FALSE;

	HKEY hKey;
	DWORD dwDisposition;

	if(strKeyName.length() == 0)
	{
		m_strCurrentPath.clear();
		return TRUE;
	}

	if(bCanCreate)
	{
		m_nLastError = ::RegCreateKeyExA(m_hRootKey, strKeyName.c_str(),
			0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
			(PHKEY)&hKey, &dwDisposition);

		if(m_nLastError != ERROR_SUCCESS)
			return FALSE;

		m_strCurrentPath = strKeyName;

		if(!m_bDelayedWrite)
			::RegFlushKey(hKey);

		::RegCloseKey(hKey);

		return TRUE;
	}
	else
	{
		m_nLastError = ::RegOpenKeyExA(m_hRootKey, strKeyName.c_str(),
			0, KEY_ALL_ACCESS, (PHKEY)&hKey);

		if(m_nLastError != ERROR_SUCCESS)
			return FALSE;

		m_strCurrentPath = strKeyName;

		if(!m_bDelayedWrite)
			::RegFlushKey(hKey);

		::RegCloseKey(hKey);

		return TRUE;
	}
}

double Reg::ReadDouble(std::string strKeyName, double fDefault)
{
	DWORD dwType = REG_BINARY;
	double fRes;
	DWORD dwSize = sizeof(fRes);
	HKEY hKey;

	assert(m_strCurrentPath.length() > 0);

	if ((m_nLastError = ::RegOpenKeyExA(m_hRootKey, m_strCurrentPath.c_str(), 0,
		KEY_READ, (PHKEY)&hKey)) != ERROR_SUCCESS) return fDefault;

	if (::RegQueryValueExA(hKey, strKeyName.c_str(), NULL,
		&dwType, (LPBYTE)&fRes, &dwSize) != ERROR_SUCCESS) 
		fRes = fDefault;

	::RegCloseKey(hKey);    

	return fRes;
}

std::string Reg::ReadString(std::string strKeyName, std::string strDefault)
{
	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;
	BOOL bSuccess = TRUE;
	char sz[255];
	HKEY hKey;

	assert(m_strCurrentPath.length() > 0);

	m_nLastError = ::RegOpenKeyExA(m_hRootKey, m_strCurrentPath.c_str(), 0,
		KEY_READ, (PHKEY)&hKey);
	if (m_nLastError != ERROR_SUCCESS) 
		return strDefault;

	m_nLastError = ::RegQueryValueExA(hKey, strKeyName.c_str(), NULL,
		&dwType, (LPBYTE)sz, &dwSize);
	if (m_nLastError != ERROR_SUCCESS) 
		bSuccess = FALSE;
	::RegCloseKey(hKey);    

	if (!bSuccess) 
		return strDefault;

	return std::string(sz);
}

DWORD Reg::ReadDword(std::string strKeyName, DWORD dwDefault)
{
	DWORD dwType = REG_DWORD;
	DWORD dwRes;
	DWORD dwSize = sizeof(dwRes);
	HKEY hKey;

	assert(m_strCurrentPath.length() > 0);

	if ((m_nLastError = ::RegOpenKeyExA(m_hRootKey, m_strCurrentPath.c_str(), 0,
		KEY_READ, (PHKEY)&hKey)) != ERROR_SUCCESS) 
		return dwDefault;

	if (::RegQueryValueExA(hKey, strKeyName.c_str(), NULL,
		&dwType, (LPBYTE)&dwRes, &dwSize) != ERROR_SUCCESS) 
		dwRes = dwDefault;

	::RegCloseKey(hKey);    

	return dwRes;
}

int Reg::ReadInt(std::string strKeyName, int nDefault)
{
	DWORD dwType = REG_BINARY;
	int nRes;
	DWORD dwSize = sizeof(nRes);
	HKEY hKey;

	assert(m_strCurrentPath.length() > 0);

	if ((m_nLastError = ::RegOpenKeyExA(m_hRootKey, m_strCurrentPath.c_str(), 0,
		KEY_READ, (PHKEY)&hKey)) != ERROR_SUCCESS) 
		return nDefault;

	if (::RegQueryValueExA(hKey, strKeyName.c_str(), NULL,
		&dwType, (LPBYTE)&nRes, &dwSize) != ERROR_SUCCESS) 
		nRes = nDefault;

	::RegCloseKey(hKey);    

	return nRes;
}

BOOL Reg::ReadBool(std::string strKeyName, BOOL bDefault)
{
	DWORD dwType = REG_BINARY;
	BOOL bRes;
	DWORD dwSize = sizeof(bRes);
	HKEY hKey;

	assert(m_strCurrentPath.length() > 0);

	if ((m_nLastError = ::RegOpenKeyExA(m_hRootKey, m_strCurrentPath.c_str(), 0,
		KEY_READ, (PHKEY)&hKey)) != ERROR_SUCCESS) 
		return bDefault;

	if (::RegQueryValueExA(hKey, strKeyName.c_str(), NULL,
		&dwType, (LPBYTE)&bRes, &dwSize) != ERROR_SUCCESS) 
		bRes = bDefault;

	::RegCloseKey(hKey);    

	return bRes;
}

BOOL Reg::WriteDouble(std::string strKeyName, double fValue)
{
	assert(m_strCurrentPath.length() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if ((m_nLastError = ::RegOpenKeyExA(m_hRootKey, m_strCurrentPath.c_str(), 0,
		KEY_WRITE, (PHKEY)&hKey)) != ERROR_SUCCESS) 
		return FALSE;

	if (::RegSetValueExA(hKey, strKeyName.c_str(), 0,
		REG_BINARY, (LPBYTE)&fValue, sizeof(fValue)) != ERROR_SUCCESS) 
		bSuccess = FALSE;

	if (!m_bDelayedWrite) 
		::RegFlushKey(hKey);

	::RegCloseKey(hKey);

	return bSuccess;
}

BOOL Reg::WriteString(std::string strKeyName, std::string strValue)
{
	assert(m_strCurrentPath.length() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;
	char sz[255];

	if (strValue.length() > 254) return FALSE;

	strcpy_s(sz, strValue.c_str());

	if ((m_nLastError = ::RegOpenKeyExA(m_hRootKey, m_strCurrentPath.c_str(), 0,
		KEY_WRITE, (PHKEY)&hKey)) != ERROR_SUCCESS) 
		return FALSE;
	if (::RegSetValueExA(hKey, strKeyName.c_str(), 0,
		REG_SZ, (LPBYTE)sz, strlen(sz) + 1) != ERROR_SUCCESS) 
		bSuccess = FALSE;

	if (!m_bDelayedWrite) 
		::RegFlushKey(hKey);

	::RegCloseKey(hKey);

	return bSuccess;
}

BOOL Reg::WriteDword(std::string strKeyName, DWORD dwValue)
{
	assert(m_strCurrentPath.length() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if ((m_nLastError = ::RegOpenKeyExA(m_hRootKey, m_strCurrentPath.c_str(), 0,
		KEY_WRITE, (PHKEY)&hKey)) != ERROR_SUCCESS) 
		return FALSE;

	if (::RegSetValueExA(hKey, strKeyName.c_str(), 0,
		REG_BINARY, (LPBYTE)&dwValue, sizeof(dwValue)) != ERROR_SUCCESS) 
		bSuccess = FALSE;

	if (!m_bDelayedWrite) 
		::RegFlushKey(hKey);

	::RegCloseKey(hKey);

	return bSuccess;
}

BOOL Reg::WriteInt(std::string strKeyName, int nValue)
{
	assert(m_strCurrentPath.length() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if ((m_nLastError = ::RegOpenKeyExA(m_hRootKey, m_strCurrentPath.c_str(), 0,
		KEY_WRITE, (PHKEY)&hKey)) != ERROR_SUCCESS) 
		return FALSE;

	if (::RegSetValueExA(hKey, strKeyName.c_str(), 0,
		REG_BINARY, (LPBYTE)&nValue, sizeof(nValue)) != ERROR_SUCCESS) 
		bSuccess = FALSE;

	if (!m_bDelayedWrite) 
		::RegFlushKey(hKey);

	::RegCloseKey(hKey);

	return bSuccess;
}

BOOL Reg::WriteBool(std::string strKeyName, BOOL bValue)
{
	assert(m_strCurrentPath.length() > 0);
	BOOL bSuccess = TRUE;
	HKEY hKey;

	if ((m_nLastError = ::RegOpenKeyExA(m_hRootKey, m_strCurrentPath.c_str(), 0,
		KEY_WRITE, (PHKEY)&hKey)) != ERROR_SUCCESS) 
		return FALSE;

	if (::RegSetValueExA(hKey, strKeyName.c_str(), 0,
		REG_BINARY, (LPBYTE)&bValue, sizeof(bValue)) != ERROR_SUCCESS) 
		bSuccess = FALSE;

	if (!m_bDelayedWrite) 
		::RegFlushKey(hKey);

	::RegCloseKey(hKey);

	return bSuccess;
}

