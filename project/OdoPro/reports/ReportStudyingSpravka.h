#pragma once
#include "ReportAbstract.h"
#include "../msoffice/WordMacro.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/

struct PrivateData
{
  string_t secondName, firstName, thirdName, bornDate;
  string_t prevDoc, prevDocYear; 
  string_t inYear, inMonth, inDay, outYear, outMonth, outDay;
  string_t vipQualifWork, specOrProfil, specializ, qualific, lang;
  string_t diplomNum, regNum, dataVidachi, dataQualific, direct;
  bool male;
};

/************************************************************************/
/*                                                                      */
/************************************************************************/
class ReportStudyingSpravka : public ReportAbstract
{
public:
  ReportStudyingSpravka(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:
  void GetPrivateData(PrivateData& data, int studentId);
};
