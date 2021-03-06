#include "mbedtls/aes.h"
#include "mbedtls/gcm.h"
#include "../../Exceptions/Exception.h"

int aesBlocksize = 16;

void decrypt_aes_cbc_192(const unsigned char *input, unsigned char *output, int length, const unsigned char *key, unsigned char *initVector)
{
	mbedtls_aes_context context;

	mbedtls_aes_setkey_dec(&context, key, 192);
	if (mbedtls_aes_crypt_cbc(&context, MBEDTLS_AES_DECRYPT, length, initVector, input, output) != 0)
	{
		throw Exception("Could not decrypt block");
	}
	mbedtls_aes_free(&context);
}

void decrypt_aes_cbc_256(const unsigned char *input, unsigned char *output, int length, const unsigned char *key, unsigned char *initVector)
{
	mbedtls_aes_context context;

	mbedtls_aes_setkey_dec(&context, key, 256);
	int result = mbedtls_aes_crypt_cbc(&context, MBEDTLS_AES_DECRYPT, length, initVector, input, output);
	if (result != 0)
	{
		throw Exception("Could not decrypt block");
	}
	mbedtls_aes_free(&context);
}

void decrypt_aes_gcm(const unsigned char *input, unsigned char *output, int length, const unsigned char *key, unsigned char *initVector)
{
	mbedtls_gcm_context context;

	mbedtls_gcm_init(&context);
	if (mbedtls_gcm_setkey(&context, MBEDTLS_CIPHER_ID_AES, key, 256) != 0) {
		throw Exception("Could not decrypt (mbedtls_gcm_setkey)");
	}

	if (mbedtls_gcm_starts(&context, MBEDTLS_GCM_DECRYPT, initVector, 16, NULL, 0) != 0) {
		throw Exception("Could not decrypt (mbedtls_gcm_starts)");
	}
	if (mbedtls_gcm_update(&context, length, input, output) != 0) {
		throw Exception("Could not decrypt (mbedtls_gcm_update)");
	}
	mbedtls_gcm_finish(&context, NULL, 0);
	mbedtls_gcm_free(&context);
}
