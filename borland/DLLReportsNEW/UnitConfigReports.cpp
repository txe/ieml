//---------------------------------------------------------------------------
#pragma hdrstop

#include "UnitConfigReports.h"
//---------------------------------------------------------------------------
const int CountOfReports = 31;

AnsiString arrTitlesOfReports[CountOfReports] = {
   /*  0 */     "��������� �� ������ (Quick)",
   /*  1 */     "������� new (Quick)",
   /*  2 */     "������� old (Quick)",
   /*  3 */     "��������������� (��������) ���� (Quick)",
   /*  4 */     "�������� ��������� ���. �������� - ���. �������� (Quick)",
   /*  5 */     "������� (Quick)",
   /*  6 */     "���������� �������� (Quick)",
   /*  7 */     "����� �� ������ �� ������ �� ������ (MS Word)",
   /*  8 */     "������ ��������� � ������� ��������� (MS Word)",
   /*  9 */     "��� ������-����� ������ ��� ��������� (MS Word)",
   /* 10 */     "�������� � ������� � ������������� ���������� (MS Word)",
   /* 11 */     "�������� � ���������� ������������ �� 1-� ���� (MS Word)",
   /* 12 */     "������� ��������� (MS Word)",
   /* 13 */     "��������� �� ������ (MS Word)",
   /* 14 */     "����� ��� ����������� (������ ���������������� �� �����)",
   /* 15 */     "����� ��� ����������� (������ � ����� ���������������� �� ��. ���)",
   /* 16 */     "����� ��� ����������� (������ ���������������� �� ���� ����)",
   /* 17 */     "����� ��� ����������� (������ � ����� ���������������� �� �������)",
   /* 18 */     "����� ��� ����������� (MS Excel)",
   /* 19 */     "����� ��� ����������� (MS Word)",
   /* 20 */     "���������� �������� ����� ��������� ������ (MS Word)",
   /* 21 */     "������ ������ ���������� �� ������� ������ (MS Word)",
   /* 22 */     "������������� ������� (MS Word)",
   /* 23 */     "���������� � ������� (MS Word)",
   /* 24 */     "�������������� �������� (MS Word)",
   /* 25 */     "������� �������� �������� (MS Excel)",
   /* 26 */     "���������� � ������� � ��������� ������ (MS Word)",
   /* 27 */     "����� �3 - (MS Excel)",
   /* 28 */     "���������� �� ������� ������",
   /* 29 */     "���������� (old)",
   /* 30 */     "����� �� ��� (MS Word)"
};

AnsiString arrHintsOfReports[CountOfReports] = {
        "��������������� ��������� �� ������",
        "������� ��� �����������",
        "������� ��� ��������������",
        "��������������� (��������) ����",
        "�������� ��������� ��������������� �������� (���. ��������)",
        "�������, �������������� ��� ������� ������ � ����",
        "���������� �������� ��������",
        "����� �� ������ ��������� �� ��������� ������ �� ������ ��������",
        "������ ��������� � ��������������� ������� ���������",
        "��� ������-����� ������ ��� ���������",
        "�������� � ������� ������������ � ������������� ����������",
        "�������� � ���������� ������������ �� 1-� ����",
        "������� ��������� �� ������������ � ������",
        "��������� �� ������",
        "����� ��� ����������� (������ ���������������� �� �����)",
        "����� ��� ����������� (������ � ����� ���������������� �� ��. ���)",
        "����� ��� ����������� (������ ���������������� �� ���� ����)",
        "����� ��� ����������� (������ � ����� ���������������� �� �������)",
        "����� ��� ����������� (MS Excel)",
        "����� ��� ����������� (MS Word)",
        "���������� �������� ����� ��������� ������",
        "������ ������ ���������� �� ������� ������",
        "������������� �������",
        "���������� � �������",
        "�������������� ��������",
        "������� �������� �������� (MS Excel)",
        "���������� � ������� � ��������� ������(MS Word)",
        "����� �3 - ��� ���������� ������ � �������� �������� ����������������� �����������(MS Excel)",
        "���������� �� ������� ������",
        "���������� (old)",
        "����� �� ��� (MS Word)"
};
//---------------------------------------------------------------------------

#pragma package(smart_init)
