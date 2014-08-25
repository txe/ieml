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
  void InitDomElement(void);	        // связывает элементы дом с отображением и инициирует
  void Report(void);                  // 
  void ReportMonth(string_t month);
  void ReportDay(string_t day);
  void ReportFio(string_t day, string_t orderNum);

private:
  // обрабатывает кнопки
  static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);

private:
  htmlayout::dom::element orderNumBox_;
  htmlayout::dom::element dateBox_;
};
