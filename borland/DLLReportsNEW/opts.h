//**************************
//* Education Developement *
//* Options file           *
//**************************
#ifndef __EDSTUDOPTS_H__
#define __EDSTUDOPTS_H__

#include "inifiles.hpp"

#define DefIniFileName ".\\edstud.ini"
//AnsiString DefIniFileName=".\\edstud.ini";

class EDOpts
{
 protected:
   AnsiString FileName;
   TIniFile *myINI;

   AnsiString Crypt(const AnsiString&);
   AnsiString UnCrypt(const AnsiString&);

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
// добавил
   virtual AnsiString GetDateDiplom(void);
   virtual void SetDateDiplom(const AnsiString&);
   virtual AnsiString GetDateQualif(void);
   virtual void SetDateQualif(const AnsiString&);

   virtual AnsiString GetDateOtch(void);
   virtual void SetDateOtch(const AnsiString&);

 public:
  __property AnsiString DBHost = {read=GetDBHost, write=SetDBHost};
  __property AnsiString DBLogin = {read=GetDBLogin, write=SetDBLogin};
  __property AnsiString DBPasswd = {read=GetDBPasswd, write=SetDBPasswd};
  __property AnsiString DBName = {read=GetDBName, write=SetDBName};
  __property AnsiString DBStudTable = {read=GetDBStudTable, write=SetDBStudTable};
  __property AnsiString DBVocTable = {read=GetDBVocTable, write=SetDBVocTable};
  __property AnsiString DBPayFacts = {read=GetDBPayFacts, write=SetDBPayFacts};
  __property AnsiString DBPayOpts = {read=GetDBPayOpts, write=SetDBPayOpts};
  __property AnsiString DBDisciplines = {read=GetDBDisciplines, write=SetDBDisciplines};
  __property AnsiString DBProgress = {read=GetDBProgress, write=SetDBProgress};
  __property bool ACCESSEditVoc = {read=GetACCESSEditVoc, write=SetACCESSEditVoc};
  __property bool ACCESSEditPayOpts = {read=GetACCESSEditPayOpts, write=SetACCESSEditPayOpts};
  __property bool ACCESSEditDisciplines = {read=GetACCESSEditDisciplines, write=SetACCESSEditDisciplines};
  __property int CountDLLs = {read=GetCountDLLs, write=SetCountDLLs};
  // добавил
  __property AnsiString DateDiplom = {read=GetDateDiplom, write=SetDateDiplom};
  __property AnsiString DateQualif = {read=GetDateQualif, write=SetDateQualif};
  __property AnsiString DateOtch = {read=GetDateOtch, write=SetDateOtch};

  virtual AnsiString GetLibraryName(int num);
  virtual void SetLibraryName(int num,const AnsiString& libname);
  void SetFileName(AnsiString& _FileName);
  EDOpts(AnsiString filename);
  EDOpts(void);
};
//////////////////////////////
EDOpts::EDOpts(void)
{
  FileName=DefIniFileName;
}
EDOpts::EDOpts(AnsiString filename)
{
 if(filename.IsEmpty())
   FileName=DefIniFileName;
 else
   FileName=filename;
}

void EDOpts::SetFileName(AnsiString& _FileName)
{
  FileName=_FileName;
}

//host

AnsiString EDOpts::GetDBHost()
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DB","host","localhost");
  delete myINI;
  return readstr;
}

void EDOpts::SetDBHost(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DB","host",str);
  delete myINI;
}

//login

AnsiString EDOpts::GetDBLogin()
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DB","login","root");
  delete myINI;
  return readstr;
}

void EDOpts::SetDBLogin(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DB","login",str);
  delete myINI;
}

//passwd

AnsiString EDOpts::GetDBPasswd()
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DB","passwd","");
  delete myINI;
  return readstr;
}

void EDOpts::SetDBPasswd(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DB","passwd",str);
  delete myINI;
}

//name

AnsiString EDOpts::GetDBName()
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DB","name","mysql");
  delete myINI;
  return readstr;
}

void EDOpts::SetDBName(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DB","name",str);
  delete myINI;
}

//studtable

AnsiString EDOpts::GetDBStudTable()
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DB","studtable","students");
  delete myINI;
  return readstr;
}

void EDOpts::SetDBStudTable(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DB","studtable",str);
  delete myINI;
}

//studtable

AnsiString EDOpts::GetDBVocTable()
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DB","voctable","voc");
  delete myINI;
  return readstr;
}

void EDOpts::SetDBVocTable(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DB","voctable",str);
  delete myINI;
}

//PayFactstable

AnsiString EDOpts::GetDBPayFacts()
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DB","payfactstable","payfacts");
  delete myINI;
  return readstr;
}

void EDOpts::SetDBPayFacts(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DB","payfactstable",str);
  delete myINI;
}

//PayOptstable

AnsiString EDOpts::GetDBPayOpts()
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DB","payoptstable","payopts");
  delete myINI;
  return readstr;
}

