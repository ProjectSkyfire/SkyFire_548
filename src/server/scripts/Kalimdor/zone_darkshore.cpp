/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "CombatAI.h"
#include "Player.h"
#include "Vehicle.h"
#include "VehicleDefines.h"
#include "EventProcessor.h"
#include "ObjectAccessor.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "MoveSpline.h"
#include "MoveSplineInit.h"
#include "SpellScript.h"

#include <cmath>
#include <iterator>
#include <set>

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

/*####
# npc_nightsaber_rider
####*/

enum NightsaberRide
{
    NPC_DARKSHORE_SENTINEL_NIGHTSABER = 33358,
    GOSSIP_MENU_NIGHTSABER_RIDER      = 10429,
    DATA_NIGHTSABER_RIDE              = 1,
};

// Road from Lor'danel over the bridge and along the coast road; the first point is the rider's post.
Position const NightsaberPathBashalAran[] =
{
    { 7242.6f, -282.2f, 21.7f }, { 7252.0f, -269.8f, 23.1f }, { 7254.3f, -261.5f, 24.1f },
    { 7258.0f, -255.3f, 22.2f }, { 7259.2f, -253.2f, 22.6f }, { 7257.4f, -251.1f, 21.1f },
    { 7252.2f, -244.7f, 20.7f }, { 7251.0f, -237.3f, 18.7f }, { 7250.2f, -232.3f, 18.3f },
    { 7246.0f, -224.3f, 18.1f }, { 7234.2f, -213.9f, 19.1f }, { 7222.4f, -203.5f, 20.0f },
    { 7212.8f, -195.0f, 21.3f }, { 7203.1f, -186.5f, 22.6f }, { 7194.2f, -175.3f, 23.4f },
    { 7179.9f, -157.4f, 21.8f }, { 7167.7f, -148.9f, 20.8f }, { 7157.3f, -137.5f, 19.2f },
    { 7146.9f, -126.1f, 17.6f }, { 7134.4f, -114.8f, 16.2f }, { 7121.9f, -103.4f, 14.8f },
    { 7105.2f, -92.8f, 13.4f }, { 7085.4f, -88.7f, 14.6f }, { 7065.6f, -84.7f, 15.9f },
    { 7042.7f, -84.8f, 16.6f }, { 7024.0f, -82.4f, 17.2f }, { 7004.2f, -82.1f, 18.5f },
    { 6984.4f, -81.8f, 19.8f }, { 6970.0f, -82.8f, 20.5f }, { 6955.5f, -83.8f, 21.3f },
    { 6938.7f, -86.2f, 21.4f }, { 6921.9f, -88.5f, 21.4f }, { 6903.8f, -86.4f, 21.3f },
    { 6885.8f, -84.2f, 21.2f }, { 6867.7f, -82.0f, 21.1f }, { 6851.1f, -71.6f, 20.2f },
    { 6839.0f, -58.0f, 20.3f }, { 6826.8f, -44.4f, 20.5f }, { 6817.3f, -27.2f, 18.1f },
    { 6812.3f, -25.8f, 17.5f }, { 6807.3f, -24.4f, 18.9f }, { 6803.2f, -21.8f, 22.3f },
    { 6797.0f, -18.0f, 24.1f },
};

// Road from Lor'danel over the eastern bridge and south along the road to the Ruins of Mathystra.
Position const NightsaberPathMathystra[] =
{
    { 7262.6f, -359.4f, 24.8f }, { 7257.6f, -360.5f, 25.2f }, { 7255.6f, -363.6f, 25.7f },
    { 7255.0f, -374.0f, 27.3f }, { 7258.1f, -380.2f, 28.5f }, { 7260.6f, -389.8f, 27.2f },
    { 7261.1f, -392.3f, 27.7f }, { 7257.7f, -401.0f, 26.0f }, { 7251.1f, -417.7f, 27.3f },
    { 7251.1f, -429.4f, 27.3f }, { 7251.1f, -442.5f, 26.4f }, { 7251.1f, -455.6f, 25.4f },
    { 7251.0f, -476.0f, 26.7f }, { 7253.1f, -494.8f, 27.6f }, { 7250.0f, -518.7f, 27.8f },
    { 7246.9f, -542.7f, 27.9f }, { 7247.9f, -555.8f, 28.7f }, { 7248.9f, -568.8f, 29.5f },
    { 7249.0f, -581.8f, 29.4f }, { 7249.0f, -594.8f, 29.3f }, { 7251.4f, -609.4f, 29.1f },
    { 7256.3f, -628.1f, 27.3f }, { 7261.1f, -638.5f, 26.7f }, { 7270.9f, -652.2f, 24.9f },
    { 7279.1f, -663.5f, 24.3f }, { 7281.0f, -668.6f, 24.1f }, { 7283.9f, -676.3f, 28.0f },
    { 7285.8f, -681.4f, 27.8f }, { 7289.7f, -701.0f, 25.2f }, { 7300.0f, -713.5f, 21.5f },
    { 7310.3f, -726.0f, 17.8f }, { 7314.4f, -743.7f, 17.0f }, { 7318.5f, -761.5f, 16.1f },
    { 7324.0f, -766.6f, 15.7f }, { 7328.1f, -767.6f, 15.6f }, { 7336.8f, -767.1f, 14.8f },
    { 7345.5f, -766.6f, 12.1f }, { 7348.4f, -766.5f, 10.2f }, { 7351.3f, -766.3f, 11.9f },
    { 7354.2f, -766.1f, 9.3f }, { 7360.0f, -765.8f, 9.8f },
};

struct NightsaberRideInfo
{
    Position Start;          // home position of the rider offering the ride
    Position const* Path;    // road points from Start to the destination
    uint32 PathSize;
    uint32 GossipOptionId;   // option in GOSSIP_MENU_NIGHTSABER_RIDER
    char const* Destination;
};

// Only the riders posted around Lor'danel offer a ride; the ones waiting at the destinations share the menu text.
NightsaberRideInfo const NightsaberRides[] =
{
    { { 7242.58f, -282.217f, 21.3077f }, NightsaberPathBashalAran, uint32(sizeof(NightsaberPathBashalAran) / sizeof(Position)), 0, "Bashal'Aran" },
    { { 7262.64f, -359.355f, 24.3714f }, NightsaberPathMathystra, uint32(sizeof(NightsaberPathMathystra) / sizeof(Position)), 1, "the Ruins of Mathystra" },
};

uint32 const MAX_NIGHTSABER_RIDES = sizeof(NightsaberRides) / sizeof(NightsaberRides[0]);

uint32 GetNightsaberRide(Creature const* rider)
{
    for (uint32 i = 0; i < MAX_NIGHTSABER_RIDES; ++i)
        if (rider->GetHomePosition().GetExactDist2d(&NightsaberRides[i].Start) < 10.0f)
            return i;
    return MAX_NIGHTSABER_RIDES;
}

class npc_nightsaber_rider : public CreatureScript
{
public:
    npc_nightsaber_rider() : CreatureScript("npc_nightsaber_rider") { }

    bool OnGossipHello(Player* player, Creature* creature) OVERRIDE
    {
        player->PlayerTalkClass->ClearMenus();
        uint32 ride = GetNightsaberRide(creature);
        if (ride < MAX_NIGHTSABER_RIDES)
            player->ADD_GOSSIP_ITEM_DB(GOSSIP_MENU_NIGHTSABER_RIDER, NightsaberRides[ride].GossipOptionId, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + ride);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action) OVERRIDE
    {
        player->PlayerTalkClass->SendCloseGossip();
        uint32 ride = action - GOSSIP_ACTION_INFO_DEF;
        if (player->GetVehicle() || ride >= MAX_NIGHTSABER_RIDES || ride != GetNightsaberRide(creature))
            return true;

        if (Creature* saber = player->SummonCreature(NPC_DARKSHORE_SENTINEL_NIGHTSABER, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ(), creature->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 5 * MINUTE * IN_MILLISECONDS))
        {
            saber->AI()->SetData(DATA_NIGHTSABER_RIDE, ride);
            player->EnterVehicle(saber, 0);
        }
        return true;
    }
};

