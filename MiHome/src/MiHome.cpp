#include <iostream>
#include "XiaoMiLoginHelp.h"
#include "Camera.h"
#include "UThreadPool.h"
#include <experimental/filesystem>
int main()
{
	/*std::multimap<std::string, std::string> params = {
		{"data", "{\"getVirtualModel\":true,\"getHuamiDevices\":1,\"get_split_device\":false,\"support_smart_home\":true}"}

	};
	for (auto & item : params)
	{
		item.second = item.first;
	}*/
	auto dir=std::experimental::filesystem::current_path();
	std::experimental::filesystem::create_directory(dir);
	{
		CTP::UThreadPool tp;
		int i = 6, j = 3;
		auto r1 = tp.commit([i, j] { return i - j; });
		std::cout << r1.get() << std::endl;
	}
	
	{
		Camera camera;
		Begin(camera, "", "", "");
	}
	XiaoMiLoginHelp::Login("", "");
}

