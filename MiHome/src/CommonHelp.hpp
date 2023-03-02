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
