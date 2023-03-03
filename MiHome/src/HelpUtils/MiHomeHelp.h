#pragma once
#include <string>
#include <vector>
#include <map>
#include "json.hpp"
class MiHomeHelp
{
public:
	MiHomeHelp()=default;
	~MiHomeHelp()=default;
	static std::string EncryptedApiPost(const std::string & url, std::multimap<std::string, std::string> &  postData, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken, bool decrypt = true);
	static std::string EncryptedApiGet(const std::string & url, std::multimap<std::string, std::string> &  postData, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken, bool decrypt = true);
	static std::string GetUserDeviceSetting(const std::string & did, const std::vector<std::string> & key, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken);
	static std::string GetUserDeviceData(const std::string & did, const std::string & type, long long beginTime, long long endTime, const std::vector<std::string> & key, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken);
	static std::vector<std::map<std::string, std::string>>  GetDevices(const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken);
private:
	static std::string GenerateNonce(long long millis);
	static std::string SignedNonce(const std::string& nonce, const std::string& ssecurity);
	static std::string GenerateEncSignature(const std::string & url, const std::string & method, const std::string & signedNonce, const std::multimap<std::string, std::string>& params);
	static void GenerateEncParams(const std::string & url, const std::string & method, const std::string & signedNonce, const std::string & nonce, std::multimap<std::string, std::string> &  postData, const std::string & ssecurity);
	static std::string Decrypt(const std::string & password, const std::string & payload);
	static std::string Encrypt(const std::string & password, const std::string & payload);
	static std::string DealJsonPost(nlohmann::json  & obj, const std::string & url, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken);
	template<typename T, typename U>
	static U FindValue(const std::multimap<T, U> &  postData, const T&& key);
};

