/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "Vehicle.h"
#include "VehicleDefines.h"
#include "EventProcessor.h"
#include "ObjectAccessor.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"

#include <cmath>

enum OceanNotSoDeep
{
    // Creatures
    NPC_GREYMIST_TIDEHUNTER          = 33262,
    NPC_GREYMIST_WARRIOR             = 33277,

    // Spells
    SPELL_DEPTH_CHARGE_PULSE         = 62684, // vehicle spell1 — starts countdown
    SPELL_DEPTH_CHARGE_DETONATION    = 62682,
    SPELL_EJECT_ALL_PASSENGERS       = 50630,
    SPELL_UNDERWATER_BREATHING       = 82367,

    // Events
    EVENT_COUNTDOWN_PULSE            = 1,
    EVENT_DETONATE                   = 2,

    // Text (creature_text.groupid)
    SAY_TAUNT                        = 0,
    SAY_DETONATION_IMMINENT          = 1,

    // Misc
    COUNTDOWN_DURATION_MS            = 8000,
    COUNTDOWN_PULSE_MS               = 2000,
    DETONATION_RANGE                 = 30,
    CONSOLE_MAP_ID                   = 1,
    RETURN_CONFIRM_MS                = 300
};

// Decoy Bot Control Console island (map 1).
float const ConsoleX = 7748.42f;
float const ConsoleY = -407.781f;
float const ConsoleZ = 1.29484f;
float const ConsoleO = 2.37364f;
float const MaxReturnDist2d = 120.0f;
float const ReturnStuckDist = 8.0f;

static bool IsValidDarkshoreReturn(uint32 mapId, float x, float y, float z)
{
    if (mapId != CONSOLE_MAP_ID)
        return false;
    if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z))
        return false;
    if (z < -5.0f || z > 50.0f)
        return false;

    float const dx = x - ConsoleX;
    float const dy = y - ConsoleY;
    return (dx * dx + dy * dy) <= (MaxReturnDist2d * MaxReturnDist2d);
}

static void RestorePlayerAfterDecoyBot(Player* player, bool stopMoving = true)
{
    if (!player)
        return;

    player->RemoveAurasDueToSpell(VEHICLE_SPELL_PARACHUTE);
    player->RemoveAurasDueToSpell(SPELL_UNDERWATER_BREATHING);
    player->StopMirrorTimers();
    player->SetDisableGravity(false);
    player->SetCanFly(false);
    player->SetControlled(false, UNIT_STATE_ROOT);
    player->ClearUnitState(UNIT_STATE_ROOT | UNIT_STATE_MOVE);
    player->RemoveUnitMovementFlag(MOVEMENTFLAG_FLYING | MOVEMENTFLAG_FALLING | MOVEMENTFLAG_FALLING_FAR | MOVEMENTFLAG_ROOT);
    player->m_movementInfo.ResetTransport();

    // StopMoving() builds a new spline; skip it around TeleportTo so we do not rubber-band.
    if (stopMoving)
    {
        player->GetMotionMaster()->Clear(false);
        player->StopMoving();
    }
}

static void SuppressPlayerBreath(Player* player)
{
    if (!player)
        return;

    player->StopMirrorTimers();

    if (!player->HasAura(SPELL_UNDERWATER_BREATHING))
        player->CastSpell(player, SPELL_UNDERWATER_BREATHING, true);
}

static void TeleportPlayerToDecoyReturn(Player* player, uint32 mapId, float x, float y, float z, float o, bool onlyIfStuck = false)
{
    if (!player || !player->IsInWorld())
        return;

    if (!IsValidDarkshoreReturn(mapId, x, y, z))
    {
        mapId = CONSOLE_MAP_ID;
        x = ConsoleX;
        y = ConsoleY;
        z = ConsoleZ;
        o = ConsoleO;
    }

    if (onlyIfStuck)
    {
        // Already on the island and above water — do not snap a moving player back.
        if (player->GetMapId() == mapId && player->GetExactDist2d(x, y) <= ReturnStuckDist && player->GetPositionZ() > -1.0f)
            return;
    }

    RestorePlayerAfterDecoyBot(player, false);
    player->GetMotionMaster()->Clear(false);
    player->UpdatePosition(x, y, z, o, true);
    player->SetFallInformation(0, z);
    player->TeleportTo(mapId, x, y, z, o, TELE_TO_NOT_UNSUMMON_PET | TELE_TO_NOT_LEAVE_COMBAT);
    RestorePlayerAfterDecoyBot(player, false);
}

