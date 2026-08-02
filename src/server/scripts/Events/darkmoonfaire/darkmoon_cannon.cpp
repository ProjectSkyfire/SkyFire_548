/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "darkmoon_faire.h"
#include "AchievementMgr.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SpellAuraDefines.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include <list>

namespace
{
    uint32 const PlayerCheckInterval = 250;
    uint32 const MaximaAmbientInterval = 30 * IN_MILLISECONDS;

    float const LandingSearchRange = 90.0f;
    float const LandingReadyZ = 0.75f;

    char const* const GossipLaunchText = "Launch me! |cFF0000FF(Darkmoon Game Token)|r";
    char const* const GossipReturnText = "Teleport me back to the cannon.";
    char const* const MissingMoneyText = "You need 30 silver to return to the cannon.";

    enum CannonEvents
    {
        EVENT_CANNON_CHECK_PLAYER = 1,
        EVENT_CANNON_MAXIMA_AMBIENT
    };

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
                || player->HasAura(SPELL_DARKMOON_TONK_TARGET_TRACKER)
                || player->HasAura(SPELL_DARKMOON_TONK_TARGET_PARTICIPANT)
                || player->HasAura(SPELL_DARKMOON_CANNON_PREPARATION)
                || player->HasAura(SPELL_DARKMOON_CANNON_MAGIC_WINGS)
                || player->HasAura(SPELL_DARKMOON_CANNON_LAUNCH)
                || player->HasAura(SPELL_DARKMOON_CANNON_ROOT));
    }

    uint32 GetCannonScore(Player* player)
    {
        return player ? DarkmoonCannon::ClampCannonScore(player->GetReqKillOrCastCurrentCount(QUEST_HUMANOID_CANNONBALL, NPC_DARKMOON_CANNON_TARGET_CREDIT), 0) : 0;
    }

    void RemoveCannonAuras(Player* player)
    {
        if (!player)
            return;

        player->RemoveAurasDueToSpell(SPELL_DARKMOON_CANNON_PREPARATION);
        player->RemoveAurasDueToSpell(SPELL_DARKMOON_CANNON_MAGIC_WINGS);
        player->RemoveAurasDueToSpell(SPELL_DARKMOON_CANNON_LAUNCH);
        player->RemoveAurasDueToSpell(SPELL_DARKMOON_CANNON_ROOT);
    }

    void RemoveDarkmoonGameAuras(Player* player)
    {
        if (!player)
            return;

        player->RemoveAurasDueToSpell(SPELL_WHACK_OVERRIDE_ACTION);
        player->RemoveAurasDueToSpell(SPELL_WHACK_ENABLE_POWERBAR);
        player->RemoveAurasDueToSpell(SPELL_SHOOTING_GALLERY_OVERRIDE_ACTION);
        player->RemoveAurasDueToSpell(SPELL_RING_TOSS_ACTION);
        player->RemoveAurasDueToSpell(SPELL_DARKMOON_TONK_CONTROLLER);
        player->RemoveAurasDueToSpell(SPELL_DARKMOON_TONK_CANNON_BLAST);
        player->RemoveAurasDueToSpell(SPELL_DARKMOON_TONK_ACTION);
        player->RemoveAurasDueToSpell(SPELL_DARKMOON_TONK_TARGET_TRACKER);
        player->RemoveAurasDueToSpell(SPELL_DARKMOON_TONK_TARGET_PARTICIPANT);
        RemoveCannonAuras(player);
        player->SetPower(POWER_ALTERNATE_POWER, 0);
    }

    void AwardCannonCredit(Player* player, uint32 creditCount)
    {
        if (!player || !creditCount)
            return;

        uint32 const currentScore = GetCannonScore(player);
        uint32 const newScore = DarkmoonCannon::ClampCannonScore(currentScore, creditCount);

        for (uint32 i = currentScore; i < newScore; ++i)
            player->KilledMonsterCredit(NPC_DARKMOON_CANNON_TARGET_CREDIT);
    }

    void CompleteBullseyeAchievement(Player* player)
    {
        if (!player || player->HasAchieved(ACHIEVEMENT_BLASTENHEIMER_BULLSEYE))
            return;

        if (AchievementEntry const* achievement = sAchievementMgr->GetAchievement(ACHIEVEMENT_BLASTENHEIMER_BULLSEYE))
            player->CompletedAchievement(achievement);
    }

    bool IsLandingReady(Player* player)
    {
        return player && (player->IsInWater() || player->GetPositionZ() <= LandingReadyZ);
    }

    Position GetLaunchPosition()
    {
        DarkmoonCannon::CannonPosition const& launch = DarkmoonCannon::GetLaunchPosition();
        Position launchPosition = { launch.X, launch.Y, launch.Z, launch.Orientation };
        return launchPosition;
    }

    Position GetReturnPosition()
    {
        DarkmoonCannon::CannonPosition const& returnPosition = DarkmoonCannon::GetReturnPosition();
        Position boothPosition = { returnPosition.X, returnPosition.Y, returnPosition.Z, returnPosition.Orientation };
        return boothPosition;
    }

    void SendPlayerToCannon(Player* player)
    {
        if (!player)
            return;

        Position const launch = GetLaunchPosition();
        player->NearTeleportTo(launch.GetPositionX(), launch.GetPositionY(), launch.GetPositionZ(), launch.GetOrientation(), true);
    }

    void SendPlayerToBooth(Player* player)
    {
        if (!player)
            return;

        Position const booth = GetReturnPosition();
        player->TeleportTo(DarkmoonCannon::MapId, booth.GetPositionX(), booth.GetPositionY(), booth.GetPositionZ(), booth.GetOrientation());
    }
}

