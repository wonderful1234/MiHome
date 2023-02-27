#include "HashHelp.h"
#include "openssl/evp.h"
std::string HashHelp::MD5(const std::string & str)
{
	unsigned char resultT[16] = {};
	EVP_Digest(str.c_str(),str.length(), resultT, nullptr, EVP_md5(), nullptr);
	return UChartoHex(resultT,16);
}

std::string HashHelp::SHA1(const std::string & str)
{
	unsigned char resultT[20] = {};
	EVP_Digest(str.c_str(), str.length(), resultT, nullptr, EVP_sha1(), nullptr);
	return UChartoHex(resultT, 20);
}

std::string HashHelp::SHA256(const std::string & str)
{
	unsigned char resultT[32] = {};
	EVP_Digest(str.c_str(), str.length(), resultT, nullptr, EVP_sha256(), nullptr);
	return UChartoHex(resultT,32);
}

std::string HashHelp::UChartoHex(unsigned char* str,int length)
{
	std::string result;
	const char map[] = "0123456789ABCDEF";
	for (int i = 0; i < length; ++i) {
		result += map[str[i] / 16];
		result += map[str[i] % 16];
	}
	return std::move(result);
}
