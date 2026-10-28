/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "thunder_king_citadel.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "CreatureAI.h"
#include "MoveSplineInit.h"
#include "SpellScript.h"
#include "Vehicle.h"
#include "LFGMgr.h"

enum Spells
{
    SPELL_LIGHTNING_ATTRACTOR          = 137114,
    SPELL_LIGHTNING_SURGE              = 139804,
    SPELL_STONE_SMASH                  = 139777,
    SPELL_LIMITED_TIME                 = 140000, // 5m
    SPELL_COMPLETE_SCENARIO_SCREEN_EFF = 140005,
    SPELL_SENTRY_BEAM                  = 139810,
    SPELL_SENTRY                       = 139808, // Areatrigger
    SPELL_SUMM_SENTRY_BEAM_BUNNY       = 139807,
    SPELL_RUNE_TRAP                    = 139798,
    SPELL_SPOTTED                      = 138954,
    SPELL_SPEED                        = 132959,
    SPELL_RING_OF_FROST                = 15063,
    SPELL_ARCANE_BOLT                  = 13748,
    SPELL_ARCANE_EXPLOSION             = 51820,
    SPELL_THROVES_OF_THE_THUNDER_KING  = 137275,
    SPELL_BATTLE_SHOUT                 = 32064,
    SPELL_MORTAL_STRIKE                = 13737,
    SPELL_IMPALING_PULL                = 82742,
    SPELL_MIGHTY_STOMP                 = 136855,
    SPELL_MIGHTY_CRASH                 = 136844,
    SPELL_ETERNAL_SLUMBER              = 140011,
    SPELL_CRUSH_ARMOR                  = 127157,
    SPELL_LEAPING_RUSH                 = 131942,
};

enum Events
{
    EVENT_RING_OF_FROST,
    EVENT_ARCANE_BOLT,
    EVENT_ARCANE_EXPLOSION,
    EVENT_BATTLE_SHOUT,
    EVENT_MORTAL_STRIKE,
    EVENT_MIGHTY_STOMP,
    EVENT_MIGHTY_CRASH,
    EVENT_ETERNAL_SLUMBER,
    EVENT_CRUSH_ARMOR,
    EVENT_LEAPING_RUSH,
};

// Lightning Pillar Master 70409
struct npc_lightning_pillar_master : public ScriptedAI
{
    npc_lightning_pillar_master(Creature* creature) : ScriptedAI(creature) { }

    float x, y;
    TaskScheduler scheduler;

