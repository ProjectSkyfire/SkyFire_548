/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "darkmoon_faire.h"
#include "Object.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SpellAuraDefines.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "TemporarySummon.h"
#include "Vehicle.h"
#include <algorithm>
#include <list>

namespace
{
    uint32 const GameDuration = 60 * IN_MILLISECONDS;
    uint32 const TargetWaveInterval = 30 * IN_MILLISECONDS;
    uint32 const TargetDespawnTime = 25 * IN_MILLISECONDS;
    uint32 const PlayerCheckInterval = IN_MILLISECONDS;
    uint32 const TonkPlacementDelay = 250;

    float const BoothRange = 110.0f;
    float const TonkSearchRange = 45.0f;
    float const TargetSearchRange = 65.0f;

    char const* const GossipInstructionsText = "How does the Tonk Challenge work?";
    char const* const GossipPlayText = "I'm ready to command a tonk! |cFF0000FF(Darkmoon Game Token)|r";
    char const* const GossipBackText = "Alright.";
    char const* const BoothBusyText = "The Tonk Challenge is already in use.";

    bool HasDarkmoonGameAura(Player const* player)
    {
        return player
            && (player->HasAura(SPELL_WHACK_OVERRIDE_ACTION)
                || player->HasAura(SPELL_WHACK_ENABLE_POWERBAR)
                || player->HasAura(SPELL_SHOOTING_GALLERY_OVERRIDE_ACTION)
                || player->HasAura(SPELL_RING_TOSS_ACTION)
                || player->HasAura(SPELL_DARKMOON_TONK_CONTROLLER)
                || player->HasAura(SPELL_DARKMOON_TONK_CANNON_BLAST)
                || player->HasAura(SPELL_DARKMOON_TONK_ACTION)
                || player->HasAura(SPELL_DARKMOON_TONK_TARGET_PARTICIPANT));
    }

    uint32 GetTonkScore(Player* player)
    {
        return player ? DarkmoonTonkChallenge::ClampTargetScore(player->GetReqKillOrCastCurrentCount(QUEST_TONK_COMMANDER, NPC_TONK_CANNON_TARGET), 0) : 0;
    }

    void RefreshPlayerScore(Player* player)
    {
        if (player)
            player->SetPower(POWER_ALTERNATE_POWER, GetTonkScore(player));
    }

    void ResetPlayerScore(Player* player)
    {
        if (!player)
            return;

        player->ResetQuestObjectiveCounter(QUEST_TONK_COMMANDER, QUEST_OBJECTIVE_TYPE_NPC, NPC_TONK_CANNON_TARGET);
        player->SetPower(POWER_ALTERNATE_POWER, 0);
    }

    bool IsPlayerTonk(Creature const* tonk, Player const* player)
    {
        if (!tonk || !player || tonk->GetEntry() != NPC_DARKMOON_STEAM_TONK)
            return false;

        if (tonk->GetCharmerGUID() == player->GetGUID() || tonk->GetOwnerGUID() == player->GetGUID() || tonk->GetCreatorGUID() == player->GetGUID())
            return true;

        if (TempSummon const* summon = tonk->ToTempSummon())
            return summon->GetSummonerGUID() == player->GetGUID();

        return false;
    }

    Player* GetTonkController(Creature* tonk)
    {
        if (!tonk)
            return NULL;

        if (Player* player = tonk->GetCharmerOrOwnerPlayerOrPlayerItself())
            if (IsPlayerTonk(tonk, player))
                return player;

        if (TempSummon const* summon = tonk->ToTempSummon())
            if (Player* player = ObjectAccessor::GetPlayer(*tonk, summon->GetSummonerGUID()))
                if (IsPlayerTonk(tonk, player))
                    return player;

        return NULL;
    }

    Position GetTonkStartPosition()
    {
        DarkmoonTonkChallenge::TargetPosition const& start = DarkmoonTonkChallenge::GetTonkStartPosition();
        Position startPosition = { start.X, start.Y, start.Z, start.Orientation };
        return startPosition;
    }

    Position GetTonkReturnPosition()
    {
        DarkmoonTonkChallenge::TargetPosition const& returnPosition = DarkmoonTonkChallenge::GetTonkReturnPosition();
        Position playerPosition = { returnPosition.X, returnPosition.Y, returnPosition.Z, returnPosition.Orientation };
        return playerPosition;
    }

