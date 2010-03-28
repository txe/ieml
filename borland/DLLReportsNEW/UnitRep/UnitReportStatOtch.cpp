//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitReportStatOtch.h"
#include "UnitInfoOcenk.h" 
#include "UnitFuncs.h"
#include "MacroWord.h"

#include "mysql.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ZAbstractDataset"
#pragma link "ZAbstractRODataset"
#pragma link "ZConnection"
#pragma link "ZDataset"
#pragma resource "*.dfm"

TFormReportStatOtch *FormReportStatOtch;

extern EDRegOpts opts;

extern AnsiString arrOcenk[];
extern AnsiString arrShortOcenk[];
extern const CountOcenk=6;

extern EDRegOpts opts;
extern MYSQL *mysql;

//---------------------------------------------------------------------------
__fastcall TFormReportStatOtch::TFormReportStatOtch(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStatOtch::SetIDGroup(int _IDGroup)
{
  idgroup=_IDGroup;
}
//---------------------------------------------------------------------------
int __fastcall TFormReportStatOtch::GetIDGroup(void)
{
  return idgroup;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStatOtch::ToolBtnCloseClick(
      TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStatOtch::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStatOtch::ToolBtnPreviewClick(
      TObject *Sender)
{
  try
  {
    pFormSemestr->ShowModal();
    int semestr_gr=pFormSemestr->GetSemestrGraphic();
    if (semestr_gr==-1)
        return; // �� ������� ��������� ������

    CreateData();
    //CreateWordDocument();
    DestroyData();

  }
  catch(...)
  {
    Application->MessageBox("������ � MS Word �� ��������. �������� ������ ����������.","������.",MB_OK|MB_ICONEXCLAMATION);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStatOtch::FormCreate(TObject *Sender)
{
  pFormSemestr=new TFormSelectSemestr(Application);

  if ( !WCConnect())
  {
    AnsiString msg="��������� ��������� � ������� "+opts.DBHost+" ��� � ���� ������ "+opts.DBName+".\n���������� � ���������� ��������������.";
    MessageBox(Handle,msg.c_str(),"������",MB_OK|MB_ICONERROR);
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportStatOtch::FormDestroy(TObject *Sender)
{
  pFormSemestr->Free();
  WCDisconnect();
}
int get_stat(int cur_stat[4])
{
           if (cur_stat[3] > 0)
                return 6;
            else
            // ���� ����� ������� �� �������� ����������� � ������ ���� ����������� ������
            if (cur_stat[2] == 1)
                return 2;
            else
               if (cur_stat[2] == 2)
                   return 3;
               else
                   if (cur_stat[2] > 2 && (cur_stat[1] > 0 || cur_stat[0] > 0))
                     return  4;
                   else
                        if (cur_stat[2] > 2 && cur_stat[1] == 0 && cur_stat[0] == 0 )
                            return  5;
                        else
                        // ������ 0 �����
            if (cur_stat[1] > 0)
                return 1;
            else
                if (cur_stat[0] > 0 && cur_stat[1] == 0 && cur_stat[2] == 0)
                    return 0;

}

//---------------------------------------------------------------------------
void __fastcall TFormReportStatOtch::CreateData()
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    AnsiString query;
    query = " SET @grpid = " + AnsiString(idgroup)+ " ";
    mysql_query(mysql, query.c_str());
    query = " SET @graphsem = " + AnsiString(pFormSemestr->GetSemestrGraphic()) + " ";
    mysql_query(mysql, query.c_str());

      mysql_query(mysql, " CREATE TEMPORARY TABLE temp_stud TYPE = HEAP "
        " SELECT st.id, st.firstname, st.secondname, st.thirdname "
        " FROM students as st "
        " WHERE  st.grpid = @grpid AND st.deleted = 0 ");

    mysql_query(mysql, " CREATE TEMPORARY TABLE temp_progress TYPE = HEAP "
        " SELECT pr.*, st.secondname "
        " FROM progress as pr, temp_stud  as st, disciplines as dis "
        " WHERE  pr.idstud = st.id AND pr.deleted = 0 AND pr.numgraphsemestr = @graphsem "
        " AND dis.id = pr.iddiscip AND dis.deleted = 0 ");
// ��������� ����������

    mysql_query(mysql, " SELECT LEFT(shorttitle, 50), temp_progress.iddiscip"
        " FROM temp_progress, disciplines "
        " WHERE disciplines.id = temp_progress.iddiscip AND disciplines.deleted = 0 "
        " GROUP BY temp_progress.iddiscip "
        " ORDER BY temp_progress.iddiscip");

    int dis_count = 0;
    result = mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
        while ((row = mysql_fetch_row(result)))
        {
            ++dis_count;
        }
    mysql_free_result(result);

     mysql_query(mysql, " SELECT CONCAT(secondname,\' \',firstname,\' \',thirdname) as fullname, id "
        " FROM temp_stud "
        " ORDER BY secondname, id ");

    int stud_count = 0;
    result = mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
        while (row = mysql_fetch_row(result))
            ++stud_count;
    mysql_free_result(result);



  mysql_query(mysql, " SELECT estimation, ball, idstud, iddiscip, secondname "
        " FROM temp_progress "
        " "
        " ORDER BY secondname, idstud, iddiscip ");


  // ��� 5, 5 � 4, ���� 3, ��� 3, > 2 �����, ��� 3, ����
    int stat[] = {0,0,0,0,0,0,0};
    // 5, 4, 3, ����
    int cur_stat[] ={0 ,0 , 0, 0};
    AnsiString cur_id;

    int i_discip = 0;
    int i_stud   = 0;
    result = mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
        while (row = mysql_fetch_row(result))
        {
            AnsiString name = row[4];
            AnsiString id = row[2];
            //AnsiString name_ = row[3];
            int est = AnsiString(row[0]).ToInt();
            AnsiString ball = row[1];
            if (cur_id == "")
            {
                cur_id = id;
                i_stud = 1;
            }
            if (cur_id == id)
            {
                i_discip++;
                if (est <= 3)
                    cur_stat[est]++;
                continue;
            }

            cur_stat[3] = dis_count -i_discip;

            int st = get_stat(cur_stat);
            stat[st]++;

            cur_stat[0] = 0;
            cur_stat[1] = 0;
            cur_stat[2] = 0;
            cur_stat[3] = 0;

            cur_id = id;
            i_stud++;
            i_discip = 1;
            if (est <= 3)
               cur_stat[est]++;

        }
    }

    cur_stat[3] = dis_count -i_discip;

    int st = get_stat(cur_stat);
    stat[st]++;


    stat[6] += (stud_count - i_stud);

    mysql_free_result(result);

    // ������ �������
    mysql_query(mysql, " drop temporary table if exists temp_stud ");
    mysql_query(mysql, " drop temporary table if exists temp_progress ");
    mysql_query(mysql, " drop temporary table if exists temp_dis ");


    AnsiString msg = AnsiString() + "�� ������ ��������� ���������� (" + AnsiString(pFormSemestr->GetSemestrGraphic()) + " ���), ��������� �:" +
        "\n 5 :" + AnsiString(stat[0]) +
        "\n 5 � 4 :" + AnsiString(stat[1]) +
        "\n ���� 3 :" + AnsiString(stat[2]) +
        "\n ��� 3 :" + AnsiString(stat[3]) +
        "\n ����� 2� ����� :" + AnsiString(stat[4]) +
        "\n ��� ������ :" + AnsiString(stat[5]) +
        "\n ���� :" + AnsiString(stat[6]);

    Application->MessageBox(msg.c_str(), "���������� �� ��������", MB_OK | MB_ICONEXCLAMATION);


 }
//---------------------------------------------------------------------------
void __fastcall TFormReportStatOtch::DestroyData()
{

}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormReportStatOtch::GetSpec()
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    AnsiString query;
    AnsiString spec;

    query = "SELECT specid FROM students WHERE deleted = 0 AND grpid = " + IntToStr(GetIDGroup())
        + " LIMIT 1";
    mysql_query(mysql, query.c_str());
    result = mysql_store_result(mysql);
    if (result && mysql_num_rows(result))
    {
        row = mysql_fetch_row(result);
        spec = WCGetTitleForKeyNum(SPECS, AnsiString(row[0]).ToInt());
    }
    mysql_free_result(result);
    return spec;
}
//---------------------------------------------------------------------------

