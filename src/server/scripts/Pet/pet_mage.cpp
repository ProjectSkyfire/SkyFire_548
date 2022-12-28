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
    SPELL_MAGE_CLONE_ME                 = 45204,
    SPELL_MAGE_CLONE_WEAPONS            = 41054,
    SPELL_MAGE_MASTERS_THREAT_LIST      = 58838
};

class npc_pet_mage_mirror_image : public CreatureScript
{
    public:
        npc_pet_mage_mirror_image() : CreatureScript("npc_pet_mage_mirror_image") { }

        struct npc_pet_mage_mirror_imageAI : CasterAI
        {
            npc_pet_mage_mirror_imageAI(Creature* creature) : CasterAI(creature) { }

            void InitializeAI() override
            {
                CasterAI::InitializeAI();
                Unit* owner = me->GetOwner();
                if (!owner)
                    return;
                // Inherit Master's Threat List (not yet implemented)
                owner->CastSpell((Unit*)NULL, SPELL_MAGE_MASTERS_THREAT_LIST, true);
                // here mirror image casts on summoner spell (not present in client dbc) 49866
                // here should be auras (not present in client dbc): 35657, 35658, 35659, 35660 selfcasted by mirror images (stats related?)
                // Clone Me!
                owner->CastSpell(me, SPELL_MAGE_CLONE_ME, true);
                owner->CastSpell(me, SPELL_MAGE_CLONE_WEAPONS, true);
            }

            // Do not reload Creature templates on evade mode enter - prevent visual lost
            void EnterEvadeMode() override
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
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_pet_mage_mirror_imageAI(creature);
        }
};

void AddSC_mage_pet_scripts()
{
    new npc_pet_mage_mirror_image();
}
