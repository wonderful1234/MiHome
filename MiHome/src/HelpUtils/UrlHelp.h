#pragma once
#include <string>
#include <tuple>
class UrlHelp
{
public:
	UrlHelp()=default;
	~UrlHelp()=default;
	static std::tuple< std::string , std::string > GetUrlDomainAndPath(const std::string & url);
	static std::string UrlEncode(const std::string & str);
	static void UrlDecode(std::string & str);
	static size_t UrlDecode(char *str, size_t len);

private:
	static int Htoi(unsigned char *s);
	static char Itoh(int n);
};