    void MoveTonkToStart(Creature* tonk)
    {
        if (!tonk)
            return;

        Position const startPosition = GetTonkStartPosition();
        tonk->SetReactState(REACT_PASSIVE);
        tonk->SetHomePosition(startPosition);
        tonk->NearTeleportTo(startPosition.GetPositionX(), startPosition.GetPositionY(), startPosition.GetPositionZ(), startPosition.GetOrientation(), true);
    }

    bool EnsureTonkVehicleKit(Creature* tonk)
    {
        if (!tonk)
            return false;

        return tonk->IsVehicle() || tonk->CreateVehicleKit(DarkmoonTonkChallenge::TonkVehicleId, NPC_DARKMOON_STEAM_TONK);
    }

    void EnsurePlayerControlsTonk(Player* player, Creature* tonk)
    {
        if (!player || !EnsureTonkVehicleKit(tonk) || player->GetVehicleBase() == tonk)
            return;

        if (tonk->HasAuraTypeWithCaster(SPELL_AURA_CONTROL_VEHICLE, player->GetGUID()))
            return;

        tonk->SendSetVehicleRecId(DarkmoonTonkChallenge::TonkVehicleId);

        if (player->GetVehicle())
            player->ExitVehicle();

        player->EnterVehicle(tonk, 0);
    }

    Creature* FindPlayerTonk(Player* player)
    {
        if (!player)
            return NULL;

        if (Unit* vehicle = player->GetVehicleBase())
            if (Creature* tonk = vehicle->ToCreature())
                if (IsPlayerTonk(tonk, player))
                    return tonk;

        if (Unit* charm = player->GetCharm())
            if (Creature* tonk = charm->ToCreature())
                if (IsPlayerTonk(tonk, player))
                    return tonk;

        std::list<Creature*> tonks;
        player->GetCreatureListWithEntryInGrid(tonks, NPC_DARKMOON_STEAM_TONK, TonkSearchRange);

        tonks.remove_if([player](Creature* tonk)
        {
            return !IsPlayerTonk(tonk, player);
        });

        if (tonks.empty())
            return NULL;

        return *std::min_element(tonks.begin(), tonks.end(), [player](Creature const* left, Creature const* right)
        {
            return player->GetExactDist2d(left) < player->GetExactDist2d(right);
        });
    }

    Creature* GetClosestFinlayFor(WorldObject* object)
    {
        return object ? GetClosestCreatureWithEntry(object, NPC_DARKMOON_FINLAY_COOLSHOT, BoothRange, true) : NULL;
    }

    void RemovePlayerTonkAuras(Player* player)
    {
        if (!player)
            return;

        player->RemoveAurasDueToSpell(SPELL_DARKMOON_TONK_TARGET_TRACKER);
        player->RemoveAurasDueToSpell(SPELL_DARKMOON_TONK_TARGET_PARTICIPANT);
        player->RemoveAurasDueToSpell(SPELL_DARKMOON_TONK_CANNON_BLAST);
        player->RemoveAurasDueToSpell(SPELL_DARKMOON_TONK_ACTION);
        player->RemoveAurasDueToSpell(SPELL_DARKMOON_TONK_CONTROLLER);
    }

    void DespawnPlayerTonk(Player* player)
    {
        if (!player)
            return;

        Creature* tonk = FindPlayerTonk(player);

        if (player->GetVehicle())
            player->ExitVehicle();

        if (tonk)
            tonk->DespawnOrUnsummon(1);
    }

    void ReturnPlayerToTonkBooth(Player* player)
    {
        if (!player || player->GetMapId() != DarkmoonTonkChallenge::MapId)
            return;

        Position const returnPosition = GetTonkReturnPosition();
        player->NearTeleportTo(returnPosition.GetPositionX(), returnPosition.GetPositionY(), returnPosition.GetPositionZ(), returnPosition.GetOrientation(), true);
    }

    void EndActiveTonkChallenge(Player* player, WorldObject* source)
    {
        if (!player)
            return;

        Creature* finlay = GetClosestFinlayFor(source ? source : player);
        if (!finlay || !finlay->AI() || finlay->AI()->GetGUID(DATA_TONK_CHALLENGE_ACTIVE_PLAYER) != player->GetGUID())
            return;

        finlay->AI()->DoAction(ACTION_TONK_CHALLENGE_CANCEL_GAME);
    }

    void ClearTonkTargets(WorldObject* object)
    {
        if (!object)
            return;

        std::list<Creature*> targets;
        object->GetCreatureListWithEntryInGrid(targets, NPC_TONK_CANNON_TARGET, TargetSearchRange);

        for (Creature* target : targets)
            if (target && DarkmoonTonkChallenge::IsTonkTarget(target->GetEntry()))
                target->DespawnOrUnsummon(1);
    }
}

