#include "XiaoMiLoginHelp.h"
#include "RequestHelp.h"
#include "HashHelp.h"
#include "fmt/core.h"
#include "json.hpp"
std::map<std::string, std::string> XiaoMiLoginHelp::Login(const std::string & userName, const std::string & passWord)
{
	std::map<std::string, std::string> result;
	LoginFirst(userName, result);
	if (result["code"] == "00")
		return std::move(result);
	LoginSecond(userName, passWord, result);
	if (result["code"] == "00")
		return std::move(result);
	LoginThird(result);
	return std::move(result);
}

void XiaoMiLoginHelp::LoginFirst(const std::string & userName, std::map<std::string, std::string>& result)
{
	std::string user = "";
	result["havePhoneNumber"] = "0";
	if (userName.length() == 11 && userName.find("@") == -1)
	{
		user = "+86" + userName;
		result["havePhoneNumber"] = "1";
	}
	auto cookie=fmt::format("userId={0};deviceId=shZWmpt1ETLrsOtz; sdkVersion=accountsdk-2020.01.09", user);
	std::multimap<std::string, std::string> headers
	{
		{"Content-Type","application/x-www-form-urlencoded"},
		{"User-Agent","Android-7.1.1-1.0.0-ONEPLUS A3010-136-AABAECECCADEE APP/xiaomi.smarthome APPV/62830"},
		{"Cookie",cookie}

	};
	auto res=RequestHelp::HttpsGet(accountUrl, headers,443,"/pass/serviceLogin?_json=true&sid=xiaomiio&_locale=zh_CN");
	result["code"] = "00";
	if (res && res->status==200)
	{
		auto body = res->body;
		auto index=body.find("{");
		body=body.substr(index);
		auto json=nlohmann::json::parse(body);
		result["code"] = "01";
		result["sign"]= json["_sign"];
		result["qs"] = json["qs"];
		result["callback"] = json["callback"];
		result["sid"] = json["sid"];
	}
		


}

void XiaoMiLoginHelp::LoginSecond(const std::string & userName, const std::string & passWord, std::map<std::string, std::string>& result)
{
	auto passWordMd5 = HashHelp::MD5(passWord);
	std::multimap<std::string, std::string> headers
	{
		{"Content-Type","application/x-www-form-urlencoded"},
		{"User-Agent","Android-7.1.1-1.0.0-ONEPLUS A3010-136-AABAECECCADEE APP/xiaomi.smarthome APPV/62830"}
	};
	auto params = fmt::format("sid={0}&hash={1}&callback={2}&qs={3}&user={4}&_sign={5}&_json=true", result["sid"],passWordMd5,result["callback"], result["qs"], userName, result["sign"]);
	if (result["havePhoneNumber"] == "1")
		params += "&cc=+86";
	auto res=RequestHelp::HttpsPost(accountUrl, params, headers, 443, "/pass/serviceLoginAuth2");
	result["code"] = "00";
	if (res && res->status == 200)
	{
		auto body = res->body;
		auto index = body.find("{");
		body = body.substr(index);
		auto json = nlohmann::json::parse(body);
		if (json.contains("ssecurity"))
		{
			result["code"] = "01";
			result["ssecurity"] = json["ssecurity"];
			result["userId"] = json["userId"].dump();
			result["cuserId"] = json["cUserId"];
			result["passToken"] = json["passToken"];
			result["location"] = json["location"];

		}
		else if (json.contains("notificationUrl"))
		{
			result["notificationUrl"] = json["notificationUrl"];
		}
		
	}
}

void XiaoMiLoginHelp::LoginThird(std::map<std::string, std::string>& result)
{
	auto & location = result["location"];
	location = location.substr(8);
	int index = location.find(".com");
	auto url=location.substr(0, index)+".com";
	auto path = location.substr(index + 4);
	std::multimap<std::string, std::string> headers
	{
		{"Content-Type","application/x-www-form-urlencoded"},
		{"User-Agent","Android-7.1.1-1.0.0-ONEPLUS A3010-136-AABAECECCADEE APP/xiaomi.smarthome APPV/62830"}
	};
	auto res = RequestHelp::HttpsGet(url, headers, 443, path);
	result["code"] = "00";
	if (res && res->status == 200)
	{
		result["code"] = "01";
		auto iter = res->headers.find("Set-Cookie");

		for (auto iter= res->headers.rbegin();iter!=res->headers.rend();++iter)
		{
			if (iter->first == "Set-Cookie")
			{
				auto beginIndex =iter->second.find("serviceToken=");
				if (beginIndex != -1)
				{
					auto endIndex =iter->second.find(";",beginIndex);
					result["serviceToken"]= iter->second.substr(beginIndex + 13,endIndex-beginIndex-13);
					return;
				}
				
			}
		}
	}

}
