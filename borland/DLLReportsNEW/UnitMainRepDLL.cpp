//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#pragma argsused

#include "UnitMainRepDLL.h"
#include <utilcls.h>

extern EDRegOpts opts;
//---------------------------------------------------------------------------
struct forreport
{
    int index;
    int _IDGroup;
    int _IDStudent;
};

//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
       ::CoInitialize(NULL);
    return 1;
}
//---------------------------------------------------------------------------
int GetCountOfReportsInDLL(void)
{
  return CountOfReports;
}
//---------------------------------------------------------------------------
char* GetTitleOfReportByIndex(int index)
{
  AnsiString Str;
  if (index<0 || index>=CountOfReports)
        Str="Invalid of index value!";
  else
        Str=arrTitlesOfReports[index];
  return Str.c_str();
}
//---------------------------------------------------------------------------
char* GetHintOfReportByIndex(int index)
{
  AnsiString Str;
  if (index<0 || index>=CountOfReports)
        Str="Invalid of index value!";
  else
        Str=arrHintsOfReports[index];
  return Str.c_str();
}
//---------------------------------------------------------------------------
void SetupDB(const char* host, const char* name, const char* user, const char* pass)
{
   opts.DBHost = host;
   opts.DBLogin = user;
   opts.DBPasswd = pass;
   opts.DBName = name;
   //AnsiString msg="server = " + opts.DBHost+",  name = "+opts.DBName+", login = "
   //+ opts.DBLogin + ", pass = " + opts.DBPasswd;
   //MessageBox(0,msg.c_str(),"Îøèáêà",MB_OK|MB_ICONERROR);
}
//---------------------------------------------------------------------------
void PreviewReportByIndex(int index, int _IDGroup, int _IDStudent)
{
   TForm* pReportForm=NULL;

   switch (index)
   {
    case RQ_VEDOMOST:
      pReportForm=new TFormReportVedom(Application);
      ((TFormReportVedom*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportVedom*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportVedom*)pReportForm)->Free();
      break;
    case RQ_DOGOVOR_2:
      pReportForm=new TFormReportDogovor(Application);
      ((TFormReportDogovor*)pReportForm)->SetIDStudent(_IDStudent);
      ((TFormReportDogovor*)pReportForm)->IsNewType(true);
      ((TFormReportDogovor*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportDogovor*)pReportForm)->Free();
      break;
   case RQ_DOGOVOR:
      pReportForm=new TFormReportDogovor(Application);
      ((TFormReportDogovor*)pReportForm)->SetIDStudent(_IDStudent);
      ((TFormReportDogovor*)pReportForm)->IsNewType(false);
      ((TFormReportDogovor*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportDogovor*)pReportForm)->Free();
      break;
    case RQ_EKZZACHLIST:
      pReportForm=new TFormReportEkzZachList(Application);
      ((TFormReportEkzZachList*)pReportForm)->SetIDStudent(_IDStudent);
      ((TFormReportEkzZachList*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportEkzZachList*)pReportForm)->Free();
      break;
    case RQ_PROTOCOL:
      pReportForm=new TFormReportProtocol(Application);
      ((TFormReportProtocol*)pReportForm)->SetIDStudent(_IDStudent);
      ((TFormReportProtocol*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportProtocol*)pReportForm)->Free();
      break;
    case RQ_SPRAVKA:
      pReportForm=new TFormReportSpravka(Application);
      ((TFormReportSpravka*)pReportForm)->SetIDStudent(_IDStudent);
      ((TFormReportSpravka*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportSpravka*)pReportForm)->Free();
      break;
    case RQ_SPRAVCARD:
      pReportForm=new TFormReportSpravCard(Application);
      ((TFormReportSpravCard*)pReportForm)->SetIDStudent(_IDStudent);
      ((TFormReportSpravCard*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportSpravCard*)pReportForm)->Free();
      break;
    case RW_OPLATAFORGROUP:
      pReportForm=new TFormReportDolgsForGroup(Application);
      ((TFormReportDolgsForGroup*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportDolgsForGroup*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportDolgsForGroup*)pReportForm)->Free();
      break;
    case RW_LISTSTUDDOGOVOR:
      pReportForm=new TFormReportListStudDogovor(Application);
//      ((TFormReportListStudDogovor*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportListStudDogovor*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportListStudDogovor*)pReportForm)->Free();
      break;
    case RW_ACTPRIEMSDACHADEL:
      pReportForm=new TFormReportActPriemSdachaDel(Application);
      ((TFormReportActPriemSdachaDel*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportActPriemSdachaDel*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportActPriemSdachaDel*)pReportForm)->Free();
      break;
    case RW_DOCDOPUSKVSTUPISPIT:
      pReportForm=new TFormReportDocDopuskVstupIspit(Application);
//      ((TFormReportDocDopuskVstupIspit*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportDocDopuskVstupIspit*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportDocDopuskVstupIspit*)pReportForm)->Free();
      break;
    case RW_DOCZACHISLENIE:
      pReportForm=new TFormReportDocZachislenie(Application);
//      ((TFormReportDocDopuskVstupIspit*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportDocZachislenie*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportDocZachislenie*)pReportForm)->Free();
      break;
    case RW_SVODNVEDOMOST:
      pReportForm=new TFormReportSvodnVedom(Application);
      ((TFormReportSvodnVedom*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportSvodnVedom*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportSvodnVedom*)pReportForm)->Free();
      break;
    case RW_VEDOMOST:
      pReportForm=new TFormReportVedomWord(Application);
      ((TFormReportVedomWord*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportVedomWord*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportVedomWord*)pReportForm)->Free();
      break;
    case RW_BUCITYMOUNTH:
      pReportForm=new TFormReportBUCityMounth(Application);
//      ((TFormReportBUCityMounth*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportBUCityMounth*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportBUCityMounth*)pReportForm)->Free();
      break;
    case RW_BUGLOBPLANDOLGS:
      pReportForm=new TFormReportBUGlobalPlanDolgs(Application);
//      ((TFormReportBUGlobalPlanDolgs*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportBUGlobalPlanDolgs*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportBUGlobalPlanDolgs*)pReportForm)->Free();
      break;
    case RW_BUCITYDAY:
      pReportForm=new TFormReportBUCityDay(Application);
//      ((TFormReportBUCityDay*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportBUCityDay*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportBUCityDay*)pReportForm)->Free();
      break;
    case RW_BUPLANDOLGSSEMESTR:
      pReportForm=new TFormReportBUPlanDolgsSemestr(Application);
//      ((TFormReportBUPlanDolgsSemestr*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportBUPlanDolgsSemestr*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportBUPlanDolgsSemestr*)pReportForm)->Free();
      break;
    case RE_BULASTREPORT:
      pReportForm=new TFormReportBULast(Application);
      ((TFormReportBULast*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportBULast*)pReportForm)->Free();
      break;
  case RE_BULASTREPORT2:
      pReportForm=new TFormReportBULast2(Application);
      ((TFormReportBULast2*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportBULast2*)pReportForm)->Free();
      break;
    case RW_AVRGBALLINGROUP:
      pReportForm=new TFormReportAvrgBallInGroup(Application);
      ((TFormReportAvrgBallInGroup*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportAvrgBallInGroup*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportAvrgBallInGroup*)pReportForm)->Free();
      break;
    case RW_STUDSCONDREDDIPLOM:
      pReportForm=new TFormReportStudsCandRedDiplom(Application);
//      ((TFormReportStudsCandRedDiplom*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportStudsCandRedDiplom*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportStudsCandRedDiplom*)pReportForm)->Free();
      break;
    case RW_AKADSPRAV:
      pReportForm=new TFormReportAkadSprav(Application);
      ((TFormReportAkadSprav*)pReportForm)->SetIDStudent(_IDStudent);
      ((TFormReportAkadSprav*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportAkadSprav*)pReportForm)->Free();
      break;
    case RW_PRILDIPLOM:
      pReportForm=new TFormReportPrilDiplom(Application);
      ((TFormReportPrilDiplom*)pReportForm)->SetIDStudent(_IDStudent);
      ((TFormReportPrilDiplom*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportPrilDiplom*)pReportForm)->Free();
      break;
    case RW_CHARACTERISTIC:
      pReportForm=new TFormReportCharacteristic(Application);
      ((TFormReportCharacteristic*)pReportForm)->SetIDStudent(_IDStudent);
      ((TFormReportCharacteristic*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportCharacteristic*)pReportForm)->Free();
      break;
    case RE_UCHEBKARTSTUD:
      pReportForm = new TFormReportUchebKartStud(Application);
      ((TFormReportUchebKartStud*)pReportForm)->SetIDStudent(_IDStudent);
      ((TFormReportUchebKartStud*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportUchebKartStud*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportUchebKartStud*)pReportForm)->Free();
      break;
    case RW_PRILDIPLOMNEPOL:
      pReportForm = new TFormReportPrilDiplomNepol(Application);
      ((TFormReportPrilDiplomNepol*)pReportForm)->SetIDStudent(_IDStudent);
      ((TFormReportPrilDiplomNepol*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportPrilDiplomNepol*)pReportForm)->Free();
      break;
    case RW_FORMA3:
      pReportForm = new TFormReportForma3(Application);
      ((TFormReportForma3*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportForma3*)pReportForm)->Free();
      break;
    case RW_STATOTCH:
      pReportForm = new TFormReportStatOtch(Application);
      ((TFormReportStatOtch*)pReportForm)->SetIDGroup(_IDGroup);
      ((TFormReportStatOtch*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportStatOtch*)pReportForm)->Free();
      break;
    case RW_STAT:
      pReportForm = new TFormStatistic(Application);
      pReportForm->ShowModal();
      ((TFormStatistic*)pReportForm)->Free();
      break;
    case RW_EGE:
      pReportForm = new TFormReportEge(Application);
      ((TFormReportEge*)pReportForm)->ToolBtnPreviewClick(pReportForm);
      ((TFormReportEge*)pReportForm)->Free();
      break;
    default:
      break;
   }
}
//---------------------------------------------------------------------------

