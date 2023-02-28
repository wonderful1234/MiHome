#include "HashHelp.h"
#include "openssl/evp.h"
#include "Encoded.h"
std::string HashHelp::MD5(const char* str, bool hex)
{
	unsigned char resultT[16+1] = {};
	EVP_Digest(str,strlen(str), resultT, nullptr, EVP_md5(), nullptr);
	if (hex)
		return Encoded::ChartoHex(resultT, 16);
	return Encoded::EncodedBase64((const char *)resultT);
}

std::string HashHelp::SHA1(const char* str, bool hex)
{
	unsigned char resultT[20+1] = {};
	EVP_Digest(str, strlen(str), resultT, nullptr, EVP_sha1(), nullptr);
	if (hex)
		return Encoded::ChartoHex(resultT, 20);
	return Encoded::EncodedBase64((const char *)resultT);
}

std::string HashHelp::SHA256(const char* str, bool hex)
{
	unsigned char resultT[32+1] = {};
	EVP_Digest(str, strlen(str), resultT, nullptr, EVP_sha256(), nullptr);
	if (hex)
		return Encoded::ChartoHex(resultT, 32);
	return Encoded::EncodedBase64((const char *)resultT);
}

