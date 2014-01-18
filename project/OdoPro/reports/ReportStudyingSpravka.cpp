#include "StdAfx.h"
#include <vector>
#include "ReportStudyingSpravka.h"
#include "../SingeltonApp.h"

//-------------------------------------------------------------------------
inline string_t date_to_str(string_t str, bool isYear = false)
{
  static string_t mounthNames[12]={ "������", "�������", "�����", "������", "���", "����", "����", "�������", "��������", "�������", "������", "�������"};
  string_t res = "";
  try
  {
    // ����� 
    res += str.subString(8,2);
    if (res.size() == 1) 
      res = "0" + res;
    res += " ";
    // ����� 
    int mNum = (str.subString(5, 2).toInt()-1) % 12;
    if (mNum < 0)
      return "<���������� ����!>";
    res += mounthNames[mNum] + " ";
    // ���
    res += str.subString(0,4);
    if (isYear) 
      res += " ����";
  }
  catch(...)
  {
    return "<���������� ����!>";
  }
  return res;
}
//-------------------------------------------------------------------------
inline string_t date_to_str2(string_t str)
{
  string_t res = "";
  try
  {
    // ����� 
    res += str.subString(8,2);
    if (res.size() == 1) 
      res = "0" + res;
    res += ".";
    // �����
    string_t m = str.subString(5, 2);
    if (m.size() == 1)
      m = m + res;
    res += m + ".";
    // ���
    res += str.subString(0,4);
  }
  catch(...)
  {
    return "<���������� ����!>";
  }
  return res;
}

