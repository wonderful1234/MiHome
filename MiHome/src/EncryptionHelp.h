#pragma once
#include <string>
class EncryptionHelp
{
public:
	EncryptionHelp()=default;
	~EncryptionHelp()=default;
	static std::string AES128CBCEncrypt(const std::string& key, const std::string& str, const std::string & iv);
	static std::string AES128CBCDecrypt(const std::string& key, const std::string& str, const std::string & iv);
	

private:

};
