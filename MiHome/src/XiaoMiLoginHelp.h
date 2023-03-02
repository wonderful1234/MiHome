#pragma once
#include <map>
const std::string accountUrl = "https://account.xiaomi.com";
class XiaoMiLoginHelp
{
public:
	XiaoMiLoginHelp()=default;
	~XiaoMiLoginHelp()=default;
	static std::map<std::string,std::string> Login(const std::string & userName,const std::string & passWord);

private:
	static void LoginFirst(const std::string & userName,std::map<std::string, std::string> & result);
	static void LoginSecond(const std::string & userName, const std::string & passWord, std::map<std::string, std::string> & result);
	static void LoginThird(std::map<std::string, std::string> & result);

};