/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_PETAI_H
#define SKYFIRE_PETAI_H

#include "CreatureAI.h"
#include "Timer.h"

class Creature;
class Spell;

class PetAI : public CreatureAI
{
public:
    explicit PetAI(Creature* c);

    void UpdateAI(uint32) OVERRIDE;
    static int Permissible(const Creature*);

    void KilledUnit(Unit* /*victim*/) OVERRIDE;
    void AttackStart(Unit* target) OVERRIDE;
    void MovementInform(uint32 moveType, uint32 data) OVERRIDE;
    void OwnerAttackedBy(Unit* attacker) OVERRIDE;
    void OwnerAttacked(Unit* target) OVERRIDE;
    void AttackedBy(Unit* attacker) OVERRIDE;
    void ReceiveEmote(Player* player, uint32 textEmote) OVERRIDE;

    // The following aren't used by the PetAI but need to be defined to override
    //  default CreatureAI functions which interfere with the PetAI
    //
    void MoveInLineOfSight(Unit* /*who*/) OVERRIDE { } // CreatureAI interferes with returning pets
    void MoveInLineOfSight_Safe(Unit* /*who*/) { } // CreatureAI interferes with returning pets
    void EnterEvadeMode() OVERRIDE { } // For fleeing, pets don't use this type of Evade mechanic

private:
    //bool _isVisible(Unit*) const;
    bool _needToStop(void);
    void _stopAttack(void);

    void UpdateAllies();

    TimeTracker i_tracker;
    bool inCombat;
    std::set<uint64> m_AllySet;
    uint32 m_updateAlliesTimer;

    Unit* SelectNextTarget(bool allowAutoSelect) const;
    void HandleReturnMovement();
    void DoAttack(Unit* target, bool chase);
    bool CanAttack(Unit* target);
    void ClearCharmInfoFlags();
};
#endif
