#include "MiHomeHelp.h"
#include <random>
#include "Encoded.h"
#include "HashHelp.h"
#include "StringHelp.h"
#include "UrlHelp.h"
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
	return Encoded::EncodedBase64((const char *) nonceBytes);
}

std::string MiHomeHelp::SignedNonce(const std::string& nonce, const std::string& ssecurity)
{
	char * nonceData = new char[nonce.length()]();
	char * ssecurityData = new char[ssecurity.length()]();
	Encoded::DecodedBase64(nonce.data(),nonceData);
	Encoded::DecodedBase64(ssecurity.data(), ssecurityData);
	int nonceLength = strlen(nonceData);
	int ssecurityLength = strlen(ssecurityData);
	int length= nonceLength + ssecurityLength;
	char * data = new char[length+1]();
	std::memcpy(data, ssecurityData, ssecurityLength);
	std::memcpy(data+ssecurityLength, nonceData, nonceLength);
	auto res=HashHelp::SHA256(data,false);
	delete[]nonceData;
	delete[]ssecurityData;
	delete[]data;
	nonceData = nullptr;
	ssecurityData = nullptr;
	data = nullptr;
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
	std::string signatureString;
	for (auto & item : signatureParams)
	{
		signatureString += item + "&";
	}
	signatureString.pop_back();
	return std::string();
}
