/*
 * Copyright (C) 2011-2021 Project SkyFire <https://www.projectskyfire.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stormstout_brewery.h"
#include "Player.h"

enum Spells
{
    SPELL_SSB_BANANA_BAR        = 107297,
    SPELL_SSB_BANANA_BAR_REMOVE = 107347
};

#define SAY_OOK_OOK_1         "Who crashing Ook party!?"

void SummonOokOokIfReady(InstanceScript* instance, Creature* creature, Unit* killer)
{
    instance->SetData(DATA_BANANA_EVENT, SPECIAL);  // Increment DeathCount
    killer->SetPower(POWER_ALTERNATE_POWER, instance->GetData64(DATA_BANANA_EVENT));

    if (instance->GetData64(DATA_BANANA_EVENT) > 39)
    {
        killer->CastSpell(killer, 107347, true);
        instance->SetBossState(DATA_BANANA_EVENT, DONE);

        if (Creature* pOokOok = creature->SummonCreature(NPC_OOK_OOK, -750.555f, 1334.4f, 162.71f, 1.83f, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, HOUR * 2 * IN_MILLISECONDS))
        {
            // Note: should be Yell instead of Say
            pOokOok->MonsterSay(SAY_OOK_OOK_1, Language::LANG_UNIVERSAL, 0);
            
            if (Creature* JumpTarget = creature->SummonCreature(18721, -754.695, 1348.25f, 147.35f, 1.83f, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10 * IN_MILLISECONDS))
                pOokOok->JumpTo(JumpTarget, 5.0f);

            //if (creature->GetVictim())
            //    pOakOak->AI()->AttackStart(creature->GetVictim());
        }
    }
};

class AreaTrigger_at_ssb_banana_bar : public AreaTriggerScript
{
public:
    AreaTrigger_at_ssb_banana_bar() : AreaTriggerScript("at_ssb_banana_bar") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
    {
        // Only trigger once
        if (InstanceScript* instance = player->GetInstanceScript())
        {
            if (player->IsAlive())
            {
                player->CastSpell(player, SPELL_SSB_BANANA_BAR, true);
                StartBananaEvent(instance, player);
                return true;
            }
        }
        return false;
    }

    void StartBananaEvent(InstanceScript* instance, Player* player)
    {
        instance->SetData(DATA_BANANA_EVENT, IN_PROGRESS);  // Reset DeathCount
        player->SetPower(POWER_ALTERNATE_POWER, instance->GetData64(DATA_BANANA_EVENT));
    }
};

class npc_banana_hozen : public CreatureScript
{
public:
    npc_banana_hozen() : CreatureScript("npc_banana_hozen") { }


    struct npc_banana_hozenAI : public ScriptedAI
    {
        npc_banana_hozenAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        void Reset() OVERRIDE { }

        void EnterCombat(Unit* /*who*/) OVERRIDE { }

        void JustDied(Unit* killer) OVERRIDE
        {
            if (instance)
                SummonOokOokIfReady(instance, me, killer);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        InstanceScript* instance;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_banana_hozenAI(creature);
    }
};



void AddSC_stormstout_brewery()
{
    new AreaTrigger_at_ssb_banana_bar();
    new npc_banana_hozen();
}
