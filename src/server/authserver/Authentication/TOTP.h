/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_TOTP_H
#define SF_TOTP_H

#include "openssl/hmac.h"
#include "openssl/evp.h"

namespace TOTP
{
    unsigned int GenerateToken(const char* b32key);
}

#endif
