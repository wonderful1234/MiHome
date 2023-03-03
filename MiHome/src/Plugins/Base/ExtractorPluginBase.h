#pragma once
#include <string>
template <typename T>
class ExtractorPluginBase
{
public:
	ExtractorPluginBase()=default;
	virtual ~ExtractorPluginBase()=default;
	void  Excute(const std::string & userName, const std::string &  passWord, const std::string & parms)
	{
		if (auto child = static_cast<T *>(this))
		{
			child->DownLoadData(userName, passWord, parms);
		}

	}

private:

};

template<class T>
void Begin(ExtractorPluginBase<T> &child,const std::string & userName, const std::string &  passWord, const std::string & parms )
{
	child.Excute(userName, passWord, parms);
}

