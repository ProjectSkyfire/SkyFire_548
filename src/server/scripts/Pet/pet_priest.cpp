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

enum PriestSpells
{
    SPELL_PRIEST_LIGHTWELL_CHARGES          = 59907
};

class npc_pet_pri_lightwell : public CreatureScript
{
    public:
        npc_pet_pri_lightwell() : CreatureScript("npc_pet_pri_lightwell") { }

        struct npc_pet_pri_lightwellAI : public PassiveAI
        {
            npc_pet_pri_lightwellAI(Creature* creature) : PassiveAI(creature)
            {
                DoCast(me, SPELL_PRIEST_LIGHTWELL_CHARGES, false);
            }

            void EnterEvadeMode() override
            {
                if (!me->IsAlive())
                    return;

                me->DeleteThreatList();
                me->CombatStop(true);
                me->ResetPlayerDamageReq();
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_pet_pri_lightwellAI(creature);
        }
};

void AddSC_priest_pet_scripts()
{
    new npc_pet_pri_lightwell();
}
