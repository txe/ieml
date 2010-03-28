//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USEFORM("MainUnit.cpp", MainForm);
USEFORM("UnitAbout.cpp", FormAbout);
USEFORM("UnitAccount.cpp", FormAccount);
USEFORM("UnitClearDB.cpp", FormClearDB);
USEFORM("UnitCommFilter.cpp", FormFilterComm);
USEFORM("UnitFilterMain.cpp", FormFilter);
USEFORM("UnitInsertVoc.cpp", FormInsertVoc);
USEFORM("UnitProgress.cpp", FormProgress);
USEFORM("UnitSelected.cpp", FormSelected);
USEFORM("UnitSplash.cpp", SplashForm);
USEFORM("UnitStatistic.cpp", FormStatistic);
USEFORM("UnitIzmSem.cpp", FormIzmSem);
USEFORM("UnitIzmGraphSem.cpp", FormIzmGraphSem);
USEFORM("UnitIzmOchenka.cpp", FormIzmOchenka);
USEFORM("UnitDeltaDogovor.cpp", FormDeltaDogovor);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
/*        HANDLE hFile = CreateFile("c:\\tempODOfile.tmp", GENERIC_WRITE, 0,
                                  NULL, CREATE_ALWAYS,
                                  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE,
                                  NULL);
        if(hFile == INVALID_HANDLE_VALUE)
        {
          ShowMessage("Зачем Вам два варианта программы на рабочем столе?\nПока не ответите запуска не произойдет!\nЕсли это смертельно необходимо, обратитесь к разработчику(Roman@fvmk.nnov.ru)!");
          return 1;
        }
  */

        try
        {
                Application->Initialize();
                Application->CreateForm(__classid(TMainForm), &MainForm);
         Application->CreateForm(__classid(TFormSelected), &FormSelected);
         Application->CreateForm(__classid(TFormInsertVoc), &FormInsertVoc);
         Application->CreateForm(__classid(TFormFilter), &FormFilter);
         Application->CreateForm(__classid(TFormProgress), &FormProgress);
         Application->CreateForm(__classid(TSplashForm), &SplashForm);
         Application->CreateForm(__classid(TFormAbout), &FormAbout);
         Application->CreateForm(__classid(TFormStatistic), &FormStatistic);
         Application->CreateForm(__classid(TFormAccount), &FormAccount);
         Application->CreateForm(__classid(TFormFilterComm), &FormFilterComm);
         Application->CreateForm(__classid(TFormClearDB), &FormClearDB);
         Application->CreateForm(__classid(TFormIzmSem), &FormIzmSem);
         Application->CreateForm(__classid(TFormIzmGraphSem), &FormIzmGraphSem);
         Application->CreateForm(__classid(TFormIzmOchenka), &FormIzmOchenka);
         Application->CreateForm(__classid(TFormDeltaDogovor), &FormDeltaDogovor);
         Application->Run();
        }
        catch (Exception &exception)
        {
                Application->ShowException(&exception);
        }

//        CloseHandle(hFile);

        return 0;
}
//---------------------------------------------------------------------------
