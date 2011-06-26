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
                 RQ_VEDOMOST=0,          // экзаменационная ведомость на группу
                 RQ_DOGOVOR_2,           // договор 2
                 RQ_DOGOVOR,             // договор
                 RQ_EKZZACHLIST,         // Экзаменационный (зачетный) лист
                 RQ_PROTOCOL,            // Протокол заседания экз. комиссии
                 RQ_SPRAVKA,             // Справка студенту, что он учится в ВУЗе
                 RQ_SPRAVCARD,           // Справочная карточка студента
                 RW_OPLATAFORGROUP,      // Отчет об олате на группу за период
                 RW_LISTSTUDDOGOVOR,     // Список студентов и номеров договоров (несколько групп)
                 RW_ACTPRIEMSDACHADEL,   // Акт приема сдачи личных дел
                 RW_DOCDOPUSKVSTUPISPIT, // Документ о допуске к вступительным испытаниям
                 RW_DOCZACHISLENIE,      // Документ о зачислении студентов на 1-й курс
                 RW_SVODNVEDOMOST,       // Сводная ведомость об успеваемости в группе
                 RW_VEDOMOST,            // экзаменационная ведомость на группу
                 RW_BUCITYMOUNTH,        // отчет для бухгалтерии (оплата представительств за месяц)
                 RW_BUGLOBPLANDOLGS,     // отчет для бухгалтерии (оплата и долги представительств за уч. год)
                 RW_BUCITYDAY,           // отчет для бухгалтерии (оплата представительств за день)
                 RW_BUPLANDOLGSSEMESTR,  // отчет для бухгалтерии (оплата и долги представительств за семестр)
                 RE_BULASTREPORT,
                 RE_BULASTREPORT2,
                 RW_AVRGBALLINGROUP,     // Вычисление среднего балла для студентов из одной группы
                 RW_STUDSCONDREDDIPLOM,  // Выявление кандидатов на красный диплом
                 RW_AKADSPRAV,           // Академическая справка
                 RW_PRILDIPLOM,          // Приложение к диплому
                 RW_CHARACTERISTIC,      // Характеристика студента (успеваемость)
                 RE_UCHEBKARTSTUD,
                 RW_PRILDIPLOMNEPOL,     // Приложение к диплому о неполном высшем
                 RW_FORMA3,              // Форма 3
                 RW_STATOTCH,            //
                 RW_STAT,                //
                 RW_EGE                  // отчет по ЕГЕ
                };

extern const int CountOfReports;
extern AnsiString arrTitlesOfReports[];
extern AnsiString arrHintsOfReports[];
//---------------------------------------------------------------------------

#endif
