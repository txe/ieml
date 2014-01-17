#include "stdafx.h"
#include "SPayment.h"
#include "LiteWnd.h"
#include "string_t.h"
#include "SingeltonApp.h"
#include "json-aux-ext.h"
#include <map>
#include "logger.h"
#include <deque>


SPayment::SPayment(void)
{
}

SPayment::~SPayment(void)
{
}

// инициализирует дом объекты и все такое
void SPayment::Init(htmlayout::dom::element root)
{
	_root			= root;
	_payment_table	= LiteWnd::link_element(_root, "payment-table"); 
	_cat_month		= LiteWnd::link_element(_root, "pay-cat-manth"); 
	_cat_year		= LiteWnd::link_element(_root, "pay-cat-year"); 
	_cat_money		= LiteWnd::link_element(_root, "pay-cat-money"); 
	_cat_half_year	= LiteWnd::link_element(_root, "pay-cat-half-year");
	
	// инициируем обработчики кнопок
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-save"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-del"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-cat-add"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-cat-del"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-cat-save"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-cat-per"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-show-only-pay"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);

	// присоединяем процедуру, отвлавливающую выбор строки категории/оплаты
	HTMLayoutAttachEventHandlerEx(_payment_table, ElementEventProcForPayTable, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
}


// обрабатывает кнопки
BOOL CALLBACK SPayment::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	SPayment* dlg = static_cast<SPayment*>(tag);
	const wchar_t* id = htmlayout::dom::element(he).get_attribute("id");

	
	if (aux::wcseq(id, L"pay-save"))
	{
		dlg->AddPay();
		return TRUE;
	}
	if (aux::wcseq(id, L"pay-del"))
	{
		dlg->DeletePay();
		return TRUE;
	}
	if (aux::wcseq(id, L"pay-cat-add"))
	{
		dlg->AddCat();
		return TRUE;
	}
	if (aux::wcseq(id, L"pay-cat-del"))
	{
		dlg->DeleteCat();
		return TRUE;
	}
	if (aux::wcseq(id, L"pay-cat-save"))
	{
		dlg->SaveUpdateCat();
		return TRUE;
	}
	if (aux::wcseq(id, L"pay-cat-per"))
	{
		dlg->PersonalCat();
		return TRUE;
	}
	if (aux::wcseq(id, L"pay-show-only-pay"))
	{
		dlg->ShowOnlyPay();
		return TRUE;
	}

	MessageBox(::GetActiveWindow(), L"Еще не работает", L"Предупреждение", 0);
	return FALSE;
}

void SPayment::UpdateView(void)
{
	UpdateViewPayment();
	UpdateViewCat();
	InitPayPay();
}

