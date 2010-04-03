// Test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OdoPro.h"
#include "SingeltonApp.h"

HINSTANCE hInst;								// current instance
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	theApp.InitInstance();
	return 0;
}
