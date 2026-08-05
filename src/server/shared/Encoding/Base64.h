/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _BASE64_H
#define _BASE64_H

#include "Define.h"
#include "Optional.h"
#include <string>
#include <vector>

namespace SkyFire::Encoding
{
    struct Base64
    {
        static std::string Encode(std::vector<uint8> const& data);
        static Optional<std::vector<uint8>> Decode(std::string const& data);
    };
}

#endif
