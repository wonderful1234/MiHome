#include "StringHelp.h"
#include <algorithm>
void StringHelp::ToUpper(std::string & str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return toupper(c); });
}

void StringHelp::ToLower(std::string & str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return tolower(c); });
}

std::vector<std::string> StringHelp::Split(const std::string & str, char sep)
{
	std::vector<std::string> res;
	std::string::const_iterator start = str.begin();
	std::string::const_iterator end = str.end();
	std::string::const_iterator next = find(start, end, sep);

	while (next != end)
	{
		res.emplace_back(start, next);
		start = next + 1;
		next = std::find(start, end, sep);
	}

	res.emplace_back(start, next);
	return res;
}


