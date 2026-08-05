/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _AES_h__
#define _AES_h__

#include "Define.h"
#include <array>
#include <openssl/evp.h>

namespace SkyFire::Crypto
{
    class AES
    {
    public:
        static constexpr size_t IV_SIZE_BYTES = 12;
        static constexpr size_t KEY_SIZE_BYTES = 16;
        static constexpr size_t TAG_SIZE_BYTES = 12;

        using IV = std::array<uint8, IV_SIZE_BYTES>;
        using Key = std::array<uint8, KEY_SIZE_BYTES>;
        using Tag = uint8[TAG_SIZE_BYTES];

        AES(bool encrypting);
        ~AES();

        void Init(Key const& key);

        bool Process(IV const& iv, uint8* data, size_t length, Tag& tag);

    private:
        EVP_CIPHER_CTX* _ctx;
        bool _encrypting;
    };
}

#endif // _AES_h__