class npc_darkshore_nightsaber : public CreatureScript
{
public:
    npc_darkshore_nightsaber() : CreatureScript("npc_darkshore_nightsaber") { }

    struct npc_darkshore_nightsaberAI : public ScriptedAI
    {
        npc_darkshore_nightsaberAI(Creature* creature) : ScriptedAI(creature), _ride(0), _started(false), _arrived(false) { }

        void Reset() OVERRIDE
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void SetData(uint32 type, uint32 data) OVERRIDE
        {
            if (type == DATA_NIGHTSABER_RIDE && data < MAX_NIGHTSABER_RIDES)
                _ride = data;
        }

        void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) OVERRIDE
        {
            if (passenger->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            if (apply)
            {
                if (_started)
                    return;
                _started = true;

                Movement::PointsArray path = BuildPath();
                if (path.size() < 2)
                {
                    SF_LOG_ERROR("scripts", "npc_darkshore_nightsaber: no path from (%f, %f, %f) to %s",
                        me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), NightsaberRides[_ride].Destination);
                    _arrived = true;
                    if (Vehicle* vehicle = me->GetVehicleKit())
                        vehicle->RemoveAllPassengers();
                    me->DespawnOrUnsummon(1000);
                    return;
                }

                me->SetSpeed(MOVE_RUN, 2.0f);
                Movement::MoveSplineInit init(me);
                init.MovebyPath(path);
                init.SetWalk(false);
                // The route spans more than the packed spline offsets can hold.
                init.SetUncompressed();
                init.Launch();
            }
            else if (!_arrived)
            {
                _arrived = true;
                me->StopMoving();
                me->DespawnOrUnsummon(1000);
            }
        }

        void UpdateAI(uint32 /*diff*/) OVERRIDE
        {
            if (!_started || _arrived || !me->movespline->Finalized())
                return;

            _arrived = true;
            if (Vehicle* vehicle = me->GetVehicleKit())
                vehicle->RemoveAllPassengers();
            me->DespawnOrUnsummon(1000);
        }

    private:
        // Follows the road points; the first one is replaced by the cat's actual position.
        Movement::PointsArray BuildPath() const
        {
            NightsaberRideInfo const& ride = NightsaberRides[_ride];
            Movement::PointsArray path;
            path.reserve(ride.PathSize);
            path.push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));
            for (uint32 i = 1; i < ride.PathSize; ++i)
                path.push_back(G3D::Vector3(ride.Path[i].GetPositionX(), ride.Path[i].GetPositionY(), ride.Path[i].GetPositionZ()));
            return path;
        }

        uint32 _ride;
        bool _started;
        bool _arrived;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_darkshore_nightsaberAI(creature);
    }
};

enum SoothingRitual
{
    NPC_FIRE_ELEMENTAL_REMNANT = 34368,
    NPC_FIRE_ELEMENTAL_RAGER   = 34370,
    NPC_SOOTHING_TOTEM_CREDIT  = 34371,
    NPC_RITUAL_FIRE_TRIGGER    = 24042,

    SAY_RITUAL_BEGUN     = 0,
    SAY_RITUAL_COMPLETE  = 1,

    EVENT_SUMMON_REMNANT = 1,
    EVENT_SUMMON_RAGER   = 2,

    RITUAL_REMNANT_COUNT = 5,
    RITUAL_END_DESPAWN   = 4000,
    RITUAL_REMNANT_FIRST = 3000,
    RITUAL_REMNANT_GAP   = 8000,
    RITUAL_RAGER_AT      = 48000,
    RITUAL_TRIGGER_RANGE = 50
};

// Quest 13580. The placed totem stays level 14. Remnants arrive over about a minute,
// then a rager. Credit is given only if the totem is still standing when the rager dies.
class npc_darkshore_soothing_totem : public CreatureScript
{
public:
    npc_darkshore_soothing_totem() : CreatureScript("npc_darkshore_soothing_totem") { }

    struct npc_darkshore_soothing_totemAI : public ScriptedAI
    {
        npc_darkshore_soothing_totemAI(Creature* creature) : ScriptedAI(creature), _summons(creature), _remnants(0), _ragerGuid(0), _credited(false) { }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            if (summoner)
            {
                me->setFaction(summoner->getFaction());
                if (summoner->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(SAY_RITUAL_BEGUN, summoner);
            }

            // UNIT_FLAG_IMMUNE_TO_PC would also make the totem untargetable for the elementals.
            me->SetReactState(REACT_PASSIVE);
            me->SetControlled(true, UNIT_STATE_ROOT);

            _events.ScheduleEvent(EVENT_SUMMON_REMNANT, RITUAL_REMNANT_FIRST);
            _events.ScheduleEvent(EVENT_SUMMON_RAGER, RITUAL_RAGER_AT);
        }

        void AttackStart(Unit* /*target*/) OVERRIDE { }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            _summons.Summon(summon);
            if (summon->GetEntry() == NPC_FIRE_ELEMENTAL_RAGER)
                _ragerGuid = summon->GetGUID();

            summon->AI()->AttackStart(me);
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) OVERRIDE
        {
            if (_credited || summon->GetGUID() != _ragerGuid)
                return;

            _credited = true;
            if (TempSummon* ritual = me->ToTempSummon())
                if (Unit* owner = ritual->GetSummoner())
                    if (Player* player = owner->ToPlayer())
                    {
                        player->KilledMonsterCredit(NPC_SOOTHING_TOTEM_CREDIT);
                        Talk(SAY_RITUAL_COMPLETE, player);
                    }

            // Leave time for the rager's death animation before the scene is cleared.
            for (SummonList::const_iterator itr = _summons.begin(); itr != _summons.end(); ++itr)
                if (Creature* attacker = ObjectAccessor::GetCreature(*me, *itr))
                    attacker->DespawnOrUnsummon(RITUAL_END_DESPAWN);
            me->DespawnOrUnsummon(RITUAL_END_DESPAWN);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            _credited = true;
            _summons.DespawnAll();
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (_credited)
                return;

            _events.Update(diff);
            while (uint32 eventId = _events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SUMMON_REMNANT:
                        SummonAttacker(NPC_FIRE_ELEMENTAL_REMNANT);
                        if (++_remnants < RITUAL_REMNANT_COUNT)
                            _events.ScheduleEvent(EVENT_SUMMON_REMNANT, RITUAL_REMNANT_GAP);
                        break;
                    case EVENT_SUMMON_RAGER:
                        SummonAttacker(NPC_FIRE_ELEMENTAL_RAGER);
                        break;
                    default:
                        break;
                }
            }
        }

    private:
        // Elementals rise from the fire triggers around the pool; away from them they appear near the totem.
        void SummonAttacker(uint32 entry)
        {
            std::list<Creature*> triggers;
            me->GetCreatureListWithEntryInGrid(triggers, NPC_RITUAL_FIRE_TRIGGER, RITUAL_TRIGGER_RANGE);

            Position pos;
            if (!triggers.empty())
            {
                std::list<Creature*>::const_iterator itr = triggers.begin();
                std::advance(itr, urand(0, uint32(triggers.size()) - 1));
                (*itr)->GetPosition(&pos);
            }
            else
                me->GetNearPosition(pos, frand(8.0f, 14.0f), frand(0.0f, 2.0f * float(M_PI)));

            me->SummonCreature(entry, pos, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000);
        }

        EventMap _events;
        SummonList _summons;
        uint8 _remnants;
        uint64 _ragerGuid;
        bool _credited;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_darkshore_soothing_totemAI(creature);
    }
};

