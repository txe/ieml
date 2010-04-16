#include "stdafx.h"
#include "StatisticDlg.h"
#include "json-aux-ext.h"
#include "string_t.h"


CStatisticDlg::CStatisticDlg(LiteWnd* pParent /*=NULL*/)
	: LiteWnd(pParent)
{
}

CStatisticDlg::~CStatisticDlg(void)
{
}

BOOL CStatisticDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!LiteWnd::PreCreateWindow(cs))
		return FALSE;

	cs.lpszName = (LPCWSTR)"Статистика";
	return TRUE;
}

int CStatisticDlg::OnCreate()
{
	SetWindowPos(m_hWnd, NULL, -1, -1, 500, 450, 
		SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	CenterWindow();

	PBYTE pb; DWORD cb;
	if(GetHtmlResource(L"IDR_HTML_STAT", pb, cb))
	{
		assert(::IsWindow(m_hWnd));
		::HTMLayoutLoadHtml(m_hWnd, pb, cb);
	}
	InitDomElement();

	return 0;
}

// обрабатывает кнопки
BOOL CALLBACK CStatisticDlg::ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != BUTTON_CLICK)
		return FALSE;

	CStatisticDlg* dlg = static_cast<CStatisticDlg*>(tag);
	const wchar_t* id = htmlayout::dom::element(he).get_attribute("id");

	if (aux::wcseq(id, L"bt-show"))
	{
		dlg->ShowStatistic();
		return TRUE;
	}
	if (aux::wcseq(id, L"bt-close"))
	{	
		dlg->Close();
		return TRUE;
	}
	
	return FALSE;
}

// связывает элементы дом с отображением и инициирует
void CStatisticDlg::InitDomElement(void)
{
	// связывает с событиями кнопок
	HTMLayoutAttachEventHandlerEx(link_element("bt-show"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(link_element("bt-close"), ElementEventProcBt, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);

	// заполянем списки
	t::LoadContentFromVocForList(link_element("educationid"),	"education");	
	t::LoadContentFromVocForList(link_element("languageid"),	"language");	
	t::LoadContentFromVocForList(link_element("dogyearid"),		"dogyear");		
	t::LoadContentFromVocForList(link_element("grpid"),			"grp");			
	t::LoadContentFromVocForList(link_element("specid"),		"spec");		
	t::LoadContentFromVocForList(link_element("eduformid"),		"eduform");		
	t::LoadContentFromVocForList(link_element("dogfastid"),		"dogfast");		
	t::LoadContentFromVocForList(link_element("citizenryid"),	"citizenry");	
}

// показывает статистику по выбранным критериям
void CStatisticDlg::ShowStatistic(void)
{
	std::string from_st  = " FROM students AS tst ";
	std::string where_st = " WHERE tst.deleted=0 ";
	string_t	message;
	
	#define CHECKED(checkbox) (aux::wcseq(json::v2t(link_element(checkbox)), L"1")) 

	if (CHECKED("switch-sex"))
	{
		where_st	+=	" AND tst.sex = '" + json::v2t(link_element("sex")) + "' ";
		message		+=	"\n   пол: " + json::get_caption(link_element("sex"));
	}
	if (CHECKED("switch-year"))
	{
		where_st	+=	" AND tst.bdate >= '" + json::v2t(link_element("year")) + "-01-01' " 
						" AND tst.bdate <= '" + json::v2t(link_element("year")) +"-12-31' ";
		message		+=	"\n   год рождения: " + json::v2t(link_element("year"));
	}
	if (CHECKED("switch-educationid"))
	{
		where_st	+=	" AND tst.educationid='" + json::v2t(link_element("educationid")) + "' ";
		message		+=	"\n   образование: " + json::get_caption(link_element("educationid"));
	}
	if (CHECKED("switch-languageid"))
	{
		where_st	+=	" AND tst.languageid='" + json::v2t(link_element("languageid")) + "' ";
		message		+=	"\n   язык: " + json::get_caption(link_element("languageid"));
	}
	if (CHECKED("switch-dogyearid"))
	{
		where_st	+=	" AND tst.dogyearid='" + json::v2t(link_element("dogyearid")) + "' ";
		message		+=	"\n   год поступления: " + json::get_caption(link_element("dogyearid"));
	}
	if (CHECKED("switch-grpid"))
	{
		where_st	+=	" AND tst.grpid='" + json::v2t(link_element("grpid")) + "' ";
		message		+=	"\n   группа: " + json::get_caption(link_element("grpid"));
	}
	if (CHECKED("switch-specid"))
	{
		where_st	+=	" AND tst.specid='" + json::v2t(link_element("specid")) + "' ";
		message		+=	"\n   специальность: " + json::get_caption(link_element("specid"));
	}
	if (CHECKED("switch-eduformid"))
	{
		where_st	+=	" AND tst.eduformid='" + json::v2t(link_element("eduformid")) + "' ";
		message		+=	"\n   форма обучения: " + json::get_caption(link_element("eduformid"));
	}
	if (CHECKED("switch-dogfastid"))
	{
		where_st	+=	" AND tst.dogfastid='" + json::v2t(link_element("dogfastid")) + "' ";
		message		+=	"\n   скорость обучения: " + json::get_caption(link_element("dogfastid"));
	}
	if (CHECKED("switch-category_num"))
	{
		where_st	+=	" AND tst.category_num = '" + json::v2t(link_element("category_num")) + "' ";
		message		+=	"\n   категория: " + json::get_caption(link_element("category_num"));
	}
	if (CHECKED("switch-citizenryid"))
	{
		where_st	+=	" AND tst.citizenryid='" + json::v2t(link_element("citizenryid")) + "' ";
		message		+=	"\n   гражданство: " + json::get_caption(link_element("citizenryid"));
	}
	if (CHECKED("switch-ignore"))
	{
		where_st	+=	" AND tvocigngrp.deleted=0 AND tvocigngrp.vkey='grp' AND "
						" tst.grpid=tvocigngrp.num AND tvocigngrp.title NOT LIKE '%ОТЧИСЛ%' ";
		from_st		+=	", voc AS tvocigngrp";
		message		+=	"\n   (без учета отчисленных студентов)";
	}
	else
		message		+=	"\n   (с учетом отчисленных студентов)";

	if (message.empty())
		message = "\n (все студенты, занесенные в базу)";

	string_t				query = " SELECT count(*) AS count " + from_st + where_st;
	mybase::MYFASTRESULT	res = theApp.GetCon().Query(query);
	mybase::MYFASTROW		row	= res.fetch_row();

	message =	"Количество студентов равно " + row["count"] + ".\n"
				"Были выбраны следующие критерии выборки :" + message;

	MessageBox(::GetActiveWindow(), message, L"Статистика", MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
}