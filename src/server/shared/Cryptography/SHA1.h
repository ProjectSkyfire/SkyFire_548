/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef _AUTH_SHA1_H
#define _AUTH_SHA1_H

#include "Define.h"
#include <string>
#include <openssl/sha.h>
#include <openssl/evp.h>

class BigNumber;

class SHA1Hash
{
    public:
        SHA1Hash();
        ~SHA1Hash();

        void UpdateBigNumbers(BigNumber* bn0, ...);

        void UpdateData(const uint8 *dta, int len);
        void UpdateData(const std::string &str);

        void Initialize();
        void Finalize();

        uint8 *GetDigest(void) { return m_digest; };
        int32 GetLength(void) const { return SHA_DIGEST_LENGTH; };

    private:
        EVP_MD_CTX* m_ctx;
        uint8 m_digest[SHA_DIGEST_LENGTH];
};
#endif

