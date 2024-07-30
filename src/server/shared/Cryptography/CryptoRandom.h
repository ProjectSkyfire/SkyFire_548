/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _CRYPTORANDOM_H
#define _CRYPTORANDOM_H

#include "Define.h"
#include <array>

namespace SkyFire::Crypto
{
    void GetRandomBytes(uint8* buf, size_t len);

    template <typename Container>
    void GetRandomBytes(Container& c)
    {
        GetRandomBytes(std::data(c), std::size(c));
    }

    template <size_t S>
    std::array<uint8, S> GetRandomBytes()
    {
        std::array<uint8, S> arr;
        GetRandomBytes(arr);
        return arr;
    }
}

#endif
