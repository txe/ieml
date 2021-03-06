#include "StdAfx.h"
#include <vector>
#include "Report_Soglasovanie.h"
#include "../SingeltonApp.h"

//-------------------------------------------------------------------------
void ReportSoglasovanie::Run(int grpId, int studentId)
{
  WordMacros macros;
  macros.BeginMacros();

  ReportSoglasovanieData data = GetData(grpId, studentId);

  macros.Replace("$TITLE$",      data.title);
  macros.Replace("$FIO$",        data.fio);
  macros.Replace("$DIPLOM_NUM$", data.diplomNum);
  macros.Replace("$PRIL_NUM$",   data.prilNum);
  macros.Replace("$DAY$",        data.day);
  macros.Replace("$MONTH$",      data.month);
  macros.Replace("$YEAR$",       data.year);
 
  macros.EndMacros();
  macros.RunMacros(theApp.GetModuleDir() + "soglasovanie.dot");
}
//---------------------------------------------------------------------------
ReportSoglasovanie::ReportSoglasovanieData ReportSoglasovanie::GetData(int grpId, int studentId)
{
  r::PrivateData privData;
  r::GetPrivateData(privData, studentId);

  ReportSoglasovanieData data;
  
  data.title = "��������� ���������� �������, ������������ �� ��������������� ��������� ";
  if (privData.specOrProfilTag.startsWith("���"))
    data.title += "\"\"" + privData.direct + "\"\"";
  else
    data.title += "\"\"" + privData.specOrProfil + "\"\"";

  data.fio       = privData.secondName + " " + privData.firstName + " " + privData.thirdName;
  data.diplomNum = privData.diplomNum;
  data.prilNum   = privData.prilNum;
  
  string_t date = r::GetCurrentDate("");
  if (date.indexOf(L"!") == -1) // ���� ���� ���������� ��������
  {
    data.day   = date.subString(0, 2);
    data.month = date.subString(3, date.size() - 3 - 4 - 1);
    data.year  = date.subString(date.size() - 4, 4);
  }

  return data;
}
