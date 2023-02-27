#include "MiHomeHelp.h"
#include <random>
#include "Encoded.h"
std::string MiHomeHelp::GenerateNonce(long millis)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);
	unsigned char nonceBytes[12] = {};
	for (int i = 0; i < 8; i++) {
		nonceBytes[i] = dis(gen);
	}
	int time=static_cast<int>(millis / 60000);
	std::memcpy(&nonceBytes[8], &time, 4);
	std::string str = std::string((char*)nonceBytes);
	return Encoded::EncodedBase64(str);
}
