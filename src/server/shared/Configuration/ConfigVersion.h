/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_CONFIG_VERSION_H
#define SKYFIRE_CONFIG_VERSION_H

#include "Define.h"

// Format is YYYYMMDDRR where RR is the revision made to the configuration file that day.
namespace Skyfire::ConfigVersion
{
    constexpr uint32 Auth = 2026090802;
    constexpr uint32 Hub = 2026091601;
    constexpr uint32 World = 2026272803;
}

#endif
