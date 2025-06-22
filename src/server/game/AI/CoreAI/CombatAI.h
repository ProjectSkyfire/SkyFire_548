/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_COMBATAI_H
#define SKYFIRE_COMBATAI_H

#include "ConditionMgr.h"
#include "CreatureAI.h"
#include "CreatureAIImpl.h"

class Creature;

class AggressorAI : public CreatureAI
{
public:
    explicit AggressorAI(Creature* c) : CreatureAI(c) { }

    void UpdateAI(uint32) OVERRIDE;
    static int Permissible(const Creature*);
};

typedef std::vector<uint32> SpellVct;

class CombatAI : public CreatureAI
{
public:
    explicit CombatAI(Creature* c) : CreatureAI(c) { }

    void InitializeAI() OVERRIDE;
    void Reset() OVERRIDE;
    void EnterCombat(Unit* who) OVERRIDE;
    void JustDied(Unit* killer) OVERRIDE;
    void UpdateAI(uint32 diff) OVERRIDE;
    void SpellInterrupted(uint32 spellId, uint32 unTimeMs) OVERRIDE;
    static int Permissible(const Creature*);
protected:
    EventMap events;
    SpellVct spells;
};

class CasterAI : public CombatAI
{
public:
    explicit CasterAI(Creature* c) : CombatAI(c) { m_attackDist = MELEE_RANGE; }
    void InitializeAI() OVERRIDE;
    void AttackStart(Unit* victim) OVERRIDE { AttackStartCaster(victim, m_attackDist); }
    void UpdateAI(uint32 diff) OVERRIDE;
    void EnterCombat(Unit* /*who*/) OVERRIDE;
private:
    float m_attackDist;
};

struct ArcherAI : public CreatureAI
{
public:
    explicit ArcherAI(Creature* c);
    void AttackStart(Unit* who) OVERRIDE;
    void UpdateAI(uint32 diff) OVERRIDE;

    static int Permissible(const Creature*);
protected:
    float m_minRange;
};

struct TurretAI : public CreatureAI
{
public:
    explicit TurretAI(Creature* c);
    bool CanAIAttack(const Unit* who) const;
    void AttackStart(Unit* who) OVERRIDE;
    void UpdateAI(uint32 diff) OVERRIDE;

    static int Permissible(const Creature*);
protected:
    float m_minRange;
};

#define VEHICLE_CONDITION_CHECK_TIME 1000
#define VEHICLE_DISMISS_TIME 5000
struct VehicleAI : public CreatureAI
{
public:
    explicit VehicleAI(Creature* c);

    void UpdateAI(uint32 diff) OVERRIDE;
    static int Permissible(const Creature*);
    void Reset() OVERRIDE;
    void MoveInLineOfSight(Unit*) OVERRIDE { }
    void AttackStart(Unit*) OVERRIDE { }
    void OnCharmed(bool apply) OVERRIDE;

private:
    bool m_IsVehicleInUse;
    void LoadConditions();
    void CheckConditions(const uint32 diff);
    ConditionList conditions;
    uint32 m_ConditionsTimer;
    bool m_DoDismiss;
    uint32 m_DismissTimer;
};

#endif