class npc_darkmoon_maxima_blastenheimer : public CreatureScript
{
public:
    npc_darkmoon_maxima_blastenheimer() : CreatureScript("npc_darkmoon_maxima_blastenheimer") { }

    struct npc_darkmoon_maxima_blastenheimerAI : public ScriptedAI
    {
        npc_darkmoon_maxima_blastenheimerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            _events.Reset();
            _events.ScheduleEvent(EVENT_CANNON_MAXIMA_AMBIENT, MaximaAmbientInterval);
        }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                if (eventId == EVENT_CANNON_MAXIMA_AMBIENT)
                {
                    Talk(1);
                    _events.ScheduleEvent(EVENT_CANNON_MAXIMA_AMBIENT, MaximaAmbientInterval);
                }
            }
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_darkmoon_maxima_blastenheimerAI(creature);
    }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (creature->IsQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (creature->AI())
            creature->AI()->Talk(0);

        if (player->GetQuestStatus(QUEST_HUMANOID_CANNONBALL) == QUEST_STATUS_INCOMPLETE
            && !DarkmoonCannon::IsCompleteScore(GetCannonScore(player))
            && !HasDarkmoonGameAura(player))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GossipLaunchText, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

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

        player->DestroyItemCount(ITEM_DARKMOON_GAME_TOKEN, 1, true);
        player->RemoveAurasByType(SPELL_AURA_MOUNTED);
        RemoveDarkmoonGameAuras(player);
        SendPlayerToCannon(player);
        player->CastSpell(player, SPELL_DARKMOON_CANNON_ROOT, true);
        player->CastSpell(player, SPELL_DARKMOON_CANNON_PREPARATION, true);
        player->CLOSE_GOSSIP_MENU();
        return true;
    }

    bool OnQuestComplete(Player* player, Creature* /*creature*/, Quest const* quest) override
    {
        if (quest && quest->GetQuestId() == QUEST_HUMANOID_CANNONBALL)
            RemoveCannonAuras(player);

        return true;
    }
};

class npc_darkmoon_cannon_target : public CreatureScript
{
public:
    npc_darkmoon_cannon_target() : CreatureScript("npc_darkmoon_cannon_target") { }

    struct npc_darkmoon_cannon_targetAI : public ScriptedAI
    {
        npc_darkmoon_cannon_targetAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            _events.Reset();
            me->SetReactState(REACT_PASSIVE);
            _events.ScheduleEvent(EVENT_CANNON_CHECK_PLAYER, PlayerCheckInterval);
        }

        void EnterCombat(Unit* /*who*/) override { }

