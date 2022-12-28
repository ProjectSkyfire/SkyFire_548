/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_MOVEMENTGENERATOR_IMPL_H
#define SKYFIRE_MOVEMENTGENERATOR_IMPL_H

#include "MovementGenerator.h"

template<class MOVEMENT_GEN>
inline MovementGenerator*
MovementGeneratorFactory<MOVEMENT_GEN>::Create(void * /*data*/) const
{
    return (new MOVEMENT_GEN());
}
#endif
