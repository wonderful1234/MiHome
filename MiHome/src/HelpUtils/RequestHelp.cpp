#include "RequestHelp.h"
#include "UrlHelp.h"
httplib::Result RequestHelp::HttpsGet(const std::string & strUrl,int port)
{
	return HttpsGet(strUrl, httplib::Headers(), port);
}

httplib::Result RequestHelp::HttpsGet(const std::string & strUrl, const std::multimap<std::string, std::string> & headers, int port)
{
	httplib::Headers hHeaders;
	for (auto item : headers)
	{
		hHeaders.emplace(item.first, item.second);
	}
	return HttpsGet(strUrl, hHeaders, port);
}

httplib::Result RequestHelp::HttpsPost(const std::string & strUrl, const std::string & params, int port)
{
	return HttpsPost(strUrl, params, httplib::Headers(), port);
}

httplib::Result RequestHelp::HttpsPost(const std::string & strUrl, const std::string & params, const std::multimap<std::string, std::string>& headers, int port)
{
	httplib::Headers hHeaders;
	for (auto & item : headers)
	{
		hHeaders.emplace(item.first, item.second);
	}
	return HttpsPost(strUrl, params, hHeaders, port);
}

httplib::Result RequestHelp::HttpsPost(const std::string & strUrl, std::multimap<std::string, std::string>& params, int port)
{
	return HttpsPost(strUrl, params, httplib::Headers(), port);
}

httplib::Result RequestHelp::HttpsPost(const std::string & strUrl, std::multimap<std::string, std::string>& params, const std::multimap<std::string, std::string>& headers, int port)
{
	httplib::Headers hHeaders;
	for (auto & item : headers)
	{
		hHeaders.emplace(item.first, item.second);
	}
	return HttpsPost(strUrl, params, hHeaders, port);
}

httplib::Result RequestHelp::HttpsGet(const std::string & strUrl, const httplib::Headers & headers, int port)
{
	std::string url = "";
	std::string path = "";
	auto tp=UrlHelp::GetUrlDomainAndPath(strUrl);
	std::tie(url, path) = tp;
	httplib::SSLClient cli(url, port);
	cli.set_ca_cert_path(CA_CERT_FILE);
	cli.enable_server_certificate_verification(false);
	auto res = cli.Get(path,headers);
	return res;
}

httplib::Result RequestHelp::HttpsPost(const std::string & strUrl, const std::string & params, const httplib::Headers & headers, int port)
{
	std::string url = "";
	std::string path = "";
	auto tp = UrlHelp::GetUrlDomainAndPath(strUrl);
	std::tie(url, path) = tp;
	httplib::SSLClient cli(url, port);
	cli.set_ca_cert_path(CA_CERT_FILE);
	cli.enable_server_certificate_verification(false);
	auto res = cli.Post(path, headers, params, "");
	return res;
}

httplib::Result RequestHelp::HttpsPost(const std::string & strUrl, const httplib::Params & params, const httplib::Headers & headers, int port)
{
	std::string url = "";
	std::string path = "";
	auto tp = UrlHelp::GetUrlDomainAndPath(strUrl);
	std::tie(url, path) = tp;
	httplib::SSLClient cli(url, port);
	cli.set_ca_cert_path(CA_CERT_FILE);
	cli.enable_server_certificate_verification(false);
	auto res = cli.Post(path, headers, params);
	return res;
}
