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

  macros.Replace("$TITLE$",    data.title);
  macros.Replace("$CUR_TIME$", data.date);
 
  macros.EndMacros();
  macros.RunMacros("soglasovanie.dot");
}
//---------------------------------------------------------------------------
ReportSoglasovanie::ReportSoglasovanieData ReportSoglasovanie::GetData(int grpId, int studentId)
{
  r::PrivateData privData;
  r::GetPrivateData(privData, studentId);

  ReportSoglasovanieData data;
  
  data.title = "�� �������� ��������� ���������, ���������� � �����, ������������ �� ��������������� ��������� \"";
  if (privData.specOrProfilTag == "���")
    data.title += privData.direct;
  else
    data.title += privData.specOrProfil;
  data.title += "\" ";
  data.title += privData.secondName + " " + privData.firstName + " " + privData.thirdName;

  data.date = r::GetCurrentDate("");
  return data;
}
