#pragma once
#include <string>
#include <sstream>
template<typename OS>
void  ConcatPR(OS& o)
{
	return;
}
template<typename OS, typename T, typename... Args>
void ConcatPR(OS & o, T t, Args && ...args)
{
	o << t;
	ConcatPR(o, args...);
}
template<typename... Args>
std::string Concat(Args && ...args)
{
	std::ostringstream oss;
	ConcatPR(oss, args ...);
	return std::move(oss.str());
}

template<typename T>
T ParseString(const std::string & str,int type=10)
{
	T i;
	std::stringstream ss(str);
	ss >> std::setbase(type) >> i;
	return i;
}
