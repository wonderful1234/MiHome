#include "DataTimeHelp.h"
#include <chrono>
long long DataTimeHelp::GetTimestampMill()
{
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto timeStamp= std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	return timeStamp;
}

long long DataTimeHelp::GetTimestampSecond()
{
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto timeStamp= std::chrono::duration_cast<std::chrono::seconds>(duration).count();
	return timeStamp;

}

long long DataTimeHelp::GetTimeStamp(tm & time, bool ten)
{
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(std::mktime(&time));
	long long timestamp = 0;
	if(ten)
		timestamp = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
	else
		timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
	return timestamp;
}

std::map<std::string, int> DataTimeHelp::GetCurrentTimeInfo()
{
	std::map<std::string, int> timeInfo;
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	struct std::tm* parts = std::localtime(&now_c);
	timeInfo["year"] = 1900 + parts->tm_year;
	timeInfo["mon"] = 1 + parts->tm_mon;
	timeInfo["day"] = 1 + parts->tm_mday;
	timeInfo["hour"] = 1 + parts->tm_hour;
	timeInfo["min"] = 1 + parts->tm_min;
	timeInfo["sec"] = 1 + parts->tm_sec;
	return std::move(timeInfo);
}
