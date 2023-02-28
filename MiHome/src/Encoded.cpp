#include "Encoded.h"
#include "openssl/evp.h"
std::string Encoded::EncodedBase64(const char*  str)
{
	int length = strlen(str);
	size_t base64_len = (length + 2) / 3 * 4;
	if (base64_len == 0)
	{
		return "";
	}
	std::string ret;
	ret.resize(base64_len);
	EVP_EncodeBlock((unsigned char*)ret.data(), (unsigned char*)str, length);
	return std::move(ret);
}

void  Encoded::DecodedBase64(const char*  str, char * out)
{
	int length = strlen(str);
	size_t destLen = (length / 4) * 3;
	EVP_DecodeBlock((unsigned char *)out, (const unsigned char*)str, length);
}

std::string Encoded::ChartoHex(const char * str, int length)
{
	std::string result;
	const char map[] = "0123456789ABCDEF";
	for (int i = 0; i < length; ++i) {
		result += map[str[i] / 16];
		result += map[str[i] % 16];
	}
	return std::move(result);
}
