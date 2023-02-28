#pragma once
#include <string>
class EncryptionHelp
{
public:
	EncryptionHelp()=default;
	~EncryptionHelp()=default;
	static void AES128CBCEncrypt(const char * key,const char * str, const char * iv,char * out);
	static void AES128CBCDecrypt(const char * key, const char * str, const char * iv, char * out);
	

private:

};