enum LeavingTheDream
{
    QUEST_LEAVING_THE_DREAM  = 13940,
    NPC_THESSERA_DREAM_RIDE  = 34400,
    SPELL_INTO_THE_NIGHTMARE = 65409,
    DREAM_EXIT_POINT         = 14, // cave mouth; the rest of the flight is in the waking world
    DREAM_LEG_RETRIES        = 3,
    DREAM_RIDE_SEAT          = 1, // vehicle 2086 only has a seat in slot 1
    DREAM_BOARD_DELAY        = 2000,
    PHASE_NIGHTMARE          = 170,
    PHASE_DEFAULT            = 169
};

float const DreamLegArrivalDist = 6.0f;
Position const ThesseraDreamSpawnPos = { 5773.186f, -165.2984f, 15.97325f, 5.35829f };

// Thessera (33166) puts the player on her flying form (34400) when Leaving the Dream is accepted.
class npc_thessera_dream : public CreatureScript
{
public:
    npc_thessera_dream() : CreatureScript("npc_thessera_dream") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) OVERRIDE
    {
        if (quest->GetQuestId() != QUEST_LEAVING_THE_DREAM || player->GetVehicle())
            return false;

        if (Creature* ride = player->SummonCreature(NPC_THESSERA_DREAM_RIDE, ThesseraDreamSpawnPos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 5 * MINUTE * IN_MILLISECONDS))
            ride->AI()->SetGUID(player->GetGUID());
        return false;
    }
};

// Thessera (34400) carries the player out of the Nightmare cave up to Malfurion's grove.
Position const ThesseraDreamPath[] =
{
    { 5806.10f, -148.111f,  8.207f }, { 5815.71f, -165.248f,  0.874f }, { 5840.48f, -194.410f,  1.207f },
    { 5844.54f, -207.028f,  1.207f }, { 5841.51f, -221.007f,  6.210f }, { 5816.73f, -213.559f,  6.210f },
    { 5815.36f, -196.439f, 12.926f }, { 5831.71f, -179.179f, 16.926f }, { 5839.44f, -163.825f, 19.371f },
    { 5835.43f, -146.861f, 19.371f }, { 5816.85f, -117.330f, 19.371f }, { 5808.97f, -103.175f, 23.884f },
    { 5816.47f,  -69.884f, 27.747f }, { 5812.03f,  -43.106f, 27.747f }, { 5760.73f,   57.283f, 38.497f },
    { 5786.31f,  117.290f, 53.663f }, { 5844.93f,  132.576f, 61.413f }, { 5867.70f,  146.859f, 72.832f },
    { 5901.92f,  188.635f, 68.831f },
};

uint32 const THESSERA_DREAM_PATH_SIZE = sizeof(ThesseraDreamPath) / sizeof(Position);

class npc_thessera_dream_ride : public CreatureScript
{
public:
    npc_thessera_dream_ride() : CreatureScript("npc_thessera_dream_ride") { }

    struct npc_thessera_dream_rideAI : public ScriptedAI
    {
        npc_thessera_dream_rideAI(Creature* creature) : ScriptedAI(creature), _passengerGuid(0), _boardTimer(0), _leg(0), _retries(0), _arrived(false) { }

        void Reset() OVERRIDE
        {
            me->SetReactState(REACT_PASSIVE);
        }

        void SetGUID(uint64 guid, int32 /*id*/) OVERRIDE
        {
            _passengerGuid = guid;
            _boardTimer = DREAM_BOARD_DELAY;
            // Stay visible to the rider whether or not the rider is still in the Nightmare phase:
            // a passenger whose client loses sight of the vehicle is thrown off it.
            me->SetPhased(PHASE_NIGHTMARE, false, true);
            me->SetPhased(PHASE_DEFAULT, true, true);
        }

        void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) OVERRIDE
        {
            if (passenger->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            if (apply)
            {
                if (_leg)
                    return;

                _passengerGuid = passenger->GetGUID();
                passenger->CombatStop(true);
                passenger->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                me->SetSpeed(MOVE_FLIGHT, 2.4f);
                me->SetSpeed(MOVE_RUN, 2.4f);
                FlyLeg(0, DREAM_EXIT_POINT);
                _leg = 1;
            }
            else
            {
                // Never leave the player stuck in the Nightmare phase.
                passenger->RemoveAurasDueToSpell(SPELL_INTO_THE_NIGHTMARE);
                passenger->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                if (!_arrived)
                {
                    _arrived = true;
                    me->StopMoving();
                    me->DespawnOrUnsummon(1000);
                }
            }
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (_boardTimer)
            {
                if (_boardTimer <= diff)
                {
                    _boardTimer = 0;
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _passengerGuid))
                        player->EnterVehicle(me, DREAM_RIDE_SEAT);
                    else
                        me->DespawnOrUnsummon();
                }
                else
                    _boardTimer -= diff;
                return;
            }

            if (!_leg || _arrived || !me->movespline->Finalized())
                return;

            uint32 const legEnd = _leg == 1 ? DREAM_EXIT_POINT : THESSERA_DREAM_PATH_SIZE - 1;
            if (me->GetExactDist(&ThesseraDreamPath[legEnd]) > DreamLegArrivalDist && _retries < DREAM_LEG_RETRIES)
            {
                // Something else stopped the flight early; pick the leg up again from here.
                ++_retries;
                FlyLeg(NearestPathPoint(_leg == 1 ? 0 : DREAM_EXIT_POINT, legEnd), legEnd);
                return;
            }

            if (_leg == 1)
            {
                // Enter the rider's waking phases before the rider drops the Nightmare one: if the client loses
                // sight of the vehicle for a single update it throws the passenger off.
                if (Player* player = ObjectAccessor::GetPlayer(*me, _passengerGuid))
                {
                    std::set<uint32> const phases = player->GetPhases();
                    for (uint32 phase : phases)
                        if (phase != PHASE_NIGHTMARE)
                            me->SetPhased(phase, false, true);
                    me->SetPhased(PHASE_DEFAULT, true, true);
                    player->RemoveAurasDueToSpell(SPELL_INTO_THE_NIGHTMARE);
                }
                me->SetPhased(PHASE_NIGHTMARE, true, false);

                _retries = 0;
                FlyLeg(DREAM_EXIT_POINT, THESSERA_DREAM_PATH_SIZE - 1);
                _leg = 2;
                return;
            }

            _arrived = true;
            if (Vehicle* vehicle = me->GetVehicleKit())
                vehicle->RemoveAllPassengers();
            me->DespawnOrUnsummon(2000);
        }

    private:
        uint32 NearestPathPoint(uint32 first, uint32 last) const
        {
            uint32 nearest = first;
            for (uint32 i = first + 1; i <= last; ++i)
                if (me->GetExactDist(&ThesseraDreamPath[i]) < me->GetExactDist(&ThesseraDreamPath[nearest]))
                    nearest = i;
            return nearest;
        }

        void FlyLeg(uint32 first, uint32 last)
        {
            Movement::PointsArray path;
            path.push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));
            for (uint32 i = first; i <= last; ++i)
                path.push_back(G3D::Vector3(ThesseraDreamPath[i].GetPositionX(), ThesseraDreamPath[i].GetPositionY(), ThesseraDreamPath[i].GetPositionZ()));

            Movement::MoveSplineInit init(me);
            init.MovebyPath(path);
            init.SetFly();
            init.SetUncompressed();
            init.Launch();
        }

        uint64 _passengerGuid;
        uint32 _boardTimer;
        uint8 _leg;
        uint8 _retries;
        bool _arrived;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_thessera_dream_rideAI(creature);
    }
};

