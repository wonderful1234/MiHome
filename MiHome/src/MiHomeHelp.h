#pragma once
#include <string>
#include <vector>
#include <map>
class MiHomeHelp
{
public:
	MiHomeHelp()=default;
	~MiHomeHelp()=default;
	static std::string GenerateNonce(long long millis);
	static std::string SignedNonce(const std::string& nonce, const std::string& ssecurity);
	static std::string GenerateEncSignature(const std::string & url, const std::string & method, const std::string & signedNonce, const std::multimap<std::string, std::string>& params);
	static void GenerateEncParams(const std::string & url, const std::string & method, const std::string & signedNonce, const std::string & nonce, std::multimap<std::string, std::string> &  postData, const std::string & ssecurity);
	static std::vector<std::map<std::string, std::string>>  GetDevices(const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken);
	static std::string Encrypt(const std::string & password, const std::string & payload);

private:

};

