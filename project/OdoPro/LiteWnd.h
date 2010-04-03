#pragma once

#include "htmlayout.h"
#include <crtdbg.h>
#include <deque>

#pragma warning(push)
#pragma warning(disable:4312)

class LiteWnd
{
public:
	HWND		m_hWnd;
	LiteWnd*	m_pParent;
	HINSTANCE	m_hInstance;
public:
	LiteWnd(LiteWnd* pParent = NULL);
	~LiteWnd(void);
private:
	LiteWnd(LiteWnd&) {};
public:
	// ������� ��������� ���������, �������� �� �����
	static  LRESULT RealWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	// ������� ��������� ���������, ���������� � ����������� ������
	virtual LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	// ������������ ��� ����� ����
	virtual ATOM	MyRegisterClass(HINSTANCE hInstance);
	// ��������������� ������� ����
	virtual BOOL	InitInstance(HINSTANCE hInstance, int nCmdShow);
	// ���������� ����� ��������� ����, ����� �������� ��-�� ����
	virtual BOOL	PreCreateWindow(CREATESTRUCT& cs); 
	// �������� ����������������� �������� ����
	virtual BOOL	Create();
	// ���������� �������� ����
	virtual INT_PTR	DoModal();
	// ��������� ����
	virtual void	Close();
	// ��������� ����
	virtual void SetCaption(const wchar_t* caption)
	{
		SetWindowTextW(m_hWnd, caption);
	}
public:
	operator HWND() const { return m_hWnd; }
public:
	// ���������� ������� �������� ����, ����� ��������� �����������
	virtual int OnCreate() { return 0; }
public:
	// HTMLayout specific notification handler.
	static  LRESULT CALLBACK HTMLayoutNotifyHandlerReal(UINT uMsg, WPARAM wParam, LPARAM lParam, LPVOID vParam);
	virtual LRESULT CALLBACK HTMLayoutNotifyHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LPVOID vParam);

public:
	// implement what is needed in your application
	virtual LRESULT OnCreateControl(LPNMHL_CREATE_CONTROL nm) { return 0; }
	virtual LRESULT OnControlCreated(LPNMHL_CREATE_CONTROL nm) { return 0; }
	virtual LRESULT OnDestroyControl(LPNMHL_DESTROY_CONTROL nm) { return 0; }
	virtual LRESULT OnDataLoaded(LPNMHL_DATA_LOADED nm) { return 0; }
	virtual LRESULT OnDocumentComplete() { return 0; }
	virtual LRESULT OnLoadData(LPNMHL_LOAD_DATA nm);
	virtual LRESULT OnAttachBehavior(LPNMHL_ATTACH_BEHAVIOR nm);
public:
	virtual BOOL GetHtmlResource(LPCWSTR pszName, /*out*/PBYTE& pb, /*out*/DWORD& cb);
	inline htmlayout::dom::element link_element(const char* name)
	{
		htmlayout::dom::element root = htmlayout::dom::element::root_element(m_hWnd);
		return link_element(root, name);
	}
	static inline htmlayout::dom::element link_element(htmlayout::dom::element root, const char* name)
	{
		assert(root.is_valid()); // �� �������� ������ �������� ���������;
		htmlayout::dom::element el	=  root.get_element_by_id(name);
		assert(el.is_valid()); // �� �������� ������ �������� ���������;
		return el;
		//	el.set_state(STATE_DISABLED);
	}
private:
	// returns a pointer the window (stored as the WindowLong)
	inline static LiteWnd* GetObjectFromWindow(HWND hWnd)
	{
		return reinterpret_cast<LiteWnd*>(GetWindowLong(hWnd, GWL_USERDATA));
	}
public:
	// ���������� ���� ������������ ������ ��� ������
	void CenterWindow(void);
};

#pragma warning(pop)