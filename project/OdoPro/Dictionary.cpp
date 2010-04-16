#include "stdafx.h"
#include "Dictionary.h"
#include "json-aux-ext.h"
#include "SingeltonApp.h"

CDictionary::CDictionary(LiteWnd* parent /* = NULL */):LiteWnd(parent)
{
}

CDictionary::~CDictionary(void)
{
}

BOOL CDictionary::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"Словарь";

	cs.cx = 600;
	cs.cy = 600;

	return TRUE;
}

int CDictionary::OnCreate()
{
	CenterWindow();

	PBYTE pb; DWORD cb;
	if (GetHtmlResource(L"IDR_HTML_DICTIONARY", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	else
		assert(FALSE);

	InitDomElement();
	UpdateView();
	ShowSelection();
	return 0;
}

void CDictionary::InitDomElement()
{
	_table	= link_element("table");
	_title	= link_element("title");
	_num	= link_element("num");
	_filter = link_element("filter");
	json::t2v(_filter, "city");

	// присоединяем процедуру, отвлавливающую выбор строки с дисциплиной
	HTMLayoutAttachEventHandlerEx(_table, ElementEventProcForTable, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

	HTMLayoutAttachEventHandlerEx(_filter, ElementEventProcChangedFilter, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

	// присоединяем обоработчики к кнопкам
	HTMLayoutAttachEventHandlerEx(link_element("bt-add"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-del"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-save"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}

// обрабатывает кнопки
BOOL CALLBACK CDictionary::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CDictionary*	dlg	= static_cast<CDictionary*>(tag);
	const wchar_t*	id	= htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-add"))
	{
		dlg->Add();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-save"))
	{
		dlg->Save();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-del"))
	{
		dlg->Delete();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-close"))
	{
		dlg->Close();
		return TRUE;
	}


	return FALSE;
}

void CDictionary::UpdateView(void)
{
	assert(_table.is_valid());

	// удаляем строки кроме заголовка таблицы
	while (_table.children_count() > 1)
		HTMLayoutDetachElement(_table.child(1));

	string_t  query = string_t() +
		" SELECT id, num, title, vkey "
		" FROM voc "
		" WHERE deleted = 0 AND vkey like '" + json::v2t(_filter.get_value()) + "' "
		" ORDER BY vkey, num, id";

	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

	string_t			buf;
	mybase::MYFASTROW	row;
	int					count = 0;
	while (row = res.fetch_row())	
	{
		buf += "<tr id=" + row["id"]+ ">";
		buf += string_t() + 
			"<td>" + row["vkey"] + "</td>"
			"<td>" + row["num"] + "</td>"
			"<td>" + row["title"] + "</td>"
			"</tr>";
	}

	if (_mbslen(buf))
		_table.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
	_table.update();

	if (_table.children_count() > 1)
	{
		element first = _table.child(1);
		first.set_state(STATE_CURRENT);
	}

	_table.update(true);
}

// отображает выбор
void CDictionary::ShowSelection()
{
	element find = _table.find_first("tr:current");
	if (find.is_valid())
	{
		json::t2v(_num, element(find.child(1)).get_text());
		json::t2v(_title, element(find.child(2)).get_text());
	}
	else
	{
		json::t2v(_num, "0");
		json::t2v(_title, "");
	}
}

// обрабатывает выбор
BOOL CALLBACK CDictionary::ElementEventProcForTable(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != TABLE_ROW_CLICK)
		return FALSE;

	CDictionary* dlg = static_cast<CDictionary*>(tag);
	dlg->ShowSelection();

	return TRUE;
}

//
BOOL CALLBACK CDictionary::ElementEventProcChangedFilter(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != SELECT_SELECTION_CHANGED)
		return FALSE;

	CDictionary* dlg = static_cast<CDictionary*>(tag);
	dlg->UpdateView();

	return TRUE;	
}

bool CDictionary::IsExistsTitleForKey( bool exclude_cur /*= false*/ )
{
	string_t title = json::v2t(_title.get_value());
	string_t vkey  = json::v2t(_filter.get_value());

	string_t query = string_t() +
		" SELECT id FROM voc "
		" WHERE deleted = 0 AND vkey='" + vkey + "' AND " + 
		(exclude_cur?(" id != " + GetRecordId() + " AND "):"") + 
		" title = '" + title + "' LIMIT 1";
	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

	return res.size() > 0;
}

void CDictionary::Add(void)
{
	string_t vkey = json::v2t(_filter.get_value());
	if (aux::streq(vkey, "%"))
	{
		string_t msg = "Не выбран ключ";
		MessageBox(m_hWnd, msg, L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
		return;
	}
	if (IsExistsTitleForKey())
	{
		string_t msg =  "Такая запись уже есть в словаре.\n" 
					    "Нажмите ОК, если хотите продолжить.\n"
					    "Нажмите CANCEL, если хотите отменить.";
		if (IDOK != MessageBox(m_hWnd, msg, L"Сообщение", MB_OKCANCEL | MB_ICONINFORMATION | MB_APPLMODAL))
			return;
	}
	
	string_t title	= json::v2t(_title);
	string_t num	= GetNextNumForKey(vkey);
	string_t query	= string_t() + 
		" INSERT INTO voc(num, vkey, title) VALUES(" + num +", '" + vkey + "', '" + title + "')";
	theApp.GetCon().Query(query);
	
	UpdateView();
}

string_t CDictionary::GetNextNumForKey( const string_t& vkey, const string_t default_num /*= "-1" */ )
{
	if (aux::wtoi(default_num, -1) > -1)
	{
		string_t query = string_t() +
			" SELECT id FROM voc "
			" WHERE deleted = 0 AND vkey='" + vkey + "' AND "
			" num = '" + default_num + "' LIMIT 1";
		mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
		if (res.size() == 0)
			return default_num;
	}
	string_t query = string_t() +
		" SELECT MAX(num) as max FROM voc "
		" WHERE deleted = 0 AND vkey='" + vkey + "'";
	mybase::MYFASTRESULT res = theApp.GetCon().Query(query, false); // можут вернуть NULL ввиде "", тогда вернем 0
	return string_t(aux::itow(aux::wtoi(mybase::MYFASTROW(res.fetch_row())["max"], 0) + 1));
}


void CDictionary::Save(void)
{
	string_t vkey = json::v2t(_filter.get_value());

	if (aux::streq(vkey, "%"))
	{
		string_t msg = "Не выбран ключ";
		MessageBox(m_hWnd, msg, L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
		return;
	}
	if (IsExistsNumForKey(true))
	{
		string_t msg = "Такой номер уже используется в словаре.\nИзменение не произошло.";
		MessageBox(m_hWnd, msg, L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
		return;
	}
	if (IsExistsTitleForKey(true))
	{
		string_t msg =  "Такая запись уже есть в словаре.\n" 
						"Нажмите ОК, если хотите продолжить.\n"
						"Нажмите CANCEL, если хотите отменить.";
		if (IDOK != MessageBox(m_hWnd, msg, L"Сообщение", MB_OKCANCEL | MB_ICONINFORMATION | MB_APPLMODAL))
			return;
	}

	string_t msg = "Вы действительно хотите сохранить изменения?";
	if (IDNO == MessageBox(m_hWnd, msg, L"Сообщение", MB_YESNO | MB_ICONINFORMATION | MB_APPLMODAL))
		return;

	string_t id = GetRecordId(false);
	if (aux::streq(id, ""))
	{
		string_t msg = "Нет записи для редактирования.\n"
		  	"Действие отменено.";
		MessageBox(m_hWnd, msg, L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
		return;
	}

	string_t num	= json::v2t(_num);
	string_t title	= json::v2t(_title);
	string_t query	= string_t() + 
		"UPDATE voc SET title = '" + title + "', num = " + num + " WHERE id = " + id;
		
	theApp.GetCon().Query(query);
	UpdateView();
}

bool CDictionary::IsExistsNumForKey( bool exclude_cur /*= false*/ )
{
	string_t num	= json::v2t(_num.get_value());
	string_t vkey	= json::v2t(_filter.get_value());

	string_t query = string_t() +
		" SELECT id FROM voc "
		" WHERE deleted = 0 AND vkey='" + vkey + "' AND " +
		(exclude_cur?(" id != " + GetRecordId() + " AND "):"") + 
		" num = '" + num + "' LIMIT 1";
	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

	return res.size() > 0;
}

string_t CDictionary::GetRecordId( bool throw_ /*= true*/ )
{
	element find = _table.find_first("tr:current");
	if (find.is_valid())
		return find.get_attribute("id");
	if (throw_)
		throw wss::exception(wss::reason_message(FULL_LOCATION()));
	else
		return "";
}

void CDictionary::Delete( void )
{
	string_t id = GetRecordId(false);
	if (aux::streq(id, ""))
	{
		string_t msg = "Нет записи для удаления.\n"
			"Действие отменено.";
		MessageBox(m_hWnd, msg, L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
		return;
	}
	
	string_t msg = "Вы действительно хотите удалить эту запись?";
	if (IDNO == MessageBox(m_hWnd, msg, L"Сообщение:", MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL))
		return;

	string_t query = "UPDATE voc SET deleted = 1 WHERE id = " + id;
	theApp.GetCon().Query(query);

	UpdateView();
}