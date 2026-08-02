/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "darkmoon_faire.h"
#include "Containers.h"
#include "GameEventMgr.h"
#include "Map.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "Transport.h"
#include "Vehicle.h"
#include <algorithm>
#include <list>
#include <vector>

void AddSC_event_darkmoon_deathmatch();
void AddSC_event_darkmoon_cannon();
void AddSC_event_darkmoon_whack_a_gnoll();
void AddSC_event_darkmoon_shooting_gallery();
void AddSC_event_darkmoon_ring_toss();
void AddSC_event_darkmoon_tonk_challenge();

namespace
{
    void TeleportPlayerFromExpiredDarkmoonFaire(Player* player)
    {
        if (!player)
            return;

        DarkmoonFaire::EventReturnPosition const& position = DarkmoonFaire::GetEventEndReturnPosition(player->GetTeam() == HORDE);

        if (player->GetVehicle())
            player->ExitVehicle();

        player->TeleportTo(position.MapId, position.X, position.Y, position.Z, position.Orientation);
    }

    void TeleportPlayerIfDarkmoonFaireExpired(Player* player)
    {
        if (!player || !DarkmoonFaire::ShouldTeleportOffIslandWhenEventInactive(sGameEventMgr->IsActiveEvent(DarkmoonFaire::GameEventId), player->GetMapId()))
            return;

        TeleportPlayerFromExpiredDarkmoonFaire(player);
    }
}

class world_darkmoon_faire : public WorldScript
{
public:
    world_darkmoon_faire() : WorldScript("world_darkmoon_faire") { }

    void OnGameEvent(bool start, uint16 eventId) override
    {
        if (!DarkmoonFaire::IsDarkmoonFaireEventEnd(start, eventId))
            return;

        std::vector<uint64> playerGuids;

        {
            SF_SHARED_GUARD readGuard(*HashMapHolder<Player>::GetLock());
            HashMapHolder<Player>::MapType const& players = ObjectAccessor::GetPlayers();
            for (HashMapHolder<Player>::MapType::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                Player* player = itr->second;
                if (player && DarkmoonFaire::ShouldTeleportOffIsland(start, eventId, player->GetMapId()))
                    playerGuids.push_back(player->GetGUID());
            }
        }

        for (std::vector<uint64>::const_iterator itr = playerGuids.begin(); itr != playerGuids.end(); ++itr)
            if (Player* player = ObjectAccessor::FindPlayer(*itr))
                if (DarkmoonFaire::ShouldTeleportOffIsland(start, eventId, player->GetMapId()))
                    TeleportPlayerFromExpiredDarkmoonFaire(player);
    }
};

class player_darkmoon_faire : public PlayerScript
{
public:
    player_darkmoon_faire() : PlayerScript("player_darkmoon_faire") { }

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        TeleportPlayerIfDarkmoonFaireExpired(player);
    }

    void OnMapChanged(Player* player) override
    {
        TeleportPlayerIfDarkmoonFaireExpired(player);
    }
};

class npc_darkmoon_moonfang : public CreatureScript
{
public:
    npc_darkmoon_moonfang() : CreatureScript("npc_darkmoon_moonfang") { }

    struct npc_darkmoon_moonfangAI : public ScriptedAI
    {
        npc_darkmoon_moonfangAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            _events.Reset();
        }

        void EnterCombat(Unit* /*who*/) override
        {
            _events.ScheduleEvent(EVENT_MOONFANG_CURSE, 19500);
            _events.ScheduleEvent(EVENT_FANGS_OF_THE_MOON, 8500);
            _events.ScheduleEvent(EVENT_MOONFANG_TEARS, 11000);
            _events.ScheduleEvent(EVENT_CALL_THE_PACK, 25000);
            _events.ScheduleEvent(EVENT_LEAP_FOR_THE_KILL, 15000);
        }

        void UpdateAI(uint32 diff) override
        {
            if (!UpdateVictim())
                return;

            _events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MOONFANG_CURSE:
                        DoCast(me, SPELL_MOONFANG_CURSE);
                        _events.ScheduleEvent(EVENT_MOONFANG_CURSE, 21000);
                        break;
                    case EVENT_CALL_THE_PACK:
                        DoCast(me, SPELL_CALL_THE_PACK);
                        _events.ScheduleEvent(EVENT_CALL_THE_PACK, 32000);
                        break;
                    case EVENT_FANGS_OF_THE_MOON:
                        DoCastVictim(SPELL_FANGS_OF_THE_MOON);
                        _events.ScheduleEvent(EVENT_FANGS_OF_THE_MOON, 12500);
                        break;
                    case EVENT_LEAP_FOR_THE_KILL:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100.0f, true))
                            DoCast(target, SPELL_LEAP_FOR_THE_KILL);
                        else
                            DoCastVictim(SPELL_LEAP_FOR_THE_KILL);
                        _events.ScheduleEvent(EVENT_LEAP_FOR_THE_KILL, 16000);
                        break;
                    case EVENT_MOONFANG_TEARS:
                        DoCast(me, SPELL_MOONFANG_TEARS);
                        _events.ScheduleEvent(EVENT_MOONFANG_TEARS, 20000);
                        break;
                    default:
                        break;
                }
                break;
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap _events;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_darkmoon_moonfangAI(creature);
    }
};

