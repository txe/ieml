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
                 RQ_VEDOMOST=0,             // экзаменационная ведомость на группу
                 RQ_DOGOVOR=1,              // договор
                 RQ_EKZZACHLIST=2,          // Экзаменационный (зачетный) лист
                 RQ_PROTOCOL=3,             // Протокол заседания экз. комиссии
                 RQ_SPRAVKA=4,              // Справка студенту, что он учится в ВУЗе
                 RQ_SPRAVCARD=5,            // Справочная карточка студента
                 RW_OPLATAFORGROUP=6,       // Отчет об олате на группу за период
                 RW_LISTSTUDDOGOVOR=7,      // Список студентов и номеров договоров (несколько групп)
                 RW_ACTPRIEMSDACHADEL=8,    // Акт приема сдачи личных дел
                 RW_DOCDOPUSKVSTUPISPIT=9,  // Документ о допуске к вступительным испытаниям
                 RW_DOCZACHISLENIE=10,      // Документ о зачислении студентов на 1-й курс
                 RW_SVODNVEDOMOST=11,       // Сводная ведомость об успеваемости в группе
                 RW_VEDOMOST=12,            // экзаменационная ведомость на группу
                 RW_BUCITYMOUNTH=13,        // отчет для бухгалтерии (оплата представительств за месяц)
                 RW_BUGLOBPLANDOLGS=14,     // отчет для бухгалтерии (оплата и долги представительств за уч. год)
                 RW_BUCITYDAY=15,           // отчет для бухгалтерии (оплата представительств за день)
                 RW_BUPLANDOLGSSEMESTR=16,  // отчет для бухгалтерии (оплата и долги представительств за семестр)
                 RW_AVRGBALLINGROUP=17,     // Вычисление среднего балла для студентов из одной группы
                 RW_STUDSCONDREDDIPLOM=18,  // Выявление кандидатов на красный диплом
                 RW_AKADSPRAV=19,           // Академическая справка
                 RW_PRILDIPLOM=20,          // Приложение к диплому
                 RW_CHARACTERISTIC=21,      // Характеристика студента (успеваемость)
                 RE_BULASTREPORT=22,
                 RE_UCHEBKARTSTUD=23,
                 RW_PRILDIPLOMNEPOL=24,     // Приложение к диплому о неполном высшем
                 RW_FORMA3=25,              // Форма 3
                 RW_STATOTCH=26,              //
                 RW_STAT=27              //
                };

extern const int CountOfReports;
extern AnsiString arrTitlesOfReports[];
extern AnsiString arrHintsOfReports[];
//---------------------------------------------------------------------------

#endif
