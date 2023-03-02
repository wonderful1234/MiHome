#include "ARC4.h"

ARC4::ARC4(unsigned char * key,int keyLen)
{
	Init(key, keyLen);
}

void ARC4::Init(unsigned char * key, int keyLen)
{
	for (int i = 0; i < 256; i++)
	{
		m_S[i] = static_cast<unsigned char>(i);
	}
	int j = 0;
	for (int i = 0; i < 256; i++)
	{
		j = (j + m_S[i] + key[i % keyLen]) & 255;
		auto temp = m_S[i];
		m_S[i] = m_S[j];
		m_S[j] = temp;
	}
}

std::vector<unsigned char> ARC4::Crypt(unsigned char * data, int dataLen)
{
	std::vector<unsigned char> result(dataLen);
	for (int i = 0; i < dataLen; i++)
	{
		m_X = (m_X + 1) & 255;
		m_Y = (m_Y + m_S[m_X]) & 255;
		auto temp = m_S[m_X];
		m_S[m_X] = m_S[m_Y];
		m_S[m_Y] = temp;
		auto xorIndex = (m_S[m_X] + m_S[m_Y]) & 255;
		result[i] = static_cast<unsigned char>((data[i] ^ m_S[xorIndex]));
	}
	return std::move(result);
}
