//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnitFuncs.h"
#include "mysql.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
AnsiString keys[12]={ "grp",
                      "city",
                      "spec",
                      "dogshifr",
                      "dogfast",
                      "dogyear",
                      "education",
                      "edudoc",
                      "eduform",
                      "language",
                      "falseedudocinfo",
                      "shifrspec"
                     };

EDRegOpts opts;
MYSQL *mysql=NULL;
//---------------------------------------------------------------------------
AnsiString ToStr(AnsiString str)
{
  return "\'"+str+"\'";
}
//---------------------------------------------------------------------------
bool WCConnect(void)
{
   if (mysql!=NULL) return true;

   mysql=NULL;
   mysql=new MYSQL;
   if ( !mysql )
   {
     return false;
   }
   if (mysql_connect(mysql,opts.DBHost.c_str(),opts.DBLogin.c_str(),opts.DBPasswd.c_str()))
   {
     mysql_select_db(mysql,opts.DBName.c_str());
     if (0!=mysql_errno(mysql))
     {
       return false;
     }
   }
   else
   {
     return false;
   }
   return true;
}
//---------------------------------------------------------------------------
void WCDisconnect(void)
{
  if (mysql!=NULL)
  {
    mysql_close(mysql);
    delete mysql;
    mysql=NULL;
  }
}
//---------------------------------------------------------------------------
AnsiString WCGetTitleForKeyNum(TTypeKeyVoc tkey,int num)
{
  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString resStr="";
  AnsiString myquery;
  myquery="SELECT title FROM "+opts.DBVocTable+" WHERE deleted=0 AND vkey="+ToStr(keys[tkey])+" AND num="+ToStr(AnsiString(num));

  mysql_query(mysql,myquery.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      row = mysql_fetch_row(result);

      resStr=AnsiString(row[0]);
    }
    mysql_free_result(result);
  }

  return resStr;
}
//---------------------------------------------------------------------------
int WCGetNumForKeyTitle(TTypeKeyVoc tkey, AnsiString title)
{
  MYSQL_RES *result;
  MYSQL_ROW row;

  int resNum=0;
  AnsiString myquery;
  myquery="SELECT num FROM "+opts.DBVocTable+" WHERE deleted=0 AND vkey="+ToStr(keys[tkey])+" AND title="+ToStr(title);

  mysql_query(mysql,myquery.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      row = mysql_fetch_row(result);

      resNum=AnsiString(row[0]).ToInt();
    }
    mysql_free_result(result);
  }

  return resNum;
}
//---------------------------------------------------------------------------
void WCUpdateComboBox(TComboBox* cmb,TTypeKeyVoc tkey)
{
  cmb->Clear();

  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString myquery;
  if (tkey==GROUPS)
    myquery="SELECT title, num FROM "+opts.DBVocTable+" WHERE vkey="+ToStr(keys[tkey])+" AND deleted=0 ORDER BY title";
  else
    myquery="SELECT title, num FROM "+opts.DBVocTable+" WHERE vkey="+ToStr(keys[tkey])+" AND deleted=0 ORDER BY num";
  mysql_query(mysql,myquery.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      while (row = mysql_fetch_row(result))
      {
        cmb->Items->Add(AnsiString(row[0]));
      }
    }
    mysql_free_result(result);
  }
}
//---------------------------------------------------------------------------