namespace DarkmoonCarousel
{
    typedef std::pair<uint32, Position> PassengerInfo;

    uint32 const MerryGoRoundEntry = 216666;
    uint32 const RideTicketItem = 81055;

    std::vector<PassengerInfo> const Passengers =
    {
        { 68233, { 6.57632f, -3.6821f, 1.201352f, 4.0477900f } },
        { 68238, { -0.1620f, 7.45662f, 1.201352f, 0.0058666f } },
        { 68231, { -6.6299f, 3.93165f, 1.201352f, 1.1105790f } },
        { 68239, { 6.56192f, 4.00892f, 1.201351f, 5.2554440f } },
        { 68232, { -6.3160f, -3.8700f, 1.201352f, 2.0703380f } },
        { 68238, { 0.27098f, -7.2955f, 1.201352f, 3.1815390f } }
    };

    bool IsCarouselMount(uint32 entry)
    {
        return std::any_of(Passengers.begin(), Passengers.end(), [entry](PassengerInfo const& passenger)
        {
            return passenger.first == entry;
        });
    }

    void SpawnPassenger(Transport* transport, uint32 entry, Position const& offset)
    {
        Map* map = transport->GetMap();
        if (!map)
            return;

        Creature* creature = new Creature();
        if (!creature->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_UNIT), map, entry, 0, 0, offset.GetPositionX(), offset.GetPositionY(), offset.GetPositionZ(), offset.GetOrientation()))
        {
            delete creature;
            return;
        }

        creature->SetRespawnTime(0);
        creature->SetTransport(transport);
        creature->m_movementInfo.transport.guid = transport->GetGUID();
        creature->m_movementInfo.transport.pos.Relocate(offset);

        float x = offset.GetPositionX();
        float y = offset.GetPositionY();
        float z = offset.GetPositionZ();
        float o = offset.GetOrientation();
        transport->CalculatePassengerPosition(x, y, z, &o);
        creature->Relocate(x, y, z, o);
        creature->SetHomePosition(x, y, z, o);
        creature->SetTransportHomePosition(creature->m_movementInfo.transport.pos);
        creature->AddUnitState(UNIT_STATE_IGNORE_PATHFINDING);

        if (!creature->IsPositionValid() || !map->AddToMap(creature))
        {
            delete creature;
            return;
        }

        transport->AddPassenger(creature);
    }
}

class vehicle_darkmoon_carousel_mount : public VehicleScript
{
public:
    vehicle_darkmoon_carousel_mount() : VehicleScript("vehicle_darkmoon_carousel_mount") { }

    void OnAddPassenger(Vehicle* vehicle, Unit* passenger, int8 /*seatId*/) override
    {
        Player* player = passenger ? passenger->ToPlayer() : NULL;
        if (!player || player->HasItemCount(DarkmoonCarousel::RideTicketItem))
            return;

        Unit* base = vehicle ? vehicle->GetBase() : NULL;
        if (!base || !DarkmoonCarousel::IsCarouselMount(base->GetEntry()))
            return;

        player->ExitVehicle();
    }
};

class transport_merry_go_round : public TransportScript
{
public:
    transport_merry_go_round() : TransportScript("transport_merry_go_round") { }

    void OnCreate(Transport* transport) override
    {
        if (transport->GetEntry() != DarkmoonCarousel::MerryGoRoundEntry)
            return;

        for (DarkmoonCarousel::PassengerInfo const& passenger : DarkmoonCarousel::Passengers)
            DarkmoonCarousel::SpawnPassenger(transport, passenger.first, passenger.second);
    }
};

class spell_whee_trigger : public SpellScriptLoader
{
public:
    spell_whee_trigger() : SpellScriptLoader("spell_whee_trigger") { }

