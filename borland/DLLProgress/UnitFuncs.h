//---------------------------------------------------------------------------

#ifndef UnitFuncsH
#define UnitFuncsH

#include "myregini.h"

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
                  DISCIPCLASSIFIC=14
                };

AnsiString ToStr(AnsiString str);
bool WCConnect(void);
void WCDisconnect(void);
AnsiString WCGetTitleForKeyNum(TTypeKeyVoc tkey,int num);
int WCGetNumForKeyTitle(TTypeKeyVoc tkey, AnsiString title);
void WCUpdateComboBox(TComboBox* cmb,TTypeKeyVoc tkey);
//---------------------------------------------------------------------------
#endif
 