void SPayment::UpdateViewPayment()
{
	// удаляем все строки
  t::ClearTable(_payment_table, 0);

  // информация об оплатах студента
	std::map<std::wstring, std::wstring> info_pay;
	std::map<std::wstring, int>			 pay;
	
	string_t pay_query = string_t() +
		" SELECT id, idstud, datepay, moneypay, idopts, deleted " 
		" FROM payfactstest "
		" WHERE deleted = 0 AND idstud = " + aux::itow(theApp.GetCurrentStudentID()) + 
		" ORDER BY idopts, datepay";

	mybase::MYFASTRESULT pay_res = theApp.GetCon().Query(pay_query);
	while (mybase::MYFASTROW pay_row = pay_res.fetch_row())	
	{	
		std::wstring optid = pay_row["idopts"];
		pay[optid] = pay[optid] + aux::wtoi(pay_row["moneypay"]);
		string_t inf = "<option idfact=" + pay_row["id"] + "><caption>дата платежа: " + pay_row["datepay"] + 
			" сумма платежа: " + pay_row["moneypay"] + "</caption></option>";
		info_pay[optid] = info_pay[optid] + inf; 
	}

	// информация о категориях оплаты
	string_t info_query = string_t() +
		" SELECT po.datestart, po.dateend, po.commoncountmoney, po.half_year, po.id, COUNT(ct.idstud) AS cnt "
		" FROM (SELECT o.id, f.idstud FROM payoptstest as o left  join payfactstest as f " 
		" ON f.idopts = o.id "
		" WHERE o.deleted = 0 AND o.idgroup = "  + aux::itow(theApp.GetCurrentGroupID()) + 
		" GROUP BY o.id, f.idstud) AS ct, payoptstest AS po "
		" WHERE po.id = ct.id GROUP BY ct.id ORDER BY po.datestart, po.commoncountmoney, po.id";

	mybase::MYFASTRESULT info_res = theApp.GetCon().Query(info_query);

	string_t			buf;
	mybase::MYFASTROW	info_row;

	while (info_row = info_res.fetch_row())	
	{	
		std::wstring optid	= info_row["id"];
		std::wstring studid	= aux::itow(theApp.GetCurrentStudentID());

		// определим есть ли персональная категория оплаты
		string_t query = string_t() +
			"SELECT id,commoncountmoney  FROM paypersonaltest "
			" WHERE idstud = " + studid + " AND idopts = "  + optid +
			" AND deleted = 0";
		std::wstring per_id = L"";
		int per_money = 0;
		mybase::MYFASTRESULT per_res = theApp.GetCon().Query(query);
		if (mybase::MYFASTROW per_row = per_res.fetch_row())
		{
			per_id = per_row["id"];
			per_money = aux::wtoi(per_row["commoncountmoney"]);
		}
		

		std::wstring period = DateToPayFormat(info_row["datestart"]) + " - " + DateToPayFormat(info_row["dateend"]);
		std::wstring atr	= string_t() + 
			" optid=" + optid +  
			(per_id == L"" ? L"" : L" per_id=" + per_id) +
			" dolg=" + (per_id == L"" ? aux::itow(aux::wtoi(info_row["commoncountmoney"]) - pay[optid]) : aux::itow(per_money - pay[optid])) + 
			" period=\"" + period +"\" " +
			" money=" + info_row["commoncountmoney"] +
			" half_year=" + info_row["half_year"] +
			" datestart=\"" + info_row["datestart"] + "\"";
		std::wstring dolg;
		if (!info_pay[optid].empty())
			dolg = string_t() + "[долг: " + (per_id == L"" ? aux::itow(aux::wtoi(info_row["commoncountmoney"]) - pay[optid]) : aux::itow(per_money - pay[optid])) + " руб.]";			
		buf += string_t() + "<options" + atr +"><caption>";
		buf += string_t() + period + " [студ: " + info_row["cnt"] + "]";
		if (0 == aux::wtoi(info_row["half_year"]))
			buf += string_t() + "<br/>[сокр: нет]";
		else
			buf += string_t() + "<br/>[сокр: <font color='#9900FF'>да</font>" + "]";
		if (per_id == L"")
			buf += string_t() + "[сумма: " + info_row["commoncountmoney"] + " руб.]";
		else
			buf += string_t() + "[сумма: " + aux::itow(per_money) + "(" + info_row["commoncountmoney"] + ") руб.]";
		buf += dolg;
		buf += "</caption>";
		buf += info_pay[optid];
		buf += "</options>";
	}

	if (_mbslen(buf))
		_payment_table.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
	
	_payment_table.update();
	
	if (_payment_table.children_count() > 0)
		element(_payment_table.child(0)).set_state(STATE_CURRENT);	
	// раскроем все записи
	for (uint i = 0; i < _payment_table.children_count(); ++i)
	{
		element option = _payment_table.child(i);
		if (option.children_count() > 1)
			option.set_state(STATE_EXPANDED);
	}
	///LOG_DEBUG << string_t(_payment_table.get_html());
}