void EDOpts::SetDBPayOpts(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DB","payoptstable",str);
  delete myINI;
}

//Disciplinestable

AnsiString EDOpts::GetDBDisciplines()
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DB","disciplinestable","disciplines");
  delete myINI;
  return readstr;
}

void EDOpts::SetDBDisciplines(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DB","disciplinestable",str);
  delete myINI;
}

//Progresstable

AnsiString EDOpts::GetDBProgress()
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DB","progresstable","progress");
  delete myINI;
  return readstr;
}

void EDOpts::SetDBProgress(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DB","progresstable",str);
  delete myINI;
}

// editvoc

bool EDOpts::GetACCESSEditVoc(void)
{
  bool readbool;
  myINI=new TIniFile(FileName);
  readbool=myINI->ReadBool("ACCESS","editvoc",false);
  delete myINI;
  return readbool;
}

void EDOpts::SetACCESSEditVoc(bool valbool)
{
  myINI=new TIniFile(FileName);
  myINI->WriteBool("ACCESS","editvoc",valbool);
  delete myINI;
}

// editpayopts

bool EDOpts::GetACCESSEditPayOpts(void)
{
  bool readbool;
  myINI=new TIniFile(FileName);
  readbool=myINI->ReadBool("ACCESS","editpayopts",false);
  delete myINI;
  return readbool;
}

void EDOpts::SetACCESSEditPayOpts(bool valbool)
{
  myINI=new TIniFile(FileName);
  myINI->WriteBool("ACCESS","editpayopts",valbool);
  delete myINI;
}

// editdisciplines

bool EDOpts::GetACCESSEditDisciplines(void)
{
  bool readbool;
  myINI=new TIniFile(FileName);
  readbool=myINI->ReadBool("ACCESS","editdisciplines",false);
  delete myINI;
  return readbool;
}

void EDOpts::SetACCESSEditDisciplines(bool valbool)
{
  myINI=new TIniFile(FileName);
  myINI->WriteBool("ACCESS","editdisciplines",valbool);
  delete myINI;
}

//CountDLLs - name of program

int EDOpts::GetCountDLLs(void)
{
  int readint;
  myINI=new TIniFile(FileName);
  readint=myINI->ReadInteger("DLL","count",0);
  delete myINI;
  return readint;
}

void EDOpts::SetCountDLLs(int count)
{
  myINI=new TIniFile(FileName);
  myINI->WriteInteger("DLL","count",count);
  delete myINI;
}

AnsiString EDOpts::GetLibraryName(int num)
{
  AnsiString Key="lib"+AnsiString(num);
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DLL",Key,"");
  delete myINI;
  return readstr;
}

void EDOpts::SetLibraryName(int num,const AnsiString& libname)
{
  AnsiString Key="lib"+AnsiString(num);
  myINI=new TIniFile(FileName);
  myINI->WriteString("DLL",Key,libname);
  delete myINI;
}

//crypt
AnsiString EDOpts::Crypt(const AnsiString &str)
{
 AnsiString NewStr="";
 char ch;
 int i;
 for (i=1;i<=str.Length();i+=2) NewStr+=str[i];
 for (i=2;i<=str.Length();i+=2) NewStr+=str[i];
 for (i=1;i<=NewStr.Length();i++)
 {
   ch=NewStr[i];
   ch=255-ch;
   NewStr[i]=ch;
 }
 return NewStr;
}

AnsiString EDOpts::UnCrypt(const AnsiString &str)
{
 AnsiString NewStr="";
 AnsiString RealStr="";
 char ch;
 int i;
 for (int i=1;i<=str.Length();i++)
 {
   ch=str[i];
   ch=255-ch;
   NewStr+=ch;
   RealStr+=ch;
 }
 int Len=NewStr.Length();
 int div=(Len%2==0) ? (Len/2) : (Len/2+1);
 for (i=1;i<=div;i++) RealStr[i*2-2+1]=NewStr[i];
 for (i=div+1;i<=Len;i++) RealStr[i*2-div*2]=NewStr[i];
 return RealStr;
}
// добавил

AnsiString EDOpts::GetDateDiplom(void)
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DATE","date_diplom","19.09.2004");
  delete myINI;
  return readstr;
}

void EDOpts::SetDateDiplom(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DATE","date_diplom",str);
  delete myINI;
}

AnsiString EDOpts::GetDateQualif(void)
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DATE","date_qualif","19.09.2004");
  delete myINI;
  return readstr;
}

void EDOpts::SetDateQualif(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DATE","date_qualif",str);
  delete myINI;
}

AnsiString EDOpts::GetDateOtch(void)
{
  AnsiString readstr;
  myINI=new TIniFile(FileName);
  readstr=myINI->ReadString("DATE","date_otch","19.09.2004");
  delete myINI;
  return readstr;
}

void EDOpts::SetDateOtch(const AnsiString &str)
{
  myINI=new TIniFile(FileName);
  myINI->WriteString("DATE","date_otch",str);
  delete myINI;
}

#endif
