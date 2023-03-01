#include "MiHomeHelp.h"
#include <random>
#include "Encoded.h"
#include "HashHelp.h"
#include "StringHelp.h"
#include "UrlHelp.h"
#include "EncryptionHelp.h"
std::string MiHomeHelp::GenerateNonce(long long millis)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	unsigned char nonceBytes[13] = {};
	for (int i = 0; i < 8; i++) {
		nonceBytes[i] = dis(gen);
	}
	int time=static_cast<int>(millis / 60000);
	unsigned char bytes[4] = {};
	std::memcpy(bytes, &time, 4);
	for (int i = 0; i < 4/2; i++)
	{
		auto tmpbyte = bytes[i];
		bytes[i] = bytes[4 - 1 - i];
		bytes[4 - 1 - i] = tmpbyte;
	}
	std::memcpy(&nonceBytes[8],bytes, 4);
	return Encoded::EncodedBase64((const char *) nonceBytes,12);
}

std::string MiHomeHelp::SignedNonce(const std::string& nonce, const std::string& ssecurity)
{
	auto nonceData=Encoded::DecodedBase64(nonce.data(), nonce.length());
	auto ssecurityData=Encoded::DecodedBase64(ssecurity.data(), ssecurity.length());
	ssecurityData.insert(ssecurityData.end(),nonceData.begin(),nonceData.end());
	auto res=HashHelp::SHA256((const char *)ssecurityData.data(),false);
	return std::move(res);
}

std::string MiHomeHelp::GenerateEncSignature(const std::string & url, const std::string & method, const std::string & signedNonce, const std::multimap<std::string, std::string>& params)
{
	auto path=std::get<1>(UrlHelp::GetUrlDomainAndPath(url));
	if (path.substr(0, 5) == "/app/")
		path = path.substr(4);
	std::string methodTmp = method;
	StringHelp::ToUpper(methodTmp);
	std::vector<std::string> signatureParams;
	signatureParams.emplace_back(methodTmp);
	signatureParams.emplace_back(path);
	for (auto & item : params)
	{
		signatureParams.emplace_back(item.first + "=" + item.second);
	}
	signatureParams.emplace_back(signedNonce);
	std::string signatureString;
	for (auto & item : signatureParams)
	{
		signatureString += item + "&";
	}
	signatureString.pop_back();
	return HashHelp::SHA1(signatureString.data(), false);
}

void MiHomeHelp::GenerateEncParams(const std::string & url, const std::string & method, const std::string & signedNonce, const std::string & nonce, std::multimap<std::string, std::string>& postData, const std::string & ssecurity)
{
	postData.emplace("rc4_hash__", GenerateEncSignature(url, method, signedNonce, postData));
	for (auto & item : postData)
	{
		item.second = item.first;
	}
	postData.emplace("signature", GenerateEncSignature(url, method, signedNonce, postData));
	postData.emplace("ssecurity", ssecurity);
	postData.emplace("_nonce", nonce);
}

std::vector<std::map<std::string, std::string>> MiHomeHelp::GetDevices(const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken)
{
	std::string url = "https://api.io.mi.com/app/home/device_list";
	std::multimap<std::string, std::string> params = {
		{"data", "{\"getVirtualModel\":true,\"getHuamiDevices\":1,\"get_split_device\":false,\"support_smart_home\":true}"}
	};
	return std::vector<std::map<std::string, std::string>>();
}

std::string MiHomeHelp::Encrypt(const std::string & password, const std::string & payload)
{
	auto passBytes=Encoded::DecodedBase64(password.data(), password.length());
	char * out = new char[payload.length()]();
	auto res=Encoded::EncodedBase64(out,payload.length());
	delete[]out;
	out = nullptr;
	return std::move(res);
}