// обрабатывает выбор категории/оплаты
BOOL CALLBACK SPayment::ElementEventProcForPayTable(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	//LOG_DEBUG << evtg;
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;
	
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != SELECT_SELECTION_CHANGED)
		return FALSE;

	SPayment* dlg = static_cast<SPayment*>(tag);
	dlg->UpdateViewCat();
	dlg->InitPayPay();

	return TRUE;
}
// переводит дату из формата sql в формат  требуемый в оплатах
string_t SPayment::DateToPayFormat(const string_t& pay_date)
{
	// 2001-02-01
	std::vector<std::wstring> result = aux::split(std::wstring(pay_date), L'-');
	if (result[1] == L"02")
		return L"фев. " + result[0];
	if (result[1] == L"09")
		return L"сент. " + result[0];
	
	throw wss::exception(wss::reason_message(FULL_LOCATION()));
}

// сохраняет оплату студента
void SPayment::AddPay(void)
{
	if (!GetCurrentCat().is_valid())
	{
		MessageBox(::GetActiveWindow(), L"Для занесения оплаты выберете категорию оплаты.", L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	if (!CheckInputPay())
		return;

	string_t pay_date	= json::v2t(LiteWnd::link_element(_root, "pay-date").get_value()); 
	string_t pay_pay	= json::get_caption(LiteWnd::link_element(_root, "pay-pay"));//json::v2t(LiteWnd::link_element(_root, "pay-pay").get_value());
	string_t studid		= aux::itow(theApp.GetCurrentStudentID());
	string_t optid		= GetCurrentCat().get_attribute("optid");
	string_t period		= GetCurrentCat().get_attribute("period");

	string_t query = string_t() + 
		"INSERT INTO payfactstest "
		"(idstud,datepay,moneypay,idopts) " 
		" VALUES(" + studid +", '" + pay_date + "', " + pay_pay + ", " + optid + ")";
	theApp.GetCon().Query(query);
	
	UpdateView();
	
	string_t msg = "Была произведена оплата."
		"\nСтудент(ка):d\t\t" + theApp.GetFIO() + 
		"\nГруппа:\t\t\t" + theApp.GetGroupName(theApp.GetCurrentGroupID()) + 
		"\nПериод оплаты:\t\t" + period + 
		"\nДата оплаты:\t\t" + pay_date + 
		"\nСумма данной оплаты:\t" + pay_pay + " руб.";
	MessageBox(::GetActiveWindow(), msg, 
		L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
}

// проверяет введенную оплату на корректность
bool SPayment::CheckInputPay(void)
{
	string_t pay_date = json::v2t(LiteWnd::link_element(_root, "pay-date").get_value()); 
	string_t pay_pay  = json::get_caption(LiteWnd::link_element(_root, "pay-pay"));//json::v2t(LiteWnd::link_element(_root, "pay-pay").get_value());

	string_t msg;
	if (pay_date.empty())
		msg= "Введенная дата не валидна.\nИсправьте пожалуйста.";
	if (aux::wtoi(pay_pay, -1) == -1)
		msg = "Сумма оплаты должна задаваться как целое положительное число.\nИсправьте пожалуйста.";
	if (!msg.empty())
	{
		MessageBox(::GetActiveWindow(), msg, 
			L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}
	
	element catpay	= GetCurrentCat(); // может ничего не вернуть, проверили в оплате
	long	dolg	= catpay.get_attribute_int("dolg");

	if (aux::wtoi(pay_pay) > dolg)
		msg = string_t() + 
			"Задолженность по оплате составляет " + aux::itow(dolg) + " руб.\n"
			"Вы хотите произвести оплату в размере " + pay_pay + " руб.\n"
			"Чтобы избежать дальнейшее непонимание, для погашения задолженности введите " + aux::itow(dolg) + 
			" руб. в качестве суммы текущей и последней оплаты в этом периоде!";

	if (!msg.empty())
	{
		MessageBox(::GetActiveWindow(), msg, 
			L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}
	return true;
}

// возвращает текущиую категорию оплаты
element SPayment::GetCurrentCat(void)
{
	element find = _payment_table.find_first(":current");
	if (!find.is_valid())
	{
		return find; // бедет невалидным
		//assert(FALSE);
		//throw wss::exception(wss::reason_message(FULL_LOCATION()));
	}
	// если оплата, то передем на категорию 
	if (aux::streq(find.get_element_type(), "option"))
		find = find.parent();
	if (!aux::streq(find.get_element_type(), "options") 
		|| -1 == find.get_attribute_int("optid", -1)
		|| -1 == find.get_attribute_int("dolg", -1))
	{
		assert(FALSE);
		throw wss::exception(wss::reason_message(FULL_LOCATION()));
	}
	return find;
}

// удалить оплату
void SPayment::DeletePay(void)
{
	element find = _payment_table.find_first(":current");
	
	if (!aux::streq(find.get_element_type(), "option") || -1 == find.get_attribute_int("idfact", -1))
	{
		MessageBox(::GetActiveWindow(), L"Для удаления оплаты выберете факт оплаты.", L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}

	string_t msg = "Вы действительно хотите удалить эту запись по факту оплаты?\n"
		"Будьте аккуратны!";
	if (IDNO == MessageBox(::GetActiveWindow(), msg, L"Предупреждение", MB_YESNO |MB_ICONINFORMATION | MB_APPLMODAL))
		return;
	
	string_t query = string_t() + 
		"UPDATE payfactstest SET deleted = 1 WHERE id = " + find.get_attribute("idfact");
	theApp.GetCon().Query(query);
	
	UpdateView();
}

// обновляет инфе о выбранной категории
void SPayment::UpdateViewCat(void)
{
	element cat = GetCurrentCat();
	if (!cat.is_valid()) // для пустой группы категории еще не введены
		return;

	std::wstring buf = cat.get_attribute("datestart"); 
	std::vector<std::wstring> date = aux::split(buf, L'-');

	json::t2v(_cat_month, date[1]);
	json::t2v(_cat_year, date[0]);
	json::t2v(_cat_money, cat.get_attribute("money"));
	json::t2v(_cat_half_year, cat.get_attribute("half_year"));	
}

// добавить категорию оплаты
void SPayment::AddCat(void)
{
	if (IsExistCat())
	{
		string_t msg = "Категория оплаты c таким периодом оплаты уже существует.\n"
			"Добавить еще одну категорию оплаты ?";
		if (IDNO == MessageBox(::GetActiveWindow(), msg, L"Сообщение", MB_YESNO | MB_ICONINFORMATION | MB_APPLMODAL))
			return;
	}
	
	string_t money		= json::v2t(_cat_money.get_value());
	string_t half_year	= json::v2t(_cat_half_year.get_value());
	string_t grpid		= aux::itow(theApp.GetCurrentGroupID());
	string_t startdate;
	string_t enddate;
	GetDateCat(startdate, enddate);

	if (aux::wtoi(money, -1) < 0)
	{
		string_t msg = "Сумма оплаты не может быть отрицательной.\nИсправьте пожалуйста.";
		MessageBox(::GetActiveWindow(), msg, L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}

	string_t query = "INSERT INTO  payoptstest (idgroup, datestart, dateend, commoncountmoney, half_year, deleted) "
		" VALUES(" + grpid + ", '" + startdate + "', '" + enddate + "', " + money + ", " + half_year + ", 0)";
	theApp.GetCon().Query(query);
	
	UpdateView();
}

// проверяет существует ли такая категория оплаты
bool SPayment::IsExistCat(bool include_cur_cat /* = true */)
{
	string_t grpid			= aux::itow(theApp.GetCurrentGroupID());
	string_t incl			= include_cur_cat || !GetCurrentCat().is_valid()?"":(string_t() + " id != " + GetCurrentCat().get_attribute("optid") + " AND ");
	string_t startdate;
	string_t enddate;

	GetDateCat(startdate, enddate);

	string_t query = "SELECT id FROM payoptstest "
		" WHERE idgroup = " + grpid + " AND " + incl +
		" datestart = '" + startdate + "' AND "
		" dateend = '" + enddate + "' AND "
		" deleted = 0";

	mybase::MYFASTRESULT	res = theApp.GetCon().Query(query);
	return res.size() > 0;
}

// возвращает даты начала и конца периода оплаты
void SPayment::GetDateCat(string_t& startdate, string_t& enddate)
{
	string_t month	= json::v2t(_cat_month.get_value());
	string_t year	= json::v2t(_cat_year.get_value());

	startdate	= year + "-0" + month + "-01";
	enddate		= string_t(aux::itow(aux::wtoi(year) + 1)) + "-0" + month + "-01"; 
}

// сохраняет изменения категории оплаты
void SPayment::SaveUpdateCat(void)
{
	if (!GetCurrentCat().is_valid())
	{
		MessageBox(::GetActiveWindow(), L"Для сохранения изменений выберете категорию оплаты.", L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	if (IsExistCat(false))
	{
		string_t msg = "Категория оплаты с такими параметрами уже существует в базе данных.\n"
			"Сохранение не произошло!";
		MessageBox(::GetActiveWindow(), msg, L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
		return;
	}
	
	string_t money		= json::v2t(_cat_money.get_value());
	string_t half_year	= json::v2t(_cat_half_year.get_value());
	string_t grpid		= aux::itow(theApp.GetCurrentGroupID());
	string_t catid		= GetCurrentCat().get_attribute("optid");
	string_t startdate;
	string_t enddate;
	GetDateCat(startdate, enddate);

	if (aux::wtoi(money, -1) < 0)
	{
		string_t msg = "Сумма оплаты не может быть отрицательной.\nИсправьте пожалуйста.";
		MessageBox(::GetActiveWindow(), msg, L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}

	string_t query = 
		" UPDATE payoptstest "
		" SET idgroup = " + grpid + ", datestart = '" + startdate + "', "
		" dateend = '" + enddate + "' , commoncountmoney = " + money + ", " 
		" half_year = " + half_year +
		" WHERE id = " + catid;

	theApp.GetCon().Query(query);
	UpdateView();
}

// удаляет выбранную категорию
void SPayment::DeleteCat(void)
{
	if (!GetCurrentCat().is_valid())
	{
		MessageBox(::GetActiveWindow(), L"Для удаления категории оплаты или персональной категории оплаты выберете категорию оплаты.", L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	int per_id = GetCurrentCat().get_attribute_int("per_id", -1);
	string_t msg;
	if (per_id == -1)
		msg = "Вы действительно хотите удалить категорию оплаты?\n"
			  "Ведь при её удалении пропадут некоторые данные о студентах,\n"
		      "а при попытке исправить ситуацию и внести запись с теми же данными структура базы не восстановится!\n"
		      "Будьте аккуратны!";
	else
		msg = "Вы действительно хотите удалить ПЕРСОНАЛЬНУЮ категорию оплаты?";

	if (IDNO == MessageBox(::GetActiveWindow(), msg, L"Предупреждение", MB_YESNO | MB_ICONERROR | MB_APPLMODAL))
		return;

	if (per_id == -1)
	{
		string_t cat_id = GetCurrentCat().get_attribute("optid");
		string_t query = "UPDATE payoptstest SET deleted = 1 WHERE id = " + cat_id;
		theApp.GetCon().Query(query);
	}
	else
	{
		string_t query = string_t() + "UPDATE paypersonaltest SET deleted = 1 WHERE id = " + aux::itow(per_id);
		theApp.GetCon().Query(query);
	}
	UpdateView();
}

// задает для выбранной категории собственную оплату студента
void SPayment::PersonalCat()
{
	if (!GetCurrentCat().is_valid())
	{
		MessageBox(::GetActiveWindow(), L"Для создания персональной оплаты выберете существующую категорию оплаты", L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	
	element  cat    = GetCurrentCat();
	string_t money  = json::v2t(_cat_money.get_value());
	string_t catid  = GetCurrentCat().get_attribute("optid");
	string_t studid	= aux::itow(theApp.GetCurrentStudentID());

	if (aux::wtoi(money, -1) < 0)
	{
		string_t msg = "Сумма оплаты не может быть отрицательной.\nИсправьте пожалуйста.";
		MessageBox(::GetActiveWindow(), msg, L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	if (aux::wtoi(money, -1) == cat.get_attribute_int("money", -1))
	{
		string_t msg = "Сумма персональной оплаты не должна совпадать с оплатой категории.";
		msg += "\nP.S.: Для удаления персональной оплаты используйте кнопку 'Удалить'.";
		MessageBox(::GetActiveWindow(), msg, L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}

	// определим есть ли уже персональная оплата, если есть то просто изменим иначе создадим новую
	string_t query = "SELECT id FROM paypersonaltest "
		" WHERE idstud = " + studid + " AND idopts = "  + catid +
		" AND deleted = 0";
	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
	if (res.size() > 0)
	{
		string_t msg = "Вы действительно хотите изменить сумму персональной оплаты?"
			"\nP.S.: Для удаления персональной оплаты используйте кнопку 'Удалить'.";
		if (IDNO == MessageBox(::GetActiveWindow(), msg, L"Предупреждение", MB_YESNO | MB_ICONERROR | MB_APPLMODAL))
			return;

		string_t query = 
			" UPDATE paypersonaltest "
			" SET commoncountmoney = " + money + 
			" WHERE idstud = " + studid + " AND idopts = " + catid;

		theApp.GetCon().Query(query);
	}
	else
	{
		string_t msg = "Вы действительно хотите задать персональную оплату по данной категории?"
			"\nP.S.: Для удаления персональной оплаты используйте кнопку 'Удалить'.";
		if (IDNO == MessageBox(::GetActiveWindow(), msg, L"Предупреждение", MB_YESNO | MB_ICONERROR | MB_APPLMODAL))
			return;

		string_t query = 
			"INSERT INTO paypersonaltest "
			"(idstud,idopts,commoncountmoney,deleted) " 
			" VALUES(" + studid +", " + catid + ", " + money + ", 0)";
		theApp.GetCon().Query(query);
	}

	UpdateView();
}

// заполняет поле оплаты разными вариантами оплаты
void SPayment::InitPayPay(void)
{
	element cat	= GetCurrentCat();
	int money	= cat.get_attribute_int("money", 0);

	element option	= LiteWnd::link_element(_root, "pay-pay").find_first("popup");
	assert(option.is_valid());

	// удаляем все строки
  t::ClearTable(option, 0);

	string_t buf = "<option value=0>0</option>";;
	for (int i = 4; i > 0; --i)
		buf += string_t() + "<option value=" + aux::itow(money/i) + ">" + aux::itow(money/i) + "</option>";

	assert(_mbslen(buf));
	if (_mbslen(buf))
		option.set_html(buf, _mbslen(buf), SIH_REPLACE_CONTENT);
	option.update();

	json::t2v(LiteWnd::link_element(_root, "pay-pay"), "0");
}

void SPayment::ShowOnlyPay(void)
{
	// предположительно оплаты уже отображены
	// осталось тока удалить категории без оплат
	assert(_payment_table.is_valid());
	using namespace htmlayout::dom;
	std::deque<element> list;

	for (element cat = _payment_table.child(0); cat.is_valid(); cat = cat.next_sibling())
	{
		assert(aux::streq(cat.get_element_type(), "options"));
		if (cat.children_count() < 2)
			list.push_back(cat);
	}
	while(list.size())
	{
		list.front().destroy();
		list.pop_front();
	}
	_payment_table.update();
}
