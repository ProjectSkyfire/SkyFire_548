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
#include "ScriptedGossip.h"
#include "TemporarySummon.h"
#include "Vehicle.h"
#include "Map.h"
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

namespace Thundermar
{
    uint32 const DrakeQuest = 28280;
    uint32 const RiderQuest = 28281;
    uint32 const NarkrallQuest = 28282;
    uint32 const Narkrall = 48257;
    uint32 const Torth = 48256;
    uint32 const Drake = 48197;
    uint32 const Skyclaw = 48194;
    uint32 const Dropper = 89557;
    Position const Landing = { -3201.64f, -5013.49f, 120.515f, 3.14f };

    std::list<Player*> Participants(Creature* creature, float range)
    {
        std::list<Player*> players;
        GetPlayerListInGrid(players, creature, range);
        players.remove_if([creature, range](Player* player)
        {
            return !player->IsAlive() || player->GetQuestStatus(NarkrallQuest) != QUEST_STATUS_INCOMPLETE ||
                !creature->IsPhased(player) || !creature->IsWithinDistInMap(player, range);
        });
        return players;
    }
}

class npc_thundermar_colin : public CreatureScript
{
public:
    npc_thundermar_colin() : CreatureScript("npc_thundermar_colin") { }
    struct AI : public ScriptedAI
    {
        AI(Creature* creature) : ScriptedAI(creature) { }
        ObjectGuid bossGuid;
        ObjectGuid mountGuid;
        uint32 check = 1000;
        uint32 elapsed = 0;
        uint32 absent = 0;
        bool active = false;
        bool fighting = false;
        bool approaching = false;

        void Cleanup()
        {
            active = fighting = approaching = false;
            ObjectGuid boss = bossGuid;
            ObjectGuid mount = mountGuid;
            bossGuid = mountGuid = 0;
            if (Creature* creature = ObjectAccessor::GetCreature(*me, boss))
                if (creature->IsAlive())
                    creature->DespawnOrUnsummon();
            if (Creature* creature = ObjectAccessor::GetCreature(*me, mount))
                creature->DespawnOrUnsummon();
            if (me->IsAlive())
            {
                me->CombatStop(true);
                me->SetReactState(REACT_PASSIVE);
                me->SetHealth(me->GetMaxHealth());
                me->GetMotionMaster()->MoveTargetedHome();
            }
        }
        void Reset() OVERRIDE
        {
            Cleanup();
            check = 1000;
            elapsed = absent = 0;
        }
        bool Start(Player* player)
        {
            if (active || !player->IsAlive() || player->GetQuestStatus(Thundermar::NarkrallQuest) != QUEST_STATUS_INCOMPLETE)
                return false;
            active = true;
            fighting = approaching = false;
            check = 1000;
            elapsed = absent = 0;
            Creature* mount = me->SummonCreature(Thundermar::Torth, -3280.0f, -5013.49f, 175.0f, 0.0f,
                TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 600000);
            Creature* boss = me->SummonCreature(Thundermar::Narkrall, -3280.0f, -5013.49f, 175.0f, 0.0f,
                TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 600000);
            if (mount) mountGuid = mount->GetGUID();
            if (boss) bossGuid = boss->GetGUID();
            if (!mount || !boss || !mount->GetVehicleKit())
            {
                Cleanup();
                return false;
            }
            mount->SetReactState(REACT_PASSIVE);
            mount->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            mount->SetCanFly(true);
            mount->SetDisableGravity(true);
            boss->SetReactState(REACT_PASSIVE);
            boss->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            boss->EnterVehicle(mount, 0);
            return true;
        }
        void DoAction(int32 /*action*/) OVERRIDE { Cleanup(); }
        void JustDied(Unit* /*killer*/) OVERRIDE { Cleanup(); }
        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
        {
            // Keep the shared quest giver available during the assisted encounter.
            if (active && damage >= me->GetHealth())
                damage = me->GetHealth() - 1;
        }
        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!active)
                return;
            if (check > diff) { check -= diff; return; }
            uint32 step = 1000 + diff - check;
            check = 1000;
            elapsed += step;
            auto players = Thundermar::Participants(me, 100.0f);
            absent = players.empty() ? absent + step : 0;
            Creature* boss = ObjectAccessor::GetCreature(*me, bossGuid);
            if (!boss || !boss->IsAlive() || absent >= 10000 || elapsed >= 600000)
            {
                Cleanup();
                return;
            }
            if (!fighting)
            {
                Creature* mount = ObjectAccessor::GetCreature(*me, mountGuid);
                if (!mount || !mount->IsAlive() || elapsed >= 30000)
                {
                    Cleanup();
                    return;
                }
                // Vehicle boarding is queued on the passenger's event processor.
                // Begin the approach only after the join event has completed.
                if (!approaching)
                {
                    if (boss->GetVehicleBase() != mount)
                    {
                        if (elapsed >= 5000)
                            Cleanup();
                        return;
                    }
                    approaching = true;
                    Talk(1);
                    mount->GetMotionMaster()->MoveLand(1, Thundermar::Landing);
                    return;
                }
                if (mount->GetDistance(Thundermar::Landing) > 4.0f || players.empty())
                    return;
                fighting = true;
                boss->ExitVehicle(&Thundermar::Landing);
                boss->SetCanFly(false);
                boss->SetDisableGravity(false);
                boss->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
                boss->SetReactState(REACT_AGGRESSIVE);
                boss->AI()->AttackStart(players.front());
                me->SetReactState(REACT_AGGRESSIVE);
                AttackStart(boss);
                mount->GetMotionMaster()->MovePoint(2, -3280.0f, -5013.49f, 190.0f);
                mount->DespawnOrUnsummon(10000);
            }
            if (UpdateVictim())
                DoMeleeAttackIfReady();
        }
    };
    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        player->PrepareGossipMenu(creature, creature->GetCreatureTemplate()->GossipMenuId, true);
        if (AI* ai = CAST_AI(AI, creature->AI()))
            if (!ai->active && player->GetQuestStatus(Thundermar::NarkrallQuest) == QUEST_STATUS_INCOMPLETE)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I'm ready to fight him!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->SendPreparedGossip(creature);
        return true;
    }
    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) OVERRIDE
    {
        player->PlayerTalkClass->ClearMenus();
        player->PlayerTalkClass->SendCloseGossip();
        if (sender == GOSSIP_SENDER_MAIN && action == GOSSIP_ACTION_INFO_DEF + 1)
            if (AI* ai = CAST_AI(AI, creature->AI()))
                ai->Start(player);
        return true;
    }
    CreatureAI* GetAI(Creature* creature) const OVERRIDE { return new AI(creature); }
};

