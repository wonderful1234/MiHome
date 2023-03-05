#include <iostream>
#include "XiaoMiLoginHelp.h"
#include "Camera.h"
#include "UThreadPool.h"
int main()
{
	/*std::multimap<std::string, std::string> params = {
		{"data", "{\"getVirtualModel\":true,\"getHuamiDevices\":1,\"get_split_device\":false,\"support_smart_home\":true}"}

	};
	for (auto & item : params)
	{
		item.second = item.first;
	}*/
	CTP::UThreadPool tp;
	int i = 6, j = 3;
	auto r1 = tp.commit([i, j] { return i - j; });
	std::cout << r1.get() << std::endl;
	{
		Camera camera;
		Begin(camera, "", "", "");
	}
	XiaoMiLoginHelp::Login("", "");
}

