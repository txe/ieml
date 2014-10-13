#pragma once
#include "litewnd.h"
#include <deque>


class CActionAddStudentsDlg 
{
public:
	CActionAddStudentsDlg(LiteWnd* pParent);

  void Run();

private:
  bool CheckHeaderData(std::vector<std::string>& data);
  bool CheckStudentData(std::vector<std::string>& data);
  bool CheckGroupExist(int groupId);
  bool CheckStudentExist(int groupId, std::string firstName, std::string secondName, std::string thirdName);
  void CreateStudent(int groupId, std::string firstName, std::string secondName, std::string thirdName, int znum);
};
