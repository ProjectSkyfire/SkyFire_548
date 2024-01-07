/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ARC4.h"
#include <openssl/sha.h>
#include <openssl/provider.h>

ARC4::ARC4() : m_ctx(EVP_CIPHER_CTX_new())
{
#if defined(OPENSSL_VERSION_MAJOR) && (OPENSSL_VERSION_MAJOR >= 3) && (OPENSSL_VERSION_MINOR >= 3)
    OSSL_PROVIDER_load(NULL, "legacy");
#endif

#if defined(OPENSSL_VERSION_MAJOR) && (OPENSSL_VERSION_MAJOR >= 3) && (OPENSSL_VERSION_MINOR < 3)
#error "UNSUPORTED OPENSSL VERSION, SKYFIRE REQUIRES 3.3 or newer."
#endif

    EVP_CIPHER_CTX_reset(m_ctx);
    EVP_EncryptInit_ex(m_ctx, EVP_rc4(), NULL, NULL, NULL);
}

ARC4::~ARC4()
{
    EVP_CIPHER_CTX_free(m_ctx);
}

void ARC4::Init(uint8* seed, uint32 len)
{
    EVP_CIPHER_CTX_set_key_length(m_ctx, len);
    EVP_EncryptInit_ex(m_ctx, NULL, NULL, seed, NULL);
}

void ARC4::UpdateData(int len, uint8 *data)
{
    int outlen = 0;
    EVP_CipherUpdate(m_ctx, data, &outlen, data, len);
    Finalize(outlen, data);
}

void ARC4::Finalize(int outlen, uint8* data)
{
    EVP_CipherFinal_ex(m_ctx, data, &outlen);
}
