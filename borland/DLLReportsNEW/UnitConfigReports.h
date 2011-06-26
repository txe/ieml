//---------------------------------------------------------------------------

#ifndef UnitConfigReportsH
#define UnitConfigReportsH

#include <vcl.h>
#include <windows.h>
//---------------------------------------------------------------------------

#include ".\\UnitRep\\UnitReportVedom.h"
#include ".\\UnitRep\\UnitReportDogovor.h"
#include ".\\UnitRep\\UnitReportEkzZachList.h"
#include ".\\UnitRep\\UnitReportProtocol.h"
#include ".\\UnitRep\\UnitReportSpravka.h"
#include ".\\UnitRep\\UnitReportSpravCard.h"
#include ".\\UnitRep\\UnitReportDolgsForGroup.h"
#include ".\\UnitRep\\UnitReportListStudDogovor.h"
#include ".\\UnitRep\\UnitReportActPriemSdachaDel.h"
#include ".\\UnitRep\\UnitReportDocDopuskVstupIspit.h"
#include ".\\UnitRep\\UnitReportDocZachislenie.h"
#include ".\\UnitRep\\UnitReportSvodnVedom.h"
#include ".\\UnitRep\\UnitReportVedomWord.h"
#include ".\\UnitRep\\UnitReportBUCityMounth.h"
#include ".\\UnitRep\\UnitReportBUGlobalPlanDolgs.h"
#include ".\\UnitRep\\UnitReportBUPlanDolgsSemestr.h"
#include ".\\UnitRep\\UnitReportBUCityDay.h"
#include ".\\UnitRep\\UnitReportAvrgBallInGroup.h"
#include ".\\UnitRep\\UnitReportStudsCandRedDiplom.h"
#include ".\\UnitRep\\UnitReportAkadSprav.h"
#include ".\\UnitRep\\UnitReportPrilDiplom.h"
#include ".\\UnitRep\\UnitReportCharacteristic.h"
#include ".\\UnitRep\\UnitReportBULast.h"
#include ".\\UnitRep\\UnitReportBULast2.h"
#include ".\\UnitRep\\UnitReportUchebKartStud.h"
#include ".\\UnitRep\\UnitReportPrilDiplomNepol.h"
#include ".\\UnitRep\\UnitReportForma3.h"
#include ".\\UnitRep\\UnitReportStatOtch.h"
#include ".\\UnitRep\\UnitStatistic.h"
#include ".\\UnitRep\\UnitReportEge.h"
//---------------------------------------------------------------------------
enum TTypeReport{
                 RQ_VEDOMOST=0,          // ��������������� ��������� �� ������
                 RQ_DOGOVOR_2,           // ������� 2
                 RQ_DOGOVOR,             // �������
                 RQ_EKZZACHLIST,         // ��������������� (��������) ����
                 RQ_PROTOCOL,            // �������� ��������� ���. ��������
                 RQ_SPRAVKA,             // ������� ��������, ��� �� ������ � ����
                 RQ_SPRAVCARD,           // ���������� �������� ��������
                 RW_OPLATAFORGROUP,      // ����� �� ����� �� ������ �� ������
                 RW_LISTSTUDDOGOVOR,     // ������ ��������� � ������� ��������� (��������� �����)
                 RW_ACTPRIEMSDACHADEL,   // ��� ������ ����� ������ ���
                 RW_DOCDOPUSKVSTUPISPIT, // �������� � ������� � ������������� ����������
                 RW_DOCZACHISLENIE,      // �������� � ���������� ��������� �� 1-� ����
                 RW_SVODNVEDOMOST,       // ������� ��������� �� ������������ � ������
                 RW_VEDOMOST,            // ��������������� ��������� �� ������
                 RW_BUCITYMOUNTH,        // ����� ��� ����������� (������ ���������������� �� �����)
                 RW_BUGLOBPLANDOLGS,     // ����� ��� ����������� (������ � ����� ���������������� �� ��. ���)
                 RW_BUCITYDAY,           // ����� ��� ����������� (������ ���������������� �� ����)
                 RW_BUPLANDOLGSSEMESTR,  // ����� ��� ����������� (������ � ����� ���������������� �� �������)
                 RE_BULASTREPORT,
                 RE_BULASTREPORT2,
                 RW_AVRGBALLINGROUP,     // ���������� �������� ����� ��� ��������� �� ����� ������
                 RW_STUDSCONDREDDIPLOM,  // ��������� ���������� �� ������� ������
                 RW_AKADSPRAV,           // ������������� �������
                 RW_PRILDIPLOM,          // ���������� � �������
                 RW_CHARACTERISTIC,      // �������������� �������� (������������)
                 RE_UCHEBKARTSTUD,
                 RW_PRILDIPLOMNEPOL,     // ���������� � ������� � �������� ������
                 RW_FORMA3,              // ����� 3
                 RW_STATOTCH,            //
                 RW_STAT,                //
                 RW_EGE                  // ����� �� ���
                };

extern const int CountOfReports;
extern AnsiString arrTitlesOfReports[];
extern AnsiString arrHintsOfReports[];
//---------------------------------------------------------------------------

#endif
