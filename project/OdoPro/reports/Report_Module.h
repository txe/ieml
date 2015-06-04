#pragma once
#include "Report_Abstract.h"
#include "../msoffice/ExcelMacro.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
class ReportModule: public ReportAbstract
{
private:
  struct ReportModuleData
  {
    string_t dimplomNum1;
    string_t dimplomNum2;
    string_t dimplomDate;
    string_t regNum;
    string_t shifrSpec;
    string_t specOrDir;
    string_t enterYear;
    string_t exitYear;
    string_t groupName;
    string_t secondName;
    string_t firstName;
    string_t thirdName;
    string_t birthDate;
    string_t sex;
  };

public:
  ReportModule(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:
  std::vector<ReportModuleData> GetData(string_t exitDate);
};
