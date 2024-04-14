/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_TOTP_H
#define SF_TOTP_H

#include "openssl/evp.h"
#include "openssl/hmac.h"
#include <string>

namespace TOTP
{
    unsigned int GenerateToken(std::string& b32key);
}

#endif
