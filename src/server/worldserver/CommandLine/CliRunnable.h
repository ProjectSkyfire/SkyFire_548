/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/// \addtogroup Trinityd
/// @{
/// \file

#ifndef __CLIRUNNABLE_H
#define __CLIRUNNABLE_H

/// Command Line Interface handling thread
class CliRunnable : public ACE_Based::Runnable
{
    public:
        void run() override;
};

#endif

/// @}
