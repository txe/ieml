#include "stdafx.h"
#include "SingeltonApp.h"
#include "LiteWnd.h"
#include "SData.h"
#include <sstream>
#include "json-aux-ext.h"
#include "string_t.h"

/* ��� ������� ��� �����������
1. ����������� ������������� �������� �������� (�����)
2. ������������� ������ �������
3. �������� �� �����������, ���������
// ����� ����������
4. ���� ���������� ������, �������� ����� ������
5. ���� ���������� �������������, �� �������� ����� ������	
*/

SData::SData(void)
{
}

SData::~SData(void)
{
}

// ���� ��� �������� �� ������� � ��������� �� � ������
BOOL CALLBACK SData::CallbackAtachElement(HELEMENT he, LPVOID param)
{
	SData* data = static_cast<SData*>(param);

	htmlayout::dom::element el = he;
	assert(el.get_attribute("id"));

	typedef std::pair<string_t, htmlayout::dom::element> PairMap;
	data->map_elements_.insert(PairMap(string_t(el.get_attribute("id")), el));

	return false;
};

// �������������� ��� ������� � ��� �����
void SData::Init(htmlayout::dom::element root)
{
	root_ = root;
	sel_egediscip_		= LiteWnd::link_element(root_, "sel-egediscip");
	table_ege_discip_	= LiteWnd::link_element(root_, "table-ege-discip");
	ege_ball_			= LiteWnd::link_element(root_, "ege-ball");

	// ���� ��� �������� �� ������� � ��������� �� � ������
	map_elements_.clear();
	HTMLayoutSelectElements(root_, "*[class=my]", CallbackAtachElement, this);

	// ��������� ������ ������������ �������
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "educationid"),	"education");
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "edudocid"),		"edudoc");
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "languageid"),	"language");
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "grpid"),			"grp");
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "specid"),		"spec", true);
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "eduformid"),		"eduform");
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "dogshifrid"),	"dogshifr");
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "dogfastid"),		"dogfast");
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "dogyearid"),		"dogyear");
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "cityid"),		"city");
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "sel-egediscip"),	"egediscip");
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "citizenryid"),	"citizenry");
	t::LoadContentFromVocForList(LiteWnd::link_element(root_, "directid"),	    "direct");

	// ��������� ��������� ������ ������������ ������
	HTMLayoutAttachEventHandlerEx(map_elements_["grpid"], ElementEventProcChangedGroup, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

	// ����� � ������� ��� ���������
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(root_, "table-ege-discip"), ElementEventProcEgeDiscip, this, HANDLE_BEHAVIOR_EVENT | DISABLE_INITIALIZATION);

	// ��������� � ��������� ������
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(root_, "bt-change-ege-disp"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(root_, "bt-delete-ege-disp"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);

	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(root_, "bt-fast-znum"),  ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(root_, "bt-fast-enter"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(root_, "bt-fast-exist"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(root_, "bt-fast-gak"),   ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
}

// ���������  ��� �������� ��� �������� �������� (���������� �� ������)
void SData::UpdateView(void)
{
	string_t query = string_t() +
		" SELECT * FROM students "
		" WHERE `deleted` = 0 AND "
		" `id` = " + aux::itow(theApp.GetCurrentStudentID()) + " LIMIT 1";
	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
	if (!res.size())
	{
		string_t msg = string_t() + "������ � ������ ���������, �� ������ ������� � id = " + 
			aux::itow(theApp.GetCurrentStudentID());
		assert(FALSE);
		throw wss::exception(wss::reason_message(FULL_LOCATION(), msg.c_str()));
	}
	string_t			buf;
	mybase::MYFASTROW	row = res.fetch_row();
	// ����������� ��������� ��������, ���� � ������� ��������� � �������-�� ���������
	// LOG_DEBUG << string_t(_root.get_html());
	for (std::map<string_t, htmlayout::dom::element>::iterator it = map_elements_.begin(); 
		it != map_elements_.end(); ++it)
	{
		htmlayout::dom::element el = it->second;
		string_t id  = el.get_attribute("id");
		el.set_attribute("old-value", row[id]);
		json::t2v(el, row[id]);
	}
	json::t2v(map_elements_["eduenddate"], t::get_year(row["eduenddate"]));

	map_elements_["grpid"].send_event(SELECT_SELECTION_CHANGED);

	UpdateEgeTable();
	
	root_.update();
}


// ��������� ������ �� ��������
bool SData::SaveData(void)
{
	std::map<string_t, string_t> values;
	get_current_value(values);
	// �������� ������
	string_t query;
	for (std::map<string_t, string_t>::iterator it = values.begin(); 
		it != values.end(); ++it)
	{
		query += "`" + it->first + "`='" + it->second + "', ";
	}
	if (query.empty())
		return false;

	// ������ ��������� �������� �������
	query.erase(query.size()-2, 1);

	query = "UPDATE `students` SET " + query + " WHERE `id` = " + aux::itow(theApp.GetCurrentStudentID());
	
	// ��������� ������ �� ��������, ���� ��� �� �� ��� �� ��������� �������

	if (IsExistsDorovorNumber())
		return false;
	if (IsExistsZnum())
		return false;
	if (IsFalseEduDocInfo())
		return false;
	// 
	mysql_autocommit(theApp.GetCon().GetCon(), false);
	theApp.GetCon().Query("START TRANSACTION;");
	bool IsOk = true;
	try{
		UpdateAllPayFactsForStudent();
		UpdateAllProgressForStudent();
		theApp.GetCon().Query(query.c_str());
		// ��������� ��� ���������
		theApp.GetCon().Query("COMMIT;");
	}
	catch (...)
	{
		theApp.ExceptionManage();
		mysql_autocommit(theApp.GetCon().GetCon(), true);
		theApp.GetCon().Query("ROLLBACK;");
		return false;
	}
	mysql_autocommit(theApp.GetCon().GetCon(), true);
	return true;
}

// ��������� ������� ������������� ������ ��������
bool SData::IsExistsDorovorNumber(void)
{
	string_t query = " SELECT id, CONCAT(secondname,\' \',firstname,\' \',thirdname) as fio, grpid " 
		" FROM students "
		" WHERE dogshifrid = '" + json::v2t(map_elements_["dogshifrid"].get_value()) + "'"
		" AND   dogyearid  = '" + json::v2t(map_elements_["dogyearid"].get_value()) + "'"
		" AND   dogfastid  = '" + json::v2t(map_elements_["dogfastid"].get_value()) + "'"
		" AND   dognum     = '" + json::v2t(map_elements_["dognum"].get_value()) + "'"
		" AND   id         != '" +  aux::itow(theApp.GetCurrentStudentID()) + "'"
		" AND   deleted    = 0 LIMIT 1";

	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
	mybase::MYFASTROW	 row;

	if (!(row = res.fetch_row()))
		return false;

	string_t shifr = json::get_caption(map_elements_["dogshifrid"]) + "-" +
		json::get_caption(map_elements_["dogyearid"]) + 
		json::get_caption(map_elements_["dogfastid"]) +	"-" + 
		json::v2t(map_elements_["dognum"].get_value()); 
	string_t msg = "������� � ������ �������� :  " + shifr + 
		"  ��� ���������� � ���� ������: \n" + row["fio"] + 
		" (������: " + theApp.GetGroupName(aux::wtoi(row["grpid"])) + 
		").\n���������� ������������ ����� ��������.\n��������� ����������.";

	MessageBox(root_.get_element_hwnd(true), msg, L"������", MB_OK | MB_ICONERROR | MB_APPLMODAL);

	return true;
}

// ��������� �� ������� ������ �������
bool SData::IsExistsZnum(void)
{
	string_t query = " SELECT id, CONCAT(secondname,\' \',firstname,\' \',thirdname) AS fio, grpid, znum "
		" FROM students "
		" WHERE `znum`= '" + json::v2t(map_elements_["znum"].get_value()) + "' " +
		" AND `id` != '" + aux::itow(theApp.GetCurrentStudentID()) + "' AND deleted = 0 LIMIT 1"; 

	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
	mybase::MYFASTROW	 row;

	if (!(row = res.fetch_row()))
		return false;

	string_t msg = "������� � ������� ������� :  " + row["znum"] + 
		"  ��� ���������� � ���� ������: \n" + row["fio"] + 
		" (������: " + theApp.GetGroupName(aux::wtoi(row["grpid"])) + ").\n�� ������ ���������� ������ ?";

	if (IDNO == MessageBox(root_.get_element_hwnd(true), msg, L"���������", 
		MB_YESNO |MB_ICONINFORMATION | MB_APPLMODAL))
		return true; 

	return false;
}

// ��������� �� ��������� �� ��� �������
bool SData::IsFalseEduDocInfo(void)
{
	/*string_t query = "SELECT title FROM "+TabVocName+" WHERE vkey="+ToStr(keys[FALSEEDUDOCINFOS])+
	" AND deleted=0 AND title="+ToStr(Editedudocinfo->Text);
	" SELECT id, CONCAT(secondname,\' \',firstname,\' \',thirdname) AS fio, grpid "
	" FROM students "
	" WHERE `znum`= '" + json::v2str(_map_elements["znum"]) + "' " +
	" AND `id` != '" + theApp.GetCurrentStudentID() + "' AND deleted = 0 LIMIT 1"; 

	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
	mybase::MYFASTROW	 row;

	if (!(row = res.fetch_row()))
	return false;

	string_t msg = "�������� ��������:\n��������� ������ ( "+Editedudocinfo->Text + 
	" ) ��������� �� ����������� �������� " + EditSecondName->Text + 
	" ��������� � ����������� ����� � ���� ������ ���������� �������." +
	"\n�� ������������� ������ �������� �������� " + EditSecondName->Text + " � ���� ���� ������?";

	if (ID_NO == MessageBox(::GetActiveWindow(), msg.c_str(), "��������������", MB_YESNO | MB_ICONINFORMATION))
	return false;
	*/
	return false;
}

// �������� ��� ����� ������ ���� ���������� ������
void SData::UpdateAllPayFactsForStudent(void)
{
	string_t new_grpid = json::v2t(map_elements_["grpid"].get_value());
	string_t old_grpid = map_elements_["grpid"].get_attribute("old-value");
	// ���� ������ �� ���������� �� �������
	if (new_grpid == old_grpid)
		return;			

	// ��� ����� ������
	string_t fact_query = string_t() +
		" SELECT pf.id, pf.idopts, po.datestart, po.dateend, po.commoncountmoney "
		" FROM payfactstest AS pf, payoptstest AS po "
		" WHERE pf.deleted=0 AND po.deleted=0 AND "
		" pf.idstud = " + aux::itow(theApp.GetCurrentStudentID()) + " AND pf.idopts = po.id AND "
		" po.idgroup = " + old_grpid;

	mybase::MYFASTRESULT fact_res = theApp.GetCon().Query(fact_query);
	mybase::MYFASTROW	 fact_row;
	// �������� �� ���� �������� ��������� ������ ������
	while (fact_row = fact_res.fetch_row())
	{
		string_t new_opts; // �������� ���. ����� �����
		string_t find_query = string_t() +
			" SELECT pon.id " +
			" FROM payoptstest AS pon, payoptstest AS po "
			" WHERE pon.deleted = 0 AND po.deleted = 0 AND "
			" po.id = " + fact_row["idopts"] + " AND po.id != pon.id AND "
			" pon.idgroup=" + new_grpid + " AND pon.datestart = po.datestart AND "
			" pon.dateend = po.dateend AND pon.commoncountmoney = po.commoncountmoney";
		mybase::MYFASTRESULT find_res = theApp.GetCon().Query(find_query);
		mybase::MYFASTROW	 find_row;
		// ���� ����� ����������� ����� ��� ����� ������
		if (find_row = find_res.fetch_row())
		{
			new_opts = find_row["id"];
		}
		else
		{
			// ����� ������� ����� ����� ������
			string_t insert_query = 
				" INSERT INTO  payoptstest (idgroup, datestart, dateend, commoncountmoney, deleted) "
				" values(" + new_grpid + ", '" + fact_row["datestart"]+"', '" + fact_row["dateend"]+"', '" + 
				fact_row["commoncountmoney"] + "',0)";
			theApp.GetCon().Query(insert_query);
			// �������� ��������� ����� ������
			string_t get_query = string_t() +
				" SELECT id " +
				" FROM payoptstest " +
				" WHERE deleted = 0 AND idgroup = " + new_grpid + " AND datestart = '" + fact_row["datestart"] + 
				"' AND dateend = '" + fact_row["dateend"] + "' AND commoncountmoney = '" + fact_row["commoncountmoney"] + "'";

			mybase::MYFASTRESULT get_res = theApp.GetCon().Query(get_query);
			mybase::MYFASTROW	 get_row;
			if (get_row = get_res.fetch_row())
			{
				new_opts = get_row["id"];
			}
			else
			{
				// ����� ������
				MessageBox(root_.get_element_hwnd(true), L"�� ������� �������� ������ �� ������ ��������.", 
					L"������", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
				throw wss::exception(wss::reason_message(FULL_LOCATION()));
			}
		}
		// ������ ������ ������ ����� ��� ����� ������
		string_t update_query = string_t() +
			" UPDATE payfactstest "
			" SET idopts = " + new_opts + 
			" WHERE deleted = 0 and id = " + fact_row["id"];
		theApp.GetCon().Query(update_query);
	}
}

// �������� ��� ������ ���� ���������� �������������
void SData::UpdateAllProgressForStudent(void)
{
	string_t new_specid = json::v2t(map_elements_["specid"].get_value());
	string_t old_specid = map_elements_["specid"].get_attribute("old-value");
	// ���� ������������� �� ���������� �� �������
	if (new_specid == old_specid)
		return;			

	// ��� ������
	string_t progr_query = string_t() +
		" SELECT p.id, p.iddiscip, d.fulltitle, d.shorttitle " +
		" FROM progress AS p, disciplines AS d " +
		" WHERE p.deleted = 0 AND d.deleted = 0 AND p.idstud = " + aux::itow(theApp.GetCurrentStudentID()) + 
		" AND p.iddiscip = d.id AND d.idspec = " + old_specid;

	mybase::MYFASTRESULT progr_res = theApp.GetCon().Query(progr_query);
	mybase::MYFASTROW	 progr_row;
	// �������� �� ���� ��������� �������
	while (progr_row = progr_res.fetch_row())
	{
		string_t new_discipid; // �������� ���. ����� ����������
		string_t find_query = string_t() +
			" SELECT dn.id "
			" FROM disciplines as dn, disciplines as d "
			" WHERE dn.deleted = 0 AND d.deleted = 0 AND d.id = " + old_specid + 
			" AND d.id != dn.id AND dn.idspec = " + new_specid + 
			" AND (dn.fulltitle = d.fulltitle OR dn.shorttitle = d.shorttitle)";

		mybase::MYFASTRESULT find_res = theApp.GetCon().Query(find_query);
		mybase::MYFASTROW	 find_row;
		// ���� ����� ����������� ���������� ��� ������
		if (find_row = find_res.fetch_row())
		{
			new_discipid = find_row["id"];
		}
		else
		{
			// ����� ���� ����� ����������, ����� ������
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!
			// *********************************************8
			/*
			string_t update_query = string_t() +
							" UPDATE progress "
							" SET deleted = 1"
							" WHERE deleted = 0 AND id = " + progr_row["id"];
						theApp.GetCon().Query(update_query);
						continue;
						*/
			//************************************************
			// ����� ������� ����� ����������
			string_t insert_query = string_t() +
				" INSERT INTO disciplines (idspec, fulltitle, shorttitle, deleted) " +
				" values(" + new_specid + ", '" + progr_row["fulltitle"] + "', '" + 
				progr_row["shorttitle"] + "', 0)";
			theApp.GetCon().Query(insert_query);
			// �������� ��������� ����������
			string_t get_query = string_t() +
				" SELECT id FROM disciplines " +
				" WHERE deleted = 0 AND idspec = " + new_specid + 
				" AND fulltitle = '" + progr_row["fulltitle"] + "' AND shorttitle = '" + progr_row["shorttitle"] + "'";
			mybase::MYFASTRESULT get_res = theApp.GetCon().Query(get_query);
			mybase::MYFASTROW	 get_row;
			if (get_row = get_res.fetch_row())
			{
				new_discipid = get_row["id"];
			}
			else
			{
				// ����� ������
				MessageBox(::GetActiveWindow(), L"�� ������� �������� ������ ������������ ��������.", 
					L"��������������", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
				throw std::runtime_error("");
			}
		}
		// ������ ������ ������ ���������� ��� ����� ������
		string_t update_query = string_t() +
			" UPDATE progress "
			" SET iddiscip = " + new_discipid + 
			" WHERE deleted = 0 AND id = " + progr_row["id"];
		theApp.GetCon().Query(update_query);
	}
}

// ���������� ����� ��� ������
long SData::GetCityIdForGroup(long GroupId)
{
	std::string group  = theApp.GetGroupName(GroupId);
	std::string digits = "-0123456789";
	// ���� ������ ��������� ����� � �������� ������
	int d_pos  = -1;
	for (int pos = 0; pos < (int)group.size(); ++pos)
		if (digits.find(group[pos], 0) != std::string::npos)
		{
			d_pos = pos; 
			break;
		}

	if (d_pos == -1 || (group.find('/', 0) == std::string::npos))
		return 0;
	
	int			sp_pos	= (int)group.find('/', 0);
	std::string city    = group.substr(d_pos + 1, sp_pos - 2 - d_pos);

	return aux::atoi(city.c_str(), 0);
}

// ��������� ������������ ��������� ������, ������������ ��
void SData::get_current_value(std::map<string_t, string_t>& value)
{
	value.clear();
	for (std::map<string_t, htmlayout::dom::element>::iterator it = map_elements_.begin(); 
		it != map_elements_.end(); ++it)
	{
		htmlayout::dom::element el	= it->second;
		string_t id = el.get_attribute("id");
		string_t new_value	= json::v2t(el.get_value());
		string_t old_value  = el.get_attribute("old-value");
		
		if (id == "edudatediplom" || id == "edudatequalif" || id == "exitdate" || id == "egedate")
			new_value = t::date2t(new_value);
		if (id == "eduenddate" && !new_value.empty())
			new_value += "-01-01";

		if (new_value == old_value)
			continue;	
		value[id] = new_value;
	}
}

// ������� �  ���� ���������� � ����� ��������
bool SData::CreateData(void)
{
	if (theApp.GetCurrentGroupID() < 0)
		throw wss::exception(wss::reason_message(FULL_LOCATION()));
	string_t query = string_t() +
		" INSERT INTO students (grpid) "
		" VALUES(" + aux::itow(theApp.GetCurrentGroupID()) + ")";

	try
	{
		theApp.GetCon().Query(query);	
	}
	catch (...)
	{
		theApp.ExceptionManage();
		return false;
	}
// �������� id ������ �������� � ������ ��� �������
	theApp.SetCurrentStudentID((long)mysql_insert_id(theApp.GetCon().GetCon()));
	return true;
}

// ������ �����, ��� ������� ������
bool SData::DeleteData(void)
{
	string_t query = string_t() +
		" UPDATE students "
		" SET deleted = 1 "
		" WHERE id = " + aux::itow(theApp.GetCurrentStudentID());
	try
	{
		theApp.GetCon().Query(query);	
	}
	catch (...)
	{
		theApp.ExceptionManage();
		return false;
	}

	theApp.SetCurrentStudentID(-1);
	return true;

}

// �������� ����� ��� ��������� ������
BOOL CALLBACK SData::ElementEventProcChangedGroup(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != SELECT_SELECTION_CHANGED)
		return FALSE;

	SData* data = static_cast<SData*>(tag);
	long city = data->GetCityIdForGroup(aux::wtoi(json::v2t(data->map_elements_["grpid"].get_value())));
	json::t2v(data->map_elements_["cityid"], string_t(aux::itow(city)));

	return TRUE;	
}

// ������������ ������
BOOL CALLBACK SData::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	SData*		    data = static_cast<SData*>(tag);
	const wchar_t*	id	 = htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-change-ege-disp"))
	{
		data->AddEgeDiscip();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-delete-ege-disp"))
	{
		data->DeleteEgeDiscip();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-fast-znum"))
	{
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-fast-enter"))
	{
		data->FastSetEnter();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-fast-exist"))
	{
		data->FastSetExit();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-fast-gak"))
	{
		return TRUE;
	}

	return FALSE;
}

// ������������ ����� �������� ���
BOOL CALLBACK SData::ElementEventProcEgeDiscip(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != TABLE_ROW_CLICK)
		return FALSE;

	SData* data = static_cast<SData*>(tag);
	
	htmlayout::dom::element row = data->GetCurEgeDiscip();
	if (row.is_valid())
	{
		json::t2v(data->sel_egediscip_, string_t(aux::itoa(row.get_attribute_int("iddiscip", -1))));
		json::t2v(data->ege_ball_, string_t(aux::itoa(row.get_attribute_int("ball", -1))));
	}

	return TRUE;
}

// ��������� ������� ��������� �� ���
void SData::UpdateEgeTable()
{
	htmlayout::dom::element list = table_ege_discip_;
	
	// ������ ����� ������������� ����������
	int cur_discip = GetCurEgeDiscip().get_attribute_int("iddiscip", -1);

	// ������� ��� ������
	while (list.children_count() > 1)
		HTMLayoutDetachElement(list.child(1));

	string_t query = string_t() +
		"  SELECT pr.id, gr.iddiscip, v.title, pr.ball FROM ege_for_group AS gr " 
		"  LEFT JOIN ege_progress AS pr ON gr.iddiscip = pr.iddiscip AND pr.idstud = " + aux::itow(theApp.GetCurrentStudentID()) + 
		"  AND pr.deleted = 0 "
		"  LEFT JOIN voc AS v ON gr.iddiscip = v.num AND v.deleted = 0 AND v.vkey = 'egediscip' "
		"  WHERE gr.deleted = 0 AND gr.grpid = "  + aux::itow(theApp.GetCurrentGroupID()) +
		"  ORDER BY v.title ";

	mybase::MYFASTRESULT res = theApp.GetCon().Query(query);

	string_t			buf;
	mybase::MYFASTROW	row;

	int count = 0;
	while (row = res.fetch_row())	
	{
		// ����� ��� � ����� �����  ����� �� ������
		buf += "<tr iddiscip=" + row["iddiscip"] + " ball=" + row["ball"] + ">" +
			   "<td>" + aux::itow(++count)	+ "</td>"
			   "<td>" + row["title"]	    + "</td>" +
			   (row["ball"].empty() ? "<td style=\"color:red\">0</td>" : "<td>" + row["ball"] + "</td>") +
			   "</tr>";
	}

	if (_mbslen(buf))
	{
		list.set_html(buf, _mbslen(buf), SIH_APPEND_AFTER_LAST);
		list.update();
		if (list.children_count() > 1)
		{
			string_t selector = string_t() + "tr[iddiscip=" + aux::itow(cur_discip) + "]";
			htmlayout::dom::element find = list.find_first(selector.c_str());
			if (!find.is_valid())
				find = list.child(1);
			find.set_state(STATE_CURRENT);
			list.send_event(TABLE_ROW_CLICK);
		}
	}
	list.update();
}

// ���������\�������� ������ �������� �� ���
void SData::AddEgeDiscip()
{
	string_t iddiscip = json::v2t(sel_egediscip_);
	string_t ball     = json::v2t(ege_ball_);
	string_t idstud   = aux::itow(theApp.GetCurrentStudentID());
	string_t idgroup  = aux::itow(theApp.GetCurrentGroupID());

	// ���� ������� �� �������
	if (iddiscip.empty())
	{
		MessageBox(::GetActiveWindow(), L"��� ���������� ��� ��������� ������ �� ���������� ��������� ������� ����������.", 
			L"��������������", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
		return;
	}

	// ���� ������� ����������� � ������ ��������� ��� ������
	string_t find_query = string_t() +
		" SELECT id FROM ege_for_group " 
		" WHERE deleted = 0 AND grpid = " + idgroup + " AND iddiscip = " + iddiscip;

	mybase::MYFASTRESULT find_res = theApp.GetCon().Query(find_query);
	if (!find_res.fetch_row())
	{
		string_t info = "��������� ���������� ����������� � ������ ��������� ��� ���� ������.\n"
				        "��� ���������� ������ ��������� �������� ���������� � ������.\n"
				        "������� ��, ���� ������ ����������.";
		if (IDOK != MessageBox(::GetActiveWindow(), info, 
			L"��������������", MB_OKCANCEL | MB_ICONINFORMATION | MB_APPLMODAL))
				return;
		try
		{
			string_t query = string_t() +
				" INSERT INTO ege_for_group (grpid, iddiscip, deleted) "
				" VALUES(" + idgroup + ", " + iddiscip + ", 0)";
			theApp.GetCon().Query(query);	
		}
		catch (...)
		{
			theApp.ExceptionManage();
			return;
		}
	}
	try
	{
		// ���� ������ �� �������� ����������� � ��������, �� ������� ����� ������ �������
		find_query = " SELECT id FROM ege_progress " 
			         " WHERE deleted = 0 AND idstud = " + idstud + " AND iddiscip = " + iddiscip;
		find_res = theApp.GetCon().Query(find_query);
		if (mybase::MYFASTROW row = find_res.fetch_row())
		{
			string_t query = string_t() +
				" UPDATE ege_progress "
				" SET ball = " + ball + " WHERE id = " + row["id"];
			theApp.GetCon().Query(query);	
		}
		else
		{
			string_t query = string_t() +
				" INSERT INTO ege_progress (idstud, iddiscip, ball, deleted) "
				" VALUES(" + idstud + ", " + iddiscip + ", " + ball + ", 0)";
			theApp.GetCon().Query(query);	
		}
	}
	catch (...)
	{
		theApp.ExceptionManage();
	}


	UpdateEgeTable();
}

// ������� ������ �������� �� ���
void SData::DeleteEgeDiscip()
{
	// ������ ��������� ���������� 
	int cur_discip = GetCurEgeDiscip().get_attribute_int("iddiscip", -1);

	// ���� ������� �� �������
	if (cur_discip == -1)
	{
		MessageBox(::GetActiveWindow(), L"��� �������� ���������� �� ������ ��������� ������ ��������� ������� ����������.", 
			L"��������������", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
		return;
	}

	string_t info = "�� �������� ���������� �� ������ ��������� ������.\n"
					"������ �� ������ ���������� � ���� ����������,\n"
					"��� ����������� ���������� ������ ����� ����� ������������.\n"
					"������� ��, ���� ������ ����������.";
	if (IDOK != MessageBox(::GetActiveWindow(), info, 
		L"��������������", MB_OKCANCEL | MB_ICONINFORMATION | MB_APPLMODAL))
			return;

	try
	{
		string_t query = string_t() +
			" UPDATE ege_for_group "
			" SET deleted = 1 "
			" WHERE grpid = " + aux::itow(theApp.GetCurrentGroupID()) +
			" AND iddiscip = " + aux::itow(cur_discip) + 
			" AND deleted = 0";
		theApp.GetCon().Query(query);	
	}
	catch (...)
	{
		theApp.ExceptionManage();
		return;
	}

	UpdateEgeTable();	
}

// ���������� ������� ���������� �� ��� � �������
htmlayout::dom::element SData::GetCurEgeDiscip()
{
	htmlayout::dom::element list = table_ege_discip_;
	htmlayout::dom::element row;
	for (UINT i = 1; i < list.children_count(); ++i)
	{
		htmlayout::dom::element t = list.child(i);
		if (t.get_state(STATE_CURRENT))
		{
			row = t;
			break;
		}
	}	
	return row;
}

// ������������� ��� ���� ������ ����� � ���� ������� � ����������
void SData::FastSetEnter()
{
	string_t enternum  = json::v2t(LiteWnd::link_element(root_, "enternum"));
	string_t enterdate = json::v2t(LiteWnd::link_element(root_, "enterdate"));
	enterdate = t::date2t(enterdate);

	string_t find_query = string_t() +  
		" SELECT id FROM students " 
		" WHERE deleted = 0 AND grpid = " + aux::itow(theApp.GetCurrentGroupID()) +
		" AND enternum IS NOT NULL AND enternum != '' AND enternum != " + enternum;
	mybase::MYFASTRESULT find_res = theApp.GetCon().Query(find_query);
	
	string_t info = 
		"����� � ���� ������� � ���������� �������� ��������\n"
		"����������� ��� ���� ������.\n";
	if (!find_res.size())
		info += "������� ��, ���� ������ ����������.";
	else
	{
		info += string_t() + 
			"� ������ ���� ��������� ��������� (" + aux::itow((long)find_res.size()) + ") � ������������ �������� ��������, ��������� �� �" + enternum + ".\n"
			"������� ��, ���� ������� � ������ ���������� ����� � ���� ������� � ���������� ��� ���� ������.";
	}
	if (IDOK != MessageBox(::GetActiveWindow(), info, 
		L"��������������", MB_OKCANCEL | MB_ICONINFORMATION | MB_APPLMODAL))
		return;

	string_t query = string_t() +
		" UPDATE students "
		" SET enternum = '" + enternum + "', enterdate = '" + enterdate + "' "
		" WHERE deleted = 0 AND grpid = " + aux::itow(theApp.GetCurrentGroupID());
	theApp.GetCon().Query(query, false);	
}
// ������������� ��� ���� ������ ����� � ���� ������� �� ����������
void SData::FastSetExit()
{
	string_t exitnum  = json::v2t(LiteWnd::link_element(root_, "exitnum"));
	string_t exitdate = json::v2t(LiteWnd::link_element(root_, "exitdate")); 
	exitdate = t::date2t(exitdate);

	string_t find_query = string_t() +  
		" SELECT id FROM students " 
		" WHERE deleted = 0 AND grpid = " + aux::itow(theApp.GetCurrentGroupID()) +
		" AND exitnum IS NOT NULL AND exitnum != '' AND exitnum != " + exitnum;
	mybase::MYFASTRESULT find_res = theApp.GetCon().Query(find_query);

	string_t info = 
		"����� � ���� ������� �� ���������� �������� ��������\n"
		"����������� ��� ���� ������.\n";
	if (!find_res.size())
		info += "������� ��, ���� ������ ����������.";
	else
	{
		info += string_t() + 
			"� ������ ���� ��������� ��������� (" + aux::itow((long)find_res.size()) + ") � ������������ �������� ��������, ��������� �� �" + exitnum + ".\n"
			"������� ��, ���� ������� � ������ ���������� ����� � ���� ������� �� ���������� ��� ���� ������.";
	}
	if (IDOK != MessageBox(::GetActiveWindow(), info, 
		L"��������������", MB_OKCANCEL | MB_ICONINFORMATION | MB_APPLMODAL))
		return;

	string_t query = string_t() +
		" UPDATE students "
		" SET exitnum = '" + exitnum + "', exitdate = '" + exitdate + "' "
		" WHERE deleted = 0 AND grpid = " + aux::itow(theApp.GetCurrentGroupID());
	theApp.GetCon().Query(query, false);
}