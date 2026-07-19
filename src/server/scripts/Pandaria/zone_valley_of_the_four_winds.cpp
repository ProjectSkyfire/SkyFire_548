/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "Spell.h"

class npc_glade_glimmer : public CreatureScript
{
public:
    npc_glade_glimmer() : CreatureScript("npc_glade_glimmer") {}

    struct npc_glade_glimmerAI : public ScriptedAI
    {
        npc_glade_glimmerAI(Creature* creature) : ScriptedAI(creature)
        {
            cooldown = 0;
        }

        void Reset() OVERRIDE
        {
            cooldown = 0;
            me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void JustRespawned() OVERRIDE
        {
            Reset();
        }

        void OnSpellClick(Unit* clicker, bool& result) OVERRIDE
        {
            if (!result)
                return;

            cooldown = 60000;
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);

        }
        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (cooldown)
            {
                if (cooldown <= diff)
                {
                    Reset();
                    cooldown = 0;
                }
                else
                    cooldown -= diff;
            }
        }
    private:
        uint32 cooldown;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_glade_glimmerAI(creature);
    }
};

void AddSC_valley_of_the_four_winds()
{
    new npc_glade_glimmer();
}
