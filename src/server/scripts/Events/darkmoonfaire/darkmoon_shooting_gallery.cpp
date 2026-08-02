/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "darkmoon_faire.h"
#include "AchievementMgr.h"
#include "Containers.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SpellAuraDefines.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "Timer.h"
#include <algorithm>
#include <cstdlib>
#include <list>

namespace
{
    uint32 const GameDuration = 60 * IN_MILLISECONDS;
    uint32 const TargetWaveInterval = 5 * IN_MILLISECONDS;
    uint32 const PlayerCheckInterval = IN_MILLISECONDS;

    float const ShotRange = 60.0f;
    float const ShotArc = 0.87266463f;
    float const BoothRange = 70.0f;
    float const TargetSearchRange = 20.0f;

    uint32 const SpellDarkmoonCannon = 102178;
    uint32 const SpellDarkmoonRingToss = 102058;
    uint32 const SpellDarkmoonTonkCommander = 102121;

    char const* const GossipInstructionsText = "How does the shooting gallery work?";
    char const* const GossipPlayText = "I'm ready to shoot! |cFF0000FF(Darkmoon Game Token)|r";
    char const* const GossipBackText = "Alright.";
    char const* const BoothBusyText = "The Shooting Gallery is already in use.";

    bool HasDarkmoonGameAura(Player const* player)
    {
        return player
            && (player->HasAura(SPELL_WHACK_OVERRIDE_ACTION)
                || player->HasAura(SPELL_WHACK_ENABLE_POWERBAR)
                || player->HasAura(SpellDarkmoonCannon)
                || player->HasAura(SpellDarkmoonRingToss)
                || player->HasAura(SpellDarkmoonTonkCommander)
                || player->HasAura(SPELL_SHOOTING_GALLERY_OVERRIDE_ACTION));
    }

    void RefreshPlayerScore(Player* player)
    {
        if (!player)
            return;

        uint32 score = player->GetReqKillOrCastCurrentCount(QUEST_SHOOTING_GALLERY, NPC_SHOOTING_GALLERY_TARGET);
        player->SetPower(POWER_ALTERNATE_POWER, DarkmoonShootingGallery::ClampTargetScore(score, 0));
    }

    void ResetPlayerScore(Player* player)
    {
        if (!player)
            return;

        player->ResetQuestObjectiveCounter(QUEST_SHOOTING_GALLERY, QUEST_OBJECTIVE_TYPE_NPC, NPC_SHOOTING_GALLERY_TARGET);
        player->SetPower(POWER_ALTERNATE_POWER, 0);
    }

    Creature* GetClosestRinlingFor(Player* player)
    {
        return player ? GetClosestCreatureWithEntry(player, NPC_DARKMOON_RINLING, BoothRange, true) : nullptr;
    }

    float GetTargetPositionDistance2d(Creature const* target, DarkmoonShootingGallery::TargetPosition const& position)
    {
        float const deltaX = target->GetPositionX() - position.X;
        float const deltaY = target->GetPositionY() - position.Y;
        return deltaX * deltaX + deltaY * deltaY;
    }

    uint32 GetClosestTargetPositionIndex(Creature const* target)
    {
        uint32 closestIndex = 0;
        float closestDistance = GetTargetPositionDistance2d(target, DarkmoonShootingGallery::GetTargetPositions()[0]);

        for (uint32 i = 1; i < DarkmoonShootingGallery::TargetPositionCount; ++i)
        {
            float const distance = GetTargetPositionDistance2d(target, DarkmoonShootingGallery::GetTargetPositions()[i]);
            if (distance < closestDistance)
            {
                closestIndex = i;
                closestDistance = distance;
            }
        }

        return closestIndex;
    }

    bool IsGalleryTargetPlacement(Creature const* target)
    {
        if (!target)
            return false;

        for (uint32 i = 0; i < DarkmoonShootingGallery::TargetPositionCount; ++i)
            if (GetTargetPositionDistance2d(target, DarkmoonShootingGallery::GetTargetPositions()[i]) <= 1.0f)
                return true;

        return false;
    }

