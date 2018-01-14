#pragma once

#include "Report_Abstract.h"
#include "../msoffice/WordMacro.h"
#include <vector>


class ReportPril4 : public ReportAbstract
{
private:
  struct Data
  {
    string_t fio;
    string_t birth_date;
    string_t order;
    string_t order_date;
    string_t prev_doc;
  };
public:
  ReportPril4(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:
  string_t GetTitle(int studentId);
  std::vector<Data> GetData(int grpId);
};
