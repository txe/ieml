//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitAccount.h"
#define MAX_FALSE_ACCOUNTS 5
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAccount *FormAccount;
//---------------------------------------------------------------------------
__fastcall TFormAccount::TFormAccount(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormAccount::FormShow(TObject *Sender)
{
  NumFalseAccounts=MAX_FALSE_ACCOUNTS;
}
//---------------------------------------------------------------------------
void __fastcall TFormAccount::BitBtn1Click(TObject *Sender)
{
  if (IsTrueAccount())
  {
    ModalResult=mrOk;
  }
  else
  {
    NumFalseAccounts--;
    AnsiString Msg;
    if (0==NumFalseAccounts)
    {
      Msg="Вы пять раз подряд вводили неправилные данные.\nПрограмма завершает свою работу.";
      MessageBox(Handle,Msg.c_str(),"incorrect account",MB_OK|MB_ICONWARNING);
      ModalResult=mrCancel;
    }
    else
    {
      Msg="Логин или пароль заданы неверно.\nВведите правильное имя пользователя и пароль\nдля доступа к базе данных.";
      MessageBox(Handle,Msg.c_str(),"incorrect account",MB_OK|MB_ICONWARNING);
    }
  }
}
//---------------------------------------------------------------------------
bool __fastcall TFormAccount::IsTrueAccount(void)
{
  if (NumFalseAccounts==1) return true;
  return false;
}
//---------------------------------------------------------------------------
