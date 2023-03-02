#pragma once
#include <string>
class HashHelp
{
public:
	HashHelp()=default;
	~HashHelp()=default;
	static std::string MD5(const char* str, int inLen, bool hex=true);
	static std::string SHA1(const char* str, int inLen, bool hex = true);
	static std::string SHA256(const char* str,int inLen, bool hex = true);

private:

};

