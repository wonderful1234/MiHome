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
