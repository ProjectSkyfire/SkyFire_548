/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "DraconicMending.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "SpellScript.h"
#include "SpellInfo.h"
#include <cmath>
#include <vector>

namespace
{
    uint32 const QuestMending = 27505;
    uint32 const QuestSeeds = 27506;
    uint32 const Mender = 45746;
    uint32 const Wyrmkiller = 45748;
    uint32 const WoundedAura = 85352;
    uint32 const WoundedSleep = 85408;
    uint32 const MendingSpell = 85340;

    bool IsWoundedDragon(uint32 entry)
    {
        return entry == 45560 || entry == 45857 || entry == 45866;
    }

    bool IsSeedTarget(uint32 entry)
    {
        return IsWoundedDragon(entry) || entry == Mender || entry == Wyrmkiller || entry == 45788 || entry == 45503;
    }

    std::list<Player*> QuestPlayers(Creature* creature, float range)
    {
        std::list<Player*> players;
        GetPlayerListInGrid(players, creature, range);
        players.remove_if([creature, range](Player* player)
        {
            return !player->IsAlive() || player->GetQuestStatus(QuestMending) != QUEST_STATUS_INCOMPLETE ||
                !creature->IsWithinDistInMap(player, range) || !creature->IsPhased(player);
        });
        return players;
    }
}

class npc_vermillion_lirastrasza_intro : public CreatureScript
{
public:
    npc_vermillion_lirastrasza_intro() : CreatureScript("npc_vermillion_lirastrasza_intro") { }
    struct AI : public ScriptedAI
    {
        AI(Creature* creature) : ScriptedAI(creature) { }
        void MoveInLineOfSight(Unit* unit) OVERRIDE
        {
            Player* player = unit ? unit->ToPlayer() : nullptr;
            if (!player || !player->IsAlive() || !me->IsWithinDistInMap(player, 20.0f) ||
                !me->IsPhased(player) || !me->IsWithinLOSInMap(player))
                return;
            // The client credit spell targets a summoner; this actor is a permanent world spawn.
            if (player->GetQuestStatus(27485) == QUEST_STATUS_INCOMPLETE ||
                player->GetQuestStatus(27486) == QUEST_STATUS_INCOMPLETE)
                player->KilledMonsterCredit(45708);
        }
        void UpdateAI(uint32 /*diff*/) OVERRIDE { }
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE { return new AI(creature); }
};

class npc_vermillion_wounded_dragon : public CreatureScript
{
public:
    npc_vermillion_wounded_dragon() : CreatureScript("npc_vermillion_wounded_dragon") { }

    struct AI : public ScriptedAI
    {
        AI(Creature* creature) : ScriptedAI(creature) { }

        std::vector<ObjectGuid> menders;
        std::vector<ObjectGuid> attackers;
        Skyfire::DraconicMending::Encounter progress;
        uint32 tick = 1000;
        uint32 wave = 15000;
        bool active = false;
        bool leaving = false;

        void Cleanup()
        {
            // Leave dead summons long enough for Ruby Seeds; their corpse timer removes them.
            for (auto const& list : {menders, attackers})
                for (ObjectGuid guid : list)
                    if (Creature* summon = ObjectAccessor::GetCreature(*me, guid))
                        if (summon->IsAlive())
                            summon->DespawnOrUnsummon();
            menders.clear();
            attackers.clear();
        }

        void Reset() OVERRIDE
        {
            Cleanup();
            active = leaving = false;
            tick = 1000;
            wave = 15000;
            progress = Skyfire::DraconicMending::Encounter();
            me->setRegeneratingHealth(false);
            me->SetReactState(REACT_PASSIVE);
            me->SetCanFly(false);
            me->SetDisableGravity(false);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetHealth(me->CountPctFromMaxHealth(25));
            me->CastSpell(me, WoundedAura, true);
            me->CastSpell(me, WoundedSleep, true);
        }

        void EnterEvadeMode() OVERRIDE { }
        void JustDied(Unit* /*killer*/) OVERRIDE { Cleanup(); }

        void Start()
        {
            active = true;
            for (uint32 i = 0; i < 5; ++i)
            {
                float angle = float(i) * 1.256637f;
                float x = me->GetPositionX() + std::cos(angle) * 10.0f;
                float y = me->GetPositionY() + std::sin(angle) * 10.0f;
                float z = me->GetPositionZ();
                me->UpdateAllowedPositionZ(x, y, z);
                if (Creature* mender = me->SummonCreature(Mender, x, y, z, angle + 3.141593f,
                    TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000))
                {
                    menders.push_back(mender->GetGUID());
                    mender->SetReactState(REACT_PASSIVE);
                    mender->AI()->SetGUID(me->GetGUID());
                }
            }
        }

        std::vector<Creature*> LivingMenders()
        {
            std::vector<Creature*> result;
            for (ObjectGuid guid : menders)
                if (Creature* mender = ObjectAccessor::GetCreature(*me, guid))
                    if (mender->IsAlive())
                        result.push_back(mender);
            return result;
        }

