﻿#include <iostream>
#include "XiaoMiLoginHelp.h"
#include "MiHomeHelp.h"
#include "Camera.h"
int main()
{
	/*std::multimap<std::string, std::string> params = {
		{"data", "{\"getVirtualModel\":true,\"getHuamiDevices\":1,\"get_split_device\":false,\"support_smart_home\":true}"}

	};
	for (auto & item : params)
	{
		item.second = item.first;
	}*/
	{
		Camera camera;
		Begin(camera, "", "", "");
	}
	XiaoMiLoginHelp::Login("", "");
}