    void ClearTargetState(Creature* target)
    {
        if (!target)
            return;

        target->RemoveAurasDueToSpell(SPELL_SHOOTING_GALLERY_TARGET_INDICATOR);

        if (target->AI())
        {
            target->AI()->SetGUID(0, DATA_SHOOTING_GALLERY_ACTIVE_PLAYER);
            target->AI()->SetData(DATA_SHOOTING_GALLERY_ACTIVE_TARGET, 0);
            target->AI()->SetData(DATA_SHOOTING_GALLERY_HIT_COUNT, 0);
        }
    }

    void GetGalleryTargets(Creature* rinling, std::list<Creature*>& targets)
    {
        if (!rinling)
            return;

        rinling->GetCreatureListWithEntryInGrid(targets, NPC_SHOOTING_GALLERY_TARGET, TargetSearchRange);
        targets.remove_if([](Creature* target)
        {
            return !target || !target->IsAlive() || !IsGalleryTargetPlacement(target);
        });

        targets.sort([](Creature const* left, Creature const* right)
        {
            return GetClosestTargetPositionIndex(left) < GetClosestTargetPositionIndex(right);
        });
    }

    void ClearGalleryTargets(Creature* rinling)
    {
        std::list<Creature*> targets;
        GetGalleryTargets(rinling, targets);

        for (Creature* target : targets)
            ClearTargetState(target);
    }

    void ActivateTarget(Creature* target, uint64 playerGuid)
    {
        if (!target)
            return;

        ClearTargetState(target);

        if (target->AI())
        {
            target->AI()->SetGUID(playerGuid, DATA_SHOOTING_GALLERY_ACTIVE_PLAYER);
            target->AI()->SetData(DATA_SHOOTING_GALLERY_ACTIVE_TARGET, 1);
        }

        target->CastSpell(target, SPELL_SHOOTING_GALLERY_TARGET_INDICATOR, false);
    }

    void CompleteQuickShotAchievement(Player* player)
    {
        if (!player || player->HasAchieved(ACHIEVEMENT_QUICK_SHOT))
            return;

        if (AchievementEntry const* achievement = sAchievementMgr->GetAchievement(ACHIEVEMENT_QUICK_SHOT))
            player->CompletedAchievement(achievement);
    }

    Creature* FindActiveTarget(Player* player)
    {
        if (!player)
            return nullptr;

        std::list<Creature*> targets;
        player->GetCreatureListWithEntryInGrid(targets, NPC_SHOOTING_GALLERY_TARGET, ShotRange);

        targets.remove_if([player](Creature* target)
        {
            return !target
                || !target->IsAlive()
                || !target->AI()
                || target->AI()->GetGUID(DATA_SHOOTING_GALLERY_ACTIVE_PLAYER) != player->GetGUID()
                || !DarkmoonShootingGallery::CanAwardTargetCredit(target->GetEntry(), target->AI()->GetData(DATA_SHOOTING_GALLERY_ACTIVE_TARGET) != 0)
                || !player->isInFront(target, ShotArc);
        });

        if (targets.empty())
            return nullptr;

        return *std::min_element(targets.begin(), targets.end(), [player](Creature* left, Creature* right)
        {
            return player->GetExactDist2d(left) < player->GetExactDist2d(right);
        });
    }

