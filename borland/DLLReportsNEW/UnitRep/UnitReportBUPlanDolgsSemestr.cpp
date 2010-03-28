//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportBUPlanDolgsSemestr.h"
#include "MacroWord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"
TFormReportBUPlanDolgsSemestr *FormReportBUPlanDolgsSemestr;

extern EDRegOpts opts;

//---------------------------------------------------------------------------
AnsiString ReturnParsedDateInSQL(AnsiString Str)
{
   // Date in format dd.mm.yyyy convert to yyyy-mm-dd
   AnsiString y=Str.SubString(7,4),
              m=Str.SubString(4,2),
              d=Str.SubString(1,2),
              date;
   date=y+"-"+m+"-"+d;
   return date;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormReportBUPlanDolgsSemestr::GetStartYear(TDateTime dt)
{
  AnsiString Str="";
  unsigned short year,mounth,day;
  dt.DecodeDate(&year,&mounth,&day);

  if (mounth>=9) Str=AnsiString(year);
  else Str=AnsiString(year-1);

  return Str;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUPlanDolgsSemestr::GetStartEndDateSQL(TDateTime dt,AnsiString& StartDate,AnsiString& EndDate)
{
  AnsiString Str;
  unsigned short year,mounth,day;
  dt.DecodeDate(&year,&mounth,&day);
  if (mounth>=9 && mounth<=12)
  {
    StartDate=AnsiString(year)+"-09-01";
    EndDate=AnsiString(year+1)+"-01-31";
  }
  if (mounth==1)
  {
    StartDate=AnsiString(year-1)+"-09-01";
    EndDate=AnsiString(year)+"-01-31";
  }
  if (mounth>=2 && mounth<9)
  {
    StartDate=AnsiString(year)+"-02-01";
    EndDate=AnsiString(year)+"-08-31";
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUPlanDolgsSemestr::GetPrevSemStartDateSQL(TDateTime dt, AnsiString& StartDate)
{
  AnsiString Str;
  unsigned short year,mounth,day;
  dt.DecodeDate(&year,&mounth,&day);
  if (mounth>=9 && mounth<=12)
    StartDate=AnsiString(year)+"-02-01";
  if (mounth==1)
    StartDate=AnsiString(year-1)+"-02-01";
  if (mounth>=2 && mounth<9)
    StartDate=AnsiString(year-1)+"-09-01";
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormReportBUPlanDolgsSemestr::GetPeriodStr(TDateTime dt)
{
  AnsiString Str="";
  unsigned short year,mounth,day;
  dt.DecodeDate(&year,&mounth,&day);

  if ((int(mounth)>=9) && (int(mounth)<=12)) Str="сентябрь "+AnsiString(int(year))+"г. - "+"январь "+AnsiString(int(year)+1)+"г.";
  if (int(mounth)==1) Str="сентябрь "+AnsiString(int(year)-1)+"г. - "+"январь "+AnsiString(int(year))+"г.";
  if ((int(mounth)>=2) && (int(mounth)<9)) Str="февраль "+AnsiString(int(year))+"г. - "+"август "+AnsiString(int(year))+"г.";
/*
  switch (mounth)
  {
    case 1: Str+="январь ";break;
    case 2: Str+="ферваль ";break;
    case 3: Str+="март ";break;
    case 4: Str+="апрель ";break;
    case 5: Str+="май ";break;
    case 6: Str+="июнь ";break;
    case 7: Str+="июль ";break;
    case 8: Str+="август ";break;
    case 9: Str+="сентябрь ";break;
    case 10: Str+="октябрь ";break;
    case 11: Str+="ноябрь ";break;
    case 12: Str+="декабрь ";break;
    default: Str+="??? ";break;
  }
  Str+=AnsiString(year);
  Str+="г.";*/

  return Str;
}
//---------------------------------------------------------------------------
__fastcall TFormReportBUPlanDolgsSemestr::TFormReportBUPlanDolgsSemestr(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUPlanDolgsSemestr::InitReportQuery(void)
{
  ZMySqlQuery->SQL->Clear();
  ZMySqlQuery->SQL->Add("select title,num from "+opts.DBVocTable+" where deleted=0 and vkey=\'city\' order by num");
  ZMySqlQuery->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUPlanDolgsSemestr::InsertNewDolgPrevYearOfCity(AnsiString CityIDStr, AnsiString NumYearStr, int sumdolg)
{
  TDateTime currd;
  currd=currd.CurrentDate();
  AnsiString datestate=ReturnParsedDateInSQL(currd.DateString());

  ZMySqlQueryPrevDolg->SQL->Clear();
  ZMySqlQueryPrevDolg->SQL->Add("select id from \`city_dolgs\` where deleted=0 and cityid="+ToStr(CityIDStr)+" and year="+ToStr(NumYearStr));
  ZMySqlQueryPrevDolg->Active=true;
  int count=ZMySqlQueryPrevDolg->RecordCount;
  if (count==0)
  {
    ZMySqlQueryPrevDolg->SQL->Clear();
    ZMySqlQueryPrevDolg->SQL->Add("insert into \`city_dolgs\` (cityid,year,datestate,dolgsum) values("+ToStr(CityIDStr)+", "+ToStr(NumYearStr)+", "+ToStr(datestate)+", "+ToStr(AnsiString(sumdolg))+")");
    ZMySqlQueryPrevDolg->Active=true;
  }
  else
  {
    ZMySqlQueryPrevDolg->SQL->Clear();
    ZMySqlQueryPrevDolg->SQL->Add("update \`city_dolgs\` set dolgsum="+ToStr(AnsiString(sumdolg))+", datestate="+ToStr(datestate)+" where deleted=0 and cityid="+ToStr(CityIDStr)+" and year="+ToStr(NumYearStr));
    ZMySqlQueryPrevDolg->Active=true;
  }
}
//---------------------------------------------------------------------------
int __fastcall TFormReportBUPlanDolgsSemestr::GetSummPlanCity(AnsiString CityIDStr,int NumSemestr,AnsiString NumYearStr)
{
  int Summ=0;
  AnsiString Pattern1Str="%4"+CityIDStr+"_/_%";
  AnsiString Pattern2Str="%6"+CityIDStr+"_/_%";
  ZMySqlQuerySumm->SQL->Clear();
/*  switch (NumSemestr)
  {
    case 1:
        ZMySqlQuerySumm->Sql->Add("select po.commoncountmoney, voc.num, voc.title from "+opts.DBVocTable+" as voc,"+opts.DBPayOpts+" as po where po.deleted=0 and voc.deleted=0 and voc.vkey=\'grp\' and (voc.title like "+ToStr(Pattern1Str)+" or voc.title like "+ToStr(Pattern2Str)+") and po.idgroup=voc.num and YEAR(po.datestart)="+ToStr(NumYearStr)+" order by voc.num");
        break;
    case 2:
        ZMySqlQuerySumm->Sql->Add("select po.commoncountmoney, voc.num, voc.title from "+opts.DBVocTable+" as voc,"+opts.DBPayOpts+" as po where po.deleted=0 and voc.deleted=0 and voc.vkey=\'grp\' and (voc.title like "+ToStr(Pattern1Str)+" or voc.title like "+ToStr(Pattern2Str)+") and po.idgroup=voc.num and ((YEAR(po.datestart)="+ToStr(NumYearStr)+" and MONTH(po.datestart)=9) or (Year(po.datestart)="+ToStr(AnsiString(NumYearStr.ToInt()+1))+" and MONTH(po.datestart)=2)) order by voc.num");
        break;
  }*/
  ZMySqlQuerySumm->SQL->Add("select po.commoncountmoney, voc.num, voc.title from "+opts.DBVocTable+" as voc,"+opts.DBPayOpts+" as po where po.deleted=0 and voc.deleted=0 and voc.vkey=\'grp\' and (voc.title like "+ToStr(Pattern1Str)+" or voc.title like "+ToStr(Pattern2Str)+") and po.idgroup=voc.num and YEAR(po.datestart)="+ToStr(NumYearStr)+" order by voc.num");

  ZMySqlQuerySumm->Active=true;

  int commoncountmoney_2;
  AnsiString grpidStr;
  int commoncountmoney;
  for (int i=0;i<ZMySqlQuerySumm->RecordCount;i++)
  {
    ZMySqlQuerySumm->RecNo=i+1;
    commoncountmoney=ZMySqlQuerySumm->Fields->FieldByNumber(1)->AsString.ToInt();
    grpidStr=ZMySqlQuerySumm->Fields->FieldByNumber(2)->AsString;

    commoncountmoney_2=commoncountmoney/2;

    ZMySqlQueryCountStuds->SQL->Clear();
    ZMySqlQueryCountStuds->SQL->Add("select count(*) from "+opts.DBStudTable+" where deleted=0 and grpid="+ToStr(grpidStr));
    ZMySqlQueryCountStuds->Active=true;
    ZMySqlQueryCountStuds->RecNo=1;
    Summ += commoncountmoney_2 * ZMySqlQueryCountStuds->Fields->FieldByNumber(1)->AsString.ToInt();
  }
  return Summ;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUPlanDolgsSemestr::CalcPlanYearForCity(AnsiString CityIDStr, AnsiString StartDateSQL,AnsiString StartDateOfPrevSemSQL, vecPlans* vec)
{
  AnsiString Pattern1Str="%4"+CityIDStr+"_/_%";
  AnsiString Pattern2Str="%6"+CityIDStr+"_/_%";
  ZMySqlQuerySumm->SQL->Clear();
  ZMySqlQuerySumm->SQL->Add("select po.commoncountmoney, voc.num, voc.title, po.id, YEAR(po.datestart) from "+opts.DBVocTable+" as voc,"+opts.DBPayOpts+" as po where po.deleted=0 and voc.deleted=0 and voc.vkey=\'grp\' and (voc.title like "+ToStr(Pattern1Str)+" or voc.title like "+ToStr(Pattern2Str)+") and po.idgroup=voc.num and (po.datestart="+ToStr(StartDateSQL)+" or po.datestart="+ToStr(StartDateOfPrevSemSQL)+") order by voc.num");
  ZMySqlQuerySumm->Active=true;

  AnsiString grpidStr="NULL";
  int commoncountmoney;
  int grpid;
  int numstudents;
  int idopts;
  int yearOFDateStart;
  bool september=true;
  for (int a=0;a<vec->size();a++) delete (*vec)[a];
  vec->resize(0);
  for (int i=0;i<ZMySqlQuerySumm->RecordCount;i++)
  {
    ZMySqlQuerySumm->RecNo=i+1;
    if (ZMySqlQuerySumm->Fields->FieldByNumber(2)->AsString==grpidStr) continue;

    commoncountmoney=ZMySqlQuerySumm->Fields->FieldByNumber(1)->AsString.ToInt();
    grpidStr=ZMySqlQuerySumm->Fields->FieldByNumber(2)->AsString;
    grpid=grpidStr.ToInt();
    idopts=ZMySqlQuerySumm->Fields->FieldByNumber(4)->AsString.ToInt();
    yearOFDateStart=ZMySqlQuerySumm->Fields->FieldByNumber(5)->AsString.ToInt();
/*    if (yearOFDateStart==NumYearStr.ToInt()) september=true;
    else september=false;
*/
    ZMySqlQueryCountStuds->SQL->Clear();
    ZMySqlQueryCountStuds->SQL->Add("select count(*) from "+opts.DBStudTable+" where deleted=0 and grpid="+ToStr(grpidStr));
    ZMySqlQueryCountStuds->Active=true;
    ZMySqlQueryCountStuds->RecNo=1;
    numstudents=ZMySqlQueryCountStuds->Fields->FieldByNumber(1)->AsString.ToInt();

    TPlan* newRec=new TPlan(grpid,idopts,september,commoncountmoney,numstudents,yearOFDateStart);
    vec->push_back(newRec);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUPlanDolgsSemestr::CalcFactYearForGroup(AnsiString GrpIDStr, TPlan* plan, vecFacts* vec, AnsiString StartDateSQL, AnsiString EndDateSQL)
{
  ZMySqlQuerySumOplata->SQL->Clear();
  AnsiString QueryStr="select pf.moneypay, YEAR(pf.datepay), MONTH(pf.datepay), st.id from "+opts.DBPayFacts+" as pf, "+opts.DBStudTable+" as st where pf.deleted=0 and st.deleted=0 and st.grpid="+ToStr(GrpIDStr)+" and st.id=pf.idstud and pf.idopts="+ToStr(AnsiString(plan->idopts))+" and pf.datepay>="+ToStr(StartDateSQL)+" and pf.datepay<="+ToStr(EndDateSQL)+" order by st.id";
  ZMySqlQuerySumOplata->SQL->Add(QueryStr);
  ZMySqlQuerySumOplata->Active=true;

  for (int a=0;a<vec->size();a++)
    delete (*vec)[a];
  vec->resize(0);
  int summ1=0;
  int summ2=0;
  int vtch=0;
  int year,month;
  int idstud=-1;
  for (int i=0;i<ZMySqlQuerySumOplata->RecordCount;i++)
  {
    ZMySqlQuerySumOplata->RecNo=i+1;
    year=ZMySqlQuerySumOplata->Fields->FieldByNumber(2)->AsString.ToInt();
    month=ZMySqlQuerySumOplata->Fields->FieldByNumber(3)->AsString.ToInt();
    if (ZMySqlQuerySumOplata->Fields->FieldByNumber(4)->AsString.ToInt()!=idstud && i>0)
    {
        TFact* new_fact=new TFact(summ1,summ2,vtch);
        vec->push_back(new_fact);

        summ1=0;
        summ2=0;
        vtch=0;
    }
    idstud=ZMySqlQuerySumOplata->Fields->FieldByNumber(4)->AsString.ToInt();
    summ1+=ZMySqlQuerySumOplata->Fields->FieldByNumber(1)->AsString.ToInt();
    /*if (plan->september)
    {
      if ((year<=plan->year_start) || (year==plan->year_start+1 && month<=1))
        summ1+=ZMySqlQuerySumOplata->Fields->FieldByNumber(1)->AsString.ToInt();
      else
        summ2+=ZMySqlQuerySumOplata->Fields->FieldByNumber(1)->AsString.ToInt();
    }
    else
    {
      if ((year<plan->year_start) || (year==plan->year_start && month<=8))
        summ1+=ZMySqlQuerySumOplata->Fields->FieldByNumber(1)->AsString.ToInt();
      else
        summ2+=ZMySqlQuerySumOplata->Fields->FieldByNumber(1)->AsString.ToInt();
    }*/
    /*if (summ1>plan->countmoney/2)
    {
      vtch=summ1-plan->countmoney/2;
      summ1=plan->countmoney/2;
      summ2+=vtch;
    }*/
  }
  if (ZMySqlQuerySumOplata->RecordCount>0)
  {
    TFact* new_fact=new TFact(summ1,summ2,vtch);
    vec->push_back(new_fact);
  }
}
//---------------------------------------------------------------------------
int __fastcall TFormReportBUPlanDolgsSemestr::GetSummOplats1(AnsiString CityIDStr,AnsiString NumYearStr)
{
  AnsiString Pattern1Str="%4"+CityIDStr+"_/_%";
  AnsiString Pattern2Str="%6"+CityIDStr+"_/_%";
  ZMySqlQuerySumOplata->SQL->Clear();

  AnsiString QueryStr="select pf.moneypay, voc.title from "+opts.DBVocTable+" as voc,"+opts.DBPayFacts+" as pf,"+opts.DBStudTable+" as st where pf.deleted=0 and voc.deleted=0 and st.deleted=0 and voc.vkey=\'grp\' and (voc.title like "+ToStr(Pattern1Str)+" or voc.title like "+ToStr(Pattern2Str)+") and st.grpid=voc.num and st.id=pf.idstud and ((YEAR(pf.datepay)="+ToStr(NumYearStr)+" and MONTH(pf.datepay)>=9) or (YEAR(pf.datepay)="+ToStr(AnsiString(NumYearStr.ToInt()+1))+" and MONTH(pf.datepay)<=1))";
  ZMySqlQuerySumOplata->SQL->Add(QueryStr);
  ZMySqlQuerySumOplata->Active=true;

  int summoney=0;
  AnsiString grpidStr;
  for (int i=0;i<ZMySqlQuerySumOplata->RecordCount;i++)
  {
    ZMySqlQuerySumOplata->RecNo=i+1;
    summoney+=ZMySqlQuerySumOplata->Fields->FieldByNumber(1)->AsString.ToInt();
//    grpidStr=ZMySqlQuerySumOplata->Fields->FieldByNumber(2)->AsString;
  }
  return summoney;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportBUPlanDolgsSemestr::GetSummOplats2(AnsiString CityIDStr,AnsiString NumYearStr)
{
  AnsiString Pattern1Str="%4"+CityIDStr+"_/_%";
  AnsiString Pattern2Str="%6"+CityIDStr+"_/_%";
  ZMySqlQuerySumOplata->SQL->Clear();

  AnsiString QueryStr="select pf.moneypay, voc.title from "+opts.DBVocTable+" as voc,"+opts.DBPayFacts+" as pf,"+opts.DBStudTable+" as st where pf.deleted=0 and voc.deleted=0 and st.deleted=0 and voc.vkey=\'grp\' and (voc.title like "+ToStr(Pattern1Str)+" or voc.title like "+ToStr(Pattern2Str)+") and st.grpid=voc.num and st.id=pf.idstud and ((YEAR(pf.datepay)="+ToStr(NumYearStr)+" and MONTH(pf.datepay)>=9) or (YEAR(pf.datepay)="+ToStr(AnsiString(NumYearStr.ToInt()+1))+" and MONTH(pf.datepay)<=8))";
  ZMySqlQuerySumOplata->SQL->Add(QueryStr);
  ZMySqlQuerySumOplata->Active=true;

  int summoney=0;
  AnsiString grpidStr;
  for (int i=0;i<ZMySqlQuerySumOplata->RecordCount;i++)
  {
    ZMySqlQuerySumOplata->RecNo=i+1;
    summoney+=ZMySqlQuerySumOplata->Fields->FieldByNumber(1)->AsString.ToInt();
//    grpidStr=ZMySqlQuerySumOplata->Fields->FieldByNumber(2)->AsString;
  }
  return summoney;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUPlanDolgsSemestr::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUPlanDolgsSemestr::FormClose(TObject *Sender,
      TCloseAction &Action)
{

  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUPlanDolgsSemestr::CreateWordDocument(void)
{
    InitReportQuery();

    if (ZMySqlQuery->RecordCount==0)
        return;

    WordMacros macros;
    macros.BeginMacros();

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.SelectionFont("Size=20");
    macros.SelectionFont("Bold=true");
    macros.SelectionText("Отчет по оплате и долгам представительств");
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Size=12");
    macros.SelectionFont("Bold=false");
    TDateTime DateRep=pFormSelDate->GetSelectedDate();
    macros.SelectionText("на "+DateRep.DateString());
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.SelectionText("Отчетный период: ");
    macros.SelectionFont("Bold=true");
    AnsiString PeriodStr=GetPeriodStr(DateRep);
//  WordApplication->Selection->TypeText(TVariant(GetPeriodStr(DateRep)+"\n\n"));
    macros.SelectionText(PeriodStr);
    macros.SelectionTypeParagraph();
    macros.SelectionTypeParagraph();
    macros.SelectionFont("Bold=false");

    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");

  // Формирование таблицы
    AnsiString Str,Strtmp;
    int CountTables=0;
    int CountRows=ZMySqlQuery->RecordCount;
    macros.TablesAdd(CountRows+1, 5);
    CountTables++;

    macros.TablesColumns(CountTables, 1, "Width = 30");
    macros.TablesColumns(CountTables, 2, "Width = 150");
    macros.TablesColumns(CountTables, 3, "Width = 90");
    macros.TablesColumns(CountTables, 4, "Width = 90");
    macros.TablesColumns(CountTables, 5, "Width = 90");

    macros.TablesColumns(CountTables, 1, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    macros.TablesColumns(CountTables, 2, "Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphLeft");
    for (int k=3;k<=5;k++)
    {
        macros.TablesColumns(CountTables, k, "Select");
        macros.SelectionParagraphFormat("Alignment = wdAlignParagraphRight");
    }
    macros.InsertLine("ActiveDocument.Tables.Item(" + IntToStr(CountTables) + ").Range.Font.Size = 10");

    macros.InsertLine("ActiveDocument.Tables.Item(" + IntToStr(CountTables) + ").Rows.Item(1).Range.Font.Bold=true");
//  WordDocument->Tables->Item(CountTables)->Range->Select();

    macros.TablesCell(CountTables, 1, 1, "Range.Text = \"№\"");
    macros.TablesCell(CountTables, 1, 1, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(CountTables, 1, 2, "Range.Text = \"Представительство\"");
    macros.TablesCell(CountTables, 1, 2, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(CountTables, 1, 3, "Range.Text = \"План\"");
    macros.TablesCell(CountTables, 1, 3, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(CountTables, 1, 4, "Range.Text = \"Уплачено\"");
    macros.TablesCell(CountTables, 1, 4, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");
    macros.TablesCell(CountTables, 1, 5, "Range.Text = \"Долг\"");
    macros.TablesCell(CountTables, 1, 5, "Range.Select");
    macros.SelectionParagraphFormat("Alignment = wdAlignParagraphCenter");

    unsigned short year,month,day;
    DateRep.DecodeDate(&year,&month,&day);
    int Plan,
      Uplacheno,
      Dolg;
//  long commsumm=0;
    AnsiString StartYear=GetStartYear(DateRep);
    AnsiString StartDateSQL, EndDateSQL;
    GetStartEndDateSQL(DateRep, StartDateSQL, EndDateSQL);
    AnsiString SrartDateOfPrevSemestr;
    GetPrevSemStartDateSQL(DateRep,SrartDateOfPrevSemestr);
    for (int i=0;i<ZMySqlQuery->RecordCount;i++)
    {
        ZMySqlQuery->RecNo=i+1;

        AnsiString cityidStr=ZMySqlQuery->Fields->FieldByNumber(2)->AsString;

        macros.TablesCell(CountTables, i+2, 1, "Range.Text = \"" + AnsiString(i+1) + "\"");
        macros.TablesCell(CountTables, i+2, 2, "Range.Text = \"" + ZMySqlQuery->Fields->FieldByNumber(1)->AsString + "\"");
        vecPlans* vec=new vecPlans(0);
        vecFacts* vecf=new vecFacts(0);
        vec->resize(0);vecf->resize(0);
        CalcPlanYearForCity(ZMySqlQuery->Fields->FieldByNumber(2)->AsString,StartDateSQL,SrartDateOfPrevSemestr,vec);
        Plan=0;
        int a;
        for (a=0;a<vec->size();a++) Plan+=(*vec)[a]->countmoney*(*vec)[a]->numstuds;
        Plan/=2;
        macros.TablesCell(CountTables, i+2, 3, "Range.Text = \"" + AnsiString(Plan) + "\"");
        Uplacheno=0;
        for (a=0;a<vec->size();a++)
        {
            CalcFactYearForGroup(AnsiString((*vec)[a]->idgrp), (*vec)[a], vecf, StartDateSQL, EndDateSQL);
            for (int b=0;b<vecf->size();b++)
            {
                Uplacheno+=(*vecf)[b]->summ1;
            }
        }
        Dolg=(Plan)-(Uplacheno);
        Dolg = (Dolg>0)? Dolg : 0;

        macros.TablesCell(CountTables, i+2, 4, "Range.Text = \"" + AnsiString(Uplacheno) + "\"");
        macros.TablesCell(CountTables, i+2, 5, "Range.Text = \"" + AnsiString(Dolg) + "\"");

        for (a=0;a<vec->size();a++)
            delete (*vec)[a];
        for (a=0;a<vecf->size();a++)
            delete (*vecf)[a];

//    commsumm+=SummStr.ToInt();
    }

    macros.TablesCell(CountTables, CountRows+1, 1, "Range.Select");
    macros.InsertLine("Selection.MoveDown Unit := wdLine");
    
    macros.EndMacros();
    macros.RunMacros();
//  WordApplication->Selection->TypeText(TVariant(AnsiString("\nИтого: общая сумма зафиксированных выплат со стороны всех представительств составляет "+AnsiString(commsumm)+" рублей.")));
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUPlanDolgsSemestr::ToolBtnPreviewClick(
      TObject *Sender)
{
  try
  {
    pFormSelDate->ShowModal();

    CreateWordDocument();
  }
  catch(...)
  {
    Application->MessageBox("Работа с MS Word не возможна. Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUPlanDolgsSemestr::FormCreate(TObject *Sender)
{
  pFormSelDate = new TFormSelectDate(this);

  bool NoError;
  try
  {
    ZMySqlDatabase->HostName=opts.DBHost;
    ZMySqlDatabase->User=opts.DBLogin;
    ZMySqlDatabase->Password=opts.DBPasswd;
    ZMySqlDatabase->Database=opts.DBName;
    ZMySqlDatabase->Connected=true;

    NoError=ZMySqlDatabase->Connected;
  }
  catch(...)
  {
    NoError=ZMySqlDatabase->Connected;
  }

  NoError=NoError && WCConnect();
  if ( !NoError)
  {
    AnsiString msg="Ошибочное обращение к серверу "+opts.DBHost+" или к базе данных "+opts.DBName+".\nОбратитесь к системному администратору.";
    MessageBox(Handle,msg.c_str(),"Ошибка",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportBUPlanDolgsSemestr::FormDestroy(TObject *Sender)
{
  pFormSelDate->Free();
  WCDisconnect();
}
//---------------------------------------------------------------------------
