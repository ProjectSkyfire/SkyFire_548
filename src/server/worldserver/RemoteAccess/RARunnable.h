/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/// \addtogroup Trinityd
/// @{
/// \file

#ifndef _SKYFIRE_RARUNNABLE_H_
#define _SKYFIRE_RARUNNABLE_H_

#include "Common.h"

#include <ace/Reactor.h>

class RARunnable : public ACE_Based::Runnable
{
public:
    RARunnable();
    virtual ~RARunnable();
    void run() OVERRIDE;

private:
    ACE_Reactor* m_Reactor;
};

#endif /* _SKYFIRE_RARUNNABLE_H_ */

/// @}