enum EyeOfAllStorms
{
    QUEST_EYE_OF_ALL_STORMS = 13588,
    NPC_TWILIGHT_RIDER      = 34282,
    SPELL_LAND_THESSERA     = 65579,
    ACTION_EYE_RIDE_LAND    = 1,
    EYE_RIDE_BOARD_DELAY    = 500,
    EYE_RIDE_TAKEOFF_DELAY  = 1500,
    EYE_RIDE_LAND_DELAY     = 500,
    EYE_RIDE_DESPAWN_DELAY  = 1000,
    EYE_RIDE_SEAT           = 0,
    EYE_CIRCLE_POINTS       = 16,
    EYE_PORTAL_MAX_RIDERS   = 5,
    EYE_PORTAL_RIDER_TIMER  = 20000,
    EYE_PORTAL_RIDER_DESPAWN = 180000
};

enum EyeRidePhase
{
    EYE_PHASE_NONE,
    EYE_PHASE_TAKEOFF,
    EYE_PHASE_RISE,
    EYE_PHASE_CIRCLE,
    EYE_PHASE_RETURN,
    EYE_PHASE_DONE
};

// Beside Thessera (34401) on the Eye of the Vortex, where the flight starts.
Position const ThesseraEyeLandingPos = { 5903.43f, 197.62f, 61.28f, 0.79f };
// Thessera circles above the Twilight Portal, inside Emerald Barrage range of the portal and the riders.
Position const ThesseraEyeCircleCenter = { 5912.9f, 181.5f, 285.0f, 0.0f };
float const ThesseraEyeCircleRadius = 60.0f;
float const ThesseraEyeCruiseZ = 140.0f;

// Thessera (34243): flies the rider up to the Twilight Portal and circles it while the rider uses
// Emerald Barrage. Land Thessera, or finishing the quest, flies back down to the Eye of the Vortex.
class npc_thessera_eye_ride : public CreatureScript
{
public:
    npc_thessera_eye_ride() : CreatureScript("npc_thessera_eye_ride") { }

    struct npc_thessera_eye_rideAI : public ScriptedAI
    {
        npc_thessera_eye_rideAI(Creature* creature) : ScriptedAI(creature), _riderGuid(0), _boardTimer(0), _phaseTimer(0), _phase(EYE_PHASE_NONE) { }

        void Reset() OVERRIDE
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetPower(POWER_ENERGY, me->GetMaxPower(POWER_ENERGY));
        }

        // The rider takes the control seat, which charms Thessera; keep this AI so it can fly her and land.
        void OnCharmed(bool /*apply*/) OVERRIDE { }

        // Ride Thessera asks for any free seat, but this seat is only usable when named, so that boarding is
        // cancelled; seat the summoner explicitly once the cancelled attempt has been cleaned up.
        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            if (summoner->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            _riderGuid = summoner->GetGUID();
            _boardTimer = EYE_RIDE_BOARD_DELAY;
            me->SetPower(POWER_ENERGY, me->GetMaxPower(POWER_ENERGY));
        }

        void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) OVERRIDE
        {
            if (passenger->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            if (apply)
            {
                _riderGuid = passenger->GetGUID();
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                me->SetSpeed(MOVE_FLIGHT, 2.5f);
                me->SetPower(POWER_ENERGY, me->GetMaxPower(POWER_ENERGY));
                _phase = EYE_PHASE_TAKEOFF;
                _phaseTimer = EYE_RIDE_TAKEOFF_DELAY;
                return;
            }

            // Left before touching down: put the rider back on the Eye instead of dropping them from the sky.
            if (_phase != EYE_PHASE_DONE)
                if (Player* player = passenger->ToPlayer())
                    player->NearTeleportTo(ThesseraEyeLandingPos.GetPositionX(), ThesseraEyeLandingPos.GetPositionY(), ThesseraEyeLandingPos.GetPositionZ(), ThesseraEyeLandingPos.GetOrientation());

            _phase = EYE_PHASE_DONE;
            me->DespawnOrUnsummon(EYE_RIDE_DESPAWN_DELAY);
        }

        void DoAction(int32 action) OVERRIDE
        {
            if (action == ACTION_EYE_RIDE_LAND)
                Land();
        }

        void SpellHit(Unit* /*caster*/, SpellInfo const* spell) OVERRIDE
        {
            if (spell->Id == SPELL_LAND_THESSERA)
                Land();
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (_boardTimer)
            {
                if (_boardTimer > diff)
                {
                    _boardTimer -= diff;
                    return;
                }

                _boardTimer = 0;
                Player* player = ObjectAccessor::GetPlayer(*me, _riderGuid);
                if (player && !player->GetVehicle())
                    player->EnterVehicle(me, EYE_RIDE_SEAT);
                else if (!player)
                    me->DespawnOrUnsummon();
                return;
            }

            switch (_phase)
            {
                case EYE_PHASE_TAKEOFF:
                    if (_phaseTimer > diff)
                    {
                        _phaseTimer -= diff;
                        break;
                    }
                    _phaseTimer = 0;
                    Rise();
                    break;
                case EYE_PHASE_RISE:
                    if (me->movespline->Finalized())
                        Circle();
                    break;
                case EYE_PHASE_CIRCLE:
                    if (Player* player = ObjectAccessor::GetPlayer(*me, _riderGuid))
                        if (player->GetQuestStatus(QUEST_EYE_OF_ALL_STORMS) == QUEST_STATUS_COMPLETE)
                            Land();
                    break;
                case EYE_PHASE_RETURN:
                    if (!me->movespline->Finalized())
                        break;
                    if (_phaseTimer > diff)
                    {
                        _phaseTimer -= diff;
                        break;
                    }
                    _phaseTimer = 0;
                    _phase = EYE_PHASE_DONE;
                    if (Vehicle* vehicle = me->GetVehicleKit())
                        vehicle->RemoveAllPassengers();
                    me->DespawnOrUnsummon(EYE_RIDE_DESPAWN_DELAY);
                    break;
                default:
                    break;
            }
        }

    private:
        float AngleFromCircleCenter() const
        {
            return std::atan2(me->GetPositionY() - ThesseraEyeCircleCenter.GetPositionY(), me->GetPositionX() - ThesseraEyeCircleCenter.GetPositionX());
        }

        G3D::Vector3 CirclePoint(float angle) const
        {
            return G3D::Vector3(ThesseraEyeCircleCenter.GetPositionX() + ThesseraEyeCircleRadius * std::cos(angle),
                ThesseraEyeCircleCenter.GetPositionY() + ThesseraEyeCircleRadius * std::sin(angle), ThesseraEyeCircleCenter.GetPositionZ());
        }

        void FlyPath(Movement::PointsArray const& path, bool cyclic)
        {
            Movement::MoveSplineInit init(me);
            init.MovebyPath(path);
            init.SetFly();
            init.SetUncompressed();
            if (cyclic)
            {
                init.SetSmooth();
                init.SetCyclic();
            }
            init.Launch();
        }

        void Rise()
        {
            _phase = EYE_PHASE_RISE;
            // The rider only aims and casts; their client must not fight the flight path.
            if (Player* player = ObjectAccessor::GetPlayer(*me, _riderGuid))
                player->SetClientControl(me, 0);
            Movement::PointsArray path;
            path.push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));
            path.push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), ThesseraEyeCruiseZ));
            path.push_back(CirclePoint(AngleFromCircleCenter()));
            FlyPath(path, false);
        }

        // The cyclic spline loops back to its first point, which is where Thessera already is.
        void Circle()
        {
            _phase = EYE_PHASE_CIRCLE;
            float const start = AngleFromCircleCenter();
            Movement::PointsArray path;
            path.push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));
            for (uint32 i = 1; i < EYE_CIRCLE_POINTS; ++i)
                path.push_back(CirclePoint(start + 2.0f * float(M_PI) * i / float(EYE_CIRCLE_POINTS)));
            FlyPath(path, true);
        }

        void Land()
        {
            if (_phase == EYE_PHASE_RETURN || _phase == EYE_PHASE_DONE)
                return;

            _phase = EYE_PHASE_RETURN;
            _phaseTimer = EYE_RIDE_LAND_DELAY;
            Movement::PointsArray path;
            path.push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));
            path.push_back(G3D::Vector3(ThesseraEyeLandingPos.GetPositionX(), ThesseraEyeLandingPos.GetPositionY(), ThesseraEyeCruiseZ));
            path.push_back(G3D::Vector3(ThesseraEyeLandingPos.GetPositionX(), ThesseraEyeLandingPos.GetPositionY(), ThesseraEyeLandingPos.GetPositionZ() + 1.0f));
            FlyPath(path, false);
        }

        uint64 _riderGuid;
        uint32 _boardTimer;
        uint32 _phaseTimer;
        EyeRidePhase _phase;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_thessera_eye_rideAI(creature);
    }
};

