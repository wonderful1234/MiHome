#include "Audio.h"
#include "XiaoMiLoginHelp.h"
#include "fmt/core.h"
#include "RequestHelp.h"
#include "json.hpp"
#include "HashHelp.h"
#include "UrlHelp.h"
#include "DataTimeHelp.h"
void Audio::DownLoadData(const std::string & userName, const std::string & passWord, const std::string & parms)
{
	auto loginInfo=Login(userName, passWord);
	auto userId = loginInfo["userId"];
	auto cuserId = loginInfo["cuserId"];
	auto serviceToken = loginInfo["serviceToken"];
	auto deviceId = GetdeviceId(parms, cuserId, serviceToken, userId);
	auto currTimeStamp=DataTimeHelp::GetTimestampMill();
	auto requestId = GetRequestId();
	auto url=fmt::format("https://userprofile.mina.mi.com/device_profile/conversation?limit=1000&timestamp={0}&requestId={1}", currTimeStamp, requestId);
	auto cookie = fmt::format("cUserId={0};deviceId={1};serviceToken={2};sn={3};userId={4}", cuserId, deviceId, serviceToken, parms, userId);
	std::multimap<std::string, std::string> headers
	{
		{"User-Agent","MICO/AndroidApp/mijia/7.6.703"},
		{"Cookie",cookie}
	};
	auto res = RequestHelp::HttpsGet(url, headers);
	if (res && res->status == 200)
	{
		auto body = res->body;
		auto json = nlohmann::json::parse(body);
		auto list=json["data"]["records"];
		for (auto & item : list)
		{
			auto time= item["timestamp"].dump();
			auto group = item["recordGroup"].dump();
			auto obj = nlohmann::json::parse(group);
			std::string q = obj["user"]["content"];
			std::string s = obj["xiaoai"]["content"];
			if (s.empty())
				s = u8"ºÃµÄ";
		}
	}
}

std::map<std::string, std::string> Audio::Login(const std::string & userName, const std::string & passWord)
{
	std::string location = "";
	std::map<std::string, std::string> result;
	auto loginInfo=XiaoMiLoginHelp::Login(userName, passWord);
	auto userId = loginInfo["userId"];
	auto passToken = loginInfo["passToken"];
	result["userId"] = userId;
	result["cuserId"] = loginInfo["cuserId"];

	std::string url ="https://account.xiaomi.com/pass/serviceLogin?_json=true&sid=micoapi&_locale=zh_CN";
	auto cookie = fmt::format("userId={0};deviceId=shZWmpt1ETLrsOtz; sdkVersion=accountsdk-2020.01.09;passToken={1}", userId,passToken);
	std::multimap<std::string, std::string> headers
	{
		{"Content-Type","application/x-www-form-urlencoded"},
		{"User-Agent","Android-7.1.1-1.0.0-ONEPLUS A3010-136-AABAECECCADEE APP/xiaomi.smarthome APPV/62830"},
		{"Cookie",cookie}

	};
	auto res = RequestHelp::HttpsGet(url, headers);
	if (res && res->status == 200)
	{
		auto body = res->body;
		auto index = body.find("{");
		body = body.substr(index);
		auto json = nlohmann::json::parse(body);
		location = json["location"];
		auto none = json["nonce"].dump();
		std::string sy = json["ssecurity"];
		auto ns=fmt::format("nonce={0}&{1}", none, sy);
		auto clientSign= UrlHelp::UrlEncode(HashHelp::SHA1(ns.data(), ns.size(), false));
		location+=fmt::format("&_userIdNeedEncrypt=true&clientSign={0}", clientSign);
		headers.erase("Cookie");
		res = RequestHelp::HttpsGet(location, headers);
		if (res && res->status == 200)
		{
			for (auto iter = res->headers.rbegin(); iter != res->headers.rend(); ++iter)
			{
				if (iter->first == "Set-Cookie")
				{
					auto beginIndex = iter->second.find("serviceToken=");
					if (beginIndex != -1)
					{
						auto endIndex = iter->second.find(";", beginIndex);
						result["serviceToken"] = iter->second.substr(beginIndex + 13, endIndex - beginIndex - 13);
						break;
					}

				}
			}
		}
	}
	return std :: move(result);
}

std::string Audio::GetdeviceId(const std::string & sn, const std::string & cuserId, const std::string & serviceToken, const std::string & userId)
{
	std::string deviceId = "";
	auto requestId= GetRequestId();
	auto url=fmt::format("https://api2.mina.mi.com/admin/v2/device_list?requestId={0}", requestId);
	auto cookie = fmt::format("cUserId={0};serviceToken={1};userId={2}",cuserId,serviceToken,userId);
	std::multimap<std::string, std::string> headers
	{
		{"User-Agent","MICO/AndroidApp/mijia/7.6.703"},
		{"Cookie",cookie}

	};
	auto res = RequestHelp::HttpsGet(url, headers);
	if (res && res->status == 200)
	{
		auto body = res->body;
		auto json = nlohmann::json::parse(body);
		auto list = json["data"];
		for (auto & item : list)
		{
			if (item["serialNumber"] == sn)
			{
				deviceId = item["serialNumber"];
				break;
			}
		}
	}
	return std::move(deviceId);
}

std::string Audio::GetRequestId()
{
	std::string possibleChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string result;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, possibleChars.size() - 1);
	for (int i = 0; i < 20; ++i) {
		result += possibleChars[dis(gen)];
	}
	return result;
}
