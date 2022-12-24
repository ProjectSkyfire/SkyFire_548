/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_GROUPREFMANAGER_H
#define SF_GROUPREFMANAGER_H

#include "RefManager.h"

class Group;
class Player;
class GroupReference;

class GroupRefManager : public RefManager<Group, Player>
{
    public:
        GroupReference* getFirst() { return ((GroupReference*)RefManager<Group, Player>::getFirst()); }
        GroupReference const* getFirst() const { return ((GroupReference const*)RefManager<Group, Player>::getFirst()); }
};
#endif
