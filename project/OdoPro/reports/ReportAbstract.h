#pragma once
#include "../string_t.h"
class ReportAbstract
{
public:
  ReportAbstract(string_t name, string_t tip) : m_Name(name), m_Tip(tip) {}
  string_t GetName() { return m_Name; }
  string_t GetTip()  { return m_Tip;  }

  virtual void Run(int grpId, int studentId) = 0;
private:

  string_t m_Name;
  string_t m_Tip;
};
