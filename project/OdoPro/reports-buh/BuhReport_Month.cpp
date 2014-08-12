#include "StdAfx.h"
#include "BuhReport_Month.h"
#include "..\SingeltonApp.h"
#include "..\json-aux-ext.h"
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

  cs.lpszName = (LPCWSTR)"ќплата за мес€ц по дн€м";
  return TRUE;
}
//-------------------------------------------------------------------------
int BuhReport_Month::OnCreate()
{
  SetWindowPos(m_hWnd, NULL, -1, -1, 800, 800, 
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
  monthGrid_    = link_element("month-grid");
  orderNumGrid_ = link_element("ordernum-grid");
  fioGrid_      = link_element("fio-grid");

  HTMLayoutAttachEventHandlerEx(monthGrid_,    ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
  HTMLayoutAttachEventHandlerEx(orderNumGrid_, ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

  // присоедин€ем обоработчики к кнопкам
  HTMLayoutAttachEventHandlerEx(link_element("bt-report"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
  HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);

  FillMonthGrid("2014-01");
}

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
    dlg->Close();
    return TRUE;
  }

  return FALSE;
}
//-------------------------------------------------------------------------
// обрабатывает выбор предмета/семестра
BOOL CALLBACK BuhReport_Month::ElementEventProcFor(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
  if (evtg != HANDLE_BEHAVIOR_EVENT)
    return FALSE;

  BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
  if (pr->cmd == TABLE_ROW_CLICK)
  {
    element el = pr->heTarget;
    BuhReport_Month* dlg = static_cast<BuhReport_Month*>(tag);
    if (dlg->monthGrid_ == he)    dlg->FillOrderNumGrid(el.get_attribute("payDay"));
    if (dlg->orderNumGrid_ == he) dlg->FillFioGrid(el.get_attribute("payDay"), el.get_attribute("orderNum"));
    return TRUE;
  }

  return FALSE;
}
//-------------------------------------------------------------------------
// заполнить таблицу мес€ца данными
void BuhReport_Month::FillMonthGrid(string_t payMonth)
{
  // получим мес€чные данные
  string_t startMonth = "'" + payMonth + "-01'";
  string_t endMonth   = "'" + payMonth + "-31'";
  mybase::MYFASTRESULT res = theApp.GetCon().Query(
    " SELECT pay.datepay, SUM(pay.moneypay) as summa "
    " FROM payfactstest as pay "
    " WHERE pay.deleted = 0 AND pay.datepay >= " + startMonth + " AND pay.datepay <= " + endMonth + 
    " GROUP BY pay.datepay ORDER BY datepay");

  string_t buf;
  while (mybase::MYFASTROW row = res.fetch_row())
  {
    buf += "<tr payday='" + row["datepay"] + "'>"
           "<td>" + row["datepay"] + "</td>"
           "<td>" + row["summa"]  + "</td>";
           "</tr>";
  }

  if (_mbslen(buf))
    monthGrid_.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
  monthGrid_.update();
}
//-------------------------------------------------------------------------
// заполнить таблицу номеров выплат
void BuhReport_Month::FillOrderNumGrid(string_t payDay)
{
  mybase::MYFASTRESULT res = theApp.GetCon().Query(
    " SELECT pay.ordernum, SUM(pay.moneypay) as summa, COUNT(*) as cnt "
    " FROM payfactstest as pay "
    " WHERE pay.deleted = 0 AND pay.datepay = '" + payDay + "'" 
    " GROUP BY pay.ordernum ORDER BY ordernum");

  string_t buf;
  while (mybase::MYFASTROW row = res.fetch_row())
  {
    buf += "<tr payday='" + payDay + "' ordernum='" + row["ordernum"] + "'>"
      "<td>" + row["ordernum"] + "</td>"
      "<td>" + row["summa"]  + "</td>"
      "<td>" + row["cnt"]  + "</td>"
      "</tr>";
  }

  t::ClearTable(orderNumGrid_, 1);
  if (_mbslen(buf))
    orderNumGrid_.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
  orderNumGrid_.update();

  t::ClearTable(fioGrid_, 1);
  fioGrid_.update();
}
//-------------------------------------------------------------------------
// заполнить таблицу фамилий
void BuhReport_Month::FillFioGrid(string_t payDay, string_t orderNum)
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
    " WHERE pay.deleted = 0 AND pay.datepay = '" + payDay + "' AND pay.ordernum=" + orderNum +  
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


  string_t buf;
  while (mybase::MYFASTROW row = res.fetch_row())
  {
    buf += "<tr>"
      "<td>" + payDay        + "</td>"
      "<td>" + orderNum      + "</td>"
      "<td>" + row["summa"]  + "</td>"
      "<td>" + row["fio"]    + "</td>"
      "<td>" + row["num"]    + "</td>"
      "</tr>";
  }

  t::ClearTable(fioGrid_, 1);
  if (_mbslen(buf))
    fioGrid_.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
  fioGrid_.update();

  theApp.GetCon().Query("drop temporary table if exists full_table");
}
