#pragma once
#include <string>
#include <vector>
class Encoded
{
public:
	Encoded()=default;
	~Encoded()=default;
	static std::string EncodedBase64(const char*  str, int inLen);
	static std::vector<unsigned char> DecodedBase64(const char*  str, int inLen);
	static std::string ChartoHex(unsigned char* str, int length);
private:

};
