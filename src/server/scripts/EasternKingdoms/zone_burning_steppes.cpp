/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "QuestDef.h"
#include "SpellScript.h"
#include "Spell.h"
#include "SpellMgr.h"
#include "Vehicle.h"
#include "CombatAI.h"
#include "DB2Stores.h"
#include "WorldSession.h"

namespace
{
    uint32 const ZONE_BURNING_STEPPES = 46;
    uint32 const NPC_OBSIDIAN_CLOAKED_DRAGON = 48434;
    uint32 const SPELL_CALL_DRAGON = 90079;
    uint32 const SPELL_DRAGON_FLIGHT = 90071;

    uint32 AssaultQuest(Player const* player)
    {
        return player->GetTeam() == ALLIANCE ? 28320 : 28454;
    }

    uint32 GloryQuest(Player const* player)
    {
        return player->GetTeam() == ALLIANCE ? 28321 : 28455;
    }

    bool HasAssault(Player const* player)
    {
        QuestStatus const status = player->GetQuestStatus(AssaultQuest(player));
        return status == QUEST_STATUS_INCOMPLETE || status == QUEST_STATUS_COMPLETE;
    }

    Player* DragonRider(Unit* unit)
    {
        if (!unit)
            return NULL;

        Unit* dragon = unit->GetEntry() == NPC_OBSIDIAN_CLOAKED_DRAGON ? unit : unit->GetVehicleBase();
        if (!dragon || dragon->GetEntry() != NPC_OBSIDIAN_CLOAKED_DRAGON || !dragon->GetVehicleKit())
            return NULL;

        Unit* rider = dragon->GetVehicleKit()->GetPassenger(0);
        return rider ? rider->ToPlayer() : NULL;
    }

    void ReturnToCamp(Player* player)
    {
        player->ExitVehicle();
        if (player->GetTeam() == ALLIANCE)
            player->NearTeleportTo(-8365.08f, -2736.93f, 185.69f, 5.65f);
        else
            player->NearTeleportTo(-7494.21f, -2171.88f, 165.56f, 4.96f);
    }

    void SendHornDefinition(Player* player)
    {
        ByteBuffer record;
        sItemSparseStore.WriteRecord(63681, uint32(player->GetSession()->GetSessionDbcLocale()), record);
        WorldPacket packet(SMSG_DB_REPLY);
        packet << uint32(63681) << uint32(time(NULL)) << uint32(DB2_REPLY_ITEM_SPARSE) << uint32(record.size());
        packet.append(record);
        player->GetSession()->SendPacket(&packet);
    }

    void AcceptIfAvailable(Player* player, uint32 id)
    {
        if (Quest const* quest = sObjectMgr->GetQuestTemplate(id))
            if (player->CanTakeQuest(quest, false) && player->CanAddQuest(quest, false))
                player->AddQuest(quest, NULL);
    }

    void RecoverFollowups(Player* player)
    {
        if (player->GetZoneId() != ZONE_BURNING_STEPPES)
            return;

        SendHornDefinition(player);

        bool const alliance = player->GetTeam() == ALLIANCE;
        QuestStatus status = player->GetQuestStatus(alliance ? 28245 : 28433);
        if (status == QUEST_STATUS_INCOMPLETE || status == QUEST_STATUS_COMPLETE || player->GetQuestRewardStatus(alliance ? 28245 : 28433))
            AcceptIfAvailable(player, alliance ? 28246 : 28434);
        status = player->GetQuestStatus(alliance ? 28278 : 28439);
        if (status == QUEST_STATUS_INCOMPLETE || status == QUEST_STATUS_COMPLETE || player->GetQuestRewardStatus(alliance ? 28278 : 28439))
            AcceptIfAvailable(player, alliance ? 28279 : 28440);

        uint32 const campaign[] = { alliance ? 28314u : 28446u, alliance ? 28315u : 28447u, alliance ? 28316u : 28448u };
        if (player->GetQuestRewardStatus(campaign[0]) && player->GetQuestRewardStatus(campaign[1]) && player->GetQuestRewardStatus(campaign[2]))
            AcceptIfAvailable(player, alliance ? 28326 : 28449);

        uint32 const deliveries[] = { alliance ? 28317u : 28450u, alliance ? 28318u : 28451u, alliance ? 28319u : 28452u };
        if (player->GetQuestRewardStatus(deliveries[0]) && player->GetQuestRewardStatus(deliveries[1]) && player->GetQuestRewardStatus(deliveries[2]))
            AcceptIfAvailable(player, alliance ? 28327 : 28453);
    }
}

