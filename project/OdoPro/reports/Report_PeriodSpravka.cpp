#include "StdAfx.h"
#include <vector>
#include "Report_PeriodSpravka.h"
#include "../SingeltonApp.h"

//-------------------------------------------------------------------------
void ReportPeriodSpravka::Run(int grpId, int studentId)
{
  r::PrivateData privData;
  r::GetPrivateData(privData, studentId);
  DirectData naprData;
  GetDirectData(naprData, privData);  
  std::vector<DiscipData> discData;
  std::vector<DiscipData> curseWork;
  GetDiscipData(discData, curseWork, studentId, privData.vkrTitle, privData.lang);

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


  bottomInfo.addAsParagraph("������ �� ���������� �� " + privData.exitDateFull + " ���� � " + privData.exitNum);


  WordMacros macros;
  macros.BeginMacros();

  // insert page number
  macros.InsertLine("Selection.Sections(1).Footers(1).PageNumbers.Add PageNumberAlignment:= _");
  macros.InsertLine("      wdAlignPageNumberLeft, FirstPage:=True");
  
  // insert text before page number
  macros.InsertLine("ActiveWindow.ActivePane.View.SeekView = wdSeekCurrentPageFooter");
  macros.InsertLine("Selection.MoveLeft Unit:=wdCharacter, Count:=2");
  macros.InsertLine("Application.Keyboard (1049)");
  macros.InsertLine("Selection.TypeText Text:=\"�������� \"");
  macros.InsertLine("ActiveWindow.ActivePane.View.SeekView = wdSeekMainDocument");

  
  // ������ �������
  macros.InsertLine("ActiveDocument.PageSetup.TopMargin=70");
  macros.InsertLine("ActiveDocument.PageSetup.BottomMargin=55");
  macros.InsertLine("ActiveDocument.PageSetup.LeftMargin=55");
  macros.InsertLine("ActiveDocument.PageSetup.RightMargin=55");
  macros.SelectionFont("Bold=false");
  macros.SelectionFont("Size=11");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");

  // TITLE
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  macros.SelectionText("\n\n\n\n\n\n\n\n\n\n\n");
  macros.SelectionText("� ����������� ��������������� ��������� ��������������� ���������� ������� ����������� �������������� ��������������� ������������-������������ ����������� (����������� ������), ����� ������ ��������");
  macros.SelectionFont("Size=6");
  macros.SelectionText("\n\n");
  macros.SelectionFont("Size=11");
  
  // ���
  macros.SelectionFont("Size=12");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
  macros.TablesAdd(1, 2);
  macros.TableStyle(WordMacros::StyleNone);
  macros.TablesColumns(1, 1, "PreferredWidth = CentimetersToPoints(2.5)");
  macros.TablesColumns(1, 1, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  macros.SelectionText("�������\n���\n��������");
  macros.TablesColumns(1, 2, "PreferredWidth = CentimetersToPoints(15)");
  macros.TablesColumns(1, 2, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  macros.SelectionText(privData.secondName + "\n" + privData.firstName + "\n" + privData.thirdName);

  macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=2");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  macros.SelectionFont("Size=6");
  macros.SelectionText("\n");
  macros.SelectionFont("Size=11");

  // ���� ��������
  macros.SelectionFont("Size=12");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
  macros.SelectionText("���� �������� " + r::to_str_date(privData.bornDate, "����"));
  macros.SelectionTypeParagraph(1);
  macros.SelectionFont("Size=11");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  macros.SelectionText("������� ����������� � �������� �� ������ �����������, �� ��������� �������� ��������" + maleEnd + " �� ��������:\n");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
  macros.SelectionText(privData.prevDoc + " " + privData.prevDocYear + " ���");
  macros.SelectionTypeParagraph(1);
  
  // ���������
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  macros.SelectionText(inInfo);
  macros.SelectionTypeParagraph(2);

  // ����������� ����������/�������������:
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  if (privData.isMagister)
    macros.SelectionText("��������� �� ��������� ������������ (�� ����� �����) �� ����������� ����������:");
  else
    macros.SelectionText("��������� �� ��������� ������������ (�� ����� �����) �� ����������� ����������:");
  macros.SelectionTypeParagraph();

  // �������� ���������� ������
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
  macros.SelectionText(naprData.stroka1Value);
  macros.SelectionTypeParagraph(1);

  // �������������/�������/���������� �������������� (���������):
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  macros.SelectionText("�������������� (�������) ��������������� ���������:");
  macros.SelectionTypeParagraph();

  // �������� ���������� ������
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
  macros.SelectionText(naprData.stroka2Value);
  macros.SelectionTypeParagraph(1);

  if (privData.isMagister)
      macros.SelectionText("���� �������� ��������� ������������ � ����� ����� �������� 2 ����");
  else
      macros.SelectionText("���� �������� ��������� ������������ � ����� ����� �������� 4 ����");
  macros.SelectionTypeParagraph(1);

  
  int nextTableNum = AddProgramTable(macros, discData);
  int tableNum = AddProjectTable(macros, curseWork, nextTableNum);

  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
  macros.SelectionFont("Size=11");
  
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.SelectionText("�������������� ��������\n");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.SelectionText("���������� ��������.\n");

  macros.TablesAdd(3, 2);
  macros.TableStyle(WordMacros::StyleNone);
  macros.TablesColumns(tableNum, 1, "PreferredWidth = InchesToPoints(6.22)");
  macros.TablesColumns(tableNum, 2, "PreferredWidth = InchesToPoints(1.46)");
  macros.Cell(tableNum, 1, 1, "Range.Text=" + toWrap("������"));
  macros.Cell(tableNum, 2, 1, "Range.Text=" + toWrap("��������"));
  macros.Cell(tableNum, 3, 1, "Range.Text=" + toWrap("���������"));
  macros.Cell(tableNum, 1, 2, "Range.Text=" + toWrap("�.�. ������"));
  macros.Cell(tableNum, 2, 2, "Range.Text=" + toWrap("�.�. �����"));
  macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=100");
 
  macros.InsertLine("Selection.TypeText Text:=vbTab & vbTab & \"�.�.\"");
  macros.SelectionText("\n");

  tableNum += 1;
  macros.TablesAdd(1, 2);
  macros.TableStyle(WordMacros::StyleNone);
  macros.TablesColumns(tableNum, 1, "PreferredWidth = InchesToPoints(4)");
  macros.TablesColumns(tableNum, 2, "PreferredWidth = InchesToPoints(4)");
  macros.TablesColumns(tableNum, 2, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphRight");
  macros.Cell(tableNum, 1, 1, "Range.Text=" + toWrap("��������������� ����� 00000"));
  macros.Cell(tableNum, 1, 2, "Range.Text=" + toWrap("���� ������ " + r::GetCurrentDate("����")));
  macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=100");

  macros.SelectionText("\n��������� �������� �������� 2 ��������");

  macros.EndMacros();
  macros.RunMacros("");
}
//-------------------------------------------------------------------------
int ReportPeriodSpravka::AddProgramTable(WordMacros& macros, const std::vector<DiscipData>& discData)
{
  struct local
  {
    static void create_table(WordMacros& macros, int tableNum, int rowCount)
    {
      macros.TablesAdd(rowCount, 4);
      macros.TablesColumns(tableNum, 1, "PreferredWidth = InchesToPoints(3.03)");
      macros.TablesColumns(tableNum, 2, "PreferredWidth = InchesToPoints(1.18)");
      macros.TablesColumns(tableNum, 3, "PreferredWidth = InchesToPoints(1.53)");
      macros.TablesColumns(tableNum, 4, "PreferredWidth = InchesToPoints(1.47)");

      macros.Cell(tableNum, 1, 1, "Range.Select");
      macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
      macros.TablesColumns(tableNum, 1, "Select");
      //macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
      macros.SelectionText("������������ ��������� (�������) ���������,\n��� � ��� ��������");
      macros.TablesColumns(tableNum, 2, "Select");
      macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");    
      macros.SelectionText("����������\n��ר����\n������");
      macros.TablesColumns(tableNum, 3, "Select");
      macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
      macros.SelectionText("����������\n�������������\n�����");
      macros.TablesColumns(tableNum, 4, "Select");
      macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
      macros.SelectionText("������");

      macros.Cell(tableNum, 1, 1, "VerticalAlignment=wdCellAlignVerticalCenter");
      macros.Cell(tableNum, 1, 2, "VerticalAlignment=wdCellAlignVerticalCenter");
      macros.Cell(tableNum, 1, 3, "VerticalAlignment=wdCellAlignVerticalCenter");
      macros.Cell(tableNum, 1, 4, "VerticalAlignment=wdCellAlignVerticalCenter");

      macros.TableRange(tableNum, 2, 1, rowCount, 4, "Select");      
      macros.InsertLine("Selection.Borders(wdBorderHorizontal).LineStyle = wdLineStyleNone");
      macros.InsertLine("Selection.Borders(wdBorderDiagonalDown).LineStyle = wdLineStyleNone");
      macros.InsertLine("Selection.Borders(wdBorderDiagonalUp).LineStyle = wdLineStyleNone");
    }
  };

  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  macros.SelectionFont("Size=11");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.SelectionText("�������� � ���������� � ����������� �������� ��������������� ���������");
  macros.SelectionTypeParagraph(1);
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");

  macros.SelectionFont("Size=10");
  bool needSecondTable = true;
  int tableNum = 2;
  int rowCount = 12;
  // check if we need one table
  if ((int)discData.size() < rowCount)
  {
    rowCount = discData.size() + 1;
    needSecondTable = false;
  } 
  else if ((int)discData.size() < rowCount + 2)
  {
    rowCount -= 2; // so more disc in the second table
  }
  local::create_table(macros, tableNum, rowCount + (needSecondTable ? 1 : 0));

  for (size_t i = 0; i < discData.size() && (int)i < rowCount - 1; ++i)
  {
    string_t title = discData[i].title;
    macros.Cell(tableNum, i + 2, 1, "Range.Text=" + toWrap(title));
    macros.Cell(tableNum, i + 2, 2, "Range.Text=" + toWrap(discData[i].zachet_edinica));
    macros.Cell(tableNum, i + 2, 3, "Range.Text=" + toWrap(discData[i].hours));
    macros.Cell(tableNum, i + 2, 4, "Range.Text=" + toWrap(discData[i].ocenka));
  }
  if (needSecondTable)
    macros.Cell(tableNum, rowCount + 1, 1, "Range.Text=" + toWrap("��������� ������� ��������� �� �������"));
  macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=100");

  macros.SelectionFont("Size=11");
  macros.InsertLine("Selection.InsertBreak Type:=wdPageBreak");

  if (!needSecondTable)
    return tableNum + 1;

  macros.SelectionFont("Size=10");
  macros.SelectionText("��������� �������� � ���������� � ����������� �������� ��������������� ���������");
  macros.SelectionTypeParagraph(1);
  tableNum = 3;
  int rowCount2 = 1 + discData.size() - (rowCount - 1);
  local::create_table(macros, tableNum, rowCount2);
  for (size_t i = (rowCount - 1); i < discData.size(); ++i)
  {
    int pos = 1 + i - (rowCount - 1) + 1;
    string_t title = discData[i].title;
    macros.Cell(tableNum, pos, 1, "Range.Text=" + toWrap(title));
    macros.Cell(tableNum, pos, 2, "Range.Text=" + toWrap(discData[i].zachet_edinica));
    macros.Cell(tableNum, pos, 3, "Range.Text=" + toWrap(discData[i].hours));
    macros.Cell(tableNum, pos, 4, "Range.Text=" + toWrap(discData[i].ocenka));
  }
  macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=100");
  macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=100");
  macros.SelectionFont("Size=11");
  macros.SelectionText("\n");
  
  return tableNum + 1;
}
//-------------------------------------------------------------------------
int ReportPeriodSpravka::AddProjectTable(WordMacros& macros, const std::vector<DiscipData>& curseWork, int tableNum)
{
  if (curseWork.empty())
    return tableNum;

  macros.SelectionFont("Size=10");
  macros.TablesAdd(curseWork.size() + 1, 2);
  macros.TablesColumns(tableNum, 1, "PreferredWidth = InchesToPoints(5.73)");
  macros.TablesColumns(tableNum, 2, "PreferredWidth = InchesToPoints(1.47)");
  macros.TablesColumns(tableNum, 1, "Select");
  macros.SelectionText("�������� ������ (�������)");
  macros.TablesColumns(tableNum, 2, "Select");
  macros.SelectionText("������");

  macros.Cell(tableNum, 1, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.TablesColumns(tableNum, 2, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");    

  for (size_t i = 0; i < curseWork.size(); ++i)
  {
    macros.Cell(tableNum, i + 2, 1, "Range.Text=" + toWrap(curseWork[i].title));
    macros.Cell(tableNum, i + 2, 2, "Range.Text=" + toWrap(curseWork[i].ocenka));
  }

  macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=100");
  macros.SelectionFont("Size=11");
  macros.SelectionText("\n");

  return tableNum + 1;
}
//-------------------------------------------------------------------------
void ReportPeriodSpravka::GetDirectData(DirectData& data, const r::PrivateData& privData)
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
void ReportPeriodSpravka::GetDiscipData(std::vector<DiscipData>& data, std::vector<DiscipData>& curseWork, int studentId, string_t vkr_title, string_t lang)
{
  data.clear();
  curseWork.clear();
  std::vector<DiscipData> practice;
  std::vector<DiscipData> itog;
  DiscipData wkrWork;

  // ������� ������ ����������
  string_t query = 
    "SELECT di.fulltitle,di.num_hours,di.idclass,di.zachet_edinica,pr.estimation,pr.iddiscip,pr.numplansemestr "
    "FROM disciplines as di, progress as pr "
    "WHERE di.deleted=0 and pr.deleted=0 and pr.idstud=" + toStr(studentId) + " and pr.iddiscip=di.id "
    "ORDER BY di.scan_number, pr.numplansemestr";

  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW	row = res.fetch_row())
  {
    DiscipData disc;
    disc.title   = row["fulltitle"];
    disc.hours   = row["num_hours"];
    disc.zachet_edinica = row["zachet_edinica"];
    disc.ocenka  = r::toOcenka(row["estimation"].toInt());
    disc.discId  = row["iddiscip"].toInt();
    disc.semestr = row["numplansemestr"].toInt();

    int idclass = row["idclass"].toInt();
    if (idclass == r::DT_CURSE_WORK || idclass == r::DT_CURSE_PRACTICE)
    {
      disc.title = disc.title.double_quote();
      curseWork.push_back(disc);
    }
    else if (idclass == r::DT_PRACTICE)
    {
      practice.push_back(disc);
    }
    else if (idclass == r::DT_ITOG_ATESTACIA)
    {
      disc.hours = "x";
      disc.zachet_edinica = "x";
      itog.push_back(disc);
    }
    else if (idclass == r::DT_KVALIFIC_WORK)
    {
      disc.hours = "x";
      disc.zachet_edinica = "x";
      disc.title = disc.title.toLower() + " �" + vkr_title + "�";
      wkrWork = disc;
    }
    else if (idclass == r::DT_COMMON)
    {
      if (disc.title.toUpper().trim() == string_t(L"����������� ����"))
        disc.title += " (" + lang + ")";
      if (disc.title.toUpper().indexOf(L"����������") != -1 && disc.title.toUpper().indexOf(L"��������") != -1)
      {
        disc.title = "���������� �������� � �����";
        disc.zachet_edinica = "-";
      }
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
  if (practice.size())
  {
    data.push_back(DiscipData("��������"));
    data.push_back(DiscipData("� ��� �����:"));
    for (size_t i = 0; i < practice.size(); ++i)
      data.push_back(practice[i]);
  }
  if (itog.size() || !wkrWork.title.empty())
  {
    data.push_back(DiscipData("��������������� �������� ����������"));
    data.push_back(DiscipData("� ��� �����:"));
    for (size_t i = 0; i < itog.size(); ++i)
      data.push_back(itog[i]);
    if (!wkrWork.title.empty())
      data.push_back(wkrWork);
  }
}
