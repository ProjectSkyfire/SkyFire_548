/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
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

/* ScriptData
SDName: Shattered_Halls
SDComment: Heroic only: the executioner and the three prisoners he kills on a timer.
SDCategory: Hellfire Citadel, Shattered Halls
EndScriptData */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "Map.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "shattered_halls.h"

class at_nethekurse_exit : public AreaTriggerScript
{
    public:
        at_nethekurse_exit() : AreaTriggerScript("at_nethekurse_exit") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) OVERRIDE
        {
            InstanceScript* instance = player->GetInstanceScript();
            if (!instance || !instance->instance->IsHeroic())
                return false;

            // The executioner stands in a part of the map that may not be loaded yet
            instance->instance->LoadGrid(ExecutionerPos.GetPositionX(), ExecutionerPos.GetPositionY());

            Creature* kargath = instance->instance->GetCreature(instance->GetData64(NPC_KARGATH_BLADEFIST));
            if (!kargath || instance->GetData(DATA_EXECUTIONER_EVENT) != EXECUTIONER_EVENT_NOT_STARTED)
                return false;

            Creature* executioner = kargath->SummonCreature(NPC_SHATTERED_EXECUTIONER, ExecutionerPos);
            kargath->AI()->DoAction(ACTION_EXECUTIONER_TAUNT);

            if (executioner)
            {
                bool alliance = instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE;
                for (uint8 i = 0; i < VictimCount; ++i)
                    executioner->SummonCreature(alliance ? executionerVictims[i].allianceEntry : executionerVictims[i].hordeEntry, executionerVictims[i].pos);
            }

            return false;
        }
};

enum Spells
{
    SPELL_CLEAVE = 15284
};

class boss_shattered_executioner : public CreatureScript
{
    public:
        boss_shattered_executioner() : CreatureScript("boss_shattered_executioner") { }

        struct boss_shattered_executionerAI : public ScriptedAI
        {
            boss_shattered_executionerAI(Creature* creature) : ScriptedAI(creature), cleaveTimer(500)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset() OVERRIDE
            {
                cleaveTimer = 500;

                if (!instance)
                    return;

                // Every executed prisoner takes one loot tier away
                me->ResetLootMode();
                uint32 prisonersExecuted = instance->GetData(DATA_PRISONERS_EXECUTED);
                if (prisonersExecuted == 0)
                    me->AddLootMode(LOOT_MODE_HARD_MODE_3);
                if (prisonersExecuted <= 1)
                    me->AddLootMode(LOOT_MODE_HARD_MODE_2);
                if (prisonersExecuted <= 2)
                    me->AddLootMode(LOOT_MODE_HARD_MODE_1);

                if (instance->GetBossState(DATA_KARGATH) == DONE)
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                else
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (!instance)
                    return;

                instance->SetData(DATA_SHATTERED_EXECUTIONER, DONE);

                // The quest needs all three prisoners alive
                if (instance->GetData(DATA_PRISONERS_EXECUTED) > 0)
                    return;

                Map::PlayerList const& players = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                    if (Player* player = itr->GetSource())
                    {
                        uint32 questId = player->GetTeam() == ALLIANCE ? QUEST_IMPRISONED_A : QUEST_IMPRISONED_H;
                        if (player->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                            player->CompleteQuest(questId);
                    }
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                if (type != DATA_PRISONERS_EXECUTED || data == 0 || data > VictimCount || !instance)
                    return;

                if (Creature* victim = Unit::GetCreature(*me, instance->GetData64(DATA_FIRST_PRISONER + data - 1)))
                    me->Kill(victim);

                if (data == 1)
                {
                    Map::PlayerList const& players = me->GetMap()->GetPlayers();
                    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                        if (Player* player = itr->GetSource())
                        {
                            uint32 questId = player->GetTeam() == ALLIANCE ? QUEST_IMPRISONED_A : QUEST_IMPRISONED_H;
                            if (player->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                                player->FailQuest(questId);
                        }
                }

                switch (data)
                {
                    case 3:
                        me->RemoveLootMode(LOOT_MODE_HARD_MODE_1);
                        // no break
                    case 2:
                        me->RemoveLootMode(LOOT_MODE_HARD_MODE_2);
                        // no break
                    case 1:
                        me->RemoveLootMode(LOOT_MODE_HARD_MODE_3);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                if (cleaveTimer <= diff)
                {
                    DoCastVictim(SPELL_CLEAVE);
                    cleaveTimer = urand(5000, 7000);
                }
                else
                    cleaveTimer -= diff;

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* instance;
            uint32 cleaveTimer;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new boss_shattered_executionerAI(creature);
        }
};

// 39288, 39289, 39290 - Kargath's Executioner
class spell_kargath_executioner : public AuraScript
{
    PrepareAuraScript(spell_kargath_executioner);

    bool Load() OVERRIDE
    {
        return GetCaster() && GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
    }

    bool AreaCheck(Unit* target)
    {
        return target->GetMap()->GetId() == 540;
    }

    void Register() OVERRIDE
    {
        DoCheckAreaTarget += AuraCheckAreaTargetFn(spell_kargath_executioner::AreaCheck);
    }
};

// 39291 - Remove Kargath's Executioner
class spell_remove_kargath_executioner : public SpellScript
{
    PrepareSpellScript(spell_remove_kargath_executioner);

    bool Load() OVERRIDE
    {
        return GetCaster() && GetCaster()->GetTypeId() == TypeID::TYPEID_PLAYER;
    }

    void HandleScript(SpellEffIndex /*effIndex*/)
    {
        Unit* target = GetCaster();

        target->RemoveAurasDueToSpell(SPELL_KARGATH_EXECUTIONER_1);
        target->RemoveAurasDueToSpell(SPELL_KARGATH_EXECUTIONER_2);
        target->RemoveAurasDueToSpell(SPELL_KARGATH_EXECUTIONER_3);
    }

    void Register() OVERRIDE
    {
        OnEffectHitTarget += SpellEffectFn(spell_remove_kargath_executioner::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

void AddSC_shattered_halls()
{
    new at_nethekurse_exit();
    new boss_shattered_executioner();
    RegisterAuraScript(spell_kargath_executioner);
    RegisterSpellScript(spell_remove_kargath_executioner);
}
