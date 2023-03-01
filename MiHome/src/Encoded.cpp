#include "Encoded.h"
#include "openssl/evp.h"

std::string  Encoded::EncodedBase64(const char*  str, int inLen)
{
	int outLen = (inLen + 2) / 3 * 4;
	std::string out;
	out.resize(outLen);
	EVP_EncodeBlock((unsigned char*)out.data(), (unsigned char*)str, inLen);
	return out;
	
}

std::vector<unsigned char>  Encoded::DecodedBase64(const char*  str, int inLen)
{
	std::vector<unsigned char> out;
	auto outLen=(inLen / 4) * 3;
	out.resize(outLen);
	auto size=EVP_DecodeBlock(out.data(), (const unsigned char*)str, inLen);
	while (str[--inLen] == '=')
	{
		size -= 1;
	}
	out.resize(size);
	return std::move(out);
}

std::string Encoded::ChartoHex( unsigned char * str, int length)
{
	std::string result;
	const char map[] = "0123456789ABCDEF";
	for (int i = 0; i < length; ++i) {
		result += map[str[i] / 16];
		result += map[str[i] % 16];
	}
	return std::move(result);
}
