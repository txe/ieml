#pragma once
#include "Report_Abstract.h"
#include "../msoffice/WordMacro.h"
#include <map>


/************************************************************************/
/*                                                                      */
/************************************************************************/
class ReportReleaseDoc : public ReportAbstract
{
  struct student
  {
    string_t name;
    string_t dogovor;
    string_t protocol;

    friend bool operator < (const student& s1, const student& s2)    { return s1.name < s2.name; }
  };
  struct profil
  {
    string_t part1;
    string_t part2;
    std::vector<student> good_student;
    std::vector<student> just_student;
  };
  

public:
  ReportReleaseDoc(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:
  void GetData(const std::vector<string_t>& groups, std::map<string_t, profil>& data);
  std::map<string_t, string_t> GetVocTable(string_t name);
  string_t GetVocValue(const std::map<string_t, string_t>& table, string_t key);
  void AddTable(WordMacros& macros, std::vector<student>& lst, int& table_count);
};
