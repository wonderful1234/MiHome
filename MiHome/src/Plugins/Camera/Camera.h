#pragma once
#include "ExtractorPluginBase.h"
#include <tuple>
#include <vector>
#include <map>
class Camera :public ExtractorPluginBase<Camera>
{
public:
	Camera()=default;
	~Camera()=default;
	void DownLoadData(const std::string & userName, const std::string &  passWord, const std::string & parms);

private:
	std::tuple<std::string, std::string > GetCamera(const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken, const std::string & mac);
	std::vector<std::map<std::string,std::string>> GetVideoList(const std::string & did, const std::string & model, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken);
	std::string  GetVideoUrl(const std::string & did, const std::string & model, const std::string & fileId, const std::string & ssecurity, const std::string & cuserId, const std::string & serviceToken);
	std::vector<unsigned char> HttpGetByte(const std::string & url);
	void DownloadVideo(const std::string & time, const std::string & keyUrl, const std::string & iv, const std::vector<std::string> & urls, const std::string & path);

};

