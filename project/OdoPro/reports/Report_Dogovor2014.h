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
    string_t oplataYears[6];
    string_t extra;
    string_t passport;
    string_t adres1;
    string_t adres2;
    string_t telefon;
    string_t details;

    string_t zakaz_fio;
    string_t zakaz_by;
    string_t zakaz_doc;
    string_t zakaz_details;
  };

public:
  ReportDogovor(string_t name, bool dopSoglashenie) : ReportAbstract(name), m_IsDopSoglashenie(dopSoglashenie) {}
  virtual void Run(int grpId, int studentId);

private:
  ReportDogovorData GetData(int grpId, int studentId);
  string_t          MoneyToStr(int money);

private:
  bool m_IsDopSoglashenie;
};
