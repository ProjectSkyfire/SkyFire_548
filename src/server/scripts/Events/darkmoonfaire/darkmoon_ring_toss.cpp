/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "darkmoon_faire.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SpellAuraDefines.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include <algorithm>
#include <list>

namespace
{
    uint32 const GameDuration = 60 * IN_MILLISECONDS;
    uint32 const PlayerCheckInterval = IN_MILLISECONDS;

    float const BoothRange = 65.0f;
    float const TurtleSearchRange = 45.0f;
    float const RingHitRadius = 2.75f;
    float const RingTossVisualSpeed = 12.0f;

    uint32 const SpellDarkmoonCannon = 102178;
    uint32 const SpellDarkmoonTonkCommander = 102121;
    uint32 const SpellShootingGalleryOverrideAction = 101871;

    char const* const GossipInstructionsText = "How does the Ring Toss work?";
    char const* const GossipPlayText = "I'm ready to toss rings! |cFF0000FF(Darkmoon Game Token)|r";
    char const* const GossipBackText = "Alright.";
    char const* const BoothBusyText = "Ring Toss is already in use.";

    bool HasDarkmoonGameAura(Player const* player)
    {
        return player
            && (player->HasAura(SPELL_WHACK_OVERRIDE_ACTION)
                || player->HasAura(SPELL_WHACK_ENABLE_POWERBAR)
                || player->HasAura(SpellDarkmoonCannon)
                || player->HasAura(SPELL_RING_TOSS_ACTION)
                || player->HasAura(SpellDarkmoonTonkCommander)
                || player->HasAura(SpellShootingGalleryOverrideAction));
    }

    uint32 ClampRingScore(uint32 score)
    {
        return score > RING_TOSS_TARGET_COUNT ? RING_TOSS_TARGET_COUNT : score;
    }

    uint32 GetRingScore(Player* player)
    {
        return player ? ClampRingScore(player->GetReqKillOrCastCurrentCount(QUEST_TARGET_TURTLE, NPC_RING_TOSS_CREDIT)) : 0;
    }

    uint32 GetRemainingRings(uint32 throwCount)
    {
        return throwCount >= RING_TOSS_MAX_THROWS ? 0 : RING_TOSS_MAX_THROWS - throwCount;
    }

    void SetRemainingRings(Player* player, uint32 throwCount)
    {
        if (!player)
            return;

        player->SetPower(POWER_ALTERNATE_POWER, GetRemainingRings(throwCount));
    }

    void ResetPlayerRingGame(Player* player)
    {
        if (!player)
            return;

        player->ResetQuestObjectiveCounter(QUEST_TARGET_TURTLE, QUEST_OBJECTIVE_TYPE_NPC, NPC_RING_TOSS_CREDIT);
        SetRemainingRings(player, 0);
    }

    Creature* GetClosestJessicaFor(Player* player)
    {
        return player ? GetClosestCreatureWithEntry(player, NPC_DARKMOON_JESSICA_ROGERS, BoothRange, true) : NULL;
    }

    float GetDistanceToDest2d(Creature const* creature, WorldLocation const* dest)
    {
        float const deltaX = creature->GetPositionX() - dest->GetPositionX();
        float const deltaY = creature->GetPositionY() - dest->GetPositionY();
        return deltaX * deltaX + deltaY * deltaY;
    }

    Creature* FindHitTurtle(Player* player, WorldLocation const* dest)
    {
        if (!player || !dest)
            return NULL;

        std::list<Creature*> turtles;
        player->GetCreatureListWithEntryInGrid(turtles, NPC_DARKMOON_DUBENKO, TurtleSearchRange);

        turtles.remove_if([dest](Creature* turtle)
        {
            return !turtle || !turtle->IsAlive() || GetDistanceToDest2d(turtle, dest) > RingHitRadius * RingHitRadius;
        });

        if (turtles.empty())
            return NULL;

        return *std::min_element(turtles.begin(), turtles.end(), [dest](Creature* left, Creature* right)
        {
            return GetDistanceToDest2d(left, dest) < GetDistanceToDest2d(right, dest);
        });
    }

    void PlayRingThrowVisual(Player* player, WorldLocation const* dest, Creature const* hitTurtle, SpellInfo const* spellInfo)
    {
        if (!player || !dest)
            return;

        uint32 visualId = DarkmoonRingToss::GetThrowSpellVisual(spellInfo ? spellInfo->SpellVisual[0] : 0);
        if (!visualId)
            return;

        if (hitTurtle)
        {
            player->SendPlaySpellVisual(visualId, hitTurtle->GetGUID(), RingTossVisualSpeed);
            return;
        }

        player->SendPlaySpellVisual(visualId, dest->GetPositionX(), dest->GetPositionY(),
            DarkmoonRingToss::GetThrowDestinationVisualZ(dest->GetPositionZ()), RingTossVisualSpeed);
    }
}

