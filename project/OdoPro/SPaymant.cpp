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

// �������������� ��� ������� � ��� �����
void SPayment::Init(htmlayout::dom::element root)
{
	_root			= root;
	_payment_table	= LiteWnd::link_element(_root, "payment-table"); 
	_cat_month		= LiteWnd::link_element(_root, "pay-cat-manth"); 
	_cat_year		= LiteWnd::link_element(_root, "pay-cat-year"); 
	_cat_money		= LiteWnd::link_element(_root, "pay-cat-money"); 
	_cat_half_year	= LiteWnd::link_element(_root, "pay-cat-half-year");
	
	// ���������� ����������� ������
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-save"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-del"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-cat-add"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-cat-del"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-cat-save"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-cat-per"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_root, "pay-show-only-pay"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);

	// ������������ ���������, �������������� ����� ������ ���������/������
	HTMLayoutAttachEventHandlerEx(_payment_table, ElementEventProcForPayTable, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
}


// ������������ ������
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

	MessageBox(::GetActiveWindow(), L"��� �� ��������", L"��������������", 0);
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
	// ������� ��� ������
  t::ClearTable(_payment_table, 0);

  // ���������� �� ������� ��������
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
		string_t inf = "<option idfact=" + pay_row["id"] + "><caption>���� �������: " + pay_row["datepay"] + 
			" ����� �������: " + pay_row["moneypay"] + "</caption></option>";
		info_pay[optid] = info_pay[optid] + inf; 
	}

	// ���������� � ���������� ������
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

		// ��������� ���� �� ������������ ��������� ������
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
			dolg = string_t() + "[����: " + (per_id == L"" ? aux::itow(aux::wtoi(info_row["commoncountmoney"]) - pay[optid]) : aux::itow(per_money - pay[optid])) + " ���.]";			
		buf += string_t() + "<options" + atr +"><caption>";
		buf += string_t() + period + " [����: " + info_row["cnt"] + "]";
		if (0 == aux::wtoi(info_row["half_year"]))
			buf += string_t() + "<br/>[����: ���]";
		else
			buf += string_t() + "<br/>[����: <font color='#9900FF'>��</font>" + "]";
		if (per_id == L"")
			buf += string_t() + "[�����: " + info_row["commoncountmoney"] + " ���.]";
		else
			buf += string_t() + "[�����: " + aux::itow(per_money) + "(" + info_row["commoncountmoney"] + ") ���.]";
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
	// �������� ��� ������
	for (uint i = 0; i < _payment_table.children_count(); ++i)
	{
		element option = _payment_table.child(i);
		if (option.children_count() > 1)
			option.set_state(STATE_EXPANDED);
	}
	///LOG_DEBUG << string_t(_payment_table.get_html());
}

// ������������ ����� ���������/������
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
// ��������� ���� �� ������� sql � ������  ��������� � �������
string_t SPayment::DateToPayFormat(const string_t& pay_date)
{
	// 2001-02-01
	std::vector<std::wstring> result = aux::split(std::wstring(pay_date), L'-');
	if (result[1] == L"02")
		return L"���. " + result[0];
	if (result[1] == L"09")
		return L"����. " + result[0];
	
	throw wss::exception(wss::reason_message(FULL_LOCATION()));
}

