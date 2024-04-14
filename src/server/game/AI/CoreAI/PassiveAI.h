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

    void MoveInLineOfSight(Unit*) OVERRIDE { }
    void AttackStart(Unit*) OVERRIDE { }
    void UpdateAI(uint32) OVERRIDE;

    static int Permissible(const Creature*) { return PERMIT_BASE_IDLE; }
};

class PossessedAI : public CreatureAI
{
public:
    explicit PossessedAI(Creature* c);

    void MoveInLineOfSight(Unit*) OVERRIDE { }
    void AttackStart(Unit* target) OVERRIDE;
    void UpdateAI(uint32) OVERRIDE;
    void EnterEvadeMode() OVERRIDE { }

    void JustDied(Unit*) OVERRIDE;
    void KilledUnit(Unit* victim) OVERRIDE;

    static int Permissible(const Creature*) { return PERMIT_BASE_IDLE; }
};

class NullCreatureAI : public CreatureAI
{
public:
    explicit NullCreatureAI(Creature* c);

    void MoveInLineOfSight(Unit*) OVERRIDE { }
    void AttackStart(Unit*) OVERRIDE { }
    void UpdateAI(uint32) OVERRIDE { }
    void EnterEvadeMode() OVERRIDE { }
    void OnCharmed(bool /*apply*/) OVERRIDE { }

    static int Permissible(const Creature*) { return PERMIT_BASE_IDLE; }
};

class CritterAI : public PassiveAI
{
public:
    explicit CritterAI(Creature* c) : PassiveAI(c) { }

    void DamageTaken(Unit* done_by, uint32& /*damage*/) OVERRIDE;
    void EnterEvadeMode() OVERRIDE;
};

class TriggerAI : public NullCreatureAI
{
public:
    explicit TriggerAI(Creature* c) : NullCreatureAI(c) { }
    void IsSummonedBy(Unit* summoner) OVERRIDE;
};

#endif
