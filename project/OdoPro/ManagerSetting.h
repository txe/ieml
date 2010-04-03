#pragma once

#include "ManagerSettingImpl.h"

class ManagerSetting : public ManagerSettingImpl
{
public:
	//--times 
	std::string	path_temp_update_dir;
	std::string path_app_dir;
public:
	std::string path_update_dir;
	std::string app_before_restart;
	std::string command_wait;
	std::string exclude_silent_update;
public:
	BEGIN_MAN_MAP(ManagerSetting)	
		LOC_ITEM(path_temp_update_dir, "ignore");
		LOC_ITEM(path_app_dir, "ignore");
		//		
		GLOB_ITEM(path_update_dir, "ignore");
		GLOB_ITEM(app_before_restart, "OdoPro.exe");
		GLOB_ITEM(command_wait, "ignore");
		GLOB_ITEM(exclude_silent_update, "ignore");
	END_MAN_MAP()
};