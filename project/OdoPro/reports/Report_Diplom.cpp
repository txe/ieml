#include "StdAfx.h"
#include <vector>
#include "Report_Diplom.h"
#include "../SingeltonApp.h"

//-------------------------------------------------------------------------
void ReportDiplom::Run(int grpId, int studentId)
{
  r::PrivateData privData;
  r::GetPrivateData(privData, studentId);
  DirectData dirData;
  GetDirectData(dirData, privData);
  std::vector<Discip> cursDiscip;
  std::vector<Discip> commonDiscip;
  std::vector<Discip> specDiscip;
  GetDiscipInfo(studentId, cursDiscip, commonDiscip, specDiscip);

  WordMacros macros;
  macros.BeginMacros();

  // ���
  macros.Cell(1, 3, 7, "Range.Select");
  macros.SelectionText(privData.secondName);
  macros.Cell(1, 4, 7, "Range.Select");
  macros.SelectionText(privData.firstName);
  macros.Cell(1, 5, 7, "Range.Select");
  macros.SelectionText(privData.thirdName);

  // ���� ��������
  macros.Cell(1, 6, 8, "Range.Select");
  macros.SelectionText(r::to_str_date(privData.bornDate, true));

  // ��������
  macros.Cell(1, 8, 6, "Range.Select");
  macros.SelectionText(privData.prevDoc.toUpperFirst() + " " + privData.prevDocYear + " ���");

  // ������� ����
  macros.Cell(1, 15, 7, "Range.Select");
  macros.SelectionText(r::GetCurrentDate());

  // ����������� (����������� � ��������, ���������, ��������� � ��������)
  macros.Cell(1, 9, 4, "Range.Select");
  macros.SelectionText(dirData.title1);

  // �� �������������, �� �����������
  macros.Cell(1, 11, 7, "Range.Select");
  macros.SelectionText(dirData.title2);

  // ������� ��������
  macros.Cell(1, 15, 9, "Range.Select");
  macros.SelectionText(dirData.title3);

  // �������� ������
  for (int i = 0; i < (int)cursDiscip.size(); ++i)
  {
     macros.CellCell(1, 3, 2, i + 1, 1, "Select");
     macros.SelectionText(toQuate(cursDiscip[i].title));
     macros.CellCell(1, 3, 2, i + 1, 2, "Select");
     macros.SelectionText(cursDiscip[i].ocenka);
  }

  // ������� ���������� �� ������ �������
  int rowCount = 53;  // ����� ����� � ������ �������
  int usedDiscip = 0; // ������� �� ����� ���� �������� ��������� � ������ �������
  for (int i = 0; i < (int)commonDiscip.size() && rowCount > 0; ++i)
  {
    usedDiscip++;
    rowCount -= PrepareDiscipTitle(commonDiscip[i].title, 63);

    macros.CellCell(2, 3, 2, i + 1, 1, "Select");
    macros.SelectionText(toQuate(commonDiscip[i].title));
    macros.CellCell(2, 3, 2, i + 1, 2, "Select");
    macros.SelectionText(commonDiscip[i].period);
    macros.CellCell(2, 3, 2, i + 1, 3, "Select");
    macros.SelectionText(commonDiscip[i].ocenka);
  }
  // ������� ���������� �� ������ �������
  int curRow = 1;
  for (int i = usedDiscip; i < (int)commonDiscip.size(); ++i)
  {
    macros.CellCell(2, 2, 7, curRow, 1, "Select");
    macros.SelectionText(toQuate(commonDiscip[i].title));
    macros.CellCell(2, 2, 7, curRow, 2, "Select");
    macros.SelectionText(commonDiscip[i].period);
    macros.CellCell(2, 2, 7, curRow, 3, "Select");
    macros.SelectionText(commonDiscip[i].ocenka);
    ++curRow;
  }

  macros.EndMacros();
  macros.RunMacros("diplom-2014.dot");
}
//-------------------------------------------------------------------------
void ReportDiplom::GetDirectData(DirectData& dirData, const r::PrivateData& privData)
{
  bool isBachelor = privData.specOrProfilTag != "";
  
  // �����������, ����������� � ��������, ���������, ��������� � ��������
  if (isBachelor && privData.isOtlDiplom)   dirData.title1 = L"��������� � ��������";
  if (isBachelor && !privData.isOtlDiplom)  dirData.title1 = L"���������";
  if (!isBachelor && privData.isOtlDiplom)  dirData.title1 = L"����������� � ��������";
  if (!isBachelor && !privData.isOtlDiplom) dirData.title1 = L"�����������";

  /*
  ������� 1
  ����� (qualific)
  �� ������������� (���� � ������� spec ����������� �����-���� ����) 
  030501.65 (shifrspec) ������������� (spec) 
  ������� 2
  �������� ������������� (qualific)
  �� ����������� ���������� (���� � ������� spec ���� ��� ����, ����1�, ����) 
  030500.62 (shifrspec) ������������� (�� ������� spec � ��� ���� ����1�; �� ������� direct � ��� ����� ���� � ����)
  �� ����������� ��������� (����������) �������, ����� ������ ����� � ������������ �������������/����������� ����������, �� ��������� �������
  */
  if (!isBachelor)
  {
    dirData.title2 = privData.qualific.toLower();
    dirData.title2 += "\n�� �������������\n";
    dirData.title2 += privData.shifrspec + " " + privData.specOrProfil.toUpperFirst();
  }
  else
  {
    string_t spec = privData.specOrProfil;
    if (privData.specOrProfilTag.toLower().trim() != "���1")
      spec = privData.direct;
    dirData.title2 = privData.qualific.toLower();
    dirData.title2 += "\n�� ����������� ����������\n";
    dirData.title2 += privData.shifrspec + " " + spec.toUpperFirst();
  }

  string_t tag = privData.specOrProfilTag.toLower();
  if (tag.empty())                        dirData.title3 = "5 ���";
  else if (tag == "���1" || tag == "���") dirData.title3 = "4 ����";
  else if (tag == "���")                  dirData.title3 = "2 ����";
  else                                    dirData.title3 = "xxxx ���";
}
//-------------------------------------------------------------------------
void ReportDiplom::GetDiscipInfo(int studentId, std::vector<Discip>& cursDiscip, std::vector<Discip>& commonDiscip, std::vector<Discip>& specDiscip)
{
  string_t query = string_t() +
    "SELECT di.fulltitle, di.num_hours, di.idclass, pr.estimation, pr.ball "
    "FROM disciplines as di, progress as pr "
    "WHERE di.deleted=0 and pr.deleted=0 and pr.idstud=" + aux::itow(studentId) + " and pr.iddiscip=di.id "
    "ORDER BY di.scan_number";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW	row = res.fetch_row())
  {
    r::DISCIP_TYPE idclass = (r::DISCIP_TYPE)row["idclass"].toInt();
    string_t       title   = row["fulltitle"];
    string_t       hours   = row["num_hours"];
    string_t       ocenka  = r::toOcenka(row["estimation"].toInt());

    if (idclass == r::DT_CURSE_WORK)
      cursDiscip.push_back(Discip(title, "", ocenka));
    if (idclass == r::DT_COMMON)
      commonDiscip.push_back(Discip(title, hours + " ���.", ocenka));
  }
}
//-------------------------------------------------------------------------
// �������� �� ������ � ������ ������� ����� ������
int ReportDiplom::PrepareDiscipTitle(string_t& title, int symbolMax)
{
  if ((int)title.size() <= symbolMax)
    return 1;
  std::vector<string_t> lines;
  while ((int)title.size() > symbolMax)
  {
    for (int i = symbolMax - 1; i >= 0; --i)
      if (title[i] == L' ' || title[i] == L'(')
      {
        lines.push_back(title.subString(0, i));
        title = title.subString(i, -1);
        break;
      }
  }
  if (!title.empty())
    lines.push_back(title);

  title = lines[0];
  for (size_t i = 1; i < lines.size(); ++i)
    title += L"\n" + lines[i];
  return lines.size();
}