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
        if (Creature* pOokOok = creature->SummonCreature(NPC_OOK_OOK, -750.555f, 1334.4f, 162.71f, 1.83f, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, HOUR * 2 * IN_MILLISECONDS))
        {
            // Note: should be Yell instead of Say
            pOokOok->MonsterSay(SAY_OOK_OOK_1, Language::LANG_UNIVERSAL, 0);
            
            pOokOok->GetMotionMaster()->MoveJump(OokOokLandPos, pOokOok->GetExactDist2d(OokOokLandPos.GetPositionX(), OokOokLandPos.GetPositionY()) * 10.0f / 5.0f, 5.0f);
            pOokOok->SetHomePosition(OokOokLandPos);
            //if (creature->GetVictim())
            //    pOakOak->AI()->AttackStart(creature->GetVictim());
        }
        killer->CastSpell(killer, SPELL_SSB_BANANA_BAR_REMOVE, true);
        instance->SetBossState(DATA_BANANA_EVENT, DONE);
    }
};

void SummonHoptallusIfReady(InstanceScript* instance, Creature* creature, Unit* killer)
{
    if (instance->GetBossState(DATA_RABBIT_EVENT) == DONE)
        return;

    if (Creature* pHoptalus = creature->SummonCreature(NPC_HOPTALLUS, -726.259f, 1250.96f, 165.235f, 0.27f, TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, HOUR * 2 * IN_MILLISECONDS))
    {
        pHoptalus->GetMotionMaster()->MoveJump(HoptallusLandPos, pHoptalus->GetExactDist2d(HoptallusLandPos.GetPositionX(), HoptallusLandPos.GetPositionY()) * 10.0f / 5.0f, 5.0f);
        pHoptalus->SetHomePosition(HoptallusLandPos);
        //if (creature->GetVictim())
        //    pOakOak->AI()->AttackStart(creature->GetVictim());
    }
    instance->SetBossState(DATA_RABBIT_EVENT, DONE);
};

class AreaTrigger_at_ssb_banana_bar : public AreaTriggerScript
{
public:
    AreaTrigger_at_ssb_banana_bar() : AreaTriggerScript("at_ssb_banana_bar") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) override
    {
        // Only trigger once
        if (InstanceScript* instance = player->GetInstanceScript())
        {
            if (instance->GetBossState(DATA_BANANA_EVENT) == DONE)
                return false;

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

        void Reset() override { }

        void EnterCombat(Unit* /*who*/) override { }

        void JustDied(Unit* killer) override
        {
            if (instance)
                SummonOokOokIfReady(instance, me, killer);
        }

        void UpdateAI(uint32 diff) override
        {
            if (instance->GetBossState(DATA_BANANA_EVENT) == DONE)
            {
                if (me->GetEntry() == NPC_ANIMAL1 || me->GetEntry() == NPC_ANIMAL2)
                {
                    if (!me->isMoving())
                    {
                        me->SetSpeed(MOVE_RUN, 1.5f);
                        me->GetMotionMaster()->MovePoint(1, BananaFleePos);
                        me->DespawnOrUnsummon(15000);
                    }
                }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        InstanceScript* instance;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_banana_hozenAI(creature);
    }
};

class npc_bopper : public CreatureScript
{
public:
    npc_bopper() : CreatureScript("npc_bopper") { }

    struct npc_bopperAI : public ScriptedAI
    {
        npc_bopperAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        void Reset() override { }

        void EnterCombat(Unit* /*who*/) override { }

        void JustDied(Unit* killer) override
        {
            if (instance)
                SummonHoptallusIfReady(instance, me, killer);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        InstanceScript* instance;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_bopperAI(creature);
    }
};

void AddSC_stormstout_brewery()
{
    new AreaTrigger_at_ssb_banana_bar();
    new npc_banana_hozen();
    new npc_bopper();
}
