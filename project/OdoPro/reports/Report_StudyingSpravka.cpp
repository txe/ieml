#include "StdAfx.h"
#include <vector>
#include "Report_StudyingSpravka.h"
#include "../SingeltonApp.h"

//-------------------------------------------------------------------------
void ReportStudyingSpravka::Run(int grpId, int studentId)
{
  r::PrivateData privData;
  r::GetPrivateData(privData, studentId);
  DirectData naprData;
  GetDirectData(naprData, privData);  
  StudyData studyData;
  GetStudyData(studyData, studentId, privData.isMale, privData.vkrTitle);
  std::vector<DiscipData> discData;
  GetDiscipData(discData, studentId);

  bool renameUniver2011 = privData.inYear.toInt() < 2011 || (privData.inYear.toInt() == 2011 && (privData.inMonth.toInt() < 7 || (privData.inMonth.toInt() == 7 && privData.inDay.toInt() < 8)));
  bool stillStudying = privData.exitNum.empty();
  string_t maleEnd = privData.isMale ? "" : "�";

  // ��������
  string_t inInfo = "��������" + maleEnd + " � " + privData.inYear;    
  if (renameUniver2011)
    inInfo += " ���� � ��������������� ��������������� ���������� ������� ����������������� ����������� �������������� ��������������� ������������-������������ ����������� (������� �����)";
  else
    inInfo += " ���� � ����������� ��������������� ��������� ��������������� ���������� ������� ����������������� ����������� �������������� ��������������� ������������-������������ ����������� (������� �����)";

  // ���������� ��� ���������� ��������
  string_t outInfo = "���������" + maleEnd + " �������� � " + privData.outYear + " ���� � ����������� ��������������� ��������� ��������������� ���������� ������� ����������� �������������� ��������������� ������������-������������ ����������� (������� �����)";
  if (stillStudying)
    outInfo = "���������" + maleEnd + " �������� �\n���������� ��������";

  // ��������������� ��������
  string_t bottomInfo;
  if (renameUniver2011)
  {
    bottomInfo += "������������ ��������������� ����������� ���������� � 2011 ����.\n������� ������������ ��������������� ����������� � ��������������� ��������������� ���������� ������� ����������������� ����������� �������������� ��������������� ������������-������������ �����������.";
    bottomInfo += "\n";
  }
  bottomInfo += "������������ ��������������� ����������� ���������� � 2016 ����.\n������� ������������ ��������������� ����������� � ����������� ��������������� ��������� ��������������� ���������� ������� ����������������� ����������� �������������� ��������������� ������������-������������ �����������.";

  //if (stillStudying)
  //  bottomInfo.addAsParagraph("������� ������ �� ����������");
  //else
  bottomInfo.addAsParagraph("������ �� ���������� �� " + privData.exitDateFull + " ���� � " + privData.exitNum);


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
  macros.Cell(1, 4, 2, "Range.Text=" + toWrap(r::GetCurrentDate("����")));

  macros.Cell(1, 2, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 2, 1, "VerticalAlignment=wdCellAlignVerticalTop");

  // ���� ��������
  macros.SelectionText("���� �������� " + r::to_str_date(privData.bornDate, "����"));
  macros.SelectionTypeParagraph(2);
  macros.SelectionText("�������� �� ������ �����������, �� ��������� �������� ��������" + maleEnd + " �� ��������:\n");
  macros.SelectionText(privData.prevDoc + " " + privData.prevDocYear + " ���");
  macros.SelectionTypeParagraph(2);

  // ���������
  macros.SelectionText(inInfo);
  macros.SelectionTypeParagraph(2);

  // �����������
  macros.SelectionText(outInfo);
  macros.SelectionTypeParagraph(2);

  //macros.SelectionText("����������� ���� �������� �� ����� ����� 4 ����");
  //macros.SelectionTypeParagraph(2);

  // ����������� ����������/�������������:
  string_t maleStudy = privData.isMale ? "��������" : "���������";
  if (privData.isMagister)
    macros.SelectionText(maleStudy + " �� ��������� ������������ �� ����������� ����������:");
  else
    macros.SelectionText(maleStudy + " �� ��������� ������������ �� ����������� ����������:");
  macros.SelectionTypeParagraph();

  // �������� ���������� ������
  macros.SelectionText(naprData.stroka1Value);
  macros.SelectionTypeParagraph(2);

  // �������������/�������/���������� �������������� (���������):
  macros.SelectionText("�������������� (�������) ��������������� ���������:");
  macros.SelectionTypeParagraph();

  // �������� ���������� ������
  macros.SelectionText(naprData.stroka2Value);
  macros.SelectionTypeParagraph(2);

  if (privData.isMagister)
      macros.SelectionText("���� �������� ��������� ������������ � ����� ����� ��������\n2 ����");
  else
      macros.SelectionText("���� �������� ��������� ������������ � ����� ����� ��������\n4 ����");
  macros.SelectionTypeParagraph(2);

  // ��������
  macros.SelectionText("�������� ������ (�������):\n");
  macros.SelectionFont("Size=9");
  macros.SelectionText(toQuate(studyData.kur));
  macros.SelectionFont("Size=11");
  macros.SelectionTypeParagraph(2);

  // ��������
  macros.SelectionText("��������:\n");
  macros.SelectionFont("Size=9");
  macros.SelectionText(toQuate(studyData.practic));
  macros.SelectionFont("Size=11");
  macros.SelectionTypeParagraph(2);

  // ������� ������
  //macros.SelectionText("������-����������������� ������:\n");
  //macros.SelectionFont("Size=9");
  //macros.SelectionText(toQuate(studyData.sci));
  //macros.SelectionFont("Size=11");
  //macros.SelectionTypeParagraph(2);

  // ����
  macros.SelectionText("��������������� �������� ����������:\n");
  macros.SelectionFont("Size=9");
  macros.SelectionText(toQuate(studyData.gos));
  macros.SelectionFont("Size=11");

  // ������ �������

  // �������� ������� ���������
  macros.InsertLine("ActiveDocument.Tables.Item(4).Rows.Item(3).Range.Select");
  if (((int)discData.size()-2) > 0)
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
  macros.RunMacros(theApp.GetModuleDir() + "spravka.dot");
}
//-------------------------------------------------------------------------
void ReportStudyingSpravka::GetDirectData(DirectData& data, const r::PrivateData& privData)
{
  // ����������� ����������/�������������:
  // ���� ��������� ����������� - ������������ ����������� ����� �������������
  data.stroka1 = privData.direct.empty() ? DirectData::S1_SPEC : DirectData::S1_DIRECT;
  if (data.stroka1 == DirectData::S1_DIRECT)
    data.stroka1Value = privData.direct;
  else
    data.stroka1Value = privData.specOrProfil;
  data.stroka1Value = privData.shifrspec + " " + data.stroka1Value;

  // �������������/�������/���������� �������������� (���������):
  // ���� ���� ����������� �� ��� �������, ����� �������������, � ���� ��� (���) �� ���������� ��������������
  if (data.stroka1 == DirectData::S1_DIRECT) data.stroka2 = DirectData::S2_PROFIL;
  else if (privData.isMagister)                   data.stroka2 = DirectData::S2_MAGISTR;
  else                                            data.stroka2 = DirectData::S2_SPECIAL;

  if (data.stroka2 == DirectData::S2_SPECIAL)
    data.stroka2Value = privData.specializ;
  else
    data.stroka2Value = privData.specOrProfil;
}
//-------------------------------------------------------------------------
void ReportStudyingSpravka::GetStudyData(StudyData& data, int studentId, bool isMale, string_t vkr_title)
{
  data.kur     = "";
  data.practic = "";
  data.gos     = "";
  data.practic = "";
  string_t vkrGos;

  string_t query = string_t() +
    "SELECT di.fulltitle, di.num_hours, di.idclass, di.zachet_edinica, pr.estimation, pr.ball "
    "FROM disciplines as di, progress as pr "
    "WHERE di.deleted=0 and pr.deleted=0 and pr.idstud=" + aux::itow(studentId) + " and pr.iddiscip=di.id "
    "ORDER BY di.scan_number";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW	row = res.fetch_row())
  {
    int      idclass = row["idclass"].toInt();
    string_t title   = row["fulltitle"];
    string_t hours   = row["num_hours"];
    string_t ocenka  = r::toOcenka(row["estimation"].toInt());
    string_t ze      = row["zachet_edinica"];

    if (idclass == 2 || idclass == 3) // �������� ������ � �������
      data.kur.addAsParagraph(title.double_quote() + ", " + ocenka);
    else if (idclass == 4) // ��������
      data.practic.addAsParagraph(title.double_quote() + " " + ze + " �.�. (" + r::hours_to_str(hours) + "), " + ocenka);
    else if (idclass == 8) // ������ ����������������� ������
      data.sci.addAsParagraph(title.double_quote());
    else if (idclass == 5) // �������� ����������
      data.gos.addAsParagraph(title + ", " + ocenka);
    else if (idclass == 6) // ��������� ������. ������
      vkrGos = "��������� ���������������� ������ �� ���� �" + vkr_title + "�, " + ocenka;
  }
  if (vkrGos != "")
    data.gos.addAsParagraph(vkrGos); // ��� ������ ��� �� ��� ��� ���� ����

  if (data.kur.empty())     data.kur     = isMale ? "�� ��������" : "�� ���������";
  if (data.practic.empty()) data.practic = isMale ? "�� ��������" : "�� ���������";
  if (data.sci.empty())     data.sci     = isMale ? "�� ��������" : "�� ���������";
  if (data.gos.empty())     data.gos     = isMale ? "�� ��������"   : "�� ���������";
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
    disc.ocenka  = r::toOcenka(row["estimation"].toInt());
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