    void AwardTargetHit(Player* player, Creature* target)
    {
        if (!player || !target)
            return;

        uint32 currentScore = player->GetReqKillOrCastCurrentCount(QUEST_SHOOTING_GALLERY, NPC_SHOOTING_GALLERY_TARGET);
        if (currentScore >= SHOOTING_GALLERY_TARGET_COUNT)
        {
            ClearTargetState(target);
            return;
        }

        uint32 spawnTime = target->AI() ? target->AI()->GetData(DATA_SHOOTING_GALLERY_SPAWN_TIME) : getMSTime();
        uint32 creditCount = DarkmoonShootingGallery::GetHitCreditCount(getMSTimeDiff(spawnTime, getMSTime()));
        uint32 newScore = DarkmoonShootingGallery::ClampTargetScore(currentScore, creditCount);

        for (uint32 score = currentScore; score < newScore; ++score)
            player->KilledMonsterCredit(NPC_SHOOTING_GALLERY_TARGET);

        player->SetPower(POWER_ALTERNATE_POWER, newScore);

        if (creditCount > 1)
            CompleteQuickShotAchievement(player);

        uint32 hitCount = target->AI() ? target->AI()->GetData(DATA_SHOOTING_GALLERY_HIT_COUNT) + 1 : SHOOTING_GALLERY_MAX_HITS_PER_TARGET;
        if (target->AI())
            target->AI()->SetData(DATA_SHOOTING_GALLERY_HIT_COUNT, hitCount);

        if (hitCount >= SHOOTING_GALLERY_MAX_HITS_PER_TARGET || newScore >= SHOOTING_GALLERY_TARGET_COUNT)
            ClearTargetState(target);

        if (DarkmoonShootingGallery::IsCompleteScore(newScore))
            if (Creature* rinling = GetClosestRinlingFor(player))
                if (rinling->AI() && rinling->AI()->GetGUID(DATA_SHOOTING_GALLERY_ACTIVE_PLAYER) == player->GetGUID())
                    rinling->AI()->DoAction(ACTION_SHOOTING_GALLERY_COMPLETE_GAME);
    }
}

class npc_darkmoon_rinling : public CreatureScript
{
public:
    npc_darkmoon_rinling() : CreatureScript("npc_darkmoon_rinling") { }

    struct npc_darkmoon_rinlingAI : public ScriptedAI
    {
        npc_darkmoon_rinlingAI(Creature* creature) : ScriptedAI(creature),
            _activePlayerGuid(0)
        {
        }

        void Reset() override
        {
            _events.Reset();
            ClearGalleryTargets(me);
            _activePlayerGuid = 0;
        }

        void DoAction(int32 action) override
        {
            if (action == ACTION_SHOOTING_GALLERY_END_GAME)
                EndGame(false);
            else if (action == ACTION_SHOOTING_GALLERY_COMPLETE_GAME)
                EndGame(true);
        }

        uint64 GetGUID(int32 id = 0) const override
        {
            return id == DATA_SHOOTING_GALLERY_ACTIVE_PLAYER ? _activePlayerGuid : 0;
        }

        bool StartGame(Player* player)
        {
            if (!player || _activePlayerGuid)
                return false;

            _activePlayerGuid = player->GetGUID();
            _events.Reset();
            ClearGalleryTargets(me);

            player->RemoveAurasByType(SPELL_AURA_MOUNTED);
            player->RemoveAurasDueToSpell(SPELL_WHACK_OVERRIDE_ACTION);
            player->RemoveAurasDueToSpell(SPELL_WHACK_ENABLE_POWERBAR);
            player->RemoveAurasDueToSpell(SpellDarkmoonCannon);
            player->RemoveAurasDueToSpell(SpellDarkmoonRingToss);
            player->RemoveAurasDueToSpell(SpellDarkmoonTonkCommander);
            player->RemoveAurasDueToSpell(SPELL_SHOOTING_GALLERY_OVERRIDE_ACTION);
            player->AddAura(SPELL_SHOOTING_GALLERY_OVERRIDE_ACTION, player);
            ResetPlayerScore(player);

            _events.ScheduleEvent(EVENT_SHOOTING_GALLERY_START_WAVE, 0);
            _events.ScheduleEvent(EVENT_SHOOTING_GALLERY_FINISH_GAME, GameDuration);
            _events.ScheduleEvent(EVENT_SHOOTING_GALLERY_CHECK_PLAYER, PlayerCheckInterval);
            return true;
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SHOOTING_GALLERY_START_WAVE:
                        SpawnTargetWave();
                        _events.ScheduleEvent(EVENT_SHOOTING_GALLERY_START_WAVE, TargetWaveInterval);
                        break;
                    case EVENT_SHOOTING_GALLERY_FINISH_GAME:
                        EndGame(true);
                        break;
                    case EVENT_SHOOTING_GALLERY_CHECK_PLAYER:
                        CheckPlayer();
                        if (_activePlayerGuid)
                            _events.ScheduleEvent(EVENT_SHOOTING_GALLERY_CHECK_PLAYER, PlayerCheckInterval);
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
            _events.Reset();
            ClearGalleryTargets(me);

            if (Player* player = ObjectAccessor::GetPlayer(*me, playerGuid))
            {
                player->SetPower(POWER_ALTERNATE_POWER, 0);
                if (removeAura)
                    player->RemoveAurasDueToSpell(SPELL_SHOOTING_GALLERY_OVERRIDE_ACTION);
            }
        }