class npc_thundermar_narkrall : public CreatureScript
{
public:
    npc_thundermar_narkrall() : CreatureScript("npc_thundermar_narkrall") { }
    struct AI : public ScriptedAI
    {
        AI(Creature* creature) : ScriptedAI(creature) { }
        EventMap events;
        void Reset() OVERRIDE { events.Reset(); }
        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            events.ScheduleEvent(1, 5000);
            events.ScheduleEvent(2, 8000);
            events.ScheduleEvent(3, 4000);
            events.ScheduleEvent(4, 12000);
        }
        void NotifyColin()
        {
            if (TempSummon* summon = me->ToTempSummon())
                if (Creature* colin = ObjectAccessor::GetCreature(*me, summon->GetSummonerGUID()))
                    colin->AI()->DoAction(1);
        }
        void EnterEvadeMode() OVERRIDE { NotifyColin(); }
        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            for (Player* player : Thundermar::Participants(me, 60.0f))
                player->KilledMonsterCredit(Thundermar::Narkrall);
            NotifyColin();
        }
        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;
            events.Update(diff);
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            if (uint32 event = events.ExecuteEvent())
            {
                switch (event)
                {
                    case 1: DoCastVictim(13730); events.ScheduleEvent(1, 20000); break;
                    case 2: DoCastVictim(9080); events.ScheduleEvent(2, 12000); break;
                    case 3: DoCastVictim(57846); events.ScheduleEvent(3, 7000); break;
                    case 4: DoCastVictim(32736); events.ScheduleEvent(4, 14000); break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE { return new AI(creature); }
};

class npc_thundermar_assault_drake : public CreatureScript
{
public:
    npc_thundermar_assault_drake() : CreatureScript("npc_thundermar_assault_drake") { }
    struct AI : public ScriptedAI
    {
        AI(Creature* creature) : ScriptedAI(creature) { }
        void JustDied(Unit* killer) OVERRIDE
        {
            Player* player = killer ? killer->GetCharmerOrOwnerPlayerOrPlayerItself() : nullptr;
            if (!player || (player->GetQuestStatus(Thundermar::DrakeQuest) == QUEST_STATUS_NONE &&
                player->GetQuestStatus(Thundermar::RiderQuest) != QUEST_STATUS_INCOMPLETE))
                return;
            float x = me->GetPositionX();
            float y = me->GetPositionY();
            float z = me->GetMap()->GetHeight(me->GetPhaseMask(), x, y, me->GetPositionZ(), true, 250.0f);
            if (z <= INVALID_HEIGHT)
                return;
            if (Creature* rider = me->SummonCreature(Thundermar::Skyclaw, x, y, z, me->GetOrientation(),
                TempSummonType::TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 180000))
                rider->AI()->AttackStart(player);
        }
    };
    CreatureAI* GetAI(Creature* creature) const OVERRIDE { return new AI(creature); }
};

class spell_thundermar_drake_dropper : public SpellScriptLoader
{
public:
    spell_thundermar_drake_dropper() : SpellScriptLoader("spell_thundermar_drake_dropper") { }
    class Script : public SpellScript
    {
        PrepareSpellScript(Script);
        SpellCastResult Check()
        {
            Player* player = GetCaster()->ToPlayer();
            Creature* target = GetExplTargetUnit() ? GetExplTargetUnit()->ToCreature() : nullptr;
            if (!player || player->GetQuestStatus(Thundermar::DrakeQuest) != QUEST_STATUS_INCOMPLETE ||
                !target || !target->IsAlive() || target->GetEntry() != Thundermar::Drake || !target->IsPhased(player))
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            return SpellCastResult::SPELL_CAST_OK;
        }
        void Register() OVERRIDE { OnCheckCast += SpellCheckCastFn(Script::Check); }
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
    new npc_thundermar_colin();
    new npc_thundermar_narkrall();
    new npc_thundermar_assault_drake();
    new spell_thundermar_drake_dropper();
}
