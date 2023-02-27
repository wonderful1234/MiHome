#include "Encoded.h"
#include "openssl/evp.h"
std::string Encoded::EncodedBase64(const std::string & str)
{
	int size = str.length();
	size_t base64_len = (size + 2) / 3 * 4;
	if (base64_len == 0)
	{
		return "";
	}
	std::string ret;
	ret.resize(base64_len);
	EVP_EncodeBlock((unsigned char*)ret.data(), (unsigned char*)str.data(), size);
	return std::move(ret);
}

std::string Encoded::DecodedBase64(const std::string & str)
{
	size_t srcLen = str.size();
	if (srcLen % 4 != 0)
		return "";
	size_t destLen = (srcLen / 4) * 3;
	std::string ret;
	ret.resize(destLen);
	int code = EVP_DecodeBlock((unsigned char*)ret.data(), (const unsigned char*)str.c_str(), (int)srcLen);
	if (code == -1)
		return "";
	int i = 0;
	while (str.at(--srcLen) == '=')
	{
		code--;
		if (++i > 2)
			return "";
	}
	ret.resize(code);
	return std::move(ret);
}

std::string Encoded::UrlEncode(const std::string & str)
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

std::string Encoded::UrlDecode(const std::string & str)
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