    private:
        void CheckPlayer()
        {
            Player* player = ObjectAccessor::GetPlayer(*me, _activePlayerGuid);
            if (!player || !player->IsAlive() || !player->HasAura(SPELL_SHOOTING_GALLERY_OVERRIDE_ACTION) || player->GetExactDist2d(me) > BoothRange)
            {
                EndGame(false);
                return;
            }

            RefreshPlayerScore(player);

            if (DarkmoonShootingGallery::IsCompleteScore(player->GetReqKillOrCastCurrentCount(QUEST_SHOOTING_GALLERY, NPC_SHOOTING_GALLERY_TARGET)))
                EndGame(true);
        }

        void SpawnTargetWave()
        {
            if (!_activePlayerGuid)
                return;

            std::list<Creature*> targets;
            GetGalleryTargets(me, targets);
            if (targets.empty())
                return;

            uint32 activeIndex = std::rand() % targets.size();
            uint32 currentIndex = 0;

            for (Creature* target : targets)
            {
                if (currentIndex++ == activeIndex)
                    ActivateTarget(target, _activePlayerGuid);
                else
                    ClearTargetState(target);
            }
        }

        EventMap _events;
        uint64 _activePlayerGuid;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_darkmoon_rinlingAI(creature);
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GossipInstructionsText, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        if (player->GetQuestStatus(QUEST_SHOOTING_GALLERY) == QUEST_STATUS_INCOMPLETE && !HasDarkmoonGameAura(player))
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

        npc_darkmoon_rinlingAI* ai = CAST_AI(npc_darkmoon_rinling::npc_darkmoon_rinlingAI, creature->AI());
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
        if (quest && quest->GetQuestId() == QUEST_SHOOTING_GALLERY)
        {
            player->SetPower(POWER_ALTERNATE_POWER, 0);
            player->RemoveAurasDueToSpell(SPELL_SHOOTING_GALLERY_OVERRIDE_ACTION);

            if (npc_darkmoon_rinlingAI* ai = CAST_AI(npc_darkmoon_rinling::npc_darkmoon_rinlingAI, creature->AI()))
                if (ai->GetGUID(DATA_SHOOTING_GALLERY_ACTIVE_PLAYER) == player->GetGUID())
                    ai->DoAction(ACTION_SHOOTING_GALLERY_END_GAME);
        }

        return true;
    }
};

class npc_darkmoon_shooting_gallery_target : public CreatureScript
{
public:
    npc_darkmoon_shooting_gallery_target() : CreatureScript("npc_darkmoon_shooting_gallery_target") { }

