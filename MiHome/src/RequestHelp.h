#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
const std::string CA_CERT_FILE = "client.crt";
class RequestHelp
{
public:
	RequestHelp()= default;
	~RequestHelp()=default;
	static httplib::Result HttpsGet(const std::string & strUrl,int port=443);
	static httplib::Result HttpsGet(const std::string & strUrl, const std::multimap<std::string, std::string>& headers, int port = 443);
	static httplib::Result HttpsPost(const std::string & strUrl,const std::string & params, int port = 443);
	static httplib::Result HttpsPost(const std::string & strUrl, const std::string & params, const std::multimap<std::string, std::string>& headers, int port = 443);
	static httplib::Result HttpsPost(const std::string & strUrl, std::multimap<std::string, std::string> & params, int port = 443);
	static httplib::Result HttpsPost(const std::string & strUrl, std::multimap<std::string, std::string> & params, const std::multimap<std::string, std::string>& headers, int port = 443);
private:
	static httplib::Result HttpsGet(const std::string & strUrl,const httplib::Headers & headers, int port);
	static httplib::Result HttpsPost(const std::string & strUrl, const std::string & params, const httplib::Headers& headers, int port);
	static httplib::Result HttpsPost(const std::string & strUrl, const httplib::Params & params, const httplib::Headers& headers, int port);
};