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
//   macros.SelectionText("Минобрнауки России\n");
//   macros.SelectionText("Федеральное государственное бюджетное образовательное учреждение\n");
//   macros.SelectionText("высшего профессионального образования\n");
//   macros.SelectionText("\"\"Нижегородский государственный архитектурно-строительный университет\"\" (ННГАСУ)\n\n");
//   macros.InsertLine("Selection.MoveUp Unit:=wdLine, Count:=2");
//   macros.SelectionParagraphFormat("Borders.Item(wdBorderBottom).LineStyle = wdLineStyleSingle");
//   macros.SelectionParagraphFormat("Borders.Item(wdBorderBottom).LineWidth = wdLineWidth050pt");
//   macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=2");
//   macros.SelectionText("ИНСТИТУТ ЭКОНОМИКИ, УПРАВЛЕНИЯ И ПРАВА\n\n");
//   macros.SelectionFont("Bold=true");
//   macros.SelectionText("ХАРАКТЕРИСТИКА\n\n");
//   macros.SelectionFont("Bold=false");
//   macros.SelectionFont("Size=12");
//   macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
// 
//   ZMySqlQuery->RecNo=1;
// 
//   AnsiString Sex = ZMySqlQuery->Fields->FieldByNumber(11)->AsString;
//   bool isMan = (Sex[1]=='М') ? true : false;
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
//   macros.SelectionText(FIO+", "+byear+" года рождения, образование "+educ+".\n");
//   macros.SelectionText("vbTab");
//   macros.FilterText(eduplace);
//   macros.SelectionText("Закончил"+AnsiString(isMan?"":"а")+" в "+eduendyear+" году "+ eduplace + ".\n");
//   macros.SelectionText("vbTab");
// 
//   if (naprav != "")
//     macros.SelectionText("В "+enteryear+" году поступил"+AnsiString(isMan?"":"а")+" в ННГАСУ на места, не финансируемые из федерального бюджета, для обучения по направлению \"\""+naprav+"\"\" по заочной форме с применением дистанционных технологий.\n");
//   else
//     macros.SelectionText("В "+enteryear+" году поступил"+AnsiString(isMan?"":"а")+" в ННГАСУ на места, не финансируемые из федерального бюджета, для обучения по специальности \"\""+spec+"\"\" по заочной форме с применением дистанционных технологий.\n");
// 
//   macros.SelectionText("vbTab");
//   macros.SelectionText("За время обучения в университете показал"+AnsiString(isMan?"":"а")+" следующую успеваемость:\n");
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
//   Str="- \"\"отлично\"\" - "+percOTLStr+" %;\n";
//   macros.SelectionText(Str);
//   Str="- \"\"хорошо\"\" - "+percHORStr+" %;\n";
//   macros.SelectionText(Str);
//   Str="- \"\"удовлетворительно\"\" - "+percUDOVLStr+" %.\n";
//   macros.SelectionText(Str);
// 
//   AnsiString caf = "???";
//   if (grpName.SubString(1,2) == "БУ" || grpName.SubString(1,3) == "ЭУН") caf = "экономического анализа и управления недвижимостью";
//   if (grpName.SubString(1,2) == "МО") caf = "инвестиционного менеджмента";
//   if (grpName.SubString(1,2) == "МР") caf = "стратегического маркетинга";
//   if (grpName.SubString(1,3) == "ПГС") caf = "архитектуры/технологии строительного производства";
//   if (grpName.SubString(1,3) == "ТГВ") caf = "теплогазоснабжения";
//   if (grpName.SubString(1,1) == "Ю") caf = "предпринимательского права";
// 
//   float itog_oc;
//   InitItog(itog_oc);
//   //    MessageBox(0, AnsiString(itog_oc).c_str(),"",0);
//   macros.SelectionText("vbTab");
//   if (grpName.SubString(1,1) != "Ю")
//     macros.SelectionText("Сдал"+AnsiString(isMan?"":"а")+" государственный экзамен по "+(naprav != "" ? "направлению" : "специальности") + " с итоговой оценкой \"\""+GetEst_cur(itog_oc)+"\"\".\n");
//   else
//     macros.SelectionText(ItogForUr(isMan));
//   macros.SelectionText("vbTab");
//   macros.SelectionText("Выполнил"+AnsiString(isMan?"":"а")+" выпускную квалификационную работу по кафедре " + caf + ".\n");
//   macros.SelectionText("vbTab");
//   macros.SelectionText(FIO+" административных взысканий за время обучения не имеет, пользуется уважением студентов и преподавателей.\n");
//   macros.SelectionText("vbTab");
// 
//   if (naprav != "")
//     macros.SelectionText("Характеристика дана для предоставления в Государственную аттестационную комиссию по защите выпускной квалификационной работы по направлению \"\""+naprav+"\"\".\n");
//   else
//     macros.SelectionText("Характеристика дана для предоставления в Государственную аттестационную комиссию по защите выпускной квалификационной работы по специальности \"\""+spec+"\"\".\n");
// 
//   macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
//   macros.SelectionText("\n");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("Директор ИЭУП, профессор");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("vbTab");
//   macros.SelectionText("Д.В. Хавин\n");
//   AnsiString curdate = TDateTime::CurrentDate().FormatString("dd.mm.yyyy");
//   macros.SelectionText("\n");
//   macros.SelectionText("vbTab");
//   macros.SelectionText(curdate+"\n");
// 
//   macros.EndMacros();
//   macros.RunMacros();
}
