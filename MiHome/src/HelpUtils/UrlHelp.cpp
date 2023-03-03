#include "UrlHelp.h"
#include <regex>
std::tuple<std::string , std::string > UrlHelp::GetUrlDomainAndPath(const std::string & url)
{
	std::tuple<std::string, std::string > data("", "");
	std::string domain = "";
	std::string path = "";
	std::regex re("^(http|https)://([^/]+)(.*)$");
	std::smatch match;
	if (std::regex_search(url, match, re))
	{
		domain = match[2];
		path = match[3];
		if (path == "")
			path = "/";
	}
	std::get<0>(data) = domain;
	std::get<1>(data) = path;
	return data;
}

std::string UrlHelp::UrlEncode(const std::string & str)
{
	std::string res;
	const char *cur = str.c_str();
	const char *ed = cur + str.size();

	while (cur < ed)
	{
		if (*cur == ' ')
			res += '+';
		else if (isalnum(*cur) || *cur == '-' || *cur == '_' || *cur == '.'
			|| *cur == '!' || *cur == '~' || *cur == '*' || *cur == '\''
			|| *cur == '(' || *cur == ')' || *cur == ':' || *cur == '/'
			|| *cur == '@' || *cur == '?' || *cur == '#' || *cur == '&')
			res += *cur;
		else
		{
			res += '%';
			res += Itoh(((const unsigned char)(*cur)) >> 4);
			res += Itoh(((const unsigned char)(*cur)) % 16);
		}

		cur++;
	}

	return res;
}

void UrlHelp::UrlDecode(std::string & str)
{
	if (str.empty())
		return;

	size_t sz = UrlDecode(const_cast<char *>(str.c_str()), str.size());

	str.resize(sz);
}

size_t UrlHelp::UrlDecode(char * str, size_t len)
{
	char *dest = str;
	char *data = str;

	while (len--)
	{
		if (*data == '%' && len >= 2
			&& isxdigit(*(data + 1))
			&& isxdigit(*(data + 2)))
		{
			*dest = Htoi((unsigned char *)data + 1);
			data += 2;
			len -= 2;
		}
		else if (*data == '+')
			*dest = ' ';
		else
			*dest = *data;

		data++;
		dest++;
	}

	*dest = '\0';
	return dest - str;
}

int UrlHelp::Htoi(unsigned char * s)
{

	int value;
	int c;
	c = s[0];
	if (isupper(c))
		c = tolower(c);

	value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

	c = s[1];
	if (isupper(c))
		c = tolower(c);

	value += (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10);
	return value;
}

char UrlHelp::Itoh(int n)
{
	if (n > 9)
		return n - 10 + 'A';

	return n + '0';
}