    void InitializeAI() OVERRIDE
    {
        scheduler
            .Schedule(Milliseconds(urand(500,5000)), [this](TaskContext context)
        {
            DoCast(me, SPELL_LIGHTNING_SURGE);
            context.Repeat(Milliseconds(urand(6000, 10000)));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Stone Sentinel 70324
struct npc_stone_sentinel : public ScriptedAI
{
    npc_stone_sentinel(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;

    void InitializeAI() OVERRIDE
    {
        scheduler
            .Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            if (Player* itr = me->FindNearestPlayer(20.0f))
            {
                if (me->HasInArc(M_PI / 3, itr))
                {
                    DoCast(me, SPELL_STONE_SMASH);
                    context.Repeat(Milliseconds(8000));
                    return;
                }
            }

            context.Repeat(Milliseconds(1000));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Lightning Pillar Master 70413
struct npc_thunder_king_treasure_sentry_totem : public ScriptedAI
{
    npc_thunder_king_treasure_sentry_totem(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 targetGUID;

    void InitializeAI() OVERRIDE
    {
        scheduler
            .Schedule(Milliseconds(1500), [this](TaskContext context)
        {
            DoCast(me, SPELL_SENTRY, true);
        });
    }

    void SetGUID(uint64 guid, int32 /*type*/) OVERRIDE
    {
        targetGUID = guid;
    }

    uint64 GetGUID(int32 /*type*/) const OVERRIDE
    {
        return targetGUID;
    }

    void JustSummoned(Creature* summon) OVERRIDE
    {
        DoCast(summon, SPELL_SENTRY_BEAM);
        summon->ClearUnitState(UNIT_STATE_CASTING);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Statis Rune 70406
struct npc_thunder_king_treasure_stasis_rune : public ScriptedAI
{
    npc_thunder_king_treasure_stasis_rune(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    bool hasTriggered;

    void InitializeAI() OVERRIDE
    {
        me->SetDisplayFromModel(0);

        hasTriggered = false;

        scheduler
            .Schedule(Seconds(1), [this](TaskContext context)
        {
            if (me->FindNearestPlayer(1.5f) && !hasTriggered)
                DoCast(me, SPELL_RUNE_TRAP, true);

            context.Repeat(Seconds(1));
        });
    }

    void DoAction(int32 actionId) OVERRIDE
    {
        hasTriggered = true;

        scheduler
            .Schedule(Seconds(6), [this](TaskContext context)
        {
            hasTriggered = false;
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Speed Rune 70405
struct npc_thunder_king_treasure_speed_rune : public ScriptedAI
{
    npc_thunder_king_treasure_speed_rune(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;

    void InitializeAI() OVERRIDE
    {
        me->SetDisplayFromModel(0);

        scheduler
            .Schedule(Seconds(1), [this](TaskContext context)
        {
            if (Player* target = me->FindNearestPlayer(1.0f))
                if (!target->HasAura(SPELL_SPEED))
                    target->CastSpell(target, SPELL_SPEED, true);

            context.Repeat(Seconds(1));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// Zandalari Arcweaver 70401
struct npc_thunder_king_treasure_zandalari_arcweaver : public customCreatureAI
{
    npc_thunder_king_treasure_zandalari_arcweaver(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_ARCANE_BOLT, urand(3500, 5 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_ARCANE_EXPLOSION, urand(8 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_RING_OF_FROST, 1 * IN_MILLISECONDS);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_ARCANE_BOLT, urand(3500, 5 * IN_MILLISECONDS), EVENT_ARCANE_BOLT, eventId);
            ExecuteTargetEvent(SPELL_RING_OF_FROST, 10 * IN_MILLISECONDS, EVENT_RING_OF_FROST, eventId, PRIORITY_SELF);
            ExecuteTargetEvent(SPELL_ARCANE_EXPLOSION, urand(8 * IN_MILLISECONDS, 12 * IN_MILLISECONDS), EVENT_ARCANE_EXPLOSION, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Sentry Beam Bunny 70412
struct npc_thunder_king_treasure_sentry_beam_bunny : public ScriptedAI
{
    npc_thunder_king_treasure_sentry_beam_bunny(Creature* creature) : ScriptedAI(creature) { }

    TaskScheduler scheduler;
    uint64 targetGUID;

    void IsSummonedBy(Unit* summoner) OVERRIDE
    {
        if (summoner->ToCreature())
            targetGUID = summoner->ToCreature()->AI()->GetGUID();

        scheduler
            .Schedule(Milliseconds(500), [this](TaskContext context)
        {
            if (Unit* target = ObjectAccessor::GetUnit(*me, targetGUID))
            {
                me->GetMotionMaster()->MoveFollow(target, 0.0f, me->GetAngle(target));

                if (me->GetExactDist2d(target) < 1.5f && !target->HasAura(SPELL_SPOTTED))
                {
                    me->AddAura(SPELL_SPOTTED, target);

                    // Call Help Here!
                    std::list<Creature*> guardianList;
                    GetCreatureListWithEntryInGrid(guardianList, me, NPC_ZANDALARI_ARCWEAVER, 70.0f);
                    GetCreatureListWithEntryInGrid(guardianList, me, NPC_STONE_WATCHER, 70.0f);
                    GetCreatureListWithEntryInGrid(guardianList, me, NPC_ZANDALARI_VENOMBLADE, 70.0f);

                    for (auto&& itr : guardianList)
                        itr->RemoveChanneledCast(target->GetGUID());
                }
            }
            context.Repeat(Milliseconds(500));
        });
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        scheduler.Update(diff);
    }
};

// God-Hulk Gulkan 70400, 70326
struct npc_god_hulk_gulkan : public customCreatureAI
{
    npc_god_hulk_gulkan(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* /*who*/) OVERRIDE
    {
        events.ScheduleEvent(EVENT_MIGHTY_STOMP, urand(10 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_MIGHTY_CRASH, 7500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_MIGHTY_CRASH, urand(9 * IN_MILLISECONDS, 15500), EVENT_MIGHTY_CRASH, eventId, PRIORITY_CHANNELED);
            ExecuteTargetEvent(SPELL_MIGHTY_STOMP, 12 * IN_MILLISECONDS, EVENT_MIGHTY_STOMP, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Zandalari Venomblade 70328
struct npc_zandalari_venomblade : public customCreatureAI
{
    npc_zandalari_venomblade(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        events.Reset();
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        DoCast(who, SPELL_IMPALING_PULL, true);
        events.ScheduleEvent(EVENT_BATTLE_SHOUT, urand(10 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
        events.ScheduleEvent(EVENT_MORTAL_STRIKE, 7500);
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_MORTAL_STRIKE, urand(5500, 10 * IN_MILLISECONDS), EVENT_MIGHTY_CRASH, eventId);
            ExecuteTargetEvent(SPELL_BATTLE_SHOUT, 12 * IN_MILLISECONDS, EVENT_BATTLE_SHOUT, eventId, PRIORITY_SELF);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Taoshi  70320
class npc_thunder_king_treasure_taoshi : public CreatureScript
{
    public:
        npc_thunder_king_treasure_taoshi() : CreatureScript("npc_thunder_king_treasure_taoshi") { }
    
        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();
    
            if (action == GOSSIP_ACTION_INFO_DEF + 1 && player->GetInstanceScript())
            {
                player->GetInstanceScript()->SetData(DATA_SPEAK_WITH_TAOSHI, DONE);
                creature->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                // Timer
                player->CastSpell(player, SPELL_THROVES_OF_THE_THUNDER_KING, true);
                creature->CastSpell(player, SPELL_LIMITED_TIME, true);
            }
    
            player->CLOSE_GOSSIP_MENU();
    
            return true;
        }
    
        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            player->ADD_GOSSIP_ITEM_DB(player->GetDefaultGossipMenuForSource(creature), 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }
};

// Taoshi 70316
class npc_thunder_king_treasure_taoshi_queue : public CreatureScript
{
    public:
        npc_thunder_king_treasure_taoshi_queue() : CreatureScript("npc_thunder_king_treasure_taoshi_queue") { }
    
        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();
    
            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                lfg::LfgDungeonSet scenario = { 620 };
                sLFGMgr->JoinLfg(player, lfg::PLAYER_ROLE_DAMAGE, scenario, "");
                player->DestroyItemCount(94222, 1, true);
            }
    
            player->CLOSE_GOSSIP_MENU();
    
            return true;
        }
    
        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            if (creature->IsQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            if (player->HasItemCount(94222) && !player->GetGroup() && !player->IsUsingLfg())
                player->ADD_GOSSIP_ITEM_DB(player->GetDefaultGossipMenuForSource(creature), 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }
};

// Stone Watcher 70327
struct npc_thunder_king_treasure_stone_watcher : public customCreatureAI
{
    npc_thunder_king_treasure_stone_watcher(Creature* creature) : customCreatureAI(creature) { }

    void Reset() OVERRIDE
    {
        me->SetReactState(REACT_PASSIVE);
        DoCast(me, SPELL_ETERNAL_SLUMBER, true);
        events.Reset();
    }

    void EnterCombat(Unit* who) OVERRIDE
    {
        me->SetReactState(REACT_AGGRESSIVE);
        me->RemoveAurasDueToSpell(SPELL_ETERNAL_SLUMBER);
        DoCast(who, SPELL_LEAPING_RUSH, true);
        events.ScheduleEvent(EVENT_CRUSH_ARMOR, urand(2500, 8 * IN_MILLISECONDS));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (!UpdateVictim())
            return;

        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            ExecuteTargetEvent(SPELL_CRUSH_ARMOR, urand(5500, 10 * IN_MILLISECONDS), EVENT_CRUSH_ARMOR, eventId);
            break;
        }

        DoMeleeAttackIfReady();
    }
};

// Tenwu of the Red Smoke 70321
class npc_thunder_king_treasure_tenwu_of_the_red_smoke : public CreatureScript
{
    public:
        npc_thunder_king_treasure_tenwu_of_the_red_smoke() : CreatureScript("npc_thunder_king_treasure_tenwu_of_the_red_smoke") { }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
                player->CastSpell(player, SPELL_TENWU_EXIT_TRIGGERED_CREDIT, true);

            player->CLOSE_GOSSIP_MENU();

            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I`m ready to leave. Let`s get our of here.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1); // hasn`t gossip info in sniffs
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }
};

// Lever 218885, 218886
class go_thunder_king_treasure_lever : public GameObjectScript
{
    public:
        go_thunder_king_treasure_lever() : GameObjectScript("go_thunder_king_treasure_lever") { }
    
        void OnGameObjectStateChanged(GameObject* go, uint32 state) OVERRIDE
        {
            if (GOState(state) == GOState::GO_STATE_ACTIVE && go->GetInstanceScript())
            {
                switch (go->GetEntry())
                {
                    case GO_LEVER_1:
                        go->GetInstanceScript()->SetData(DATA_OPEN_FIRST_DOOR_PACK, DONE);
                        break;
                    case GO_LEVER_2:
                        go->GetInstanceScript()->SetData(DATA_OPEN_SECOND_DOOR_PACK, DONE);
                        break;
                }

                go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_INTERACT_COND);
            }
        }
};

// Mogu Treasure Chest 218757, 218772
class go_thunder_king_treasure_mogu_chest : public GameObjectScript
{
    public:
        go_thunder_king_treasure_mogu_chest() : GameObjectScript("go_thunder_king_treasure_mogu_chest") 
        {
            chestGUIDs.clear();
        }
    
        void OnLootStateChanged(GameObject* go, uint32 state, Unit* unit) OVERRIDE
        {
            if (LootState(state) == LootState::GO_ACTIVATED && unit)
            {
                if (std::find(chestGUIDs.begin(), chestGUIDs.end(), go->GetGUID()) != chestGUIDs.end())
                    return;

                chestGUIDs.push_back(go->GetGUID());
                
                if (unit->GetInstanceScript())
                {
                    unit->GetInstanceScript()->SetData(CHEST_DATA, go->GetEntry() == GO_MOGU_GOLDEN_TREASURE_CHEST ? 10 : 5);

                    if (go->GetEntry() == GO_MOGU_GOLDEN_TREASURE_CHEST)
                        unit->GetInstanceScript()->SetData(GOLDEN_CHEST_DATA, 1);
                }
            }
        }
    
        private:
            std::list<uint64> chestGUIDs;
};

// Rune Trap 139798
class spell_thunder_king_treasure_rune_trap : public AuraScript
{
    PrepareAuraScript(spell_thunder_king_treasure_rune_trap);

    void HandleOnApply(AuraEffect const* /*aureff*/, AuraEffectHandleModes /*mode*/)
    {
        if (GetCaster() && GetCaster()->ToCreature())
            GetCaster()->ToCreature()->AI()->DoAction(1);
    }

    void Register() OVERRIDE
    {
        OnEffectApply += AuraEffectApplyFn(spell_thunder_king_treasure_rune_trap::HandleOnApply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
    }
};

// Limited Time 140000
class spell_thunder_king_treasure_limited_time : public AuraScript
{
    PrepareAuraScript(spell_thunder_king_treasure_limited_time);

    void HandleOnRemove(AuraEffect const* aureff, AuraEffectHandleModes /*mode*/)
    {
        if (aureff->GetBase()->GetDuration() > 0 && GetOwner() && GetOwner()->ToUnit() && GetOwner()->ToUnit()->GetInstanceScript())
            GetOwner()->ToUnit()->GetInstanceScript()->SetData(TIME_DATA, aureff->GetBase()->GetDuration());

        if (Unit* owner = GetOwner()->ToUnit())
        {
            owner->RemoveAurasDueToSpell(SPELL_THROVES_OF_THE_THUNDER_KING);
            owner->CastSpell(owner, SPELL_COMPLETE_SCENARIO_SCREEN_EFF, true);
        }
    }

    void HandleOnApply(AuraEffect const* aureff, AuraEffectHandleModes /*mode*/)
    {
        if (GetOwner() && GetOwner()->ToUnit() && GetOwner()->ToUnit()->GetInstanceScript() && GetOwner()->ToUnit()->GetInstanceScript()->GetData(TIME_DATA) > 0)
            SetDuration(GetOwner()->ToUnit()->GetInstanceScript()->GetData(TIME_DATA));
    }

    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_thunder_king_treasure_limited_time::HandleOnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectApply += AuraEffectApplyFn(spell_thunder_king_treasure_limited_time::HandleOnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// Scenario Completion Blackout Area 140005
class spell_thunder_king_treasure_scenario_completion_blackout : public AuraScript
{
    PrepareAuraScript(spell_thunder_king_treasure_scenario_completion_blackout);

    void HandleOnRemove(AuraEffect const* /*aureff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* owner = GetOwner()->ToPlayer())
        {
            if (owner->GetInstanceScript())
                owner->GetInstanceScript()->SetData(DATA_REACH_THE_EXIT, DONE);

            owner->NearTeleportTo(treasureRoomPos.GetPositionX(), treasureRoomPos.GetPositionY(), treasureRoomPos.GetPositionZ(), treasureRoomPos.GetOrientation());
        }
    }

    void Register() OVERRIDE
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_thunder_king_treasure_scenario_completion_blackout::HandleOnRemove, EFFECT_0, SPELL_AURA_SCREEN_EFFECT, AURA_EFFECT_HANDLE_REAL);
    }
};

// Lightning Surge 139804
class spell_thunder_king_treasure_lightning_surge : public SpellScript
{
    PrepareSpellScript(spell_thunder_king_treasure_lightning_surge);

    void SelectTargets(std::list<WorldObject*>&targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != NPC_LIGHTNING_PILLAR_TARGET_BUNNY; });
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_thunder_king_treasure_lightning_surge::SelectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_thunder_king_treasure_lightning_surge::SelectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
    }
};

// Lightning Surge Eff 140469
class spell_thunder_king_treasure_lightning_surge_eff : public SpellScript
{
    PrepareSpellScript(spell_thunder_king_treasure_lightning_surge_eff);

    void SelectTargets(std::list<WorldObject*>&targets)
    {
        targets.remove_if([=](WorldObject* target) { return target->GetEntry() != NPC_LIGHTNING_PILLAR_TARGET_BUNNY; });
    }

    void Register() OVERRIDE
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_thunder_king_treasure_lightning_surge_eff::SelectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
    }
};

// 851. Summoned by 139808 - Sentry
class sat_thunder_king_treasure_sentry : public IAreaTriggerAura
{
    bool CheckTriggering(WorldObject* pTriggering) OVERRIDE
    {
        return pTriggering && (pTriggering->ToPlayer() || pTriggering->GetEntry() == NPC_SENTRY_BEAM_BUNNY);
    }

    void OnTriggeringApply(WorldObject* pTriggering) OVERRIDE
    {
        if (Player* target = pTriggering->ToPlayer())
        {
            if (Creature* caster = GetCaster()->ToCreature())
            {
                caster->AI()->SetGUID(target->GetGUID());
                caster->CastSpell(caster, SPELL_SUMM_SENTRY_BEAM_BUNNY, true);
            }
        }
    }

    void OnTriggeringRemove(WorldObject* pTriggering) OVERRIDE
    {
        if (Creature* target = pTriggering->ToCreature())
            target->DespawnOrUnsummon();
    }
};

void AddSC_thunder_king_citadel()
{
    new creature_script<npc_lightning_pillar_master>("npc_lightning_pillar_master");
    new creature_script<npc_stone_sentinel>("npc_stone_sentinel");
    new creature_script<npc_thunder_king_treasure_sentry_totem>("npc_thunder_king_treasure_sentry_totem");
    new creature_script<npc_thunder_king_treasure_stasis_rune>("npc_thunder_king_treasure_stasis_rune");
    new creature_script<npc_thunder_king_treasure_speed_rune>("npc_thunder_king_treasure_speed_rune");
    new creature_script<npc_thunder_king_treasure_zandalari_arcweaver>("npc_thunder_king_treasure_zandalari_arcweaver");
    new creature_script<npc_thunder_king_treasure_sentry_beam_bunny>("npc_thunder_king_treasure_sentry_beam_bunny");
    new creature_script<npc_god_hulk_gulkan>("npc_god_hulk_gulkan");
    new creature_script<npc_zandalari_venomblade>("npc_zandalari_venomblade");
    new creature_script<npc_thunder_king_treasure_stone_watcher>("npc_thunder_king_treasure_stone_watcher");
    new npc_thunder_king_treasure_taoshi();
    new npc_thunder_king_treasure_tenwu_of_the_red_smoke();
    new npc_thunder_king_treasure_taoshi_queue();
    new go_thunder_king_treasure_lever();
    new go_thunder_king_treasure_mogu_chest();

    new GenericAuraScriptLoader<spell_thunder_king_treasure_rune_trap>("spell_thunder_king_treasure_rune_trap");
    new GenericAuraScriptLoader<spell_thunder_king_treasure_limited_time>("spell_thunder_king_treasure_limited_time");
    new GenericAuraScriptLoader<spell_thunder_king_treasure_scenario_completion_blackout>("spell_thunder_king_treasure_scenario_completion_blackout");
    new GenericSpellScriptLoader<spell_thunder_king_treasure_lightning_surge>("spell_thunder_king_treasure_lightning_surge");
    new GenericSpellScriptLoader<spell_thunder_king_treasure_lightning_surge_eff>("spell_thunder_king_treasure_lightning_surge_eff");
    new atrigger_script<sat_thunder_king_treasure_sentry>("sat_thunder_king_treasure_sentry");
}