#pragma once
#include <string>
#include <vector>
class StringHelp
{
public:
	StringHelp()=default;
	~StringHelp()=default;
	static void ToUpper(std::string &  str);
	static void ToLower(std::string & str);
	static std::vector<std::string> Split(const std::string& str, char sep);
	static std::vector<unsigned char> HexStringToChar(const std::string & hexStr);
	static bool StartWith(const std::string & str, const std::string & prefix);
	static std::string  BigEndianToLittleEndian(const std::string& bigEndianHexString);
	static std::string DecNumStrToBigEndianHexStr(long long decimalNum, int length);
private:

};

