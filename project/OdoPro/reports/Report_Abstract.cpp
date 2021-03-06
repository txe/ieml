#include "stdafx.h"
#include "Report_Abstract.h"
#include "../SingeltonApp.h"

//---------------------------------------------------------------------------
string_t r::to_str_date(string_t sqlDate, string_t suffix /* = L"" */)
{
  static string_t mounthNames[12]={ "������", "�������", "�����", "������", "���", "����", "����", "�������", "��������", "�������", "������", "�������"};
  string_t res = "";
  try
  {
    // ����� 
    res += sqlDate.subString(8,2);
    if (res.size() == 1) 
      res = "0" + res;
    res += " ";
    // ����� 
    int mNum = (sqlDate.subString(5, 2).toInt()-1) % 12;
    if (mNum < 0)
      return "<���������� ����!>";
    res += mounthNames[mNum] + " ";
    // ���
    res += sqlDate.subString(0,4);
    if (suffix.size()) 
      res += L" " + suffix;
  }
  catch(...)
  {
    return "<���������� ����!>";
  }
  return res;
}
//-------------------------------------------------------------------------
string_t r::to_digital_date(string_t sqlDate)
{
  string_t res = "";
  try
  {
    // ����� 
    res += sqlDate.subString(8,2);
    if (res.size() == 1) 
      res = "0" + res;
    res += ".";
    // �����
    string_t m = sqlDate.subString(5, 2);
    if (m.size() == 1)
      m = m + res;
    res += m + ".";
    // ���
    res += sqlDate.subString(0,4);
  }
  catch(...)
  {
    return "<���������� ����!>";
  }
  return res;
}
//-------------------------------------------------------------------------
string_t r::hours_to_str(string_t hours)
{
  int num = hours.toInt();
  if (num >= 5 && num <= 20)       hours += " �����"; // ���������� �� ������
  else
  {
    num %= 10;
    if (num == 1)                  hours += " ���";
    else if (num >= 2 && num <= 4) hours += " ����";
    else                           hours += " �����";
  }
  return hours;
}
//-------------------------------------------------------------------------
string_t r::weeks_to_str(string_t weeks)
{
  int num = weeks.toInt();
  if (num >= 5 && num <= 20)       weeks += " ������"; // ���������� �� ������
  else
  {
    num %= 10;
    if (num == 1)                  weeks += " ������";
    else if (num >= 2 && num <= 4) weeks += " ������";
    else                           weeks += " ������";
  }
  return weeks;
}
//---------------------------------------------------------------------------
string_t r::GetCurrentDate(string_t suffix)
{
  mybase::MYFASTRESULT res = theApp.GetCon().Query("SELECT CURDATE() as date");
  if (mybase::MYFASTROW	row = res.fetch_row())
    return r::to_str_date(row["date"], suffix);
  return r::to_str_date("");
}
//---------------------------------------------------------------------------
void r::GetPrivateData(PrivateData& data, int studentId)
{
  data.firstName = data.secondName = data.thirdName = data.bornDate = data.vkrTitle = data.prevEdu = data.prevDoc = data.prevDocYear = data.inYear = "???";
  data.inMonth = data.inMonth = "0";
  data.outYear = data.outMonth = data.outDay = data.exitDate = data.exitDateFull = data.exitNum = "???";
  data.specOrProfil = data.direct = data.specializ = data.qualific = data.lang = data.gakNum = "???";
  data.diplomNum = "xxx 000000";
  data.regNum = data.dataVidachi = data.dataQualific = "00.00.0000";
  data.isMagister = false;
  data.eduformid = "???";

  string_t  query = string_t() +
    "SELECT s.secondname,s.firstname,s.thirdname,s.grpid,s.bdate,s.vkr_title," \
    " s.educationid,s.edudocid,s.eduenddate,s.eduplace, s.specid,s.enterdate,s.exitdate, s.exitnum,s.sex,v.title as lang, s.edunumdiplom, " \
    " s.eduformid, s.edunumprilog, s.edunumreg, s.edudatediplom,s.edudatequalif,s.directid,s.edudiplomotl, s.edunumprotgak " \
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
    data.grpName    = theApp.GetTitleForKeyFromVoc(vok_key::VK_GRP, row["grpid"].toInt(), true);
    data.bornDate   = row["bdate"];
    data.vkrTitle   = row["vkr_title"];
    
    data.prevEdu       = theApp.GetTitleForKeyFromVoc(vok_key::VK_EDUCATIONS, row["educationid"].toInt(), true);
    data.prevDoc       = theApp.GetTitleForKeyFromVoc(vok_key::VK_EDUDOC, row["edudocid"].toInt(), true);
    data.prevDocYear   = GetYear(row["eduenddate"]);
    data.prevPlace     = row["eduplace"];
    
    data.specOrProfil  = theApp.GetTitleForKeyFromVoc(vok_key::VK_SPECS, row["specid"].toInt(), true, &data.specOrProfilTag);
    data.specializ  = theApp.GetTitleForKeyFromVoc(vok_key::VK_SPEZIALIZ, row["specid"].toInt(), true);
    data.qualific   = theApp.GetTitleForKeyFromVoc(vok_key::VK_QUALIFIC, row["specid"].toInt(), true);
    data.direct     = theApp.GetTitleForKeyFromVoc(vok_key::VK_DIRECT, row["directid"].toInt(), true);
    data.inYear     = GetYear(row["enterdate"]);
    data.inMonth    = GetMonth(row["enterdate"]);
    data.inDay      = GetDay(row["enterdate"]);
    data.outYear    = GetYear(row["exitdate"]);
    data.outMonth   = GetMonth(row["exitdate"]);
    data.outDay     = GetDay(row["exitdate"]);
    data.exitDate   = to_digital_date(row["exitdate"]);
    data.exitDateFull = to_str_date(row["exitdate"]);
    data.exitNum    = row["exitnum"];
    data.isMale     = row["sex"] != string_t("�") && row["sex"] != string_t("�");
    data.lang       = row["lang"];
    data.isOtlDiplom = row["edudiplomotl"] == "1";
    data.eduformid  = row["eduformid"];

    data.diplomNum    = row["edunumdiplom"];
    data.prilNum      = row["edunumprilog"];
    data.regNum       = row["edunumreg"];
    if (data.regNum.empty())
      data.regNum = "<���������� �����!>";
    data.dataVidachi  = row["edudatediplom"];
    data.dataQualific = to_str_date(row["edudatequalif"]);
    data.gakNum = row["edunumprotgak"];

    if (data.direct.empty())
      data.shifrspec = theApp.GetTitleForKeyFromVoc(vok_key::VK_SHIFRSPEC, row["specid"].toInt(), true);
    else
      data.shifrspec = theApp.GetTitleForKeyFromVoc(vok_key::VK_SHIFRSPEC, row["directid"].toInt(), true);

    data.isMagister = data.specOrProfilTag.toLower().trim() == "���";
  }
}