#pragma once

#include <htmlayout.h>
#include "string_t.h"

using namespace htmlayout::dom;

class SPayment
{
	
public:
	SPayment(void);
	~SPayment(void);
private:
	element	_root;
	element	_payment_table; // ������ ���������
  element _bt_pay_add;
  element _bt_pay_edit;
	element _cat_month;
	element _cat_year;
	element _cat_money;
	element _cat_half_year;

public:
	// �������������� ��� ������� � ��� �����
	void Init(htmlayout::dom::element root);
private:
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ ����� ����������
	static BOOL CALLBACK ElementEventProcForPayTable(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
public:
	// ��������� �� ������ ������������ ��������
	void UpdateView(void);
	void UpdateViewPayment();

private:
	// ��������� ���� �� ������� sql � ������  ��������� � �������
	string_t DateToPayFormat(const string_t& pay_date);
	// ��������� ������ ��������
	void AddPay(void);
  // �������� ������ ��������
  void EditPay(void);
	// ��������� ������ ���������
	void UpdateViewStatus(void);
	// ��������� ��������� ������ �� ������������
	bool CheckInputPay(bool isEdit);
	// ���������� �������� ��������� ������
	element GetCurrentCat(void);
  // ���������� ������� ������
  element GetCurrentPay(void);
	// ������� ������
	void DeletePay(void);
	// ��������� ���� � ��������� ���������
	void UpdateViewCat(void);
	// �������� ��������� ������
	void AddCat(void);
	// ��������� ���������� �� ����� ��������� ������
	bool IsExistCat(bool include_cur_cat = true);
	// ���������� ���� ������ � ����� ������� ������
	void GetDateCat(string_t& startdate, string_t& enddate);
	// ��������� ��������� ��������� ������
	void SaveUpdateCat(void);
	// ������� ��������� ���������
	void DeleteCat(void);
	// ������ ��� ��������� ��������� ����������� ������ ��������
	void PersonalCat();
	// ��������� ���� ������ ������� ���������� ������
	void FillPayCombo(void);
  // ����������� ���������� ��������� ������
  void UpdatePayControls();
	// �������� ������ ������
	void ShowOnlyPay(void);
};
