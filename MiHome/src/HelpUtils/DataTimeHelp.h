#pragma once
#include <ctime>
#include <map>
class DataTimeHelp
{
public:
	DataTimeHelp()=default;
	~DataTimeHelp()=default;
	static long long GetTimestampMill();
	static long long  GetTimestampSecond();
	static long long  GetTimeStamp(tm & time,bool ten=false);
	static std::map<std::string, int> GetCurrentTimeInfo();

private:

};