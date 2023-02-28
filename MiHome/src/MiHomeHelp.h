#pragma once
#include <string>
#include <map>
class MiHomeHelp
{
public:
	MiHomeHelp()=default;
	~MiHomeHelp()=default;
	static std::string GenerateNonce(long long millis);
	static std::string SignedNonce(const std::string& nonce, const std::string& ssecurity);
	static std::string GenerateEncSignature(const std::string & url, const std::string & method, const std::string & signedNonce, const std::multimap<std::string, std::string>& params);

private:

};

