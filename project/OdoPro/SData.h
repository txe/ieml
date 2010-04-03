#pragma once

#include "htmlayout.h"
#include <map>
#include <map>
#include "string_t.h"

class SData
{
private:
	std::map<string_t, htmlayout::dom::element>	map_elements_;

	htmlayout::dom::element	root_;
	htmlayout::dom::element	sel_egediscip_;
	htmlayout::dom::element	table_ege_discip_;
	htmlayout::dom::element	ege_ball_;
public:
	SData(void);
	~SData(void);
public:
	// �������������� ��� �������� � ��� �����
	void Init(htmlayout::dom::element root);
	// ���������  ��� �������� ��� �������� ��������
	void UpdateView(void);
private:
	// ���� ��� �������� �� ������� � ��������� �� � ������
	static BOOL CALLBACK CallbackAtachElement(HELEMENT he, LPVOID param);
	// �������� ����� ��� ��������� ������
	static BOOL CALLBACK ElementEventProcChangedGroup(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ ������
	static BOOL CALLBACK ElementEventProcBt(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
	// ������������ ����� �������� ���
	static BOOL CALLBACK ElementEventProcEgeDiscip(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
private:
	// ��������� ������� ��������� �� ���
	void UpdateEgeTable();
	// ���������\�������� ������ �������� �� ���
	void AddEgeDiscip();
	// ���������� ������� ���������� �� ��� � �������
	htmlayout::dom::element GetCurEgeDiscip();
	// ������� ������ �������� �� ���
	void DeleteEgeDiscip();
	// ��������� ������� ������������� ������ ��������
	bool IsExistsDorovorNumber(void);
	// ��������� �� ������� ������ �������
	bool IsExistsZnum(void);
	// ��������� �� ��������� �� ��� �������
	bool IsFalseEduDocInfo(void);
	// �������� ��� ����� ������ ���� ���������� ������
	void UpdateAllPayFactsForStudent(void);
	// �������� ��� ������ ���� ���������� �������������
	void UpdateAllProgressForStudent(void);
	// ���������� ����� ��� ������
	long GetCityIdForGroup(long GroupId);
	// ��������� ������������ ��������� ������, ������������ ��
	void get_current_value(std::map<string_t, string_t>& value);
public:
	// ������� �  ���� ���������� � ����� ��������
	bool CreateData(void);
	// ������ �����, ��� ������� ������
	bool DeleteData(void);
	// ��������� ������ �� ��������
	bool SaveData(void);
};
