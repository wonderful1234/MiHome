#include "RequestHelp.h"

httplib::Result RequestHelp::HttpsGet(const std::string & strUrl,int port, const std::string & path)
{
	return HttpsGet(strUrl, httplib::Headers(), port,path);
}

httplib::Result RequestHelp::HttpsGet(const std::string & strUrl, const std::multimap<std::string, std::string> & headers, int port, const std::string & path)
{
	httplib::Headers hHeaders;
	for (auto item : headers)
	{
		hHeaders.emplace(item.first, item.second);
	}
	return HttpsGet(strUrl, hHeaders, port,path);
}

httplib::Result RequestHelp::HttpsPost(const std::string & strUrl, const std::string & params, int port, const std::string & path)
{
	return HttpsPost(strUrl, params, httplib::Headers(), port,path);
}

httplib::Result RequestHelp::HttpsPost(const std::string & strUrl, const std::string & params, const std::multimap<std::string, std::string>& headers, int port, const std::string & path)
{
	httplib::Headers hHeaders;
	for (auto & item : headers)
	{
		hHeaders.emplace(item.first, item.second);
	}
	return HttpsPost(strUrl, params, hHeaders, port,path);
}

httplib::Result RequestHelp::HttpsPost(const std::string & strUrl, std::multimap<std::string, std::string>& params, int port, const std::string & path)
{
	return HttpsPost(strUrl, params, httplib::Headers(), port,path);
}

httplib::Result RequestHelp::HttpsPost(const std::string & strUrl, std::multimap<std::string, std::string>& params, const std::multimap<std::string, std::string>& headers, int port, const std::string & path)
{
	httplib::Headers hHeaders;
	for (auto & item : headers)
	{
		hHeaders.emplace(item.first, item.second);
	}
	return HttpsPost(strUrl, params, hHeaders, port,path);
}

httplib::Result RequestHelp::HttpsGet(const std::string & strUrl, const httplib::Headers & headers, int port, const std::string & path)
{
	httplib::SSLClient cli(strUrl, port);
	cli.set_ca_cert_path(CA_CERT_FILE);
	cli.enable_server_certificate_verification(false);
	auto res = cli.Get(path,headers);
	return res;
}

httplib::Result RequestHelp::HttpsPost(const std::string & strUrl, const std::string & params, const httplib::Headers & headers, int port, const std::string & path)
{
	httplib::SSLClient cli(strUrl, port);
	cli.set_ca_cert_path(CA_CERT_FILE);
	cli.enable_server_certificate_verification(false);
	auto res = cli.Post(path, headers, params, "");
	return res;
}

httplib::Result RequestHelp::HttpsPost(const std::string & strUrl, const httplib::Params & params, const httplib::Headers & headers, int port, const std::string & path)
{
	httplib::SSLClient cli(strUrl, port);
	cli.set_ca_cert_path(CA_CERT_FILE);
	cli.enable_server_certificate_verification(false);
	auto res = cli.Post(path, headers, params);
	return res;
}