class npc_darkmoon_jessica_rogers : public CreatureScript
{
public:
    npc_darkmoon_jessica_rogers() : CreatureScript("npc_darkmoon_jessica_rogers") { }

    struct npc_darkmoon_jessica_rogersAI : public ScriptedAI
    {
        npc_darkmoon_jessica_rogersAI(Creature* creature) : ScriptedAI(creature),
            _activePlayerGuid(0), _throwCount(0)
        {
        }

        void Reset() override
        {
            _events.Reset();
            _activePlayerGuid = 0;
            _throwCount = 0;
        }

        void DoAction(int32 action) override
        {
            if (action == ACTION_RING_TOSS_END_GAME)
                EndGame(false);
            else if (action == ACTION_RING_TOSS_COMPLETE_GAME)
                EndGame(true);
        }

        uint32 GetData(uint32 id = 0) const override
        {
            return id == DATA_RING_TOSS_THROW_COUNT ? _throwCount : 0;
        }

        uint64 GetGUID(int32 id = 0) const override
        {
            return id == DATA_RING_TOSS_ACTIVE_PLAYER ? _activePlayerGuid : 0;
        }

        bool StartGame(Player* player)
        {
            if (!player || _activePlayerGuid)
                return false;

            player->RemoveAurasByType(SPELL_AURA_MOUNTED);
            player->RemoveAurasDueToSpell(SPELL_WHACK_OVERRIDE_ACTION);
            player->RemoveAurasDueToSpell(SPELL_WHACK_ENABLE_POWERBAR);
            player->RemoveAurasDueToSpell(SpellDarkmoonCannon);
            player->RemoveAurasDueToSpell(SPELL_RING_TOSS_ACTION);
            player->RemoveAurasDueToSpell(SpellDarkmoonTonkCommander);
            player->RemoveAurasDueToSpell(SpellShootingGalleryOverrideAction);

            _activePlayerGuid = player->GetGUID();
            _throwCount = 0;
            _events.Reset();

            player->AddAura(SPELL_RING_TOSS_ACTION, player);
            ResetPlayerRingGame(player);

            _events.ScheduleEvent(EVENT_RING_TOSS_FINISH_GAME, GameDuration);
            _events.ScheduleEvent(EVENT_RING_TOSS_CHECK_PLAYER, PlayerCheckInterval);
            return true;
        }

        bool RegisterThrow(Player* player)
        {
            if (!player || _activePlayerGuid != player->GetGUID())
                return false;

            ++_throwCount;
            SetRemainingRings(player, _throwCount);
            return true;
        }

        void CheckThrowLimit(Player* player)
        {
            if (!player || _activePlayerGuid != player->GetGUID())
                return;

            if (_throwCount >= RING_TOSS_MAX_THROWS && player->GetReqKillOrCastCurrentCount(QUEST_TARGET_TURTLE, NPC_RING_TOSS_CREDIT) < RING_TOSS_TARGET_COUNT)
                EndGame(true);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_RING_TOSS_FINISH_GAME:
                        EndGame(true);
                        break;
                    case EVENT_RING_TOSS_CHECK_PLAYER:
                        CheckPlayer();
                        if (_activePlayerGuid)
                            _events.ScheduleEvent(EVENT_RING_TOSS_CHECK_PLAYER, PlayerCheckInterval);
                        break;
                    default:
                        break;
                }
            }
        }

        void EndGame(bool removeAura)
        {
            uint64 playerGuid = _activePlayerGuid;
            _activePlayerGuid = 0;
            _throwCount = 0;
            _events.Reset();

            if (Player* player = ObjectAccessor::GetPlayer(*me, playerGuid))
            {
                player->SetPower(POWER_ALTERNATE_POWER, 0);
                if (removeAura)
                    player->RemoveAurasDueToSpell(SPELL_RING_TOSS_ACTION);
            }
        }

    private:
        void CheckPlayer()
        {
            Player* player = ObjectAccessor::GetPlayer(*me, _activePlayerGuid);
            if (!player || !player->IsAlive() || !player->HasAura(SPELL_RING_TOSS_ACTION) || player->GetExactDist2d(me) > BoothRange)
            {
                EndGame(false);
                return;
            }

            SetRemainingRings(player, _throwCount);

            if (GetRingScore(player) >= RING_TOSS_TARGET_COUNT)
                EndGame(true);
        }

        EventMap _events;
        uint64 _activePlayerGuid;
        uint32 _throwCount;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_darkmoon_jessica_rogersAI(creature);
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GossipInstructionsText, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if (player->GetQuestStatus(QUEST_TARGET_TURTLE) == QUEST_STATUS_INCOMPLETE && !HasDarkmoonGameAura(player))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GossipPlayText, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GossipBackText, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        if (action == GOSSIP_ACTION_INFO_DEF + 3)
            return OnGossipHello(player, creature);

        if (action != GOSSIP_ACTION_INFO_DEF + 2)
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

        npc_darkmoon_jessica_rogersAI* ai = CAST_AI(npc_darkmoon_jessica_rogers::npc_darkmoon_jessica_rogersAI, creature->AI());
        if (!ai || !ai->StartGame(player))
        {
            creature->MonsterWhisper(BoothBusyText, player);
            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        player->DestroyItemCount(ITEM_DARKMOON_GAME_TOKEN, 1, true);
        player->CLOSE_GOSSIP_MENU();
        return true;
    }

    bool OnQuestComplete(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest && quest->GetQuestId() == QUEST_TARGET_TURTLE)
        {
            player->SetPower(POWER_ALTERNATE_POWER, 0);
            player->RemoveAurasDueToSpell(SPELL_RING_TOSS_ACTION);

            if (npc_darkmoon_jessica_rogersAI* ai = CAST_AI(npc_darkmoon_jessica_rogers::npc_darkmoon_jessica_rogersAI, creature->AI()))
                if (ai->GetGUID(DATA_RING_TOSS_ACTIVE_PLAYER) == player->GetGUID())
                    ai->DoAction(ACTION_RING_TOSS_END_GAME);
        }

        return true;
    }
};