class npc_darkmoon_finlay_coolshot : public CreatureScript
{
public:
    npc_darkmoon_finlay_coolshot() : CreatureScript("npc_darkmoon_finlay_coolshot") { }

    struct npc_darkmoon_finlay_coolshotAI : public ScriptedAI
    {
        npc_darkmoon_finlay_coolshotAI(Creature* creature) : ScriptedAI(creature),
            _activePlayerGuid(0)
        {
        }

        void Reset() override
        {
            _events.Reset();
            ClearTonkTargets(me);
            _activePlayerGuid = 0;
        }

        void DoAction(int32 action) override
        {
            if (action == ACTION_TONK_CHALLENGE_END_GAME)
                EndGame(false);
            else if (action == ACTION_TONK_CHALLENGE_COMPLETE_GAME)
                EndGame(true);
            else if (action == ACTION_TONK_CHALLENGE_CANCEL_GAME && _activePlayerGuid)
                _events.ScheduleEvent(EVENT_TONK_CHALLENGE_CANCEL_GAME, 1);
        }

        uint64 GetGUID(int32 id = 0) const override
        {
            return id == DATA_TONK_CHALLENGE_ACTIVE_PLAYER ? _activePlayerGuid : 0;
        }

        bool StartGame(Player* player)
        {
            if (!player || _activePlayerGuid)
                return false;

            _activePlayerGuid = player->GetGUID();
            _events.Reset();
            ClearTonkTargets(me);

            player->RemoveAurasByType(SPELL_AURA_MOUNTED);
            player->RemoveAurasDueToSpell(SPELL_WHACK_OVERRIDE_ACTION);
            player->RemoveAurasDueToSpell(SPELL_WHACK_ENABLE_POWERBAR);
            player->RemoveAurasDueToSpell(SPELL_SHOOTING_GALLERY_OVERRIDE_ACTION);
            player->RemoveAurasDueToSpell(SPELL_RING_TOSS_ACTION);
            RemovePlayerTonkAuras(player);
            ResetPlayerScore(player);

            player->CastSpell(player, SPELL_DARKMOON_TONK_CONTROLLER, true);

            _events.ScheduleEvent(EVENT_TONK_CHALLENGE_PLACE_TONK, TonkPlacementDelay);
            _events.ScheduleEvent(EVENT_TONK_CHALLENGE_START_WAVE, IN_MILLISECONDS);
            _events.ScheduleEvent(EVENT_TONK_CHALLENGE_FINISH_GAME, GameDuration);
            _events.ScheduleEvent(EVENT_TONK_CHALLENGE_CHECK_PLAYER, PlayerCheckInterval);
            return true;
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_TONK_CHALLENGE_PLACE_TONK:
                        if (!PlacePlayerTonk() && _activePlayerGuid)
                            _events.ScheduleEvent(EVENT_TONK_CHALLENGE_PLACE_TONK, TonkPlacementDelay);
                        break;
                    case EVENT_TONK_CHALLENGE_START_WAVE:
                        SpawnTargetWave();
                        if (_activePlayerGuid)
                            _events.ScheduleEvent(EVENT_TONK_CHALLENGE_START_WAVE, TargetWaveInterval);
                        break;
                    case EVENT_TONK_CHALLENGE_FINISH_GAME:
                        EndGame(true);
                        break;
                    case EVENT_TONK_CHALLENGE_CHECK_PLAYER:
                        CheckPlayer();
                        if (_activePlayerGuid)
                            _events.ScheduleEvent(EVENT_TONK_CHALLENGE_CHECK_PLAYER, PlayerCheckInterval);
                        break;
                    case EVENT_TONK_CHALLENGE_CANCEL_GAME:
                        EndGame(true);
                        break;
                    default:
                        break;
                }
            }
        }

        void EndGame(bool removeAuras)
        {
            uint64 playerGuid = _activePlayerGuid;
            _activePlayerGuid = 0;
            _events.Reset();
            ClearTonkTargets(me);

            if (Player* player = ObjectAccessor::GetPlayer(*me, playerGuid))
            {
                player->SetPower(POWER_ALTERNATE_POWER, 0);

                if (removeAuras)
                    RemovePlayerTonkAuras(player);

                DespawnPlayerTonk(player);
                ReturnPlayerToTonkBooth(player);
            }
        }

    private:
        bool PlacePlayerTonk()
        {
            Player* player = ObjectAccessor::GetPlayer(*me, _activePlayerGuid);
            if (!player)
                return false;

            if (Creature* tonk = FindPlayerTonk(player))
            {
                if (player->GetVehicleBase() != tonk)
                    MoveTonkToStart(tonk);

                EnsurePlayerControlsTonk(player, tonk);
                return true;
            }

            return false;
        }

        void CheckPlayer()
        {
            Player* player = ObjectAccessor::GetPlayer(*me, _activePlayerGuid);
            if (!player || !player->IsAlive() || player->GetQuestStatus(QUEST_TONK_COMMANDER) != QUEST_STATUS_INCOMPLETE || player->GetExactDist2d(me) > BoothRange)
            {
                EndGame(true);
                return;
            }

            RefreshPlayerScore(player);

            if (DarkmoonTonkChallenge::IsCompleteScore(GetTonkScore(player)))
                EndGame(true);
        }

        void SpawnTargetWave()
        {
            if (!_activePlayerGuid)
                return;

            ClearTonkTargets(me);

            DarkmoonTonkChallenge::TargetPosition const* positions = DarkmoonTonkChallenge::GetTargetPositions();
            for (uint32 i = 0; i < DarkmoonTonkChallenge::TargetPositionCount; ++i)
            {
                Position position = { positions[i].X, positions[i].Y, positions[i].Z, positions[i].Orientation };
                Creature* target = me->SummonCreature(NPC_TONK_CANNON_TARGET, position, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, TargetDespawnTime);
                if (!target)
                    continue;

                target->SetReactState(REACT_PASSIVE);
                target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

                if (target->AI())
                    target->AI()->SetGUID(_activePlayerGuid, DATA_TONK_CHALLENGE_ACTIVE_PLAYER);
            }
        }

        EventMap _events;
        uint64 _activePlayerGuid;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_darkmoon_finlay_coolshotAI(creature);
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GossipInstructionsText, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if (player->GetQuestStatus(QUEST_TONK_COMMANDER) == QUEST_STATUS_INCOMPLETE && !HasDarkmoonGameAura(player))
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

        npc_darkmoon_finlay_coolshotAI* ai = CAST_AI(npc_darkmoon_finlay_coolshot::npc_darkmoon_finlay_coolshotAI, creature->AI());
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
        if (quest && quest->GetQuestId() == QUEST_TONK_COMMANDER)
        {
            player->SetPower(POWER_ALTERNATE_POWER, 0);
            RemovePlayerTonkAuras(player);
            DespawnPlayerTonk(player);

            if (npc_darkmoon_finlay_coolshotAI* ai = CAST_AI(npc_darkmoon_finlay_coolshot::npc_darkmoon_finlay_coolshotAI, creature->AI()))
                if (ai->GetGUID(DATA_TONK_CHALLENGE_ACTIVE_PLAYER) == player->GetGUID())
                    ai->DoAction(ACTION_TONK_CHALLENGE_END_GAME);
        }

        return true;
    }
};

