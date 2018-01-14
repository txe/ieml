#pragma once

#include "../string_t.h"

class ReportAbstract
{
public:
  ReportAbstract(string_t name) : m_Name(name) {}
  string_t GetName() { return m_Name; }
  virtual void Run(int grpId, int studentId) = 0;

private:
  string_t m_Name;
};

namespace r
{
  enum DISCIP_TYPE { DT_COMMON = 1,         // ����� ����������
                     DT_CURSE_WORK = 2,     //  �������� ������
                     DT_CURSE_PRACTICE = 3, // �������� ��������
                     DT_PRACTICE = 4,       // ��������
                     DT_ITOG_ATESTACIA = 5, // �������� ���������
                     DT_KVALIFIC_WORK = 6,  // ��������� ���������������� ������
                     DT_DOPOLNIT = 7,       // �������������
                     DT_SCI_WORK = 8        // ������-����������������� �����
  };
//-------------------------------------------------------------------------
  string_t to_str_date(string_t sqlDate, string_t suffix = L"");
  string_t to_digital_date(string_t sqlDate);
  string_t hours_to_str(string_t hours);
  string_t weeks_to_str(string_t weeks);
  
  inline string_t GetYear(string_t sqlDate)  { return sqlDate.empty() ? "0000" : sqlDate.subString(0,4); }
  inline string_t GetMonth(string_t sqlDate) { return sqlDate.empty() ? "00"   : sqlDate.subString(5,2); }
  inline string_t GetDay(string_t sqlDate)   { return sqlDate.empty() ? "00"   : sqlDate.subString(8,2); }
  
  string_t GetCurrentDate(string_t suffix);

  inline string_t toOcenka(int num)
  {
    static string_t ocenkaList[]={"�������","������","�����������������","�������������������","�������","���������"};
    return ocenkaList[num];
  }

//---------------------------------------------------------------------------
  struct PrivateData
  {
    string_t secondName, firstName, thirdName, grpName, bornDate;
    string_t prevEdu, prevDoc, prevDocYear, prevPlace; 
    string_t inYear, inMonth, inDay, outYear, outMonth, outDay, exitDate, exitDateFull, exitNum;
    string_t vkrTitle, specOrProfil, specializ, qualific, lang, shifrspec;
    string_t diplomNum, prilNum, regNum, dataVidachi, dataQualific, direct, gakNum;
    string_t eduformid;
    bool     isMale, isMagister;
    bool     isOtlDiplom;
    string_t specOrProfilTag;
  };
  void GetPrivateData(PrivateData& data, int studentId);

}