class player_burning_steppes_progression : public PlayerScript
{
public:
    player_burning_steppes_progression() : PlayerScript("player_burning_steppes_progression") { }

    void OnQuestAdd(Player* player, Quest const* quest) OVERRIDE
    {
        uint32 const id = quest->GetQuestId();
        if (id == AssaultQuest(player))
            SendHornDefinition(player);
        if (id == 28245 || id == 28433 || id == 28278 || id == 28439)
            RecoverFollowups(player);
    }

    void OnQuestReward(Player* player, Quest const* quest, Object* /*giver*/) OVERRIDE
    {
        uint32 const id = quest->GetQuestId();
        if ((id >= 28314 && id <= 28319) || (id >= 28446 && id <= 28452))
            RecoverFollowups(player);
    }

    void OnLogin(Player* player, bool /*firstLogin*/) OVERRIDE { RecoverFollowups(player); }
    void OnUpdateZone(Player* player, uint32 /*zone*/, uint32 /*area*/) OVERRIDE { RecoverFollowups(player); }
};

class npc_burning_steppes_stage_giver : public CreatureScript
{
public:
    npc_burning_steppes_stage_giver() : CreatureScript("npc_burning_steppes_stage_giver") { }

    bool OnQuestAccept(Player* player, Creature* /*creature*/, Quest const* quest) OVERRIDE
    {
        if (quest->GetQuestId() == AssaultQuest(player) && !player->GetVehicleBase())
            player->CastSpell(player, SPELL_CALL_DRAGON, true);
        return false;
    }
};

class npc_burning_steppes_assault_dragon : public CreatureScript
{
public:
    npc_burning_steppes_assault_dragon() : CreatureScript("npc_burning_steppes_assault_dragon") { }

    struct DragonAI : public VehicleAI
    {
        DragonAI(Creature* creature) : VehicleAI(creature), owner(0), checkTimer(1000), emptyTime(0) { }
        uint64 owner;
        uint32 checkTimer;
        uint32 emptyTime;

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            Player* player = summoner ? summoner->ToPlayer() : NULL;
            if (!player || !HasAssault(player) || player->GetZoneId() != ZONE_BURNING_STEPPES)
            {
                me->DespawnOrUnsummon(1);
                return;
            }

            owner = player->GetGUID();
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            me->SetCanFly(true);
            // Let the rider's movement packets control takeoff and landing.
            me->SetDisableGravity(false);
            UpdateFlightAnimation();
            me->CastSpell(me, SPELL_DRAGON_FLIGHT, true);
        }

        void UpdateFlightAnimation()
        {
            // CAN_FLY is permission; FLYING is the rider's current flight mode.
            // Changing only the animation tier leaves Space/X and gravity to the client.
            me->SetAnimTier(me->HasUnitMovementFlag(MOVEMENTFLAG_FLYING) ? ANIM_TIER_FLY : ANIM_TIER_GROUND);
        }

        // NPC immunity also blocks outgoing hostile area-target selection.
        void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE { damage = 0; }

