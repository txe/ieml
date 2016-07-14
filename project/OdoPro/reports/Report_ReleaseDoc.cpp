#include "StdAfx.h"
#include <vector>
#include "Report_ReleaseDoc.h"
#include "../SingeltonApp.h"
#include <sys/timeb.h>
#include <sstream>
#include <iomanip>
#include "../SelectGroups.h"
#include "..\json-aux-ext.h"


//-------------------------------------------------------------------------
void ReportReleaseDoc::Run(int grpId, int studentId)
{
  CSelectGroupsDlg dlg(theApp.GetMainWnd());
  dlg.DoModal();
  if (dlg.groups.empty())
    return;

  std::map<string_t, ReportReleaseDoc::profil> data;
  GetData(dlg.groups, data);
 
  WordMacros macros;
  macros.BeginMacros();

  macros.InsertLine("ActiveDocument.PageSetup.TopMargin=70");
  macros.InsertLine("ActiveDocument.PageSetup.BottomMargin=55");
  macros.InsertLine("ActiveDocument.PageSetup.LeftMargin=55");
  macros.InsertLine("ActiveDocument.PageSetup.RightMargin=55");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");

  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.SelectionFont("Bold=false");
  macros.SelectionFont("Size=12");
  macros.SelectionText("ДОКЛАДНАЯ\n");

  int table_count = 0;
  for (std::map<string_t, ReportReleaseDoc::profil>::iterator it = data.begin(); it != data.end(); ++it)
  {
    profil& p = it->second;
    macros.SelectionText("\n");
    macros.SelectionFont("Bold=true");
    macros.SelectionFont("Size=12");
    macros.SelectionText(p.part1);
    macros.SelectionFont("Bold=false");
    macros.SelectionText(" с профилем ");
    macros.SelectionFont("Bold=true");
    macros.SelectionText(p.part2);
    macros.SelectionFont("Bold=false");

    std::sort(p.good_student.begin(), p.good_student.end());
    std::sort(p.just_student.begin(), p.just_student.end());
    if (p.good_student.size())
    {
      macros.SelectionText("\nвыдать ");
      macros.SelectionFont("Bold=true");
      macros.SelectionText("диплом");
      macros.SelectionFont("Bold=false");
      macros.SelectionText(" с отличием\n");
      AddTable(macros, p.good_student, table_count);
    }
    if (p.just_student.size())
    {
      macros.SelectionText("\nвыдать диплом\n\n");
      AddTable(macros, p.just_student, table_count);
    }
  }

  macros.EndMacros();
  macros.RunMacros("");
}
//-------------------------------------------------------------------------
void ReportReleaseDoc::AddTable(WordMacros& macros, std::vector<ReportReleaseDoc::student>& lst, int& table_count)
{
  ++table_count;
  macros.TablesAdd(lst.size(), 4);
  macros.TablesColumns(table_count, 1, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphRight");
  macros.TablesColumns(table_count, 2, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.TablesColumns(table_count, 3, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.TablesColumns(table_count, 4, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");

  macros.TablesColumns(table_count, 1, "PreferredWidth = CentimetersToPoints(1)");
  macros.TablesColumns(table_count, 2, "PreferredWidth = CentimetersToPoints(6)");
  macros.TablesColumns(table_count, 3, "PreferredWidth = CentimetersToPoints(5)");
  macros.TablesColumns(table_count, 4, "PreferredWidth = CentimetersToPoints(6)");

  macros.Tables(table_count, "Range.Font.Size = 10");

  for (size_t i = 0; i < lst.size(); ++i)
  {
    string_t num = aux::itoa((int)i+1);
    macros.Cell(table_count, i+1, 1, "Range.Text=" + toWrap(num + "."));
    macros.Cell(table_count, i+1, 2, "Range.Text=" + toWrap(lst[i].name));
    macros.Cell(table_count, i+1, 3, "Range.Text=" + toWrap(lst[i].dogovor));
    macros.Cell(table_count, i+1, 4, "Range.Text=" + toWrap(lst[i].protocol));
    macros.Cell(table_count, i+1, 3, "Range.Font.Size = 8");
    macros.Cell(table_count, i+1, 4, "Range.Font.Size = 8");
  }

  macros.InsertLine("Selection.EndKey Unit:=wdStory");
}

//-------------------------------------------------------------------------
void ReportReleaseDoc::GetData(const std::vector<string_t>& groups, std::map<string_t, ReportReleaseDoc::profil>& data)
{
  std::map<string_t, string_t> t_direct = GetVocTable("direct");
  std::map<string_t, string_t> t_dogyear = GetVocTable("dogyear");
  std::map<string_t, string_t> t_dogshift = GetVocTable("dogshifr");
  std::map<string_t, string_t> t_dogfast = GetVocTable("dogfast");
  std::map<string_t, string_t> t_shifrspec = GetVocTable("shifrspec");
  std::map<string_t, string_t> t_spec = GetVocTable("spec");

  string_t where_groups;
  for (size_t i = 0; i < groups.size(); ++i)
    if (i == 0)
      where_groups = "s.grpid=" + groups[i];
    else
      where_groups += " or s.grpid=" + groups[i];
  
  string_t query;
  query += "SELECT s.secondname,s.firstname,s.thirdname,s.grpid," \
    " s.specid, s.directid, s.edudiplomotl, s.edunumprotgak, s.edudatequalif, "
    " s.dogshifrid, s.dogyearid, s.dogfastid, s.dognum " \
    " FROM students as s " \
    " where s.deleted=0 and (" + where_groups + ") ";

  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW	row = res.fetch_row())
  {
    string_t secondname = row["secondname"];
    string_t firstname  = row["firstname"];
    string_t thirdname  = row["thirdname"];
    bool     otl        = row["edudiplomotl"] == "1";
    string_t direct     = GetVocValue(t_direct, row["directid"]);
    string_t specOrProfil = GetVocValue(t_spec, row["specid"]);

    string_t shifrspec;
    if (direct.empty())
      shifrspec = GetVocValue(t_shifrspec, row["specid"]);
    else
      shifrspec = GetVocValue(t_shifrspec, row["directid"]);
    // уберем часть шифра, если там только одна точка
    int dotPos = shifrspec.indexOf(L".");
    if (dotPos != -1)
      if (-1 == shifrspec.subString(dotPos + 1, -1).indexOf(L"."))  
        shifrspec = shifrspec.subString(0, dotPos);

    string_t dognum = GetVocValue(t_dogshift, row["dogshifrid"]) + "-" + GetVocValue(t_dogyear, row["dogyearid"]) 
      + GetVocValue(t_dogfast, row["dogfastid"]) + "-" + row["dognum"];
    dognum = "(договор № " + dognum + ")";

    string_t gak = "(протокол ГЭК №" + row["edunumprotgak"] + " от " + r::to_digital_date(row["edudatequalif"]) + " г.)";
    
    student st;
    st.name = secondname + " " + firstname + " " + thirdname;
    st.dogovor = dognum;
    st.protocol = gak;

    string_t key = shifrspec + direct + specOrProfil;
    profil& p = data[key];
    if (p.part1.empty() && p.part2.empty())
    {
      p.part1 = shifrspec + " " + direct;
      p.part2 = specOrProfil;
    }
    if (otl)
      p.good_student.push_back(st);
    else
      p.just_student.push_back(st);
  }
}
//-------------------------------------------------------------------------
std::map<string_t, string_t> ReportReleaseDoc::GetVocTable(string_t name)
{
  string_t query;
  query += "SELECT v.num, v.title FROM voc as v WHERE v.deleted = 0 and v.vkey = '" + name +"'";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

  std::map<string_t, string_t> data;
  while (mybase::MYFASTROW	row = res.fetch_row())
    data[row["num"]] = row["title"];

  return data;
}
//-------------------------------------------------------------------------
string_t ReportReleaseDoc::GetVocValue(const std::map<string_t, string_t>& table, string_t key)
{
  std::map<string_t, string_t>::const_iterator it = table.find(key);
  if (it == table.end())
    return "";
  return it->second;
}
