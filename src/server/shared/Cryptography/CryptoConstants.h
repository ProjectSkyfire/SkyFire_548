/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _CRYPTO_CONSTANTS_H
#define _CRYPTO_CONSTANTS_H

#include "Define.h"

namespace SkyFire::Crypto
{
    struct Constants
    {
        static constexpr size_t SHA1_DIGEST_LENGTH_BYTES   = 20;
        static constexpr size_t SHA256_DIGEST_LENGTH_BYTES = 32;
    };
}

#endif