        void PassengerBoarded(Unit* passenger, int8 /*seat*/, bool apply) OVERRIDE
        {
            Player* player = passenger->ToPlayer();
            if (!player)
                return;

            if (apply)
            {
                if (player->GetGUID() != owner || !HasAssault(player))
                    player->ExitVehicle();
                else
                {
                    UpdateFlightAnimation();
                    emptyTime = 0;
                }
            }
            else
            {
                // Voluntary dismounts and abandoning the quest must not strand a rider in the air.
                if (player->IsAlive())
                    player->CastSpell(player, 45472, true);
                me->DespawnOrUnsummon(1000);
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            UpdateFlightAnimation();

            if (checkTimer > diff)
            {
                checkTimer -= diff;
                return;
            }
            checkTimer = 1000;

            Player* player = ObjectAccessor::GetPlayer(*me, owner);
            if (!player || !player->IsAlive() || !HasAssault(player) || player->GetZoneId() != ZONE_BURNING_STEPPES)
            {
                if (player && player->GetVehicleBase() == me)
                    player->ExitVehicle();
                me->DespawnOrUnsummon(1000);
                return;
            }

            if (DragonRider(me) != player)
            {
                emptyTime += 1000;
                if (emptyTime >= 5000)
                    me->DespawnOrUnsummon();
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE { return new DragonAI(creature); }
};

class npc_burning_steppes_assault_target : public CreatureScript
{
public:
    npc_burning_steppes_assault_target() : CreatureScript("npc_burning_steppes_assault_target") { }

    struct TargetAI : public ScriptedAI
    {
        TargetAI(Creature* creature) : ScriptedAI(creature) { }
        bool IsInvader() const { return me->GetEntry() == 48414 || me->GetEntry() == 48432; }

        void Reset() OVERRIDE { me->SetReactState(IsInvader() ? REACT_PASSIVE : REACT_AGGRESSIVE); }

        void DamageTaken(Unit* attacker, uint32& damage) OVERRIDE
        {
            Player* player = IsInvader() ? DragonRider(attacker) : (attacker ? attacker->GetCharmerOrOwnerPlayerOrPlayerItself() : NULL);
            if (!player || !player->IsAlive() || player->GetMapId() != 0 || player->GetZoneId() != ZONE_BURNING_STEPPES ||
                player->GetQuestStatus(IsInvader() ? AssaultQuest(player) : GloryQuest(player)) != QUEST_STATUS_INCOMPLETE)
                damage = 0;
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            if (!IsInvader() && UpdateVictim())
                DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE { return new TargetAI(creature); }
};

class spell_burning_steppes_dragon_horn : public SpellScriptLoader
{
public:
    spell_burning_steppes_dragon_horn() : SpellScriptLoader("spell_burning_steppes_dragon_horn") { }
    class Script : public SpellScript
    {
        PrepareSpellScript(Script);
        SpellCastResult Check()
        {
            Player* player = GetCaster()->ToPlayer();
            if (!player || !player->IsAlive() || player->GetMapId() != 0 || player->GetZoneId() != ZONE_BURNING_STEPPES || !HasAssault(player))
                return SpellCastResult::SPELL_FAILED_NOT_HERE;
            return player->GetVehicleBase() ? SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW : SpellCastResult::SPELL_CAST_OK;
        }
        void Register() OVERRIDE { OnCheckCast += SpellCheckCastFn(Script::Check); }
    };
    SpellScript* GetSpellScript() const OVERRIDE { return new Script(); }
};

class spell_burning_steppes_dragon_breath : public SpellScriptLoader
{
public:
    spell_burning_steppes_dragon_breath() : SpellScriptLoader("spell_burning_steppes_dragon_breath") { }
    class Script : public SpellScript
    {
        PrepareSpellScript(Script);
        bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
        {
            SpellInfo const* visual = sSpellMgr->GetSpellInfo(90067);
            return visual && visual->SpellVisual[0];
        }
        void PlayBreathVisual()
        {
            if (WorldLocation const* dest = GetExplTargetDest())
            {
                // Destination visuals use travel time: the native speed (25) made this last 25 seconds.
                // Keep a short visual; 90068 remains the sole damage and native credit cast.
                SpellInfo const* visual = sSpellMgr->GetSpellInfo(90067);
                GetCaster()->SendPlaySpellVisual(visual->SpellVisual[0], dest->GetPositionX(),
                    dest->GetPositionY(), dest->GetPositionZ(), 0.6f);
            }
        }
        bool Load() OVERRIDE { return GetCaster()->GetEntry() == NPC_OBSIDIAN_CLOAKED_DRAGON; }
        SpellCastResult Check()
        {
            Player* player = DragonRider(GetCaster());
            return player && player->GetZoneId() == ZONE_BURNING_STEPPES &&
                player->GetQuestStatus(AssaultQuest(player)) == QUEST_STATUS_INCOMPLETE ?
                SpellCastResult::SPELL_CAST_OK : SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
        }
        void Filter(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target) { return target->GetEntry() != 48414 && target->GetEntry() != 48432; });
        }
        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(Script::Check);
            OnCast += SpellCastFn(Script::PlayBreathVisual);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(Script::Filter, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        }
    };
    SpellScript* GetSpellScript() const OVERRIDE { return new Script(); }
};

class spell_burning_steppes_dragon_return : public SpellScriptLoader
{
public:
    spell_burning_steppes_dragon_return() : SpellScriptLoader("spell_burning_steppes_dragon_return") { }
    class Script : public SpellScript
    {
        PrepareSpellScript(Script);
        bool Load() OVERRIDE { return GetCaster()->GetEntry() == NPC_OBSIDIAN_CLOAKED_DRAGON; }
        SpellCastResult Check() { return DragonRider(GetCaster()) ? SpellCastResult::SPELL_CAST_OK : SpellCastResult::SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW; }
        void Return(SpellEffIndex effect)
        {
            PreventHitDefaultEffect(effect);
            if (Player* player = DragonRider(GetCaster()))
                ReturnToCamp(player);
        }
        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(Script::Check);
            OnEffectHitTarget += SpellEffectFn(Script::Return, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };
    SpellScript* GetSpellScript() const OVERRIDE { return new Script(); }
};

class spell_burning_steppes_scorpid_barb : public SpellScriptLoader
{
public:
    spell_burning_steppes_scorpid_barb() : SpellScriptLoader("spell_burning_steppes_scorpid_barb") { }
    class Script : public SpellScript
    {
        PrepareSpellScript(Script);
        SpellCastResult Check()
        {
            Player* player = GetCaster()->ToPlayer();
            Creature* target = GetExplTargetUnit() ? GetExplTargetUnit()->ToCreature() : NULL;
            if (!player || !target || !target->IsAlive() || player->GetZoneId() != ZONE_BURNING_STEPPES ||
                (target->GetEntry() != 48099 && target->GetEntry() != 48100 && target->GetEntry() != 9176))
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            uint32 const quest = player->GetTeam() == ALLIANCE ? 28246 : 28434;
            if (player->GetQuestStatus(quest) != QUEST_STATUS_INCOMPLETE || player->GetReqKillOrCastCurrentCount(quest, target->GetEntry()))
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            return SpellCastResult::SPELL_CAST_OK;
        }
        void Assassinate(SpellEffIndex /*effect*/)
        {
            Player* player = GetCaster()->ToPlayer();
            Creature* target = GetHitCreature();
            if (!player || !target || target != GetExplTargetUnit() || Check() != SpellCastResult::SPELL_CAST_OK)
                return;
            if (target->GetEntry() == 48100)
                player->SummonCreature(48106, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 6000);
            // A real kill gives the native objective credit once; do not grant a second scripted credit.
            target->SetLootRecipient(player);
            target->LowerPlayerDamageReq(target->GetHealth());
            player->Kill(target);
        }
        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(Script::Check);
            OnEffectHitTarget += SpellEffectFn(Script::Assassinate, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };
    SpellScript* GetSpellScript() const OVERRIDE { return new Script(); }
};

void AddSC_burning_steppes()
{
    new player_burning_steppes_progression();
    new npc_burning_steppes_stage_giver();
    new npc_burning_steppes_assault_dragon();
    new npc_burning_steppes_assault_target();
    new spell_burning_steppes_dragon_horn();
    new spell_burning_steppes_dragon_breath();
    new spell_burning_steppes_dragon_return();
    new spell_burning_steppes_scorpid_barb();
}
