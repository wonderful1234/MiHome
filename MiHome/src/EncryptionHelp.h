#pragma once
#include <string>
class EncryptionHelp
{
public:
	EncryptionHelp()=default;
	~EncryptionHelp()=default;
	static void AES128CBCEncrypt(const char * key,const char * str, const char * iv,char * out);
	static void AES128CBCDecrypt(const char * key, const char * str, const char * iv, char * out);
	static void Rc4Encrypt(const char * key, const char * str,int inLen);
	static void Rc4Decrypt(const char * key, const char * str, char * out);
	

private:

};
