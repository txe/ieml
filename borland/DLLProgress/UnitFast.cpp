//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitFast.h"
#include "UnitProgressDefSemestr.h"
#include "UnitInfoOcenk.h"
#include <sstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

extern EDRegOpts opts;
extern MYSQL *mysql;

//---------------------------------------------------------------------------
__fastcall TFormFast::TFormFast(TComponent* Owner)
    : TForm(Owner)
{
    estim = 0;

    StrGridListOcenk->Cells[0][0] = "№п/п";
    StrGridListOcenk->Cells[1][0] = "Фамилия Имя Отчество";
    StrGridListOcenk->Cells[2][0] = "Оценка";
}
//---------------------------------------------------------------------------
void __fastcall TFormFast::StrGridListOcenkSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
    CanSelect = (ACol == 2)?true:false;
}
//---------------------------------------------------------------------------
void TFormFast::CreateTable()
{
//
    MYSQL_RES*  result;
    MYSQL_ROW   row;
    AnsiString  query;

    // получаем список студентов
    query = "SELECT id, CONCAT(secondname,' ',firstname,' ',thirdname) AS fio \
        FROM students \
        WHERE deleted = 0 AND grpid = " + idgroup + " \
        ORDER BY fio, id ";

    mysql_query(mysql,query.c_str());
    if (mysql_field_count(mysql))
    {
        result = mysql_store_result(mysql);
        if (result && mysql_num_rows(result))
        {
            while (row = mysql_fetch_row(result))
            {
                pr_data* cur_data = new pr_data;
                cur_data->idstud = row[0];
                cur_data->fio = row[1];
                cur_data->estim = "";
                cur_data->ball = "";
                data.push_back(cur_data);
            }
        }
        mysql_free_result(result);
    }
    // получаем оценки по предмету за этот семестр, если они есть
    query = "SELECT pr.idstud, pr.ball                       \
                FROM students AS st, progress AS pr          \
                WHERE                                        \
                st.deleted = 0 AND st.grpid = " + idgroup + " AND         \
                pr.deleted = 0 AND pr.idstud = st.id AND pr.`numplansemestr` = " + plan_sem + " AND  \
                pr.`numgraphsemestr` = " + graf_sem + " AND pr.`iddiscip` = " + iddiscip;

    mysql_query(mysql,query.c_str());
    if (mysql_field_count(mysql))
    {
        result = mysql_store_result(mysql);
        if (result && mysql_num_rows(result))
        {
            // если есть оценки то перебором их запишем
            // нужным студентам
            TReplaceFlags flags;
            flags << rfReplaceAll << rfIgnoreCase;
            while (row = mysql_fetch_row(result))
                for(int cur_index = 0; cur_index < data.size(); ++cur_index)
                    if (data[cur_index]->idstud == row[0])
                    {
                        data[cur_index]->ball =  StringReplace(row[1], ".", ",", flags);
                        break;
                    }
        }
        mysql_free_result(result);
    }

// заполним строчку данными
    AnsiString caption;
    caption = "семестр по плану: \t" + plan_sem + "\n" \
            "семестр по графику: \t" + graf_sem + "\n" \
            "предмет \t\t" + name_pred + "\n" \
            "тип оценки: \t\t" + AnsiString(estim == 1?"экзамен":"зачет") + "\n";
    LabelStuff->Caption = caption;

    UpdateTable();
}
//---------------------------------------------------------------------------
void  TFormFast::UpdateTable()
{
    StrGridListOcenk->RowCount = data.size() + 1;
    for (int i = 0; i < data.size(); i++)
    {
        StrGridListOcenk->Cells[0][i+1] = i + 1;
        StrGridListOcenk->Cells[1][i+1] = data[i]->fio;
        StrGridListOcenk->Cells[2][i+1] = data[i]->ball;
    }

    TGridRect myRect;
	myRect.Left = 2;
	myRect.Top = 1;
	myRect.Right = 2;
	myRect.Bottom = 1;
	StrGridListOcenk->Selection = myRect;
}
//---------------------------------------------------------------------------
void __fastcall TFormFast::FormDestroy(TObject *Sender)
{
    for (int i = 0; i < data.size(); i++)
        delete data[i];

    data.clear();
}
//---------------------------------------------------------------------------
void __fastcall TFormFast::StrGridListOcenkKeyPress(TObject *Sender,
      char &Key)
{
    if (Key == VK_RETURN)
    {
        if (StrGridListOcenk->Row < StrGridListOcenk->RowCount - 1)
            StrGridListOcenk->Row ++;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormFast::ButtonSaveClick(TObject *Sender)
{
// проверим введенные значения
    double val;
    for (int i = 1; i < StrGridListOcenk->RowCount; i++)
        if ((StrGridListOcenk->Cells[2][i] == "") || (TryStrToFloat(StrGridListOcenk->Cells[2][i], val) && val >= 0 && val <= 5))
        {
            data[i-1]->estim = GetEstByBall(val);
            data[i-1]->ball = StrGridListOcenk->Cells[2][i];
        }
        else
        {
            TGridRect myRect;
	        myRect.Left = 2;
	        myRect.Top = i;
	        myRect.Right = 2;
        	myRect.Bottom = i;
            StrGridListOcenk->Selection = myRect;
            StrGridListOcenk->SetFocus();
            MessageBeep(MB_OK);
            return;
        }
//
    MYSQL_RES*  result;
    MYSQL_ROW   row;
    AnsiString  query;
// надо удалить предыдущие оценки которые были
    query = "DELETE pr.*                       \
                FROM students AS st, progress AS pr          \
                WHERE                                        \
                st.deleted = 0 AND st.grpid = " + idgroup + " AND         \
                pr.deleted = 0 AND pr.idstud = st.id AND pr.`numplansemestr` = " + plan_sem + " AND  \
                pr.`numgraphsemestr` = " + graf_sem + " AND pr.`iddiscip` = " + iddiscip;
   mysql_query(mysql,query.c_str());

// запишем новые оценки
// есть ограничение на количество вставляемых строк, по видимому

    query = "INSERT INTO progress (idstud,iddiscip,numplansemestr,numgraphsemestr,estimation,ball) VALUES";
    TReplaceFlags flags;
    flags << rfReplaceAll << rfIgnoreCase;
    for (int index = 0; index < data.size(); ++index)
    {
        if (data[index]->ball == "")
            continue;
        query += "(" +
            data[index]->idstud + "," +
            iddiscip + "," +
            plan_sem + "," +
            graf_sem + "," +
            data[index]->estim + "," +
            StringReplace(data[index]->ball, ",", ".", flags) +"),";
    }
        query.Delete(query.Length(),1);
        mysql_query(mysql,query.c_str());

    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormFast::StrGridListOcenkDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
    double val;
    if  (ACol == 2)
    {
        if (StrGridListOcenk->Cells[2][ARow] == "")
            StrGridListOcenk->Canvas->Brush->Color = RGB(213,208,0);
        else
            if (TryStrToFloat(StrGridListOcenk->Cells[2][ARow], val) && val >= 0 && val <= 5)
                StrGridListOcenk->Canvas->Brush->Color = RGB(42,187,104);
            else
                StrGridListOcenk->Canvas->Brush->Color = RGB(244,69,56);
        if (ACol > 0 && ARow>0)
        {
            StrGridListOcenk->Canvas->FillRect(Rect);
            StrGridListOcenk->Canvas->TextOut(Rect.Left,Rect.Top,StrGridListOcenk->Cells[ACol][ARow]);
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormFast::FormResize(TObject *Sender)
{
    StrGridListOcenk->ColWidths[0] = 25;
    StrGridListOcenk->ColWidths[1] = StrGridListOcenk->ClientWidth - 99;
    StrGridListOcenk->ColWidths[2] = 70;
}
//---------------------------------------------------------------------------
AnsiString TFormFast::GetEstByBall(const double &val)
{
   if (estim == 1) // экзамен
    {
        // отлично
        if ( val >= 4.5)
            return "0";
        // хорошо
        if ( val >= 3.5 && val < 4.5 )
            return "1";
        // уд
        if ( val >= 2.5 && val < 3.5 )
            return "2";
        // неуд
        if ( val <= 2.5)
            return "3";
    }
    if (estim == 2) // зачет
    {
        // зачтено
        if ( val >= 2.5)
            return "4";

        // незачтено
        return "5";
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormFast::ButtonCopyClick(TObject *Sender)
{
    AnsiString list = "";

   for (int i = 1; i < StrGridListOcenk->RowCount; ++i)
        list += StrGridListOcenk->Cells[2][i] + "\r\n";

    if (!OpenClipboard(Handle))
        return ;
    EmptyClipboard();
    char* buf = (char*) LocalAlloc(LMEM_FIXED, list.Length()*sizeof(char));
    if (buf == NULL)
    {
        CloseClipboard();
        return ;
    }
    memcpy(buf, list.c_str(), list.Length());
    SetClipboardData(CF_TEXT, buf);
    CloseClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TFormFast::ButtonInsertClick(TObject *Sender)
{
    if (!IsClipboardFormatAvailable(CF_TEXT))
        return;
    if (!OpenClipboard(Handle))
        return ;

    HGLOBAL hglb = GetClipboardData(CF_TEXT);
    if (hglb != NULL)
    {
        LPTSTR lptstr = (LPTSTR)GlobalLock(hglb);
        if (lptstr != NULL)
        {
            std::istringstream  sb(lptstr);
            std::string    s;
            for (int i = 1; i < StrGridListOcenk->RowCount; ++i)
            {
                if (std::getline(sb, s, '\r'))
                {
                    StrGridListOcenk->Cells[2][i] = AnsiString(s.c_str());
                    std::getline(sb, s, '\n'); // избавляемся от перевода строки
                }
                else
                    StrGridListOcenk->Cells[2][i] = "";
            }
            GlobalUnlock(hglb);
        }
    }
    CloseClipboard();

}
//---------------------------------------------------------------------------

