#pragma once

#include <htmlayout.h>
#include "..\litewnd.h"
#include "..\string_t.h"

using namespace htmlayout::dom;

class BuhReport_Month : public LiteWnd
{
public:
  BuhReport_Month(LiteWnd* parent);

public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual int	 OnCreate();

private:
  void InitDomElement(void);	                          // ��������� �������� ��� � ������������ � ����������
  void FillMonthGrid(string_t payMonth);                // ��������� ������� ������ �������
  void FillOrderNumGrid(string_t payDay);               // ��������� ������� ������� ������
  void FillFioGrid(string_t payDay, string_t orderNum); // ��������� ������� �������

  // ������������ ������
  static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
  // ������������ �����
  static BOOL CALLBACK ElementEventProcFor(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);

private:
  htmlayout::dom::element monthGrid_;
  htmlayout::dom::element orderNumGrid_;
  htmlayout::dom::element fioGrid_;

};
