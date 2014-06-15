#pragma once
#include "Report_Abstract.h"
#include "../msoffice/WordMacro.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
class ReportDogovor : public ReportAbstract
{
private:
  struct ReportDogovorData
  {
    string_t dogovorNum;
    string_t currentData;
    string_t fio;
    string_t kod;
    string_t forma;
    string_t kvalif;
    string_t numSum;
    string_t strSum;
    string_t oplata[6];
    string_t passport;
    string_t adres1;
    string_t adres2;
    string_t telefon;
  };

public:
  ReportDogovor(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:
  ReportDogovorData GetData(int grpId, int studentId);
  string_t          MoneyToStr(int money);
};
