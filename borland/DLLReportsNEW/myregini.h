//**************************
//* Education Developement *
//* Register Options file  *
//**************************
#ifndef __EDREGOPTS_H__
#define __EDREGOPTS_H__

#include <registry.hpp>
#include "opts.h"

#define DefPath "ODOBase"
#define DefKeyDB "DB"
#define DefKeyDLL "DLL"
#define DefKeyACCESS "ACCESS"
#define DefKeyDATE "DATE"
#define DefKeyOTHER "OTHER"

const HKEY MyRootKey=HKEY_CURRENT_USER;

class EDRegOpts: public EDOpts
{
   private:
     TRegIniFile* MyRegIniFile;

   protected:
     virtual AnsiString GetDBHost();
     virtual void SetDBHost(const AnsiString&);
     virtual AnsiString GetDBLogin();
     virtual void SetDBLogin(const AnsiString&);
     virtual AnsiString GetDBPasswd();
     virtual void SetDBPasswd(const AnsiString&);
     virtual AnsiString GetDBName();
     virtual void SetDBName(const AnsiString&);

     virtual AnsiString GetDBStudTable();
     virtual void SetDBStudTable(const AnsiString&);
     virtual AnsiString GetDBVocTable();
     virtual void SetDBVocTable(const AnsiString&);
     virtual AnsiString GetDBPayFacts();
     virtual void SetDBPayFacts(const AnsiString&);
     virtual AnsiString GetDBPayOpts();
     virtual void SetDBPayOpts(const AnsiString&);
     virtual AnsiString GetDBDisciplines();
     virtual void SetDBDisciplines(const AnsiString&);
     virtual AnsiString GetDBProgress();
     virtual void SetDBProgress(const AnsiString&);

     virtual bool GetACCESSEditVoc(void);
     virtual void SetACCESSEditVoc(bool);
     virtual bool GetACCESSEditPayOpts(void);
     virtual void SetACCESSEditPayOpts(bool);
     virtual bool GetACCESSEditDisciplines(void);
     virtual void SetACCESSEditDisciplines(bool);

     virtual int GetCountDLLs(void);
     virtual void SetCountDLLs(int count);
//   добавил
     virtual AnsiString GetDateDiplom(void);
     virtual void SetDateDiplom(const AnsiString&);
     virtual AnsiString GetDateQualif(void);
     virtual void SetDateQualif(const AnsiString&);

     virtual AnsiString GetDateOtch(void);
     virtual void SetDateOtch(const AnsiString&);

   public:
     virtual AnsiString GetLibraryName(int num);
     virtual void SetLibraryName(int num,const AnsiString& libname);

     AnsiString GetDeltaDogovor(bool first);
     void SetDeltaDogovor(AnsiString& param);

};

