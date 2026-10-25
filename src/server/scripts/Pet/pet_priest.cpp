/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "npc_pet_pri_".
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "PassiveAI.h"
#include "PetAI.h"
#include "Group.h"
#include "Player.h"
#include "SpellAuras.h"
#include "TemporarySummon.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include <set>

enum PriestSpells
{
    SPELL_PRIEST_LIGHTWELL_CHARGES          = 59907,
    SPELL_PRIEST_LIGHTSPRING_CHARGES        = 126150,
    SPELL_PRIEST_LIGHTSPRING_RENEW          = 126154,
    SPELL_PRIEST_LEAP_OF_FAITH_JUMP         = 73325,
    SPELL_PRIEST_PSYCHIC_TERROR             = 113792
};

// Shared with spell_pri_psychic_terror
enum PsyfiendData
{
    PSYFIEND_DATA_TERROR_TARGET             = 1,
    PSYFIEND_DATA_TERRIFIED                 = 2
};

static void ApplyLightwellImmunities(Creature* me)
{
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_HEAL, true);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_HEAL_PCT, true);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_JUMP_DEST, true);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_JUMP, true);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
    me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_PRIEST_LEAP_OF_FAITH_JUMP, true);
    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_PERIODIC_HEAL, true);
    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_OBS_MOD_HEALTH, true);
    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_SCHOOL_ABSORB, true);
    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MECHANIC_IMMUNITY, true);
}

class npc_pet_pri_lightspring : public CreatureScript
{
    public:
        npc_pet_pri_lightspring() : CreatureScript("npc_pet_pri_lightspring") { }

        struct npc_pet_pri_lightspringAI : public PassiveAI
        {
            npc_pet_pri_lightspringAI(Creature* creature) : PassiveAI(creature), _checkTimer(1000)
            {
                ApplyLightwellImmunities(me);
                DoCast(me, SPELL_PRIEST_LIGHTSPRING_CHARGES, true);
            }

            void EnterEvadeMode() OVERRIDE
            {
                if (!me->IsAlive())
                    return;

                me->DeleteThreatList();
                me->CombatStop(true);
                me->ResetPlayerDamageReq();
            }

            bool CanHeal(Unit* owner, Unit* target) const
            {
                return target->IsAlive() && target->HealthBelowPct(50) && !target->HasAura(SPELL_PRIEST_LIGHTSPRING_RENEW)
                    && me->IsWithinDistInMap(target, 40.0f) && me->IsWithinLOSInMap(target) && owner->IsValidAssistTarget(target);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (_checkTimer > diff)
                {
                    _checkTimer -= diff;
                    return;
                }
                _checkTimer = 1000;

                Unit* owner = me->GetOwner();
                if (!owner && me->IsSummon())
                    owner = me->ToTempSummon()->GetSummoner();
                if (!owner)
                    return;

                std::list<Unit*> targets;
                Player* player = owner->ToPlayer();
                if (Group* group = player ? player->GetGroup() : NULL)
                {
                    for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                        if (Player* member = itr->GetSource())
                            if (member->IsInMap(me) && CanHeal(owner, member))
                                targets.push_back(member);
                }
                else if (CanHeal(owner, owner))
                    targets.push_back(owner);

                for (std::list<Unit*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                {
                    owner->CastSpell(*itr, SPELL_PRIEST_LIGHTSPRING_RENEW, true);

                    Aura* charges = me->GetAura(SPELL_PRIEST_LIGHTSPRING_CHARGES);
                    if (!charges || charges->ModCharges(-1))
                    {
                        if (me->IsSummon())
                            me->ToTempSummon()->UnSummon();
                        return;
                    }
                }
            }

        private:
            uint32 _checkTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_pet_pri_lightspringAI(creature);
        }
};

class npc_pet_pri_lightwell : public CreatureScript
{
    public:
        npc_pet_pri_lightwell() : CreatureScript("npc_pet_pri_lightwell") { }

        struct npc_pet_pri_lightwellAI : public PassiveAI
        {
            npc_pet_pri_lightwellAI(Creature* creature) : PassiveAI(creature)
            {
                ApplyLightwellImmunities(me);
                DoCast(me, SPELL_PRIEST_LIGHTWELL_CHARGES, false);
            }

            void EnterEvadeMode() OVERRIDE
            {
                if (!me->IsAlive())
                    return;

                me->DeleteThreatList();
                me->CombatStop(true);
                me->ResetPlayerDamageReq();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_pet_pri_lightwellAI(creature);
        }
};

// 59190 - Psyfiend
class npc_pet_pri_psyfiend : public CreatureScript
{
    public:
        npc_pet_pri_psyfiend() : CreatureScript("npc_pet_pri_psyfiend") { }

        struct npc_pet_pri_psyfiendAI : public PassiveAI
        {
            npc_pet_pri_psyfiendAI(Creature* creature) : PassiveAI(creature), _checkTimer(500), _targetGuid(0) { }

            void EnterEvadeMode() OVERRIDE { }

            uint64 GetGUID(int32 id) const OVERRIDE
            {
                return id == PSYFIEND_DATA_TERROR_TARGET ? _targetGuid : 0;
            }

            void SetGUID(uint64 guid, int32 id) OVERRIDE
            {
                if (id == PSYFIEND_DATA_TERRIFIED)
                    _terrified.insert(guid);
            }

            bool IsInvalidTarget(Unit const* target) const
            {
                switch (target->GetCreatureType())
                {
                    case CREATURE_TYPE_CRITTER:
                    case CREATURE_TYPE_NON_COMBAT_PET:
                    case CREATURE_TYPE_WILD_PET:
                        return true;
                    default:
                        break;
                }

                if (_terrified.find(target->GetGUID()) != _terrified.end())
                    return true;

                if (target->HasBreakableByDamageCrowdControlAura())
                    return true;

                if (target->HasInvisibilityAura() || target->HasStealthAura())
                    return true;

                if (me->HasVisionObscured(target))
                    return true;

                return !me->IsWithinLOSInMap(target);
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (_checkTimer > diff)
                {
                    _checkTimer -= diff;
                    return;
                }
                _checkTimer = 500;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                std::list<Unit*> targets;
                Skyfire::AnyUnfriendlyNoTotemUnitInObjectRangeCheck check(me, me, 20.0f);
                Skyfire::UnitListSearcher<Skyfire::AnyUnfriendlyNoTotemUnitInObjectRangeCheck> searcher(me, targets, check);
                me->VisitNearbyObject(20.0f, searcher);

                Unit* owner = me->GetOwner();
                Unit* target = NULL;
                for (std::list<Unit*>::const_iterator itr = targets.begin(); itr != targets.end(); ++itr)
                {
                    if (IsInvalidTarget(*itr))
                        continue;

                    // Units attacking the priest are terrified first
                    if (owner && (*itr)->GetVictim() == owner)
                    {
                        target = *itr;
                        break;
                    }

                    if (!target || me->GetExactDist2d(*itr) < me->GetExactDist2d(target))
                        target = *itr;
                }

                if (!target)
                    return;

                _targetGuid = target->GetGUID();
                DoCast(me, SPELL_PRIEST_PSYCHIC_TERROR);
            }

        private:
            uint32 _checkTimer;
            uint64 _targetGuid;
            std::set<uint64> _terrified;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_pet_pri_psyfiendAI(creature);
        }
};

void AddSC_priest_pet_scripts()
{
    new npc_pet_pri_lightspring();
    new npc_pet_pri_lightwell();
    new npc_pet_pri_psyfiend();
}
