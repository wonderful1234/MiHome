#include "EncryptionHelp.h"
#include "openssl/evp.h"
#include "Encoded.h"

std::string EncryptionHelp::AES128CBCEncrypt(const std::string & key, const std::string & str, const std::string & iv)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	int len = 0;
	int ciphertextLen = 0;
	int plaintextLen = str.length();
	unsigned char cipherText[128] = {};
	EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, (const unsigned char *)key.c_str(), (const unsigned char *)iv.c_str());
	EVP_EncryptUpdate(ctx, cipherText, &len, (const unsigned char *)str.c_str(), plaintextLen);
	ciphertextLen = len;
	EVP_EncryptFinal_ex(ctx, cipherText + len, &len);
	ciphertextLen += len;
	EVP_CIPHER_CTX_free(ctx);
	std::string res = std::string((char*)ciphertextLen);
	res.resize(ciphertextLen);
	return Encoded::EncodedBase64(res);

}

std::string EncryptionHelp::AES128CBCDecrypt(const std::string & key, const std::string & str, const std::string & iv)
{
	auto cipherText=Encoded::DecodedBase64(str);
	int len = 0;
	int plaintextLen = 0;
	int ciphertextLen = cipherText.length();
	unsigned char plainText[128] = {};
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit_ex(ctx,EVP_aes_128_cbc(), nullptr, (const unsigned char *)key.c_str(), (const unsigned char *)iv.c_str());
	EVP_DecryptUpdate(ctx, plainText, &len, (const unsigned char *)cipherText.c_str(), ciphertextLen);
	plaintextLen = len;
	EVP_DecryptFinal_ex(ctx, plainText + len, &len);
	plaintextLen += len;
	EVP_CIPHER_CTX_free(ctx);
	std::string res = std::string((char*)plainText);
	res.resize(plaintextLen);
	return std::move(res);
}
