#include "MiHomeHelp.h"
#include <random>
#include "Encoded.h"
#include "HashHelp.h"
#include "StringHelp.h"
#include "UrlHelp.h"
#include "EncryptionHelp.h"
#include "ARC4.h"
#include "DataTimeHelp.h"
#include "RequestHelp.h"
#include "fmt/core.h"
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
	auto res=HashHelp::SHA256((const char *)ssecurityData.data(), ssecurityData.size(),false);
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
	return HashHelp::SHA1(signatureString.data(), signatureString.size(), false);
}

void MiHomeHelp::GenerateEncParams(const std::string & url, const std::string & method, const std::string & signedNonce, const std::string & nonce, std::multimap<std::string, std::string>& postData, const std::string & ssecurity)
{
	postData.emplace("rc4_hash__", GenerateEncSignature(url, method, signedNonce, postData));
	for (auto & item : postData)
	{
		item.second = Encrypt(signedNonce, item.second);
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
	auto res=EncryptedApiPost(url, params, ssecurity, cuserId, serviceToken);
	auto json = nlohmann::json::parse(res);
	auto list = json["result"]["list"];
	std::vector<std::map<std::string, std::string>> devices;
	devices.reserve(list.size());
	for (auto & item : list)
	{
		std::map<std::string, std::string> device;
		device["did"] = item["did"];
		device["localip"] = item["localip"];
		device["mac"] = item["mac"];
		device["model"] = item["model"];
		devices.emplace_back(device);
	}
	return devices;
}

std::string MiHomeHelp::DealJsonGet(nlohmann::json & obj, const std::string & url, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken, bool decrypt)
{
	auto values = obj.dump();
	std::multimap<std::string, std::string> postData;
	postData.emplace("data", values);
	return EncryptedApiGet(url, postData, ssecurity, cuserId, serviceToken, decrypt);
}

std::string MiHomeHelp::Encrypt(const std::string & password, const std::string & payload)
{
	auto passBytes=Encoded::DecodedBase64(password.data(), password.length());
	ARC4 arc4(passBytes.data(),passBytes.size());
	unsigned char tmp[1024] = {};
	arc4.Crypt(tmp, 1024);
	auto text=arc4.Crypt((unsigned char*)payload.data(), payload.size());
	auto res=Encoded::EncodedBase64((const char *)text.data(),text.size()-1);
	return std::move(res);
}

std::string MiHomeHelp::Decrypt(const std::string & password, const std::string & payload)
{
	auto passBytes = Encoded::DecodedBase64(password.data(), password.length());
	auto payBytes= Encoded::DecodedBase64(payload.data(), payload.length());
	ARC4 arc4(passBytes.data(), passBytes.size());
	unsigned char tmp[1024] = {};
	arc4.Crypt(tmp, 1024);
	auto text = arc4.Crypt(payBytes.data(), payBytes.size());
	std::string res= std::string((char*)text.data());
	return std::move(res);
}

std::string MiHomeHelp::DealJsonPost(nlohmann::json & obj, const std::string & url, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken)
{
	auto values = obj.dump();
	std::multimap<std::string, std::string> postData;
	postData.emplace("data", values);
	return EncryptedApiPost(url, postData, ssecurity, cuserId, serviceToken);
}

std::string MiHomeHelp::EncryptedApiPost(const std::string & url, std::multimap<std::string, std::string>& postData, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken, bool decrypt)
{
	auto millis = DataTimeHelp::GetTimestampMill();
	auto nonce = GenerateNonce(millis);
	auto signedNonce = SignedNonce(nonce, ssecurity);
	GenerateEncParams(url, "POST", signedNonce, nonce, postData, ssecurity);
	auto cookie = fmt::format("cUserId={0};yetAnotherServiceToken={1};serviceToken={1};locale=zh_CN;timezone=GMT+08:00;is_daylight=0;dst_offset=0;channel=MI_APP_STORE", cuserId, serviceToken);
	std::multimap<std::string, std::string> headers
	{
		{"Content-Type","application/x-www-form-urlencoded"},
		{"User-Agent","Android-7.1.1-1.0.0-ONEPLUS A3010-136-AABAECECCADEE APP/xiaomi.smarthome APPV/62830"},
		{"Cookie",cookie},
		{"Accept-Encoding","identity"},
		{"x-xiaomi-protocal-flag-cli","PROTOCAL-HTTP2"},
		{"MIOT-ENCRYPT-ALGORITHM","ENCRYPT-RC4"}
	};
	auto res=RequestHelp::HttpsPost(url, postData, headers);
	std::string body= std::string();
	if (res && res->status == 200)
	{
		body = res->body;
		if (decrypt)
			return Decrypt(signedNonce, body);
	}
	return std::move(body);
}

std::string MiHomeHelp::EncryptedApiGet(const std::string & url, std::multimap<std::string, std::string>& postData, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken, bool decrypt)
{
	auto millis = DataTimeHelp::GetTimestampMill();
	auto nonce = GenerateNonce(millis);
	auto signedNonce = SignedNonce(nonce, ssecurity);
	GenerateEncParams(url, "GET", signedNonce, nonce, postData, ssecurity);
	for (auto & item : postData)
	{
		item.second=UrlHelp::UrlEncode(item.second);
	}
	auto data=FindValue<std::string,std::string>(postData, "data");
	auto hash=FindValue<std::string, std::string>(postData, "rc4_hash__"); 
	auto sig = FindValue<std::string, std::string>(postData, "signature");
	auto nonc = FindValue<std::string, std::string>(postData, "_nonce");
	auto ssec = FindValue<std::string, std::string>(postData, "ssecurity");
	auto params = fmt::format("data={0}&rc4_hash__={1}&signature={2}&_nonce={3}&ssecurity={4}", data,hash,sig,nonc,ssec);
	auto cookie = fmt::format("cUserId={0};yetAnotherServiceToken={1};serviceToken={1};locale=zh_CN;timezone=GMT+08:00;is_daylight=0;dst_offset=0;channel=MI_APP_STORE", cuserId, serviceToken);
	std::multimap<std::string, std::string> headers
	{
		{"Content-Type","application/x-www-form-urlencoded"},
		{"User-Agent","Android-7.1.1-1.0.0-ONEPLUS A3010-136-AABAECECCADEE APP/xiaomi.smarthome APPV/62830"},
		{"Cookie",cookie},
		{"Accept-Encoding","identity"},
		{"x-xiaomi-protocal-flag-cli","PROTOCAL-HTTP2"},
		{"MIOT-ENCRYPT-ALGORITHM","ENCRYPT-RC4"}
	};
	auto tempUrl = url + "?" + params;
	auto res = RequestHelp::HttpsGet(tempUrl, headers);
	std::string body = std::string();
	if (res && res->status == 200)
	{
		body = res->body;
		if (decrypt)
			return Decrypt(signedNonce, body);
	}
	return std::move(body);
}

std::string MiHomeHelp::GetUserDeviceSetting(const std::string & did, const std::vector<std::string>& key, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken)
{
	std::string  url = "https://api.io.mi.com/app/v2/device/getsettingv2";
	nlohmann::json obj;
	obj["did"] = did;
	obj["settings"] = key;
	return DealJsonPost(obj, url, ssecurity, cuserId, serviceToken);
}

std::string MiHomeHelp::GetUserDeviceData(const std::string & did, const std::string & type, long long beginTime, long long endTime, const std::vector<std::string>& key, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken)
{
	auto url = "https://api.io.mi.com/app/user/get_user_device_data";
	nlohmann::json obj;
	obj["did"] = did;
	obj["time_start"]=beginTime;
	obj["time_end"]=endTime;
	obj["limit"]=10000;
	obj["type"]=type;
	obj["key"]=key;
	return DealJsonPost(obj, url, ssecurity, cuserId, serviceToken);
}

template<typename T, typename U>
 U MiHomeHelp::FindValue(const std::multimap<T, U>& postData, const T&& key)
{
	 auto iter=postData.find(key);
	 if (iter != postData.end())
		 return iter->second;
	return U();
}
