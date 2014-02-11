#pragma once
#include "Report_Abstract.h"
#include "../msoffice/WordMacro.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
class ReportDiplom : public ReportAbstract
{
public:
  ReportDiplom(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);
};
