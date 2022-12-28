/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/// \addtogroup Trinityd
/// @{
/// \file

#ifndef __WORLDRUNNABLE_H
#define __WORLDRUNNABLE_H

/// Heartbeat thread for the World
class WorldRunnable : public ACE_Based::Runnable
{
    public:
        void run() override;
};

#endif

/// @}
