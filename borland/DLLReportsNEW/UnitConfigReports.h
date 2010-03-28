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
#include ".\\UnitRep\\UnitReportUchebKartStud.h"
#include ".\\UnitRep\\UnitReportPrilDiplomNepol.h"
#include ".\\UnitRep\\UnitReportForma3.h"
#include ".\\UnitRep\\UnitReportStatOtch.h"
#include ".\\UnitRep\\UnitStatistic.h"
//---------------------------------------------------------------------------
enum TTypeReport{
                 RQ_VEDOMOST=0,             // ��������������� ��������� �� ������
                 RQ_DOGOVOR=1,              // �������
                 RQ_EKZZACHLIST=2,          // ��������������� (��������) ����
                 RQ_PROTOCOL=3,             // �������� ��������� ���. ��������
                 RQ_SPRAVKA=4,              // ������� ��������, ��� �� ������ � ����
                 RQ_SPRAVCARD=5,            // ���������� �������� ��������
                 RW_OPLATAFORGROUP=6,       // ����� �� ����� �� ������ �� ������
                 RW_LISTSTUDDOGOVOR=7,      // ������ ��������� � ������� ��������� (��������� �����)
                 RW_ACTPRIEMSDACHADEL=8,    // ��� ������ ����� ������ ���
                 RW_DOCDOPUSKVSTUPISPIT=9,  // �������� � ������� � ������������� ����������
                 RW_DOCZACHISLENIE=10,      // �������� � ���������� ��������� �� 1-� ����
                 RW_SVODNVEDOMOST=11,       // ������� ��������� �� ������������ � ������
                 RW_VEDOMOST=12,            // ��������������� ��������� �� ������
                 RW_BUCITYMOUNTH=13,        // ����� ��� ����������� (������ ���������������� �� �����)
                 RW_BUGLOBPLANDOLGS=14,     // ����� ��� ����������� (������ � ����� ���������������� �� ��. ���)
                 RW_BUCITYDAY=15,           // ����� ��� ����������� (������ ���������������� �� ����)
                 RW_BUPLANDOLGSSEMESTR=16,  // ����� ��� ����������� (������ � ����� ���������������� �� �������)
                 RW_AVRGBALLINGROUP=17,     // ���������� �������� ����� ��� ��������� �� ����� ������
                 RW_STUDSCONDREDDIPLOM=18,  // ��������� ���������� �� ������� ������
                 RW_AKADSPRAV=19,           // ������������� �������
                 RW_PRILDIPLOM=20,          // ���������� � �������
                 RW_CHARACTERISTIC=21,      // �������������� �������� (������������)
                 RE_BULASTREPORT=22,
                 RE_UCHEBKARTSTUD=23,
                 RW_PRILDIPLOMNEPOL=24,     // ���������� � ������� � �������� ������
                 RW_FORMA3=25,              // ����� 3
                 RW_STATOTCH=26,              //
                 RW_STAT=27              //
                };

extern const int CountOfReports;
extern AnsiString arrTitlesOfReports[];
extern AnsiString arrHintsOfReports[];
//---------------------------------------------------------------------------

#endif
