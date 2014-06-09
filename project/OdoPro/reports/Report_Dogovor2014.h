#pragma once
#include "Report_Abstract.h"
#include "../msoffice/WordMacro.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
class ReportDogovor : public ReportAbstract
{
public:
  ReportDogovor(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);
};
