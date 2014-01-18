#pragma once

#include "ReportAbstract.h"
#include "../msoffice/WordMacro.h"


class ReportCharacteristic : public ReportAbstract
{
public:
  ReportCharacteristic(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:

private:
  void GetPrivateData(r::PrivateData& data, int studentId);

};
