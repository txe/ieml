//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#include "UnitReportUchebKartStud.h"

#include "UnitFuncs.h"
#include "mysql.h"
#include "MacroExcel.h"
#include <vector>
#include <string>
#include <sstream>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

extern EDRegOpts opts;
extern MYSQL *mysql;
//---------------------------------------------------------------------------
enum ETypeDiscip {
        tdCommon = 1,
        tdCourseWork = 2,
        tdCourseProject = 3,
        tdPraktica = 4,
        tdItogAttest = 5,
        tdVipQualifWork = 6,
        tdAdditional = 7
};
AnsiString tip_ocenok[] = {"отл", "хор", "удовл", "неуд", "зач", "незач"};
AnsiString MounthNames[12]={ "января",
                             "февраля",
                             "марта",
                             "апреля",
                             "мая",
                             "июня",
                             "июля",
                             "августа",
                             "сентября",
                             "октября",
                             "ноября",
                             "декабря"};
const int max_len = 19;   // кол-во букв в одной ячейке
//const int max_count_line = 17; // кол-во строк в одном семестре
//const int max_len_title = 35;
//---------------------------------------------------------------------------
AnsiString TFormReportUchebKartStud::GetDateAsString(const AnsiString& str, bool isYear)
{
    AnsiString res = "";
    res += str.SubString(9,2);
    if (res.Length() == 1)
        res = "0" + res;
    res += " ";
    res += MounthNames[(str.SubString(6, 2).ToInt() - 1) % 12] + " ";
    res += str.SubString(1, 4);
    if (isYear)
        res += " г.";
    return res;
}
//---------------------------------------------------------------------------
int TFormReportUchebKartStud::GetCountLine(const AnsiString& text, const int& max_len)
{
    int ret = 1;
    int cur_len = 0;
    int count = 0;
    int flag = false; // показывает что уже был пропуск
                    // это позволяет учитывать две точки за одну букву
    for (int i = 1; i <= text.Length(); i++)
    {
        char ch = text[i];
        if (ch == ' ')
            ret = i;
        if (ch == '.' || ch == ',') // тчк и зпт занимают мало места можно не учитывать
        {
            flag = !flag;
            if (flag)
                continue;
        }
        cur_len++;
        if (cur_len == max_len)
        {
            i = ret;
            count++;
            cur_len = 0;
        }
        if (count > 6) // из за возможного зацикливания
            if (text.Length() > 2*max_len)
                return 3;     // млин слишком длиинно слово попалось придется возвращать 3 строки
            else
                return 1;    
    }
    return count;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportUchebKartStud::UpdateView()
{
    for (int i = 0; i < mas_ochenka.size(); i++)
    {
        Progress*   pr =  mas_ochenka[i];
        pr->title = pr->dis_shorttitle;//(pr->dis_fulltitle.Length() < max_len_title)?pr->dis_fulltitle:pr->dis_shorttitle;
        pr->count_line = GetCountLine(pr->title, max_len);

    }
  /*  for (int sem = 1; sem <= 11; sem++) // проходим по отдельным семестрам
    {
        while(1)
        {
            // инициализация
            int pos_max_title = 0; // пложение самого длинного названия
            int len_max_title = 0; // его дляна

            int count_line = 0; // сколько строк займет один семестр
            for (int i = 0; i < mas_ochenka.size(); i++)
            {
                Progress*   pr =  mas_ochenka[i];
                // пропуск всего ненужного
                if (pr->numplansemestr != sem)
                    continue;
                if (!(pr->idclass == tdCommon || pr->idclass == tdCourseWork || \
                    pr->idclass == tdCourseProject || pr->idclass == tdPraktica))
                        continue;
                // ищем заодно самое длинное title
                if (pr->title.Length() >= len_max_title && pr->title != pr->dis_shorttitle)
                {
                    len_max_title = pr->title.Length();
                    pos_max_title = i;
                }
                count_line += pr->count_line + 1; // единица из за того что начало отчета 0
            }
            if (count_line < max_count_line)
                break;
            // иначе заменяем самое длинок титле на его короткий аналог
            // !!!!!!
            // из за этого может быть зацикливание
            // наплевать
            mas_ochenka[pos_max_title]->title = mas_ochenka[pos_max_title]->dis_shorttitle;
            mas_ochenka[pos_max_title]->count_line = GetCountLine(mas_ochenka[pos_max_title]->title, max_len);
         }
    }
    */
}
//---------------------------------------------------------------------------
__fastcall TFormReportUchebKartStud::TFormReportUchebKartStud(TComponent* Owner)
    : TForm(Owner),idstudent(-1),idgroup(-1)
{
}
//---------------------------------------------------------------------------
void    __fastcall  TFormReportUchebKartStud::SetIDStudent(int _IDStudent)
{
    idstudent = _IDStudent;
}
//---------------------------------------------------------------------------
int    __fastcall  TFormReportUchebKartStud::GetIDStudent(void)
{
    return idstudent;
}
//---------------------------------------------------------------------------
void    __fastcall  TFormReportUchebKartStud::SetIDGroup(int _IDGroup)
{
    idgroup = _IDGroup;
}
//---------------------------------------------------------------------------
int    __fastcall  TFormReportUchebKartStud::GetIDGroup(void)
{
    return idgroup;
}
//---------------------------------------------------------------------------
void    __fastcall  TFormReportUchebKartStud::CreateWordDocument(void)
{
    AnsiString NumZ, SecondName, FirstName, ThirdName, Adres, NumPricas, InDate,
        spec, VipQualifWork, OutDate, Qualific, nGak, direct, profil;
    AnsiString perevodInfo[5];

    GetOchenkiStudenta();
    UpdateView();
    GetInfoStudent(NumZ, SecondName, FirstName, ThirdName, Adres, NumPricas, InDate,
        spec, VipQualifWork, OutDate, Qualific, nGak, perevodInfo, direct, profil);

    ExcelMacros macros;
    macros.BeginMacros();

    // заполянем верхнюю часть катрочки
    macros.Select("L2");
    macros.Formula("     №   " + NumZ);
    macros.Select("A6");
    macros.Formula("1.   " + SecondName + " " + FirstName + " " + ThirdName);
    macros.Select("A8");
    macros.FilterText(Adres);
    macros.Formula("2. Адрес:   " + Adres);
    macros.Select("A10");
    macros.Formula("3. Зачислен в ВУЗ приказом №   " + NumPricas);
    macros.Select("A11");
    macros.Formula("от   " + GetDateAsString(InDate));
    macros.Select("A16");
    macros.Formula("по направлению   " + direct);
    macros.Select("A17");
    macros.Formula("с профилем   " + profil);
    macros.Select("H21");
    macros.FilterText(VipQualifWork);
    macros.Formula("  " + VipQualifWork);
    macros.Select("H27");
    macros.Formula("присвоена квалификация   " + Qualific);
    if (OutDate.Length() > 0)
    {
        macros.Select("H26");
        macros.Formula("от " + GetDateAsString(OutDate, true) + ")");
        macros.Select("H23");
        macros.Formula("и защитил " + GetDateAsString(OutDate, true));
    }
    if (nGak.Length() > 0)
    {
        macros.Select("H25");
        macros.Formula(" комиссии  (протокол №    " + nGak);
    }

    // заполняем отметки о переводах
    AnsiString perevodPos[5] = {"A62", "H62", "A96", "H96", "A130"};
    for (int i = 0; i < 5; ++i)
       if (perevodInfo[i] != "")
       {
         macros.Select(perevodPos[i]);
         macros.Formula(perevodInfo[i]);
       }


    // зполняем оценками таблицу

    for (int i = 0; i < mas_ochenka.size(); i++)
    {
        Progress* pr =  mas_ochenka[i];
        if (!(pr->idclass == tdCommon || pr->idclass == tdCourseWork || \
            pr->idclass == tdCourseProject || pr->idclass == tdPraktica))
                continue;

        if (pr->numplansemestr > 12 || pr->numplansemestr < 1)
        {
            AnsiString er = "Предмет \"" + pr->title + "\" записан на " + \
                pr->numplansemestr + " - это неправильно \n" \
                "Отчет не будет выведен";
            Application->MessageBox(er.c_str(), "Ошибка.", MB_OK | MB_ICONEXCLAMATION);
            return;
        }
        if (pr->numplansemestr == 12 )
            pr->numplansemestr = 12;
        AnsiString range = "";
        if (pr->count_line != 0)
        {
            for (int j = 1; j <= 5; j++)
            {
                range = AnsiString(pos_semestr[pr->numplansemestr - 1].col[j]) +
                    IntToStr(pos_semestr[pr->numplansemestr - 1].row);
                range += ":" + AnsiString(pos_semestr[pr->numplansemestr - 1].col[j]) +
                    IntToStr(pos_semestr[pr->numplansemestr - 1].row + pr->count_line);
                macros.Select(range);
                macros.MergeCell();
            }
            pos_semestr[pr->numplansemestr - 1].row += pr->count_line;
         }

         range = AnsiString(pos_semestr[pr->numplansemestr - 1].col[1]) +
             IntToStr(pos_semestr[pr->numplansemestr - 1].row);
         macros.Select(range);
         macros.Formula(pr->title);

         range = AnsiString(pos_semestr[pr->numplansemestr - 1].col[2]) +
             IntToStr(pos_semestr[pr->numplansemestr - 1].row);
         macros.Select(range);
         //macros.Formula((pr->count_hours == "0")?AnsiString(""):pr->count_hours);
         if (pr->idclass == tdCommon)
            if (pr->numplansemestr != 12 && pr->hours[pr->numplansemestr - 1] != "0  ")
                macros.Formula(pr->hours[pr->numplansemestr - 1]);
            else
                macros.Formula((pr->count_hours == "0")?AnsiString(""):pr->count_hours);



         AnsiString pos = "";
         if (pr->estimation < 4)    // экзамен
            pos = AnsiString(pos_semestr[pr->numplansemestr - 1].col[3]);
         else                      // зачет
            pos = AnsiString(pos_semestr[pr->numplansemestr - 1].col[4]);

         range = pos + IntToStr(pos_semestr[pr->numplansemestr - 1].row);
         macros.Select(range);
         macros.Formula(tip_ocenok[pr->estimation]);

         pos_semestr[pr->numplansemestr - 1].row ++;
    }
    // заполнение по междисц испыт
    int pos = 16;  // начиная с 16 строки в екселе
    for (int i = 0; i < mas_ochenka.size(); i++)
    {
        Progress* pr =  mas_ochenka[i];
        if (pr->idclass != tdItogAttest)
            continue;
        macros.Select("J" + IntToStr(pos));
        macros.Formula(pr->dis_shorttitle);
        macros.Select("M" + IntToStr(pos));
        macros.Formula(tip_ocenok[pr->estimation]);
        pos++;
    }
    macros.Select("A4");

    macros.EndMacros();

    // получаем текущее положение
    char path_buf[_MAX_PATH];
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];

    GetModuleFileName(NULL,path_buf,sizeof(path_buf)-1);
    _splitpath(path_buf,drive,dir,0,0);
    _makepath(path_buf,drive,dir,0,0);

    AnsiString path_host_template = AnsiString(drive) + AnsiString(dir) + "template.xlt";
    if (FileExists(path_host_template))
        macros.RunMacros(path_host_template);
    else
       macros.RunMacros("c:\\template.xlt");

}
//---------------------------------------------------------------------------
void    __fastcall  TFormReportUchebKartStud::GetOchenkiStudenta(void)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    AnsiString myquery = "SELECT DISTINCT dis.fulltitle ,dis.shorttitle, " \
        "pr.numplansemestr, dis.num_hours, pr.estimation, dis.idclass, dis.sem_hours FROM " \
        "students AS st,progress as pr, disciplines AS dis WHERE " \
        "st.id = " + IntToStr(GetIDStudent()) + " AND pr.idstud = st.id AND " \
        "dis.id = pr.iddiscip AND dis.deleted = 0 AND pr.deleted = 0 ORDER BY pr.numplansemestr" \
        ", dis.fulltitle";
    mysql_query(mysql,myquery.c_str());
    if (mysql_field_count(mysql))
    {
        result=mysql_store_result(mysql);
        if (result && mysql_num_rows(result)){
            while (row = mysql_fetch_row(result))
            {
                int idclass = AnsiString(row[5]).ToInt();
                if (!(idclass == tdCommon || idclass == tdCourseWork \
                    || idclass == tdCourseProject || idclass == tdItogAttest || idclass == tdPraktica))
                        continue;
                Progress* pr = new Progress();
                pr->dis_fulltitle = AnsiString(row[0]);
                pr->dis_shorttitle = AnsiString(row[1]);
                pr->numplansemestr = AnsiString(row[2]).ToInt();
                pr->count_hours = AnsiString(row[3]);
                pr->estimation = AnsiString(row[4]).ToInt();
                pr->idclass = AnsiString(row[5]).ToInt();
                StringToAudHours(pr->hours, AnsiString(row[6]));
                mas_ochenka.push_back(pr);
            }
        }
        mysql_free_result(result);
    }

    for (int i = 1; i <= 12; i++)
        if (i == 3 || i == 4 || i == 7 || i == 8 || i == 11 || i == 12)
            pos_semestr[i-1].col = "JKLMN";
        else
            pos_semestr[i-1].col = "CDEFG";

    pos_semestr[0].row = pos_semestr[2].row = 33;
    pos_semestr[1].row = pos_semestr[3].row = 47;
    pos_semestr[4].row = pos_semestr[6].row = 65;
    pos_semestr[5].row = pos_semestr[7].row = 79;
    pos_semestr[8].row = pos_semestr[10].row = 98;
    pos_semestr[9].row = pos_semestr[11].row = 114;
}
//---------------------------------------------------------------------------
void __fastcall TFormReportUchebKartStud::GetInfoStudent(AnsiString& NumZ, AnsiString& SN, AnsiString& FN, AnsiString& TN,
        AnsiString& Adres, AnsiString& NumPricas, AnsiString& InDate, AnsiString& spec,
        AnsiString& VipQualifWork, AnsiString& OutDate, AnsiString& Qualific, AnsiString& nGak, AnsiString perevodInfo[5],
        AnsiString& direct, AnsiString& profil)
{
    NumZ = "???";
    SN = "???";
    FN = "???";
    TN = "???";
    Adres = "???";
    NumPricas = "???";
    InDate = "???";
    spec = "???";
    VipQualifWork = "???";
    OutDate = "???";
    Qualific = "???";

    direct = "???";
    profil = "???";

    MYSQL_RES *result;
    MYSQL_ROW row;

    AnsiString myquery = "select s.secondname,s.firstname,s.thirdname, s.addr," \
        "s.enternum,s.enterdate,s.specid, s.vkr_title, s.znum, s.edudatequalif, s.edunumprotgak, s.perevod_na_kurs, " \
        "s.directid from "+opts.DBStudTable+" as s "\
        "where s.deleted=0 and s.id=" + ToStr(AnsiString(idstudent));

    mysql_query(mysql,myquery.c_str());
    if (mysql_field_count(mysql))
    {
        result=mysql_store_result(mysql);
        if (result && mysql_num_rows(result))
        {
             while (row = mysql_fetch_row(result))
            {
                SN = AnsiString(row[0]);
                FN = AnsiString(row[1]);
                TN = AnsiString(row[2]);
                Adres = AnsiString(row[3]);
                NumPricas = AnsiString(row[4]);
                InDate = AnsiString(row[5]);
                spec = WCGetTitleForKeyNum(SPECS, AnsiString(row[6]).ToInt());
                VipQualifWork = AnsiString(row[7]);
                Qualific = WCGetTitleForKeyNum(QUALIFIC, AnsiString(row[6]).ToInt());
                NumZ = AnsiString(row[8]);

                OutDate = AnsiString(row[9]);
                nGak =  AnsiString(row[10]);
                StringToPerevodInfo(perevodInfo, AnsiString(row[11]));

                GetDirectInfo(direct, profil, AnsiString(row[6]).ToInt(), AnsiString(row[12]).ToInt());
            }
        }
        mysql_free_result(result);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportUchebKartStud::ToolBtnCloseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormReportUchebKartStud::ToolBtnPreviewClick(TObject *Sender)
{
    try
    {
        CreateWordDocument();
    }
    catch(...)
    {
        Application->MessageBox("Работа с MS Word не возможна. Просмотр отчета невозможен.","Ошибка.",MB_OK|MB_ICONEXCLAMATION);
        Close();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormReportUchebKartStud::FormCreate(TObject *Sender)
{
    if (!WCConnect())
    {
        AnsiString msg = "Ошибочное обращение к серверу " \
            + opts.DBHost + " или к базе данных " + opts.DBName + ".\nОбратитесь к системному администратору.";
        MessageBox(Handle, msg.c_str(), "Ошибка", MB_OK | MB_ICONERROR);
        Close();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormReportUchebKartStud::FormDestroy(TObject *Sender)
{
    for (int i = 0; i < mas_ochenka.size(); i++)
        delete mas_ochenka[i];

    WCDisconnect();
}
//---------------------------------------------------------------------------
bool __fastcall  TFormReportUchebKartStud::FileExists(const AnsiString& fname)
{
    return ::GetFileAttributes((LPCTSTR)fname.c_str()) != DWORD(-1);
}
//---------------------------------------------------------------------------
void __fastcall TFormReportUchebKartStud::StringToAudHours(AnsiString  h[12], AnsiString &str_hours)
{
    int cur_sem = 0;
    int begin = 1;
    for(int pos = 1; pos <= str_hours.Length(); pos++)
    {
        if (str_hours[pos] == ';')
        {
            h[cur_sem++] = str_hours.SubString(begin, pos - begin);
            begin = pos + 1;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportUchebKartStud::StringToPerevodInfo(AnsiString info[5], AnsiString str)
{
    std::vector<AnsiString> list;
    std::stringstream ss(str.c_str());
	std::string item;
    while (std::getline(ss, item, '&'))
	    list.push_back(item.c_str());

    if (list.size() == 10)
        for (int i = 0; i < 10; i += 2)
        {
            AnsiString number = list[i];
            AnsiString date   = list[i+1];
            if (number != "" && date != "")
            {
              try
              {
                date = GetDateAsString(date, true);
                info[i/2] = "Приказ № " + number + "  от  " + date;
              } catch (...) {}
             }

        }
}
//---------------------------------------------------------------------------
void __fastcall TFormReportUchebKartStud::GetDirectInfo(AnsiString& direct, AnsiString& profil, int specid, int directid)
{
  // смотри ReportStudyingSpravka::GetDirectData
  AnsiString specOrProfileTag;
  AnsiString specOrProfile = WCGetTitleForKeyNum(SPECS, specid, &specOrProfileTag);
  AnsiString specializ = WCGetTitleForKeyNum(SPEZIALIZS, specid);
  AnsiString dbDirect = WCGetTitleForKeyNum(DIRECTS, directid);
  bool isMagister = specOrProfileTag.LowerCase().Trim() == "маг";

  if (dbDirect != "")
    direct = dbDirect;
  else
    direct = specOrProfile;
  if (dbDirect != "" || isMagister)
    profil = specOrProfile;
  else
    profil = specializ;
}


