#include "Camera.h"
#include "MiHomeHelp.h"
#include "DataTimeHelp.h"
#include "RequestHelp.h"
#include "XiaoMiLoginHelp.h"
#include "UThreadPool.h"
#include "StringHelp.h"
#include "EncryptionHelp.h"
#include <iostream>
void Camera::DownLoadData(const std::string & userName, const std::string &  passWord, const std::string & parms)
{
	auto loginMessage=XiaoMiLoginHelp::Login(userName, passWord);
	auto ssecurity = loginMessage["ssecurity"];
	auto cuserId = loginMessage["cuserId"];
	auto serviceToken = loginMessage["serviceToken"];
	auto camera = GetCamera(ssecurity, cuserId, serviceToken,parms);
	std::string did = "";
	std::string model = "";
	std::tie(did, model) = camera;
	auto videoList=GetVideoList(did, model, ssecurity, cuserId, serviceToken);
	CTP::UThreadPool tp;
	std::regex re("AES-128,\\s*URI=\"\"?(https?://[^"",]+)\"\"?,\\s*IV=(?:0x)?(\\w+)");
	std::smatch match;
	for (auto& item : videoList)
	{
		tp.commit([&]() {
			auto fileId = item["fileId"];
			std::cout << fileId << std::endl;

			auto urls=GetVideoUrl(did, model, fileId, ssecurity, cuserId, serviceToken);
			auto time = item["createTime"];
			auto array = StringHelp::Split(urls, '\n');
			std::string key = "";
			std::string iv = "";
			if (std::regex_search(array[4], match, re))
			{
				key = match[1];
				iv = match[2];
			}
			std::vector<std::string> list;
			for (size_t i = 6; i < array.size(); i++)
			{
				if (StringHelp::StartWith(array[i], "https://"))
					list.push_back(array[i]);
			}
			DownloadVideo(time, key, iv, list, "");
		});
	}

}

std::tuple<std::string, std::string> Camera::GetCamera(const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken, const std::string & mac)
{
	auto list = MiHomeHelp::GetDevices(ssecurity, cuserId, serviceToken);
	for (auto & item : list)
	{
		if (item["mac"] == mac)
		{
			std::tuple<std::string, std::string > data(item["did"], item["model"]);
			return std::move(data);
		}
	}
	return std::tuple<std::string, std::string>();
}

std::vector<std::map<std::string, std::string>> Camera::GetVideoList(const std::string & did, const std::string & model, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken)
{
	std::string  url = "https://business.smartcamera.api.io.mi.com/common/app/get/eventlist";
	auto timeInfo=DataTimeHelp::GetCurrentTimeInfo();
	auto year= timeInfo["year"] - 1900;
	auto mon= timeInfo["mon"] - 1;
	auto day= timeInfo["day"];

	tm dateBegin = {};
	dateBegin.tm_year = year;
	dateBegin.tm_mon = mon;
	dateBegin.tm_mday = day;
	dateBegin.tm_hour = 0;
	dateBegin.tm_min = 0;
	dateBegin.tm_sec = 0;
	tm dateEnd = {};
	dateEnd.tm_year = year;
	dateEnd.tm_mon = mon;
	dateEnd.tm_mday = day;
	dateEnd.tm_hour = 23;
	dateEnd.tm_min = 59;
	dateEnd.tm_sec = 59;
	std::vector<std::map<std::string, std::string>> list;
	auto beginTime = DataTimeHelp::GetTimeStamp(dateBegin);
	auto endTime= DataTimeHelp::GetTimeStamp(dateEnd);
	for (int i = 0; i < 8; i++)
	{
		auto nextTime = endTime;
		bool haveNext = true;
		while (haveNext)
		{
			nlohmann::json obj;
			obj["region"] = "CN";
			obj["did"] = did;
			obj["sortType"] = "DESC";
			obj["doorBell"] = false;
			obj["needMerge"] = true;
			obj["limit"] = 50;
			obj["endTime"] = nextTime;
			obj["beginTime"] = beginTime;
			obj["eventType"] = "Default";
			obj["model"] = model;
			auto res=MiHomeHelp::DealJsonGet(obj, url, ssecurity, cuserId, serviceToken);
			auto json = nlohmann::json::parse(res);
			auto jsonList = json["data"]["thirdPartPlayUnits"];
			if(jsonList.size()==0)
				haveNext = false;
			else
			{
				for (auto & item : jsonList)
				{
					std::map<std::string, std::string> video;
					video["fileId"] = item["fileId"];
					video["createTime"] = item["createTime"].dump();
					list.emplace_back(video);
				}
				haveNext = json["data"]["isContinue"];
				nextTime = json["data"]["nextTime"];
			}
		}
		endTime -= 86400000;
		beginTime -= 86400000;
	}
	return std::move(list);
}

std::string Camera::GetVideoUrl(const std::string & did, const std::string & model, const std::string & fileId, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken)
{
	std::string url = "https://business.smartcamera.api.io.mi.com/common/app/m3u8";
	nlohmann::json obj;
	obj["did"] = did;
	obj["fileId"] = fileId;
	obj["model"] = model;
	obj["isAlarm"] = true;
	obj["videoCodec"] = "H265";
	return MiHomeHelp::DealJsonGet(obj, url, ssecurity, cuserId, serviceToken, false);
}

std::vector<unsigned char> Camera::HttpGetByte(const std::string & url)
{
	std::vector<unsigned char> data;
	auto res=RequestHelp::HttpsGet(url);
	if (res && res->status == 200)
	{
		auto body=res->body;
		for (auto c : body)
			data.push_back(c);
	}
	data.push_back('\0');
	return data;


}

void Camera::DownloadVideo(const std::string & time, const std::string & keyUrl, const std::string & iv, const std::vector<std::string>& urls, const std::string & path)
{
	auto key = HttpGetByte(keyUrl);
	auto ivByte = StringHelp::HexStringToChar(iv);
	for (size_t i = 0; i < urls.size(); i++)
	{
		auto url = urls[i];
		auto encByte = HttpGetByte(url);
		auto decByte=EncryptionHelp::AES128CBCDecrypt(key.data(), (const char *)encByte.data(), ivByte.data(), encByte.size()-1);
	}
}
