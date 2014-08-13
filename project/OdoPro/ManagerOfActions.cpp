#include "stdafx.h"
#include "ManagerOfActions.h"

#include "ActionChangeSem.h"
#include "ActionChangeTypeEstimation.h"
#include "ActionBuhReport.h"
#include "ActionBuhReport2.h"
#include "ActionFastInputGak.h"
#include "StatisticDlg.h"
#include "json-aux-ext.h"
#include "LiteWnd.h"
#include "registry.h"
#include "reports-buh/BuhReport_Month.h"
#include "reports-buh/BuhReport_Saldo.h"

void ManagerOfActions::Init(LiteWnd* parent, HELEMENT menu, IActionParent* actionParent)
{
  _parent = parent;
  _menu   = menu;
  _actionParent = actionParent;

  // обработчик для меню
  HTMLayoutAttachEventHandlerEx(_menu, ElementEventProcMenu, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
  HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_menu, "vert-delta-1"), ElementEventProc, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
  HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_menu, "vert-delta-2"), ElementEventProc, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
  HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_menu, "show-dog-num"), ElementEventProc, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
  SerializeDelta(false);
  SerializeShowDogNum(false);
}

BOOL CALLBACK ManagerOfActions::ElementEventProc( LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
{
  BEHAVIOR_EVENT_PARAMS* pr = static_cast<BEHAVIOR_EVENT_PARAMS*>(prms);
  if (pr->cmd == EDIT_VALUE_CHANGED)
  {
    ManagerOfActions* dlg = static_cast<ManagerOfActions*>(tag);
    dlg->SerializeDelta(true);
    return TRUE;
  }
  if (pr->cmd == BUTTON_STATE_CHANGED || pr->cmd == BUTTON_CLICK)
  {
    ManagerOfActions* dlg = static_cast<ManagerOfActions*>(tag);
    dlg->SerializeShowDogNum(true);
    dlg->_actionParent->IActionParent_UpdateWindow();
    return TRUE;
  }

  return FALSE;
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
	if (aux::wcseq(id, L"input-gak"))
	{
		CActionFastInputGakDlg dlg(main->_parent);
		dlg.DoModal();
		return TRUE;
	}
	if (aux::wcseq(id, L"buh-report")) // оплата подробно
	{
		CActionBuhReport dlg(main->_parent);
		dlg.DoModal();
		return TRUE;
	}
	if (aux::wcseq(id, L"buh-report2")) // оплата на начало месяца
	{
		CActionBuhReport2 dlg(main->_parent);
		dlg.DoModal();
		return TRUE;
	}
  if (aux::wcseq(id, L"buh-report-month")) // оплата за месяц по дням
  {
    BuhReport_Month dlg(main->_parent);
    dlg.DoModal();
    return TRUE;
  }
  if (aux::wcseq(id, L"buh-report-saldo")) // оборотно-сальдовая ведомость
  {
    //BuhReport_Saldo dlg(main->_parent);
    //dlg.DoModal();
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

void ManagerOfActions::SerializeDelta(bool toSave)
{
	Reg reg;
	reg.SetRootKey(HKEY_CURRENT_USER);
	reg.SetKey("ODOBase\\ODOBase\\OTHER");

  if (toSave)
  {
    reg.WriteString("deltadogovor_1", json::v2t(LiteWnd::link_element(_menu, "vert-delta-1")).c_str());
    reg.WriteString("deltadogovor_2", json::v2t(LiteWnd::link_element(_menu, "vert-delta-2")).c_str());
  }
  else
  {
	  json::t2v(LiteWnd::link_element(_menu, "vert-delta-1"), reg.ReadString("deltadogovor_1", "0"));
	  json::t2v(LiteWnd::link_element(_menu, "vert-delta-2"), reg.ReadString("deltadogovor_2", "0"));
  }
}

void ManagerOfActions::SerializeShowDogNum(bool toSave)
{
	Reg reg;
	reg.SetRootKey(HKEY_CURRENT_USER);
	reg.SetKey("ODOBase\\ODOBase\\OTHER");

  if (toSave)
    reg.WriteString("show-dog-num", json::v2t(LiteWnd::link_element(_menu, "show-dog-num")).c_str());
  else
    json::t2v(LiteWnd::link_element(_menu, "show-dog-num"), reg.ReadString("show-dog-num", "0"));
}