/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "TOTP.h"
#include <cstring>
#include <openssl/evp.h>
#include <openssl/hmac.h>

constexpr std::size_t SkyFire::Crypto::TOTP::RECOMMENDED_SECRET_LENGTH;
static constexpr uint32 TOTP_INTERVAL = 30;
static constexpr uint32 HMAC_RESULT_SIZE = 20;

/*static*/ uint32 SkyFire::Crypto::TOTP::GenerateToken(Secret const& secret, time_t timestamp)
{
    timestamp /= TOTP_INTERVAL;
    unsigned char challenge[8];

    for (int i = 8; i--; timestamp >>= 8)
        challenge[i] = timestamp;

    unsigned char digest[HMAC_RESULT_SIZE];
    uint32 digestSize = HMAC_RESULT_SIZE;
    HMAC(EVP_sha1(), secret.data(), secret.size(), challenge, 8, digest, &digestSize);

    uint32 offset = digest[19] & 0xF;
    uint32 truncated = (digest[offset] << 24) | (digest[offset + 1] << 16) | (digest[offset + 2] << 8) | (digest[offset + 3]);
    truncated &= 0x7FFFFFFF;

    return (truncated % 1000000);
}

/*static*/ bool SkyFire::Crypto::TOTP::ValidateToken(Secret const& secret, uint32 token)
{
    time_t now = time(nullptr);
    return (
        (token == GenerateToken(secret, now - TOTP_INTERVAL)) ||
        (token == GenerateToken(secret, now)) ||
        (token == GenerateToken(secret, now + TOTP_INTERVAL))
    );
}
