#include "StdAfx.h"
#include <vector>
#include "Report_Diplom2023.h"
#include "../SingeltonApp.h"

//-------------------------------------------------------------------------
void ReportDiplom2023::Run(int grpId, int studentId)
{
  r::PrivateData privData;
  r::GetPrivateData(privData, studentId);
  DirectData dirData;
  GetDirectData(dirData, privData);
  std::vector<Discip> commonDiscip;
  bool useZe = privData.specOrProfilTag.startsWith("���") || privData.specOrProfilTag == "���";    // ��� �� ��� ������� ������ ���������
  bool anotherEnd = privData.specOrProfilTag.empty() || privData.specOrProfilTag == "���1";
  GetDiscipInfo(studentId, commonDiscip, privData.lang, privData.vkrTitle, useZe, anotherEnd);

  WordMacros macros;
  macros.BeginMacros();

  // ���
  macros.ReplaceBoxText("$S$", privData.firstName);
  macros.ReplaceBoxText("$F$", privData.secondName);
  macros.ReplaceBoxText("$T$", privData.thirdName);

  // ���� ��������
  macros.ReplaceBoxText("$BIRTH$", r::to_str_date(privData.bornDate, "����"));

  // ��������
  macros.Replace("$ATT$", privData.prevDoc.toUpperFirst() + " " + privData.prevDocYear + " ���");

  // ����� �����������
  macros.Replace("$REGNUM$", privData.regNum);

  // ���� ������
  macros.Replace("$ISSUEYEAR$", r::to_str_date(privData.dataVidachi, "����"));  

  // ����������� (����������� � ��������, ���������, ��������� � ��������)
  macros.Replace("$SPEC$", dirData.title1);

  // �� �������������, �� �����������
  macros.Replace("$TITLE2$", dirData.title2_2023);
  macros.Replace("$GAK$", dirData.title2_gak);

  // ������� ��������
  macros.Replace("$SPECYEAR$", dirData.title3);

  // �������������� ����
  macros.CellCell(1, 1, 1, 2, 1, "Select");
  macros.SelectionText(dirData.bottomInfo);
  
  struct local
  {
    static void table_text(WordMacros& macros, bool secondTable, int row, int col, string_t text) { 
      if (secondTable)
        macros.Cell(3, row, col, "Select");
      else
        macros.Cell(2, row, col, "Select");
      macros.SelectionText(text); 
    }
  };

  // ������� ���������� �� ������ �������
  int symbolsInRow = 55;
  int table1RowCount = 50;  // ����� �������� ����� � ������ �������
  int table1RowCount2 = 50;
  int usedDiscip = 0;       // ������� �� ����� ���� �������� ��������� � ������ �������
  for (int i = 0; i < (int)commonDiscip.size() && table1RowCount > 0; ++i)
  {
    table1RowCount -= PrepareDiscipTitle(commonDiscip[i].title, symbolsInRow);
    if (table1RowCount < 0) // ���� ������ �� �������� � ����� �������, �� ����� ������ ����������
      break;

    usedDiscip++;
    local::table_text(macros, false, i+2, 1, toQuate(commonDiscip[i].title));
    local::table_text(macros, false, i+2, 2, commonDiscip[i].period);
    local::table_text(macros, false, i+2, 3, commonDiscip[i].ocenka);
  }
  
  // remove unused rows in table 1
  if (usedDiscip > 0)
  {
    for (int i = table1RowCount2; usedDiscip < i; --i)
    {
       macros.TablesRows(2, usedDiscip + 2, "Delete");
    }
  }

  int table2CurRow = 1;
  bool need_second_table = usedDiscip < (int)commonDiscip.size();
  // ������� ���������� �� ������ ������� ���� �� ������� ����� �� ������
  if (need_second_table)
    for (int i = usedDiscip; i < (int)commonDiscip.size(); ++i)
    {
      PrepareDiscipTitle(commonDiscip[i].title, symbolsInRow);
      local::table_text(macros, true, table2CurRow, 1, toQuate(commonDiscip[i].title));
      local::table_text(macros, true, table2CurRow, 2, commonDiscip[i].period);
      local::table_text(macros, true, table2CurRow, 3, commonDiscip[i].ocenka);
      ++table2CurRow;
    }

  macros.EndMacros();
  macros.RunMacros(theApp.GetModuleDir() + "diplom-2023.dot");
}
//-------------------------------------------------------------------------
void ReportDiplom2023::GetDirectData(DirectData& dirData, const r::PrivateData& privData)
{
  // ���� ���� ��� �����������
  bool isBachelor = privData.specOrProfilTag != "" && privData.specOrProfilTag != "����";
  
  // �����������, ����������� � ��������, ���������, ��������� � ��������
  if (privData.specOrProfilTag == "���")                                          dirData.title1 = L"��������";
  else if (privData.specOrProfilTag != "" && privData.specOrProfilTag != "����") dirData.title1 = L"���������";
  else                                                                            dirData.title1 = L"�����������";
  
  if (privData.isOtlDiplom)
    dirData.title1 += L" � ��������";

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
  string_t shifr = privData.shifrspec;
  if (shifr.empty())
    shifr = "xxxx";
  // ������ ����� �����, ���� ��� ������ ���� �����
  int dotPos = shifr.indexOf(L".");
  if (dotPos != -1)
    if (-1 == shifr.subString(dotPos + 1, -1).indexOf(L"."))  
      shifr = shifr.subString(0, dotPos);

  if (!isBachelor)
  {
    dirData.title2 = privData.qualific.toLower();
    dirData.title2 += "\n�� �������������\n";
    dirData.title2 += shifr + " " + privData.specOrProfil.toUpperFirst();

    dirData.title2_2023 = privData.qualific.toLower() + " " + shifr + " " + privData.specOrProfil.toUpperFirst();
  }
  else
  {
    string_t spec = privData.specOrProfil;
    if (privData.specOrProfilTag.toLower().trim() != "���1")
      spec = privData.direct;
    dirData.title2 = privData.qualific.toLower();
    dirData.title2 += "\n�� ����������� ����������\n";
    dirData.title2 += shifr + " " + spec.toUpperFirst();

    dirData.title2_2023 = privData.qualific.toLower() + " " + shifr + " " + spec.toUpperFirst();
  }

  dirData.title2_gak = "(�������� � " + privData.gakNum + " �� " + privData.dataQualific + " �.)";

  string_t tag = privData.specOrProfilTag.toLower();
  if (tag.empty() || tag == "����") dirData.title3 = "5 ���";
  else if (tag.startsWith("���"))   dirData.title3 = "4 ����";
  else if (tag == "���")            dirData.title3 = "2 ����";
  else                              dirData.title3 = "xxxx ���";

  // �������������� ����������
  if (privData.direct.empty() || privData.specOrProfilTag == "����")
    dirData.bottomInfo += L"�������������: " + privData.specializ + L".";
  else if (tag.startsWith("���") || tag == "���")
    dirData.bottomInfo += L"�������������� (�������) ��������������� ���������: " + privData.specOrProfil + L".";
  dirData.bottomInfo += L"\n";
  dirData.bottomInfo += L"����� ��������: �������.";
  //dirData.bottomInfo += L"\n����� ��������������� ��������� � ������ ? ������ ������� � ?.";
}
//-------------------------------------------------------------------------
void ReportDiplom2023::GetDiscipInfo(int studentId, std::vector<Discip>& commonDiscip, string_t lang, string_t vkrTitle, bool useZe, bool anotherEnd)
{
  struct fun
  {
    static string_t ze_hours(bool useZe, string_t val) { return useZe ? (val + "  �.�.") : (val + " ���."); }
    static string_t ze_weeks(bool useZe, string_t val) { return useZe ? (val + "  �.�.") : r::weeks_to_str(val); }
  };

  std::vector<Practice> practice; // ��������
  std::vector<Discip> itog;       // ���. ����������
  std::vector<Discip> cursDiscip; // �������� ������
  Discip              vkrWork("", "", "");  // ���
  int                 practicTime = 0;
  string_t            itogTime = "0";
  int                 zeTimeCounter = 0;

  string_t query = string_t() +
    "SELECT di.fulltitle, di.num_hours, di.idclass, pr.estimation, pr.ball,di.zachet_edinica,pr.numplansemestr "
    "FROM disciplines as di, progress as pr "
    "WHERE di.deleted=0 and pr.deleted=0 and pr.idstud=" + aux::itow(studentId) + " and pr.iddiscip=di.id "
    "ORDER BY di.scan_number";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW	row = res.fetch_row())
  {
    r::DISCIP_TYPE idclass = (r::DISCIP_TYPE)row["idclass"].toInt();
    string_t       title   = row["fulltitle"];
    string_t       ocenka  = r::toOcenka(row["estimation"].toInt());
    int            numPlan = row["numplansemestr"].toInt();
    string_t       times   = useZe ? row["zachet_edinica"] : row["num_hours"];

    if (idclass == r::DT_CURSE_WORK || idclass == r::DT_CURSE_PRACTICE)
      cursDiscip.push_back(Discip(title.double_quote() + " (�������� ������)", "", ocenka));
    if (idclass == r::DT_COMMON)
    {
      if (title.toUpper().trim() == string_t(L"����������� ����"))
        title += " (" + lang + ")";
      if (AddDiscip(commonDiscip, Discip(title, fun::ze_hours(useZe, times), ocenka, numPlan)))
        zeTimeCounter += times.toInt();
    }
    if (idclass == r::DT_PRACTICE)
    {
      Practice* find = NULL;
      for (size_t i = 0; i < practice.size() && !find; ++i)
        if (practice[i].title == title)
          find = &practice[i];
      if (find)
      {
        find->ocenka += (5 - row["estimation"].toInt()); // �������� ������, ��� �� ���������� �������� ���������
        find->practice_amount += 1;
      }
      else
      {
        zeTimeCounter += times.toInt();
        practicTime += times.toInt();
        practice.push_back(Practice(title, fun::ze_weeks(useZe, times), 5 - row["estimation"].toInt()));
      }
    }
    if (idclass == r::DT_ITOG_ATESTACIA)
    {
      itog.push_back(Discip(title, "�", ocenka));
    }
    if (idclass == r::DT_KVALIFIC_WORK)
    {
      zeTimeCounter += times.toInt();
      itogTime = times;
      vkrWork = Discip(title.toLower() + " �" + vkrTitle + "�", "�", ocenka);
    }
  }

  // ��������
  commonDiscip.push_back(Discip("��������", fun::ze_weeks(useZe, toStr(practicTime)), "x"));
  commonDiscip.push_back(Discip("� ��� �����:", "", ""));
  for (size_t i = 0; i < practice.size(); ++i)
  {
    int ocenka = (int)(double(practice[i].ocenka) / double(practice[i].practice_amount) + 0.5);
    ocenka = 5 - ocenka; // ������ ���� �� ������ ��� ����������� ���������� � ������ �������
    commonDiscip.push_back(Discip(practice[i].title, practice[i].period, r::toOcenka(ocenka)));
  }

  // ���. ����������
  commonDiscip.push_back(Discip("��������������� �������� ����������", fun::ze_weeks(useZe, itogTime), "x"));
  commonDiscip.push_back(Discip("� ��� �����:", "", ""));
  for (size_t i = 0; i < itog.size(); ++i)
    commonDiscip.push_back(itog[i]);
  if (!vkrWork.title.empty())
    commonDiscip.push_back(vkrWork);

  if (useZe)
  {
    string_t ze = fun::ze_weeks(true, toStr(zeTimeCounter));
    commonDiscip.push_back(Discip("����� ��������������� ���������", ze, "x"));
    commonDiscip.push_back(Discip("� ��� ����� ����� ������ ����������� �� �������������� � ��������������:", "800 ���.", "x"));
  }
  else if (anotherEnd)
  {
    commonDiscip.push_back(Discip("���� �������� ��������������� ���������", "260 ������", "x"));
    commonDiscip.push_back(Discip("� ��� ����� ���������� �����:", "800 ���.", "x"));
  }
  else
  {
    commonDiscip.push_back(Discip("����� ������������ ��������������� ���������", "260 ������", "x"));
    commonDiscip.push_back(Discip("� ��� ����� ����� ������ ����������� �� �������������� � ��������������:", "800 ���.", "x"));
  }

  // �������� ������
  for (size_t i = 0; i < cursDiscip.size(); ++i)
    commonDiscip.push_back(cursDiscip[i]);
}
//-------------------------------------------------------------------------
// �������� �� ������ � ������ ������� ����� ������
int ReportDiplom2023::PrepareDiscipTitle(string_t& title, int symbolMax)
{
  if ((int)title.size() <= symbolMax)
    return 1;

  std::vector<string_t> lines;
  while ((int)title.size() > symbolMax)
  {
    for (int i = symbolMax - 1; i >= 0; --i)
      if (title[i] == L' ' || title[i] == L'(')
      {
        if (title[i] == L' ') { ++i; }
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
  
  /*
   if ((int)title2.size() <= symbolMax)
    return 1;

   string_t buf = title2;
  std::vector<string_t> lines;
  while ((int)buf.size() > symbolMax)
  {
    for (int i = symbolMax - 1; i >= 0; --i)
      if (buf[i] == L' ' || buf[i] == L'(')
      {
        lines.push_back(buf.subString(0, i));
        buf = buf.subString(i, -1);
        break;
      }
  }
  if (!buf.empty())
    lines.push_back(buf);

  buf = lines[0];
  for (size_t i = 1; i < lines.size(); ++i)
    buf += L"\n" + lines[i];
  return lines.size();
  */
}
//-------------------------------------------------------------------------
bool ReportDiplom2023::AddDiscip(std::vector<Discip>& disList, const Discip& discip)
{
  // �������� ��� ����� ��� ������� ������ ������
  for (size_t i = 0; i < disList.size(); ++i)
    if (disList[i].title == discip.title)
    {
      if (discip.numPlan >= disList[i].numPlan)
        disList[i] = discip;
      return false;
    }
  disList.push_back(discip);
  return true;
}
