#include "Lock.h"
#include "MiHomeHelp.h"
#include "DataTimeHelp.h"
#include "XiaoMiLoginHelp.h"
#include "json.hpp"
#include "CommonHelp.hpp"
#include "fmt/core.h"
void Lock::DownLoadData(const std::string & userName, const std::string & passWord, const std::string & parms)
{
	auto loginInfo=XiaoMiLoginHelp::Login(userName, passWord);
	auto ssecurity = loginInfo["ssecurity"];
	auto cuserId = loginInfo["cuserId"];
	auto serviceToken = loginInfo["serviceToken"];
	auto lockInfo=GetLockDid(ssecurity, cuserId, serviceToken);
	std::string did = "";
	std::string model = "";
	std::tie(did, model) = lockInfo;
	DownloadSetting(did, ssecurity, cuserId, serviceToken);
	DownloadEvent(did, ssecurity, cuserId, serviceToken, "LockEvent");
	DownloadEvent(did, ssecurity, cuserId, serviceToken, "DoorEvent");
	DownloadEvent(did, ssecurity, cuserId, serviceToken, "DefenceEvent");

}

std::tuple<std::string, std::string> Lock::GetLockDid(const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken)
{
	std::tuple<std::string, std::string > data("", "");
	auto list=MiHomeHelp::GetDevices(ssecurity, cuserId, serviceToken);
	for (auto & item : list)
	{
		if (item["model"].find(".lock") != -1)
		{
			std::get<0>(data) = item["did"];
			auto model = item["model"];
			auto index = model.find_last_of('.');
			std::get<1>(data) = model.substr(index + 1);
			break;
		}
	}
	return std::move(data);
}

std::string Lock::DownloadSetting(const std::string & did, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken)
{
	auto body= MiHomeHelp::GetUserDeviceSetting(did, std::vector<std::string>(), ssecurity, cuserId, serviceToken);
	auto json = nlohmann::json::parse(body);
	std::string firstData = json["result"]["settings"]["_SERVER_USER_LIST_0"];
	auto index = firstData.find_first_of(",");
	auto countString = firstData.substr(0, index);
	auto count=ParseString<int>(countString);
	auto data = firstData.substr(index + 1);
	for (int i = 0; i < count; i++)
	{
		auto j=fmt::format("_SERVER_USER_LIST_{0}", i);
		std::string tmpData = json["result"]["settings"][j];
		data += tmpData;
	}
	return std::move(data);
}

std::string Lock::DownloadEvent(const std::string & did, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken, const std::string & eventType)
{
	auto endTime=DataTimeHelp::GetTimestampSecond();
	long long  beginTime = 1038130849; //2000-01-01 00:00:00
	std::vector<std::string> jarray;
	jarray.push_back(eventType);
	return MiHomeHelp::GetUserDeviceData(did, "event", beginTime, endTime, jarray, ssecurity, cuserId, serviceToken);
}
