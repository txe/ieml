#include "stdafx.h"
#include "ManagerOfActions.h"

#include "ActionChangeSem.h"
#include "ActionChangeTypeEstimation.h"
#include "ActionBuhReport.h"
#include "ActionBuhReport2.h"
#include "StatisticDlg.h"
#include "json-aux-ext.h"
#include "LiteWnd.h"
#include "registry.h"

ManagerOfActions::ManagerOfActions(void)
{
}

ManagerOfActions::~ManagerOfActions(void)
{
}

BOOL CALLBACK ManagerOfActions::ElementEventProcMenu( LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	if (evtg != HANDLE_BEHAVIOR_EVENT)
		return FALSE;

	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != MENU_ITEM_CLICK)
		return FALSE;

	ManagerOfActions* main = static_cast<ManagerOfActions*>(tag);
	const wchar_t* id = element(pr->heTarget).get_attribute("id");

	if (aux::wcseq(id, L"show-statistic"))
	{
		CStatisticDlg dlg(main->_parent);
		dlg.DoModal();
		return TRUE;
	}
	if (aux::wcseq(id, L"change-plan-sem"))
	{
		CActionChangeSem dlg(main->_parent);
		dlg.SetTypeSem(CActionChangeSem::PLAN_SEM);
		dlg.DoModal();
		return TRUE;
	}
	if (aux::wcseq(id, L"change-graph-sem"))
	{
		CActionChangeSem dlg(main->_parent);
		dlg.SetTypeSem(CActionChangeSem::GRAPH_SEM);
		dlg.DoModal();
		return TRUE;
	}
	if (aux::wcseq(id, L"change-estim"))
	{
		CActionChangeTypeEstimation dlg(main->_parent);
		dlg.DoModal();
		return TRUE;
	}
	if (aux::wcseq(id, L"load-fio"))
	{
		main->FioToClipBoard();
		return TRUE;
	}
	if (aux::wcseq(id, L"buh-report"))
	{
		CActionBuhReport dlg(main->_parent);
		dlg.DoModal();
		return TRUE;
	}
	if (aux::wcseq(id, L"buh-report2"))
	{
		CActionBuhReport2 dlg(main->_parent);
		dlg.DoModal();
		return TRUE;
	}
	return FALSE;
}

void ManagerOfActions::FioToClipBoard()
{
	// загружает в буфер обмена список фамилий вида Фамилия И.О. номер зачетки
	string_t query = string_t() + 
		" SELECT CONCAT(secondname, ' ',LEFT(firstname, 1) ,'. ', LEFT(thirdname, 1), '.') AS fio, znum "
		" FROM students "
		" WHERE grpid = " + aux::itow(theApp.GetCurrentGroupID()) +
		" AND deleted = 0 "
		" ORDER BY secondname, firstname, thirdname, id;";

	mybase::MYFASTRESULT	res = theApp.GetCon().Query(query);
	string_t				list;
	mybase::MYFASTROW		row;

	while (row = res.fetch_row())
		list += row["fio"] + "\t" + row["znum"] + "\n"; 


	if (!OpenClipboard(NULL))
		return;

	EmptyClipboard();
	char* buf = (char*) LocalAlloc(LMEM_FIXED, list.size()*sizeof(char));
	if (buf == NULL)
	{
		CloseClipboard();
		return ;
	}
	memcpy(buf, list.c_str(), list.size());
	SetClipboardData(CF_TEXT, buf);
	CloseClipboard();
}

void ManagerOfActions::Init(LiteWnd* parent, HELEMENT menu)
{
	_parent = parent;
	_menu = menu;
	// обработчик для меню
	HTMLayoutAttachEventHandlerEx(_menu, ElementEventProcMenu, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_menu, "vert-delta-1"), ElementEventProcDelta, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_menu, "vert-delta-2"), ElementEventProcDelta, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	LoadDelta();
}

BOOL CALLBACK ManagerOfActions::ElementEventProcDelta( LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
	BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
	if (pr->cmd != EDIT_VALUE_CHANGED)
		return FALSE;

	ManagerOfActions* dlg = static_cast<ManagerOfActions*>(tag);
	dlg->SaveDelta();

	return TRUE;
}

void ManagerOfActions::LoadDelta()
{
	Reg reg;
	reg.SetRootKey(HKEY_CURRENT_USER);
	reg.SetKey("ODOBase\\ODOBase\\OTHER");
	
	json::t2v(LiteWnd::link_element(_menu, "vert-delta-1"), reg.ReadString("deltadogovor_1", "0"));
	json::t2v(LiteWnd::link_element(_menu, "vert-delta-2"), reg.ReadString("deltadogovor_2", "0"));
}

void ManagerOfActions::SaveDelta()
{
	Reg reg;
	reg.SetRootKey(HKEY_CURRENT_USER);
	reg.SetKey("ODOBase\\ODOBase\\OTHER");

	reg.WriteString("deltadogovor_1", json::v2t(LiteWnd::link_element(_menu, "vert-delta-1")).c_str());
	reg.WriteString("deltadogovor_2", json::v2t(LiteWnd::link_element(_menu, "vert-delta-2")).c_str());
}