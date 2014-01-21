#pragma once

#include "ReportAbstract.h"
#include "../msoffice/WordMacro.h"


class ReportCharacteristic : public ReportAbstract
{
public:
  ReportCharacteristic(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:
  void     GetBallPercent(int studentId, string_t& otlPercent, string_t& horPercent, string_t& udlPercent);
  string_t CommonItog(int studentId, bool isMale, bool isDirect);
  string_t UrItog(int studentId, bool isMale);
  string_t DirectItog(int studentId, bool isMale);
};