class npc_darkmoon_steam_tonk : public CreatureScript
{
public:
    npc_darkmoon_steam_tonk() : CreatureScript("npc_darkmoon_steam_tonk") { }

    struct npc_darkmoon_steam_tonkAI : public ScriptedAI
    {
        npc_darkmoon_steam_tonkAI(Creature* creature) : ScriptedAI(creature), _activePlayerGuid(0), _challengeEnding(false) { }

        void Reset() override
        {
            _activePlayerGuid = 0;
            _challengeEnding = false;
            EnsureTonkVehicleKit(me);
            me->SetReactState(REACT_PASSIVE);
        }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) override
        {
            if (!damage || damage < me->GetHealth())
                return;

            EndTonkChallengeOnLoss();
        }

        void JustDied(Unit* /*killer*/) override
        {
            EndTonkChallengeOnLoss();
        }

        void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) override
        {
            Player* player = passenger ? passenger->ToPlayer() : NULL;
            if (!player)
                return;

            if (apply)
            {
                _activePlayerGuid = player->GetGUID();
                return;
            }

            if (!_activePlayerGuid)
                _activePlayerGuid = player->GetGUID();

            if (_challengeEnding)
                return;

            _challengeEnding = true;
            EndActiveTonkChallenge(player, me);
        }

        void EnterCombat(Unit* /*who*/) override { }

    private:
        void EndTonkChallengeOnLoss()
        {
            if (_challengeEnding)
                return;

            Player* player = _activePlayerGuid ? ObjectAccessor::GetPlayer(*me, _activePlayerGuid) : NULL;
            if (!player)
                player = GetTonkController(me);

            if (!player)
                return;

            _challengeEnding = true;
            EndActiveTonkChallenge(player, me);
        }

        uint64 _activePlayerGuid;
        bool _challengeEnding;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_darkmoon_steam_tonkAI(creature);
    }
};

