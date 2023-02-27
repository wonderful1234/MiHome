#pragma once
#include <string>
class HashHelp
{
public:
	HashHelp()=default;
	~HashHelp()=default;
	static std::string MD5(const std::string & str);
	static std::string SHA1(const std::string & str);
	static std::string SHA256(const std::string & str);
	static std::string UChartoHex(unsigned char* str,int length);

private:

};

