#include "StdAfx.h"
#include <vector>
#include "Report_Dogovor2014.h"
#include "../SingeltonApp.h"

//-------------------------------------------------------------------------
void ReportDogovor::Run(int grpId, int studentId)
{
  WordMacros macros;
  macros.BeginMacros();

  ReportDogovorData data = GetData(grpId, studentId);

  macros.Replace("$DOG_NUM$",   data.dogovorNum);
  macros.Replace("$CUR_TIME$",  data.currentData);
  macros.Replace("$FIO$",       data.fio);
  macros.Replace("$KOD$",       data.kod);
  macros.Replace("$FORMA$",     data.forma);
  macros.Replace("$KVALIF$",    data.kvalif);
  macros.Replace("$SUM_NUM$",   data.numSum);
  macros.Replace("$SUM_STR$",   data.strSum);

  for (int i = 0; i < 6; ++i)
    macros.Replace("$OPLATA" + string_t(aux::itow(i+1)) + "$", data.oplata[i]);

  macros.Replace("$EXTRA$",    data.extra);
  macros.Replace("$PASSPORT$", data.passport);
  macros.Replace("$ADRES1$",   data.adres1);
  macros.Replace("$ADRES2$",   data.adres2);
  macros.Replace("$TEL$",      data.telefon);

  macros.EndMacros();
  macros.RunMacros(theApp.GetModuleDir() + "dogovor-2014.dot");
}
//---------------------------------------------------------------------------
ReportDogovor::ReportDogovorData ReportDogovor::GetData(int grpId, int studentId)
{
  r::PrivateData privData;
  r::GetPrivateData(privData, studentId);

  ReportDogovorData data;
  data.fio         = privData.secondName + " " + privData.firstName + " " + privData.thirdName;
  data.currentData = r::GetCurrentDate("г.");
  data.kvalif      = privData.qualific.toLower();

  // код и наименование программы
  if (privData.direct.empty())
    data.kod = privData.shifrspec + " " + privData.specOrProfil;
  else
    data.kod = "по направлению подготовки " + privData.shifrspec + " " + privData.direct + " (аккредитованна€ образовательна€ программа) c профилем " + privData.specOrProfil;

  string_t dogovorQuery = string_t() +
    "SELECT s.dogextra, s.dogyearid,s.dogshifrid,s.dogfastid,s.dognum,s.eduformid,s.passseries,s.passnum,s.passkod,s.passdate,s.passplace,s.addr,s.liveaddr,s.phones " \
    " FROM students as s WHERE s.deleted=0 and s.id=" + aux::itow(studentId);
  mybase::MYFASTRESULT dogovorRes = theApp.GetCon().Query(dogovorQuery);
  if (mybase::MYFASTROW	row = dogovorRes.fetch_row())
  {
    // форма обучени€
    data.kod += ", " + theApp.GetTitleForKeyFromVoc(vok_key::VK_EDUFORM, row["eduformid"].toInt(), true) + ".";

    // особые сведени€
    data.extra = theApp.GetTitleForKeyFromVoc(vok_key::VK_DOG_EXTRA, row["dogextra"].toInt(), true);

    // номер договора
    string_t dogYear  = theApp.GetTitleForKeyFromVoc(vok_key::VK_DOG_YEAR,  row["dogyearid"].toInt(), true);
    string_t dogShifr = theApp.GetTitleForKeyFromVoc(vok_key::VK_DOG_SHIFR, row["dogshifrid"].toInt(), true);
    string_t dogFast  = theApp.GetTitleForKeyFromVoc(vok_key::VK_DOG_FAST,  row["dogfastid"].toInt(), true);
    data.dogovorNum = dogShifr + "-" + dogYear + dogFast + "-" + row["dognum"];

    //
    data.passport = "ѕаспорт сери€ " + row["passseries"] + " є" + row["passnum"] + " код подразделени€ " + row["passkod"] + " выдан " 
      + r::to_str_date(row["passdate"], "года ") + row["passplace"];
    data.adres1 = "јдрес по месту регистрации: " + row["addr"];
    data.adres2 = "јдрес по месту фактического проживани€: " + row["liveaddr"];
    data.telefon = "“ел. " + row["phones"];
  }

  // разберемс€ с деньгами
  std::map<int, int> moneyYear; // год -> деньги
  string_t moneyQuery = string_t() + 
    "SELECT opts.id, opts.datestart, opts.commoncountmoney, opts.half_year FROM payoptstest as opts WHERE opts.deleted = 0 AND opts.idgroup = "  + aux::itow(grpId);
  mybase::MYFASTRESULT moneyRes = theApp.GetCon().Query(moneyQuery);
  while (mybase::MYFASTROW row = moneyRes.fetch_row())
    moneyYear[r::GetYear(row["datestart"]).toInt()] = row["commoncountmoney"].toInt();
  
  int firstYear = -1;
  for (std::map<int, int>::iterator it = moneyYear.begin(); it != moneyYear.end(); ++it)
    if (firstYear == -1 || it->first < firstYear)
      firstYear = it->first;
  if (firstYear == -1)
    firstYear = 2014;
  
  int allMoney = 0;
  for (int yearNum = 0; yearNum < 6; ++yearNum) 
    if (int money = moneyYear[yearNum + firstYear])
    {
      allMoney += money;
      data.oplata[yearNum] = aux::itow(money);
    }
    else
      data.oplata[yearNum] = "-";

  data.numSum = aux::itow(allMoney);
  data.strSum = MoneyToStr(allMoney);

  return data;
}
//---------------------------------------------------------------------------
string_t ReportDogovor::MoneyToStr(int money)
{
  static const string_t hausends[20]={"одна","две","три","четыре","п€ть","шесть","семь","восемь","дев€ть","дес€ть","одиннадцать",
                                      "двенадцать","тринадцать","четырнадцать","п€тнадцать","шестнадцать","семнадцать",
                                      "восемнадцать","дев€тнадцать","двадцать"};
  static const string_t hausendsdec[10] = {"дес€ть","двадцать","тридцать","сорок","п€тьдес€т","шестьдес€т","семьдес€т","восемьдес€т","дев€носто","сто"};
  static const string_t handred[10]     = {"сто","двести","триста","четыреста","п€тьсот","шестьсот","семьсот","восемьсот","дев€тьсот","одна тыс€ча"};
  static const string_t eds[20]         = {"один","два","три","четыре","п€ть","шесть","семь","восемь","дев€ть","дес€ть","одиннадцать",
                                           "двенадцать","тринадцать","четырнадцать","п€тнадцать","шестнадцать","семнадцать",
                                           "восемнадцать","дев€тнадцать","двадцать"};

  string_t moneyStr = aux::itow(money);
  string_t result = "";

  int dig,dec,ed,hand;
  if (moneyStr.size() <= 6)
  {
    if (moneyStr.size() == 6)
    {
      hand = moneyStr.subString(0,1).toInt();
      if (hand > 0)
        result = handred[hand-1] + " ";
      moneyStr = moneyStr.subString(1, -1);
    }
    if (moneyStr.size() == 5)
    {
      dig = moneyStr.subString(0,2).toInt();
      if (dig < 20 && dig > 0)
      {
        result += hausends[dig-1];
        if (dig == 1)              result += " тыс€ча";
        if (dig >= 2 && dig <= 4)  result += " тыс€чи";
        if (dig >= 5 && dig <= 20) result += " тыс€ч";
      }
      if (dig >= 20)
      {
        dec = dig/10;
        ed  = dig%10;
        if (dec > 0)
          result += hausendsdec[dec-1];
        if (ed > 0)
        {
          result += " " + hausends[ed-1];
          if (ed == 1)            result += " тыс€ча";
          if (ed >= 2 && ed <= 4) result += " тыс€чи";
          if (ed >= 5 && ed <= 9) result += " тыс€ч";
        }
        else result += " тыс€ч";
      }

      moneyStr = moneyStr.subString(2, -1);
    }
    if (moneyStr.size() == 4)
    {
      dig = moneyStr.subString(0,1).toInt();
      if (dig < 20 && dig > 0)
      {
        result += hausends[dig-1];
        if (dig == 1)             result += " тыс€ча";
        if (dig >= 2 && dig <= 4) result += " тыс€чи";
        if (dig >= 5 && dig <= 9) result += " тыс€ч";
      }

      moneyStr = moneyStr.subString(1, -1);
    }
    if (moneyStr.size() == 3)
    {
      hand = moneyStr.subString(0,1).toInt();
      if (hand > 0)
        result += " " + handred[hand-1];

      dig = moneyStr.subString(1,2).toInt();
      if (dig < 20 && dig > 0) 
        result += " " + eds[dig-1];
      if (dig >= 20)
      {
        dec = dig/10;
        ed  = dig%10;
        if (dec > 0) result += " " + hausendsdec[dec-1];
        if (ed  > 0) result += " " + eds[ed-1];
      }
    }
  }
  if (result.size() > 0)
    result = result.toUpperFirst();
 
  return result;
}