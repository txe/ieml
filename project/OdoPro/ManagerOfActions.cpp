#include "stdafx.h"
#include "ManagerOfActions.h"

#include "ActionChangeSem.h"
#include "ActionChangeTypeEstimation.h"
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
		CStatisticDlg dlg;
		dlg.DoModal();
		return TRUE;
	}
	if (aux::wcseq(id, L"change-plan-sem"))
	{
		CActionChangeSem dlg;
		dlg.SetTypeSem(CActionChangeSem::PLAN_SEM);
		dlg.DoModal();
		return TRUE;
	}
	if (aux::wcseq(id, L"change-graph-sem"))
	{
		CActionChangeSem dlg;
		dlg.SetTypeSem(CActionChangeSem::GRAPH_SEM);
		dlg.DoModal();
		return TRUE;
	}
	if (aux::wcseq(id, L"change-estim"))
	{
		CActionChangeTypeEstimation dlg;
		dlg.DoModal();
		return TRUE;
	}
	if (aux::wcseq(id, L"load-fio"))
	{
		main->FioToClipBoard();
		return TRUE;
	}
	return FALSE;
}

void ManagerOfActions::FioToClipBoard()
{
	// ��������� � ����� ������ ������ ������� ���� ������� �.�. ����� �������
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

void ManagerOfActions::Init( HELEMENT menu )
{
	_menu = menu;
	// ���������� ��� ����
	HTMLayoutAttachEventHandlerEx(_menu, ElementEventProcMenu, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
	HTMLayoutAttachEventHandlerEx(LiteWnd::link_element(_menu, "vert-delta"), ElementEventProcDelta, this, HANDLE_BEHAVIOR_EVENT|DISABLE_INITIALIZATION);
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
	
	json::t2v(LiteWnd::link_element(_menu, "vert-delta"), reg.ReadString("deltadogovor", "0"));
}

void ManagerOfActions::SaveDelta()
{
	Reg reg;
	reg.SetRootKey(HKEY_CURRENT_USER);
	reg.SetKey("ODOBase\\ODOBase\\OTHER");

	reg.WriteString("deltadogovor", json::v2t(LiteWnd::link_element(_menu, "vert-delta")).c_str());
}