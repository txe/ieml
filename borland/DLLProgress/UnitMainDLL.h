#ifndef UNITMAINDLL_H
#define UNITMAINDLL_H
//---------------------------------------------------------------------------
#ifdef __DLL__ 
# define DLL_EI __declspec(dllexport) 
#else 
# define DLL_EI __declspec(dllimport) 
#endif 
//---------------------------------------------------------------------------
extern "C" TForm* DLL_EI CreatePage(int,TForm*);
extern "C" char* DLL_EI GetTitlePage(void);
extern "C" void DLL_EI LoadData(char *, int, TForm*);
//---------------------------------------------------------------------------
#endif
