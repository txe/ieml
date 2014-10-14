#include "stdafx.h"
#include "SingeltonApp.h"
#include "ActionAddStudents.h"
#include "json-aux-ext.h"
#include <fstream>

CActionAddStudentsDlg::CActionAddStudentsDlg(LiteWnd* pParent)
{

}

void CActionAddStudentsDlg::Run()
{
  std::ifstream file("c:\\students.txt");
  if (!file)
  {
    MessageBox(::GetActiveWindow(), L"Не найден файл \"c:\\students.txt\"" , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    return;
  }

  std::vector<std::vector<std::string>> lines;
  while (file)
  {
    std::string line;
    std::getline(file, line);
    std::stringstream lineStream(line);

    std::vector<std::string> words;
    std::string word;
    while (std::getline(lineStream, word, '\t'))
      words.push_back(word);
    if (!words.empty())
      lines.push_back(words);
  }

  if (lines.size() < 2)
  {
    MessageBox(::GetActiveWindow(), L"Слишком мало данных в файле" , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    return;
  }
  
  // проверим содержимое файла
  if (!CheckHeaderData(lines[0]))
    return;
  for (size_t i = 1; i < lines.size(); ++i)
    if (!CheckStudentData(lines[i]))
      return;

  // добавим студентов в базу
  for (size_t i = 1; i < lines.size(); ++i)
  {
    std::vector<std::string>& line = lines[i];
    std::string firstName  = line[1];
    std::string secondName = line[2];
    std::string thirdName  = line[3];
    int groupId            = aux::atoi(line[6].c_str(), 0);
    std::string zNum       = line[7];
    if (!CheckGroupExist(groupId))
    {
      std::wstring msg = L"Отсутствует группа в базе: " + std::wstring(aux::itow(groupId)); 
      MessageBox(::GetActiveWindow(), msg.c_str() , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
      return;
    }
    if (CheckStudentExist(groupId, firstName, secondName, thirdName))
      continue;

    CreateStudent(groupId, firstName, secondName, thirdName, zNum);
  }
  MessageBox(::GetActiveWindow(), L"Готово" , L"", MB_OK | MB_ICONERROR | MB_APPLMODAL);
}

bool CActionAddStudentsDlg::CheckHeaderData(std::vector<std::string>& data)
{
  if (data.size() < 8)
  {
    MessageBox(::GetActiveWindow(), L"Мало колонок в хидере" , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    return false;
  }

  std::string header[8] = {"id", "firstname", "secondname", "thirdname", "sex", "bdate", "grpid", "znum"};
  for (int i = 0; i < 8; ++i)
    if (data[i] != header[i])
    {
      MessageBox(::GetActiveWindow(), L"Колонки перечислены в ошибочном порядке" , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
      return false;
    }
  return true;
}

bool CActionAddStudentsDlg::CheckStudentData(std::vector<std::string>& data)
{
  if (!data[0].empty())
  {
    MessageBox(::GetActiveWindow(), L"Колонка id должна быть пустой" , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    return false;
  }
  if (data[1].empty())
  {
    MessageBox(::GetActiveWindow(), L"Колонка firstname должна быть полной" , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    return false;
  }  
  if (data[2].empty())
  {
    MessageBox(::GetActiveWindow(), L"Колонка secondname должна быть полной" , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    return false;
  }  
  if (data[3].empty())
  {
    //MessageBox(::GetActiveWindow(), L"Колонка thirdname должна быть полной" , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    //return false;
  }  
  if (data[6].empty() || aux::atoi(data[6].c_str(), 0) == 0)
  {
    MessageBox(::GetActiveWindow(), L"Колонка grpid должна быть полной и быть числом" , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    return false;
  }  
  if (data[7].empty() || aux::atoi(data[7].c_str(), 0) == 0)
  {
    MessageBox(::GetActiveWindow(), L"Колонка znum должна быть полной и быть числом" , L"Ошибка", MB_OK | MB_ICONERROR | MB_APPLMODAL);
    return false;
  }
  return true;
}

bool CActionAddStudentsDlg::CheckGroupExist(int groupId)
{
  return theApp.GetTitleForKeyFromVoc(vok_key::VK_GRP, groupId, true) != "";
}

bool CActionAddStudentsDlg::CheckStudentExist(int groupId, std::string firstName, std::string secondName, std::string thirdName)
{
  string_t query = "SELECT id FROM students WHERE grpid = " + std::string(aux::itoa(groupId)) + " AND firstname = '" + firstName + "' AND secondname = '" + secondName + "' AND thirdname = '" + thirdName + "' AND deleted = 0 LIMIT 1";

  mybase::MYFASTRESULT res = theApp.GetCon().Query(query);
  if (!res.size())
    return false;
  return true;
}

void CActionAddStudentsDlg::CreateStudent(int groupId, std::string firstName, std::string secondName, std::string thirdName, std::string znum)
{
  string_t query = string_t() +
    " INSERT INTO students (grpid, firstname, secondname, thirdname, znum) "
    " VALUES(" + aux::itoa(groupId) + ", '" + firstName +"', '" + secondName + "', '" + thirdName + "', '" + znum + "')";

  try
  {
    theApp.GetCon().Query(query);	
  }
  catch (...)
  {
    theApp.ExceptionManage();
  }
}
