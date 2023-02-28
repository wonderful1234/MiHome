#pragma once
#include <string>
#include <tuple>
class UrlHelp
{
public:
	UrlHelp()=default;
	~UrlHelp()=default;
	static std::tuple<std::string,std::string> GetUrlDomainAndPath(const std::string & url);
	static std::string UrlEncode(const std::string & str);
	static std::string UrlDecode(const std::string & str);

private:

};

