#pragma once

#include <string>

#define HKCU HKEY_CURRENT_USER
#define HKLM HKEY_LOCAL_MACHINE
#define HKCR HKEY_CLASSES_ROOT
#define HKU HKEY_USERS

class Reg
{
public:
	Reg();
	~Reg();

protected:
	HKEY		m_hRootKey;
	BOOL		m_bDelayedWrite;
	std::string m_strCurrentPath;
	int			m_nLastError;

public:
	inline int			GetLastClassError() { return m_nLastError; }
	inline BOOL			IsPathValid() { return (m_strCurrentPath.length() > 0); }
	inline std::string	GetCurrentPath() { return m_strCurrentPath; }
	inline HKEY			GetRootKey() { return m_hRootKey; }
	inline void			SetDelayedWrite(BOOL bDelayed = FALSE) { m_bDelayedWrite = bDelayed; }
	inline BOOL			GetDelayedWrite() { return m_bDelayedWrite; }

	BOOL ClearKey();
	BOOL SetRootKey(HKEY hRootKey);
	BOOL CreateKey(std::string strKey);
	BOOL DeleteKey(std::string strKey);
	BOOL DeleteValue(std::string strKey);
	BOOL SetKey(std::string strKeyName, BOOL bCanCreate = TRUE);
	BOOL KeyExists(std::string strKeyName, HKEY hRoot = NULL);

	double ReadDouble(std::string strKeyName, double fDefault = 0.0);    
	std::string ReadString(std::string strKeyName, std::string strDefault = std::string(""));
	int ReadInt(std::string strKeyName, int nDefault = 0);
	BOOL ReadBool(std::string strKeyName, BOOL bDefault = FALSE);
	DWORD ReadDword(std::string strKeyName, DWORD dwDefault = 0);

	BOOL WriteDouble(std::string strKeyName, double fValue = 0.0);
	BOOL WriteString(std::string strKeyName, std::string strValue = std::string(""));
	BOOL WriteInt(std::string strKeyName, int nValue = 0);
	BOOL WriteBool(std::string strKeyName, BOOL bValue = FALSE);
	BOOL WriteDword(std::string strKeyName, DWORD dwValue = 0);
};

