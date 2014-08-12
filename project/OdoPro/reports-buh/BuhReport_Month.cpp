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
  SetWindowPos(m_hWnd, NULL, -1, -1, 450, 450, 
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
//   HTMLayoutAttachEventHandlerEx(link_element("o-all"), ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
//   HTMLayoutAttachEventHandlerEx(link_element("o-spec"), ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
//   HTMLayoutAttachEventHandlerEx(link_element("o-grp"), ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
//   HTMLayoutAttachEventHandlerEx(link_element("pay-radio"), ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
//   HTMLayoutAttachEventHandlerEx(link_element("cat-radio"), ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
// 
//   // присоедин€ем обоработчики к кнопкам
//   HTMLayoutAttachEventHandlerEx(link_element("bt-report"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
//   HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
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
  if (pr->cmd == BUTTON_CLICK)
  {
    BuhReport_Month* dlg = static_cast<BuhReport_Month*>(tag);
    //dlg->StateChange();
    return TRUE;
  }

  return FALSE;
}
