/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef _AUTH_HMAC_H
#define _AUTH_HMAC_H

#include "Define.h"
#include <string>
#include <openssl/hmac.h>
#include <openssl/sha.h>

class BigNumber;

#define SEED_KEY_SIZE 16

class HmacHash
{
    public:
        HmacHash(uint32 len, uint8 *seed);
        ~HmacHash();
        void UpdateData(const std::string &str);
        void UpdateData(const uint8* data, size_t len);
        void Finalize();
        uint8 *ComputeHash(BigNumber* bn);
        uint8 *GetDigest() { return (uint8*)m_digest; }
        int GetLength() const { return SHA_DIGEST_LENGTH; }
    private:
        EVP_MAC* m_mac;
        EVP_MAC_CTX* m_ctx;
        uint8 m_digest[SHA_DIGEST_LENGTH];
        OSSL_PARAM m_params[2];
};
#endif

