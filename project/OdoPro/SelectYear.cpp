#include "stdafx.h"
#include "SingeltonApp.h"
#include "SelectYear.h"
#include "json-aux-ext.h"

using namespace htmlayout::dom;
CSelectYearDlg::CSelectYearDlg(LiteWnd* pParent, string_t year) : LiteWnd(pParent), _year(year)
{
}

CSelectYearDlg::~CSelectYearDlg(void)
{
}

BOOL CSelectYearDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"Выбор года для отчета";
	return TRUE;
}

int CSelectYearDlg::OnCreate()
{
	SetWindowPos(m_hWnd, NULL, -1, -1, 450, 100, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	PBYTE pb; DWORD cb;
	if(GetHtmlResource(L"IDR_HTML_SELECT_YEAR", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	InitDomElement();

	return 0;
}

// обрабатывает кнопки
BOOL CALLBACK CSelectYearDlg::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CSelectYearDlg* dlg = static_cast<CSelectYearDlg*>(tag);
	const wchar_t* id = htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-close"))
	{
    dlg->_year = "";
		dlg->Close();
		return TRUE;
	}
  if (aux::wcseq(id, L"bt-set"))
  {
    dlg->_year = json::v2t(dlg->link_element("year").get_value());
    dlg->Close();
    return TRUE;
  }

	return FALSE;
}

// связывает элементы дом с отображением и инициирует
void CSelectYearDlg::InitDomElement(void)
{
	// связывает с событиями кнопок
	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-set"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);

  json::t2v(link_element("year"), _year);
}
