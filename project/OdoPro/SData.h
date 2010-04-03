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
private:
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
