/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "ARC4.h"
#include <openssl/sha.h>

ARC4::ARC4() : m_ctx(EVP_CIPHER_CTX_new())
{
    EVP_CIPHER_CTX_init(m_ctx);
    EVP_EncryptInit_ex(m_ctx, EVP_rc4(), NULL, NULL, NULL);
}

ARC4::~ARC4()
{
    EVP_CIPHER_CTX_cleanup(m_ctx);
}

void ARC4::Init(uint8* seed, uint32 len)
{
    EVP_CIPHER_CTX_set_key_length(m_ctx, len);
    EVP_EncryptInit_ex(m_ctx, NULL, NULL, seed, NULL);
}

void ARC4::UpdateData(int len, uint8 *data)
{
    int outlen = 0;
    EVP_EncryptUpdate(m_ctx, data, &outlen, data, len);
    EVP_EncryptFinal_ex(m_ctx, data, &outlen);
}
