#include "Encoded.h"
#include "openssl/evp.h"
#include "iconv.h"
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
	auto outLen = (inLen / 4) * 3;
	std::vector<unsigned char> out(outLen);
	auto size=EVP_DecodeBlock(out.data(), (const unsigned char*)str, inLen);
	while (str[--inLen] == '=')
	{
		size -= 1;
	}
	out.resize(size);
	return std::move(out);
}

std::vector<char> Encoded::UTF8toGBK(char * inStr, size_t inLen)
{
	std::vector<char> outStr(inLen);
	char ** inBuffer = &inStr;
	auto outChars = outStr.data();
	char ** outBuffer = &outChars;
	size_t  outLen = inLen;
	iconv_t cd = iconv_open("GBK", "UTF-8");
	iconv(cd, inBuffer, &inLen, outBuffer, &outLen);
	iconv_close(cd);
	return outStr;
}

std::vector<char> Encoded::GBKtoUTF8(char * inStr, size_t inLen)
{
	std::vector<char> outStr(inLen);
	char ** inBuffer = &inStr;
	auto outChars = outStr.data();
	char ** outBuffer = &outChars;
	size_t  outLen = inLen;
	iconv_t cd = iconv_open("UTF-8", "GBK");
	iconv(cd, inBuffer, &inLen, outBuffer, &outLen);
	iconv_close(cd);
	return outStr;
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
