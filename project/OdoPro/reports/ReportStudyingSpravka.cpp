#include "StdAfx.h"
#include <vector>
#include "ReportStudyingSpravka.h"
#include "../SingeltonApp.h"

//-------------------------------------------------------------------------
inline string_t date_to_str(string_t str, bool isYear = false)
{
    static string_t mounthNames[12]={ "января", "февраля", "марта", "апреля", "мая", "июня", "июля", "августа", "сентября", "октября", "ноября", "декабря"};
    string_t res = "";
    try
    {
      // число 
        res += str.subString(8,2);
        if (res.size() == 1) 
            res = "0" + res;
        res += " ";
      // месяц 
        int mNum = (str.subString(5, 2).toInt()-1) % 12;
        if (mNum < 0)
            return "<невалидная дата!>";
        res += mounthNames[mNum] + " ";
      // год
        res += str.subString(0,4);
        if (isYear) 
            res += " года";
    }
    catch(...)
    {
        return "<невалидная дата!>";
    }
    return res;
}
//-------------------------------------------------------------------------
inline string_t date_to_str2(string_t str)
{
  string_t res = "";
  try
  {
    // число 
    res += str.subString(8,2);
    if (res.size() == 1) 
      res = "0" + res;
    res += ".";
    // месяц
    string_t m = str.subString(5, 2);
    if (m.size() == 1)
      m = m + res;
    res += m + ".";
    // год
    res += str.subString(0,4);
  }
  catch(...)
  {
    return "<невалидная дата!>";
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
  static string_t ocenkaList[]={"отлично","хорошо","удовлетворительно","неудовлетворительно","зачтено","незачтено"};
  return ocenkaList[num];
}
//-------------------------------------------------------------------------
void ReportStudyingSpravka::Run(int grpId, int studentId)
{
  PrivateData privData;
  GetPrivateData(privData, studentId);
  StudyData studyData;
  GetStudyData(studyData, studentId, privData.isMale);
  std::vector<DiscipData> discData;
  GetDiscipData(discData, studentId);

  // поступил
  string_t inS = " году в федеральное государственное бюджетное образовательное учреждение высшего профессионального образования «Нижегородский государственный архитектурно-строительный университет» (заочная форма)";
  if (privData.inYear.toInt() < 2011 || (privData.inYear.toInt() == 2011 && (privData.inMonth.toInt() < 7 || (privData.inMonth.toInt() == 7 && privData.inDay.toInt() < 8))))
    inS = " году в государственное образовательное учреждение высшего профессионального образования \"\"Нижегородский государственный архитектурно-строительный университет\"\" (заочная форма)";
  if (privData.isMale)
    inS = "Поступил в " + privData.inYear + inS;
  else
    inS = "Поступила в " + privData.inYear + inS;
  // выпустился
  string_t outS = " году в федеральном государственном бюджетном образовательном учреждении высшего профессионального образования «Нижегородский государственный архитектурно-строительный университет» (заочная форма)";
  if (privData.isMale)
    outS = "Завершил обучение в " + privData.outYear + outS;
  else
    outS = "Завершила обучение в " + privData.outYear + outS;


  WordMacros macros;
  macros.BeginMacros();

  // заполнение первой таблицы
  macros.Cell(1, 1, 2, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 1, 2, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.SelectionText(privData.secondName + "\n" + privData.firstName + "\n" + privData.thirdName);

  macros.Cell(1, 2, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 2, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.Cell(1, 2, 1, "Range.Text=" + toWrap("Дата рождения " + privData.bornDate)); // TODO просклонять

  macros.Cell(1, 4, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 4, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.Cell(1, 4, 1, "Range.Text=" + toWrap(privData.prevDoc + " " + privData.prevDocYear + " год"));

  macros.Cell(1, 5, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 5, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.Cell(1, 5, 1, "Range.Text=" + toWrap(inS));

  macros.Cell(1, 6, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 6, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.Cell(1, 6, 1, "Range.Text=" + toWrap(outS));

  macros.Cell(1, 11, 2, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.Cell(1, 11, 2, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.Cell(1, 11, 2, "Range.Text=" + toWrap(CurrentDate()));

  macros.Cell(1, 13, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 13, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.SelectionText(studyData.kur);

  macros.Cell(1, 15, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 15, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.SelectionText(studyData.practic);

  macros.Cell(1, 17, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 17, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.SelectionText(studyData.sci);

  macros.Cell(1, 19, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 19, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.SelectionText(studyData.gos);

  // заполним таблицу дисциплин
  macros.InsertLine("ActiveDocument.Tables.Item(4).Rows.Item(2).Range.Select");
  macros.InsertLine("Selection.InsertRowsBelow " + toStr(discData.size()-1));
  for (size_t i = 0; i < discData.size(); ++i)
  {
    string_t title = discData[i].title;
    if (title.toUpper().trim() == string_t(L"ИНОСТРАННЫЙ ЯЗЫК"))
      title += " (" + privData.lang + ")";
    macros.Cell(4, i + 2, 1, "Range.Text=" + toWrap(title));
    macros.Cell(4, i + 2, 2, "Range.Text=" + toWrap(discData[i].zachet_edinica));
    macros.Cell(4, i + 2, 3, "Range.Text=" + toWrap(discData[i].hours));
    macros.Cell(4, i + 2, 4, "Range.Text=" + toWrap(discData[i].ocenka));
  }

  macros.Cell(5, 1, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(5, 1, 1, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.SelectionText("Приказ об отчислении от " + privData.exitDate + " г. № " + privData.exitNum);
  
  macros.EndMacros();
  macros.RunMacros("spravka.dot");//"spravka.doc");
}
//-------------------------------------------------------------------------
void ReportStudyingSpravka::GetPrivateData(PrivateData& data, int studentId)
{
  data.firstName = data.secondName = data.thirdName = data.bornDate = data.vipQualifWork = data.prevDoc = data.prevDocYear = data.inYear = "???";
  data.inMonth = data.inMonth = "0";
  data.outYear = data.outMonth = data.outDay = data.exitDate = data.exitNum = "???";
  data.specOrProfil = data.direct = data.specializ = data.qualific = data.lang = "???";
  data.diplomNum = "xxx 000000";
  data.regNum = data.dataVidachi = data.dataQualific = "00.00.0000";

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
    data.exitDate   = date_to_str2(row["exitdate"]);
    data.exitNum    = row["exitnum"];
    data.isMale     = row["sex"] != string_t("Ж") && row["sex"] != string_t("ж");
    data.lang       = row["lang"];

    data.diplomNum = row["edunumdiplom"];
    data.regNum    = row["edunumreg"];
    data.dataVidachi = date_to_str(row["edudatediplom"]);
    data.dataQualific = date_to_str(row["edudatequalif"]);
  }
}
//-------------------------------------------------------------------------
void ReportStudyingSpravka::GetStudyData(StudyData& data, int studentId, bool isMale)
{
  data.kur     = "";
  data.practic = "";
  data.gos     = "";
  data.practic = "";

  struct local
  {
    static string_t hours_to_str(string_t hours)
    {
      int num = hours.toInt();
      if (num >= 5 && num <= 20) hours += " часов"; // исключение из правил
      else
      {
        num %= 10;
        if (num == 1) hours += " час";
        else if (num >= 2 && num <= 4) hours += " часа";
        else hours += " часов";
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

    if (idclass == 2 || idclass == 3) // курсовые работы и проекты
      data.kur += title + ", " + ocenka + "\n";
    else if (idclass == 4) // практика
      data.practic += title + " " + local::hours_to_str(hours) + ", " + ocenka + "\n";
    else if (idclass == 8) // научно исследовательская работа
      data.sci += title + "\n";
    else if (idclass == 5) // итоговая аттестация
      data.gos += title + ", " + ocenka + "\n";
  }
  
  if (!data.kur.empty())     data.kur = data.kur.subString(0, -1);
  if (!data.practic.empty()) data.practic = data.practic.subString(0, -1);
  if (!data.sci.empty())     data.sci = data.sci.subString(0, -1);
  if (!data.gos.empty())     data.gos = data.gos.subString(0, -1);

  if (data.kur.empty())     data.kur     = isMale ? "не выполнял" : "не выполняла";
  if (data.practic.empty()) data.practic = isMale ? "не проходил" : "не проходила";
  if (data.sci.empty())     data.sci     = isMale ? "не выполнял" : "не выполняла";
  if (data.gos.empty())     data.gos     = isMale ? "не сдавал"   : "не сдавала";
}
//-------------------------------------------------------------------------
void ReportStudyingSpravka::GetDiscipData(std::vector<DiscipData>& data, int studentId)
{
  data.clear();

  // соберем сперва дисциплины
  string_t query = 
    "SELECT di.fulltitle,di.num_hours,di.idclass,di.zachet_edinica,pr.estimation,pr.iddiscip,pr.numplansemestr "
    "FROM disciplines as di, progress as pr "
    "WHERE di.deleted=0 and pr.deleted=0 and pr.idstud=" + toStr(studentId) + " and pr.iddiscip=di.id "
    "ORDER BY di.scan_number, pr.numplansemestr";
  
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  while (mybase::MYFASTROW	row = res.fetch_row())
  {
    int idclass = row["idclass"].toInt();
    if (idclass != 1) // требуются только обычные дисциплины
      continue;

    DiscipData disc;
    disc.title   = row["fulltitle"];
    disc.hours   = row["num_hours"];
    disc.zachet_edinica = row["zachet_edinica"];
    disc.ocenka  = toOcenka(row["estimation"].toInt());
    disc.discId  = row["iddiscip"].toInt();
    disc.semestr = row["numplansemestr"].toInt();

    // проверим что может это слишком старая оценка
    for (size_t i = 0; i < data.size(); ++i)
      if (data[i].discId == disc.discId && data[i].title == disc.title)
      {
        if (disc.semestr >= data[i].semestr)
          data[i] = disc;
        disc.discId = -1; // уже заменили или не надо добавлять
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