#include "stdafx.h"
#include "SingeltonApp.h"
#include "SelectGroups.h"
#include "json-aux-ext.h"

using namespace htmlayout::dom;
CSelectGroupsDlg::CSelectGroupsDlg(LiteWnd* pParent) : LiteWnd(pParent)
{
}

CSelectGroupsDlg::~CSelectGroupsDlg(void)
{
}

BOOL CSelectGroupsDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"Выбор группы для отчета";
	return TRUE;
}

int CSelectGroupsDlg::OnCreate()
{
	SetWindowPos(m_hWnd, NULL, -1, -1, 250, 400, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	PBYTE pb; DWORD cb;
	if(GetHtmlResource(L"IDR_HTML_SELECT_GROUPS", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	InitDomElement();

	return 0;
}

// обрабатывает кнопки
BOOL CALLBACK CSelectGroupsDlg::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CSelectGroupsDlg* dlg = static_cast<CSelectGroupsDlg*>(tag);
	const wchar_t* id = htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-close"))
	{
    dlg->groups.clear();
		dlg->Close();
		return TRUE;
	}
  if (aux::wcseq(id, L"bt-run"))
  {
    dlg->SetGroups();
    dlg->Close();
    return TRUE;
  }

	return FALSE;
}

// связывает элементы дом с отображением и инициирует
void CSelectGroupsDlg::InitDomElement(void)
{
  // don't forget check in CActionBuhReport2 - it's copy past
  string_t filter[10] = {"БУ%", "МО%", "МР%", "Ю%", "ЭП%", "ЭУН%", "ПМ%", "ПГС%", "ТГВ%",
    "%'"
    " AND title NOT like 'БУ%'"
    " AND title NOT like 'МО%'"
    " AND title NOT like 'МР%'"
    " AND title NOT like 'Ю%'"
    " AND title NOT like 'ЭП%'"
    " AND title NOT like 'ЭУН%'"
    " AND title NOT like 'ПМ%'"
    " AND title NOT like 'ПГС%'"
    " AND title NOT like 'ТГВ%"};
  string_t name[10] = {"БУ", "МО", "МР", "Ю", "ЭП", "ЭУН", "ПМ", "ПГС", "ТГВ", "Остальные"};

  string_t buf = "<widget id=\"grp-lst\" type=\"select\"  style=\"width:100%%;height:100%%;visibility:show\">";
  for (int i = 0; i < 10; ++i)
  {
    string_t query	= 
      " SELECT num , title FROM voc "
      " WHERE vkey='grp' AND deleted = 0 AND "
      " title like '" + filter[i] + "' "
      " ORDER BY title, id";
    mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
    mybase::MYFASTROW row;

    buf += "<OPTIONS>" + name[i];
    while (row = res.fetch_row())	
      buf += "<OPTION><button type='checkbox' #shot value=" + row["num"] + ">" +row["title"] + "</button></OPTION>";
    buf += "</OPTIONS>";
  }
  buf += "</widget>";

  assert(_mbslen(buf));
  if (_mbslen(buf))
    link_element("d").set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
  link_element("d").update();
	
  // связывает с событиями кнопок
	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-run"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}

void CSelectGroupsDlg::SetGroups()
{
  struct checker_cb: htmlayout::dom::callback 
  {
    bool on_element(HELEMENT he) 
    { 
      htmlayout::dom::element el = he; 
      if (el.get_state(STATE_CHECKED))
        if (string_t(el.get_element_type()) == "button")
          v.push_back(el.get_attribute("value"));

      return false; /*continue enumeration*/ 
    }
    std::vector<string_t> v;
  };
  checker_cb checker;
  link_element("grp-lst").find_all(&checker,":checked");
  groups = checker.v;
}