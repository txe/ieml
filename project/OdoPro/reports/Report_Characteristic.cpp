#include "StdAfx.h"
#include "Report_Characteristic.h"
#include "../SingeltonApp.h"
#include "../json-aux-ext.h"

//---------------------------------------------------------------------------
void ReportCharacteristic::Run(int grpId, int studentId)
{
  r::PrivateData privData;
  GetPrivateData(privData, studentId);
  string_t fio = privData.secondName + " " + privData.firstName + " " + privData.thirdName;

  string_t otlPercent, horPercent, udlPercent;
  GetBallPercent(studentId, otlPercent, horPercent, udlPercent);

  string_t cafedra = "???";
  if (privData.grpName.subString(0,2) == "БУ" || privData.grpName.subString(0,3) == "ЭУН") cafedra = "экономического анализа и управления недвижимостью";
  if (privData.grpName.subString(0,2) == "МО")    cafedra = "инновационного менеджмента";
  if (privData.grpName.subString(0,2) == "МР")    cafedra = "стратегического маркетинга";
  if (privData.grpName.subString(0,3) == "ПГС")   cafedra = "архитектуры/технологии строительного производства";
  if (privData.grpName.subString(0,3) == "ТГВ")   cafedra = "теплогазоснабжения";
  if (privData.grpName.subString(0,1) == "Ю")     cafedra = "предпринимательского права";
  if (cafedra == "???")
    if (privData.direct == "Экономика")          cafedra = "экономического анализа и управления недвижимостью";
    else if (privData.direct == "Юриспруденция" && privData.specOrProfilTag == "маг") cafedra = "гражданского права и гражданского процесса";
    else if (privData.direct == "Юриспруденция") cafedra = "предпринимательского права";
    else if (privData.direct == "Менеджмент")    cafedra = "инновационного менеджмента";


  // хотя для некоторых групп не включено направление, но оно будет
  bool hackGroup = 
    privData.grpName == "ЮБ400/1" || privData.grpName == "ЮБ401/1" || privData.grpName == "ЮБ430/1" || 
    privData.grpName == "ЮБ431/1" || privData.grpName == "ЮБ500/1" || privData.grpName == "ЮБ501/1" || privData.grpName == "ЮБ530/1";
  if (hackGroup && privData.direct.empty())
    privData.direct = privData.specOrProfil;
  
    
  string_t itog;
  if (privData.direct != "")
    itog = DirectItog(studentId, privData.isMale);
  else if (privData.grpName.subString(0,1) == "Ю")
    itog = UrItog(studentId, privData.isMale);
  else
    itog = CommonItog(studentId, privData.isMale, privData.direct != "");

  string_t _A = privData.isMale ? "" : "а";

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
  macros.SelectionText("Минобрнауки России\n");
  macros.SelectionText("Федеральное государственное бюджетное образовательное учреждение\n");
  macros.SelectionText("высшего образования\n");
  macros.SelectionText("\"\"Нижегородский государственный архитектурно-строительный университет\"\" (ННГАСУ)\n\n");
  macros.InsertLine("Selection.MoveUp Unit:=wdLine, Count:=2");
  macros.SelectionParagraphFormat("Borders.Item(wdBorderBottom).LineStyle = wdLineStyleSingle");
  macros.SelectionParagraphFormat("Borders.Item(wdBorderBottom).LineWidth = wdLineWidth050pt");
  macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=2");
  macros.SelectionText("ИНСТИТУТ ЭКОНОМИКИ, УПРАВЛЕНИЯ И ПРАВА\n\n");
  macros.SelectionFont("Bold=true");
  macros.SelectionText("ХАРАКТЕРИСТИКА\n\n");
  macros.SelectionFont("Bold=false");
  macros.SelectionFont("Size=12");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");

  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphJustify");
  macros.SelectionText("vbTab");
  macros.SelectionText(fio + ", " + r::GetYear(privData.bornDate) + " года рождения, образование " + privData.prevEdu + ".\n");
  macros.SelectionText("vbTab");
  macros.SelectionText("Закончил" + _A + " в " + privData.prevDocYear + " году " + toQuate(privData.prevPlace) + ".\n");
  macros.SelectionText("vbTab");

  if (privData.direct != "")
    macros.SelectionText("В " + privData.inYear + " году поступил" + _A + " в ННГАСУ на места, не финансируемые из федерального бюджета, для обучения по направлению \"\"" + privData.direct + "\"\" по заочной форме.\n");
  else
    macros.SelectionText("В " + privData.inYear + " году поступил" + _A + " в ННГАСУ на места, не финансируемые из федерального бюджета, для обучения по специальности \"\"" + privData.specOrProfil + "\"\" по заочной форме.\n");
 
  macros.SelectionText("vbTab");
  macros.SelectionText("За время обучения в университете показал" + _A + " следующую успеваемость:\n");
 
  macros.SelectionText("- \"\"отлично\"\" - " + otlPercent + " %;\n");
  macros.SelectionText("- \"\"хорошо\"\" - " + horPercent + " %;\n");
  macros.SelectionText("- \"\"удовлетворительно\"\" - " + udlPercent + " %.\n");
 
  macros.SelectionText("vbTab");
  macros.SelectionText(itog);

  macros.SelectionText("vbTab");
  macros.SelectionText("Выполнил" + _A + " выпускную квалификационную работу по кафедре " + cafedra + ".\n");
  macros.SelectionText("vbTab");
  macros.SelectionText(fio + " административных взысканий за время обучения не имеет, пользуется уважением студентов и преподавателей.\n");
  macros.SelectionText("vbTab");

  if (privData.direct != "")
    macros.SelectionText("Характеристика дана для предоставления в Государственную экзаменационную комиссию по направлению \"\"" + privData.direct + "\"\".\n");
  else
    macros.SelectionText("Характеристика дана для предоставления в Государственную экзаменационную комиссию по специальности \"\"" + privData.specOrProfil + "\"\".\n");
 
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.SelectionText("\n");
  macros.SelectionText("vbTab");
  macros.SelectionText("Директор ИЭУП, профессор");
  macros.SelectionText("vbTab");
  macros.SelectionText("vbTab");
  macros.SelectionText("vbTab");
  macros.SelectionText("vbTab");
  macros.SelectionText("vbTab");
  macros.SelectionText("vbTab");
  macros.SelectionText("Д.В. Хавин\n");
  macros.SelectionText("\n");
  macros.SelectionText("vbTab");
  macros.SelectionText(r::GetCurrentDate("года") + "\n");
 
  macros.EndMacros();
  macros.RunMacros("");
}
//---------------------------------------------------------------------------
double SimpleRoundTo(double x,int z=-2)
{ 
  int j,k=1;
  if (0>x) 
    x=-x,k=-1;
  if (--z)
    if (z>0)
      for (j=0; j-z; j++)
        x/=10;
      else
        for (j=0;j+z;j++) 
          x*=10;
  x += 5; 
  x /= 10;
  x = (unsigned long long)x;
  if (++z > 0)
    for (j=0; j-z; j++) 
      x*=10;
    else
      for (j=0; j+z; j++) 
        x/=10;
  return x*k;
} 
//---------------------------------------------------------------------------
void ReportCharacteristic::GetBallPercent(int studentId, string_t& otlPercent, string_t& horPercent, string_t& udlPercent)
{
  theApp.GetCon().Query("drop temporary table if exists temp_stud");
  theApp.GetCon().Query("drop temporary table if exists temp_progress1");
  theApp.GetCon().Query("drop temporary table if exists temp_progress2");
  theApp.GetCon().Query("drop temporary table if exists temp_progress3");

  string_t query = "set @id = " + toStr(studentId);
    
  theApp.GetCon().Query(query);
  theApp.GetCon().Query("CREATE TEMPORARY TABLE temp_stud TYPE = HEAP "
        "SELECT st.id, st.firstname, st.secondname, st.thirdname, st.znum "
        "FROM students as st "
        "WHERE  st.id = @id AND st.deleted = 0 ");

  theApp.GetCon().Query("CREATE TEMPORARY TABLE temp_progress1 TYPE = HEAP "
        "SELECT pr.*, st.firstname, st.secondname, st.thirdname, st.znum "
        "FROM progress as pr, temp_stud  as st "
        "WHERE  pr.idstud = st.id AND pr.deleted = 0 AND pr.estimation < 3 ");

  theApp.GetCon().Query("CREATE TEMPORARY TABLE temp_progress2 TYPE = HEAP "
        "SELECT idstud, iddiscip, MAX(numplansemestr) as numplansemestr "
        "FROM temp_progress1 "
        "GROUP BY idstud, iddiscip ");

  theApp.GetCon().Query("CREATE TEMPORARY TABLE temp_progress3 TYPE = HEAP "
        "SELECT pr1.* "
        "FROM temp_progress1 as pr1, temp_progress2 as pr2 "
        "WHERE pr1.idstud = pr2.idstud AND pr1.iddiscip = pr2.iddiscip "
        "AND pr1.numplansemestr = pr2.numplansemestr ");

  mybase::MYFASTRESULT res = theApp.GetCon().Query("select idstud,secondname,firstname, thirdname, znum, "
        "sum(case  when estimation=0 then 1 else 0 end) as otl, "
        "sum(case  when estimation=1 then 1 else 0 end) as hor, "
        "sum(case  when estimation=2 then 1 else 0 end) as ud, "
        "AVG(ball) as sr "
        "from temp_progress3 "
        "group by idstud "
        "order by secondname ");

  if (mybase::MYFASTROW	row = res.fetch_row())
  {
    int otlCount = row["otl"].toInt();
    int horCount = row["hor"].toInt();
    int udlCount = row["ud"].toInt();

    int count = otlCount + horCount + udlCount;

    double percHOR = SimpleRoundTo(100.0*(double)horCount/(double)count, 0);
    double percOTL = SimpleRoundTo(100.0*(double)otlCount/(double)count, 0);
    double percUDOVL = 100.0 - percHOR - percOTL;

    char str[10];
    sprintf(str, "%4.0lf", percUDOVL);
    udlPercent = str;
    sprintf(str, "%4.0lf", percHOR);
    horPercent = str;
    sprintf(str, "%4.0lf", percOTL);
    otlPercent = str;
  }

  theApp.GetCon().Query("drop temporary table if exists temp_stud");
  theApp.GetCon().Query("drop temporary table if exists temp_progress1");
  theApp.GetCon().Query("drop temporary table if exists temp_progress2");
  theApp.GetCon().Query("drop temporary table if exists temp_progress3");
}
//---------------------------------------------------------------------------
string_t ReportCharacteristic::CommonItog(int studentId, bool isMale, bool isDirect)
{
  double itog_oc = 0.0;
  int num_itog = 0;

  string_t query = 
    "SELECT di.idclass,pr.ball "
    "FROM disciplines as di, progress as pr "
    "WHERE di.deleted=0 and pr.deleted=0 and pr.idstud=" + toStr(studentId) + " and pr.iddiscip=di.id";

  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW	row = res.fetch_row())
  {
    int d_class = row["idclass"].toInt();
    if (d_class == 5)   // Итоговая аттестация
    {
      num_itog++;
      itog_oc += aux::strtod(row["ball"]);
    }
  }
  if (num_itog > 0)
  {
    itog_oc = itog_oc / (float)num_itog;
    itog_oc = (float(int(itog_oc*10)) + (((itog_oc*10 - float((int(itog_oc*10)))) >= 0.5f) ? 1.0 : 0.0)) / 10.0f;
    itog_oc = float(itog_oc);
  }

  string_t _A = isMale ? "" :"а";
  string_t ocenka = r::toOcenka(t::type2cod(0, itog_oc));
  string_t result = "Сдал" + _A + " государственный экзамен по " +(isDirect ? "направлению" : "специальности") + " с итоговой оценкой \"\"" + ocenka + "\"\".\n";
  return result;
}
//---------------------------------------------------------------------------
string_t ReportCharacteristic::UrItog(int studentId, bool isMale)
{
  string_t query = 
    "SELECT di.idclass,pr.ball,di.fulltitle "
    "FROM disciplines as di, progress as pr "
    "WHERE di.deleted=0 and pr.deleted=0 and pr.idstud=" + toStr(studentId) + " and pr.iddiscip=di.id order by di.fulltitle";
  
  bool  second = false;
  string_t itog = "Сдал" + string_t(isMale ? "" : "а");
  string_t prip = " государственный экзамен по ";

  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW row = res.fetch_row())
  {
    int d_class = row["idclass"].toInt();
    if (d_class == 5)  // Итоговая аттестация
    {
      itog += (second ? ", " : "") + prip;
      second = true;

      string_t title = row["fulltitle"];
      if (title == "Гражданское право и гражданский процесс")
        itog += "гражданскому праву и гражданскому процессуальному праву";
      else
        itog += "теории государства и права";

      float itog_oc = aux::strtod(row["ball"]);
      string_t ocenka = r::toOcenka(t::type2cod(0, itog_oc));
      itog += " с оценкой \"\"" + ocenka + "\"\"";
    }
  }
  itog +=".\n";
  return itog;
}
//---------------------------------------------------------------------------
string_t ReportCharacteristic::DirectItog(int studentId, bool isMale)
{
  string_t query = 
    "SELECT di.idclass,pr.ball,di.fulltitle "
    "FROM disciplines as di, progress as pr "
    "WHERE di.deleted=0 and pr.deleted=0 and pr.idstud=" + toStr(studentId) + " and pr.iddiscip=di.id order by di.fulltitle";

  bool  second = false;
  string_t itog = "Сдал" + string_t(isMale ? "" : "а");
 // string_t prip = " государственный экзамен по ";

  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW row = res.fetch_row())
  {
    int d_class = row["idclass"].toInt();
    if (d_class == 5)  // Итоговая аттестация
    {
      itog += (second ? ", " : "");//+ prip;
      second = true;

      string_t title = row["fulltitle"];
      itog += title;

      float itog_oc = aux::strtod(row["ball"]);
      string_t ocenka = r::toOcenka(t::type2cod(0, itog_oc));
      itog += " с оценкой \"\"" + ocenka + "\"\"";
    }
  }
  itog +=".\n";
  return itog;

}
