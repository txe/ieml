#include "StdAfx.h"
#include <vector>
#include "Report_Dogovor2014.h"
#include "../SingeltonApp.h"

//-------------------------------------------------------------------------
void ReportDogovor::Run(int grpId, int studentId)
{
  WordMacros macros;
  macros.BeginMacros();


  macros.EndMacros();
  macros.RunMacros("diplom-2014.dot");
}
