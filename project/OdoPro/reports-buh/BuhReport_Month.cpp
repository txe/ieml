#include "StdAfx.h"
#include "BuhReport_Month.h"
#include "..\SingeltonApp.h"
#include "..\json-aux-ext.h"
#include "..\registry.h"
#include <sys/timeb.h>
#include <sstream>
#include <iomanip>
#include <map>
#include <ctime>

//-------------------------------------------------------------------------
BuhReport_Month::BuhReport_Month(LiteWnd* parent):LiteWnd(parent)
{
}
//-------------------------------------------------------------------------
BOOL BuhReport_Month::PreCreateWindow(CREATESTRUCT& cs)
{
  if (!LiteWnd::PreCreateWindow(cs))
    return FALSE;

  cs.lpszName = (LPCWSTR)"Оплата за месяц по дням";
  return TRUE;
}
//-------------------------------------------------------------------------
int BuhReport_Month::OnCreate()
{
  SetWindowPos(m_hWnd, NULL, -1, -1, 300, 230, 
    SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
  CenterWindow();

  PBYTE pb; DWORD cb;
  if (GetHtmlResource(L"IDR_HTML_BUH_REPORT_MONTH", pb, cb))
  {
    assert(::IsWindow(m_hWnd));
    ::HTMLayoutLoadHtml(m_hWnd, pb, cb);
  }
  else
    assert(FALSE);

  InitDomElement();
  return 0;
}
//-------------------------------------------------------------------------
void BuhReport_Month::InitDomElement()
{
  orderNumBox_ = link_element("order-num");
  dateBox_     = link_element("xday");

  SerializeData(false);

  // присоединяем обоработчики к кнопкам
  HTMLayoutAttachEventHandlerEx(link_element("bt-report"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
  HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}
//-------------------------------------------------------------------------
// обрабатывает кнопки
BOOL CALLBACK BuhReport_Month::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
  BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
  if (pr->cmd != BUTTON_CLICK)
    return FALSE;

  BuhReport_Month* dlg = static_cast<BuhReport_Month*>(tag);
  std::wstring     id	 = htmlayout::dom::element(he).get_attribute("id");

  if (id == L"bt-close")
  {
    dlg->SerializeData(true);
    dlg->Close();
    return TRUE;
  }
  if (id == L"bt-report")
  {
    dlg->SerializeData(true);
    dlg->Report();
    return TRUE;
  }

  return FALSE;
}
//-------------------------------------------------------------------------
void BuhReport_Month::Report(void)
{
  string_t xDate = json::v2t(dateBox_.get_value());
  if (xDate.empty())
  {
    MessageBox(m_hWnd, L"Не введена дата", L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
    return;
  }

  if (link_element("month-radio").get_state(STATE_CHECKED))
    ReportMonth(xDate.subString(0, xDate.size()-2));
  if (link_element("day-radio").get_state(STATE_CHECKED))
    ReportDay(xDate);
  if (link_element("fio-radio").get_state(STATE_CHECKED))
    ReportFio(xDate, json::v2t(dateBox_.get_value()));
}
//-------------------------------------------------------------------------
void BuhReport_Month::ReportMonth(string_t month)
{
  // получим месячные данные
  string_t startMonth = month + "01";
  string_t endMonth   = month + "31";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(
    " SELECT pay.datepay, SUM(pay.moneypay) as summa "
    " FROM payfactstest as pay "
    " WHERE pay.deleted = 0 AND pay.datepay >= '" + startMonth + "' AND pay.datepay <= '" + endMonth + "'"
    " GROUP BY pay.datepay ORDER BY datepay");

  struct month_pay
  {
    string_t day;
    string_t pay;
    month_pay(string_t d, string_t p) : day(d),pay(p) {}
  };
  std::vector<month_pay> lst;
  while (mybase::MYFASTROW row = res.fetch_row())
    lst.push_back(month_pay(row["datepay"], row["summa"]));

  WordMacros macros;
  macros.BeginMacros();
  ReportHeader(macros, "Оплата за месяц по дням", toRightDate(startMonth) + " - " + toRightDate(endMonth));

  macros.TablesAdd(lst.size()+2, 2);
  macros.InsertLine("ActiveDocument.Tables.Item(1).Columns.Item(1).Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.InsertLine("ActiveDocument.Tables.Item(1).Columns.Item(2).Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphRight");
  macros.Cell(1, 1, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.Cell(1, 1, 2, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphCenter");
  macros.Cell(1, lst.size()+2, 1, "Range.Select");
  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");

  macros.TablesColumns(1, 1, "PreferredWidth = CentimetersToPoints(4)");
  macros.TablesColumns(1, 2, "PreferredWidth = CentimetersToPoints(4)");
  macros.Cell(1, 1, 1, "Range.Text=" + toWrap("Дата"));
  macros.Cell(1, 1, 2, "Range.Text=" + toWrap("Сумма"));

  int itog = 0;
  for (size_t i = 0; i < lst.size(); ++i)
  {
    macros.Cell(1, i+2, 1, "Range.Text=" + toWrap(toRightDate(lst[i].day)));
    macros.Cell(1, i+2, 2, "Range.Text=" + toWrap(lst[i].pay));
    itog += lst[i].pay.toInt();
  }
  macros.Cell(1, lst.size()+2, 1, "Range.Text=" + toWrap("Итог"));
  macros.Cell(1, lst.size()+2, 2, "Range.Text=" + toWrap((LPCSTR)aux::itoa(itog)));

  macros.EndMacros();
  macros.RunMacros("");
}
//-------------------------------------------------------------------------
void BuhReport_Month::ReportDay(string_t day)
{
  mybase::MYFASTRESULT res = theApp.GetCon().Query(
    " SELECT pay.ordernum, SUM(pay.moneypay) as summa, COUNT(*) as cnt "
    " FROM payfactstest as pay "
    " WHERE pay.deleted = 0 AND pay.datepay = '" + day + "'" 
    " GROUP BY pay.ordernum ORDER BY ordernum");

//   string_t buf;
//   while (mybase::MYFASTROW row = res.fetch_row())
//   {
//     buf += "<tr payday='" + payDay + "' ordernum='" + row["ordernum"] + "'>"
//       "<td>" + row["ordernum"] + "</td>"
//       "<td>" + row["summa"]  + "</td>"
//       "<td>" + row["cnt"]  + "</td>"
//       "</tr>";
//   }
}
//-------------------------------------------------------------------------
void BuhReport_Month::ReportFio(string_t day, string_t orderNum)
{
  theApp.GetCon().Query("drop temporary table if exists full_table");
  theApp.GetCon().Query(
    "CREATE TEMPORARY TABLE full_table "
    " ( "
    "  id        int(11), "
    "  summa     int(11)  "
    " ) ");
  theApp.GetCon().Query("INSERT full_table (id, summa) "
    " SELECT pay.idstud, SUM(pay.moneypay) as summa "
    " FROM payfactstest as pay "
    " WHERE pay.deleted = 0 AND pay.datepay = '" + day + "' AND pay.ordernum=" + orderNum +  
    " GROUP BY id");

  string_t query = string_t() +
    " SELECT ft.summa, CONCAT(s.secondname, ' ', s.firstname, ' ', thirdname) as fio, CONCAT(vShift.title, '-', vYear.title, vFast.title, '-',s.dognum) as num"
    " FROM full_table as ft, students as s, voc as vYear, voc as vShift, voc as vFast"
    " WHERE s.id = ft.id AND s.deleted = 0"
    " AND vYear.deleted = 0 AND vYear.vkey = 'dogyear' AND vYear.num = s.dogyearid"
    " AND vShift.deleted = 0 AND vShift.vkey = 'dogshifr' AND vShift.num = s.dogshifrid"
    " AND vFast.deleted = 0 AND vFast.vkey = 'dogfast' AND vFast.num = s.dogfastid"
    " ORDER BY num, fio";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

// 
//   string_t buf;
//   while (mybase::MYFASTROW row = res.fetch_row())
//   {
//     buf += "<tr>"
//       "<td>" + payDay        + "</td>"
//       "<td>" + orderNum      + "</td>"
//       "<td>" + row["summa"]  + "</td>"
//       "<td>" + row["fio"]    + "</td>"
//       "<td>" + row["num"]    + "</td>"
//       "</tr>";
//   }
}
//-------------------------------------------------------------------------
void BuhReport_Month::SerializeData(bool toSave)
{
  Reg reg;
  reg.SetRootKey(HKEY_CURRENT_USER);
  reg.SetKey("ODOBase\\ODOBase\\OTHER");

  if (toSave)
  {
    reg.WriteString("buh-report-month-date",    json::v2t(dateBox_).c_str());
    reg.WriteString("buh-report-month-order",   json::v2t(orderNumBox_).c_str());
    reg.WriteString("buh-report-month-radio-1", json::v2t(link_element("month-radio")).c_str());
    reg.WriteString("buh-report-month-radio-2", json::v2t(link_element("day-radio")).c_str());
    reg.WriteString("buh-report-month-radio-3", json::v2t(link_element("fio-radio")).c_str());
  }
  else
  {
    json::t2v(dateBox_,                    reg.ReadString("buh-report-month-date",    ""));
    json::t2v(orderNumBox_,                reg.ReadString("buh-report-month-order",   "0"));
    json::t2v(link_element("month-radio"), reg.ReadString("buh-report-month-radio-1", "1"));
    json::t2v(link_element("day-radio"),   reg.ReadString("buh-report-month-radio-2", "0"));
    json::t2v(link_element("fio-radio"),   reg.ReadString("buh-report-month-radio-3", "0"));
  }
}
//-------------------------------------------------------------------------
void BuhReport_Month::ReportHeader(WordMacros& macros, string_t title, string_t date)
{
  __timeb64 tstruct;
  _ftime64(&tstruct);
  tm lTime = *_localtime64(&tstruct.time);
  std::wostringstream nowDate;
  nowDate << (1900+lTime.tm_year) << L"-"
       << std::setiosflags(std::ios::right) << std::setfill(L'0')
       << std::setw(2) << (1+lTime.tm_mon)
       << L"-"
       << std::setw(2) << lTime.tm_mday;
  std::wostringstream nowTime;
  nowTime << std::setw(2) << lTime.tm_hour << L":"
          << std::setw(2) << lTime.tm_min  << L":"
          << std::setw(2) << lTime.tm_sec;
   
  macros.InsertLine("ActiveDocument.PageSetup.TopMargin=70");
  macros.InsertLine("ActiveDocument.PageSetup.BottomMargin=55");
  macros.InsertLine("ActiveDocument.PageSetup.LeftMargin=55");
  macros.InsertLine("ActiveDocument.PageSetup.RightMargin=55");

  macros.SelectionParagraphFormat("LineSpacingRule = wdLineSpace1pt5");

  macros.SelectionParagraphFormat("Alignment=wdAlignParagraphLeft");
  macros.SelectionFont("Bold=false");
  macros.SelectionFont("Size=12");
  macros.SelectionText("Отчет \"\"" + title +"\"\"\n");
  macros.SelectionText("диапазон: " + date + "\n");
  macros.SelectionText("составлен: " + toRightDate(nowDate.str()) + " " + nowTime.str() + "\n");
}
//-------------------------------------------------------------------------
string_t BuhReport_Month::toRightDate(string_t date)
{
  string_t year = date.subString(0, 4);
  string_t month = date.subString(5, 2);
  string_t day = date.subString(8, 2);
  return day + "/" + month + "/" + year;
}