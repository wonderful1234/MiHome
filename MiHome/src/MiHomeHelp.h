#pragma once
#include <string>
class MiHomeHelp
{
public:
	MiHomeHelp()=default;
	~MiHomeHelp()=default;
	static std::string GenerateNonce(long millis);

private:

};

