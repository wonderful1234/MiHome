#include "EncryptionHelp.h"
#include "openssl/evp.h"

std::vector<unsigned char>EncryptionHelp::AES128CBCEncrypt(const char * key, const char * str, const unsigned char * iv, int inLen)
{
	std::vector<unsigned char> out(inLen + 16);
	auto ff=EVP_CIPHER_block_size(EVP_aes_128_cbc()); 
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	int len = 0;
	int ciphertextLen = 0;
	EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, (const unsigned char *)key, iv);
	EVP_EncryptUpdate(ctx, out.data(), &len, (const unsigned char *)str, inLen);
	ciphertextLen = len;
	EVP_EncryptFinal_ex(ctx, &out[ciphertextLen], &len);
	ciphertextLen += len;
	EVP_CIPHER_CTX_free(ctx);
	out.resize(ciphertextLen);
	return out;

}

 std::vector<unsigned char> EncryptionHelp::AES128CBCDecrypt(const char * key, const char * str, const unsigned char * iv, int inLen)
 {
	std::vector<unsigned char> out(inLen +1);
	int len = 0;
	int plaintextLen = 0;
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	EVP_DecryptInit_ex(ctx,EVP_aes_128_cbc(), nullptr, (const unsigned char *)key, iv);
	EVP_DecryptUpdate(ctx, out.data(), &len, (const unsigned char *)str, inLen);
	plaintextLen = len;
	EVP_DecryptFinal_ex(ctx, &out[plaintextLen], &len);
	plaintextLen += len;
	EVP_CIPHER_CTX_free(ctx);
	out.resize(plaintextLen);
	return out;
}
