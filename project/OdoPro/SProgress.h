#pragma once

#include <htmlayout.h>

class SProgress
{
public:
	SProgress(void);
	~SProgress(void);
private:
	htmlayout::dom::element	_root;
	htmlayout::dom::element	_discip; // ������ ���������
public:
	// �������������� ��� ������� � ��� �����
	void Init(htmlayout::dom::element root);
private:
	// ��������� ������ ���������
	void LoadListDiscip(void);
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ ����� ����������
	static BOOL CALLBACK ElementEventProcForDiscip(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ ����� �����
	static BOOL CALLBACK ElementEventProcForBall(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
public:
	// ��������� �� ������ ������������ ��������
	void UpdateView(void);
private:
	// �������� ��������� ����������
	long GetIDSelectedDiscip(void);
	// ��������� ������� ������ ��� ���������� ��������
	void UpdateViewEstimForDiscip();
	// ��������� ������ ��� ���������� �������� ��������
	void SaveBall(void);
	// ������� ��������� ����
	void DeleteBall(void);
public:
	// ��������� �� ������������ ��������� ����
	int CheckValue(void);
private:
	// ���������� �� ���������� ����
	long GetIDSelectedBall(void);
public:
	// �������� ���� �������� ����� ������ ��� ���� ������ ��� ������ ����������
	void FastInput(void);
	// ��������� ���������� �� �������
	void ProgressForSem(void);
	// ���������� ������ �������������� ���������
	void EditDiscip(void);
};
