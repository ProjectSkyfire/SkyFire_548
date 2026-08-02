/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "darkmoon_faire.h"
#include "Containers.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include <algorithm>
#include <list>

namespace DarkmoonWhackAGnoll
{
    Position const PlayPosition = { -3994.28f, 6283.58f, 13.12f, 0.727784f };

    uint32 const GnollTimer = 3 * IN_MILLISECONDS;
    uint32 const DollTimer = 6 * IN_MILLISECONDS;
    uint32 const BossTimer = 10 * IN_MILLISECONDS;
    uint32 const PositionTimer = IN_MILLISECONDS;

    float const ArenaRadius = 15.0f;
    float const PlayerScanRange = 30.0f;
    float const BarrelSearchRange = 50.0f;
    float const HammerRange = 3.0f;
    float const HammerArc = 1.5707963f;
    float const BarrelReleaseRange = 5.0f;

    char const* const GossipPlayText = "I want to play Whack-a-Gnoll! |cFF0000FF(Darkmoon Game Token)|r";

    bool IsWhackTarget(uint32 entry)
    {
        return entry == NPC_WHACK_GNOLL || entry == NPC_WHACK_DOLL || entry == NPC_WHACK_BOSS;
    }

    void CleanupPlayer(Player* player, bool castForbiddenZone)
    {
        if (!player)
            return;

        player->SetPower(POWER_ALTERNATE_POWER, 0);
        player->RemoveAurasDueToSpell(SPELL_WHACK_ENABLE_POWERBAR);
        player->RemoveAurasDueToSpell(SPELL_WHACK_DOLL_STUN);

        if (castForbiddenZone)
            player->CastSpell(player, SPELL_WHACK_FORBIDDEN_ZONE, true);
    }

    void RefreshPlayerScore(Player* player)
    {
        if (!player)
            return;

        player->SetPower(POWER_ALTERNATE_POWER, player->GetReqKillOrCastCurrentCount(QUEST_WHACK_A_GNOLL, NPC_WHACK_GNOLL_CREDIT));
    }

    void ResetPlayerScore(Player* player)
    {
        if (!player)
            return;

        player->ResetQuestObjectiveCounter(QUEST_WHACK_A_GNOLL, QUEST_OBJECTIVE_TYPE_NPC, NPC_WHACK_GNOLL_CREDIT);
        player->SetPower(POWER_ALTERNATE_POWER, 0);
    }

    void ReleaseNearestBarrel(Creature* target)
    {
        if (!target)
            return;

        std::list<Creature*> barrels;
        target->GetCreatureListWithEntryInGrid(barrels, NPC_WHACK_BARREL, BarrelReleaseRange);

        if (barrels.empty())
            return;

        Creature* closestBarrel = *std::min_element(barrels.begin(), barrels.end(), [target](Creature* left, Creature* right)
        {
            return target->GetExactDist2d(left) < target->GetExactDist2d(right);
        });

        if (closestBarrel)
            closestBarrel->RemoveAurasDueToSpell(SPELL_WHACK_OK_TO_HIT);
    }
}

class npc_whack_gnoll_bunny : public CreatureScript
{
public:
    npc_whack_gnoll_bunny() : CreatureScript("npc_whack_gnoll_bunny") { }

    struct npc_whack_gnoll_bunnyAI : public ScriptedAI
    {
        npc_whack_gnoll_bunnyAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
            RefreshBarrels();

            _events.Reset();
            _events.ScheduleEvent(EVENT_WHACK_SUMMON_GNOLL, DarkmoonWhackAGnoll::GnollTimer);
            _events.ScheduleEvent(EVENT_WHACK_SUMMON_DOLL, DarkmoonWhackAGnoll::DollTimer);
            _events.ScheduleEvent(EVENT_WHACK_SUMMON_BOSS, DarkmoonWhackAGnoll::BossTimer);
            _events.ScheduleEvent(EVENT_WHACK_CHECK_PLAYER_POSITION, DarkmoonWhackAGnoll::PositionTimer);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            if (_barrelGuids.size() < MAX_WHACK_BARRELS)
                RefreshBarrels();

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WHACK_SUMMON_GNOLL:
                        SummonFromBarrel(SPELL_WHACK_SUMMON_GNOLL);
                        _events.ScheduleEvent(EVENT_WHACK_SUMMON_GNOLL, DarkmoonWhackAGnoll::GnollTimer);
                        break;
                    case EVENT_WHACK_SUMMON_DOLL:
                        SummonFromBarrel(SPELL_WHACK_SUMMON_DOLL);
                        _events.ScheduleEvent(EVENT_WHACK_SUMMON_DOLL, DarkmoonWhackAGnoll::DollTimer);
                        break;
                    case EVENT_WHACK_SUMMON_BOSS:
                        SummonFromBarrel(SPELL_WHACK_SUMMON_BOSS);
                        _events.ScheduleEvent(EVENT_WHACK_SUMMON_BOSS, DarkmoonWhackAGnoll::BossTimer);
                        break;
                    case EVENT_WHACK_CHECK_PLAYER_POSITION:
                        CheckPlayers();
                        _events.ScheduleEvent(EVENT_WHACK_CHECK_PLAYER_POSITION, DarkmoonWhackAGnoll::PositionTimer);
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        void RefreshBarrels()
        {
            _barrelGuids.clear();

            std::list<Creature*> barrels;
            me->GetCreatureListWithEntryInGrid(barrels, NPC_WHACK_BARREL, DarkmoonWhackAGnoll::BarrelSearchRange);

            for (Creature* barrel : barrels)
                _barrelGuids.push_back(barrel->GetGUID());
        }

