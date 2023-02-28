#pragma once
#include <string>
class UrlHelp
{
public:
	UrlHelp()=default;
	~UrlHelp()=default;
	static void GetUrlDomainAndPath(const std::string & url);
	static std::string UrlEncode(const std::string & str);
	static std::string UrlDecode(const std::string & str);

private:

};

