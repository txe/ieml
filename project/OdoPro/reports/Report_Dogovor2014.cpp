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
  {
    macros.Replace("$OPLATA" + string_t(aux::itow(i+1)) + "$", data.oplata[i]);
    macros.Replace("$YEAR" + string_t(aux::itow(i+1)) + "$", data.oplataYears[i]);
  }

  macros.Replace("$DETAILS$",  data.details);
  macros.Replace("$EXTRA$",    data.extra);
  macros.Replace("$PASSPORT$", data.passport);
  macros.Replace("$ADRES1$",   data.adres1);
  macros.Replace("$ADRES2$",   data.adres2);
  macros.Replace("$TEL$",      data.telefon);

  macros.Replace("$ZAKAZ_FIO$",     data.zakaz_fio);
  macros.Replace("$ZAKAZ_BY$",      data.zakaz_by);
  macros.Replace("$ZAKAZ_DOC$",     data.zakaz_doc);
  macros.Replace("$ZAKAZ_DETAILS$", data.zakaz_details);

  macros.EndMacros();
  if (m_IsDopSoglashenie)
    macros.RunMacros(theApp.GetModuleDir() + "dogovor(dop.sog.)-2014.dot");
  else
    macros.RunMacros(theApp.GetModuleDir() + "dogovor-2014.dot");

}
//---------------------------------------------------------------------------
ReportDogovor::ReportDogovorData ReportDogovor::GetData(int grpId, int studentId)
{
  r::PrivateData privData;
  r::GetPrivateData(privData, studentId);

  ReportDogovorData data;
  data.fio         = privData.secondName + " " + privData.firstName + " " + privData.thirdName;
  data.currentData = r::GetCurrentDate("�.");
  data.kvalif      = privData.qualific.toLower();

  // ��� � ������������ ���������
  if (privData.direct.empty())
    data.kod = privData.shifrspec + " " + privData.specOrProfil;
  else
    data.kod = "�� ����������� ���������� " + privData.shifrspec + " " + privData.direct + " (��������������� ��������������� ���������) c �������� " + privData.specOrProfil;

  string_t dogovorQuery = string_t() +
    "SELECT s.dogextra, s.dogyearid,s.dogshifrid,s.dogfastid,s.dognum,s.eduformid,s.passseries,s.passnum,s.passkod,s.passdate,s.passplace,s.addr,s.liveaddr,s.phones, " \
    " s.zakaz_fio, s.zakaz_by, s.zakaz_doc, s.zakaz_details "\
    " FROM students as s WHERE s.deleted=0 and s.id=" + aux::itow(studentId);
  mybase::MYFASTRESULT dogovorRes = theApp.GetCon().Query(dogovorQuery);
  if (mybase::MYFASTROW	row = dogovorRes.fetch_row())
  {
    // ����� ��������
    data.kod += ", " + theApp.GetTitleForKeyFromVoc(vok_key::VK_EDUFORM, row["eduformid"].toInt(), true) + ".";

    // ������ ��������
    data.extra = theApp.GetTitleForKeyFromVoc(vok_key::VK_DOG_EXTRA, row["dogextra"].toInt(), true);

    // ����� ��������
    string_t dogYear  = theApp.GetTitleForKeyFromVoc(vok_key::VK_DOG_YEAR,  row["dogyearid"].toInt(), true);
    string_t dogShifr = theApp.GetTitleForKeyFromVoc(vok_key::VK_DOG_SHIFR, row["dogshifrid"].toInt(), true);
    string_t dogFast  = theApp.GetTitleForKeyFromVoc(vok_key::VK_DOG_FAST,  row["dogfastid"].toInt(), true);
    data.dogovorNum = dogShifr + "-" + dogYear + dogFast + "-" + row["dognum"];

    //
    data.passport = "������� ����� " + row["passseries"] + " �" + row["passnum"] + " ��� ������������� " + row["passkod"] + " ����� " 
      + r::to_str_date(row["passdate"], "���� ") + row["passplace"];
    data.adres1 = "����� �� ����� �����������: " + row["addr"];
    data.adres2 = "����� �� ����� ������������ ����������: " + row["liveaddr"];
    data.telefon = "���. " + row["phones"];

    data.details = data.fio + "\n" + data.passport + "\n" + data.adres1 + "\n" + data.adres2 + "\n" + data.telefon;

    data.zakaz_fio = row["zakaz_fio"];
    data.zakaz_by = row["zakaz_by"];
    data.zakaz_doc = row["zakaz_doc"];
    data.zakaz_details = data.zakaz_fio + "\n" + row["zakaz_details"];

    if (data.zakaz_fio.empty())
    {
      data.zakaz_fio = data.fio;
      data.zakaz_details = data.details;
    }
  }

  // ������ ������ ������������ ������
  std::map<int, int> personalCat;
  string_t personalMoneyQuery = string_t() + 
    "SELECT idopts, commoncountmoney FROM paypersonaltest WHERE idstud = " + aux::itow(studentId) + " AND deleted = 0";
  mybase::MYFASTRESULT persMoneyRes = theApp.GetCon().Query(personalMoneyQuery);
  while (mybase::MYFASTROW row = persMoneyRes.fetch_row())
    personalCat[row["idopts"].toInt()] = row["commoncountmoney"].toInt();

  // ���������� � ��������
  std::map<int, int> moneyYear; // ��� -> ������
  bool isFeb = false;           // �������
  string_t moneyQuery = string_t() + 
    "SELECT opts.id, opts.datestart, opts.commoncountmoney, opts.half_year FROM payoptstest as opts WHERE opts.deleted = 0 AND opts.idgroup = "  + aux::itow(grpId);
  mybase::MYFASTRESULT moneyRes = theApp.GetCon().Query(moneyQuery);
  while (mybase::MYFASTROW row = moneyRes.fetch_row())
  { 
    int money = row["commoncountmoney"].toInt();
    std::map<int,int>::iterator it = personalCat.find(row["id"].toInt());
    if (it != personalCat.end())
      money = it->second;
    if (money == 0)
      continue;

    moneyYear[r::GetYear(row["datestart"]).toInt()] = money;
    isFeb = r::GetMonth(row["datestart"]).toInt() == 2;
  }

  // ������ ��������� ��� 
  int firstYear = -1;
  for (std::map<int, int>::iterator it = moneyYear.begin(); it != moneyYear.end(); ++it)
    if (firstYear == -1 || it->first < firstYear)
      firstYear = it->first;
  if (firstYear == -1)
    firstYear = 2014;
  
  // �������� ����� � ��� ����
  for (int yearNum = 0; yearNum < 6; ++yearNum)
    if (isFeb) // ���� ������ 2015 ���, �� ��� ����� ��� 2014-2015
      data.oplataYears[yearNum] = string_t(aux::itow(yearNum + (firstYear - 1))) + L"-" + aux::itow(yearNum + (firstYear - 1) + 1);
    else
      data.oplataYears[yearNum] = string_t(aux::itow(yearNum + firstYear)) + L"-" + aux::itow(yearNum + firstYear + 1);  
   
  // �������� ����� � ��� ������ ��� ���
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
  static const string_t hausends[20]={"����","���","���","������","����","�����","����","������","������","������","�����������",
                                      "����������","����������","������������","����������","�����������","����������",
                                      "������������","������������","��������"};
  static const string_t hausendsdec[10] = {"������","��������","��������","�����","���������","����������","���������","�����������","���������","���"};
  static const string_t handred[10]     = {"���","������","������","���������","�������","��������","�������","���������","���������","���� ������"};
  static const string_t eds[20]         = {"����","���","���","������","����","�����","����","������","������","������","�����������",
                                           "����������","����������","������������","����������","�����������","����������",
                                           "������������","������������","��������"};

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
        if (dig == 1)              result += " ������";
        if (dig >= 2 && dig <= 4)  result += " ������";
        if (dig >= 5 && dig <= 20) result += " �����";
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
          if (ed == 1)            result += " ������";
          if (ed >= 2 && ed <= 4) result += " ������";
          if (ed >= 5 && ed <= 9) result += " �����";
        }
        else result += " �����";
      }

      moneyStr = moneyStr.subString(2, -1);
    }
    if (moneyStr.size() == 4)
    {
      dig = moneyStr.subString(0,1).toInt();
      if (dig < 20 && dig > 0)
      {
        result += hausends[dig-1];
        if (dig == 1)             result += " ������";
        if (dig >= 2 && dig <= 4) result += " ������";
        if (dig >= 5 && dig <= 9) result += " �����";
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