#include "StdAfx.h"
#include <vector>
#include "Report_Diplom2015.h"
#include "../SingeltonApp.h"
#include "../json-aux-ext.h"

//-------------------------------------------------------------------------
void ReportDiplom2015::Run(int grpId, int studentId)
{
  WordMacros macros;
  macros.BeginMacros();

  ReportDiplim2015Data data = GetData(grpId, studentId);

  macros.Replace("$FIO_1$",        data.fio1);
  macros.Replace("$FIO_2$",        data.fio2);
  macros.Replace("$SPEC$",         data.spec);
  macros.Replace("$KVALIF$",       data.kvalif);
  macros.Replace("$GAK_NUM$",      data.gakNum);
  macros.Replace("$D1$",           data.kvalifDay);
  macros.Replace("$D2$",           data.kvalifMonth);
  macros.Replace("$D3$",           data.kvalifYear);
  macros.Replace("$REG_NUM$",      data.regNum);
  macros.Replace("$DIPLOM_DATE$", data.diplomDate);

  macros.EndMacros();
  macros.RunMacros("diplom-2015.dot");
}
//---------------------------------------------------------------------------
ReportDiplom2015::ReportDiplim2015Data ReportDiplom2015::GetData(int grpId, int studentId)
{
  r::PrivateData privData;
  r::GetPrivateData(privData, studentId);

  ReportDiplim2015Data data;

  data.fio1   = privData.secondName;
  data.fio2   = privData.firstName + " " + privData.thirdName;
  data.kvalif = privData.qualific.toLower();
  data.gakNum = privData.gakNum;
  data.regNum = privData.regNum;

  // уберем часть шифра, если там только одна точка
  data.spec = privData.shifrspec;
  int dotPos = data.spec.indexOf(L".");
  if (dotPos != -1)
    if (-1 == data.spec.subString(dotPos + 1, -1).indexOf(L"."))  
      data.spec = data.spec.subString(0, dotPos);
  if (privData.direct.empty())
    data.spec += " " + privData.specOrProfil;
  else
    data.spec += " " + privData.direct;

  data.kvalifDay = data.kvalifMonth = data.kvalifYear = "??";
  if (privData.dataQualific.indexOf(L"!") == -1)
  {
    std::vector<std::wstring> splitDate = aux::split((const wchar_t*)privData.dataQualific, ' ');
    data.kvalifDay   = splitDate[0];
    data.kvalifMonth = splitDate[1];
    data.kvalifYear  = splitDate[2].substr(2);
  }
  data.diplomDate = r::to_str_date(privData.dataVidachi, "года");

  return data;
}