// 65579 - Land Thessera
class spell_darkshore_land_thessera : public SpellScriptLoader
{
public:
    spell_darkshore_land_thessera() : SpellScriptLoader("spell_darkshore_land_thessera") { }

    class spell_darkshore_land_thessera_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_darkshore_land_thessera_SpellScript);

        void HandleDummy(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (Creature* thessera = GetCaster()->ToCreature())
                if (thessera->IsAIEnabled)
                    thessera->AI()->DoAction(ACTION_EYE_RIDE_LAND);
        }

        void Register() OVERRIDE
        {
            OnEffectHit += SpellEffectFn(spell_darkshore_land_thessera_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_darkshore_land_thessera_SpellScript();
    }
};

// 65063 - Emerald Barrage
class spell_darkshore_emerald_barrage : public SpellScriptLoader
{
public:
    spell_darkshore_emerald_barrage() : SpellScriptLoader("spell_darkshore_emerald_barrage") { }

    class spell_darkshore_emerald_barrage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_darkshore_emerald_barrage_SpellScript);

        // The spell accepts any unit, so without a hostile target it lands on the rider.
        SpellCastResult CheckTarget()
        {
            Unit* target = GetExplTargetUnit();
            if (!target || !GetCaster()->IsValidAttackTarget(target))
                return SpellCastResult::SPELL_FAILED_BAD_TARGETS;
            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() OVERRIDE
        {
            OnCheckCast += SpellCheckCastFn(spell_darkshore_emerald_barrage_SpellScript::CheckTarget);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_darkshore_emerald_barrage_SpellScript();
    }
};

// Twilight Portal (34316): sends Twilight Riders out over the Eye of the Vortex while it stands.
class npc_darkshore_twilight_portal : public CreatureScript
{
public:
    npc_darkshore_twilight_portal() : CreatureScript("npc_darkshore_twilight_portal") { }

    struct npc_darkshore_twilight_portalAI : public ScriptedAI
    {
        npc_darkshore_twilight_portalAI(Creature* creature) : ScriptedAI(creature), _summons(creature), _riderTimer(EYE_PORTAL_RIDER_TIMER) { }

        void Reset() OVERRIDE
        {
            me->SetReactState(REACT_PASSIVE);
            _riderTimer = EYE_PORTAL_RIDER_TIMER;
        }

        void JustSummoned(Creature* summon) OVERRIDE
        {
            _summons.Summon(summon);

            float const angle = frand(0.0f, 2.0f * float(M_PI));
            float const dist = frand(30.0f, 80.0f);
            summon->SetCanFly(true);
            summon->SetDisableGravity(true);
            Position post;
            post.Relocate(me->GetPositionX() + dist * std::cos(angle), me->GetPositionY() + dist * std::sin(angle), frand(285.0f, 315.0f), angle);
            summon->SetHomePosition(post);
            summon->GetMotionMaster()->MovePoint(0, post);
        }

        void SummonedCreatureDespawn(Creature* summon) OVERRIDE
        {
            _summons.Despawn(summon);
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) OVERRIDE
        {
            _summons.Despawn(summon);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (_riderTimer > diff)
            {
                _riderTimer -= diff;
                return;
            }

            _riderTimer = EYE_PORTAL_RIDER_TIMER;
            if (_summons.size() < EYE_PORTAL_MAX_RIDERS)
                me->SummonCreature(NPC_TWILIGHT_RIDER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, EYE_PORTAL_RIDER_DESPAWN);
        }

    private:
        SummonList _summons;
        uint32 _riderTimer;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_darkshore_twilight_portalAI(creature);
    }
};

enum MastersGlaiveSpies
{
    QUEST_LEAVE_NO_TRACKS           = 13892,
    QUEST_STEPPING_UP_SURVEILLANCE  = 13948,
    NPC_TWILIGHT_WORKER             = 34405,
    NPC_THALYA_THE_VOIDCALLER       = 34427,
    NPC_INTELLIGENCE_CREDIT_1       = 34410,
    NPC_INTELLIGENCE_CREDIT_2       = 34411,
    SPELL_PANTHER_FIGURINE          = 65426,
    DISPLAY_TWILIGHT_WORKER_FEMALE  = 2869,
    SAY_WORKER_WHY_DIG              = 0,
    SAY_BALSOTH_WATCH_TONGUE        = 0,
    SAY_BALSOTH_BACK_TO_WORK        = 1,
    SAY_TREVELLION_RITUAL           = 0,
    SAY_TREVELLION_SACRIFICE        = 1,
    SAY_THALYA_OFFERINGS            = 0,
    SPY_CHECK_INTERVAL              = 1000,
    SPY_LINE_DELAY                  = 7000,
    SPY_COOLDOWN                    = 20000,
    WORKER_WALK_TIME                = 4000,
    WORKER_LINE_DELAY               = 5000,
    WORKER_RETURN_DELAY             = 3000
};

float const SpyListenRange = 30.0f;
float const WorkerSearchRange = 20.0f;

static bool IsMastersGlaiveSpy(Player* player, uint32 questId)
{
    return player->IsAlive() && player->HasAura(SPELL_PANTHER_FIGURINE)
        && player->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE;
}

static bool HasMastersGlaiveSpyNearby(Creature* creature, uint32 questId)
{
    std::list<Player*> players;
    creature->GetPlayerListInGrid(players, SpyListenRange);
    for (Player* player : players)
        if (IsMastersGlaiveSpy(player, questId))
            return true;
    return false;
}

static void CreditMastersGlaiveSpies(Creature* creature, uint32 questId, uint32 creditEntry)
{
    std::list<Player*> players;
    creature->GetPlayerListInGrid(players, SpyListenRange);
    for (Player* player : players)
        if (IsMastersGlaiveSpy(player, questId))
            player->KilledMonsterCredit(creditEntry);
}

// Foreman Balsoth (34406): when a disguised spy on Leave No Tracks sneaks into the dig, a worker walks up and
// asks why they are digging Soggoth up; Balsoth puts her in her place and everyone listening in gets the credit.
class npc_foreman_balsoth : public CreatureScript
{
public:
    npc_foreman_balsoth() : CreatureScript("npc_foreman_balsoth") { }

    struct npc_foreman_balsothAI : public ScriptedAI
    {
        npc_foreman_balsothAI(Creature* creature) : ScriptedAI(creature), _timer(SPY_CHECK_INTERVAL), _step(0) { }