    struct npc_darkmoon_shooting_gallery_targetAI : public ScriptedAI
    {
        npc_darkmoon_shooting_gallery_targetAI(Creature* creature) : ScriptedAI(creature),
            _activePlayerGuid(0), _spawnTime(getMSTime()), _hitCount(0), _active(false)
        {
        }

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_IMMUNE_TO_NPC);
            me->RemoveAurasDueToSpell(SPELL_SHOOTING_GALLERY_TARGET_INDICATOR);
            _activePlayerGuid = 0;
            _spawnTime = getMSTime();
            _hitCount = 0;
            _active = false;
        }

        uint32 GetData(uint32 id = 0) const override
        {
            switch (id)
            {
                case DATA_SHOOTING_GALLERY_SPAWN_TIME:
                    return _spawnTime;
                case DATA_SHOOTING_GALLERY_HIT_COUNT:
                    return _hitCount;
                case DATA_SHOOTING_GALLERY_ACTIVE_TARGET:
                    return _active ? 1 : 0;
                default:
                    return 0;
            }
        }

        void SetGUID(uint64 guid, int32 id = 0) override
        {
            if (id == DATA_SHOOTING_GALLERY_ACTIVE_PLAYER)
                _activePlayerGuid = guid;
        }

        uint64 GetGUID(int32 id = 0) const override
        {
            return id == DATA_SHOOTING_GALLERY_ACTIVE_PLAYER ? _activePlayerGuid : 0;
        }

        void SetData(uint32 id, uint32 value) override
        {
            if (id == DATA_SHOOTING_GALLERY_HIT_COUNT)
                _hitCount = value;
            else if (id == DATA_SHOOTING_GALLERY_ACTIVE_TARGET)
            {
                _active = value != 0;
                _spawnTime = getMSTime();
                _hitCount = 0;
                if (!_active)
                    _activePlayerGuid = 0;
            }
        }

    private:
        uint64 _activePlayerGuid;
        uint32 _spawnTime;
        uint32 _hitCount;
        bool _active;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_darkmoon_shooting_gallery_targetAI(creature);
    }
};

class spell_darkmoon_shooting_gallery_shoot : public SpellScriptLoader
{
public:
    spell_darkmoon_shooting_gallery_shoot() : SpellScriptLoader("spell_darkmoon_shooting_gallery_shoot") { }

    class spell_darkmoon_shooting_gallery_shoot_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_darkmoon_shooting_gallery_shoot_SpellScript);

        void HandleAfterCast()
        {
            Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
            if (!player || !player->HasAura(SPELL_SHOOTING_GALLERY_OVERRIDE_ACTION))
                return;

            if (Creature* target = FindActiveTarget(player))
                AwardTargetHit(player, target);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_darkmoon_shooting_gallery_shoot_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_darkmoon_shooting_gallery_shoot_SpellScript();
    }
};

class spell_darkmoon_shooting_gallery_override_action : public SpellScriptLoader
{
public:
    spell_darkmoon_shooting_gallery_override_action() : SpellScriptLoader("spell_darkmoon_shooting_gallery_override_action") { }

    class spell_darkmoon_shooting_gallery_override_action_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_darkmoon_shooting_gallery_override_action_AuraScript);

        void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Player* player = GetTarget() ? GetTarget()->ToPlayer() : nullptr;
            if (!player)
                return;

            player->SetPower(POWER_ALTERNATE_POWER, 0);

            if (Creature* rinling = GetClosestRinlingFor(player))
                if (rinling->AI() && rinling->AI()->GetGUID(DATA_SHOOTING_GALLERY_ACTIVE_PLAYER) == player->GetGUID())
                    rinling->AI()->DoAction(ACTION_SHOOTING_GALLERY_END_GAME);
        }

        void Register() override
        {
            AfterEffectRemove += AuraEffectRemoveFn(spell_darkmoon_shooting_gallery_override_action_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_OVERRIDE_SPELLS, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_darkmoon_shooting_gallery_override_action_AuraScript();
    }
};

void AddSC_event_darkmoon_shooting_gallery()
{
    new npc_darkmoon_rinling();
    new npc_darkmoon_shooting_gallery_target();
    new spell_darkmoon_shooting_gallery_shoot();
    new spell_darkmoon_shooting_gallery_override_action();
}
