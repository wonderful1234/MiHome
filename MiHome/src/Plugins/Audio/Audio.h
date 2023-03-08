#pragma once
#include "ExtractorPluginBase.h"
#include <map>
class Audio :public ExtractorPluginBase<Audio>
{
public:
	Audio()=default;
	~Audio()=default;
	void DownLoadData(const std::string & userName, const std::string &  passWord, const std::string & parms);
private:
	std::map<std::string, std::string> Login(const std::string & userName, const std::string &  passWord);
	std::string GetdeviceId(const std::string & sn, const std::string & cuserId, const std::string & serviceToken, const std::string & userId);
	std::string GetRequestId();
};