string_t GetYear(string_t str)
{
  return str.subString(0,4);
}
string_t GetMonth(string_t str)
{
  return str.subString(5,2);
}
string_t GetDay(string_t str)
{
  return str.subString(8,2);
}
string_t toOcenka(int num)
{
  static string_t ocenkaList[]={"�������","������","�����������������","�������������������","�������","���������"};
  return ocenkaList[num];
}
//-------------------------------------------------------------------------
void ReportStudyingSpravka::Run(int grpId, int studentId)
{
  PrivateData privData;
  GetPrivateData(privData, studentId);
  StudyData studyData;
  GetStudyData(studyData, studentId, privData.isMale, privData.vkrTitle);
  std::vector<DiscipData> discData;
  GetDiscipData(discData, studentId);

  bool renameUniver = privData.inYear.toInt() < 2011 || (privData.inYear.toInt() == 2011 && (privData.inMonth.toInt() < 7 || (privData.inMonth.toInt() == 7 && privData.inDay.toInt() < 8)));
  bool stillStudying = privData.exitNum.empty();

  // ��������
  string_t inInfo = " ���� � ����������� ��������������� ��������� ��������������� ���������� ������� ����������������� ����������� �������������� ��������������� ������������-������������ ����������� (������� �����)";
  if (renameUniver)
    inInfo = " ���� � ��������������� ��������������� ���������� ������� ����������������� ����������� �������������� ��������������� ������������-������������ ����������� (������� �����)";
  if (privData.isMale)
    inInfo = "�������� � " + privData.inYear + inInfo;
  else
    inInfo = "��������� � " + privData.inYear + inInfo;

  // ����������
  string_t outInfo = " ���� � ����������� ��������������� ��������� ��������������� ���������� ������� ����������������� ����������� �������������� ��������������� ������������-������������ ����������� (������� �����)";
  if (privData.isMale)
    outInfo = "�������� �������� � " + privData.outYear + outInfo;
  else
    outInfo = "��������� �������� � " + privData.outYear + outInfo;

  //  ��� ���������� ��������
  if (stillStudying)
    if (privData.isMale)
      outInfo = "�������� �������� �\n���������� ��������";
    else
      outInfo = "��������� �������� �\n���������� ��������";

  // ��������������� ��������
  string_t bottomInfo;
  if (renameUniver)
    bottomInfo = "��� ������������ � 2011 ����;\n������ ������ ���������� ����: ��������������� ��������������� ���������� ������� ����������������� ����������� �������������� ��������������� ������������-������������ �����������";
  if (stillStudying)
    bottomInfo.addAsParagraph("������� ������ �� ����������");
  else
    bottomInfo.addAsParagraph("������ �� ���������� �� " + privData.exitDate + " �. � " + privData.exitNum);


  WordMacros macros;
  macros.BeginMacros();

  // ������ �������

  // ���
  macros.Cell(1, 1, 2, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 1, 2, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.SelectionText(privData.secondName + "\n" + privData.firstName + "\n" + privData.thirdName);

  // ������� ����
  macros.Cell(1, 4, 2, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.Cell(1, 4, 2, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.Cell(1, 4, 2, "Range.Text=" + toWrap(CurrentDate()));

  macros.Cell(1, 2, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 2, 1, "VerticalAlignment=wdCellAlignVerticalTop");

  // ���� ��������
  macros.SelectionText("���� �������� " + privData.bornDate);
  macros.SelectionTypeParagraph(2);
  macros.SelectionText("�������� �� ������ �����������, �� ��������� �������� ��������� �� ��������:\n");
  macros.SelectionText(privData.prevDoc + " " + privData.prevDocYear + " ���");
  macros.SelectionTypeParagraph(2);

  // ���������
  macros.SelectionText(inInfo);
  macros.SelectionTypeParagraph(2);

  // �����������
  macros.SelectionText(outInfo);
  macros.SelectionTypeParagraph(2);

  macros.SelectionText("����������� ���� �������� �� ����� ����� 4 ����");
  macros.SelectionTypeParagraph(2);

  // ����������� ����������/�������������:
  macros.SelectionUnderlineText("����������� ����������", privData.stroka1 == PrivateData::S1_DIRECT);
  macros.SelectionText("/");
  macros.SelectionUnderlineText("�������������", privData.stroka1 == PrivateData::S1_SPEC);
  macros.SelectionText(":");
  macros.SelectionTypeParagraph();

  // �������� ���������� ������
  macros.SelectionText(privData.stroka1Value);
  macros.SelectionTypeParagraph(2);

  // �������������/�������/���������� �������������� (���������):
  macros.SelectionUnderlineText("�������������", privData.stroka2 == PrivateData::S2_SPECIAL);
  macros.SelectionText("/");
  macros.SelectionUnderlineText("�������", privData.stroka2 == PrivateData::S2_PROFIL);
  macros.SelectionText("/");
  macros.SelectionUnderlineText("���������� �������������� (���������)", privData.stroka2 == PrivateData::S2_MAGISTR);
  macros.SelectionText(":");
  macros.SelectionTypeParagraph();

  // �������� ���������� ������
  macros.SelectionText(privData.stroka2Value);
  macros.SelectionTypeParagraph(2);

  // ��������
  macros.SelectionText("�������� ������ (�������):\n");
  macros.SelectionFont("Size=9");
  macros.SelectionText(studyData.kur);
  macros.SelectionFont("Size=11");
  macros.SelectionTypeParagraph(2);

  // ��������
  macros.SelectionText("��������:\n");
  macros.SelectionFont("Size=9");
  macros.SelectionText(studyData.practic);
  macros.SelectionFont("Size=11");
  macros.SelectionTypeParagraph(2);

  // ������� ������
  macros.SelectionText("������-����������������� ������:\n");
  macros.SelectionFont("Size=9");
  macros.SelectionText(studyData.sci);
  macros.SelectionFont("Size=11");
  macros.SelectionTypeParagraph(2);

  // ����
  macros.SelectionText("��������������� �������� ����������:\n");
  macros.SelectionFont("Size=9");
  macros.SelectionText(studyData.gos);
  macros.SelectionFont("Size=11");

  // ������ �������

  // �������� ������� ���������
  macros.InsertLine("ActiveDocument.Tables.Item(4).Rows.Item(3).Range.Select");
  macros.InsertLine("Selection.InsertRowsBelow " + toStr(discData.size()-2));
  for (size_t i = 0; i < discData.size(); ++i)
  {
    string_t title = discData[i].title;
    if (title.toUpper().trim() == string_t(L"����������� ����"))
      title += " (" + privData.lang + ")";
    macros.Cell(4, i + 2, 1, "Range.Text=" + toWrap(title));
    macros.Cell(4, i + 2, 2, "Range.Text=" + toWrap(discData[i].zachet_edinica));
    macros.Cell(4, i + 2, 3, "Range.Text=" + toWrap(discData[i].hours));
    macros.Cell(4, i + 2, 4, "Range.Text=" + toWrap(discData[i].ocenka));
  }

  macros.Cell(5, 1, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(5, 1, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.SelectionFont("Size=9");
  macros.SelectionText(bottomInfo);
  macros.SelectionFont("Size=11");

  macros.EndMacros();
  macros.RunMacros("spravka.dot");
}
//-------------------------------------------------------------------------
void ReportStudyingSpravka::GetPrivateData(PrivateData& data, int studentId)
{
  data.firstName = data.secondName = data.thirdName = data.bornDate = data.vkrTitle = data.prevDoc = data.prevDocYear = data.inYear = "???";
  data.inMonth = data.inMonth = "0";
  data.outYear = data.outMonth = data.outDay = data.exitDate = data.exitNum = "???";
  data.specOrProfil = data.direct = data.specializ = data.qualific = data.lang = "???";
  data.diplomNum = "xxx 000000";
  data.regNum = data.dataVidachi = data.dataQualific = "00.00.0000";

  string_t specOrProfilTag;
  bool isMag = false;
  string_t  query = string_t() +
    "SELECT s.secondname,s.firstname,s.thirdname,s.bdate,s.vkr_title," \
    "s.edudocid,s.eduenddate,s.specid,s.enterdate,s.exitdate, s.exitnum,s.sex,v.title as lang,s.edunumdiplom," \
    "s.edunumreg,s.edudatediplom,s.edudatequalif,s.directid " \
    " FROM students as s, "\
    " voc as v where s.deleted=0 and v.deleted=0 and s.id=" + aux::itow(studentId) + 
    " and s.languageid=v.num and v.vkey=\"language\"";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  if (mybase::MYFASTROW	row = res.fetch_row())
  {
    int i1 = row["edudocid"].toInt();
    int i2 = row["edudocid"].toInt();
    int i3 = row["edudocid"].toInt();

    data.secondName = row["secondname"];
    data.firstName  = row["firstname"];
    data.thirdName  = row["thirdname"];
    data.bornDate   = date_to_str(row["bdate"],true);
    data.vkrTitle = row["vkr_title"];
    data.prevDoc       = theApp.GetTitleForKeyFromVoc(VK_EDUDOC, row["edudocid"].toInt(), true);
    data.prevDocYear   = GetYear(row ["eduenddate"]);
    data.specOrProfil  = theApp.GetTitleForKeyFromVoc(VK_SPECS, row["specid"].toInt(), true, &specOrProfilTag);
    data.specializ  = theApp.GetTitleForKeyFromVoc(VK_SPEZIALIZ, row["specid"].toInt(), true);
    data.qualific   = theApp.GetTitleForKeyFromVoc(VK_QUALIFIC, row["specid"].toInt(), true);
    data.direct     = theApp.GetTitleForKeyFromVoc(VK_DIRECT, row["directid"].toInt(), true);
    data.inYear     = GetYear(row["enterdate"]);
    data.inMonth    = GetMonth(row["enterdate"]);
    data.inDay      = GetDay(row["enterdate"]);
    data.outYear    = GetYear(row["exitdate"]);
    data.outMonth   = GetMonth(row["exitdate"]);
    data.outDay     = GetDay(row["exitdate"]);
    data.exitDate   = date_to_str2(row["exitdate"]);
    data.exitNum    = row["exitnum"];
    data.isMale     = row["sex"] != string_t("�") && row["sex"] != string_t("�");
    data.lang       = row["lang"];

    data.diplomNum = row["edunumdiplom"];
    data.regNum    = row["edunumreg"];
    data.dataVidachi = date_to_str(row["edudatediplom"]);
    data.dataQualific = date_to_str(row["edudatequalif"]);

    isMag = specOrProfilTag.toUpper().trim() == "���";
  }

  // ����������� ����������/�������������:
  // ���� ��������� ����������� - ������������ ����������� ����� �������������
  data.stroka1 = data.direct.empty() ? PrivateData::S1_SPEC : PrivateData::S1_DIRECT;
  if (data.stroka1 == PrivateData::S1_DIRECT)
    data.stroka1Value = data.direct;
  else
    data.stroka1Value = data.specOrProfil;

  // �������������/�������/���������� �������������� (���������):
  // ���� ���� ����������� �� ��� �������, ����� �������������, � ���� ��� (���) �� ���������� ��������������
  if (data.stroka1 == PrivateData::S1_DIRECT) data.stroka2 = PrivateData::S2_PROFIL;
  else if (isMag)                             data.stroka2 = PrivateData::S2_MAGISTR;
  else                                        data.stroka2 = PrivateData::S2_SPECIAL;

  if (data.stroka2 == PrivateData::S2_SPECIAL)
    data.stroka2Value = data.specializ;
  else
    data.stroka2Value = data.specOrProfil;
}
//-------------------------------------------------------------------------
void ReportStudyingSpravka::GetStudyData(StudyData& data, int studentId, bool isMale, string_t vkr_title)
{
  data.kur     = "";
  data.practic = "";
  data.gos     = "";
  data.practic = "";
  string_t vkrGos;

  struct local
  {
    static string_t hours_to_str(string_t hours)
    {
      int num = hours.toInt();
      if (num >= 5 && num <= 20) hours += " �����"; // ���������� �� ������
      else
      {
        num %= 10;
        if (num == 1) hours += " ���";
        else if (num >= 2 && num <= 4) hours += " ����";
        else hours += " �����";
      }
      return hours;
    }
  };

  string_t query = string_t() +
    "SELECT di.fulltitle, di.num_hours, di.idclass, pr.estimation, pr.ball "
    "FROM disciplines as di, progress as pr "
    "WHERE di.deleted=0 and pr.deleted=0 and pr.idstud=" + aux::itow(studentId) + " and pr.iddiscip=di.id "
    "ORDER BY di.scan_number";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW	row = res.fetch_row())
  {
    int      idclass = row["idclass"].toInt();
    string_t title   = row["fulltitle"];
    string_t hours   = row["num_hours"];
    string_t ocenka  = toOcenka(row["estimation"].toInt());

    if (idclass == 2 || idclass == 3) // �������� ������ � �������
      data.kur.addAsParagraph(title + ", " + ocenka);
    else if (idclass == 4) // ��������
      data.practic.addAsParagraph(title + " " + local::hours_to_str(hours) + ", " + ocenka);
    else if (idclass == 8) // ������ ����������������� ������
      data.sci.addAsParagraph(title);
    else if (idclass == 5) // �������� ����������
      data.gos.addAsParagraph(title + ", " + ocenka);
    else if (idclass == 6) // ��������� ������. ������
      vkrGos = "��������� ���������������� ������ �� ���� �" + vkr_title + "�, " + ocenka + "\n";
  }
  data.gos += vkrGos; // ��� ������ ��� �� ��� ��� ���� ����

  if (data.kur.empty())     data.kur     = isMale ? "�� ��������" : "�� ���������";
  if (data.practic.empty()) data.practic = isMale ? "�� ��������" : "�� ���������";
  if (data.sci.empty())     data.sci     = isMale ? "�� ��������" : "�� ���������";
  if (data.gos.empty())     data.gos     = isMale ? "�� ������"   : "�� �������";
}
//-------------------------------------------------------------------------
void ReportStudyingSpravka::GetDiscipData(std::vector<DiscipData>& data, int studentId)
{
  data.clear();

  // ������� ������ ����������
  string_t query = 
    "SELECT di.fulltitle,di.num_hours,di.idclass,di.zachet_edinica,pr.estimation,pr.iddiscip,pr.numplansemestr "
    "FROM disciplines as di, progress as pr "
    "WHERE di.deleted=0 and pr.deleted=0 and pr.idstud=" + toStr(studentId) + " and pr.iddiscip=di.id "
    "ORDER BY di.scan_number, pr.numplansemestr";

  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW	row = res.fetch_row())
  {
    int idclass = row["idclass"].toInt();
    if (idclass != 1) // ��������� ������ ������� ����������
      continue;

    DiscipData disc;
    disc.title   = row["fulltitle"];
    disc.hours   = row["num_hours"];
    disc.zachet_edinica = row["zachet_edinica"];
    disc.ocenka  = toOcenka(row["estimation"].toInt());
    disc.discId  = row["iddiscip"].toInt();
    disc.semestr = row["numplansemestr"].toInt();

    // �������� ��� ����� ��� ������� ������ ������
    for (size_t i = 0; i < data.size(); ++i)
      if (data[i].discId == disc.discId && data[i].title == disc.title)
      {
        if (disc.semestr >= data[i].semestr)
          data[i] = disc;
        disc.discId = -1; // ��� �������� ��� �� ���� ���������
        break;
      }
      if (disc.discId != -1)
        data.push_back(disc);
  }
}
//-------------------------------------------------------------------------
string_t ReportStudyingSpravka::CurrentDate()
{
  mybase::MYFASTRESULT res = theApp.GetCon().Query("SELECT CURDATE() as date");
  if (mybase::MYFASTROW	row = res.fetch_row())
    return date_to_str(row["date"], true);
  return date_to_str("");
}