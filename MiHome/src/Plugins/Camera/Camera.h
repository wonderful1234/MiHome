#pragma once
#include "ExtractorPluginBase.h"
class Camera :public ExtractorPluginBase<Camera>
{
public:
	Camera()=default;
	~Camera()=default;
	void DownLoadData(const std::string & userName, const std::string &  passWord, const std::string & parms);

private:

};

