#include "XiaoMiLoginHelp.h"
#include "MiHomeHelp.h"
#include "EncryptionHelp.h"
int main()
{
	/*std::multimap<std::string, std::string> params = {
		{"data", "{\"getVirtualModel\":true,\"getHuamiDevices\":1,\"get_split_device\":false,\"support_smart_home\":true}"}

	};
	for (auto & item : params)
	{
		item.second = item.first;
	}*/
	EncryptionHelp::Rc4Encrypt("123", "345", 3);
	auto fff=MiHomeHelp::Encrypt("qegk3+P/u8wx1h23TsEn8Yps8dTLScSqRQDDxAP/l1k=","{\"getVirtualModel\":true,\"getHuamiDevices\":1,\"get_split_device\":false,\"support_smart_home\":true}");
	XiaoMiLoginHelp::Login("", "");
}

