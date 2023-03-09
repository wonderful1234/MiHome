#include "StringHelp.h"
#include <algorithm>
#include <sstream>
#include <regex>
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

std::string StringHelp::BigEndianToLittleEndian(const std::string & bigEndianHexString)
{
	auto tmpStr = bigEndianHexString;
	if (tmpStr.size() % 2 != 0)
		tmpStr = '0' + tmpStr;
	std::string littleEndianHexString = "";
	for (size_t i = tmpStr.size() - 2; i >= 0; i -= 2)
		littleEndianHexString += tmpStr.substr(i, 2);
	return std::move(littleEndianHexString);
}

std::string StringHelp::DecNumStrToBigEndianHexStr(long long decimalNum, int length)
{
	std::stringstream ss;
	ss << std::uppercase << std::hex << decimalNum; 
	std::string hexByteStr = ss.str();

	if (hexByteStr.length() < length) { 
		hexByteStr = std::string(length - hexByteStr.length(), '0') + hexByteStr;
	}

	std::string bigEndianString="";
	for (int i = 0; i < length; i += 2) { 
		bigEndianString = hexByteStr.substr(i, 2) + bigEndianString;
	}
	return bigEndianString;
}

std::string StringHelp::listGetData(const std::vector<std::string>& list, const std::string & containText, const std::string & removeText, bool getNext, const std::string & nextContainText, bool allMatch)
{
	std::string ret = "";
	std::regex containReg(containText);
	std::regex nextContainReg(nextContainText);
	std::regex removeReg(removeText);
	std::smatch match;

	auto FindIndexOfReg = [&](std::string str) -> int {
		auto size = match.size();
		if (size == 1 && match[0] == str)
			return 0;
		for (auto i = 1; i < size; i++)
		{
			std::string item = match[i];
			if (item.empty())
				continue;
			auto index = str.find_first_of(item);
			if (index != -1)
				return index;
		}
		return -1;

	};

	auto removeFReg = [&](std::string &str) {

		auto size = match.size();
		if (size == 1 && match[0] == str)
		{
			str = "";
			return;
		}
		for (auto i = 1; i < size; i++)
		{
			std::string item = match[i];
			if (item.empty())
				continue;
			auto index = str.find_first_of(item);
			if (index != -1)
			{
				str.replace(index, index + item.size(), "");
			}	
		}
			
	};

	for (auto it = list.begin(); it != list.end(); ++it)
	{
		if(!std::regex_search(*it, match, containReg))
			continue;
		ret = *it;
		if (getNext)
		{
			++it;
			if (!nextContainText.empty())
			{
				if (it == list.end())
				{
					if (std::regex_search(ret, match, nextContainReg)) //Õ³Á¬ÁË
					{
						auto index = FindIndexOfReg(ret);
						if (index != -1)
							ret = ret.substr(index);
					}
					else
					{
						return "";
					}
				}
				else
				{
					if (!std::regex_search(*it, match, nextContainReg) && !std::regex_search(ret, match, nextContainReg))
						return "";
					else if (std::regex_search(*it, match, nextContainReg))
						ret = *it;
					else if (std::regex_search(ret, match, nextContainReg))
					{
						auto index = FindIndexOfReg(ret);
						if (index != -1)
							ret = ret.substr(index);
					}

				}
			}
			else
			{
				if (it == list.end())
					return "";
				ret = *it;

			}
		}
		if (allMatch)
		{
			if (!std::regex_search(ret, match, nextContainReg))
				return ret;
			ret = match[0];
		}
		else if (!removeText.empty())
		{
			if (std::regex_search(ret, match, removeReg))
				removeFReg(std::ref(ret));
		}
		ret = Trim(ret);
		if(ret.empty() && it!=list.end())
			continue;
		break;
	}
	return std::move(ret);
}

std::string StringHelp::Trim(const std::string & str)
{
	size_t first = str.find_first_not_of(' ');
	if (first == std::string::npos) {
		return "";
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, last - first + 1);
}


