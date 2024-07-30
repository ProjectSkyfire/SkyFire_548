/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ARC4.h"
#include "Errors.h"
#include <openssl/provider.h>

SkyFire::Crypto::ARC4::ARC4()
    : _ctx(EVP_CIPHER_CTX_new())
{
    OSSL_PROVIDER_load(NULL, "legacy");
    EVP_CIPHER_CTX_init(_ctx);
    int result = EVP_EncryptInit_ex(_ctx, EVP_rc4(), nullptr, nullptr, nullptr);
    ASSERT(result == 1);
}

SkyFire::Crypto::ARC4::~ARC4()
{
    EVP_CIPHER_CTX_free(_ctx);
}

void SkyFire::Crypto::ARC4::Init(uint8 const* seed, size_t len)
{
    int result1 = EVP_CIPHER_CTX_set_key_length(_ctx, len);
    ASSERT(result1 == 1);
    int result2 = EVP_EncryptInit_ex(_ctx, nullptr, nullptr, seed, nullptr);
    ASSERT(result2 == 1);
}

void SkyFire::Crypto::ARC4::UpdateData(uint8* data, size_t len)
{
    int outlen = 0;
    int result1 = EVP_EncryptUpdate(_ctx, data, &outlen, data, len);
    ASSERT(result1 == 1);
    int result2 = EVP_EncryptFinal_ex(_ctx, data, &outlen);
    ASSERT(result2 == 1);
}
