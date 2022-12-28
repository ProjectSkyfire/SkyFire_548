/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_REACTORAI_H
#define SKYFIRE_REACTORAI_H

#include "CreatureAI.h"

class Unit;

class ReactorAI : public CreatureAI
{
    public:
        explicit ReactorAI(Creature* c) : CreatureAI(c) { }

        void MoveInLineOfSight(Unit*) override { }
        void UpdateAI(uint32 diff) override;

        static int Permissible(const Creature*);
};
#endif
