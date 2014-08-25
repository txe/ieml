#pragma once

#include <htmlayout.h>
#include "..\litewnd.h"
#include "..\string_t.h"
#include "../msoffice/WordMacro.h"

using namespace htmlayout::dom;

class BuhReport_Month : public LiteWnd
{
public:
  BuhReport_Month(LiteWnd* parent);

public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual int	 OnCreate();

private:
  void InitDomElement(void);	        // ��������� �������� ��� � ������������ � ����������
  void Report(void);                  // 
  void ReportMonth(string_t month);
  void ReportDay(string_t day);
  void ReportFio(string_t day, string_t orderNum);
  void SerializeData(bool toSave);
  void ReportHeader(WordMacros& macros, string_t title, string_t date);
  string_t toRightDate(string_t date);

private:
  // ������������ ������
  static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);

private:
  htmlayout::dom::element orderNumBox_;
  htmlayout::dom::element dateBox_;
};
