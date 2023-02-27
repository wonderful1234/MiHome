#pragma once
#include <string>
class Encoded
{
public:
	Encoded()=default;
	~Encoded()=default;
	static std::string EncodedBase64(const std::string & str);
	static std::string DecodedBase64(const std::string & str);
	static std::string UrlEncode(const std::string & str);
	static std::string UrlDecode(const std::string & str);
private:

};