        void UpdateAI(uint32 diff) override
        {
            _events.Update(diff);

            while (uint32 eventId = _events.ExecuteEvent())
            {
                if (eventId == EVENT_CANNON_CHECK_PLAYER)
                {
                    CheckPlayers();
                    _events.ScheduleEvent(EVENT_CANNON_CHECK_PLAYER, PlayerCheckInterval);
                }
            }
        }

    private:
        void CheckPlayers()
        {
            std::list<Player*> players;
            me->GetPlayerListInGrid(players, LandingSearchRange);

            for (Player* player : players)
            {
                if (!player || player->GetMapId() != DarkmoonCannon::MapId || player->GetQuestStatus(QUEST_HUMANOID_CANNONBALL) != QUEST_STATUS_INCOMPLETE)
                    continue;

                if (!player->HasAura(SPELL_DARKMOON_CANNON_LAUNCH) || !IsLandingReady(player))
                    continue;

                float const distance = player->GetExactDist2d(me);
                uint32 const credit = DarkmoonCannon::GetLandingCredit(distance);
                uint32 resultSpell = SPELL_DARKMOON_CANNON_TARGET_MISS;

                if (distance <= DarkmoonCannon::BullseyeDistance)
                {
                    resultSpell = SPELL_DARKMOON_CANNON_TARGET_CENTER;
                    CompleteBullseyeAchievement(player);
                }
                else if (distance <= DarkmoonCannon::NearDistance)
                    resultSpell = SPELL_DARKMOON_CANNON_TARGET_NEAR;

                RemoveCannonAuras(player);
                me->CastSpell(player, resultSpell, true);
                AwardCannonCredit(player, credit);
            }
        }

        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_darkmoon_cannon_targetAI(creature);
    }
};

class npc_darkmoon_fozlebub : public CreatureScript
{
public:
    npc_darkmoon_fozlebub() : CreatureScript("npc_darkmoon_fozlebub") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        if (player->GetQuestStatus(QUEST_HUMANOID_CANNONBALL) == QUEST_STATUS_INCOMPLETE
            || player->GetQuestStatus(QUEST_HUMANOID_CANNONBALL) == QUEST_STATUS_COMPLETE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GossipReturnText, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

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

        if (!player->HasEnoughMoney(uint64(DarkmoonCannon::ReturnCost)))
        {
            creature->MonsterWhisper(MissingMoneyText, player);
            player->CLOSE_GOSSIP_MENU();
            return true;
        }

        player->ModifyMoney(-int64(DarkmoonCannon::ReturnCost));
        RemoveCannonAuras(player);
        player->CLOSE_GOSSIP_MENU();
        SendPlayerToBooth(player);
        return true;
    }
};

class spell_darkmoon_cannon_preparation : public SpellScriptLoader
{
public:
    spell_darkmoon_cannon_preparation() : SpellScriptLoader("spell_darkmoon_cannon_preparation") { }

    class spell_darkmoon_cannon_preparation_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_darkmoon_cannon_preparation_AuraScript);

        void HandleLaunch(AuraEffect const* /*aurEff*/)
        {
            Player* player = GetTarget() ? GetTarget()->ToPlayer() : NULL;
            if (!player || player->HasAura(SPELL_DARKMOON_CANNON_LAUNCH))
                return;

            player->RemoveAurasDueToSpell(SPELL_DARKMOON_CANNON_ROOT);
            player->SetOrientation(DarkmoonCannon::GetLaunchPosition().Orientation);
            player->AddAura(SPELL_DARKMOON_CANNON_LAUNCH, player);
            player->AddAura(SPELL_DARKMOON_CANNON_MAGIC_WINGS, player);

            DarkmoonCannon::CannonPosition const& source = DarkmoonCannon::GetLaunchSourcePosition();
            player->KnockbackFrom(source.X, source.Y, DarkmoonCannon::LaunchHorizontalSpeed, DarkmoonCannon::LaunchVerticalSpeed);
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_darkmoon_cannon_preparation_AuraScript::HandleLaunch, EFFECT_2, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_darkmoon_cannon_preparation_AuraScript();
    }
};

void AddSC_event_darkmoon_cannon()
{
    new npc_darkmoon_maxima_blastenheimer();
    new npc_darkmoon_cannon_target();
    new npc_darkmoon_fozlebub();
    new spell_darkmoon_cannon_preparation();
}
