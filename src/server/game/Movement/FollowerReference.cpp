/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "FollowerReference.h"
#include "TargetedMovementGenerator.h"
#include "Unit.h"

void FollowerReference::targetObjectBuildLink()
{
    getTarget()->addFollower(this);
}

void FollowerReference::targetObjectDestroyLink()
{
    getTarget()->removeFollower(this);
}

void FollowerReference::sourceObjectDestroyLink()
{
    GetSource()->stopFollowing();
}
