#pragma once
#include "Report_Abstract.h"
#include "../msoffice/ExcelMacro.h"
#include <map>

/************************************************************************/
/*                                                                      */
/************************************************************************/
class ReportModule: public ReportAbstract
{
private:
  struct ReportModuleData
  {
    string_t dimplomNum1;
    string_t dimplomNum2;
    string_t dimplomDate;
    string_t regNum;
    string_t shifrSpec;
    string_t specOrDir;
    string_t enterYear;
    string_t exitYear;
    string_t groupName;
    string_t secondName;
    string_t firstName;
    string_t thirdName;
    string_t birthDate;
    string_t sex;
  };
  
  struct voc_data
  {
    struct voc_value
    {
      string_t title;
      string_t tag;
      voc_value(string_t _title, string_t _tag) : title(_title), tag(_tag) {}
    };
    std::map<int, voc_value> data;

    void add(string_t key, string_t title, string_t tag)
    {
      data.insert(std::make_pair(key.toInt(), voc_value(title, tag)));
    }
    voc_value find(string_t key) 
    {
      std::map<int, voc_value>::iterator it = data.find(key.toInt());
      if (it == data.end())
        return voc_value("", "");
      return it->second;
    }
  };

public:
  ReportModule(string_t name) : ReportAbstract(name) {}
  virtual void Run(int grpId, int studentId);

private:
  std::vector<ReportModuleData> GetData(string_t exitDate);
  voc_data GetVocData(string_t vkey);
};
