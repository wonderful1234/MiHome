#include "DataTimeHelp.h"
#include <chrono>
long DataTimeHelp::GetTimestampMill()
{
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto timeStamp= std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	return static_cast<long>(timeStamp);
}

long DataTimeHelp::GetTimestampSecond()
{
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto timeStamp= std::chrono::duration_cast<std::chrono::seconds>(duration).count();
	return static_cast<long>(timeStamp);
}
