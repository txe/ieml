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
//-------------------------------------------------------------------------
  string_t to_str_date(string_t sqlDate, bool isYear = false);
  string_t to_digital_date(string_t sqlDate);
  
  inline string_t GetYear(string_t sqlDate)  { return sqlDate.subString(0,4); }
  inline string_t GetMonth(string_t sqlDate) { return sqlDate.subString(5,2); }
  inline string_t GetDay(string_t sqlDate)   { return sqlDate.subString(8,2); }
  
  string_t GetCurrentDate();

  inline string_t toOcenka(int num)
  {
    static string_t ocenkaList[]={"отлично","хорошо","удовлетворительно","неудовлетворительно","зачтено","незачтено"};
    return ocenkaList[num];
  }

//---------------------------------------------------------------------------
  struct PrivateData
  {
    string_t secondName, firstName, thirdName, grpName, bornDate;
    string_t prevEdu, prevDoc, prevDocYear, prevPlace; 
    string_t inYear, inMonth, inDay, outYear, outMonth, outDay, exitDate, exitNum;
    string_t vkrTitle, specOrProfil, specializ, qualific, lang;
    string_t diplomNum, regNum, dataVidachi, dataQualific, direct;
    bool     isMale, isMagister;
  };
  void GetPrivateData(PrivateData& data, int studentId);

}