class spell_darkmoon_ring_toss_throw : public SpellScriptLoader
{
public:
    spell_darkmoon_ring_toss_throw() : SpellScriptLoader("spell_darkmoon_ring_toss_throw") { }

    class spell_darkmoon_ring_toss_throw_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_darkmoon_ring_toss_throw_SpellScript);

        void HandleAfterCast()
        {
            Player* player = GetCaster() ? GetCaster()->ToPlayer() : NULL;
            if (!player || !player->HasAura(SPELL_RING_TOSS_ACTION))
                return;

            WorldLocation const* dest = GetExplTargetDest();
            if (!dest)
                return;

            Creature* jessica = GetClosestJessicaFor(player);
            if (!jessica)
                return;

            npc_darkmoon_jessica_rogers::npc_darkmoon_jessica_rogersAI* ai = CAST_AI(npc_darkmoon_jessica_rogers::npc_darkmoon_jessica_rogersAI, jessica->AI());
            if (!ai || !ai->RegisterThrow(player))
                return;

            Creature* hitTurtle = FindHitTurtle(player, dest);
            PlayRingThrowVisual(player, dest, hitTurtle, GetSpellInfo());

            if (hitTurtle)
                AwardRingHit(player, ai);

            ai->CheckThrowLimit(player);
        }

        void AwardRingHit(Player* player, npc_darkmoon_jessica_rogers::npc_darkmoon_jessica_rogersAI* ai)
        {
            if (!player || !ai)
                return;

            uint32 currentScore = player->GetReqKillOrCastCurrentCount(QUEST_TARGET_TURTLE, NPC_RING_TOSS_CREDIT);
            if (currentScore >= RING_TOSS_TARGET_COUNT)
                return;

            player->KilledMonsterCredit(NPC_RING_TOSS_CREDIT);

            if (GetRingScore(player) >= RING_TOSS_TARGET_COUNT)
                ai->DoAction(ACTION_RING_TOSS_COMPLETE_GAME);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_darkmoon_ring_toss_throw_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_darkmoon_ring_toss_throw_SpellScript();
    }
};

class spell_darkmoon_ring_toss_action : public SpellScriptLoader
{
public:
    spell_darkmoon_ring_toss_action() : SpellScriptLoader("spell_darkmoon_ring_toss_action") { }

    class spell_darkmoon_ring_toss_action_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_darkmoon_ring_toss_action_AuraScript);

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* player = GetTarget() ? GetTarget()->ToPlayer() : NULL;
            if (!player)
                return;

            player->SetPower(POWER_ALTERNATE_POWER, 0);

            if (Creature* jessica = GetClosestJessicaFor(player))
                if (jessica->AI() && jessica->AI()->GetGUID(DATA_RING_TOSS_ACTIVE_PLAYER) == player->GetGUID())
                    jessica->AI()->DoAction(ACTION_RING_TOSS_END_GAME);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_darkmoon_ring_toss_action_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_darkmoon_ring_toss_action_AuraScript();
    }
};

void AddSC_event_darkmoon_ring_toss()
{
    new npc_darkmoon_jessica_rogers();
    new spell_darkmoon_ring_toss_throw();
    new spell_darkmoon_ring_toss_action();
}
