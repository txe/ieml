#include "StdAfx.h"
#include "Report_Pril4.h"
#include "../SingeltonApp.h"
#include "../json-aux-ext.h"

//---------------------------------------------------------------------------
void ReportPril4::Run(int grpId, int studentId)
{
  string_t title = GetTitle(studentId);

  WordMacros macros;
  macros.BeginMacros();

  macros.InsertLine("ActiveDocument.PageSetup.TopMargin=70");
  macros.InsertLine("ActiveDocument.PageSetup.BottomMargin=55");
  macros.InsertLine("ActiveDocument.PageSetup.LeftMargin=55");
  macros.InsertLine("ActiveDocument.PageSetup.RightMargin=55");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");

  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.SelectionFont("Bold=false");
  macros.SelectionFont("Size=10");
  macros.SelectionText("Приложение № ___ к акту № ___ от ________ 2017 г.\n");
  macros.SelectionFont("Bold=true"); 
  macros.SelectionText(title);
  macros.SelectionFont("Bold=false");

  std::vector<Data> lst = GetData(grpId);
  macros.TablesAdd(lst.size()+1, 6);
  macros.TablesColumns(1, 1, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.TablesColumns(1, 2, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.TablesColumns(1, 3, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.TablesColumns(1, 4, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.TablesColumns(1, 5, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.TablesColumns(1, 6, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 1, 6, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");

  macros.InsertLine("ActiveDocument.Tables.Item(1).Rows.Item(1).Range.Font.Bold = true");

  macros.TablesColumns(1, 1, "PreferredWidth = CentimetersToPoints(0.5)");
  macros.TablesColumns(1, 2, "PreferredWidth = CentimetersToPoints(10)");
  macros.TablesColumns(1, 3, "PreferredWidth = CentimetersToPoints(3)");
  macros.TablesColumns(1, 4, "PreferredWidth = CentimetersToPoints(3)");
  macros.TablesColumns(1, 5, "PreferredWidth = CentimetersToPoints(3)");
  macros.TablesColumns(1, 6, "PreferredWidth = CentimetersToPoints(10)");

  macros.Tables(1, "Range.Font.Size = 10");

  macros.Cell(1, 1, 1, "Range.Text=" + toWrap("№ п/п"));
  macros.Cell(1, 1, 2, "Range.Text=" + toWrap("Ф.И.О."));
  macros.Cell(1, 1, 3, "Range.Text=" + toWrap("Дата рождения"));
  macros.Cell(1, 1, 4, "Range.Text=" + toWrap("№ приказа о зачислении"));
  macros.Cell(1, 1, 5, "Range.Text=" + toWrap("Дата приказа о зачислении"));
  macros.Cell(1, 1, 6, "Range.Text=" + toWrap("Документ о предыдущем образовании"));
  for (size_t k = 1; k < 7; ++k)
    macros.Cell(1, 1, k, "VerticalAlignment=wdCellAlignVerticalCenter");

  for (size_t i = 0; i < lst.size(); ++i)
  {
    Data& data = lst[i];
    macros.Cell(1, 2+i, 1, "Range.Text=" + toWrap(string_t(aux::itow(i+1))));
    macros.Cell(1, 2+i, 2, "Range.Text=" + toWrap(data.fio));
    macros.Cell(1, 2+i, 3, "Range.Text=" + toWrap(data.birth_date));
    macros.Cell(1, 2+i, 4, "Range.Text=" + toWrap(data.order));
    macros.Cell(1, 2+i, 5, "Range.Text=" + toWrap(data.order_date));
    macros.Cell(1, 2+i, 6, "Range.Text=" + toWrap(data.prev_doc));
    for (size_t k = 1; k < 7; ++k)
      macros.Cell(1, 2+i, k, "VerticalAlignment=wdCellAlignVerticalCenter");
  }

  macros.EndMacros();
  macros.RunMacros("");
 
}
//---------------------------------------------------------------------------
string_t ReportPril4::GetTitle(int studentId)
{
  r::PrivateData privData;
  GetPrivateData(privData, studentId);
  string_t title = "Направление подготовки " + privData.shifrspec + " " + privData.direct;
  title += " профиль \"\"" + privData.specOrProfil.toUpperFirst() + "\"\"\n";
  title += theApp.GetTitleForKeyFromVoc(vok_key::VK_EDUFORM, privData.eduformid.toInt(), true);
  title += "\n";
  title += "____ курс   ____ семестр   гр. " + privData.grpName;
  return title;
}
//---------------------------------------------------------------------------
std::vector<ReportPril4::Data> ReportPril4::GetData(int grpId)
{
  std::vector<Data> lst;
  string_t query = string_t() +
    " SELECT CONCAT(s.secondname, ' ', s.firstname, ' ', thirdname) as fio, s.bdate, s.enternum, s.enterdate, vEdudoc.title as edudoc, s.edudocinfo "
    " FROM students as s, voc as vEdudoc "
    " WHERE s.deleted = 0 and s.grpid = " + aux::itoa(grpId) +
    " and vEdudoc.deleted = 0 and vEdudoc.vkey='edudoc' and vEdudoc.num = s.edudocid"
    " ORDER BY fio";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW row = res.fetch_row())
  {
    Data data;
    data.fio = row["fio"];
    data.birth_date = r::to_digital_date(row["bdate"]);
    data.order = row["enternum"];
    data.order_date = r::to_digital_date(row["enterdate"]);
    data.prev_doc = row["edudoc"] + " № " + row["edudocinfo"];
    if (data.prev_doc.indexOf(L"аттест") != -1) data.prev_doc += ", приложение к аттестату";
    if (data.prev_doc.indexOf(L"диплом") != -1) data.prev_doc += ", приложение к диплому";

    lst.push_back(data);
  }
  return lst;
}

