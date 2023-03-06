#pragma once
#include <string>
#include <vector>
class EncryptionHelp
{
public:
	EncryptionHelp()=default;
	~EncryptionHelp()=default;
	static std::vector<unsigned char> AES128CBCEncrypt(const char * key,const char * str, const unsigned char * iv,int inLen);
	static std::vector<unsigned char> AES128CBCDecrypt(const unsigned char * key, const char * str, const unsigned char * iv, int inLen);
private:

};
