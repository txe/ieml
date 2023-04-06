#pragma once
#include "Report_Abstract.h"
#include "../msoffice/WordMacro.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
class ReportDiplom2023 : public ReportAbstract
{
private:
  struct DirectData
  {
    string_t title1;
    string_t title2;
    string_t title2_2023;
    string_t title2_gak;
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

  struct Practice
  {
    string_t title;
    string_t period;
    int      ocenka;
    int      practice_amount;
    Practice(string_t _title, string_t _period, int _ocenka) : title(_title), period(_period), ocenka(_ocenka), practice_amount(1) {}
  };


public:
  ReportDiplom2023(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:
  void GetDirectData(DirectData& dirData, const r::PrivateData& privData);
  void GetDiscipInfo(int studentId, std::vector<Discip>& commonDiscip, string_t lang, string_t vkrTitle, bool useZe, bool anotherEnd);
  int  PrepareDiscipTitle(string_t& title, int symbolMax);
  bool AddDiscip(std::vector<Discip>& disList, const Discip& discip);
};
