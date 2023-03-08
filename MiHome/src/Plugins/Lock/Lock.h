#pragma once
#include "ExtractorPluginBase.h"
#include <tuple>
class Lock : public ExtractorPluginBase<Lock>
{
public:
	Lock()=default;
	~Lock()=default;
	void DownLoadData(const std::string & userName, const std::string &  passWord, const std::string & parms);
private:
	std::tuple<std::string, std::string> GetLockDid(const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken);
	std::string DownloadSetting(const std::string & did, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken);
	std::string DownloadEvent(const std::string & did, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken, const std::string & eventType);
};

