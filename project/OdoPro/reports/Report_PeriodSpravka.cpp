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
  string_t maleEnd = privData.isMale ? "" : "а";

  // поступил
  string_t inInfo = "Поступил" + maleEnd + " в " + privData.inYear;    
  if (renameUniver2011)
    inInfo += " году в государственное образовательное учреждение высшего профессионального образования «Нижегородский государственный архитектурно-строительный университет» (заочная форма)";
  else
    inInfo += " году в федеральное государственное бюджетное образовательное учреждение высшего профессионального образования «Нижегородский государственный архитектурно-строительный университет» (заочная форма)";

  // выпустился или продолжает обучение
  string_t outInfo = "Прекратил" + maleEnd + " обучение в " + privData.outYear + " году в федеральном государственном бюджетном образовательном учреждении высшего образования «Нижегородский государственный архитектурно-строительный университет» (заочная форма)";
  if (stillStudying)
    outInfo = "Прекратил" + maleEnd + " обучение в\nПродолжает обучение";

  // дополнительниые сведения
  string_t bottomInfo;
  if (renameUniver2011)
  {
    bottomInfo += "Наименование образовательной организации изменилось в 2011 году.\nПрежнее наименование образовательной организации – государственное образовательное учреждение высшего профессионального образования «Нижегородский государственный архитектурно-строительный университет».";
    bottomInfo += "\n";
  }
  bottomInfo += "Наименование образовательной организации изменилось в 2016 году.\nПрежнее наименование образовательной организации – федеральное государственное бюджетное образовательное учреждение высшего профессионального образования «Нижегородский государственный архитектурно-строительный университет».";


  bottomInfo.addAsParagraph("Приказ об отчислении от " + privData.exitDateFull + " года № " + privData.exitNum);


  WordMacros macros;
  macros.BeginMacros();

  // insert page number
  macros.InsertLine("Selection.Sections(1).Footers(1).PageNumbers.Add PageNumberAlignment:= _");
  macros.InsertLine("      wdAlignPageNumberLeft, FirstPage:=True");
  
  // insert text before page number
  macros.InsertLine("ActiveWindow.ActivePane.View.SeekView = wdSeekCurrentPageFooter");
  macros.InsertLine("Selection.MoveLeft Unit:=wdCharacter, Count:=2");
  macros.InsertLine("Application.Keyboard (1049)");
  macros.InsertLine("Selection.TypeText Text:=\"Страница \"");
  macros.InsertLine("ActiveWindow.ActivePane.View.SeekView = wdSeekMainDocument");

  
  // ПЕРВАЯ СТОРОНА
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
  macros.SelectionText("в федеральном государственном бюджетном образовательном учреждении высшего образования «Нижегородский государственный архитектурно-строительный университет» (Минобрнауки России), город Нижний Новгород");
  macros.SelectionFont("Size=6");
  macros.SelectionText("\n\n");
  macros.SelectionFont("Size=11");
  
  // ФИО
  macros.SelectionFont("Size=12");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
  macros.TablesAdd(1, 2);
  macros.TableStyle(WordMacros::StyleNone);
  macros.TablesColumns(1, 1, "PreferredWidth = CentimetersToPoints(2.5)");
  macros.TablesColumns(1, 1, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  macros.SelectionText("Фамилия\nИмя\nОтчество");
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

  // дата рождения
  macros.SelectionFont("Size=12");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
  macros.SelectionText("Дата рождения " + r::to_str_date(privData.bornDate, "года"));
  macros.SelectionTypeParagraph(1);
  macros.SelectionFont("Size=11");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  macros.SelectionText("Уровень образования и документ об уровне образования, на основании которого поступил" + maleEnd + " на обучение:\n");
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
  macros.SelectionText(privData.prevDoc + " " + privData.prevDocYear + " год");
  macros.SelectionTypeParagraph(1);
  
  // поступила
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  macros.SelectionText(inInfo);
  macros.SelectionTypeParagraph(2);

  // Направление подготовки/специальность:
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  if (privData.isMagister)
    macros.SelectionText("Обучается по программе магистратуры (по очной форме) по направлению подготовки:");
  else
    macros.SelectionText("Обучается по программе бакалавриата (по очной форме) по направлению подготовки:");
  macros.SelectionTypeParagraph();

  // значение предыдущей строки
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
  macros.SelectionText(naprData.stroka1Value);
  macros.SelectionTypeParagraph(1);

  // Специализация/профиль/профильная направленность (программа):
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpaceSingle");
  macros.SelectionText("Направленность (профиль) образовательной программы:");
  macros.SelectionTypeParagraph();

  // значение предыдущей строки
  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
  macros.SelectionText(naprData.stroka2Value);
  macros.SelectionTypeParagraph(1);

  if (privData.isMagister)
      macros.SelectionText("Срок освоения программы магистратуры в очной форме обучения 2 года");
  else
      macros.SelectionText("Срок освоения программы бакалавриата в очной форме обучения 4 года");
  macros.SelectionTypeParagraph(1);

  
  int nextTableNum = AddProgramTable(macros, discData);
  int tableNum = AddProjectTable(macros, curseWork, nextTableNum);

  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");
  macros.SelectionFont("Size=11");
  
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.SelectionText("ДОПОЛНИТЕЛЬНЫЕ СВЕДЕНИЯ\n");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.SelectionText("Продолжает обучение.\n");

  macros.TablesAdd(3, 2);
  macros.TableStyle(WordMacros::StyleNone);
  macros.TablesColumns(tableNum, 1, "PreferredWidth = InchesToPoints(6.22)");
  macros.TablesColumns(tableNum, 2, "PreferredWidth = InchesToPoints(1.46)");
  macros.Cell(tableNum, 1, 1, "Range.Text=" + toWrap("Ректор"));
  macros.Cell(tableNum, 2, 1, "Range.Text=" + toWrap("Директор"));
  macros.Cell(tableNum, 3, 1, "Range.Text=" + toWrap("Секретарь"));
  macros.Cell(tableNum, 1, 2, "Range.Text=" + toWrap("А.А. Лапшин"));
  macros.Cell(tableNum, 2, 2, "Range.Text=" + toWrap("Д.В. Хавин"));
  macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=100");
 
  macros.InsertLine("Selection.TypeText Text:=vbTab & vbTab & \"М.П.\"");
  macros.SelectionText("\n");

  tableNum += 1;
  macros.TablesAdd(1, 2);
  macros.TableStyle(WordMacros::StyleNone);
  macros.TablesColumns(tableNum, 1, "PreferredWidth = InchesToPoints(4)");
  macros.TablesColumns(tableNum, 2, "PreferredWidth = InchesToPoints(4)");
  macros.TablesColumns(tableNum, 2, "Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphRight");
  macros.Cell(tableNum, 1, 1, "Range.Text=" + toWrap("Регистрационный номер 00000"));
  macros.Cell(tableNum, 1, 2, "Range.Text=" + toWrap("Дата выдачи " + r::GetCurrentDate("года")));
  macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=100");

  macros.SelectionText("\nНастоящий документ содержит 2 страницы");

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
      macros.SelectionText("НАИМЕНОВАНИЕ ДИСЦИПЛИН (МОДУЛЕЙ) ПРОГРАММЫ,\nВИД И ТИП ПРАКТИКИ");
      macros.TablesColumns(tableNum, 2, "Select");
      macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");    
      macros.SelectionText("КОЛИЧЕСТВО\nЗАЧЁТНЫХ\nЕДИНИЦ");
      macros.TablesColumns(tableNum, 3, "Select");
      macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
      macros.SelectionText("КОЛИЧЕСТВО\nАКАДЕМИЧЕСКИХ\nЧАСОВ");
      macros.TablesColumns(tableNum, 4, "Select");
      macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
      macros.SelectionText("ОЦЕНКА");

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
  macros.SelectionText("СВЕДЕНИЯ О СОДЕРЖАНИИ И РЕЗУЛЬТАТАХ ОСВОЕНИЯ ОБРАЗОВАТЕЛЬНОЙ ПРОГРАММЫ");
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
    macros.Cell(tableNum, rowCount + 1, 1, "Range.Text=" + toWrap("Окончание таблицы приведено на обороте"));
  macros.InsertLine("Selection.MoveDown Unit:=wdLine, Count:=100");

  macros.SelectionFont("Size=11");
  macros.InsertLine("Selection.InsertBreak Type:=wdPageBreak");

  if (!needSecondTable)
    return tableNum + 1;

  macros.SelectionFont("Size=10");
  macros.SelectionText("ОКОНЧАНИЕ СВЕДЕНИЙ О СОДЕРЖАНИИ И РЕЗУЛЬТАТАХ ОСВОЕНИЯ ОБРАЗОВАТЕЛЬНОЙ ПРОГРАММЫ");
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
  macros.SelectionText("КУРСОВЫЕ РАБОТЫ (ПРОЕКТЫ)");
  macros.TablesColumns(tableNum, 2, "Select");
  macros.SelectionText("ОЦЕНКА");

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
  // Направление подготовки/специальность:
  // если заполнено направление - использовать направление иначе специальности
  data.stroka1 = privData.direct.empty() ? DirectData::S1_SPEC : DirectData::S1_DIRECT;
  if (data.stroka1 == DirectData::S1_DIRECT)
    data.stroka1Value = privData.direct;
  else
    data.stroka1Value = privData.specOrProfil;
  data.stroka1Value = privData.shifrspec + " " + data.stroka1Value;

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
void ReportPeriodSpravka::GetDiscipData(std::vector<DiscipData>& data, std::vector<DiscipData>& curseWork, int studentId, string_t vkr_title, string_t lang)
{
  data.clear();
  curseWork.clear();
  std::vector<DiscipData> practice;
  std::vector<DiscipData> itog;
  DiscipData wkrWork;

  // соберем сперва дисциплины
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
      disc.title = disc.title.toLower() + " «" + vkr_title + "»";
      wkrWork = disc;
    }
    else if (idclass == r::DT_COMMON)
    {
      if (disc.title.toUpper().trim() == string_t(L"ИНОСТРАННЫЙ ЯЗЫК"))
        disc.title += " (" + lang + ")";
      if (disc.title.toUpper().indexOf(L"ФИЗИЧЕСКАЯ") != -1 && disc.title.toUpper().indexOf(L"КУЛЬТУРА") != -1)
      {
        disc.title = "Физическая культура и спорт";
        disc.zachet_edinica = "-";
      }
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
  if (practice.size())
  {
    data.push_back(DiscipData("Практики"));
    data.push_back(DiscipData("в том числе:"));
    for (size_t i = 0; i < practice.size(); ++i)
      data.push_back(practice[i]);
  }
  if (itog.size() || !wkrWork.title.empty())
  {
    data.push_back(DiscipData("Государственная итоговая аттестация"));
    data.push_back(DiscipData("в том числе:"));
    for (size_t i = 0; i < itog.size(); ++i)
      data.push_back(itog[i]);
    if (!wkrWork.title.empty())
      data.push_back(wkrWork);
  }
}
