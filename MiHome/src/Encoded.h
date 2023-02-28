#pragma once
#include <string>
class Encoded
{
public:
	Encoded()=default;
	~Encoded()=default;
	static std::string EncodedBase64(const char*  str);
	static void  DecodedBase64(const char*  str,char * out);
	static std::string ChartoHex(unsigned char* str, int length);
private:

};
