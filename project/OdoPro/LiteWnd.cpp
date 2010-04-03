#include "stdafx.h"
#include "LiteWnd.h"
#include <exception>
#include "resource.h"

#pragma warning(push)
#pragma warning(disable:4311)

LiteWnd::LiteWnd(LiteWnd* pParent /* = NULL */):m_hWnd(NULL),m_pParent(pParent)
{

}

LiteWnd::~LiteWnd(void)
{
	if (std::uncaught_exception())
		if (::IsWindow(m_hWnd))
		{
			DestroyWindow(m_hWnd);
			if (m_pParent)
			{
				::EnableWindow(m_pParent->m_hWnd, TRUE);
				SetForegroundWindow(m_pParent->m_hWnd);
			}
		}
}

BOOL LiteWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	return TRUE;
}

BOOL LiteWnd::Create()
{
	HINSTANCE hIns = (HINSTANCE)GetModuleHandle(NULL);
	MyRegisterClass(hIns);
	return InitInstance(hIns, SW_SHOW);
}

LRESULT LiteWnd::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_NCCREATE:
			if (!PreCreateWindow(*((LPCREATESTRUCT)lParam)))
				return FALSE;
			break;
	}
	return HTMLayoutProc(hwnd ,message, wParam, lParam);
}

LRESULT LiteWnd::RealWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LiteWnd* pWnd;

	if (WM_NCCREATE == message)
			::SetWindowLong(hwnd, GWL_USERDATA, (LONG)((LPCREATESTRUCT(lParam))->lpCreateParams));

	// get the pointer to the window
	pWnd = GetObjectFromWindow(hwnd);
	
	// if we have the pointer, go to the message handler of the window
	// else, use DefWindowProc
	if (pWnd)
		return pWnd->WindowProc(hwnd, message, wParam, lParam);
	else	
		return ::DefWindowProc(hwnd, message, wParam, lParam);
}

ATOM LiteWnd::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= reinterpret_cast<WNDPROC>(RealWindowProc);
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)NULL;//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"TMyBase";//HTMLayoutClassNameW();
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE("IDR_MAINFRAME"));

	return RegisterClassEx(&wcex);
}

BOOL LiteWnd::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	m_hWnd = CreateWindow(L"TMyBase", /* szTitle */ NULL, 
		WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		NULL, NULL, hInstance, (void *)this);
	
	assert(::IsWindow(m_hWnd));

	if (!m_hWnd)
	{
		return FALSE;
	}
	//CenterWindow();
	HTMLayoutSetCallback(m_hWnd, &HTMLayoutNotifyHandlerReal, this);

	if (OnCreate() != 0)
		return FALSE;

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return TRUE;
}

INT_PTR	LiteWnd::DoModal()
{
	if (!::IsWindow(m_hWnd))
		if (!Create())
			return -1;
	
	//ShowWindow(m_hWnd, SW_SHOW);

	if (m_pParent)
		::EnableWindow(m_pParent->m_hWnd, FALSE);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (!::IsWindow(m_hWnd))
			break;
	}
	
	if (m_pParent && ::IsWindow(m_pParent->m_hWnd))
	{
		::EnableWindow(m_pParent->m_hWnd, TRUE);
		//ShowWindow(m_pParent->m_hWnd, SW_HIDE);
		SetForegroundWindow(m_pParent->m_hWnd);
		SetActiveWindow(m_pParent->m_hWnd);
		//ShowWindow(m_pParent->m_hWnd, SW_SHOW);
	}
	
	return 0;
}

void LiteWnd::Close()
{
	PostMessage(m_hWnd, WM_CLOSE, 0, 0);
}

LRESULT LiteWnd::OnLoadData(LPNMHL_LOAD_DATA nm)
{
	LPBYTE pb; DWORD cb;

	if (GetHtmlResource(nm->uri, pb, cb))
	{	
		HTMLayoutDataReady(m_hWnd, nm->uri, pb, cb);
		return LOAD_OK;
	}
	else
	{	
		assert(FALSE);
		return LOAD_DISCARD;
	}
}

LRESULT LiteWnd::OnAttachBehavior(LPNMHL_ATTACH_BEHAVIOR nm)
{
	// attach custom behaviors
	htmlayout::event_handler *pb = htmlayout::behavior::find(nm->behaviorName, nm->element);
	if (pb)
	{
		nm->elementTag  = pb;
		nm->elementProc = htmlayout::behavior::element_proc;
		nm->elementEvents = pb->subscribed_to;
	}
	else
		assert(FALSE);

	return 0;
}

BOOL LiteWnd::GetHtmlResource(LPCWSTR pszName, /*out*/PBYTE& pb, /*out*/DWORD& cb)
{
	// Find specified resource and check if ok
	HINSTANCE hIns = (HINSTANCE)GetModuleHandle(NULL);
	HRSRC hrsrc = ::FindResourceW(hIns, pszName, MAKEINTRESOURCE(RT_HTML));

	if(!hrsrc) 
		return FALSE;

	// Load specified resource and check if ok

	HGLOBAL hgres = ::LoadResource(hIns, hrsrc);
	if(!hgres) return FALSE;

	// Retrieve resource data and check if ok
	pb = (PBYTE)::LockResource(hgres); 
	if (!pb) 
		return FALSE;
	cb = ::SizeofResource(hIns, hrsrc); 
	if (!cb) 
		return FALSE;

	return TRUE;
}

LRESULT CALLBACK LiteWnd::HTMLayoutNotifyHandlerReal(UINT uMsg, WPARAM wParam, LPARAM lParam, LPVOID vParam)
{
	// all HTMLayout notification are comming here.
	LiteWnd* pView = static_cast<LiteWnd*>(vParam);
	NMHDR*	phdr = (NMHDR*)lParam;
	switch (phdr->code)
	{
		case HLN_CREATE_CONTROL:    return  pView->OnCreateControl((LPNMHL_CREATE_CONTROL) lParam);
		case HLN_CONTROL_CREATED:   return  pView->OnControlCreated((LPNMHL_CREATE_CONTROL) lParam);
		case HLN_DESTROY_CONTROL:   return  pView->OnDestroyControl((LPNMHL_DESTROY_CONTROL) lParam);
		case HLN_LOAD_DATA:         return  pView->OnLoadData((LPNMHL_LOAD_DATA) lParam);
		case HLN_DATA_LOADED:       return  pView->OnDataLoaded((LPNMHL_DATA_LOADED)lParam);
		case HLN_DOCUMENT_COMPLETE: return  pView->OnDocumentComplete();
		case HLN_ATTACH_BEHAVIOR:   return  pView->OnAttachBehavior((LPNMHL_ATTACH_BEHAVIOR)lParam );
	}
	return 0;
	//return	 pView->HTMLayoutNotifyHandler(uMsg, wParam, lParam, vParam);	
}

LRESULT CALLBACK LiteWnd::HTMLayoutNotifyHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LPVOID vParam)
{
	assert(FALSE);
	return 0;
}

// центрирует окно относительно предка или экрана
void LiteWnd::CenterWindow(void)
{
	RECT rcWnd;
	RECT rcArea;

	::GetWindowRect(m_hWnd, &rcWnd);

	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(
			MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTOPRIMARY), &mi);
	rcArea = mi.rcWork;

		int xLeft = (rcArea.right - rcArea.left)/2 - (rcWnd.right - rcWnd.left)/2;
	int yTop = (rcArea.bottom - rcArea.top) / 2 - (rcWnd.bottom - rcWnd.top)/2;

	
	SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