        Creature* GetRandomAvailableBarrel()
        {
            if (_barrelGuids.empty())
                return nullptr;

            std::list<uint64> candidates(_barrelGuids);

            while (!candidates.empty())
            {
                uint64 barrelGuid = Skyfire::Containers::SelectRandomContainerElement(candidates);
                Creature* barrel = ObjectAccessor::GetCreature(*me, barrelGuid);
                if (barrel && barrel->IsAlive() && !barrel->HasAura(SPELL_WHACK_OK_TO_HIT))
                    return barrel;

                candidates.remove(barrelGuid);
            }

            return nullptr;
        }

        void SummonFromBarrel(uint32 spellId)
        {
            Creature* barrel = GetRandomAvailableBarrel();
            if (!barrel)
                return;

            barrel->CastSpell(barrel, spellId, false);
            barrel->CastSpell(barrel, SPELL_WHACK_SPAWN_VISUAL, false);
            barrel->AddAura(SPELL_WHACK_OK_TO_HIT, barrel);
        }

        void CheckPlayers()
        {
            std::list<Player*> players;
            me->GetPlayerListInGrid(players, DarkmoonWhackAGnoll::PlayerScanRange);

            for (Player* player : players)
            {
                if (player->HasAura(SPELL_WHACK_ENABLE_POWERBAR))
                {
                    if (player->GetExactDist2d(me) >= DarkmoonWhackAGnoll::ArenaRadius)
                        player->NearTeleportTo(DarkmoonWhackAGnoll::PlayPosition.GetPositionX(), DarkmoonWhackAGnoll::PlayPosition.GetPositionY(), DarkmoonWhackAGnoll::PlayPosition.GetPositionZ(), DarkmoonWhackAGnoll::PlayPosition.GetOrientation(), true);
                }
                else if (player->GetExactDist2d(me) < DarkmoonWhackAGnoll::ArenaRadius)
                    player->CastSpell(player, SPELL_WHACK_FORBIDDEN_ZONE, true);
            }
        }

        EventMap _events;
        std::list<uint64> _barrelGuids;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_whack_gnoll_bunnyAI(creature);
    }
};

class npc_whack_gnoll_mola : public CreatureScript
{
public:
    npc_whack_gnoll_mola() : CreatureScript("npc_whack_gnoll_mola") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (player->GetQuestStatus(QUEST_WHACK_A_GNOLL) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, DarkmoonWhackAGnoll::GossipPlayText, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();

        if (action != GOSSIP_ACTION_INFO_DEF + 1)
        {
            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        if (!player->HasItemCount(ITEM_DARKMOON_GAME_TOKEN, 1, true))
        {
            player->GetSession()->SendNotification("%s", DarkmoonFaire::GetMissingGameTokenText());
            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        DarkmoonWhackAGnoll::CleanupPlayer(player, false);
        player->DestroyItemCount(ITEM_DARKMOON_GAME_TOKEN, 1, true);
        player->NearTeleportTo(DarkmoonWhackAGnoll::PlayPosition.GetPositionX(), DarkmoonWhackAGnoll::PlayPosition.GetPositionY(), DarkmoonWhackAGnoll::PlayPosition.GetPositionZ(), DarkmoonWhackAGnoll::PlayPosition.GetOrientation(), true);
        creature->AddAura(SPELL_WHACK_OVERRIDE_ACTION, player);
        creature->AddAura(SPELL_WHACK_ENABLE_POWERBAR, player);
        DarkmoonWhackAGnoll::ResetPlayerScore(player);
        player->CLOSE_GOSSIP_MENU();
        return true;
    }

    bool OnQuestComplete(Player* player, Creature* /*creature*/, Quest const* quest) override
    {
        if (quest && quest->GetQuestId() == QUEST_WHACK_A_GNOLL)
            DarkmoonWhackAGnoll::CleanupPlayer(player, false);

        return true;
    }
};

class npc_whack_gnoll_barrel : public CreatureScript
{
public:
    npc_whack_gnoll_barrel() : CreatureScript("npc_whack_gnoll_barrel") { }

    struct npc_whack_gnoll_barrelAI : public ScriptedAI
    {
        npc_whack_gnoll_barrelAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_whack_gnoll_barrelAI(creature);
    }
};

class npc_whack_gnoll_target : public CreatureScript
{
public:
    npc_whack_gnoll_target() : CreatureScript("npc_whack_gnoll_target") { }

    struct npc_whack_gnoll_targetAI : public ScriptedAI
    {
        npc_whack_gnoll_targetAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_whack_gnoll_targetAI(creature);
    }
};

class spell_whack_gnoll_whack : public SpellScriptLoader
{
public:
    spell_whack_gnoll_whack() : SpellScriptLoader("spell_whack_gnoll_whack") { }

    class spell_whack_gnoll_whack_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_whack_gnoll_whack_SpellScript);

        SpellCastResult CheckCast()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player)
                return SpellCastResult::SPELL_CAST_OK;

