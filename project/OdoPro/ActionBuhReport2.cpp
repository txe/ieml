#include "stdafx.h"
#include "ActionBuhReport2.h"
#include "SingeltonApp.h"
#include "json-aux-ext.h"
#include <sys/timeb.h>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <map>

CActionBuhReport2::CActionBuhReport2(LiteWnd* parent /* = NULL */):LiteWnd(parent)
{
}

CActionBuhReport2::~CActionBuhReport2(void)
{
}

BOOL CActionBuhReport2::PreCreateWindow(CREATESTRUCT& cs)
{ 
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"Оплата на начало месяца";
	return TRUE;
}

int CActionBuhReport2::OnCreate()
{
	SetWindowPos(m_hWnd, NULL, -1, -1, 450, 450, 
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();
 
	PBYTE pb; DWORD cb;
	if (GetHtmlResource(L"IDR_HTML_BUH_REPORT_2", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	} 
	else
		assert(FALSE);

	InitDomElement();
	return 0;
}

void CActionBuhReport2::InitDomElement()
{
	FullGrpInLst();
	FullSpecInLst();

	HTMLayoutAttachEventHandlerEx(link_element("o-all"),     ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("o-spec"),    ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("o-grp"),     ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("o-cur-grp"), ElementEventProcFor, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

 	// присоединяем обоработчики к кнопкам
 	HTMLayoutAttachEventHandlerEx(link_element("bt-report"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
 	HTMLayoutAttachEventHandlerEx(link_element("bt-close"),  ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}

// обрабатывает кнопки
BOOL CALLBACK CActionBuhReport2::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CActionBuhReport2* dlg = static_cast<CActionBuhReport2*>(tag);
	const wchar_t*	   id  = htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-report"))
	{
		dlg->Report();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-close"))
	{
		dlg->Close();
		return TRUE;
	}

	return FALSE;
}

// обрабатывает выбор предмета/семестра
BOOL CALLBACK CActionBuhReport2::ElementEventProcFor(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd == BUTTON_CLICK)
	{
		CActionBuhReport2* dlg = static_cast<CActionBuhReport2*>(tag);
		dlg->StateChange();
		return TRUE;
	}
	
	return FALSE;
}

// отображает выбор
void CActionBuhReport2::StateChange()
{
	bool sel_spec = link_element("o-spec").get_state(STATE_CHECKED);
	link_element("spec-but").set_state(sel_spec ? 0 : STATE_DISABLED, !sel_spec ? 0 : STATE_DISABLED);
	bool sel_grp = link_element("o-grp").get_state(STATE_CHECKED);
	link_element("grp-but").set_state(sel_grp ? 0 : STATE_DISABLED, !sel_grp ? 0 : STATE_DISABLED);	
}

void CActionBuhReport2::FullGrpInLst()
{
	// don't forget check in CActionBuhReport - it's copy past
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

	string_t buf = "...<popup>"
		"<widget id=\"grp-lst\" type=\"select\"  style=\"width:180;height:300px;visibility:show\">";
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
	buf += "</widget></popup>";

	assert(_mbslen(buf));
	if (_mbslen(buf))
		link_element("grp-but").set_html(buf, _mbslen(buf), SIH_REPLACE_CONTENT);
	link_element("grp-but").set_state(STATE_DISABLED, 0);
}
void CActionBuhReport2::FullSpecInLst()
{
	string_t query = string_t() +
		" SELECT num , title, tag FROM voc " +
		" WHERE vkey= 'spec' AND deleted = 0 " +
		" ORDER BY title, tag, id";
	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

	string_t buf = "...<popup>"
		"<widget type=\"select\" id=\"spec-lst\" multiple=\"checks\" style=\"width: 100%%; height: 300px;visibility:show\">";
	mybase::MYFASTROW row;
	while (row = res.fetch_row())
	{
		string_t title = row["title"];
		string_t tag = row["tag"];
		if (!tag.empty())
			title += " (" + tag + ")";
		if (title.empty())
			title = "&nbsp;";
		buf += "<option value=" + row["num"] + ">" + title + "</option>";
	}
	buf += "</widget></popup>";

	assert(_mbslen(buf));
	if (_mbslen(buf))
		link_element("spec-but").set_html(buf, _mbslen(buf), SIH_REPLACE_CONTENT);
	link_element("spec-but").set_state(STATE_DISABLED, 0);
}

void CActionBuhReport2::GetYearMonthDay(std::wstring date1, int& year, int& month, int& day)
{
	std::vector<std::wstring> result = aux::split(date1, L'-');

	year  = aux::wtoi((result.size() > 0)?result[0].c_str():L"0", 0);
	month = aux::wtoi((result.size() > 1)?result[1].c_str():L"0", 0);
	day   = aux::wtoi((result.size() > 2)?result[2].c_str():L"0", 0);
}

int CActionBuhReport2::MonthCount(int studyYear, int studyMonth, int selectYear, int selectMonth)
{
  int count = 0;
  if (studyYear == selectYear)
    count = selectMonth - studyMonth;
  else if (selectYear > studyYear)
    count =  (12 - studyMonth) + selectMonth + (selectYear - studyYear - 1) * 12;
  else
    count = 0;

  if (count < 0)
    count = 0;
  return count;
}

void CActionBuhReport2::Report(void)
{
	string_t date1 = json::v2t(link_element("date-1").get_value());
    if (date1.empty())
	{
		MessageBox(m_hWnd, L"Не введена дата", L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
		return;
	}
	int xYear, xMonth, xDay;
	GetYearMonthDay(std::wstring(date1), xYear, xMonth, xDay);
	if (xDay != 1)
	{
		MessageBox(m_hWnd, L"Можно задавать только первый день месяца", L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
		return;
	}

	string_t year  = json::v2t(link_element("pay-year").get_value());
	int dYear  = aux::wtoi(year);

  // !!! ранее февральские планы делились пополам 6 и 6 месяцев
  // теперь они деляться на 7 и 5 месяцев

	// учебый год 2013-2014, найдем сколько месяцев попадет в отчет
	// на февр 13-14, начало половинки будет в сентябре 13 и до февраля, т.е. не более 5 месяцев
	int koef1 = MonthCount(dYear, 9, xYear, xMonth);
	// на сент 13-14, начало будет в сентябре 13 и до следующиего сентября, т.е. не более 12 месяцев, но для сокращ. не более 7
	int koef2 = MonthCount(dYear, 9, xYear, xMonth);
	// на февр 13-14, начало половинки будет в феврале 14 и до сентября, т.е. не более 7 месяцев
	int koef3 = MonthCount(dYear+1, 2, xYear, xMonth);

	if (link_element("o-spec").get_state(STATE_CHECKED))
		if (GetSpecLst("").empty())
		{
			MessageBox(m_hWnd, L"Выберете специальности", L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
			return;
		}

	if (link_element("o-grp").get_state(STATE_CHECKED))
		if (GetGrpLst("").empty())
		{
			MessageBox(m_hWnd, L"Выберете группы", L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
			return;
		}

	CreateBuhData(koef1, koef2, koef3);
}

//---------------------------------------------------------------------------
void CActionBuhReport2::CreateBuhData(int koef1, int koef2, int koef3)
{
	theApp.GetCon().Query("drop temporary table if exists full_table");
	theApp.GetCon().Query(
		"CREATE TEMPORARY TABLE full_table "
		" ( "
		"  idstud    int(11), "
		"  grpid     int(11), "
		"  specid    int(11), "
		"  plan      DECIMAL DEFAULT '0', "
		"  pay       int(11) DEFAULT '0', "
		"  dolg      DECIMAL DEFAULT '0', "
		"  pere      DECIMAL DEFAULT '0', "
		"  remove    int(11) DEFAULT '1', "
		"  INDEX (idstud), "
		"  INDEX (grpid) "
		" ) TYPE = HEAP ");

	// найдем студентов, или все, или с указанными специальностями, или с указанными группами
	// сделаем группировки т.к. может быть несколько план за один период
	if (link_element("o-all").get_state(STATE_CHECKED))
		theApp.GetCon().Query("INSERT full_table (idstud,grpid,specid) "
			" SELECT studs.id, studs.grpid, studs.specid"
			" FROM students AS studs, voc as v "
			" WHERE studs.deleted = 0 AND studs.grpid=v.num "
			" AND v.vkey='grp' AND v.deleted=0 "
			" GROUP BY studs.id ");
	else if (link_element("o-spec").get_state(STATE_CHECKED))
		theApp.GetCon().Query("INSERT full_table (idstud,grpid,specid) "
		" SELECT studs.id, studs.grpid, studs.specid"
		" FROM students AS studs, voc as v "
		" WHERE studs.deleted = 0 AND " + GetSpecLst("studs") + " "
		" AND studs.grpid=v.num  AND v.vkey='grp' AND v.deleted=0 "
		" GROUP BY studs.id ");
	else if (link_element("o-grp").get_state(STATE_CHECKED))
		theApp.GetCon().Query("INSERT full_table (idstud,grpid,specid) "
		" SELECT studs.id, studs.grpid, studs.specid"
		" FROM students AS studs "
		" WHERE studs.deleted = 0 AND " + GetGrpLst("studs") +
		" GROUP BY studs.id ");
	else if (link_element("o-cur-grp").get_state(STATE_CHECKED))
		theApp.GetCon().Query("INSERT full_table (idstud,grpid,specid) "
		" SELECT studs.id, studs.grpid, studs.specid"
		" FROM students AS studs "
		" WHERE studs.deleted = 0 AND studs.grpid=" + string_t(aux::itow(theApp.GetCurrentGroupID())) + 
		" GROUP BY studs.id ");

	ProcessPlan(koef1, koef2, koef3);   // расчитаем план для учебного года
	ProcessPay();         // оплата на 01 число за текущий год

	// удалим из основной таблицы группы (студентов) которые не имеют категорий оплаты
	theApp.GetCon().Query(" DELETE FROM full_table "
						  " WHERE full_table.remove = 1 ");

	// расчитаем долг и переплату
	theApp.GetCon().Query(" UPDATE full_table SET dolg = plan - pay WHERE plan > pay");
	theApp.GetCon().Query(" UPDATE full_table SET pere = pay - plan WHERE plan < pay");

	// для групп, в которых отчисленные, уберем план и долг, но сохраним оплаты
	theApp.GetCon().Query(" UPDATE full_table as f, voc as v "
		" SET f.plan = 0, f.dolg = 0, f.pere = 0 "
		" WHERE f.grpid = v.num AND v.deleted = 0 "
		" AND v.vkey='grp' AND title like '%отчисл%'");

	// удалим тех, кто не учиться в этом году
	//theApp.GetCon().Query(" DELETE FROM full_table WHERE plan = 0");

	// получим значения из таблицы
	// посчитаем и запомним итоги для каждой специальности
	std::map<std::wstring, std::wstring> spec_itog;
	mybase::MYFASTRESULT res =  theApp.GetCon().Query(
		"SELECT specid, SUM(plan) as plan, SUM(pay) as pay, SUM(dolg) as dolg, SUM(pere) as pere "
		" FROM full_table as f "
		" GROUP BY specid ");
	mybase::MYFASTROW row;
	while (row = res.fetch_row())
		spec_itog.insert(std::make_pair(row["specid"], row["plan"] + "\t" + row["pay"] + "\t" + row["dolg"] + "\t" + row["pere"]));
	bool set_grp = link_element("field-grp").get_state(STATE_CHECKED);
	bool set_fio = link_element("field-fio").get_state(STATE_CHECKED) && !link_element("field-fio").get_state(STATE_DISABLED);

	if (!set_grp && !set_fio)
	{
		res = theApp.GetCon().Query(
			" SELECT f.specid as specid, v.title as spec, v.tag as tag , SUM(f.plan) as plan , SUM(f.pay) as pay, SUM(f.dolg) as dolg, SUM(f.pere) as pere "
			" FROM full_table as f, voc as v "
			" WHERE v.vkey='spec' AND v.num = f.specid AND v.deleted = 0 "
			" GROUP BY f.specid "
			" ORDER BY v.title, v.tag, f.specid " );
	}
	else if (!set_fio)
	{
		res = theApp.GetCon().Query(
			" SELECT f.specid as specid, v.title as spec, v.tag as tag, v2.title as grp, SUM(f.plan) as plan, SUM(f.pay) as pay, SUM(f.dolg) as dolg, SUM(f.pere) as pere "
			" FROM full_table as f, voc as v, voc as v2 "
			" WHERE v.vkey='spec' AND v.num = f.specid AND v.deleted = 0 "
			" AND v2.vkey='grp' AND v2.num = f.grpid AND v2.deleted = 0 "
			" GROUP BY f.specid, f.grpid "
			" ORDER BY v.title, v.tag, f.specid, v2.title " );
	}
	else // set_grp && set_fio
	{	
		res = theApp.GetCon().Query(
			" SELECT f.specid as specid, v.title as spec, v.tag as tag, v2.title as grp, CONCAT_WS(' ', s.secondname, s.firstname, s.thirdname) as fio, f.plan as plan, f.pay as pay, f.dolg as dolg, f.pere as pere"
			" FROM full_table as f, voc as v, voc as v2, students as s "
			" WHERE v.vkey='spec' AND v.num = f.specid AND v.deleted = 0 "
			" AND v2.vkey='grp' AND v2.num = f.grpid AND v2.deleted = 0 "
			" AND s.id = f.idstud "
			" ORDER BY v.title, v.tag, f.specid, v2.title, fio " );
	}
	
	string_t text;
	if (!set_grp && !set_fio)	text += "Специальность\tПлан оплаты\tФакт оплаты\tДолг\tПереплата\n";
	else if (!set_fio)			text += "Специальность\tГруппа\tПлан оплаты\tФакт оплаты\tДолг\tПереплата\n";
	else						text += "Специальность\tГруппа\tФИО\tПлан оплаты\tФакт оплаты\tДолг\tПереплата\n";
	
	std::wstring old_spec_id = L"";
	while (row = res.fetch_row())
	{
		// если пошла новая специальность, то подведем  итоги о старой
		if (old_spec_id == L"")
			old_spec_id = row["specid"];
		if (old_spec_id != row["specid"])
		{
			string_t txt;
			std::map<std::wstring, std::wstring>::iterator it = spec_itog.find(old_spec_id);
			if (it != spec_itog.end())
				txt = it->second;
			if (!set_grp && !set_fio)	text += "итого по специальности\t" + txt + "\n";
			else if (!set_fio)			text += "итого по специальности\t\t" + txt + "\n";
			else						text += "итого по специальности\t\t\t" + txt + "\n";
			old_spec_id = row["specid"];
		}

		// 
		string_t spec = row["spec"];
		string_t tag  = row["tag"];
		if (!tag.empty())
			spec += " (" + tag + ")";
		if (!set_grp && !set_fio)	text += spec + "\t" + row["plan"] + "\t" + row["pay"] + "\t" + row["dolg"] + "\t" + row["pere"] + "\n";
		else if (!set_fio)			text += spec + "\t" + row["grp"] + "\t" + row["plan"] + "\t" + row["pay"] + "\t" + row["dolg"] + "\t" + row["pere"] + "\n";
		else						text += spec + "\t" + row["grp"] + "\t" + row["fio"] + "\t" + row["plan"] + "\t" + row["pay"] + "\t" + row["dolg"] + "\t" + row["pere"] + "\n";
    }
	// подведем итоги о последней специальности
	string_t txt;
	std::map<std::wstring, std::wstring>::iterator it = spec_itog.find(old_spec_id);
	if (it != spec_itog.end())
		txt = it->second;
	if (!set_grp && !set_fio)	text += "итого по специальности\t" + txt + "\n";
	else if (!set_fio)			text += "итого по специальности\t\t" + txt + "\n";
	else						text += "итого по специальности\t\t\t" + txt + "\n";

	// подведем итоги по универу
	if (link_element("o-all").get_state(STATE_CHECKED))
	{
		mybase::MYFASTRESULT res =  theApp.GetCon().Query("SELECT SUM(plan) as plan, SUM(pay) as pay, SUM(dolg) as dolg, SUM(pere) as pere  FROM full_table");
		string_t txt;
		while (mybase::MYFASTROW row = res.fetch_row())
			txt = row["plan"] + "\t" + row["pay"] + "\t" + row["dolg"] + "\t" + row["pere"];
		if (!set_grp && !set_fio)	text += "всего по институту\t" +  txt;
		else if (!set_fio)			text += "всего по институту\t\t" + txt;
		else						text += "всего по институту\t\t\t" + txt;
	}

	theApp.GetCon().Query("drop temporary table if exists full_table");

	// создадим имя файла
	__timeb64 tstruct;
	_ftime64(&tstruct);
	tm lTime = *_localtime64(&tstruct.time);
	std::wostringstream name;
	name << L"Buh_report_2_"
		 << L"_"
		 << (1900+lTime.tm_year)
		 << std::setiosflags(std::ios::right) << std::setfill(L'0')
		 << std::setw(2) << (1+lTime.tm_mon)
		 << std::setw(2) << lTime.tm_mday
		 << L"_"
		 << std::setw(2) << lTime.tm_hour
		 << std::setw(2) << lTime.tm_min
		 << std::setw(2) << lTime.tm_sec
		 << L".xls.prn";
	HANDLE hfile = CreateFile(name.str().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwBytesWritten;
	WriteFile(hfile, text.c_str(), text.size(), &dwBytesWritten, NULL);
	CloseHandle(hfile);

	ShellExecute(NULL, L"open", name.str().c_str(), NULL, NULL, SW_SHOWNORMAL);
}

string_t maxTo(int monthCount, int maxCount)
{
  if (monthCount > maxCount)
    return string_t(aux::itow(maxCount));
  return string_t(aux::itow(monthCount));
}


void CActionBuhReport2::ProcessPlan(int koef1, int koef2, int koef3)
{
	// соберем оплаты по каждой категории
	theApp.GetCon().Query("drop temporary table if exists old_pay");
	theApp.GetCon().Query(
		"CREATE TEMPORARY TABLE old_pay "
		" ( "
		"  id        int(11) NOT NULL AUTO_INCREMENT, "
		"  idstud    int(11) NOT NULL, "
		"  idopt     int(11) NOT NULL, "
		"  plan      DECIMAL NOT NULL, "
		"  half_year int(1)  NOT NULL, "
		"  pay       int(11) NOT NULL, "
		"  datestart date    NOT NULL, "
		"  type      char(1) NOT NULL, "
		"  INDEX (id),     "
		"  INDEX (idstud)  "
		" ) TYPE = HEAP ");

	// зададим даты
	string_t first  = json::v2t(link_element("pay-year").get_value());
	string_t second = aux::itow(aux::wtoi(first) + 1);
	// зададим даты периода 2013-2014
	string_t d1 = "'" + first + "-02-01'";  // 2013-02-01
	string_t d2 = "'" + first + "-09-01'";  // 2013-09-01
	string_t d3 = "'" + second + "-02-01'"; // 2014-02-01

	//# это обычная сентябрьская оплата
	theApp.GetCon().Query("INSERT old_pay (idstud, idopt, datestart, plan, half_year, pay, type)       "
		" SELECT s.idstud, s.idopt, " + d2 + ",s.commoncountmoney, s.half_year, SUM(COALESCE(fact.moneypay, 0)), 'a'      "
		" FROM (                                                                                  "
		"      SELECT st.idstud as idstud, opts.id as idopt, opts.commoncountmoney, opts.half_year	  "
		"      FROM full_table AS st, payoptstest as opts										  "
		"      WHERE opts.deleted = 0 AND opts.idgroup = st.grpid						          "
		"      AND opts.datestart= " + d2 + "  							                          "
		" ) as s                                                                                  "
		" LEFT JOIN payfactstest AS fact                                                          "
		" ON s.idopt = fact.idopts AND s.idstud = fact.idstud AND fact.deleted = 0                "
		" GROUP BY s.idstud, s.idopt");
	// проверим на наличие персональных категорий оплат
	theApp.GetCon().Query("UPDATE old_pay, paypersonaltest as p "
		" SET old_pay.plan = p.commoncountmoney "
		" WHERE old_pay.type = 'a' AND p.idstud = old_pay.idstud "
		" AND p.idopts = old_pay.idopt AND p.deleted = 0 ");

	// поправим план
  if (koef2 <= 0)
    theApp.GetCon().Query("UPDATE old_pay SET old_pay.plan = 0 WHERE old_pay.type = 'a'");
  else
  {
    // на сент 13-14, начало будет в сентябре 13 и до следующиего сентября, т.е. не более 12 месяцев, но для сокращ. не более 5
    theApp.GetCon().Query("UPDATE old_pay SET old_pay.plan = (old_pay.plan * " + maxTo(koef2, 12) + " / 12.0) WHERE old_pay.type = 'a' AND old_pay.half_year = 0");
    theApp.GetCon().Query("UPDATE old_pay SET old_pay.plan = (old_pay.plan * " + maxTo(koef2, 5) + " / 5.0) WHERE old_pay.type = 'a' AND old_pay.half_year = 1");
  }


	//# это февральские предыдущие
	theApp.GetCon().Query("INSERT old_pay (idstud, idopt, datestart, plan, half_year, pay, type)             "
		" SELECT s.idstud, s.idopt, " + d1 + ",s.commoncountmoney, s.half_year, SUM(COALESCE(fact.moneypay, 0)), 'b'    "
		" FROM (                                                                                  "
		"      SELECT st.idstud as idstud, opts.id as idopt, opts.commoncountmoney, opts.half_year "
		"      FROM full_table AS st, payoptstest as opts										  "
		"	   WHERE opts.deleted = 0 AND opts.idgroup = st.grpid						          "
		"      AND opts.datestart=" + d1 + "						                              "
		" ) as s                                                                                  "
		" LEFT JOIN payfactstest AS fact                                                          "
		" ON s.idopt = fact.idopts AND s.idstud = fact.idstud AND fact.deleted = 0                "
		" GROUP BY s.idstud, s.idopt");
	// проверим на наличие персональных категорий оплат
	theApp.GetCon().Query("UPDATE old_pay, paypersonaltest as p "
		" SET old_pay.plan = p.commoncountmoney "
		" WHERE old_pay.type = 'b' AND p.idstud = old_pay.idstud "
		" AND p.idopts = old_pay.idopt AND p.deleted = 0 ");
	
  // уберем план для сокращенных февральских, т.к. у них нет подходящей половинки 
	theApp.GetCon().Query("UPDATE old_pay SET old_pay.plan = 0 WHERE old_pay.type = 'b' AND old_pay.half_year = 1 ");
  //скоректируем выплаты, они должны остаться только для второй половике
  theApp.GetCon().Query("UPDATE old_pay SET old_pay.pay = old_pay.pay - (old_pay.plan * 7.0 / 12.0) WHERE old_pay.type = 'b' ");
  //сделаем для них проверку что бы не было отрицательных оплат
  theApp.GetCon().Query("UPDATE old_pay SET old_pay.pay = 0 WHERE old_pay.pay < 0 AND old_pay.type = 'b' ");

  // поправим план
  if (koef1 <= 0) // от 0 до 5
    theApp.GetCon().Query("UPDATE old_pay SET old_pay.plan = 0 WHERE old_pay.type = 'b'");
  else
  {
    // для полн. не более 5 месяцев, а сокращ. вообще не должны попасть
    theApp.GetCon().Query("UPDATE old_pay SET old_pay.plan = (old_pay.plan * " + maxTo(koef1, 5) + " / 12.0) WHERE old_pay.type = 'b'");
  }

	//# это февральские последующие
	theApp.GetCon().Query("INSERT old_pay (idstud, idopt, datestart, plan, half_year, pay, type)            "
		" SELECT s.idstud, s.idopt," + d3+ ", s.commoncountmoney, s.half_year, SUM(COALESCE(fact.moneypay, 0)), 'c'    "
		" FROM (                                                                                  "
		"      SELECT st.idstud as idstud, opts.id as idopt, opts.commoncountmoney, opts.half_year "
		"      FROM full_table AS st, payoptstest as opts                                         "
		"	   WHERE opts.deleted = 0 AND opts.idgroup = st.grpid AND opts.datestart=" + d3 + "	  "
		" ) as s                                                                                  "
		" LEFT JOIN payfactstest AS fact                                                          "
		" ON s.idopt = fact.idopts AND s.idstud = fact.idstud AND fact.deleted = 0                "
		" GROUP BY s.idstud, s.idopt");
	// проверим на наличие персональных категорий оплат
	theApp.GetCon().Query("UPDATE old_pay, paypersonaltest as p "
		" SET old_pay.plan = p.commoncountmoney "
		" WHERE old_pay.type = 'c' AND p.idstud = old_pay.idstud "
		" AND p.idopts = old_pay.idopt AND p.deleted = 0 ");

  // поправим план
  if (koef3 <= 0)
    theApp.GetCon().Query("UPDATE old_pay SET old_pay.plan = 0 WHERE old_pay.type = 'c'");
  else
  {
    // для полного и сокращ. не может быть больше 7 месяцев
    theApp.GetCon().Query("UPDATE old_pay SET old_pay.plan = (old_pay.plan * " + maxTo(koef3, 7) + " / 12.0) WHERE old_pay.type = 'c' AND old_pay.half_year = 0");
    theApp.GetCon().Query("UPDATE old_pay SET old_pay.plan = (old_pay.plan * " + maxTo(koef3, 7) + " / 7.0) WHERE old_pay.type = 'c' AND old_pay.half_year = 1");
  }

  //# сделаем для них проверку что бы не было оплаты больше плана
  theApp.GetCon().Query("UPDATE old_pay SET old_pay.pay = old_pay.plan WHERE old_pay.pay > old_pay.plan AND old_pay.type = 'c' ");


	// этап 2
	// у стедента могут несколько категорий оплат за текущий период (у февр два периода), то есть два сценария поведения
	// 1 или если хотя бы по одной было заплачено(может быть несколько), то удалить те которые без оплаты
	// 2 или если ни по одной не заплачено, то оставить только ту которая основная в группе

	// сдесь исправил, у ферв пред и послед всегда будет несколько категорий, и если по ней небудет заплачено
	// то она удалится

	// найдем студентов у которых проблемы
	theApp.GetCon().Query("drop temporary table if exists bad_stud");
	theApp.GetCon().Query(
		" CREATE TEMPORARY TABLE bad_stud                  "
		" (                                                "
		"  id        int(11) NOT NULL AUTO_INCREMENT,      "
		"  idstud    int(11) NOT NULL,                     "
		"  datestart date    NOT NULL,                     "
		"  _count    int(11) NOT NULL,                     "
		"  pay       int(11) NOT NULL,                     "
		"  INDEX (id),                                     "
		"  INDEX (idstud)                                  "
		" ) TYPE = HEAP ");
	theApp.GetCon().Query("INSERT bad_stud (idstud, datestart, _count, pay) "
		" SELECT idstud, datestart, COUNT(0) as c, SUM(pay)           "
		" FROM old_pay                                     "
		" GROUP BY idstud, datestart "
		" HAVING c > 1");

	// найдем котагории которые являются для проблемных студентов основными
	theApp.GetCon().Query("drop temporary table if exists main_opt");
	theApp.GetCon().Query(
		" CREATE TEMPORARY TABLE main_opt                  "
		" (                                                "
		"  id        int(11) NOT NULL AUTO_INCREMENT,      "
		"  grpid     int(11) NOT NULL,                     "
		"  idopt     int(11) NOT NULL,                     "
		"  count     int(11) NOT NULL,"
		"  INDEX (id),                                     "
		"  INDEX (grpid)                                   "
		" ) TYPE = HEAP ");
	theApp.GetCon().Query("INSERT main_opt (grpid, idopt, count)  "
		" SELECT m.grpid, m.idopt, MAX(m.c)               "
		" FROM (SELECT s.grpid, pay.idopt, COUNT(*) as c  "
		"       FROM old_pay as pay, full_table as s      "
		"       WHERE pay.idstud = s.idstud AND pay.pay > 0   "
		"       GROUP BY s.grpid, pay.idopt               "
		"      ) as m                                     "
		" GROUP BY m.grpid");

	// или сценарий 1 ( если хотя бы по одной было заплачено(может быть несколько), то удалить те которые без оплаты)
	theApp.GetCon().Query(" DELETE old_pay                "
		" FROM old_pay, bad_stud                  "
		" WHERE old_pay.idstud = bad_stud.idstud  "
		" AND old_pay.datestart = bad_stud.datestart "
		" AND bad_stud.pay > 0 AND old_pay.pay = 0");
	// или сценарий 2 (если ни по одной не заплачено, то оставить только ту которая основная в группе)
	theApp.GetCon().Query(" DELETE old_pay                     "
		" FROM old_pay, bad_stud, full_table as s, main_opt    "
		" WHERE old_pay.idstud = bad_stud.idstud               "
		" AND old_pay.datestart = bad_stud.datestart           "
		" AND bad_stud.pay = 0 AND s.idstud = bad_stud.idstud  "
		" AND main_opt.grpid = s.grpid                         "
		" AND old_pay.idopt != main_opt.idopt ");

	theApp.GetCon().Query("drop temporary table if exists bad_stud");
	theApp.GetCon().Query("drop temporary table if exists main_opt");

	// у студента может быть несколько категорий оплат
	theApp.GetCon().Query("drop temporary table if exists plan");
	theApp.GetCon().Query(
		"CREATE TEMPORARY TABLE plan "
		" ( "
		"  id        int(11) NOT NULL AUTO_INCREMENT, "
		"  idstud    int(11) NOT NULL, "
		"  plan      DECIMAL NOT NULL, "
		"  INDEX (id),     "
		"  INDEX (idstud)  "
		" ) TYPE = HEAP ");
	theApp.GetCon().Query(" INSERT plan (idstud, plan)  "
		" SELECT pay.idstud, SUM(pay.plan - 0)	"
		" FROM old_pay AS pay					"
		" GROUP BY pay.idstud ");


	// перенесем в основную таблицу и пометим что эти студенты попали в вычисления
	theApp.GetCon().Query(" UPDATE full_table, plan "
		" SET full_table.plan = plan.plan, full_table.remove = 0 "
		" WHERE full_table.idstud = plan.idstud");

	theApp.GetCon().Query("drop temporary table if exists plan");
	theApp.GetCon().Query("drop temporary table if exists old_pay");
}

void CActionBuhReport2::ProcessPay()
{
	// т.к. у студентов может оказаться несколько категорий оплат на один период
	// то сделаем в два этапа
	theApp.GetCon().Query("drop temporary table if exists pay1");
	theApp.GetCon().Query(
		"CREATE TEMPORARY TABLE pay1 "
		" ( "
		"  id        int(11) NOT NULL AUTO_INCREMENT, "
		"  idstud    int(11) NOT NULL, "
		"  idopt     int(11) NOT NULL, "
		"  plan      int(11) NOT NULL, "
		"  half_year int(1)  NOT NULL, "
		"  pay       int(11) NOT NULL, "
		"  type      char(1) NOT NULL, "
		"  INDEX (id),     "
		"  INDEX (idstud)  "
		" ) TYPE = HEAP ");


	// зададим даты
	string_t first  = json::v2t(link_element("pay-year").get_value());
	string_t second = aux::itow(aux::wtoi(first) + 1);
	// зададим даты для текущего периода 2013-2014
	string_t d1 = "'" + first + "-02-01'";    // 2013-02-01
	string_t d2 = "'" + first + "-09-01'";    // 2013-09-01
	string_t d3 = "'" + second + "-02-01'";   // 2014-02-01
	// дата, по которую требуется отчет
	string_t data1 = json::v2t(link_element("date-1").get_value());

	//# это обычная сентябрьская оплата
	theApp.GetCon().Query("INSERT pay1 (idstud, idopt, plan, half_year, pay, type)            "
		" SELECT s.idstud, s.idopt, s.commoncountmoney, s.half_year, SUM(COALESCE(fact.moneypay, 0)), 'a'"
		" FROM (                                                                   "
		"      SELECT st.idstud as idstud, opts.id as idopt, opts.commoncountmoney, opts.half_year "
		"      FROM full_table AS st, payoptstest as opts                          "
		"      WHERE opts.deleted = 0 AND opts.idgroup = st.grpid				   "
		"      AND opts.datestart=" + d2 + "                                       "
		" ) as s                                                                   "
		" LEFT JOIN payfactstest AS fact                                           "
		" ON s.idopt = fact.idopts AND s.idstud = fact.idstud AND fact.deleted = 0 "
		" AND fact.datepay <= '" + t::date2t(data1) + "' "  
		" GROUP BY s.idstud, s.idopt");
	// проверим на наличие персональных категорий оплат
	theApp.GetCon().Query("UPDATE pay1, paypersonaltest as p "
		" SET pay1.plan = p.commoncountmoney "
		" WHERE pay1.type = 'a' AND p.idstud = pay1.idstud "
		" AND p.idopts = pay1.idopt AND p.deleted = 0 ");


	//# это февральские предыдущие
	theApp.GetCon().Query("INSERT pay1 (idstud, idopt, plan, half_year, pay, type)                    "
		" SELECT s.idstud, s.idopt, s.commoncountmoney, s.half_year, SUM(COALESCE(fact.moneypay, 0)), 'b'  "
		" FROM (                                                                    "
		"      SELECT st.idstud as idstud, opts.id as idopt, opts.commoncountmoney, opts.half_year  "
		"      FROM full_table AS st, payoptstest as opts                           "
		"	     WHERE opts.deleted = 0 AND opts.idgroup = st.grpid                 "
		"      AND opts.datestart=" + d1 + "                                        "
		" ) as s                                                                    "
		" LEFT JOIN payfactstest AS fact                                            "
		" ON s.idopt = fact.idopts AND s.idstud = fact.idstud AND fact.deleted = 0 " 
		" AND fact.datepay <= '" + t::date2t(data1) + "' "  
		" GROUP BY s.idstud, s.idopt");
	// проверим на наличие персональных категорий оплат
	theApp.GetCon().Query("UPDATE pay1, paypersonaltest as p "
		" SET pay1.plan = p.commoncountmoney "
		" WHERE pay1.type = 'b' AND p.idstud = pay1.idstud "
		" AND p.idopts = pay1.idopt AND p.deleted = 0 ");

	// уполовиним план, но если это сокращенный год, то этого делать не надо , здесь правильно что 7.0
	theApp.GetCon().Query("UPDATE pay1 SET pay1.plan = (pay1.plan * 7.0 / 12.0)  WHERE pay1.type = 'b' AND pay1.half_year = 0 ");
	//скоректируем выплаты
	theApp.GetCon().Query("UPDATE pay1 SET pay1.pay = pay1.pay - pay1.plan WHERE pay1.type = 'b' ");
	// сделаеим для них проверку что бы не было отрицательных оплат
	theApp.GetCon().Query("UPDATE pay1 SET pay1.pay = 0 WHERE pay1.pay < 0 AND pay1.type = 'b' ");


	//# это февральские последующие
	theApp.GetCon().Query("INSERT pay1 (idstud, idopt, plan, half_year, pay, type)                    "
		" SELECT s.idstud, s.idopt, s.commoncountmoney, s.half_year, SUM(COALESCE(fact.moneypay, 0)), 'c'  "
		" FROM (                                                                    "
		"      SELECT st.idstud as idstud, opts.id as idopt, opts.commoncountmoney, opts.half_year  "
		"      FROM full_table AS st, payoptstest as opts                           "
		"	     WHERE opts.deleted = 0 AND opts.idgroup = st.grpid                 "
		"      AND opts.datestart=" + d3 + "                                        "
		" ) as s                                                                    "
		" LEFT JOIN payfactstest AS fact                                            "
		" ON s.idopt = fact.idopts AND s.idstud = fact.idstud AND fact.deleted = 0 " 
		" AND fact.datepay <= '" + t::date2t(data1) + "' "  
		" GROUP BY s.idstud, s.idopt");
	// проверим на наличие персональных категорий оплат
	theApp.GetCon().Query("UPDATE pay1, paypersonaltest as p "
		" SET pay1.plan = p.commoncountmoney "
		" WHERE pay1.type = 'c' AND p.idstud = pay1.idstud "
		" AND p.idopts = pay1.idopt AND p.deleted = 0 ");
	// уполовиним план, но если это сокращенный год, то этого делать не надо, здесь правильно что 7.0
	theApp.GetCon().Query("UPDATE pay1 SET pay1.plan = (pay1.plan * 7.0 / 12.0) WHERE pay1.type = 'c' AND pay1.half_year = 0 ");
	// сделаим для них проверку что бы не было оплаты больше плана
	theApp.GetCon().Query("UPDATE pay1 SET pay1.pay = pay1.plan WHERE pay1.pay > pay1.plan AND pay1.type = 'c' ");


	// может быть несколько оплат
	theApp.GetCon().Query("drop temporary table if exists pay2");
	theApp.GetCon().Query(
		" CREATE TEMPORARY TABLE pay2                 "
		" (                                           "
		"  id        int(11) NOT NULL AUTO_INCREMENT, "
		"  idstud    int(11) NOT NULL,                "
		"  pay       int(11) NOT NULL,                "
		"  INDEX (id),                                "
		"  INDEX (idstud)                             "
		" ) TYPE = HEAP ");
	theApp.GetCon().Query("INSERT pay2  (idstud, pay) "
		" SELECT pay1.idstud, SUM(pay1.pay)         "
		" FROM pay1                                 "
		" GROUP BY pay1.idstud ");

	// перенесем в основную таблицу
	theApp.GetCon().Query(" UPDATE full_table, pay2 "
		" SET full_table.pay = pay2.pay "
		" WHERE full_table.idstud = pay2.idstud");


	theApp.GetCon().Query("drop temporary table if exists pay1");
	theApp.GetCon().Query("drop temporary table if exists pay2");
}

string_t CActionBuhReport2::GetSpecLst(string_t name)
{
	std::vector<string_t> v;

	htmlayout::dom::element lst = link_element("spec-lst");
	for (UINT i = 0; i < lst.children_count(); ++i)
	{
		htmlayout::dom::element t = lst.child(i);
		if (t.get_state(STATE_CHECKED))
			v.push_back(t.get_attribute("value"));
	}	
	if (v.empty())
		return "";
	
	string_t buf = " ( ";
	for (size_t i = 0; i < v.size()-1; ++i)
		buf += " " + name + ".specid=" + v[i] + " or ";
	buf += " " + name + ".specid=" + v.back() + " ) ";
	
	return buf;
}

string_t CActionBuhReport2::GetGrpLst(string_t name)
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

	if (checker.v.empty())
		return "";

	string_t buf = " ( ";
	for (size_t i = 0; i < checker.v.size()-1; ++i)
		buf += " " + name + ".grpid=" + checker.v[i] + " or ";
	buf += " " + name + ".grpid=" + checker.v.back() + " ) ";

	return buf;
}