class ReturnPlayerEvent : public BasicEvent
{
public:
    ReturnPlayerEvent(uint64 playerGuid, uint32 mapId, float x, float y, float z, float o, bool onlyIfStuck)
        : _playerGuid(playerGuid), _mapId(mapId), _x(x), _y(y), _z(z), _o(o), _onlyIfStuck(onlyIfStuck) { }

    bool Execute(uint64 /*time*/, uint32 /*diff*/) override
    {
        if (Player* player = ObjectAccessor::FindPlayer(_playerGuid))
            TeleportPlayerToDecoyReturn(player, _mapId, _x, _y, _z, _o, _onlyIfStuck);
        return true;
    }

private:
    uint64 _playerGuid;
    uint32 _mapId;
    float _x, _y, _z, _o;
    bool _onlyIfStuck;
};

class npc_foolhardy_adventurer : public CreatureScript
{
public:
    npc_foolhardy_adventurer() : CreatureScript("npc_foolhardy_adventurer") { }

    struct npc_foolhardy_adventurerAI : public VehicleAI
    {
        npc_foolhardy_adventurerAI(Creature* creature) : VehicleAI(creature)
        {
            _playerGuid = 0;
            _returnMapId = CONSOLE_MAP_ID;
            _returnX = ConsoleX;
            _returnY = ConsoleY;
            _returnZ = ConsoleZ;
            _returnO = ConsoleO;
            _countdownTicks = 0;
            _countdownActive = false;
            _detonated = false;
            _hasReturnPos = false;
            _returnScheduled = false;
        }

        void Reset() OVERRIDE
        {
            _events.Reset();
        }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            if (Player* player = summoner->ToPlayer())
            {
                _playerGuid = player->GetGUID();
                SaveReturnPosition(player);
            }
        }

