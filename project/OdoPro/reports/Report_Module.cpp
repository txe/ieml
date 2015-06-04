#include "StdAfx.h"
#include <vector>
#include "Report_Module.h"
#include "../SingeltonApp.h"
#include <sys/timeb.h>
#include <sstream>
#include <iomanip>

//-------------------------------------------------------------------------
void ReportModule::Run(int grpId, int studentId)
{
  std::vector<ReportModuleData> dataLst = GetData("2015");
  string_t text;
 // text = "Серия документа;Номер документа;Дата выдачи;Регистрационный номер;Код специальности/направление подготовки;Наименование специальности/направление обучения;Год поступления;Год окончания;Группа;Фамилия получателя;Имя получателя;Отчество получателя;Дата рождения получателя;Пол получателя\n";
  string_t dlm = "\t";
  for (size_t i = 0; i < dataLst.size(); ++i)
  {
    const ReportModuleData& data = dataLst[i];
    text += 
      data.dimplomNum1 + dlm +
      data.dimplomNum2 + dlm +
      data.dimplomDate + dlm +
      data.regNum + dlm +
      data.shifrSpec + dlm +
      data.specOrDir + dlm +
      data.enterYear + dlm +
      data.exitYear + dlm +
      data.groupName + dlm +
      data.secondName + dlm +
      data.firstName + dlm +
      data.thirdName + dlm +
      data.birthDate + dlm +
      data.sex + "\n";
  }

  std::wstring text2 = text;
  if (!OpenClipboard(NULL))
    return;

  EmptyClipboard();
  char* buf = (char*) LocalAlloc(LMEM_FIXED, (text2.size()+1)*sizeof(wchar_t));
  if (buf == NULL)
  {
    CloseClipboard();
    return ;
  }
  memcpy(buf, text2.c_str(), text2.length()*sizeof(wchar_t));
  SetClipboardData(CF_UNICODETEXT, buf);
  CloseClipboard();

  
  ExcelMacro macros;
  macros.BeginMacros();

  macros.Cells(2, 1, dataLst.size() + 1, 1, "Select");
  macros.InsertLine("Selection.NumberFormat = \"@\"");
  macros.Cells(2, 2, dataLst.size() + 1, 2, "Select");
  macros.InsertLine("Selection.NumberFormat = \"@\"");

  macros.InsertLine("Range(\"A2\").Select");
  macros.InsertLine("ActiveSheet.Paste");

  macros.EndMacros();
  macros.RunMacros(theApp.GetModuleDir() + "module.xlt");

  if (OpenClipboard(NULL))
  {
    EmptyClipboard();
    CloseClipboard();
  }
}
//---------------------------------------------------------------------------
std::vector<ReportModule::ReportModuleData> ReportModule::GetData(string_t exitDate)
{
  std::vector<ReportModuleData> dataLst;
  dataLst.reserve(400);

  string_t query = string_t() +
    "SELECT st.edunumdiplom, st.edudatediplom, st.edunumreg, YEAR(st.enterdate) as enterdate, YEAR(st.exitdate) as exitdate, voc.title as grp, st.secondname, st.firstname, st.thirdname, st.bdate, st.sex "
    "FROM students as st, voc "
    "WHERE st.deleted = 0 AND YEAR(st.exitdate) = '" + exitDate + "' AND voc.deleted = 0 AND voc.vkey = 'grp' AND voc.num = st.grpid AND voc.title not like '%отчис%' "
    "ORDER BY voc.title, st.secondname, st.firstname, st.thirdname";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW	row = res.fetch_row())
  {
    ReportModuleData data;
    data.dimplomNum1 = row["edunumdiplom"];
    data.dimplomNum2 = row["edunumdiplom"];
    data.regNum      = row["edunumreg"];
    data.enterYear   = row["enterdate"];
    data.exitYear    = row["exitdate"];
    data.groupName   = row["grp"];
    data.secondName  = row["secondname"];
    data.firstName   = row["firstname"];
    data.thirdName   = row["thirdname"];
    data.birthDate   = row["bdate"];
    data.sex         = row["sex"];

    data.dimplomNum1 = data.dimplomNum1.subString(0, 6);
    if (data.dimplomNum2.indexOf(L" ") != -1)
      data.dimplomNum2 = data.dimplomNum2.subString(data.dimplomNum2.indexOf(L" ") + 1, -1);
    data.dimplomNum1 = "\"" + data.dimplomNum1 + "\"";
    data.dimplomNum2 = "\"" + data.dimplomNum2 + "\"";

    dataLst.push_back(data);
  }
  return dataLst;
}