AnsiString EDRegOpts::GetDBHost(void)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDB,"host","localhost");
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDBHost(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDB,"host",Str);
  delete MyRegIniFile;
}
AnsiString EDRegOpts::GetDBLogin(void)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDB,"login",Crypt("root"));
  Str=UnCrypt(Str);
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDBLogin(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDB,"login",Crypt(Str));
  delete MyRegIniFile;
}
AnsiString EDRegOpts::GetDBPasswd(void)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDB,"passwd",Crypt("pass"));
  Str=UnCrypt(Str);
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDBPasswd(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDB,"passwd",Crypt(Str));
  delete MyRegIniFile;
}
AnsiString EDRegOpts::GetDBName(void)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDB,"name","odoportal");
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDBName(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDB,"name",Str);
  delete MyRegIniFile;
}
AnsiString EDRegOpts::GetDBStudTable()
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDB,"studtable","students");
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDBStudTable(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDB,"studtable",Str);
  delete MyRegIniFile;
}
AnsiString EDRegOpts::GetDBVocTable(void)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDB,"voctable","voc");
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDBVocTable(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDB,"voctable",Str);
  delete MyRegIniFile;
}
AnsiString EDRegOpts::GetDBPayFacts(void)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDB,"payfactstable","payfactstest");
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDBPayFacts(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDB,"payfactstable",Str);
  delete MyRegIniFile;
}
AnsiString EDRegOpts::GetDBPayOpts(void)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDB,"payoptstable","payoptstest");
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDBPayOpts(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDB,"payoptstable",Str);
  delete MyRegIniFile;
}
AnsiString EDRegOpts::GetDBDisciplines(void)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDB,"disciplinestable","disciplines");
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDBDisciplines(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDB,"disciplinestable",Str);
  delete MyRegIniFile;
}
AnsiString EDRegOpts::GetDBProgress(void)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDB,"progresstable","progress");
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDBProgress(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDB,"progresstable",Str);
  delete MyRegIniFile;
}
bool EDRegOpts::GetACCESSEditVoc(void)
{
  bool readbool;
  int readint;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  readint=MyRegIniFile->ReadString(DefKeyACCESS,"editvoc","1").ToInt();
  readbool = (readint==1) ? true : false;
  delete MyRegIniFile;
  return readbool;
}
void EDRegOpts::SetACCESSEditVoc(bool valbool)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyACCESS,"editvoc",((valbool)?"1":"0"));
  delete MyRegIniFile;
}
bool EDRegOpts::GetACCESSEditPayOpts(void)
{
  bool readbool;
  int readint;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  readint=MyRegIniFile->ReadString(DefKeyACCESS,"editpayopts","1").ToInt();
  readbool = (readint==1) ? true : false;
  delete MyRegIniFile;
  return readbool;
}
void EDRegOpts::SetACCESSEditPayOpts(bool valbool)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyACCESS,"editpayopts",((valbool)?"1":"0"));
  delete MyRegIniFile;
}
bool EDRegOpts::GetACCESSEditDisciplines(void)
{
  bool readbool;
  int readint;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  readint=MyRegIniFile->ReadString(DefKeyACCESS,"editdisciplines","1").ToInt();
  readbool = (readint==1) ? true : false;
  delete MyRegIniFile;
  return readbool;
}
void EDRegOpts::SetACCESSEditDisciplines(bool valbool)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyACCESS,"editdisciplines",((valbool)?"1":"0"));
  delete MyRegIniFile;
}
int EDRegOpts::GetCountDLLs(void)
{
  int readint;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  readint=MyRegIniFile->ReadString(DefKeyDLL,"count","2").ToInt();
  delete MyRegIniFile;
  return readint;
}
void EDRegOpts::SetCountDLLs(int count)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDLL,"count",AnsiString(count));
  delete MyRegIniFile;
}
AnsiString EDRegOpts::GetLibraryName(int num)
{
  AnsiString Ident="lib"+AnsiString(num);
  AnsiString readstr;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  readstr=MyRegIniFile->ReadString(DefKeyDLL,Ident,"");
  delete MyRegIniFile;
  return readstr;
}
void EDRegOpts::SetLibraryName(int num,const AnsiString& libname)
{
  AnsiString Ident="lib"+AnsiString(num);
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDLL,Ident,libname);
  delete MyRegIniFile;
}
// добавил
AnsiString EDRegOpts::GetDateDiplom(void)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDATE,"date_diplom","19.09.2004");
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDateDiplom(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDATE,"date_diplom",Str);
  delete MyRegIniFile;
}
AnsiString EDRegOpts::GetDateQualif(void)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDATE,"date_qualif","19.09.2004");
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDateQualif(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDATE,"date_qualif",Str);
  delete MyRegIniFile;
}

AnsiString EDRegOpts::GetDeltaDogovor(bool first)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyOTHER, first ? "deltadogovor_1" : "deltadogovor_2","0");
  delete MyRegIniFile;
  return Str;
}
void EDRegOpts::SetDeltaDogovor(AnsiString& param)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyOTHER,"deltadogovor", param);
  delete MyRegIniFile;
}

AnsiString EDRegOpts::GetDateOtch(void)
{
  AnsiString Str;
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,false);
  Str=MyRegIniFile->ReadString(DefKeyDATE,"date_otch","19.09.2004");
  delete MyRegIniFile;
  return Str;
}

void EDRegOpts::SetDateOtch(const AnsiString& Str)
{
  MyRegIniFile=new TRegIniFile(DefPath);
  MyRegIniFile->RootKey=MyRootKey;
  MyRegIniFile->OpenKey(DefPath,true);
  MyRegIniFile->WriteString(DefKeyDATE,"date_otch",Str);
  delete MyRegIniFile;
}
#endif