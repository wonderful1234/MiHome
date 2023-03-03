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

private:

};

