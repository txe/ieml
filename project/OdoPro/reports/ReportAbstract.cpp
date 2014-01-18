#include "stdafx.h"
#include "ReportAbstract.h"
#include "../SingeltonApp.h"

//---------------------------------------------------------------------------
string_t r::to_str_date(string_t sqlDate, bool isYear /*= false*/)
{
  static string_t mounthNames[12]={ "€нвар€", "феврал€", "марта", "апрел€", "ма€", "июн€", "июл€", "августа", "сент€бр€", "окт€бр€", "но€бр€", "декабр€"};
  string_t res = "";
  try
  {
    // число 
    res += sqlDate.subString(8,2);
    if (res.size() == 1) 
      res = "0" + res;
    res += " ";
    // мес€ц 
    int mNum = (sqlDate.subString(5, 2).toInt()-1) % 12;
    if (mNum < 0)
      return "<невалидна€ дата!>";
    res += mounthNames[mNum] + " ";
    // год
    res += sqlDate.subString(0,4);
    if (isYear) 
      res += " года";
  }
  catch(...)
  {
    return "<невалидна€ дата!>";
  }
  return res;
}
//-------------------------------------------------------------------------
string_t r::to_digital_date(string_t sqlDate)
{
  string_t res = "";
  try
  {
    // число 
    res += sqlDate.subString(8,2);
    if (res.size() == 1) 
      res = "0" + res;
    res += ".";
    // мес€ц
    string_t m = sqlDate.subString(5, 2);
    if (m.size() == 1)
      m = m + res;
    res += m + ".";
    // год
    res += sqlDate.subString(0,4);
  }
  catch(...)
  {
    return "<невалидна€ дата!>";
  }
  return res;
}
//---------------------------------------------------------------------------
string_t r::GetCurrentDate()
{
  mybase::MYFASTRESULT res = theApp.GetCon().Query("SELECT CURDATE() as date");
  if (mybase::MYFASTROW	row = res.fetch_row())
    return r::to_str_date(row["date"], true);
  return r::to_str_date("");
}
//---------------------------------------------------------------------------
void r::GetPrivateData(PrivateData& data, int studentId)
{
  data.firstName = data.secondName = data.thirdName = data.bornDate = data.vkrTitle = data.prevEdu = data.prevDoc = data.prevDocYear = data.inYear = "???";
  data.inMonth = data.inMonth = "0";
  data.outYear = data.outMonth = data.outDay = data.exitDate = data.exitNum = "???";
  data.specOrProfil = data.direct = data.specializ = data.qualific = data.lang = "???";
  data.diplomNum = "xxx 000000";
  data.regNum = data.dataVidachi = data.dataQualific = "00.00.0000";
  data.isMagister = false;

  string_t specOrProfilTag;

  string_t  query = string_t() +
    "SELECT s.secondname,s.firstname,s.thirdname,s.grpid,s.bdate,s.vkr_title," \
    "s.educationid,s.edudocid,s.eduenddate,s.eduplace, s.specid,s.enterdate,s.exitdate, s.exitnum,s.sex,v.title as lang,s.edunumdiplom," \
    "s.edunumreg,s.edudatediplom,s.edudatequalif,s.directid " \
    " FROM students as s, "\
    " voc as v where s.deleted=0 and v.deleted=0 and s.id=" + aux::itow(studentId) + 
    " and s.languageid=v.num and v.vkey=\"language\"";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  if (mybase::MYFASTROW	row = res.fetch_row())
  {
    int i1 = row["edudocid"].toInt();
    int i2 = row["edudocid"].toInt();
    int i3 = row["edudocid"].toInt();

    data.secondName = row["secondname"];
    data.firstName  = row["firstname"];
    data.thirdName  = row["thirdname"];
    data.grpName    = theApp.GetTitleForKeyFromVoc(VK_GRP, row["grpid"].toInt(), true);
    data.bornDate   = row["bdate"];
    data.vkrTitle   = row["vkr_title"];
    
    data.prevEdu       = theApp.GetTitleForKeyFromVoc(VK_EDUCATIONS, row["educationid"].toInt(), true);
    data.prevDoc       = theApp.GetTitleForKeyFromVoc(VK_EDUDOC, row["edudocid"].toInt(), true);
    data.prevDocYear   = GetYear(row["eduenddate"]);
    data.prevPlace     = row["eduplace"];
    
    data.specOrProfil  = theApp.GetTitleForKeyFromVoc(VK_SPECS, row["specid"].toInt(), true, &specOrProfilTag);
    data.specializ  = theApp.GetTitleForKeyFromVoc(VK_SPEZIALIZ, row["specid"].toInt(), true);
    data.qualific   = theApp.GetTitleForKeyFromVoc(VK_QUALIFIC, row["specid"].toInt(), true);
    data.direct     = theApp.GetTitleForKeyFromVoc(VK_DIRECT, row["directid"].toInt(), true);
    data.inYear     = GetYear(row["enterdate"]);
    data.inMonth    = GetMonth(row["enterdate"]);
    data.inDay      = GetDay(row["enterdate"]);
    data.outYear    = GetYear(row["exitdate"]);
    data.outMonth   = GetMonth(row["exitdate"]);
    data.outDay     = GetDay(row["exitdate"]);
    data.exitDate   = to_digital_date(row["exitdate"]);
    data.exitNum    = row["exitnum"];
    data.isMale     = row["sex"] != string_t("∆") && row["sex"] != string_t("ж");
    data.lang       = row["lang"];

    data.diplomNum = row["edunumdiplom"];
    data.regNum    = row["edunumreg"];
    data.dataVidachi = to_str_date(row["edudatediplom"]);
    data.dataQualific = to_str_date(row["edudatequalif"]);

    data.isMagister = specOrProfilTag.toUpper().trim() == "маг";
  }
}