// ��������� ������ ��������
void SPayment::AddPay(void)
{
	if (!GetCurrentCat().is_valid())
	{
		MessageBox(::GetActiveWindow(), L"��� ��������� ������ �������� ��������� ������.", L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
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
	
	string_t msg = "���� ����������� ������."
		"\n�������(��):d\t\t" + theApp.GetFIO() + 
		"\n������:\t\t\t" + theApp.GetGroupName(theApp.GetCurrentGroupID()) + 
		"\n������ ������:\t\t" + period + 
		"\n���� ������:\t\t" + pay_date + 
		"\n����� ������ ������:\t" + pay_pay + " ���.";
	MessageBox(::GetActiveWindow(), msg, 
		L"���������", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
}

// ��������� ��������� ������ �� ������������
bool SPayment::CheckInputPay(void)
{
	string_t pay_date = json::v2t(LiteWnd::link_element(_root, "pay-date").get_value()); 
	string_t pay_pay  = json::get_caption(LiteWnd::link_element(_root, "pay-pay"));//json::v2t(LiteWnd::link_element(_root, "pay-pay").get_value());

	string_t msg;
	if (pay_date.empty())
		msg= "��������� ���� �� �������.\n��������� ����������.";
	if (aux::wtoi(pay_pay, -1) == -1)
		msg = "����� ������ ������ ���������� ��� ����� ������������� �����.\n��������� ����������.";
	if (!msg.empty())
	{
		MessageBox(::GetActiveWindow(), msg, 
			L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}
	
	element catpay	= GetCurrentCat(); // ����� ������ �� �������, ��������� � ������
	long	dolg	= catpay.get_attribute_int("dolg");

	if (aux::wtoi(pay_pay) > dolg)
		msg = string_t() + 
			"������������� �� ������ ���������� " + aux::itow(dolg) + " ���.\n"
			"�� ������ ���������� ������ � ������� " + pay_pay + " ���.\n"
			"����� �������� ���������� �����������, ��� ��������� ������������� ������� " + aux::itow(dolg) + 
			" ���. � �������� ����� ������� � ��������� ������ � ���� �������!";

	if (!msg.empty())
	{
		MessageBox(::GetActiveWindow(), msg, 
			L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}
	return true;
}

// ���������� �������� ��������� ������
element SPayment::GetCurrentCat(void)
{
	element find = _payment_table.find_first(":current");
	if (!find.is_valid())
	{
		return find; // ����� ����������
		//assert(FALSE);
		//throw wss::exception(wss::reason_message(FULL_LOCATION()));
	}
	// ���� ������, �� ������� �� ��������� 
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

// ������� ������
void SPayment::DeletePay(void)
{
	element find = _payment_table.find_first(":current");
	
	if (!aux::streq(find.get_element_type(), "option") || -1 == find.get_attribute_int("idfact", -1))
	{
		MessageBox(::GetActiveWindow(), L"��� �������� ������ �������� ���� ������.", L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}

	string_t msg = "�� ������������� ������ ������� ��� ������ �� ����� ������?\n"
		"������ ���������!";
	if (IDNO == MessageBox(::GetActiveWindow(), msg, L"��������������", MB_YESNO |MB_ICONINFORMATION | MB_APPLMODAL))
		return;
	
	string_t query = string_t() + 
		"UPDATE payfactstest SET deleted = 1 WHERE id = " + find.get_attribute("idfact");
	theApp.GetCon().Query(query);
	
	UpdateView();
}

// ��������� ���� � ��������� ���������
void SPayment::UpdateViewCat(void)
{
	element cat = GetCurrentCat();
	if (!cat.is_valid()) // ��� ������ ������ ��������� ��� �� �������
		return;

	std::wstring buf = cat.get_attribute("datestart"); 
	std::vector<std::wstring> date = aux::split(buf, L'-');

	json::t2v(_cat_month, date[1]);
	json::t2v(_cat_year, date[0]);
	json::t2v(_cat_money, cat.get_attribute("money"));
	json::t2v(_cat_half_year, cat.get_attribute("half_year"));	
}

// �������� ��������� ������
void SPayment::AddCat(void)
{
	if (IsExistCat())
	{
		string_t msg = "��������� ������ c ����� �������� ������ ��� ����������.\n"
			"�������� ��� ���� ��������� ������ ?";
		if (IDNO == MessageBox(::GetActiveWindow(), msg, L"���������", MB_YESNO | MB_ICONINFORMATION | MB_APPLMODAL))
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
		string_t msg = "����� ������ �� ����� ���� �������������.\n��������� ����������.";
		MessageBox(::GetActiveWindow(), msg, L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}

	string_t query = "INSERT INTO  payoptstest (idgroup, datestart, dateend, commoncountmoney, half_year, deleted) "
		" VALUES(" + grpid + ", '" + startdate + "', '" + enddate + "', " + money + ", " + half_year + ", 0)";
	theApp.GetCon().Query(query);
	
	UpdateView();
}

// ��������� ���������� �� ����� ��������� ������
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

// ���������� ���� ������ � ����� ������� ������
void SPayment::GetDateCat(string_t& startdate, string_t& enddate)
{
	string_t month	= json::v2t(_cat_month.get_value());
	string_t year	= json::v2t(_cat_year.get_value());

	startdate	= year + "-0" + month + "-01";
	enddate		= string_t(aux::itow(aux::wtoi(year) + 1)) + "-0" + month + "-01"; 
}

// ��������� ��������� ��������� ������
void SPayment::SaveUpdateCat(void)
{
	if (!GetCurrentCat().is_valid())
	{
		MessageBox(::GetActiveWindow(), L"��� ���������� ��������� �������� ��������� ������.", L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	if (IsExistCat(false))
	{
		string_t msg = "��������� ������ � ������ ����������� ��� ���������� � ���� ������.\n"
			"���������� �� ���������!";
		MessageBox(::GetActiveWindow(), msg, L"���������", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
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
		string_t msg = "����� ������ �� ����� ���� �������������.\n��������� ����������.";
		MessageBox(::GetActiveWindow(), msg, L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
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

// ������� ��������� ���������
void SPayment::DeleteCat(void)
{
	if (!GetCurrentCat().is_valid())
	{
		MessageBox(::GetActiveWindow(), L"��� �������� ��������� ������ ��� ������������ ��������� ������ �������� ��������� ������.", L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	int per_id = GetCurrentCat().get_attribute_int("per_id", -1);
	string_t msg;
	if (per_id == -1)
		msg = "�� ������������� ������ ������� ��������� ������?\n"
			  "���� ��� � �������� �������� ��������� ������ � ���������,\n"
		      "� ��� ������� ��������� �������� � ������ ������ � ���� �� ������� ��������� ���� �� �������������!\n"
		      "������ ���������!";
	else
		msg = "�� ������������� ������ ������� ������������ ��������� ������?";

	if (IDNO == MessageBox(::GetActiveWindow(), msg, L"��������������", MB_YESNO | MB_ICONERROR | MB_APPLMODAL))
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

// ������ ��� ��������� ��������� ����������� ������ ��������
void SPayment::PersonalCat()
{
	if (!GetCurrentCat().is_valid())
	{
		MessageBox(::GetActiveWindow(), L"��� �������� ������������ ������ �������� ������������ ��������� ������", L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	
	element  cat    = GetCurrentCat();
	string_t money  = json::v2t(_cat_money.get_value());
	string_t catid  = GetCurrentCat().get_attribute("optid");
	string_t studid	= aux::itow(theApp.GetCurrentStudentID());

	if (aux::wtoi(money, -1) < 0)
	{
		string_t msg = "����� ������ �� ����� ���� �������������.\n��������� ����������.";
		MessageBox(::GetActiveWindow(), msg, L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}
	if (aux::wtoi(money, -1) == cat.get_attribute_int("money", -1))
	{
		string_t msg = "����� ������������ ������ �� ������ ��������� � ������� ���������.";
		msg += "\nP.S.: ��� �������� ������������ ������ ����������� ������ '�������'.";
		MessageBox(::GetActiveWindow(), msg, L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return;
	}

	// ��������� ���� �� ��� ������������ ������, ���� ���� �� ������ ������� ����� �������� �����
	string_t query = "SELECT id FROM paypersonaltest "
		" WHERE idstud = " + studid + " AND idopts = "  + catid +
		" AND deleted = 0";
	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
	if (res.size() > 0)
	{
		string_t msg = "�� ������������� ������ �������� ����� ������������ ������?"
			"\nP.S.: ��� �������� ������������ ������ ����������� ������ '�������'.";
		if (IDNO == MessageBox(::GetActiveWindow(), msg, L"��������������", MB_YESNO | MB_ICONERROR | MB_APPLMODAL))
			return;

		string_t query = 
			" UPDATE paypersonaltest "
			" SET commoncountmoney = " + money + 
			" WHERE idstud = " + studid + " AND idopts = " + catid;

		theApp.GetCon().Query(query);
	}
	else
	{
		string_t msg = "�� ������������� ������ ������ ������������ ������ �� ������ ���������?"
			"\nP.S.: ��� �������� ������������ ������ ����������� ������ '�������'.";
		if (IDNO == MessageBox(::GetActiveWindow(), msg, L"��������������", MB_YESNO | MB_ICONERROR | MB_APPLMODAL))
			return;

		string_t query = 
			"INSERT INTO paypersonaltest "
			"(idstud,idopts,commoncountmoney,deleted) " 
			" VALUES(" + studid +", " + catid + ", " + money + ", 0)";
		theApp.GetCon().Query(query);
	}

	UpdateView();
}

// ��������� ���� ������ ������� ���������� ������
void SPayment::InitPayPay(void)
{
	element cat	= GetCurrentCat();
	int money	= cat.get_attribute_int("money", 0);

	element option	= LiteWnd::link_element(_root, "pay-pay").find_first("popup");
	assert(option.is_valid());

	// ������� ��� ������
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
	// ���������������� ������ ��� ����������
	// �������� ���� ������� ��������� ��� �����
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