        void SpawnWave(std::vector<Creature*> const& living)
        {
            uint32 alive = 0;
            for (ObjectGuid guid : attackers)
                if (Creature* attacker = ObjectAccessor::GetCreature(*me, guid))
                    if (attacker->IsAlive())
                        ++alive;
            for (uint32 i = 0; i < 2 && alive < 6 && !living.empty(); ++i, ++alive)
            {
                float angle = frand(0.0f, 6.283185f);
                float x = me->GetPositionX() + std::cos(angle) * 23.0f;
                float y = me->GetPositionY() + std::sin(angle) * 23.0f;
                float z = me->GetPositionZ();
                me->UpdateAllowedPositionZ(x, y, z);
                if (Creature* attacker = me->SummonCreature(Wyrmkiller, x, y, z, angle + 3.141593f,
                    TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 60000))
                {
                    attackers.push_back(attacker->GetGUID());
                    attacker->AI()->AttackStart(living[urand(0, uint32(living.size() - 1))]);
                }
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (leaving)
                return;
            if (tick > diff)
            {
                tick -= diff;
                return;
            }
            uint32 elapsed = 1000 + diff - tick;
            tick = 1000;
            auto players = QuestPlayers(me, 50.0f);
            if (!active)
            {
                if (!players.empty())
                    Start();
                return;
            }

            auto living = LivingMenders();
            auto result = progress.Update(elapsed, !players.empty(), me->HealthAbovePct(99), unsigned(living.size()));
            if (result == Skyfire::DraconicMending::Result::Recovered)
            {
                // Each surviving mender counts once, only for nearby players still on the quest.
                for (Player* player : players)
                    for (size_t i = 0; i < living.size() && player->GetQuestStatus(QuestMending) == QUEST_STATUS_INCOMPLETE; ++i)
                        player->KilledMonsterCredit(Mender);
                Cleanup();
                leaving = true;
                me->RemoveAurasDueToSpell(WoundedAura);
                me->RemoveAurasDueToSpell(WoundedSleep);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                me->GetMotionMaster()->MovePoint(1, me->GetPositionX() + 40.0f, me->GetPositionY(), me->GetPositionZ() + 50.0f);
                me->DespawnOrUnsummon(8000);
                return;
            }
            if (result != Skyfire::DraconicMending::Result::Running)
            {
                Reset();
                return;
            }
            if (wave <= elapsed)
            {
                SpawnWave(living);
                wave = 15000;
            }
            else
                wave -= elapsed;
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE { return new AI(creature); }
};

class npc_vermillion_mender : public CreatureScript
{
public:
    npc_vermillion_mender() : CreatureScript("npc_vermillion_mender") { }
    struct AI : public ScriptedAI
    {
        AI(Creature* creature) : ScriptedAI(creature) { }
        ObjectGuid dragonGuid;
        uint32 check = 1000;
        void SetGUID(uint64 guid, int32 /*id*/ = 0) OVERRIDE { dragonGuid = guid; }
        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (check > diff) { check -= diff; return; }
            check = 2000;
            Creature* dragon = ObjectAccessor::GetCreature(*me, dragonGuid);
            if (!dragon || !dragon->IsAlive())
                return;
            if (!me->IsNonMeleeSpellCasted(false))
                DoCast(dragon, MendingSpell);
        }
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE { return new AI(creature); }
};

class npc_vermillion_wyrmkiller : public CreatureScript
{
public:
    npc_vermillion_wyrmkiller() : CreatureScript("npc_vermillion_wyrmkiller") { }
    struct AI : public ScriptedAI
    {
        AI(Creature* creature) : ScriptedAI(creature) { }
        uint32 breath = 6000;
        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;
            if (breath <= diff)
            {
                DoCastVictim(85349);
                breath = 8000;
            }
            else
                breath -= diff;
            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE { return new AI(creature); }
};

class spell_vermillion_ruby_seeds : public SpellScriptLoader
{
public:
    spell_vermillion_ruby_seeds() : SpellScriptLoader("spell_vermillion_ruby_seeds") { }
    class Script : public SpellScript
    {
        PrepareSpellScript(Script);
        SpellCastResult Check()
        {
            Player* player = GetCaster()->ToPlayer();
            Creature* corpse = GetExplTargetUnit() ? GetExplTargetUnit()->ToCreature() : nullptr;
            if (!player || player->GetQuestStatus(QuestSeeds) != QUEST_STATUS_INCOMPLETE ||
                player->GetAreaId() != 5155 || !corpse || corpse->IsAlive() || !IsSeedTarget(corpse->GetEntry()) ||
                corpse->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            return SpellCastResult::SPELL_CAST_OK;
        }
        void Plant(SpellEffIndex /*effect*/)
        {
            if (Check() != SpellCastResult::SPELL_CAST_OK)
                return;
            Creature* corpse = GetHitCreature();
            Player* player = GetCaster()->ToPlayer();
            if (!corpse || corpse != GetExplTargetUnit())
                return;
            corpse->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            player->KilledMonsterCredit(45503);
            player->SummonCreature(45833, corpse->GetPositionX(), corpse->GetPositionY(), corpse->GetPositionZ(),
                corpse->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 30000);
            corpse->DespawnOrUnsummon(1000);
        }
        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(Script::Check);
            OnEffectHitTarget += SpellEffectFn(Script::Plant, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };
    SpellScript* GetSpellScript() const OVERRIDE { return new Script(); }
};

void AddSC_twilight_highlands()
{
    new npc_vermillion_lirastrasza_intro();
    new npc_vermillion_wounded_dragon();
    new npc_vermillion_mender();
    new npc_vermillion_wyrmkiller();
    new spell_vermillion_ruby_seeds();
}