        void Reset() OVERRIDE
        {
            SendWorkerHome();
            _timer = SPY_CHECK_INTERVAL;
            _step = 0;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            SendWorkerHome();
            _step = 0;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (UpdateVictim())
            {
                DoMeleeAttackIfReady();
                return;
            }

            if (_timer > diff)
            {
                _timer -= diff;
                return;
            }

            switch (_step)
            {
                case 0:
                {
                    _timer = SPY_CHECK_INTERVAL;
                    if (!HasMastersGlaiveSpyNearby(me, QUEST_LEAVE_NO_TRACKS))
                        return;
                    if (Creature* worker = SelectWorker())
                    {
                        _workerGUID = worker->GetGUID();
                        Position pos;
                        me->GetNearPosition(pos, 2.5f, me->GetRelativeAngle(worker));
                        worker->SetWalk(true);
                        worker->GetMotionMaster()->MovePoint(0, pos);
                    }
                    _timer = WORKER_WALK_TIME;
                    break;
                }
                case 1:
                    if (Creature* worker = GetWorker())
                    {
                        me->SetFacingToObject(worker);
                        worker->SetFacingToObject(me);
                        worker->AI()->Talk(SAY_WORKER_WHY_DIG);
                    }
                    _timer = WORKER_LINE_DELAY;
                    break;
                case 2:
                    Talk(SAY_BALSOTH_WATCH_TONGUE);
                    _timer = SPY_LINE_DELAY;
                    break;
                case 3:
                    Talk(SAY_BALSOTH_BACK_TO_WORK);
                    CreditMastersGlaiveSpies(me, QUEST_LEAVE_NO_TRACKS, NPC_INTELLIGENCE_CREDIT_1);
                    _timer = WORKER_RETURN_DELAY;
                    break;
                case 4:
                    SendWorkerHome();
                    me->SetFacingTo(me->GetHomePosition().GetOrientation());
                    _timer = SPY_COOLDOWN;
                    _step = 0;
                    return;
            }

            ++_step;
        }

    private:
        Creature* SelectWorker() const
        {
            std::list<Creature*> workers;
            me->GetCreatureListWithEntryInGrid(workers, NPC_TWILIGHT_WORKER, WorkerSearchRange);
            Creature* nearest = NULL;
            Creature* nearestFemale = NULL;
            for (Creature* worker : workers)
            {
                if (!worker->IsAlive() || worker->IsInCombat() || !worker->IsAIEnabled)
                    continue;
                if (!nearest || me->GetDistance(worker) < me->GetDistance(nearest))
                    nearest = worker;
                if (worker->GetDisplayId() == DISPLAY_TWILIGHT_WORKER_FEMALE
                    && (!nearestFemale || me->GetDistance(worker) < me->GetDistance(nearestFemale)))
                    nearestFemale = worker;
            }
            return nearestFemale ? nearestFemale : nearest;
        }

        Creature* GetWorker() const
        {
            Creature* worker = _workerGUID ? ObjectAccessor::GetCreature(*me, _workerGUID) : NULL;
            return worker && worker->IsAlive() && worker->IsAIEnabled && !worker->IsInCombat() ? worker : NULL;
        }

        void SendWorkerHome()
        {
            if (Creature* worker = GetWorker())
                worker->GetMotionMaster()->MoveTargetedHome();
            _workerGUID = 0;
        }

        uint32 _timer;
        uint8 _step;
        uint64 _workerGUID = 0;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_foreman_balsothAI(creature);
    }
};

// Doomspeaker Trevellion (34326): up on the platform he talks the ritual over with Thalya the Voidcaller when a
// disguised spy on Stepping Up Surveillance creeps close; everyone listening in gets the surveillance credit.
class npc_doomspeaker_trevellion : public CreatureScript
{
public:
    npc_doomspeaker_trevellion() : CreatureScript("npc_doomspeaker_trevellion") { }

    struct npc_doomspeaker_trevellionAI : public ScriptedAI
    {
        npc_doomspeaker_trevellionAI(Creature* creature) : ScriptedAI(creature), _timer(SPY_CHECK_INTERVAL), _step(0) { }

        void Reset() OVERRIDE
        {
            _timer = SPY_CHECK_INTERVAL;
            _step = 0;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _step = 0;
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (UpdateVictim())
            {
                DoMeleeAttackIfReady();
                return;
            }

            if (_timer > diff)
            {
                _timer -= diff;
                return;
            }

            switch (_step)
            {
                case 0:
                    _timer = SPY_CHECK_INTERVAL;
                    if (!HasMastersGlaiveSpyNearby(me, QUEST_STEPPING_UP_SURVEILLANCE))
                        return;
                    Talk(SAY_TREVELLION_RITUAL);
                    break;
                case 1:
                    _timer = SPY_LINE_DELAY;
                    if (Creature* thalya = me->FindNearestCreature(NPC_THALYA_THE_VOIDCALLER, 20.0f))
                        if (thalya->IsAIEnabled)
                            thalya->AI()->Talk(SAY_THALYA_OFFERINGS);
                    break;
                case 2:
                    Talk(SAY_TREVELLION_SACRIFICE);
                    break;
                case 3:
                    CreditMastersGlaiveSpies(me, QUEST_STEPPING_UP_SURVEILLANCE, NPC_INTELLIGENCE_CREDIT_2);
                    _timer = SPY_COOLDOWN;
                    _step = 0;
                    return;
            }

            _timer = SPY_LINE_DELAY;
            ++_step;
        }

    private:
        uint32 _timer;
        uint8 _step;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_doomspeaker_trevellionAI(creature);
    }
};

enum InDefenseOfDarkshore
{
    QUEST_IN_DEFENSE_OF_DARKSHORE       = 13885,
    NPC_KATHRENA_WINTERWISP_FLIGHT      = 34376,
    NPC_GRIZZLED_THISTLE_BEAR           = 2165,
    NPC_YOUNG_GRIZZLED_THISTLE_BEAR     = 34417,
    NPC_WHITETAIL_STAG                  = 34318,
    NPC_WHITETAIL_DOE                   = 34396,
    NPC_MOONSTALKER_MATRIARCH           = 2071,
    NPC_MOONSTALKER_SIRE                = 2237,
    NPC_THISTLE_BEAR_PROTECTED_CREDIT   = 34323,
    NPC_WHITETAIL_PROTECTED_CREDIT      = 34324,
    NPC_MOONSTALKER_PROTECTED_CREDIT    = 34325,
    SPELL_PROTECTED_FROM_CONSUMPTION    = 65193,
    DISPLAY_GROVE_HIPPOGRYPH_MOUNT      = 22473,

    SAY_KATHRENA_HOLD_ON                = 0,
    SAY_KATHRENA_FROM_THE_AIR           = 1,
    SAY_KATHRENA_DEER                   = 2,
    SAY_KATHRENA_MOONSTALKERS           = 3,
    SAY_KATHRENA_UPDRAFT                = 4,
    SAY_KATHRENA_THISTLE_BEARS          = 5,
    SAY_KATHRENA_DIG_SITE               = 6,
    SAY_KATHRENA_GOOD_WORK              = 7,
    SAY_NONE                            = -1,

    HIPPOGRYPH_SEAT                     = 0,
    HIPPOGRYPH_BOARD_DELAY              = 500,
    HIPPOGRYPH_TAKEOFF_DELAY            = 3000,
    HIPPOGRYPH_DESPAWN_DELAY            = 1000,
    KATHRENA_DESPAWN_DELAY              = 8000
};

enum GroveHippogryphPhase
{
    HIPPOGRYPH_PHASE_NONE,
    HIPPOGRYPH_PHASE_TAKEOFF,
    HIPPOGRYPH_PHASE_PATROL,
    HIPPOGRYPH_PHASE_RETURN,
    HIPPOGRYPH_PHASE_DONE
};

struct WildlifeFlightPoint
{
    float x, y, z;
    int8 text;
};

