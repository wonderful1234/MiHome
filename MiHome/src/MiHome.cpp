﻿#include <iostream>
#include "XiaoMiLoginHelp.h"
#include "Camera.h"
#include <experimental/filesystem>
#include "Lock.h"
int main()
{
	/*std::multimap<std::string, std::string> params = {
		{"data", "{\"getVirtualModel\":true,\"getHuamiDevices\":1,\"get_split_device\":false,\"support_smart_home\":true}"}

	};
	for (auto & item : params)
	{
		item.second = item.first;
	}*/
	/*auto dir=std::experimental::filesystem::current_path();
	std::experimental::filesystem::create_directory(dir);*/
	{
		Lock lock;
		Begin(lock, "", "", "60:7E:A4:4C:07:17");

	}
	{
		/*Camera camera;
		Begin(camera, "", "", "60:7E:A4:4C:07:17");*/
	}
}

