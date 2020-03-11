#include "StdAfx.h"
#include <vector>
#include "Report_Module.h"
#include "../SingeltonApp.h"
#include <sys/timeb.h>
#include <sstream>
#include <iomanip>
#include "../SelectYear.h"

//-------------------------------------------------------------------------
void ReportModule::Run(int grpId, int studentId)
{
  CSelectYearDlg dlg(theApp.GetMainWnd());
  dlg.DoModal();
  if (dlg._year.empty())
    return;

  std::vector<ReportModuleData> dataLst = GetData(dlg._year);
  string_t text;
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
  macros.Cells(2, 4, dataLst.size() + 1, 4, "Select");
  macros.InsertLine("Selection.NumberFormat = \"@\"");
  macros.Cells(2, 5, dataLst.size() + 1, 5, "Select");
  macros.InsertLine("Selection.NumberFormat = \"@\"");
  macros.Border("A1:N" + toStr(dataLst.size() + 1));

  macros.InsertLine("Range(\"A2\").Select");
  macros.InsertLine("ActiveSheet.Paste");
  macros.InsertLine("Range(\"A2\").Select");

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
  voc_data vocShifr = GetVocData("shifrspec");
  voc_data vocDirect = GetVocData("direct");
  voc_data vocSpecOrProfile = GetVocData("spec");


  std::vector<ReportModuleData> dataLst;
  dataLst.reserve(400);

  string_t query = string_t() +
    "SELECT st.edunumdiplom, st.edudatediplom, st.edunumreg, YEAR(st.enterdate) as enterdate, YEAR(st.exitdate) as exitdate, voc.title as grp, st.secondname, st.firstname, st.thirdname, st.bdate, st.sex, st.directid, st.specid "
    "FROM students as st, voc "
    "WHERE st.deleted = 0 AND YEAR(st.exitdate) = '" + exitDate + "' AND voc.deleted = 0 AND voc.vkey = 'grp' AND voc.num = st.grpid AND voc.title not like '%отчис%' "
    "ORDER BY voc.title, st.secondname, st.firstname, st.thirdname";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW	row = res.fetch_row())
  {
    ReportModuleData data;    
    data.regNum      = row["edunumreg"];
    data.dimplomDate = row["edudatediplom"];
    data.enterYear   = row["enterdate"];
    data.exitYear    = row["exitdate"];
    data.groupName   = row["grp"];
    data.secondName  = row["secondname"];
    data.firstName   = row["firstname"];
    data.thirdName   = row["thirdname"];
    data.birthDate   = row["bdate"];
    data.sex         = row["sex"];

    string_t diplomNum = row["edunumdiplom"].trim();
    data.dimplomNum1 = diplomNum.subString(0, 6);
    if (diplomNum.indexOf(L" ") != -1)
      data.dimplomNum2 = diplomNum.subString(diplomNum.indexOf(L" ") + 1, -1);

    // более сложные вычисления
    string_t direct = vocDirect.find(row["directid"]).title;
    if (direct.empty())
      data.shifrSpec = vocShifr.find(row["specid"]).title;
    else
      data.shifrSpec = vocShifr.find(row["directid"]).title;
  
    string_t specOrProfileTag = vocSpecOrProfile.find(row["specid"]).tag.toLower();
    if (specOrProfileTag.startsWith("бак") || specOrProfileTag == "маг")
      data.specOrDir = direct;
    else
      data.specOrDir = vocSpecOrProfile.find(row["specid"]).title;

    dataLst.push_back(data);
  }
  return dataLst;
}
//-------------------------------------------------------------------------
ReportModule::voc_data ReportModule::GetVocData(string_t vkey)
{
  string_t query = string_t() +
    "SELECT num, title, tag FROM voc "
    "WHERE deleted = 0 AND vkey = '" + vkey + "'";
 
  voc_data data;
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW	row = res.fetch_row())
    data.add(row["num"], row["title"], row["tag"]);

  return data;
}