// Over the deer and Moonstalkers on the east shore, west past the thistle bears and the Master's Glaive,
// through the western hills and back to Orseus. Heights keep the flight above the treetops.
WildlifeFlightPoint const WildlifeFlightPath[] =
{
    { 4961.6f, 130.6f,  88.1f, SAY_KATHRENA_FROM_THE_AIR },
    { 5100.0f, 120.0f,  78.8f, SAY_KATHRENA_DEER },
    { 5205.0f, 126.0f,  80.6f, SAY_NONE },
    { 5297.0f,  60.0f,  82.3f, SAY_NONE },
    { 5400.0f, 110.0f,  80.6f, SAY_NONE },
    { 5476.0f, 174.0f,  47.6f, SAY_KATHRENA_MOONSTALKERS },
    { 5494.0f, 214.0f,  52.1f, SAY_NONE },
    { 5482.0f, 295.0f,  51.0f, SAY_NONE },
    { 5403.0f, 325.0f,  42.7f, SAY_NONE },
    { 5387.0f, 375.0f,  41.3f, SAY_NONE },
    { 5386.0f, 467.0f,  40.7f, SAY_NONE },
    { 5238.0f, 430.0f,  62.2f, SAY_KATHRENA_UPDRAFT },
    { 5078.0f, 424.0f,  63.7f, SAY_KATHRENA_THISTLE_BEARS },
    { 5004.0f, 389.0f,  62.8f, SAY_NONE },
    { 4900.0f, 404.0f,  59.9f, SAY_NONE },
    { 4801.0f, 388.0f,  64.6f, SAY_NONE },
    { 4695.0f, 379.0f,  71.1f, SAY_KATHRENA_DIG_SITE },
    { 4580.0f, 400.0f,  91.6f, SAY_NONE },
    { 4397.0f, 398.0f,  93.0f, SAY_NONE },
    { 4305.0f, 498.0f,  93.8f, SAY_NONE },
    { 4198.0f, 539.0f,  94.0f, SAY_NONE },
    { 4179.0f, 411.0f,  93.6f, SAY_NONE },
    { 4239.0f, 311.0f,  96.9f, SAY_NONE },
    { 4407.0f, 290.0f,  96.5f, SAY_NONE },
    { 4409.0f, 166.0f,  90.4f, SAY_NONE },
    { 4354.0f,  59.0f,  92.5f, SAY_NONE },
    { 4310.0f,   8.0f, 118.1f, SAY_NONE },
    { 4600.0f,  60.0f, 118.1f, SAY_NONE },
    { 4850.0f, 110.0f,  99.0f, SAY_NONE },
    { 4968.5f, 135.5f,  75.0f, SAY_NONE },
    { 4968.5f, 135.5f,  52.7f, SAY_NONE }
};

// Beside Orseus (34392), where the flight lands.
Position const GroveHippogryphLandingPos = { 4968.5f, 135.5f, 52.7f, 0.79f };
// Clears every hill between the patrol and the grove when the flight is cut short.
float const GroveHippogryphReturnZ = 125.0f;
float const GroveHippogryphFlightSpeed = 14.0f;
// Kathrena flies in formation: the same path, shifted by this offset.
float const KathrenaOffsetX = -6.0f;
float const KathrenaOffsetY = 6.0f;
float const KathrenaOffsetZ = 3.0f;

static uint32 GetWildlifeProtectedCredit(uint32 entry)
{
    switch (entry)
    {
        case NPC_GRIZZLED_THISTLE_BEAR:
        case NPC_YOUNG_GRIZZLED_THISTLE_BEAR:
            return NPC_THISTLE_BEAR_PROTECTED_CREDIT;
        case NPC_WHITETAIL_STAG:
        case NPC_WHITETAIL_DOE:
            return NPC_WHITETAIL_PROTECTED_CREDIT;
        case NPC_MOONSTALKER_MATRIARCH:
        case NPC_MOONSTALKER_SIRE:
            return NPC_MOONSTALKER_PROTECTED_CREDIT;
        default:
            return 0;
    }
}

// Grove Hippogryph (34375): summoned by Orseus' gossip for In Defense of Darkshore. Flies the rider over the
// animals with Kathrena alongside while the rider casts Protect Wildlife, then lands back beside Orseus.
class npc_grove_hippogryph : public CreatureScript
{
public:
    npc_grove_hippogryph() : CreatureScript("npc_grove_hippogryph") { }

    struct npc_grove_hippogryphAI : public ScriptedAI
    {
        npc_grove_hippogryphAI(Creature* creature) : ScriptedAI(creature), _riderGuid(0), _kathrenaGuid(0), _boardTimer(0), _phaseTimer(0),
            _nextPoint(0), _phase(HIPPOGRYPH_PHASE_NONE) { }

        void Reset() OVERRIDE
        {
            me->SetReactState(REACT_PASSIVE);
        }

        // The rider takes the control seat, which charms the hippogryph; keep this AI so it can fly.
        void OnCharmed(bool /*apply*/) OVERRIDE { }

        void IsSummonedBy(Unit* summoner) OVERRIDE
        {
            if (summoner->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            _riderGuid = summoner->GetGUID();
            _boardTimer = HIPPOGRYPH_BOARD_DELAY;
        }

        void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) OVERRIDE
        {
            if (passenger->GetTypeId() != TypeID::TYPEID_PLAYER)
                return;

            if (apply)
            {
                _riderGuid = passenger->GetGUID();
                me->SetCanFly(true);
                me->SetDisableGravity(true);
                SummonKathrena();
                KathrenaTalk(SAY_KATHRENA_HOLD_ON);
                _phase = HIPPOGRYPH_PHASE_TAKEOFF;
                _phaseTimer = HIPPOGRYPH_TAKEOFF_DELAY;
                return;
            }

            // Left before landing: put the rider back beside Orseus instead of dropping them from the sky.
            if (_phase != HIPPOGRYPH_PHASE_DONE)
            {
                if (Player* player = passenger->ToPlayer())
                    player->NearTeleportTo(GroveHippogryphLandingPos.GetPositionX(), GroveHippogryphLandingPos.GetPositionY(), GroveHippogryphLandingPos.GetPositionZ(), GroveHippogryphLandingPos.GetOrientation());
                if (Creature* kathrena = ObjectAccessor::GetCreature(*me, _kathrenaGuid))
                    kathrena->DespawnOrUnsummon();
            }

            _phase = HIPPOGRYPH_PHASE_DONE;
            me->DespawnOrUnsummon(HIPPOGRYPH_DESPAWN_DELAY);
        }

        // Protect Wildlife hit an animal: each one counts once per flight.
        void SetGUID(uint64 guid, int32 /*id*/) OVERRIDE
        {
            Player* rider = ObjectAccessor::GetPlayer(*me, _riderGuid);
            Creature* animal = ObjectAccessor::GetCreature(*me, guid);
            if (!rider || !animal || !animal->IsAlive())
                return;

            uint32 credit = GetWildlifeProtectedCredit(animal->GetEntry());
            if (!credit || !_protectedGuids.insert(guid).second)
                return;

            animal->CastSpell(animal, SPELL_PROTECTED_FROM_CONSUMPTION, true);
            rider->KilledMonsterCredit(credit);
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (_boardTimer)
            {
                if (_boardTimer > diff)
                {
                    _boardTimer -= diff;
                    return;
                }

                _boardTimer = 0;
                Player* player = ObjectAccessor::GetPlayer(*me, _riderGuid);
                if (player && !player->GetVehicle())
                    player->EnterVehicle(me, HIPPOGRYPH_SEAT);
                else if (!player)
                    me->DespawnOrUnsummon();
                return;
            }

            switch (_phase)
            {
                case HIPPOGRYPH_PHASE_TAKEOFF:
                    if (_phaseTimer > diff)
                    {
                        _phaseTimer -= diff;
                        break;
                    }
                    _phaseTimer = 0;
                    StartPatrol();
                    break;
                case HIPPOGRYPH_PHASE_PATROL:
                    AnnounceReachedPoints();
                    if (me->movespline->Finalized())
                        Land();
                    else if (Player* player = ObjectAccessor::GetPlayer(*me, _riderGuid))
                        if (player->GetQuestStatus(QUEST_IN_DEFENSE_OF_DARKSHORE) == QUEST_STATUS_COMPLETE)
                            ReturnEarly();
                    break;
                case HIPPOGRYPH_PHASE_RETURN:
                    if (me->movespline->Finalized())
                        Land();
                    break;
                default:
                    break;
            }
        }

