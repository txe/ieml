#include "StdAfx.h"
#include <vector>
#include "ReportStudyingSpravka.h"
#include "../SingeltonApp.h"

//
inline string_t GetDate(string_t str, bool isYear = false)
{
    static string_t mounthNames[12]={ "������", "�������", "�����", "������", "���", "����", "����", "�������", "��������", "�������", "������", "�������"};
    string_t res = "";
    try
    {
        res += str.subString(8,2);
        if (res.size() == 1) 
            res = "0" + res;
        res += " ";
        int mNum = (str.subString(5, 2).toInt()-1) % 12;
        if (mNum < 0)
            return "<���������� ����!>";
        res += mounthNames[mNum] + " ";
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

//-------------------------------------------------------------------------
void ReportStudyingSpravka::Run(int grpId, int studentId)
{
  PrivateData data;
  GetPrivateData(data, studentId);
  std::vector<string_t> cursJob;
  std::vector<string_t> practicJob;


  // ��������
  string_t inS = " ���� � ����������� ��������������� ��������� ��������������� ���������� ������� ����������������� ����������� �������������� ��������������� ������������-������������ ����������� (������� �����)";
  if (data.inYear.toInt() < 2011 || (data.inYear.toInt() == 2011 && (data.inMonth.toInt() < 7 || (data.inMonth.toInt() == 7 && data.inDay.toInt() < 8))))
    inS = " ���� � ��������������� ��������������� ���������� ������� ����������������� ����������� \"\"������������� ��������������� ������������-������������ �����������\"\" (������� �����)";
  if (data.male)
    inS = "�������� � " + data.inYear + inS;
  else
    inS = "��������� � " + data.inYear + inS;
  // ����������
  string_t outS = " ���� � ����������� ��������������� ��������� ��������������� ���������� ������� ����������������� ����������� �������������� ��������������� ������������-������������ ����������� (������� �����)";
  if (data.male)
    outS = "�������� �������� � " + data.outYear + outS;
  else
    outS = "��������� �������� � " + data.outYear + outS;



  WordMacros macros;
  macros.BeginMacros();

  // ���������� ������ �������
  macros.Cell(1, 1, 2, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 1, 2, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.SelectionText(data.secondName + "\n" + data.firstName + "\n" + data.thirdName);

  macros.Cell(1, 2, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 2, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.Cell(1, 2, 1, "Range.Text= \"���� �������� " + data.bornDate + "\""); // TODO �����������

  macros.Cell(1, 4, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 4, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.Cell(1, 4, 1, "Range.Text= \"" + data.prevDoc + " " + data.prevDocYear + " ���\"");

  macros.Cell(1, 5, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 5, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.Cell(1, 5, 1, "Range.Text= \"" + inS + "\"");

  macros.Cell(1, 6, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 6, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.Cell(1, 6, 1, "Range.Text= \"" + outS + "\"");

  macros.EndMacros();
  macros.RunMacros("spravka.dot");//"spravka.doc");
}
//-------------------------------------------------------------------------
void ReportStudyingSpravka::GetPrivateData(PrivateData& data, int studentId)
{
  data.firstName = data.secondName = data.thirdName = data.bornDate = data.vipQualifWork = data.prevDoc = data.prevDocYear = data.inYear = "???";
  data.inMonth = data.inMonth = "0";
  data.outYear = data.outMonth = data.outDay = "???";
  data.specOrProfil = data.direct = data.specializ = data.qualific = data.lang = "???";
  data.diplomNum = "xxx 000000";
  data.regNum = data.dataVidachi = data.dataQualific = "00.00.0000";

  string_t  query = string_t() +
    "SELECT s.secondname,s.firstname,s.thirdname,s.bdate,s.vkr_title," \
    "s.edudocid,s.eduenddate,s.specid,s.enterdate,s.exitdate,s.sex,v.title as lang,s.edunumdiplom," \
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
    data.bornDate   = GetDate(row["bdate"],true);
    data.vipQualifWork = row["vkr_title"];
    data.prevDoc       = theApp.GetTitleForKeyFromVoc(VK_EDUDOC, row["edudocid"].toInt(), true);
    data.prevDocYear   = GetYear(row ["eduenddate"]);
    data.specOrProfil  = theApp.GetTitleForKeyFromVoc(VK_SPECS, row["specid"].toInt(), true);
    data.specializ  = theApp.GetTitleForKeyFromVoc(VK_SPEZIALIZ, row["specid"].toInt(), true);
    data.qualific   = theApp.GetTitleForKeyFromVoc(VK_QUALIFIC, row["specid"].toInt(), true);
    data.direct     = theApp.GetTitleForKeyFromVoc(VK_DIRECT, row["directid"].toInt(), true);
    data.inYear     = GetYear(row["enterdate"]);
    data.inMonth    = GetMonth(row["enterdate"]);
    data.inDay      = GetDay(row["enterdate"]);
    data.outYear    = GetYear(row["exitdate"]);
    data.outMonth   = GetMonth(row["exitdate"]);
    data.outDay     = GetDay(row["exitdate"]);
    data.male       = row["sex"] != string_t("�") && row["sex"] != string_t("�");
    data.lang       = row["lang"];

    data.diplomNum = row["edunumdiplom"];
    data.regNum    = row["edunumreg"];
    data.dataVidachi = GetDate(row["edudatediplom"]);
    data.dataQualific = GetDate(row["edudatequalif"]);
  }
}