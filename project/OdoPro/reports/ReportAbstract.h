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
