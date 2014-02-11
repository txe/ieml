#include "StdAfx.h"
#include <vector>
#include "Report_Diplom.h"
#include "../SingeltonApp.h"

//-------------------------------------------------------------------------
void ReportDiplom::Run(int grpId, int studentId)
{
  r::PrivateData privData;
  r::GetPrivateData(privData, studentId);

  WordMacros macros;
  macros.BeginMacros();

  // ФИО
  macros.Cell(1, 3, 7, "Range.Select");
  macros.SelectionText(privData.secondName);
  macros.Cell(1, 4, 7, "Range.Select");
  macros.SelectionText(privData.firstName);
  macros.Cell(1, 5, 7, "Range.Select");
  macros.SelectionText(privData.thirdName);

  // дата рождения
  macros.Cell(1, 6, 8, "Range.Select");
  macros.SelectionText(r::to_str_date(privData.bornDate, true));
  
  macros.EndMacros();
  macros.RunMacros("diplom-2014.dot");
}