            std::list<Creature*> targets;
            player->GetCreatureListWithEntryInGrid(targets, NPC_WHACK_GNOLL, DarkmoonWhackAGnoll::HammerRange);
            player->GetCreatureListWithEntryInGrid(targets, NPC_WHACK_DOLL, DarkmoonWhackAGnoll::HammerRange);
            player->GetCreatureListWithEntryInGrid(targets, NPC_WHACK_BOSS, DarkmoonWhackAGnoll::HammerRange);

            targets.remove_if([player](Creature* target)
            {
                return !target || !target->IsAlive() || !DarkmoonWhackAGnoll::IsWhackTarget(target->GetEntry()) || !player->isInFront(target, DarkmoonWhackAGnoll::HammerArc);
            });

            if (targets.empty())
                return SpellCastResult::SPELL_CAST_OK;

            Creature* target = *std::min_element(targets.begin(), targets.end(), [player](Creature* left, Creature* right)
            {
                return player->GetExactDist2d(left) < player->GetExactDist2d(right);
            });

            switch (target->GetEntry())
            {
                case NPC_WHACK_GNOLL:
                    player->CastSpell(player, SPELL_WHACK_KILL_CREDIT, true);
                    break;
                case NPC_WHACK_DOLL:
                    player->CastSpell(player, SPELL_WHACK_DOLL_STUN, true);
                    break;
                case NPC_WHACK_BOSS:
                    player->CastSpell(player, SPELL_WHACK_KILL_CREDIT, true);
                    player->CastSpell(player, SPELL_WHACK_KILL_CREDIT, true);
                    player->CastSpell(player, SPELL_WHACK_KILL_CREDIT, true);
                    break;
                default:
                    break;
            }

            DarkmoonWhackAGnoll::ReleaseNearestBarrel(target);
            player->Kill(target);
            target->DespawnOrUnsummon(100);
            DarkmoonWhackAGnoll::RefreshPlayerScore(player);
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(spell_whack_gnoll_whack_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_whack_gnoll_whack_SpellScript();
    }
};

class spell_whack_gnoll_override_action : public SpellScriptLoader
{
public:
    spell_whack_gnoll_override_action() : SpellScriptLoader("spell_whack_gnoll_override_action") { }

    class spell_whack_gnoll_override_action_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_whack_gnoll_override_action_AuraScript);

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                DarkmoonWhackAGnoll::CleanupPlayer(player, true);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_whack_gnoll_override_action_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_whack_gnoll_override_action_AuraScript();
    }
};

class at_whack_a_gnoll_entrance : public AreaTriggerScript
{
public:
    at_whack_a_gnoll_entrance() : AreaTriggerScript("at_whack_a_gnoll_entrance") { }

    bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/) override
    {
        if (!player->HasAura(SPELL_WHACK_ENABLE_POWERBAR))
            player->CastSpell(player, SPELL_WHACK_FORBIDDEN_ZONE, true);

        return false;
    }
};

void AddSC_event_darkmoon_whack_a_gnoll()
{
    new npc_whack_gnoll_bunny();
    new npc_whack_gnoll_mola();
    new npc_whack_gnoll_barrel();
    new npc_whack_gnoll_target();
    new spell_whack_gnoll_whack();
    new spell_whack_gnoll_override_action();
    new at_whack_a_gnoll_entrance();
}
