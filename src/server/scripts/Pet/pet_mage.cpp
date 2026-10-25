/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "npc_pet_mag_".
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "CombatAI.h"
#include "Pet.h"
#include "TemporarySummon.h"

enum MageSpells
{
    SPELL_MIRROR_IMAGE_CLONE_ME            = 45204,
    SPELL_MIRROR_IMAGE_COPY_WEAPONS        = 41054,
    SPELL_MIRROR_IMAGE_MASTERS_THREAT_LIST = 58838,

    SPELL_FROZEN_ORB_VISUAL                = 123605,
    SPELL_FROZEN_ORB_PERIODIC              = 84717,
    SPELL_FROZEN_ORB_DAMAGE_TRIGGER        = 84718,
    SPELL_FROZEN_ORB_SELF_SNARE            = 82736,
    SPELL_FINGERS_OF_FROST                 = 44544,

    SPELL_ENCHANTED_REFLECTION_CLONE       = 102284,
    SPELL_ENCHANTED_REFLECTION_COPY        = 102288,
    SPELL_COSMETIC_ALPHA_STATE_25          = 82978,
    SPELL_TRACK_MASTER_AURAS               = 110951,
};

class npc_pet_mage_mirror_image : public CreatureScript
{
    public:
        npc_pet_mage_mirror_image() : CreatureScript("npc_pet_mage_mirror_image") { }

        struct npc_pet_mage_mirror_imageAI : CasterAI
        {
            npc_pet_mage_mirror_imageAI(Creature* creature) : CasterAI(creature) { }

            uint32 spellId = 0;

            void InitializeAI() OVERRIDE
            {
                CasterAI::InitializeAI();
                Unit* owner = me->GetOwner();
                if (!owner)
                    return;


                switch (me->GetEntry())
                {
                case 31216: spellId = 59638;  break; // Frost bolt
                case 47243: spellId = 88084;  break; // Arcane Blast
                case 47244: spellId = 88082;  break; // Fireball
                }

                // Inherit Master's Threat List (not yet implemented)
                me->CastSpell(me, SPELL_MIRROR_IMAGE_MASTERS_THREAT_LIST, true);
                // here mirror image casts on summoner spell (not present in client dbc) 49866
                // here should be auras (not present in client dbc): 35657, 35658, 35659, 35660 selfcasted by mirror images (stats related?)
                // Clone Me!
                owner->CastSpell(me, SPELL_MIRROR_IMAGE_CLONE_ME, true);
                owner->CastSpell(me, SPELL_MIRROR_IMAGE_COPY_WEAPONS, true);
            }

            // Do not reload Creature templates on evade mode enter - prevent visual lost
            void EnterEvadeMode() OVERRIDE
            {
                if (me->IsInEvadeMode() || !me->IsAlive())
                    return;

                Unit* owner = me->GetCharmerOrOwner();

                me->CombatStop(true);
                if (owner && !me->HasUnitState(UNIT_STATE_FOLLOW))
                {
                    me->GetMotionMaster()->Clear(false);
                    me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle(), MOTION_SLOT_ACTIVE);
                }
            }

            void UpdateAI(uint32 diff) override
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (me->GetVictim()->GetSchoolImmunityMask() == SPELL_SCHOOL_MASK_ALL || me->GetVictim()->HasBreakableByDamageCrowdControlAura(me))
                {
                    me->InterruptNonMeleeSpells(false);
                    EnterEvadeMode();
                    return;
                }

                DoCastVictim(spellId);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_pet_mage_mirror_imageAI(creature);
        }
};

// 45322 - Frozen Orb
class npc_pet_mage_frozen_orb : public CreatureScript
{
    public:
        npc_pet_mage_frozen_orb() : CreatureScript("npc_pet_mage_frozen_orb") { }

        struct npc_pet_mage_frozen_orbAI : public ScriptedAI
        {
            npc_pet_mage_frozen_orbAI(Creature* creature) : ScriptedAI(creature), _damageTimer(0), _hitTarget(false) { }

            void Reset() OVERRIDE
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
            }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                me->CastSpell(me, SPELL_FROZEN_ORB_VISUAL, true);
                me->CastSpell(me, SPELL_FROZEN_ORB_PERIODIC, true);
                me->SetSpeed(MOVE_RUN, 1.4285f, true);
                me->SetSpeed(MOVE_WALK, 1.4285f, true);

                Position pos;
                summoner->GetPosition(&pos);
                summoner->MovePositionToFirstCollision(pos, 110.0f, 0.0f);
                me->GetMotionMaster()->MovePoint(0, pos);
            }

            void SpellHitTarget(Unit* /*target*/, SpellInfo const* spell) OVERRIDE
            {
                if (_hitTarget || spell->Id != SPELL_FROZEN_ORB_DAMAGE_TRIGGER)
                    return;

                _hitTarget = true;
                me->CastSpell(me, SPELL_FROZEN_ORB_SELF_SNARE, true);
                if (Unit* mage = GetMage())
                    mage->CastSpell(mage, SPELL_FINGERS_OF_FROST, true);

                // After the first hit the orb pulses once per second instead of following the periodic aura
                me->RemoveAurasDueToSpell(SPELL_FROZEN_ORB_PERIODIC);
                _damageInterval = uint32(std::max(spell->Effects[EFFECT_0].BasePoints, 1));
                _damageTimer = _damageInterval;
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                damage = 0;
            }

            void EnterEvadeMode() OVERRIDE { }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!_hitTarget)
                    return;

                if (_damageTimer <= diff)
                {
                    me->CastSpell(me, SPELL_FROZEN_ORB_DAMAGE_TRIGGER, true);
                    _damageTimer = _damageInterval;
                }
                else
                    _damageTimer -= diff;
            }

        private:
            Unit* GetMage() const
            {
                if (Unit* owner = me->GetOwner())
                    return owner;
                if (TempSummon* summon = me->ToTempSummon())
                    return summon->GetSummoner();
                return NULL;
            }

            uint32 _damageTimer;
            uint32 _damageInterval = 1000;
            bool _hitTarget;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_pet_mage_frozen_orbAI(creature);
        }
};

// 58542 - Past Self, summoned by Alter Time (108978)
class npc_pet_mage_past_self : public CreatureScript
{
    public:
        npc_pet_mage_past_self() : CreatureScript("npc_pet_mage_past_self") { }

        struct npc_pet_mage_past_selfAI : public ScriptedAI
        {
            npc_pet_mage_past_selfAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset() OVERRIDE
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
            }

            void IsSummonedBy(Unit* summoner) OVERRIDE
            {
                summoner->CastSpell(me, SPELL_ENCHANTED_REFLECTION_CLONE, true);
                summoner->CastSpell(me, SPELL_ENCHANTED_REFLECTION_COPY, true);
                me->CastSpell(me, SPELL_COSMETIC_ALPHA_STATE_25, true);
                me->CastSpell(me, SPELL_TRACK_MASTER_AURAS, true);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
            {
                damage = 0;
            }

            void EnterEvadeMode() OVERRIDE { }

            void UpdateAI(uint32 /*diff*/) OVERRIDE { }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_pet_mage_past_selfAI(creature);
        }
};

void AddSC_mage_pet_scripts()
{
    new npc_pet_mage_mirror_image();
    new npc_pet_mage_frozen_orb();
    new npc_pet_mage_past_self();
}
