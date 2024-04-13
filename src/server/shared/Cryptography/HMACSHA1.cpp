/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "BigNumber.h"
#include "Common.h"
#include "HMACSHA1.h"

HmacHash::HmacHash(uint32 len, uint8* seed) : m_mac(EVP_MAC_fetch(NULL, "HMAC", NULL)), m_ctx(EVP_MAC_CTX_new(m_mac))
{
    m_params[0] = OSSL_PARAM_construct_utf8_string("digest", const_cast<char*>("SHA1"), 0);
    m_params[1] = OSSL_PARAM_construct_end();
    EVP_MAC_init(m_ctx, seed, len, m_params);
}

HmacHash::~HmacHash()
{
    EVP_MAC_CTX_free(m_ctx);
    EVP_MAC_free(m_mac);
}

void HmacHash::UpdateData(const std::string& str)
{
    EVP_MAC_update(m_ctx, (uint8 const*)str.c_str(), str.length());
}

void HmacHash::UpdateData(const uint8* data, size_t len)
{
    EVP_MAC_update(m_ctx, data, len);
}

void HmacHash::Finalize()
{
    size_t length = 0;
    EVP_MAC_final(m_ctx, (uint8*)m_digest, &length, sizeof(m_digest));
    ASSERT(length == SHA_DIGEST_LENGTH);
}

uint8* HmacHash::ComputeHash(BigNumber* bn)
{
    EVP_MAC_update(m_ctx, bn->AsByteArray(), bn->GetNumBytes());
    Finalize();
    return (uint8*)m_digest;
}
