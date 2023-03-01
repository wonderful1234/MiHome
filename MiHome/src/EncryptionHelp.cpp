#include "EncryptionHelp.h"
#include "openssl/evp.h"
#include "Encoded.h"

void  EncryptionHelp::AES128CBCEncrypt(const char * key, const char * str, const char * iv, char * out)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	int len = 0;
	int ciphertextLen = 0;
	int plaintextLen = strlen(str);
	EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, (const unsigned char *)key, (const unsigned char *)iv);
	EVP_EncryptUpdate(ctx, (unsigned char *)out, &len, (const unsigned char *)str, plaintextLen);
	ciphertextLen = len;
	EVP_EncryptFinal_ex(ctx, (unsigned char *)out + len, &len);
	ciphertextLen += len;
	EVP_CIPHER_CTX_free(ctx);

}

void EncryptionHelp::AES128CBCDecrypt(const char * key, const char * str, const char * iv, char * out)
{
	int len = 0;
	int plaintextLen = 0;
	int ciphertextLen =strlen(str);
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit_ex(ctx,EVP_aes_128_cbc(), nullptr, (const unsigned char *)key, (const unsigned char *)iv);
	EVP_DecryptUpdate(ctx, (unsigned char *)out, &len, (const unsigned char *)str, ciphertextLen);
	plaintextLen = len;
	EVP_DecryptFinal_ex(ctx, (unsigned char *)out + len, &len);
	plaintextLen += len;
	EVP_CIPHER_CTX_free(ctx);
}

void EncryptionHelp::Rc4Encrypt(const char * key, const char * str, int inLen)
{
	int len = 0;
	unsigned char tmpIn[1024] = {};
	unsigned char tmpOut[1024] = {};
	std::vector<unsigned char> out(inLen+ EVP_MAX_BLOCK_LENGTH);
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(ctx);
	EVP_EncryptInit_ex(ctx, EVP_rc4(), nullptr, (const unsigned char *)key, nullptr);
	/*EVP_EncryptUpdate(ctx, tmpOut, &len, tmpIn, 1024);
	EVP_DecryptFinal_ex(ctx, tmpOut + len, &len);
	auto  data = out.data();*/
	auto  data = out.data();
	EVP_EncryptUpdate(ctx, data, &len, (const unsigned char *)str, inLen);
	EVP_EncryptFinal_ex(ctx, out.data() + len, &len);
	EVP_CIPHER_CTX_free(ctx);
}

void EncryptionHelp::Rc4Decrypt(const char * key, const char * str, char * out)
{
}
