/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_PASSIVEAI_H
#define SKYFIRE_PASSIVEAI_H

#include "CreatureAI.h"

class PassiveAI : public CreatureAI
{
    public:
        explicit PassiveAI(Creature* c);

        void MoveInLineOfSight(Unit*) override { }
        void AttackStart(Unit*) override { }
        void UpdateAI(uint32) override;

        static int Permissible(const Creature*) { return PERMIT_BASE_IDLE;  }
};

class PossessedAI : public CreatureAI
{
    public:
        explicit PossessedAI(Creature* c);

        void MoveInLineOfSight(Unit*) override { }
        void AttackStart(Unit* target) override;
        void UpdateAI(uint32) override;
        void EnterEvadeMode() override { }

        void JustDied(Unit*) override;
        void KilledUnit(Unit* victim) override;

        static int Permissible(const Creature*) { return PERMIT_BASE_IDLE;  }
};

class NullCreatureAI : public CreatureAI
{
    public:
        explicit NullCreatureAI(Creature* c);

        void MoveInLineOfSight(Unit*) override { }
        void AttackStart(Unit*) override { }
        void UpdateAI(uint32) override { }
        void EnterEvadeMode() override { }
        void OnCharmed(bool /*apply*/) override { }

        static int Permissible(const Creature*) { return PERMIT_BASE_IDLE;  }
};

class CritterAI : public PassiveAI
{
    public:
        explicit CritterAI(Creature* c) : PassiveAI(c) { }

        void DamageTaken(Unit* done_by, uint32& /*damage*/) override;
        void EnterEvadeMode() override;
};

class TriggerAI : public NullCreatureAI
{
    public:
        explicit TriggerAI(Creature* c) : NullCreatureAI(c) { }
        void IsSummonedBy(Unit* summoner) override;
};

#endif
