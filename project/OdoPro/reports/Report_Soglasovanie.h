#pragma once
#include "Report_Abstract.h"
#include "../msoffice/WordMacro.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
class ReportSoglasovanie : public ReportAbstract
{
private:
  struct ReportSoglasovanieData
  {
    string_t title;
    string_t fio;
    string_t diplomNum;
    string_t prilNum;
    string_t day;
    string_t month;
    string_t year;
  };

public:
  ReportSoglasovanie(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:
  ReportSoglasovanieData GetData(int grpId, int studentId);
};
