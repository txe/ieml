#include "StdAfx.h"
#include <vector>
#include "Report_Diplom.h"
#include "../SingeltonApp.h"

//-------------------------------------------------------------------------
void ReportDiplom::Run(int grpId, int studentId)
{
  r::PrivateData privData;
  r::GetPrivateData(privData, studentId);
  DirectData dirData;
  GetDirectData(dirData, privData);
  std::vector<Discip> cursDiscip;
  std::vector<Discip> commonDiscip;
  std::vector<Discip> specDiscip;
  bool useZe = privData.specOrProfilTag == "бак" || privData.specOrProfilTag == "бакус" || privData.specOrProfilTag == "маг";    // так же для расчета объема программы
  bool anotherEnd = privData.specOrProfilTag.empty() || privData.specOrProfilTag == "бак1";
  GetDiscipInfo(studentId, cursDiscip, commonDiscip, specDiscip, privData.lang, privData.vkrTitle, useZe, anotherEnd);

  WordMacros macros;
  macros.BeginMacros();

  // ФИО
  macros.Cell(1, 3, 7, "Range.Select");
  macros.SelectionText(privData.secondName);
  macros.Cell(1, 4, 7, "Range.Select");
  macros.SelectionText(privData.firstName);
  macros.Cell(1, 5, 7, "Range.Select");
  macros.SelectionText(privData.thirdName);

  // дата рождения
  macros.Cell(1, 6, 8, "Range.Select");
  macros.SelectionText(r::to_str_date(privData.bornDate, "года"));

  // аттестат
  macros.Cell(1, 8, 6, "Range.Select");
  macros.SelectionText(privData.prevDoc.toUpperFirst() + " " + privData.prevDocYear + " год");

  // номер регистрации
  macros.Cell(1, 13, 5, "Range.Select");
  macros.SelectionText(privData.regNum);

  // дата выдачи
  macros.Cell(1, 15, 7, "Range.Select");
  macros.SelectionText(r::to_str_date(privData.dataVidachi, "года"));  

  // специалиста (специалиста с отличием, бакалавра, бакалавра с отличием)
  macros.Cell(1, 9, 4, "Range.Select");
  macros.SelectionText(dirData.title1);

  // по специальности, по направлению
  macros.Cell(1, 11, 9, "Range.Select");
  macros.SelectionText(dirData.title2);

  // сколько обучался
  macros.Cell(1, 15, 9, "Range.Select");
  macros.SelectionText(dirData.title3);

  // дополнительная информация
  macros.Cell(1, 12, 3, "Range.Select");
  macros.SelectionText(dirData.bottomInfo);

  // курсовые работы
  for (int i = 0; i < (int)cursDiscip.size(); ++i)
  {
     macros.CellCell(1, 3, 2, i + 1, 1, "Select");
     macros.SelectionText(toQuate(cursDiscip[i].title));
     macros.CellCell(1, 3, 2, i + 1, 2, "Select");
     macros.SelectionText(cursDiscip[i].ocenka);
  }

  // обычные дисциплины на первой таблице
  int rowCount = 56;  // всего строк в первой таблице
  int usedDiscip = 0; // сколько на самом деле добавили дисциплин в первую таблицу
  for (int i = 0; i < (int)commonDiscip.size() && rowCount > 0; ++i)
  {
    rowCount -= PrepareDiscipTitle(commonDiscip[i].title, 63);
    if (rowCount < 0) // если строка не уберется в конце таблицы, то сразу нечего продолжать
      break;

    usedDiscip++;
    macros.CellCell(2, 3, 2, i + 1, 1, "Select");
    macros.SelectionText(toQuate(commonDiscip[i].title));
    macros.CellCell(2, 3, 2, i + 1, 2, "Select");
    macros.SelectionText(commonDiscip[i].period);
    macros.CellCell(2, 3, 2, i + 1, 3, "Select");
    macros.SelectionText(commonDiscip[i].ocenka);
  }
  // обычные дисциплины во второй таблице
  int curRow = 1;
  for (int i = usedDiscip; i < (int)commonDiscip.size(); ++i)
  {
    macros.CellCell(2, 2, 7, curRow, 1, "Select");
    macros.SelectionText(toQuate(commonDiscip[i].title));
    macros.CellCell(2, 2, 7, curRow, 2, "Select");
    macros.SelectionText(commonDiscip[i].period);
    macros.CellCell(2, 2, 7, curRow, 3, "Select");
    macros.SelectionText(commonDiscip[i].ocenka);
    ++curRow;
  }
  // остальные данные
  for (int i = 0; i < (int)specDiscip.size(); ++i)
  {
    macros.CellCell(2, 2, 7, curRow, 1, "Select");
    macros.SelectionText(toQuate(specDiscip[i].title));
    macros.CellCell(2, 2, 7, curRow, 2, "Select");
    macros.SelectionText(specDiscip[i].period);
    macros.CellCell(2, 2, 7, curRow, 3, "Select");
    macros.SelectionText(specDiscip[i].ocenka);
    ++curRow;
  }

  macros.EndMacros();
  macros.RunMacros(theApp.GetModuleDir() + "diplom-2014.dot");
}
//-------------------------------------------------------------------------
void ReportDiplom::GetDirectData(DirectData& dirData, const r::PrivateData& privData)
{
  bool isBachelor = privData.specOrProfilTag != "";
  
  // специалиста, специалиста с отличием, бакалавра, бакалавра с отличием
  if (isBachelor  && privData.isOtlDiplom)  dirData.title1 = L"бакалавра с отличием";
  if (isBachelor  && !privData.isOtlDiplom) dirData.title1 = L"бакалавра";
  if (!isBachelor && privData.isOtlDiplom)  dirData.title1 = L"специалиста с отличием";
  if (!isBachelor && !privData.isOtlDiplom) dirData.title1 = L"специалиста";

  /*
  ВАРИАНТ 1
  юрист (qualific)
  по специальности (если в разделе spec отсутствуют какие-либо теги) 
  030501.65 (shifrspec) Юриспруденция (spec) 
  ВАРИАНТ 2
  бакалавр юриспруденции (qualific)
  по направлению подготовки (если в таблице spec есть тег «бак», «бак1», «маг») 
  030500.62 (shifrspec) Юриспруденция (из таблицы spec – для тега «бак1»; из таблицы direct – для тегов «бак» и «маг»)
  Всё заполняется строчными (маленькими) буквами, кроме первой буквы в наименовании специальности/направления подготовки, на отдельных строках
  */
  string_t shifr = privData.shifrspec;
  if (shifr.empty())
    shifr = "xxxx";
  // уберем часть шифра, если там только одна точка
  int dotPos = shifr.indexOf(L".");
  if (dotPos != -1)
    if (-1 == shifr.subString(dotPos + 1, -1).indexOf(L"."))  
      shifr = shifr.subString(0, dotPos);

  if (!isBachelor)
  {
    dirData.title2 = privData.qualific.toLower();
    dirData.title2 += "\nпо специальности\n";
    dirData.title2 += shifr + " " + privData.specOrProfil.toUpperFirst();
  }
  else
  {
    string_t spec = privData.specOrProfil;
    if (privData.specOrProfilTag.toLower().trim() != "бак1")
      spec = privData.direct;
    dirData.title2 = privData.qualific.toLower();
    dirData.title2 += "\nпо направлению подготовки\n";
    dirData.title2 += shifr + " " + spec.toUpperFirst();
  }

  string_t tag = privData.specOrProfilTag.toLower();
  if (tag.empty())       dirData.title3 = "5 лет";
  else if (tag == "бак1" 
    || tag == "бак" 
    || tag == "бакус")   dirData.title3 = "4 года";
  else if (tag == "маг") dirData.title3 = "2 года";
  else                   dirData.title3 = "xxxx лет";

  // дополнительная информация
  bool renameUniver = privData.inYear.toInt() < 2011 || (privData.inYear.toInt() == 2011 && (privData.inMonth.toInt() < 7 || (privData.inMonth.toInt() == 7 && privData.inDay.toInt() < 8)));
  if (renameUniver)
  {
    dirData.bottomInfo =  L"Образовательная организация переименована в 2011 году.";
    dirData.bottomInfo += L"\nСтарое полное официальное наименование образовательной организации – Государственное образовательное учреждение высшего профессионального образования «Нижегородский государственный архитектурно-строительный университет».";
    dirData.bottomInfo += L"\n";
  }
  dirData.bottomInfo += L"Форма обучения: заочная.";
  dirData.bottomInfo += L"\nЧасть образовательной программы в объеме ? недель освоена в ?.";

  if (privData.direct.empty())
    dirData.bottomInfo += L"\nСпециализация: " + privData.specializ + L".";
  else if (tag == "бак" || tag == "бакус")
    dirData.bottomInfo += L"\nНаправленность (профиль) образовательной программы: " + privData.specOrProfil + L".";
}
//-------------------------------------------------------------------------
void ReportDiplom::GetDiscipInfo(int studentId, std::vector<Discip>& cursDiscip, std::vector<Discip>& commonDiscip, std::vector<Discip>& specDiscip, string_t lang, string_t vkrTitle, bool useZe, bool anotherEnd)
{
  struct fun
  {
    static string_t ze_hours(bool useZe, string_t val) { return useZe ? (val + "  з.е.") : (val + " час."); }
    static string_t ze_weeks(bool useZe, string_t val) { return useZe ? (val + "  з.е.") : r::weeks_to_str(val); }
  };

  std::vector<Practice> practice; // практики
  std::vector<Discip> itog;       // гос. аттестации
  Discip              vkrWork("", "", "");  // ВКР
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
      cursDiscip.push_back(Discip(title, "", ocenka));
    if (idclass == r::DT_COMMON)
    {
      if (title.toUpper().trim() == string_t(L"ИНОСТРАННЫЙ ЯЗЫК"))
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
        find->ocenka += (5 - row["estimation"].toInt()); // обратная оценка, что бы округление работало правильно
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
      itog.push_back(Discip(title, "х", ocenka));
    }
    if (idclass == r::DT_KVALIFIC_WORK)
    {
      zeTimeCounter += times.toInt();
      itogTime = times;
      vkrWork = Discip("выпускная квалификационная работа – дипломная работа на тему «" + vkrTitle + "»", "х", ocenka);
    }
  }

  // сформируем specDiscip
  // практики
  specDiscip.push_back(Discip("Практики", fun::ze_weeks(useZe, toStr(practicTime)), "x"));
  specDiscip.push_back(Discip("в том числе:", "", ""));
  for (size_t i = 0; i < practice.size(); ++i)
  {
    int ocenka = (int)(double(practice[i].ocenka) / double(practice[i].practice_amount) + 0.5);
    ocenka = 5 - ocenka; // оценка была на оборот для правильного округления в лучшую сторону
    specDiscip.push_back(Discip(practice[i].title, practice[i].period, r::toOcenka(ocenka)));
  }

  // гос. аттестация
  specDiscip.push_back(Discip("Государственная итоговая аттестация", fun::ze_weeks(useZe, itogTime), "x"));
  specDiscip.push_back(Discip("в том числе:", "", ""));
  for (size_t i = 0; i < itog.size(); ++i)
    specDiscip.push_back(itog[i]);
  if (!vkrWork.title.empty())
    specDiscip.push_back(vkrWork);

  if (useZe)
  {
    string_t ze = fun::ze_weeks(true, toStr(zeTimeCounter));
    specDiscip.push_back(Discip("Объем образовательной программы", ze, "x"));
    specDiscip.push_back(Discip("в том числе объем работы обучающихся во взаимодействии с преподавателем:", "800 час.", "x"));
  }
  else if (anotherEnd)
  {
    specDiscip.push_back(Discip("Срок освоения образовательной программы", "260 недель", "x"));
    specDiscip.push_back(Discip("в том числе аудиторных часов:", "800 час.", "x"));
  }
  else
  {
    specDiscip.push_back(Discip("Общая трудоемкость образовательной программы", "260 недель", "x"));
    specDiscip.push_back(Discip("в том числе объем работы обучающихся во взаимодействии с преподавателем:", "800 час.", "x"));
  }

}
//-------------------------------------------------------------------------
// разобьет на строки и вернет сколько строк займет
int ReportDiplom::PrepareDiscipTitle(string_t& title, int symbolMax)
{
  if ((int)title.size() <= symbolMax)
    return 1;
  std::vector<string_t> lines;
  while ((int)title.size() > symbolMax)
  {
    for (int i = symbolMax - 1; i >= 0; --i)
      if (title[i] == L' ' || title[i] == L'(')
      {
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
}
//-------------------------------------------------------------------------
bool ReportDiplom::AddDiscip(std::vector<Discip>& disList, const Discip& discip)
{
  // проверим что может это слишком старая оценка
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
