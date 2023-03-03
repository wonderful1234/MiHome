#pragma once
#include <vector>
class ARC4
{
public:
	ARC4()=default;
	ARC4(unsigned char * key,int keyLen);
	~ARC4()=default;
	std::vector<unsigned char> Crypt(unsigned char * data, int dataLen);

private:
	unsigned char m_S[256] = {};
	int m_X = 0;
	int m_Y = 0;
	void Init(unsigned char * key,int keyLen);
};
