#pragma once
#include "Report_Abstract.h"
#include "../msoffice/WordMacro.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/


/************************************************************************/
/*                                                                      */
/************************************************************************/
class ReportPeriodSpravka : public ReportAbstract
{
  struct DirectData
{
  enum STROKA_1 {S1_DIRECT, S1_SPEC};
  enum STROKA_2 {S2_SPECIAL, S2_PROFIL, S2_MAGISTR};
  STROKA_1 stroka1;
  STROKA_2 stroka2;
  string_t stroka1Value;
  string_t stroka2Value;
};

struct StudyData
{
  string_t kur;     // курсовые работы
  string_t practic; // практика
  string_t sci;     // научная
  string_t gos;     // государственная аттестация
};

struct DiscipData
{
  string_t title;
  string_t zachet_edinica;
  string_t hours;
  string_t ocenka;

  int discId;
  int semestr;

  DiscipData(string_t t = "") { title = t; }
};

public:
  ReportPeriodSpravka(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:
  int AddProgramTable(WordMacros& macros, const std::vector<DiscipData>& discData);
  int AddProjectTable(WordMacros& macros, const std::vector<DiscipData>& curseWork, int tableNum);
  void GetDirectData(DirectData& data, const r::PrivateData& privData);
  void GetDiscipData(std::vector<DiscipData>& data, std::vector<DiscipData>& curseWork, int studentId, string_t vkr_title, string_t lang);
};
