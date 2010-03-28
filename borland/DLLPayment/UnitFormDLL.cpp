//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitFormDLL.h"
#include "UnitOptsOplataForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDLL *FormDLL;

extern EDRegOpts opts;
extern MYSQL *mysql;

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
AnsiString ReturnParsedDateInMask(AnsiString Str)
{
   // Date in format yyyy-mm-dd convert to dd.mm.yyyy
   AnsiString y=Str.SubString(1,4),
              m=Str.SubString(6,2),
              d=Str.SubString(9,2),
              date;
   date=d+"."+m+"."+y;
   return date;
}
//---------------------------------------------------------------------------
void __fastcall TFormDLL::SetFilterCheck(const int& flag)
{
    filter_checker = flag;
}
//---------------------------------------------------------------------------
int __fastcall TFormDLL::GetFilterCheck()
{
   return filter_checker;
}
//---------------------------------------------------------------------------
__fastcall TFormDLL::TFormDLL(TComponent* Owner)
        : TForm(Owner),filter_checker(0) // ставим на группу
{
  listIDFacts=new TStringList;
  
  CurIDOpts=ERROR_ID;
  idstud=ERROR_ID;
}
//---------------------------------------------------------------------------
void __fastcall TFormDLL::FormShow(TObject *Sender)
{
  TDateTime dt;
  DateTimePickerOplata->Date=dt.CurrentDate();
}
//---------------------------------------------------------------------------
void __fastcall TFormDLL::BtnOplataFactClick(TObject *Sender)
{
  AnsiString MoneyPayStr=ComboBoxMoney->Text;
  AnsiString NormalDatePayStr=DateTimePickerOplata->DateTime.DateString();
  AnsiString DatePayStr=ReturnParsedDateInSQL(NormalDatePayStr);
  AnsiString IDStudStr=AnsiString(GetIDCurStud());
  AnsiString IDOptsStr=AnsiString(CurIDOpts);
  try
  {
    int money=MoneyPayStr.ToInt();
    if (money<=0) throw 1;
  }
  catch(...)
  {
    AnsiString msg="Сумма оплаты должна задаваться как целое положительное число!\nИсправьте пожалуйста!";
    MessageBox(Handle,msg.c_str(),"Ошибка:",MB_OK|MB_ICONERROR);
    //ComboBoxMoney->SetFocus();
    ComboBoxMoney->SelectAll();
    return;
  }

  AnsiString msg="Задолженность по оплате составляет "+EditDolg->Text+" руб.\nВы хотите произвести оплату в размере "+MoneyPayStr+" руб.\nЧтобы избежать дальнейшее непонимание, для погашения задолженности введите "+EditDolg->Text+" руб. в качестве суммы текущей и последней оплаты в этом периоде!";
  if (EditDolg->Text.ToInt()<MoneyPayStr.ToInt())
  {
    MessageBox(Handle,msg.c_str(),"Сообщение:",MB_OK|MB_ICONINFORMATION);
    ComboBoxMoney->SelectAll();
    return;
  }

  msg="Подтвердите оплату!\n\nПроизвести оплату c данными(проверьте данные во избежании последующих проблем с определением дат оплат):\n";
  msg+="\tСтудент(ка): "+GetFIOForID(idstud)+"\n\tГруппа: "+LabGroupVal->Caption+"\n\tПериод оплаты: "+LabDatesVal->Caption+"\n\tДата оплаты: "+NormalDatePayStr+"\n\tСумма данной оплаты: "+ComboBoxMoney->Text+" руб.?\n\nБудьте аккуратны!";
  if (MessageBox(Handle,msg.c_str(),"Сообщение:",MB_YESNO|MB_ICONQUESTION)==ID_YES)
  {
    // регистрация оплаты(запись в БД)
    AnsiString myquery="INSERT INTO "+opts.DBPayFacts+" (idstud,datepay,moneypay,idopts) VALUES("+ToStr(IDStudStr)+","+ToStr(DatePayStr)+","+ToStr(MoneyPayStr)+","+ToStr(IDOptsStr)+")";
    mysql_query(mysql,myquery.c_str());
    MessageBox(Handle,"Оплата произведена!","Сообщение:",MB_OK|MB_ICONINFORMATION);

    InitPrevOplata();
  }
  else
  {
    MessageBox(Handle,"Оплата не была произведена!","Сообщение:",MB_OK|MB_ICONINFORMATION);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDLL::LoadData(char* IniFileName,int _idstud)
{
  opts.SetFileName(AnsiString(IniFileName));
  BtnOptsOplata->Enabled=opts.ACCESSEditPayOpts;
  idstud=_idstud;
  CurIDOpts=ERROR_ID;
  if (CurIDOpts==ERROR_ID)
  {
    LabOpts->Caption="Категория оплаты не выбрана";

    LabGroup->Hide();
    LabDates->Hide();
    LabMoney->Hide();
    LabGroupVal->Hide();
    LabDatesVal->Hide();
    LabMoneyVal->Hide();
    GroupBoxOplata->Enabled=false;
    ComboBoxMoney->Clear();
    CommonMoneyForPeriod=-1;
    InitPrevOplata();
  }

  // Loading
  AnsiString Msg="Неудачное подключение к "+opts.DBHost+" пользователя "+opts.DBLogin+"!";
  if (!WCConnect())
  {
    MessageBox(Handle,Msg.c_str(),"Error",MB_OK);
    WCDisconnect();
  }
  else
  {
    AnsiString query="CREATE TABLE IF NOT EXISTS "+opts.DBPayOpts+" (id INT PRIMARY KEY AUTO_INCREMENT NOT NULL, idgroup INT NOT NULL, datestart DATE NOT NULL, dateend DATE NOT NULL, commoncountmoney INT NOT NULL, deleted BIT NOT NULL DEFAULT 0)";
    mysql_query(mysql,query.c_str());
    query="CREATE TABLE IF NOT EXISTS "+opts.DBPayFacts+" (id INT PRIMARY KEY AUTO_INCREMENT NOT NULL, idstud INT NOT NULL, datepay DATE NOT NULL, moneypay INT NOT NULL, idopts INT NOT NULL, deleted BIT NOT NULL DEFAULT 0)";
    mysql_query(mysql,query.c_str());
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDLL::FormClose(TObject *Sender, TCloseAction &Action)
{
  WCDisconnect();
}
//---------------------------------------------------------------------------
void __fastcall TFormDLL::BtnOptsOplataClick(TObject *Sender)
{
  FormOptsOplata=new TFormOptsOplata(this);
  FormOptsOplata->SetMyFormDLL(this);
  FormOptsOplata->ShowModal();
  CurIDOpts=FormOptsOplata->GetResultIDOpts();

  AnsiString GroupStr,DatesStr,MoneyStr;
  FormOptsOplata->GetSelectedFieldsOfOpts(GroupStr,DatesStr,MoneyStr);
  LabGroupVal->Caption=GroupStr;
  LabDatesVal->Caption=DatesStr;
  LabMoneyVal->Caption=MoneyStr+" руб.";

  if (CurIDOpts==ERROR_ID)
  {
    LabOpts->Caption="Категория оплаты не выбрана";
    LabGroup->Hide();
    LabDates->Hide();
    LabMoney->Hide();
    LabGroupVal->Hide();
    LabDatesVal->Hide();
    LabMoneyVal->Hide();
    GroupBoxOplata->Enabled=false;
    ComboBoxMoney->Clear();
    CommonMoneyForPeriod=-1;
  }
  else
  {
    LabOpts->Caption="Выбранна категория:";
    LabGroup->Show();
    LabDates->Show();
    LabMoney->Show();
    LabGroupVal->Show();
    LabDatesVal->Show();
    LabMoneyVal->Show();
    GroupBoxOplata->Enabled=true;

    int MoneyInt=MoneyStr.ToInt();
    int MoneyInt4=MoneyInt/4;
    ComboBoxMoney->Clear();
    ComboBoxMoney->Items->Add(AnsiString(MoneyInt4));
    ComboBoxMoney->Items->Add(AnsiString(2*MoneyInt4));
    ComboBoxMoney->Items->Add(AnsiString(3*MoneyInt4));
    ComboBoxMoney->Items->Add(AnsiString(MoneyInt));
    ComboBoxMoney->ItemIndex=0;

    CommonMoneyForPeriod=MoneyStr.ToInt();
  }
  InitPrevOplata();

  delete FormOptsOplata;
}
//---------------------------------------------------------------------------
int __fastcall TFormDLL::GetIDCurStud(void)
{
  return idstud;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormDLL::GetFIOForID(int _idstud)
{
  AnsiString FIOStr="??? ??? ???";
  AnsiString Strtmp="";

  MYSQL_RES *result;
  MYSQL_ROW row;

  AnsiString query="SELECT CONCAT(secondname,\' \',firstname,\' \',thirdname),dogshifrid,dogfastid,dogyearid,dognum FROM "+opts.DBStudTable+" WHERE deleted=0 AND id="+AnsiString(_idstud);
  mysql_query(mysql,query.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      row = mysql_fetch_row(result);
      FIOStr=AnsiString(row[0])+" ( договор: ";
      FIOStr+=WCGetTitleForKeyNum(DOGSHIFRS,AnsiString(row[1]).ToInt())+"-";
      FIOStr+=WCGetTitleForKeyNum(DOGYEARS,AnsiString(row[3]).ToInt());
      Strtmp=WCGetTitleForKeyNum(DOGFASTS,AnsiString(row[2]).ToInt());
      if (!Strtmp.IsEmpty()) FIOStr+="-"+Strtmp;
      FIOStr+="-";
      FIOStr+=AnsiString(row[4]);
      FIOStr+=" )";

    }
    mysql_free_result(result);
  }
  return FIOStr;
}
//---------------------------------------------------------------------------
void __fastcall TFormDLL::InitPrevOplata(void)
{
  ListBoxPrevOplata->Clear();
  listIDFacts->Clear();

  AnsiString IDOptsStr=AnsiString(CurIDOpts);
  AnsiString IDStudStr=AnsiString(idstud);
  AnsiString StrForListBox;

  int SumMoneyOfOplats=0;  // сумма предыдущих выплат в этой периоде

  if (CurIDOpts==ERROR_ID)
  {
    ListBoxPrevOplata->Items->Add("Категория оплаты не выбрана,");
    listIDFacts->Add("");
    ListBoxPrevOplata->Items->Add("а поэтому нет предыдущих оплат!");
    listIDFacts->Add("");

    EditSumMoney->Text="нет категории";
    EditDolg->Text="нет категории";
    ListBoxPrevOplataClick(ListBoxPrevOplata);
    return;
  }
  else
  {
    ListBoxPrevOplata->Items->Add(" Дата оплаты           Сумма оплаты");
    listIDFacts->Add("");
    ListBoxPrevOplata->Items->Add("");
    listIDFacts->Add("");
  }

  MYSQL_RES *result;
  MYSQL_ROW row;
         // id для удаления фактов
  AnsiString query="SELECT id,datepay,moneypay FROM "+opts.DBPayFacts+" WHERE deleted=0 AND idstud="+IDStudStr+" AND idopts="+IDOptsStr+" ORDER BY datepay";
  mysql_query(mysql,query.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      while (row = mysql_fetch_row(result))
      {
        SumMoneyOfOplats+=AnsiString(row[2]).ToInt();
        StrForListBox=" "+ReturnParsedDateInMask(AnsiString(row[1]))+"                  "+AnsiString(row[2]);

        listIDFacts->Add(AnsiString(row[0]));
        ListBoxPrevOplata->Items->Add(StrForListBox);
      }
    }
    mysql_free_result(result);
  }

  EditSumMoney->Text=AnsiString(SumMoneyOfOplats);
  int Dolg=CommonMoneyForPeriod-SumMoneyOfOplats;
  EditDolg->Text=AnsiString(Dolg);
  ListBoxPrevOplataClick(ListBoxPrevOplata);
}
//---------------------------------------------------------------------------
void __fastcall TFormDLL::FormDestroy(TObject *Sender)
{
  delete listIDFacts;
}
//---------------------------------------------------------------------------
void __fastcall TFormDLL::BtnDelClick(TObject *Sender)
{
  int rowsel=ListBoxPrevOplata->ItemIndex;
  if (rowsel==-1) return; // Запись не выбрана
  if (listIDFacts->Strings[rowsel].IsEmpty()) return; // это не запись факта оплаты

  AnsiString IDFactStr=listIDFacts->Strings[rowsel];

  AnsiString myquery;
  AnsiString msg="Вы действительно хотите удалить эту запись-запись по факту оплаты?\nБудьте аккуратны!";
  if (MessageBox(Handle,msg.c_str(),"Сообщение:",MB_YESNO|MB_ICONQUESTION)==ID_YES)
  {
    myquery="UPDATE "+opts.DBPayFacts+" SET deleted=1 WHERE id="+IDFactStr;
    mysql_query(mysql,myquery.c_str());

    InitPrevOplata();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDLL::ListBoxPrevOplataClick(TObject *Sender)
{
  int rowsel=ListBoxPrevOplata->ItemIndex;
  if (rowsel==-1) BtnDel->Enabled=false;
  else
    if (listIDFacts->Strings[rowsel].IsEmpty()) BtnDel->Enabled=false;
    else BtnDel->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormDLL::BtnAllFactsClick(TObject *Sender)
{
  ListBoxPrevOplata->Clear();
  listIDFacts->Clear();

  AnsiString IDStudStr=AnsiString(idstud);
  AnsiString StrForListBox;

  EditSumMoney->Text="нет категории";
  EditDolg->Text="нет категории";
  ListBoxPrevOplataClick(ListBoxPrevOplata);

  ListBoxPrevOplata->Items->Add(" Дата оплаты           Сумма оплаты           Группа");
  listIDFacts->Add("");
  ListBoxPrevOplata->Items->Add("");
  listIDFacts->Add("");

  MYSQL_RES *result;
  MYSQL_ROW row;
         // id для удаления фактов
  AnsiString query="SELECT f.id,f.datepay,f.moneypay,op.idgroup FROM "+opts.DBPayFacts+" as f, "+opts.DBPayOpts+" as op WHERE f.deleted=0 AND op.deleted=0 AND f.idstud="+IDStudStr+" AND f.idopts=op.id ORDER BY f.datepay";
  mysql_query(mysql,query.c_str());
  if (mysql_field_count(mysql))
  {
    result=mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
      while (row = mysql_fetch_row(result))
      {
        StrForListBox=" "+ReturnParsedDateInMask(AnsiString(row[1]))+"                  "+AnsiString(row[2])+"                           "+WCGetTitleForKeyNum(GROUPS,AnsiString(row[3]).ToInt());

        listIDFacts->Add("");
        ListBoxPrevOplata->Items->Add(StrForListBox);
      }
    }
    mysql_free_result(result);
  }

  ListBoxPrevOplataClick(ListBoxPrevOplata);
}
//---------------------------------------------------------------------------

