#include "UrlHelp.h"
#include <regex>
std::tuple<std::string, std::string> UrlHelp::GetUrlDomainAndPath(const std::string & url)
{
	std::tuple<std::string, std::string> data{"",""};
	std::regex re("^(http|https)://([^/]+)(.*)$");
	std::smatch match;
	if (std::regex_search(url, match, re))
	{
		std::string domain = match[2];
		std::string path = match[3];
		if (path == "")
			path = "/";
		std::get<0>(data) = domain;
		std::get<1>(data) = path;

	}
	return data;
}

std::string UrlHelp::UrlEncode(const std::string & str)
{
	std::string result = "";
	for (auto c : str)
	{
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
		{
			result += c;
		}
		else
		{
			result += '%' + std::string(1, c);
		}
	}
	return std::move(result);
}

std::string UrlHelp::UrlDecode(const std::string & str)
{
	std::string result = "";
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == '%')
		{
			if (i + 2 < str.size())
			{
				std::string hex = str.substr(i + 1, 2);
				char ch = (char)strtol(hex.c_str(), nullptr, 16);
				result += ch; i += 2;
			}
		}
		else {
			result += str[i];
		}
	}
	return std::move(result);
}
