#pragma once
#include "Report_Abstract.h"
#include "../msoffice/WordMacro.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
class ReportDiplom2015 : public ReportAbstract
{
private:
  struct ReportDiplim2015Data
  {
    string_t fio1;
    string_t fio2;
    string_t spec;
    string_t kvalif;
    string_t gakNum;
    string_t kvalifDay;
    string_t kvalifMonth;
    string_t kvalifYear;
    string_t regNum;
    string_t diplomDate;
  };
public:
  ReportDiplom2015(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:
  ReportDiplim2015Data GetData(int grpId, int studentId);
};
