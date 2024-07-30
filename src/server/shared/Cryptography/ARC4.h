/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _AUTH_SARC4_H
#define _AUTH_SARC4_H

#include "Define.h"

#include <array>
#include <openssl/evp.h>

namespace SkyFire::Crypto
{
    class ARC4
    {
        public:
            ARC4();
            ~ARC4();

            void Init(uint8 const* seed, size_t len);
            template <typename Container>
            void Init(Container const& c) { Init(std::data(c), std::size(c)); }

            void UpdateData(uint8* data, size_t len);
            template <typename Container>
            void UpdateData(Container& c) { UpdateData(std::data(c), std::size(c)); }
        private:
            EVP_CIPHER_CTX* _ctx;
    };
}

#endif

