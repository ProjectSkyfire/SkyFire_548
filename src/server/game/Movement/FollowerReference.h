/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_FOLLOWERREFERENCE_H
#define SF_FOLLOWERREFERENCE_H

#include "Reference.h"

class TargetedMovementGeneratorBase;
class Unit;

class FollowerReference : public Reference<Unit, TargetedMovementGeneratorBase>
{
    protected:
        void targetObjectBuildLink();
        void targetObjectDestroyLink();
        void sourceObjectDestroyLink();
};
#endif
