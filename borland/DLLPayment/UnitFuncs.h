//---------------------------------------------------------------------------

#ifndef UnitFuncsH
#define UnitFuncsH

#include "myregini.h"

//---------------------------------------------------------------------------
enum TTypeKeyVoc{
                  GROUPS=0,
                  CITIES=1,
                  SPECS=2,
                  DOGSHIFRS=3,
                  DOGFASTS=4,
                  DOGYEARS=5,
                  EDUCATIONS=6,
                  EDUDOCS=7,
                  EDUFORMS=8,
                  LANGUAGES=9,
                  FALSEEDUDOCINFOS=10,
                  SHIFRSPECS=11
                };

AnsiString ToStr(AnsiString str);
bool WCConnect(void);
void WCDisconnect(void);
AnsiString WCGetTitleForKeyNum(TTypeKeyVoc tkey,int num);
int WCGetNumForKeyTitle(TTypeKeyVoc tkey, AnsiString title);
void WCUpdateComboBox(TComboBox* cmb,TTypeKeyVoc tkey);
//---------------------------------------------------------------------------
#endif
 