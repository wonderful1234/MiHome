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

std::vector<unsigned char> StringHelp::HexStringToChar(const std::string & hexStr)
{
	std::vector<unsigned char> buffer;
	for (size_t i = 0; i < hexStr.size();i+=2)
	{
		std::string byteString = hexStr.substr(i, 2);
		auto value=strtol(byteString.c_str(), nullptr, 16);
		buffer.push_back(static_cast<unsigned char>(value));
	}
	buffer.push_back('\0');
	return buffer;
}

bool StringHelp::StartWith(const std::string & str, const std::string & prefix)
{
	size_t prefix_len = prefix.size();
	if (str.size() < prefix_len)
		return false;
	for (size_t i = 0; i < prefix_len; i++)
	{
		if (str[i] != prefix[i])
			return false;
	}
	return true;
}


