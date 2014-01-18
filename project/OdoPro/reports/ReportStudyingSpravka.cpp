#include "StdAfx.h"
#include <vector>
#include "ReportStudyingSpravka.h"
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

  bool renameUniver = privData.inYear.toInt() < 2011 || (privData.inYear.toInt() == 2011 && (privData.inMonth.toInt() < 7 || (privData.inMonth.toInt() == 7 && privData.inDay.toInt() < 8)));
  bool stillStudying = privData.exitNum.empty();

  // поступил
  string_t inInfo = " году в федеральное государственное бюджетное образовательное учреждение высшего профессионального образования «Нижегородский государственный архитектурно-строительный университет» (заочная форма)";
  if (renameUniver)
    inInfo = " году в государственное образовательное учреждение высшего профессионального образования «Нижегородский государственный архитектурно-строительный университет» (заочная форма)";
  if (privData.isMale)
    inInfo = "Поступил в " + privData.inYear + inInfo;
  else
    inInfo = "Поступила в " + privData.inYear + inInfo;

  // выпустился
  string_t outInfo = " году в федеральном государственном бюджетном образовательном учреждении высшего профессионального образования «Нижегородский государственный архитектурно-строительный университет» (заочная форма)";
  if (privData.isMale)
    outInfo = "Завершил обучение в " + privData.outYear + outInfo;
  else
    outInfo = "Завершила обучение в " + privData.outYear + outInfo;

  //  или продолжает обучение
  if (stillStudying)
    if (privData.isMale)
      outInfo = "Завершил обучение в\nПродолжает обучение";
    else
      outInfo = "Завершила обучение в\nПродолжает обучение";

  // дополнительниые сведения
  string_t bottomInfo;
  if (renameUniver)
    bottomInfo = "Вуз переименован в 2011 году;\nстарое полное именование вуза: Государственное образовательное учреждение высшего профессионального образования «Нижегородский государственный архитектурно-строительный университет»";
  if (stillStudying)
    bottomInfo.addAsParagraph("Справка выдана по требованию");
  else
    bottomInfo.addAsParagraph("Приказ об отчислении от " + privData.exitDate + " г. № " + privData.exitNum);


  WordMacros macros;
  macros.BeginMacros();

  // ПЕРВАЯ СТОРОНА

  // ФИО
  macros.Cell(1, 1, 2, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 1, 2, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.SelectionText(privData.secondName + "\n" + privData.firstName + "\n" + privData.thirdName);

  // текущая дата
  macros.Cell(1, 4, 2, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.Cell(1, 4, 2, "VerticalAlignment=wdCellAlignVerticalTop");
  macros.Cell(1, 4, 2, "Range.Text=" + toWrap(CurrentDate()));

  macros.Cell(1, 2, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.Cell(1, 2, 1, "VerticalAlignment=wdCellAlignVerticalTop");

  // дата рождения
  macros.SelectionText("Дата рождения " + privData.bornDate);
  macros.SelectionTypeParagraph(2);
  macros.SelectionText("Документ об уровне образования, на основании которого поступила на обучение:\n");
  macros.SelectionText(privData.prevDoc + " " + privData.prevDocYear + " год");
  macros.SelectionTypeParagraph(2);

  // поступила
  macros.SelectionText(inInfo);
  macros.SelectionTypeParagraph(2);

  // выпустилась
  macros.SelectionText(outInfo);
  macros.SelectionTypeParagraph(2);

  macros.SelectionText("Нормативный срок обучения по очной форме 4 года");
  macros.SelectionTypeParagraph(2);

  // Направление подготовки/специальность:
  macros.SelectionUnderlineText("Направление подготовки", naprData.stroka1 == DirectData::S1_DIRECT);
  macros.SelectionText("/");
  macros.SelectionUnderlineText("специальность", naprData.stroka1 == DirectData::S1_SPEC);
  macros.SelectionText(":");
  macros.SelectionTypeParagraph();

  // значение предыдущей строки
  macros.SelectionText(naprData.stroka1Value);
  macros.SelectionTypeParagraph(2);

  // Специализация/профиль/профильная направленность (программа):
  macros.SelectionUnderlineText("Специализация", naprData.stroka2 == DirectData::S2_SPECIAL);
  macros.SelectionText("/");
  macros.SelectionUnderlineText("профиль", naprData.stroka2 == DirectData::S2_PROFIL);
  macros.SelectionText("/");
  macros.SelectionUnderlineText("профильная направленность (программа)", naprData.stroka2 == DirectData::S2_MAGISTR);
  macros.SelectionText(":");
  macros.SelectionTypeParagraph();

  // значение предыдущей строки
  macros.SelectionText(naprData.stroka2Value);
  macros.SelectionTypeParagraph(2);

  // курсовые
  macros.SelectionText("Курсовые работы (проекты):\n");
  macros.SelectionFont("Size=9");
  macros.SelectionText(studyData.kur);
  macros.SelectionFont("Size=11");
  macros.SelectionTypeParagraph(2);

  // практика
  macros.SelectionText("Практика:\n");
  macros.SelectionFont("Size=9");
  macros.SelectionText(studyData.practic);
  macros.SelectionFont("Size=11");
  macros.SelectionTypeParagraph(2);

  // научные работы
  macros.SelectionText("Научно-исследовательская работа:\n");
  macros.SelectionFont("Size=9");
  macros.SelectionText(studyData.sci);
  macros.SelectionFont("Size=11");
  macros.SelectionTypeParagraph(2);

  // госы
  macros.SelectionText("Государственная итоговая аттестация:\n");
  macros.SelectionFont("Size=9");
  macros.SelectionText(studyData.gos);
  macros.SelectionFont("Size=11");

  // ВТОРАЯ СТОРОНА

  // заполним таблицу дисциплин
  macros.InsertLine("ActiveDocument.Tables.Item(4).Rows.Item(3).Range.Select");
  macros.InsertLine("Selection.InsertRowsBelow " + toStr(discData.size()-2));
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
  macros.SelectionFont("Size=9");
  macros.SelectionText(bottomInfo);
  macros.SelectionFont("Size=11");

  macros.EndMacros();
  macros.RunMacros("spravka.dot");
}
//-------------------------------------------------------------------------
void ReportStudyingSpravka::GetDirectData(DirectData& data, const r::PrivateData& privData)
{
  // Направление подготовки/специальность:
  // если заполнено направление - использовать направление иначе специальности
  data.stroka1 = privData.direct.empty() ? DirectData::S1_SPEC : DirectData::S1_DIRECT;
  if (data.stroka1 == DirectData::S1_DIRECT)
    data.stroka1Value = privData.direct;
  else
    data.stroka1Value = privData.specOrProfil;

  // Специализация/профиль/профильная направленность (программа):
  // если было направление то это профиль, иначе Специализация, а если тег (маг) то профильная направленность
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
    string_t ocenka  = r::toOcenka(row["estimation"].toInt());

    if (idclass == 2 || idclass == 3) // курсовые работы и проекты
      data.kur.addAsParagraph(title + ", " + ocenka);
    else if (idclass == 4) // практика
      data.practic.addAsParagraph(title + " " + local::hours_to_str(hours) + ", " + ocenka);
    else if (idclass == 8) // научно исследовательская работа
      data.sci.addAsParagraph(title);
    else if (idclass == 5) // итоговая аттестация
      data.gos.addAsParagraph(title + ", " + ocenka);
    else if (idclass == 6) // выпускная квалиф. работа
      vkrGos = "Выпускная квалификационная работа на тему «" + vkr_title + "», " + ocenka + "\n";
  }
  data.gos += vkrGos; // так сделал что бы вкр был ниже всех

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
    disc.ocenka  = r::toOcenka(row["estimation"].toInt());
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
    return r::to_str_date(row["date"], true);
  return r::to_str_date("");
}