/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/// \addtogroup Skyfired
/// @{
/// \file

#ifndef __WORLDRUNNABLE_H
#define __WORLDRUNNABLE_H

#include "Define.h"
#include <atomic>

// Published by the world thread; the hub control thread never reads session containers.
extern std::atomic<uint64> HubWorldMetrics;
extern std::atomic<uint64> HubWorldTick;

/// Heartbeat thread for the World
class WorldRunnable
{
public:
    void Run();
};

#endif

/// @}
