/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "CryptoRandom.h"
#include "Errors.h"
#include <openssl/rand.h>

void SkyFire::Crypto::GetRandomBytes(uint8* buf, size_t len)
{
    int result = RAND_bytes(buf, len);
    ASSERT(result == 1);
}
