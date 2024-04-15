/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ARC4.h"
#include <openssl/provider.h>
#include <openssl/sha.h>

ARC4::ARC4() : m_ctx(EVP_CIPHER_CTX_new())
{
    OSSL_PROVIDER_load(NULL, "legacy");
    EVP_CIPHER_CTX_reset(m_ctx);
    EVP_CipherInit(m_ctx, EVP_rc4(), NULL, NULL, 0);
}

ARC4::~ARC4()
{
    EVP_CIPHER_CTX_free(m_ctx);
}

void ARC4::Init(uint8* seed, size_t len)
{
#if defined(WIN32)
    m_params[0] = OSSL_PARAM_construct_size_t("keylen", &len);
    m_params[1] = OSSL_PARAM_construct_end();
    EVP_CipherInit_ex2(m_ctx, NULL, seed, NULL, 0, m_params);
#else
    EVP_CIPHER_CTX_set_key_length(m_ctx, len);
    EVP_CipherInit(m_ctx, NULL, seed, NULL, 0);
#endif
}

void ARC4::UpdateData(int len, uint8* data)
{
    int outlen = 0;
    EVP_CipherUpdate(m_ctx, data, &outlen, data, len);
    Finalize(outlen, data);
}

void ARC4::Finalize(int outlen, uint8* data)
{
    EVP_CipherFinal_ex(m_ctx, data, &outlen);
}