    class spell_whee_trigger_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_whee_trigger_SpellScript);

        enum WheeData
        {
            SPELL_WHEE = 46668,
            SPELL_DARKMOON_TOP_HAT = 136583,
            WHEE_DURATION_INCREMENT = 5 * MINUTE * IN_MILLISECONDS,
            MAX_WHEE_DURATION = HOUR * IN_MILLISECONDS
        };

        void HandleHit()
        {
            Unit* hitUnit = GetHitUnit();
            if (!hitUnit)
                return;

            Player* player = hitUnit->ToPlayer();
            if (!player)
                return;

            if (Aura* aura = player->GetAura(SPELL_WHEE))
            {
                int32 duration = aura->GetDuration() + int32(WHEE_DURATION_INCREMENT);
                duration = std::min(duration, int32(MAX_WHEE_DURATION));
                aura->SetMaxDuration(duration);
                aura->SetDuration(duration);
                return;
            }

            if (!player->HasItemCount(DarkmoonCarousel::RideTicketItem) || player->HasAura(SPELL_DARKMOON_TOP_HAT))
                return;

            player->CastSpell(player, SPELL_WHEE, true);
            player->DestroyItemCount(DarkmoonCarousel::RideTicketItem, 1, true);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_whee_trigger_SpellScript::HandleHit);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_whee_trigger_SpellScript();
    }
};

class spell_darkmoon_free_your_mind : public SpellScriptLoader
{
public:
    spell_darkmoon_free_your_mind() : SpellScriptLoader("spell_darkmoon_free_your_mind") { }

    class spell_darkmoon_free_your_mind_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_darkmoon_free_your_mind_SpellScript);

        void HandleAfterCast()
        {
            if (Unit* caster = GetCaster())
                if (roll_chance_i(50))
                    caster->CastSpell(caster, SPELL_FREE_YOUR_MIND_EFF, true);
        }

        void Register() override
        {
            AfterCast += SpellCastFn(spell_darkmoon_free_your_mind_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_darkmoon_free_your_mind_SpellScript();
    }
};

class spell_darkmoon_moonfang_tears : public SpellScriptLoader
{
public:
    spell_darkmoon_moonfang_tears() : SpellScriptLoader("spell_darkmoon_moonfang_tears") { }

    class spell_darkmoon_moonfang_tears_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_darkmoon_moonfang_tears_SpellScript);

        void HandleOnEffectHit(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            if (!caster || !target)
                return;

            caster->CastSpell(target, GetSpellInfo()->Effects[effIndex].BasePoints, true);
        }

        void CountTargets(std::list<WorldObject*>& targets)
        {
            _originalTargets = targets;

            Unit* caster = GetCaster();
            targets.remove_if([caster](WorldObject* target)
            {
                if (!caster || !target)
                    return false;

                Player* player = target->ToPlayer();
                return player && player->GetExactDist2d(caster) < 12.5f;
            });

            if (targets.size() >= 3)
                return;

            targets = _originalTargets;
            if (targets.size() > 3)
                Skyfire::Containers::RandomResizeList(targets, 3);
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_darkmoon_moonfang_tears_SpellScript::HandleOnEffectHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_darkmoon_moonfang_tears_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
        }

    private:
        std::list<WorldObject*> _originalTargets;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_darkmoon_moonfang_tears_SpellScript();
    }
};

class spell_darkmoon_moonfang_curse : public SpellScriptLoader
{
public:
    spell_darkmoon_moonfang_curse() : SpellScriptLoader("spell_darkmoon_moonfang_curse") { }

    class spell_darkmoon_moonfang_curse_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_darkmoon_moonfang_curse_SpellScript);

        void CountTargets(std::list<WorldObject*>& targets)
        {
            uint32 targetCount = targets.size() / 2;

            if (targets.size() > targetCount)
                Skyfire::Containers::RandomResizeList(targets, targetCount);

            _targets = targets;
        }

        void CopyTargets(std::list<WorldObject*>& targets)
        {
            targets = _targets;
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_darkmoon_moonfang_curse_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_darkmoon_moonfang_curse_SpellScript::CopyTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_darkmoon_moonfang_curse_SpellScript::CopyTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_darkmoon_moonfang_curse_SpellScript::CopyTargets, EFFECT_3, TARGET_UNIT_SRC_AREA_ENEMY);
        }

    private:
        std::list<WorldObject*> _targets;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_darkmoon_moonfang_curse_SpellScript();
    }
};

void AddSC_event_darkmoon_faire()
{
    new world_darkmoon_faire();
    new player_darkmoon_faire();
    new npc_darkmoon_moonfang();
    new vehicle_darkmoon_carousel_mount();
    new transport_merry_go_round();
    new spell_whee_trigger();
    new spell_darkmoon_free_your_mind();
    new spell_darkmoon_moonfang_tears();
    new spell_darkmoon_moonfang_curse();
    AddSC_event_darkmoon_cannon();
    AddSC_event_darkmoon_deathmatch();
    AddSC_event_darkmoon_shooting_gallery();
    AddSC_event_darkmoon_whack_a_gnoll();
    AddSC_event_darkmoon_ring_toss();
    AddSC_event_darkmoon_tonk_challenge();
}