class npc_darkmoon_tonk_cannon_target : public CreatureScript
{
public:
    npc_darkmoon_tonk_cannon_target() : CreatureScript("npc_darkmoon_tonk_cannon_target") { }

    struct npc_darkmoon_tonk_cannon_targetAI : public ScriptedAI
    {
        npc_darkmoon_tonk_cannon_targetAI(Creature* creature) : ScriptedAI(creature),
            _activePlayerGuid(0), _creditAwarded(false)
        {
        }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
            _creditAwarded = false;
        }

        void EnterCombat(Unit* /*who*/) override { }

        void DamageTaken(Unit* /*attacker*/, uint32& damage) override
        {
            damage = 0;
        }

        void SetGUID(uint64 guid, int32 id = 0) override
        {
            if (id == DATA_TONK_CHALLENGE_ACTIVE_PLAYER)
                _activePlayerGuid = guid;
        }

        uint64 GetGUID(int32 id = 0) const override
        {
            return id == DATA_TONK_CHALLENGE_ACTIVE_PLAYER ? _activePlayerGuid : 0;
        }

        void SpellHit(Unit* caster, SpellInfo const* spellInfo) override
        {
            if (_creditAwarded || !caster || !spellInfo || !DarkmoonTonkChallenge::IsTonkHitSpell(spellInfo->Id))
                return;

            Player* player = caster->GetCharmerOrOwnerPlayerOrPlayerItself();
            if (!player || player->GetGUID() != _activePlayerGuid || player->GetQuestStatus(QUEST_TONK_COMMANDER) != QUEST_STATUS_INCOMPLETE)
                return;

            Creature* finlay = GetClosestFinlayFor(me);
            if (!finlay || !finlay->AI() || finlay->AI()->GetGUID(DATA_TONK_CHALLENGE_ACTIVE_PLAYER) != player->GetGUID())
                return;

            _creditAwarded = true;

            uint32 currentScore = GetTonkScore(player);
            if (currentScore < TONK_CHALLENGE_TARGET_COUNT)
                player->KilledMonsterCredit(NPC_TONK_CANNON_TARGET);

            RefreshPlayerScore(player);

            if (DarkmoonTonkChallenge::IsCompleteScore(GetTonkScore(player)))
                finlay->AI()->DoAction(ACTION_TONK_CHALLENGE_COMPLETE_GAME);

            me->DespawnOrUnsummon(1);
        }

    private:
        uint64 _activePlayerGuid;
        bool _creditAwarded;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_darkmoon_tonk_cannon_targetAI(creature);
    }
};

class spell_darkmoon_tonk_controller : public SpellScriptLoader
{
public:
    spell_darkmoon_tonk_controller() : SpellScriptLoader("spell_darkmoon_tonk_controller") { }

    class spell_darkmoon_tonk_controller_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_darkmoon_tonk_controller_SpellScript);

        void HandleSummonPosition(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            if (!caster || caster->GetMapId() != DarkmoonTonkChallenge::MapId)
                return;

            DarkmoonTonkChallenge::TargetPosition const& start = DarkmoonTonkChallenge::GetTonkStartPosition();
            WorldLocation summonPos(DarkmoonTonkChallenge::MapId, start.X, start.Y, start.Z, start.Orientation);

            SetExplTargetDest(summonPos);

            if (WorldLocation* hitDest = GetHitDest())
                hitDest->WorldRelocate(summonPos);
        }

        void Register() override
        {
            OnEffectHit += SpellEffectFn(spell_darkmoon_tonk_controller_SpellScript::HandleSummonPosition, EFFECT_ALL, SPELL_EFFECT_SUMMON);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_darkmoon_tonk_controller_SpellScript();
    }
};

void AddSC_event_darkmoon_tonk_challenge()
{
    new npc_darkmoon_finlay_coolshot();
    new npc_darkmoon_steam_tonk();
    new npc_darkmoon_tonk_cannon_target();
    new spell_darkmoon_tonk_controller();
}
