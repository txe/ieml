#include "StdAfx.h"
#include "ReportCharacteristic.h"

//---------------------------------------------------------------------------
void ReportCharacteristic::Run(int grpId, int studentId)
{
//   WordMacros macros;
// 
//   macros.BeginMacros();
// 
//   macros.InsertLine("ActiveDocument.PageSetup.TopMargin=70");
//   macros.InsertLine("ActiveDocument.PageSetup.BottomMargin=55");
//   macros.InsertLine("ActiveDocument.PageSetup.LeftMargin=55");
//   macros.InsertLine("ActiveDocument.PageSetup.RightMargin=55");
// 
//   macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
// 
//   macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
//   macros.SelectionFont("Bold=false");
//   macros.SelectionFont("Size=12");
//   macros.SelectionText("����������� ������\n");
//   macros.SelectionText("����������� ��������������� ��������� ��������������� ����������\n");
//   macros.SelectionText("������� ����������������� �����������\n");
//   macros.SelectionText("\"\"������������� ��������������� ������������-������������ �����������\"\" (������)\n\n");
//   macros.InsertLine("Selection.MoveUp Unit:=wdLine, Count:=2");
//   macros.SelectionParagraphFormat("Borders.Item(wdBorderBottom).LineStyle = wdLineStyleSingle");
//   macros.SelectionParagraphFormat("Borders.Item(wdBorderBottom).LineWidth = wdLineWidth050pt");
//   macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=2");
//   macros.SelectionText("�������� ���������, ���������� � �����\n\n");
//   macros.SelectionFont("Bold=true");
//   macros.SelectionText("��������������\n\n");
//   macros.SelectionFont("Bold=false");
//   macros.SelectionFont("Size=12");
//   macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
// 
//   ZMySqlQuery->RecNo=1;
// 
//   AnsiString Sex = ZMySqlQuery->Fields->FieldByNumber(11)->AsString;
//   bool isMan = (Sex[1]=='�') ? true : false;
//   AnsiString FIO = ZMySqlQuery->Fields->FieldByNumber(2)->AsString;
//   AnsiString byear = GetYear_cur(ZMySqlQuery->Fields->FieldByNumber(6)->AsString);
//   AnsiString educ = WCGetTitleForKeyNum(EDUCATIONS,ZMySqlQuery->Fields->FieldByNumber(7)->AsString.ToInt());
//   AnsiString eduendyear = GetYear_cur(ZMySqlQuery->Fields->FieldByNumber(8)->AsString);
//   AnsiString eduplace = ZMySqlQuery->Fields->FieldByNumber(9)->AsString;
//   AnsiString enteryear = GetYear_cur(ZMySqlQuery->Fields->FieldByNumber(10)->AsString);
//   AnsiString spec = WCGetTitleForKeyNum(SPECS, ZMySqlQuery->Fields->FieldByNumber(3)->AsString.ToInt());
//   AnsiString grpName = WCGetTitleForKeyNum(GROUPS, ZMySqlQuery->Fields->FieldByNumber(12)->AsString.ToInt());
//   AnsiString naprav = WCGetTitleForKeyNum(DIRECTS, ZMySqlQuery->Fields->FieldByNumber(13)->AsString.ToInt());
// 
//   macros.SelectionParagraphFormat("Alignment=wdAlignParagraphJustify");
//   macros.SelectionText("vbTab");
//   macros.SelectionText(FIO+", "+byear+" ���� ��������, ����������� "+educ+".\n");
//   macros.SelectionText("vbTab");
//   macros.FilterText(eduplace);
//   macros.SelectionText("��������"+AnsiString(isMan?"":"�")+" � "+eduendyear+" ���� "+ eduplace + ".\n");
//   macros.SelectionText("vbTab");
// 
//   if (naprav != "")
//     macros.SelectionText("� "+enteryear+" ���� ��������"+AnsiString(isMan?"":"�")+" � ������ �� �����, �� ������������� �� ������������ �������, ��� �������� �� ����������� \"\""+naprav+"\"\" �� ������� ����� � ����������� ������������� ����������.\n");
//   else
//     macros.SelectionText("� "+enteryear+" ���� ��������"+AnsiString(isMan?"":"�")+" � ������ �� �����, �� ������������� �� ������������ �������, ��� �������� �� ������������� \"\""+spec+"\"\" �� ������� ����� � ����������� ������������� ����������.\n");
// 
//   macros.SelectionText("vbTab");
//   macros.SelectionText("�� ����� �������� � ������������ �������"+AnsiString(isMan?"":"�")+" ��������� ������������:\n");
//   int countUDOVL,countHOR,countOTL;
//   double percUDOVL,percHOR,percOTL;
//   AnsiString AvrgBallStr;
//   GetBallsCount(countUDOVL,percUDOVL,countHOR,percHOR,countOTL,percOTL,AvrgBallStr,AnsiString(idstudent));
// 
//   AnsiString percUDOVLStr=AnsiString(percUDOVL),
//     percHORStr=AnsiString(percHOR),
//     percOTLStr=AnsiString(percOTL);
//   AnsiString Str;
//   char str[10];
//   sprintf(str,"%4.0lf",percUDOVLStr.ToDouble());
//   percUDOVLStr = AnsiString(str);
//   sprintf(str,"%4.0lf",percHORStr.ToDouble());
//   percHORStr = AnsiString(str);
//   sprintf(str,"%4.0lf",percOTLStr.ToDouble());
//   percOTLStr = AnsiString(str);
// 
//   Str="- \"\"�������\"\" - "+percOTLStr+" %;\n";
//   macros.SelectionText(Str);
//   Str="- \"\"������\"\" - "+percHORStr+" %;\n";
//   macros.SelectionText(Str);
//   Str="- \"\"�����������������\"\" - "+percUDOVLStr+" %.\n";
//   macros.SelectionText(Str);
// 
//   AnsiString caf = "???";
//   if (grpName.SubString(1,2) == "��" || grpName.SubString(1,3) == "���") caf = "�������������� ������� � ���������� �������������";
//   if (grpName.SubString(1,2) == "��") caf = "��������������� �����������";
//   if (grpName.SubString(1,2) == "��") caf = "��������������� ����������";
//   if (grpName.SubString(1,3) == "���") caf = "�����������/���������� ������������� ������������";
//   if (grpName.SubString(1,3) == "���") caf = "������������������";
//   if (grpName.SubString(1,1) == "�") caf = "�������������������� �����";
// 
//   float itog_oc;
//   InitItog(itog_oc);
//   //    MessageBox(0, AnsiString(itog_oc).c_str(),"",0);
//   macros.SelectionText("vbTab");
//   if (grpName.SubString(1,1) != "�")
//     macros.SelectionText("����"+AnsiString(isMan?"":"�")+" ��������������� ������� �� "+(naprav != "" ? "�����������" : "�������������") + " � �������� ������� \"\""+GetEst_cur(itog_oc)+"\"\".\n");
//   else
//     macros.SelectionText(ItogForUr(isMan));
//   macros.SelectionText("vbTab");
//   macros.SelectionText("��������"+AnsiString(isMan?"":"�")+" ��������� ���������������� ������ �� ������� " + caf + ".\n");
//   macros.SelectionText("vbTab");
//   macros.SelectionText(FIO+" ���������������� ��������� �� ����� �������� �� �����, ���������� ��������� ��������� � ��������������.\n");
//   macros.SelectionText("vbTab");
// 
//   if (naprav != "")
//     macros.SelectionText("�������������� ���� ��� �������������� � ��������������� �������������� �������� �� ������ ��������� ���������������� ������ �� ����������� \"\""+naprav+"\"\".\n");
//   else
//     macros.SelectionText("�������������� ���� ��� �������������� � ��������������� �������������� �������� �� ������ ��������� ���������������� ������ �� ������������� \"\""+spec+"\"\".\n");
// 
//   macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
//   macros.SelectionText("\n");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("�������� ����, ���������");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("�.�. �����\n");
//   AnsiString curdate = TDateTime::CurrentDate().FormatString("dd.mm.yyyy");
//   macros.SelectionText("\n");
//   macros.SelectionText("vbTab");
//   macros.SelectionText(curdate+"\n");
// 
//   macros.EndMacros();
//   macros.RunMacros();
}
