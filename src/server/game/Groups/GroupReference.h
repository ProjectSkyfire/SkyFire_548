/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_GROUPREFERENCE_H
#define SF_GROUPREFERENCE_H

#include "LinkedReference/Reference.h"

class Group;
class Player;

class GroupReference : public Reference<Group, Player>
{
    protected:
        uint8 iSubGroup;
        void targetObjectBuildLink();
        void targetObjectDestroyLink();
        void sourceObjectDestroyLink();
    public:
        GroupReference() : Reference<Group, Player>(), iSubGroup(0) { }
        ~GroupReference() { unlink(); }
        GroupReference* next() { return (GroupReference*)Reference<Group, Player>::next(); }
        GroupReference const* next() const { return (GroupReference const*)Reference<Group, Player>::next(); }
        uint8 getSubGroup() const { return iSubGroup; }
        void setSubGroup(uint8 pSubGroup) { iSubGroup = pSubGroup; }
};
#endif
