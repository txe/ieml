#include "stdafx.h"
#include "SingeltonApp.h"
#include "ActionFastInputGak.h"
#include "json-aux-ext.h"

using namespace htmlayout::dom;
CActionFastInputGakDlg::CActionFastInputGakDlg(LiteWnd* pParent /*= NULL */) : LiteWnd(pParent)
{
}

CActionFastInputGakDlg::~CActionFastInputGakDlg(void)
{
}

BOOL CActionFastInputGakDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"Быстрый ввод данных протокола ГАК для группы";
	return TRUE;
}

int CActionFastInputGakDlg::OnCreate()
{
	SetWindowPos(m_hWnd, NULL, -1, -1, 640, 550, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	PBYTE pb; DWORD cb;
	if(GetHtmlResource(L"IDR_HTML_FAST_INPUT_GAK", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	InitDomElement();
	UpdateView();

	return 0;
}

// обрабатывает кнопки
BOOL CALLBACK CActionFastInputGakDlg::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CActionFastInputGakDlg* dlg = static_cast<CActionFastInputGakDlg*>(tag);
	const wchar_t* id = htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-save"))
	{
		dlg->Save();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-close"))
	{
		dlg->Close();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-set"))
	{
		dlg->SetGak();
		return TRUE;
	}
	return FALSE;
}

// обрабатывает изменение семестров
BOOL CALLBACK CActionFastInputGakDlg::ElementEventProcSem(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != EDIT_VALUE_CHANGED)
		return FALSE;

	CActionFastInputGakDlg* dlg = static_cast<CActionFastInputGakDlg*>(tag);
	dlg->UpdateView();

	return TRUE;
}

// связывает элементы дом с отображением и инициирует
void CActionFastInputGakDlg::InitDomElement(void)
{
	// связывает с событиями кнопок
	HTMLayoutAttachEventHandlerEx(link_element("bt-save"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-set"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}

// получение и отображение свойств студента
bool CActionFastInputGakDlg::UpdateView(void)
{
	htmlayout::dom::element table = link_element("gak-table");
	while (table.children_count() > 1)
		htmlayout::dom::element(table.child(1)).destroy();

	// получаем список студентов
	string_t query = string_t() +
		" SELECT id, CONCAT(secondname,' ',firstname,' ',thirdname) AS fio, edudatequalif, edunumprotgak"
		" FROM students "
		" WHERE deleted = 0 AND grpid = " + aux::itow(theApp.GetCurrentGroupID()) +
		" ORDER BY fio, id ";

	mybase::MYFASTRESULT stud_res = theApp.GetCon().Query(query);
	mybase::MYFASTROW	 stud_row;
	string_t			 buf;
	int					 pos = 1;
	while (stud_row = stud_res.fetch_row())	
	{	
		string_t stud_id = stud_row["id"];
		string_t gak = stud_row["edunumprotgak"];
		string_t date = stud_row["edudatequalif"];
		buf += "<tr "
			" stud_id=" + stud_id + 
			" gak='" +gak + "' "
			" date='" + date + "'"
			">";
		buf += string_t() +
			"<td>" + aux::itow(pos++)	+ "</td>"
			"<td>" + stud_row["fio"]	+ "</td>"
			"<td>" + gak + "</td>"
			"<td state='bad' type='masked' mask='####-##-##' leading-zero=true value='" + date + "'></td>"
			"</tr>";
	}
	if (_mbslen(buf))
		table.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);

	if (table.children_count() > 1)
		htmlayout::dom::element(table.child(1)).set_state(STATE_CURRENT);

	// обновляем заголовок
	link_element("header").set_text(theApp.GetGroupName(theApp.GetCurrentGroupID()));
	link_element("header").update();

	table.update();

	return true;
}

// сохранение
void CActionFastInputGakDlg::Save(void)
{
	htmlayout::dom::element table = link_element("gak-table");
	
	if (table.children_count() < 2)
		return;

	bool good = true;
	std::vector<string_t> queries;
	for (htmlayout::dom::element row = table.child(1); row.is_valid(); row = row.next_sibling())
	{
		htmlayout::dom::element dateCell = row.child(3);
		string_t oldDate = row.get_attribute("date");
		string_t newDate = json::v2t(dateCell);

		row.set_attribute("state", L"good");
		if (oldDate != newDate)
		{
			std::vector<std::wstring> date = aux::split(std::wstring(newDate), L'-');
			int year  = date.size() == 0 ? 0 : aux::wtoi(date[0].c_str());
			int month = date.size() <= 1 ? 0 : aux::wtoi(date[1].c_str());
			int day   = date.size() <= 2 ? 0 : aux::wtoi(date[2].c_str());
			if (year < 2010 || year > 2025 || month < 1 || month > 12 || day < 1 || day > 31)
			{
				good = false;
				row.set_attribute("state", L"bad");
			}

		}
		
		string_t oldGak = row.get_attribute("gak");
		string_t newGak = json::v2t(htmlayout::dom::element(row.child(2)).get_value());
		
		string_t id = row.get_attribute("stud_id");
		if (oldGak != newGak)
		{
			string_t query = "UPDATE students SET edunumprotgak = '" + newGak + "' WHERE id = " + id; 
			queries.push_back(query);
		}
		if (oldDate != newDate)
		{
			string_t query = "UPDATE students SET edudatequalif = '" + newDate + "' WHERE id = " + id; 
			queries.push_back(query);
		}
	}

	if (good == false)
	{
		table.update();
		MessageBeep(MB_OK);
		return;
	}
		
	if (!queries.empty())
	{
		// удаление и сохранение оценок
		mysql_autocommit(theApp.GetCon().GetCon(), false);
		theApp.GetCon().Query("START TRANSACTION;");
		try
		{
			for (int i = 0; i < queries.size(); ++i)
			{
				string_t query = queries[i];
				theApp.GetCon().Query(query.c_str());
			}
			// применить все изменения
			theApp.GetCon().Query("COMMIT;");
			mysql_autocommit(theApp.GetCon().GetCon(), true);
		}
		catch(...)
		{
			mysql_autocommit(theApp.GetCon().GetCon(), true);
			theApp.ExceptionManage();
			MessageBeep(MB_ICONEXCLAMATION);
			theApp.GetCon().Query("ROLLBACK;");		
			return;		
		}
	}

	
	Close();
}

// задать всем гак
void CActionFastInputGakDlg::SetGak()
{
	htmlayout::dom::element table = link_element("gak-table");
	string_t gak_value	= json::v2t(link_element("gak-value").get_value());
	if (gak_value.empty())
		return;

	element row = table.child(1);
	for (; row.is_valid(); row = row.next_sibling())
	{
		string_t oldVal = json::v2t(element(row.child(2)).get_value());
		if (oldVal.trim().size() <= 3)
			json::t2v(element(row.child(2)), gak_value + oldVal.trim());
	}
}
