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

enum MageSpells
{
    SPELL_MIRROR_IMAGE_CLONE_ME            = 45204,
    SPELL_MIRROR_IMAGE_COPY_WEAPONS        = 41054,
    SPELL_MIRROR_IMAGE_MASTERS_THREAT_LIST = 58838,
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

void AddSC_mage_pet_scripts()
{
    new npc_pet_mage_mirror_image();
}
