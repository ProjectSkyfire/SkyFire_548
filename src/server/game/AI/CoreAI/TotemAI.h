/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_TOTEMAI_H
#define SKYFIRE_TOTEMAI_H

#include "CreatureAI.h"
#include "Timer.h"

class Creature;
class Totem;

class TotemAI : public CreatureAI
{
    public:
        explicit TotemAI(Creature* c);

        void MoveInLineOfSight(Unit* who) override;
        void AttackStart(Unit* victim) override;
        void EnterEvadeMode() override;

        void UpdateAI(uint32 diff) override;
        static int Permissible(Creature const* creature);

    private:
        uint64 i_victimGuid;
};
#endif
