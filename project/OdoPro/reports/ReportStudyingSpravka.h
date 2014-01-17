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
  bool isMale;
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
  void GetStudyData(StudyData& data, int studentId, bool isMale);
  void GetDiscipData(std::vector<DiscipData>& data, int studentId);
  string_t CurrentDate();
};