        void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) OVERRIDE
        {
            Player* player = passenger->ToPlayer();
            if (!player)
                return;

            if (apply)
            {
                _playerGuid = player->GetGUID();
                _detonated = false;
                _countdownActive = false;
                _countdownTicks = 0;
                _returnScheduled = false;

                if (!_hasReturnPos)
                    UseConsoleFallback();

                me->CastSpell(me, SPELL_UNDERWATER_BREATHING, true);
                SuppressPlayerBreath(player);
                return;
            }

            _events.CancelEvent(EVENT_COUNTDOWN_PULSE);
            _events.CancelEvent(EVENT_DETONATE);

            RestorePlayerAfterDecoyBot(player);
            SchedulePlayerReturn(player);
        }

        void SpellHit(Unit* /*caster*/, SpellInfo const* spell) OVERRIDE
        {
            // Vehicle spell1 — DBC handles pulse AoE; script only starts the detonation timer/text.
            if (spell->Id == SPELL_DEPTH_CHARGE_PULSE)
                BeginCountdown();
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (Player* player = GetControllingPlayer())
                SuppressPlayerBreath(player);

            _events.Update(diff);

            switch (_events.ExecuteEvent())
            {
                case EVENT_COUNTDOWN_PULSE:
                    if (_detonated)
                        break;

                    DoCountdownPulse();
                    if (_countdownActive)
                        _events.ScheduleEvent(EVENT_COUNTDOWN_PULSE, COUNTDOWN_PULSE_MS);
                    break;
                case EVENT_DETONATE:
                    if (!_detonated)
                        HandleDetonation();
                    break;
                default:
                    break;
            }
        }

    private:
        Player* GetControllingPlayer() const
        {
            return ObjectAccessor::GetPlayer(*me, _playerGuid);
        }

        void UseConsoleFallback()
        {
            _returnMapId = CONSOLE_MAP_ID;
            _returnX = ConsoleX;
            _returnY = ConsoleY;
            _returnZ = ConsoleZ;
            _returnO = ConsoleO;
            _hasReturnPos = true;
        }

        void SaveReturnPosition(Player* player)
        {
            float x = player->GetPositionX();
            float y = player->GetPositionY();
            float z = player->GetPositionZ();
            float o = player->GetOrientation();
            uint32 mapId = player->GetMapId();

            if (!IsValidDarkshoreReturn(mapId, x, y, z))
            {
                UseConsoleFallback();
                return;
            }

            _returnMapId = mapId;
            _returnX = x;
            _returnY = y;
            _returnZ = z;
            _returnO = o;
            _hasReturnPos = true;
        }

        void SchedulePlayerReturn(Player* player)
        {
            if (!player || _returnScheduled)
                return;

            if (!_hasReturnPos || !IsValidDarkshoreReturn(_returnMapId, _returnX, _returnY, _returnZ))
                UseConsoleFallback();

            _returnScheduled = true;

            // Confirm once: only re-teleport if still stuck underwater / far from return.
            player->m_Events.AddEvent(
                new ReturnPlayerEvent(_playerGuid, _returnMapId, _returnX, _returnY, _returnZ, _returnO, true),
                player->m_Events.CalculateTime(RETURN_CONFIRM_MS));
        }

        void BeginCountdown()
        {
            if (_countdownActive || _detonated)
                return;

            _countdownActive = true;
            _countdownTicks = 0;

            Talk(SAY_TAUNT);
            DoCountdownPulse();

            _events.ScheduleEvent(EVENT_COUNTDOWN_PULSE, COUNTDOWN_PULSE_MS);
            _events.ScheduleEvent(EVENT_DETONATE, COUNTDOWN_DURATION_MS);
        }

        void DoCountdownPulse()
        {
            if (_countdownTicks == 3)
                Talk(SAY_DETONATION_IMMINENT);

            ++_countdownTicks;
        }

        void HandleDetonation()
        {
            if (_detonated)
                return;

            _detonated = true;
            _countdownActive = false;

            _events.CancelEvent(EVENT_COUNTDOWN_PULSE);
            _events.CancelEvent(EVENT_DETONATE);

            Player* player = GetControllingPlayer();

            me->CastSpell(me, SPELL_DEPTH_CHARGE_DETONATION, true);
            FinishMurlocsInRange();

            if (player)
            {
                // ExitVehicle starts a transport-exit spline to the bot. PassengerBoarded
                // schedules a stuck-check confirm; teleport immediately after clearing motion.
                player->ExitVehicle();
                player->GetMotionMaster()->Clear(false);
                TeleportPlayerToDecoyReturn(player, _returnMapId, _returnX, _returnY, _returnZ, _returnO);
            }
            else
                me->CastSpell(me, SPELL_EJECT_ALL_PASSENGERS, true);

            me->DespawnOrUnsummon(3000);
        }

        void FinishMurlocsInRange()
        {
            std::list<Creature*> murlocs;
            me->GetCreatureListWithEntryInGrid(murlocs, NPC_GREYMIST_TIDEHUNTER, DETONATION_RANGE);
            me->GetCreatureListWithEntryInGrid(murlocs, NPC_GREYMIST_WARRIOR, DETONATION_RANGE);

            for (Creature* murloc : murlocs)
            {
                if (murloc && murloc->IsAlive())
                    murloc->Kill(me);
            }
        }

        EventMap _events;
        uint64 _playerGuid;
        uint32 _returnMapId;
        float _returnX, _returnY, _returnZ, _returnO;
        uint8 _countdownTicks;
        bool _countdownActive;
        bool _detonated;
        bool _hasReturnPos;
        bool _returnScheduled;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_foolhardy_adventurerAI(creature);
    }
};

void AddSC_darkshore()
{
    new npc_foolhardy_adventurer();
}
