#pragma once
#include "Report_Abstract.h"
#include "../msoffice/WordMacro.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
class ReportDiplom : public ReportAbstract
{
private:
  struct DirectData
  {
    string_t title1;
    string_t title2;
    string_t title3;
    string_t bottomInfo;
  };
  struct Discip
  {
    string_t title;
    string_t period;
    string_t ocenka;
    int      numPlan;
    Discip(string_t _title, string_t _period, string_t _ocenka, int _numPlan = 0) : title(_title), period(_period), ocenka(_ocenka), numPlan(_numPlan) {}
  };

public:
  ReportDiplom(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:
  void GetDirectData(DirectData& dirData, const r::PrivateData& privData);
  void GetDiscipInfo(int studentId, std::vector<Discip>& cursDiscip, std::vector<Discip>& commonDiscip, std::vector<Discip>& specDiscip, string_t lang, string_t vkrTitle, bool useZe);
  int  PrepareDiscipTitle(string_t& title, int symbolMax);
  void AddDiscip(std::vector<Discip>& disList, const Discip& discip);
};
