#pragma once

#include <htmlayout.h>
#include "..\litewnd.h"
#include "..\string_t.h"

using namespace htmlayout::dom;

class BuhReport_Saldo : public LiteWnd
{
public:
  BuhReport_Saldo(LiteWnd* parent);

public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual int	 OnCreate();

private:
  void InitDomElement(void);	// связывает элементы дом с отображением и инициирует

  // обрабатывает кнопки
  static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
  // обрабатывает выбор
  static BOOL CALLBACK ElementEventProcFor(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
};