    private:
        void SummonKathrena()
        {
            Creature* kathrena = me->SummonCreature(NPC_KATHRENA_WINTERWISP_FLIGHT, me->GetPositionX() + KathrenaOffsetX, me->GetPositionY() + KathrenaOffsetY,
                me->GetPositionZ() + KathrenaOffsetZ, me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            if (!kathrena)
                return;

            _kathrenaGuid = kathrena->GetGUID();
            kathrena->SetReactState(REACT_PASSIVE);
            kathrena->Mount(DISPLAY_GROVE_HIPPOGRYPH_MOUNT);
            kathrena->SetCanFly(true);
            kathrena->SetDisableGravity(true);
        }

        void KathrenaTalk(int8 text)
        {
            Creature* kathrena = ObjectAccessor::GetCreature(*me, _kathrenaGuid);
            if (kathrena && kathrena->IsAIEnabled)
                kathrena->AI()->Talk(uint8(text), ObjectAccessor::GetPlayer(*me, _riderGuid));
        }

        static void FlyPath(Unit* unit, Movement::PointsArray const& path)
        {
            Movement::MoveSplineInit init(unit);
            init.MovebyPath(path);
            init.SetFly();
            init.SetSmooth();
            init.SetUncompressed();
            init.SetVelocity(GroveHippogryphFlightSpeed);
            init.Launch();
        }

        // The hippogryph flies the path and Kathrena the same path shifted by her offset, so both arrive together.
        void FlyWithKathrena(Movement::PointsArray const& path)
        {
            FlyPath(me, path);

            Creature* kathrena = ObjectAccessor::GetCreature(*me, _kathrenaGuid);
            if (!kathrena)
                return;

            G3D::Vector3 const offset(KathrenaOffsetX, KathrenaOffsetY, KathrenaOffsetZ);
            Movement::PointsArray kathrenaPath;
            kathrenaPath.push_back(G3D::Vector3(kathrena->GetPositionX(), kathrena->GetPositionY(), kathrena->GetPositionZ()));
            for (size_t i = 1; i < path.size(); ++i)
                kathrenaPath.push_back(path[i] + offset);
            FlyPath(kathrena, kathrenaPath);
        }

        void StartPatrol()
        {
            _phase = HIPPOGRYPH_PHASE_PATROL;
            _nextPoint = 0;
            // The rider only aims and casts; their client must not fight the flight path.
            if (Player* player = ObjectAccessor::GetPlayer(*me, _riderGuid))
                player->SetClientControl(me, 0);

            Movement::PointsArray path;
            path.push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));
            for (WildlifeFlightPoint const& point : WildlifeFlightPath)
                path.push_back(G3D::Vector3(point.x, point.y, point.z));
            FlyWithKathrena(path);
        }

        // Path point 0 is where the hippogryph started, so flight point i is path point i + 1: being past path
        // point n means flight points 0 to n - 1 have been reached.
        void AnnounceReachedPoints()
        {
            uint32 const reached = me->movespline->Finalized() ? uint32(std::size(WildlifeFlightPath)) : uint32(std::max(me->movespline->currentPathIdx(), 0));
            for (; _nextPoint < reached && _nextPoint < std::size(WildlifeFlightPath); ++_nextPoint)
                if (WildlifeFlightPath[_nextPoint].text != SAY_NONE)
                    KathrenaTalk(WildlifeFlightPath[_nextPoint].text);
        }

        void ReturnEarly()
        {
            _phase = HIPPOGRYPH_PHASE_RETURN;
            Movement::PointsArray path;
            path.push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()));
            path.push_back(G3D::Vector3(me->GetPositionX(), me->GetPositionY(), std::max(me->GetPositionZ(), GroveHippogryphReturnZ)));
            path.push_back(G3D::Vector3(GroveHippogryphLandingPos.GetPositionX(), GroveHippogryphLandingPos.GetPositionY(), GroveHippogryphReturnZ));
            path.push_back(G3D::Vector3(GroveHippogryphLandingPos.GetPositionX(), GroveHippogryphLandingPos.GetPositionY(), GroveHippogryphLandingPos.GetPositionZ()));
            FlyWithKathrena(path);
        }

        void Land()
        {
            _phase = HIPPOGRYPH_PHASE_DONE;
            KathrenaTalk(SAY_KATHRENA_GOOD_WORK);

            if (Creature* kathrena = ObjectAccessor::GetCreature(*me, _kathrenaGuid))
            {
                Movement::PointsArray path;
                path.push_back(G3D::Vector3(kathrena->GetPositionX(), kathrena->GetPositionY(), kathrena->GetPositionZ()));
                path.push_back(G3D::Vector3(kathrena->GetPositionX() - 40.0f, kathrena->GetPositionY() + 40.0f, kathrena->GetPositionZ() + 40.0f));
                FlyPath(kathrena, path);
                kathrena->DespawnOrUnsummon(KATHRENA_DESPAWN_DELAY);
            }

            if (Vehicle* vehicle = me->GetVehicleKit())
                vehicle->RemoveAllPassengers();
            me->DespawnOrUnsummon(HIPPOGRYPH_DESPAWN_DELAY);
        }

        uint64 _riderGuid;
        uint64 _kathrenaGuid;
        uint32 _boardTimer;
        uint32 _phaseTimer;
        uint32 _nextPoint;
        GroveHippogryphPhase _phase;
        std::set<uint64> _protectedGuids;
    };

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new npc_grove_hippogryphAI(creature);
    }
};

// 65373 - Protect Wildlife
class spell_darkshore_protect_wildlife : public SpellScriptLoader
{
public:
    spell_darkshore_protect_wildlife() : SpellScriptLoader("spell_darkshore_protect_wildlife") { }

    class spell_darkshore_protect_wildlife_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_darkshore_protect_wildlife_SpellScript);

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            targets.remove_if([](WorldObject* target)
            {
                Creature* animal = target->ToCreature();
                return !animal || !animal->IsAlive() || !GetWildlifeProtectedCredit(animal->GetEntry());
            });
        }

        // Cast from the hippogryph's action bar, so the caster is the hippogryph that tracks the credit.
        void HandleDummy(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            Unit* caster = GetCaster();
            Creature* hippogryph = caster->ToCreature();
            if (!hippogryph && caster->GetVehicleBase())
                hippogryph = caster->GetVehicleBase()->ToCreature();
            if (hippogryph && hippogryph->IsAIEnabled && GetHitUnit())
                hippogryph->AI()->SetGUID(GetHitUnit()->GetGUID());
        }

        void Register() OVERRIDE
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_darkshore_protect_wildlife_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
            OnEffectHitTarget += SpellEffectFn(spell_darkshore_protect_wildlife_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const OVERRIDE
    {
        return new spell_darkshore_protect_wildlife_SpellScript();
    }
};

void AddSC_darkshore()
{
    new npc_foolhardy_adventurer();
    new npc_nightsaber_rider();
    new npc_darkshore_nightsaber();
    new npc_darkshore_soothing_totem();
    new npc_thessera_dream();
    new npc_thessera_dream_ride();
    new npc_thessera_eye_ride();
    new spell_darkshore_land_thessera();
    new spell_darkshore_emerald_barrage();
    new npc_darkshore_twilight_portal();
    new npc_foreman_balsoth();
    new npc_doomspeaker_trevellion();
    new npc_grove_hippogryph();
    new spell_darkshore_protect_wildlife();
}
