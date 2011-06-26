//---------------------------------------------------------------------------

#ifndef UnitFuncsH
#define UnitFuncsH

#include "myregini.h"
#include <vector>

//---------------------------------------------------------------------------
enum TTypeKeyVoc{
                  GROUPS=0,
                  CITIES=1,
                  SPECS=2,
                  SPEZIALIZS=3,
                  QUALIFIC=4,
                  DOGSHIFRS=5,
                  DOGFASTS=6,
                  DOGYEARS=7,
                  EDUCATIONS=8,
                  EDUDOCS=9,
                  EDUFORMS=10,
                  LANGUAGES=11,
                  FALSEEDUDOCINFOS=12,
                  SHIFRSPECS=13,
                  DISCIPCLASSIFIC=14,
                  DIRECTS=15
                };

AnsiString ToStr(AnsiString str);
bool WCConnect(void);
void WCDisconnect(void);
AnsiString WCGetTitleForKeyNum(TTypeKeyVoc tkey,int num);
int WCGetNumForKeyTitle(TTypeKeyVoc tkey, AnsiString title);
void WCUpdateComboBox(TComboBox* cmb,TTypeKeyVoc tkey);
TStringList* WCGetAllGroupTitles(void);


struct TPlan
{
   int idgrp;
   int idopts;
   bool september;
   int countmoney;
   int numstuds;
   int year_start;

   TPlan(int _idgrp, int _idopts, bool _sept, int _countmoney, int _numstuds, int _year)
   { idgrp = _idgrp; idopts = _idopts; september = _sept; countmoney = _countmoney; numstuds = _numstuds; year_start = _year; }
};

struct TFact
{
   int summ1;
   int vtch;
   int summ2;

   TFact(int _summ1, int _summ2, int _vtch)
   { summ1 = _summ1; summ2 = _summ2; vtch = _vtch; }
};

typedef std::vector<TPlan*> vecPlans;
typedef std::vector<TFact*> vecFacts;



//---------------------------------------------------------------------------
#endif
 