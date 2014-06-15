#include "StdAfx.h"
#include <vector>
#include "Report_Dogovor2014.h"
#include "../SingeltonApp.h"

//-------------------------------------------------------------------------
void ReportDogovor::Run(int grpId, int studentId)
{
  WordMacros macros;
  macros.BeginMacros();

  ReportDogovorData data = GetData(grpId, studentId);

  macros.Replace("$DOG_NUM$",   data.dogovorNum);
  macros.Replace("$CUR_TIME$",  data.currentData);
  macros.Replace("$FIO$",       data.fio);
  macros.Replace("$PROGRAMMA$", data.programma);
  macros.Replace("$KVALIF$",    data.kvalif);
  macros.Replace("$SUM1$",      data.numSum);
  macros.Replace("$SUM2$",      data.strSum);
  macros.Replace("$DATA1$",     data.data1);

  macros.EndMacros();
  macros.RunMacros("dogovor-2014.dot");
}
//---------------------------------------------------------------------------
ReportDogovor::ReportDogovorData ReportDogovor::GetData(int grpId, int studentId)
{
  ReportDogovorData data;
  data.dogovorNum = L"123";
  data.currentData  = L"01 августа 2014 г.";
  data.fio = L"Гурьянов Евгений Витальевич";
  data.programma = L"программа";
  data.kvalif = L"студент";
  data.numSum = L"32000";
  data.strSum = L"много денег";
  data.data1  = L"что то где то";

  return data;
}