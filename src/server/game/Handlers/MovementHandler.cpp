/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Battleground.h"
#include "BattlePetMgr.h"
#include "Common.h"
#include "Corpse.h"
#include "InstanceSaveMgr.h"
#include "Log.h"
#include "MapManager.h"
#include "MovementStructures.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "PetTransportController.h"
#include "PetTransportSupport.h"
#include "Player.h"
#include "SpellAuras.h"
#include "Transport.h"
#include "WaypointMovementGenerator.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#define MOVEMENT_PACKET_TIME_DELAY 0

namespace
{
struct MoveTeleportAckRequest
{
    ObjectGuid guid;
    uint32 flags;
    uint32 time;
};

struct ActiveMoverRequest
{
    ObjectGuid guid;
};

struct ForceSpeedChangeAckRequest
{
    MovementInfo movementInfo;
    float newSpeed;
    uint32 opcode;
};

struct SummonResponseRequest
{
    ObjectGuid summonerGuid;
    bool accept;
};

MoveTeleportAckRequest ReadMoveTeleportAckRequest(WorldPacket& recvPacket)
{
    MoveTeleportAckRequest request;
    recvPacket >> request.time >> request.flags;

    request.guid[0] = recvPacket.ReadBit();
    request.guid[7] = recvPacket.ReadBit();
    request.guid[3] = recvPacket.ReadBit();
    request.guid[5] = recvPacket.ReadBit();
    request.guid[4] = recvPacket.ReadBit();
    request.guid[6] = recvPacket.ReadBit();
    request.guid[1] = recvPacket.ReadBit();
    request.guid[2] = recvPacket.ReadBit();

    recvPacket.ReadByteSeq(request.guid[4]);
    recvPacket.ReadByteSeq(request.guid[1]);
    recvPacket.ReadByteSeq(request.guid[6]);
    recvPacket.ReadByteSeq(request.guid[7]);
    recvPacket.ReadByteSeq(request.guid[0]);
    recvPacket.ReadByteSeq(request.guid[2]);
    recvPacket.ReadByteSeq(request.guid[5]);
    recvPacket.ReadByteSeq(request.guid[3]);

    return request;
}

MovementInfo ReadMovementInfoRequest(Player* player, WorldPacket& recvPacket)
{
    MovementInfo movementInfo;
    player->ReadMovementInfo(recvPacket, &movementInfo);
    return movementInfo;
}

ForceSpeedChangeAckRequest ReadForceSpeedChangeAckRequest(Player* player, WorldPacket& recvData)
{
    ForceSpeedChangeAckRequest request;
    request.opcode = recvData.GetOpcode();

    static MovementStatusElements const speedElement = MSEExtraFloat;
    Movement::ExtraMovementStatusElement extras(&speedElement);
    player->ReadMovementInfo(recvData, &request.movementInfo, &extras);
    request.newSpeed = extras.Data.floatData;

    return request;
}

ActiveMoverRequest ReadActiveMoverRequest(WorldPacket& recvPacket)
{
    ActiveMoverRequest request;

    recvPacket.ReadBit();

    request.guid[3] = recvPacket.ReadBit();
    request.guid[0] = recvPacket.ReadBit();
    request.guid[2] = recvPacket.ReadBit();
    request.guid[1] = recvPacket.ReadBit();
    request.guid[5] = recvPacket.ReadBit();
    request.guid[4] = recvPacket.ReadBit();
    request.guid[7] = recvPacket.ReadBit();
    request.guid[6] = recvPacket.ReadBit();

    recvPacket.ReadByteSeq(request.guid[3]);
    recvPacket.ReadByteSeq(request.guid[4]);
    recvPacket.ReadByteSeq(request.guid[5]);
    recvPacket.ReadByteSeq(request.guid[2]);
    recvPacket.ReadByteSeq(request.guid[7]);
    recvPacket.ReadByteSeq(request.guid[0]);
    recvPacket.ReadByteSeq(request.guid[1]);
    recvPacket.ReadByteSeq(request.guid[6]);

    return request;
}

MovementInfo ReadMoveHoverAckRequest(Player* player, WorldPacket& recvData)
{
    uint64 guid;                                            // guid - unused
    recvData.readPackGUID(guid);

    recvData.read_skip<uint32>();                          // unk

    MovementInfo movementInfo;
    player->ReadMovementInfo(recvData, &movementInfo);

    recvData.read_skip<uint32>();                          // unk2

    return movementInfo;
}

SummonResponseRequest ReadSummonResponseRequest(WorldPacket& recvData)
{
    SummonResponseRequest request;

    request.summonerGuid[1] = recvData.ReadBit();
    request.summonerGuid[3] = recvData.ReadBit();
    request.summonerGuid[5] = recvData.ReadBit();
    request.summonerGuid[2] = recvData.ReadBit();
    request.accept = recvData.ReadBit();
    request.summonerGuid[7] = recvData.ReadBit();
    request.summonerGuid[0] = recvData.ReadBit();
    request.summonerGuid[4] = recvData.ReadBit();
    request.summonerGuid[6] = recvData.ReadBit();

    recvData.ReadByteSeq(request.summonerGuid[0]);
    recvData.ReadByteSeq(request.summonerGuid[1]);
    recvData.ReadByteSeq(request.summonerGuid[6]);
    recvData.ReadByteSeq(request.summonerGuid[3]);
    recvData.ReadByteSeq(request.summonerGuid[5]);
    recvData.ReadByteSeq(request.summonerGuid[4]);
    recvData.ReadByteSeq(request.summonerGuid[2]);
    recvData.ReadByteSeq(request.summonerGuid[7]);

    return request;
}

MovementInfo ReadMoveSetCanFlyAckRequest(Player* player, WorldPacket& recvData)
{
    return ReadMovementInfoRequest(player, recvData);
}

void ReadSetCollisionHeightAckRequest(Player* player, WorldPacket& recvPacket)
{
    static MovementStatusElements const heightElements[] = { MSEExtraFloat, MSEExtra2Bits };
    Movement::ExtraMovementStatusElement extra(heightElements);
    MovementInfo movementInfo;
    player->ReadMovementInfo(recvPacket, &movementInfo, &extra);
}

void ReadMovementForceAckRequest(Player* player, WorldPacket& recvPacket)
{
    if (recvPacket.GetOpcode() == CMSG_MOVE_APPLY_MOVEMENT_FORCE_ACK)
    {
        static MovementStatusElements const unkElements[] = { MSEExtraFloat, MSEExtraInt32, MSEExtraFloat };
        Movement::ExtraMovementStatusElement extra(unkElements);
        MovementInfo movementInfo;
        player->ReadMovementInfo(recvPacket, &movementInfo, &extra);
    }
    else
        ReadMovementInfoRequest(player, recvPacket);
}

bool ValidateMoveTeleportAck(Player* mover, ObjectGuid guid)
{
    return mover && mover->IsBeingTeleportedNear() && guid == mover->GetGUID();
}

bool ShouldIgnoreMovementWhileTeleporting(Player* mover, WorldPacket& recvPacket)
{
    if (!mover || !mover->IsBeingTeleported())
        return false;

    recvPacket.rfinish();                              // prevent warnings spam
    return true;
}

bool ValidateMovementInfo(Unit* mover, MovementInfo const& movementInfo)
{
    if (movementInfo.guid != mover->GetGUID())
    {
        SF_LOG_ERROR("network", "HandleMovementOpcodes: guid error");
        return false;
    }

    if (!movementInfo.pos.IsPositionValid())
    {
        SF_LOG_ERROR("network", "HandleMovementOpcodes: Invalid Position");
        return false;
    }

    return true;
}

bool ValidateTransportMovementInfo(WorldPacket& recvPacket, MovementInfo const& movementInfo)
{
    if (!movementInfo.transport.guid)
        return true;

    // transports size limited
    // (also received at zeppelin leave by some reason with t_* as absolute in continent coordinates, can be safely skipped)
    if (!Skyfire::PetTransport::IsPassengerOffsetWithinLimit(movementInfo.transport.pos.GetPositionX(),
        movementInfo.transport.pos.GetPositionY(), movementInfo.transport.pos.GetPositionZ(), 50.0f))
    {
        recvPacket.rfinish();                          // prevent warnings spam
        return false;
    }

    if (!Skyfire::IsValidMapCoord(movementInfo.pos.GetPositionX() + movementInfo.transport.pos.GetPositionX(), movementInfo.pos.GetPositionY() + movementInfo.transport.pos.GetPositionY(),
        movementInfo.pos.GetPositionZ() + movementInfo.transport.pos.GetPositionZ(), movementInfo.pos.GetOrientation() + movementInfo.transport.pos.GetOrientation()))
    {
        recvPacket.rfinish();                          // prevent warnings spam
        return false;
    }

    return true;
}

bool ValidateSpeedChangeAck(WorldPacket& recvData, Player* player, MovementInfo const& movementInfo)
{
    if (player->GetGUID() == movementInfo.guid)
        return true;

    recvData.rfinish();                                // prevent warnings spam
    return false;
}

bool ValidateActiveMoverGuid(Player* player, ObjectGuid guid)
{
    if (!player->IsInWorld() || player->m_mover->GetGUID() == guid)
        return true;

    SF_LOG_ERROR("network", "HandleSetActiveMoverOpcode: incorrect mover guid: mover is " UI64FMTD " (%s - Entry: %u) and should be " UI64FMTD, uint64(guid), GetLogNameForGuid(guid), GUID_ENPART(guid), player->m_mover->GetGUID());
    return false;
}

bool ValidateKnockBackAck(Unit* mover, MovementInfo const& movementInfo)
{
    return mover->GetGUID() == movementInfo.guid;
}
}

void WorldSession::HandleMoveWorldportAckOpcode(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: got MSG_MOVE_WORLDPORT_ACK.");
    HandleMoveWorldportAckOpcode();
}

void WorldSession::HandleMoveWorldportAckOpcode()
{
    // ignore unexpected far teleports
    if (!GetPlayer()->IsBeingTeleportedFar())
        return;

    GetPlayer()->SetSemaphoreTeleportFar(false);
    GetPlayer()->SetSemaphoreTeleportForcedFar(false);

    // get the teleport destination
    WorldLocation const& loc = GetPlayer()->GetTeleportDest();

    // possible errors in the coordinate validity check
    if (!MapManager::IsValidMapCoord(loc))
    {
        LogoutPlayer(false);
        return;
    }

    // get the destination map entry, not the current one, this will fix homebind and reset greeting
    MapEntry const* mEntry = sMapStore.LookupEntry(loc.GetMapId());
    InstanceTemplate const* mInstance = sObjectMgr->GetInstanceTemplate(loc.GetMapId());

    // reset instance validity, except if going to an instance inside an instance
    if (GetPlayer()->m_InstanceValid == false && !mInstance)
        GetPlayer()->m_InstanceValid = true;

    Map* oldMap = GetPlayer()->GetMap();
    Map* newMap = sMapMgr->CreateMap(loc.GetMapId(), GetPlayer());

    if (GetPlayer()->IsInWorld())
    {
        SF_LOG_ERROR("network", "Player %s (GUID: %u) is still in world when teleported from map %s (%u) to new map %s (%u)", GetPlayer()->GetName().c_str(), GUID_LOPART(GetPlayer()->GetGUID()), oldMap->GetMapName(), oldMap->GetId(), newMap ? newMap->GetMapName() : "Unknown", loc.GetMapId());
        oldMap->RemovePlayerFromMap(GetPlayer(), false);
    }

    // relocate the player to the teleport destination
    // the CanEnter checks are done in TeleporTo but conditions may change
    // while the player is in transit, for example the map may get full
    if (!newMap || !newMap->CanEnter(GetPlayer()))
    {
        SF_LOG_ERROR("network", "Map %d (%s) could not be created for player %d (%s), porting player to homebind", loc.GetMapId(), newMap ? newMap->GetMapName() : "Unknown", GetPlayer()->GetGUIDLow(), GetPlayer()->GetName().c_str());
        GetPlayer()->TeleportTo(GetPlayer()->m_homebindMapId, GetPlayer()->m_homebindX, GetPlayer()->m_homebindY, GetPlayer()->m_homebindZ, GetPlayer()->GetOrientation());
        return;
    }
    else
        GetPlayer()->Relocate(&loc);

    GetPlayer()->ResetMap();
    GetPlayer()->SetMap(newMap);

    GetPlayer()->SendInitialPacketsBeforeAddToMap();
    if (!GetPlayer()->GetMap()->AddPlayerToMap(GetPlayer()))
    {
        SF_LOG_ERROR("network", "WORLD: failed to teleport player %s (%d) to map %d (%s) because of unknown reason!",
            GetPlayer()->GetName().c_str(), GetPlayer()->GetGUIDLow(), loc.GetMapId(), newMap ? newMap->GetMapName() : "Unknown");
        GetPlayer()->ResetMap();
        GetPlayer()->SetMap(oldMap);
        GetPlayer()->TeleportTo(GetPlayer()->m_homebindMapId, GetPlayer()->m_homebindX, GetPlayer()->m_homebindY, GetPlayer()->m_homebindZ, GetPlayer()->GetOrientation());
        return;
    }

    // battleground state prepare (in case join to BG), at relogin/tele player not invited
    // only add to bg group and object, if the player was invited (else he entered through command)
    if (_player->InBattleground())
    {
        // cleanup setting if outdated
        if (!mEntry->IsBattlegroundOrArena())
        {
            // We're not in BG
            _player->SetBattlegroundId(0, BattlegroundTypeId::BATTLEGROUND_TYPE_NONE);
            // reset destination bg team
            _player->SetBGTeam(0);
        }
        // join to bg case
        else if (Battleground* bg = _player->GetBattleground())
        {
            if (_player->IsInvitedForBattlegroundInstance(_player->GetBattlegroundId()))
                bg->AddPlayer(_player);
        }
    }

    GetPlayer()->SendInitialPacketsAfterAddToMap();

    // flight fast teleport case
    if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
    {
        if (!_player->InBattleground())
        {
            // short preparations to continue flight
            FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());
            flight->Initialize(GetPlayer());
            return;
        }

        // battleground state prepare, stop flight
        GetPlayer()->GetMotionMaster()->MovementExpired();
        GetPlayer()->CleanupAfterTaxiFlight();
    }

    // resurrect character at enter into instance where his corpse exist after add to map
    Corpse* corpse = GetPlayer()->GetCorpse();
    if (corpse && corpse->GetType() != CorpseType::CORPSE_BONES && corpse->GetMapId() == GetPlayer()->GetMapId())
    {
        if (mEntry->IsInstance())
        {
            GetPlayer()->ResurrectPlayer(0.5f, false);
            GetPlayer()->SpawnCorpseBones();
        }
    }

    bool allowMount = !mEntry->IsInstance() || mEntry->IsBattlegroundOrArena();
    if (mInstance)
    {
        DifficultyID diff = GetPlayer()->GetDifficulty(mEntry);
        if (MapDifficulty const* mapDiff = GetMapDifficultyData(mEntry->MapID, diff))
        {
            if (mapDiff->resetTime)
            {
                if (time_t timeReset = sInstanceSaveMgr->GetResetTimeFor(mEntry->MapID, diff))
                {
                    uint32 timeleft = uint32(timeReset - time(NULL));
                    GetPlayer()->SendInstanceResetWarning(mEntry->MapID, diff, timeleft);
                }
            }
        }
        allowMount = mInstance->AllowMount;
    }

    // mount allow check
    if (!allowMount)
        _player->RemoveAurasByType(SPELL_AURA_MOUNTED);

    // update zone immediately, otherwise leave channel will cause crash in mtmap
    uint32 newzone, newarea;
    GetPlayer()->GetZoneAndAreaId(newzone, newarea);
    GetPlayer()->UpdateZone(newzone, newarea);

    // honorless target
    if (GetPlayer()->pvpInfo.IsHostile)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);

    // in friendly area
    else if (GetPlayer()->IsPvP() && !GetPlayer()->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
        GetPlayer()->UpdatePvP(false, false);

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();

    // resummon battle pet
    GetPlayer()->GetBattlePetMgr()->ResummonLastBattlePet();

    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMoveTeleportAck(WorldPacket& recvPacket)
{
    SF_LOG_DEBUG("network", "CMSG_MOVE_TELEPORT_ACK");

    MoveTeleportAckRequest request = ReadMoveTeleportAckRequest(recvPacket);
    ObjectGuid guid = request.guid;
    uint32 flags = request.flags;
    uint32 time = request.time;

    SF_LOG_DEBUG("network", "Guid " UI64FMTD, uint64(guid));
    SF_LOG_DEBUG("network", "Flags %u, time %u", flags, time / IN_MILLISECONDS);

    Player* plMover = _player->m_mover->ToPlayer();

    if (!ValidateMoveTeleportAck(plMover, guid))
        return;

    plMover->SetSemaphoreTeleportNear(false);

    uint32 old_zone = plMover->GetZoneId();

    WorldLocation const& dest = plMover->GetTeleportDest();

    plMover->UpdatePosition(dest, true);
    // Near teleports can finish with the player already at dest coords; force a full
    // visibility refresh so creates for the destination cell are sent after ACK.
    plMover->UpdateObjectVisibility(true);

    uint32 newzone, newarea;
    plMover->GetZoneAndAreaId(newzone, newarea);
    plMover->UpdateZone(newzone, newarea);

    // new zone
    if (old_zone != newzone)
    {
        // honorless target
        if (plMover->pvpInfo.IsHostile)
            plMover->CastSpell(plMover, 2479, true);

        // in friendly area
        else if (plMover->IsPvP() && !plMover->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
            plMover->UpdatePvP(false, false);
    }

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();

    // resummon battle pet
    GetPlayer()->GetBattlePetMgr()->ResummonLastBattlePet();

    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMovementOpcodes(WorldPacket& recvPacket)
{
    uint16 opcode = recvPacket.GetOpcode();

    Unit* mover = _player->m_mover;

    ASSERT(mover != NULL);                      // there must always be a mover

    Player* plrMover = mover->ToPlayer();

    if (plrMover && plrMover->IsInCinematic())
        return;

    // ignore, waiting processing in WorldSession::HandleMoveWorldportAckOpcode and WorldSession::HandleMoveTeleportAck
    if (ShouldIgnoreMovementWhileTeleporting(plrMover, recvPacket))
        return;

    /* extract packet */
    MovementInfo movementInfo = ReadMovementInfoRequest(GetPlayer(), recvPacket);

    // prevent tampered movement data
    if (!ValidateMovementInfo(mover, movementInfo))
        return;

    Transport* pendingPetTransportRemoval = NULL;

    /* handle special cases */
    if (movementInfo.transport.guid)
    {
        if (!ValidateTransportMovementInfo(recvPacket, movementInfo))
            return;

        // if we boarded a transport, add us to it
        if (plrMover)
        {
            if (!plrMover->GetTransport())
            {
                if (Transport* transport = plrMover->GetMap()->GetTransport(movementInfo.transport.guid))
                {
                    plrMover->m_transport = transport;
                    transport->AddPassenger(plrMover);
                }
            }
            else if (plrMover->GetTransport()->GetGUID() != movementInfo.transport.guid)
            {
                bool foundNewTransport = false;
                Transport* oldTransport = plrMover->m_transport;
                oldTransport->RemovePassenger(plrMover);
                if (Transport* transport = plrMover->GetMap()->GetTransport(movementInfo.transport.guid))
                {
                    foundNewTransport = true;
                    plrMover->m_transport = transport;
                    transport->AddPassenger(plrMover);
                }

                if (!foundNewTransport)
                {
                    plrMover->m_transport = NULL;
                    movementInfo.ResetTransport();
                    pendingPetTransportRemoval = oldTransport;
                }
            }
        }

        if (!mover->GetTransport() && !mover->GetVehicle())
        {
            GameObject* go = mover->GetMap()->GetGameObject(movementInfo.transport.guid);
            if (!go || go->GetGoType() != GAMEOBJECT_TYPE_TRANSPORT)
                movementInfo.transport.guid = 0;
        }
    }
    else if (plrMover && plrMover->GetTransport())                // if we were on a transport, leave
    {
        pendingPetTransportRemoval = plrMover->m_transport;
        plrMover->m_transport->RemovePassenger(plrMover);
        plrMover->m_transport = NULL;
        movementInfo.ResetTransport();
    }

    // fall damage generation (ignore in flight case that can be triggered also at lags in moment teleportation to another map).
    if (opcode == MSG_MOVE_FALL_LAND && plrMover && !plrMover->IsInFlight())
        plrMover->HandleFall(movementInfo);

    if (plrMover && ((movementInfo.flags & MOVEMENTFLAG_SWIMMING) != 0) != plrMover->IsInWater())
    {
        // now client not include swimming flag in case jumping under water
        plrMover->SetInWater(!plrMover->IsInWater() || plrMover->GetBaseMap()->IsUnderWater(movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY(), movementInfo.pos.GetPositionZ()));
    }

    uint32 mstime = getMSTime();
    /*----------------------*/
    if (m_clientTimeDelay == 0)
        m_clientTimeDelay = mstime - movementInfo.time;

    /* process position-change */
    movementInfo.time = movementInfo.time + m_clientTimeDelay + MOVEMENT_PACKET_TIME_DELAY;

    movementInfo.guid = mover->GetGUID();
    mover->m_movementInfo = movementInfo;

    if (plrMover && plrMover->GetTransport())
        Skyfire::PetTransport::BoardOwnerHunterPet(plrMover, plrMover->GetTransport());

    /*----------------------*/
    /* process position-change */
    // this is almost never true (not sure why it is sometimes, but it is), normally use mover->IsVehicle()
    if (mover->GetVehicle())
    {
        mover->SetOrientation(movementInfo.pos.GetOrientation());
        return;
    }

    if (Transport* transport = mover->GetTransport())
    {
        movementInfo.pos.Relocate(movementInfo.transport.pos);
        transport->CalculatePassengerPosition(
            movementInfo.pos.m_positionX,
            movementInfo.pos.m_positionY,
            movementInfo.pos.m_positionZ,
            &movementInfo.pos.m_orientation);
    }

    mover->UpdatePosition(movementInfo.pos);

    if (plrMover && pendingPetTransportRemoval)
        Skyfire::PetTransport::RemoveOwnerHunterPet(plrMover, pendingPetTransportRemoval);

    if (plrMover && plrMover->GetTransport())
        Skyfire::PetTransport::BoardOwnerHunterPet(plrMover, plrMover->GetTransport());

    if (mover->GetUInt32Value(UNIT_FIELD_NPC_EMOTESTATE) == 10)
    {
        mover->SetUInt32Value(UNIT_FIELD_NPC_EMOTESTATE, 0x00);
    }

    WorldPacket data(SMSG_PLAYER_MOVE, recvPacket.size());
    mover->WriteMovementInfo(data);
    mover->SendMessageToSet(&data, _player);

    if (plrMover)                                            // nothing is charmed, or player charmed
    {
        plrMover->UpdateFallInformationIfNeed(movementInfo, opcode);

        AreaTableEntry const* zone = GetAreaEntryByAreaID(plrMover->GetAreaId());
        float depth = zone ? zone->m_MinElevation : -500.0f;
        if (movementInfo.pos.GetPositionZ() < depth)
        {
            if (!(plrMover->GetBattleground() && plrMover->GetBattleground()->HandlePlayerUnderMap(_player)))
            {
                // NOTE: this is actually called many times while falling
                // even after the player has been teleported away
                /// @todo discard movement packets after the player is rooted
                if (plrMover->IsAlive())
                {
                    plrMover->EnvironmentalDamage(DAMAGE_FALL_TO_VOID, GetPlayer()->GetMaxHealth());
                    // player can be alive if GM/etc
                    // change the death state to CORPSE to prevent the death timer from
                    // starting in the next player update
                    if (!plrMover->IsAlive())
                        plrMover->KillPlayer();
                }
            }
        }
    }
}

void WorldSession::HandleForceSpeedChangeAck(WorldPacket& recvData)
{
    /* extract packet */
    ForceSpeedChangeAckRequest request = ReadForceSpeedChangeAckRequest(GetPlayer(), recvData);
    MovementInfo const& movementInfo = request.movementInfo;
    uint32 opcode = request.opcode;

    // now can skip not our packet
    if (!ValidateSpeedChangeAck(recvData, _player, movementInfo))
        return;

    float newspeed = request.newSpeed;
    /*----------------*/

    // client ACK send one packet for mounted/run case and need skip all except last from its
    // in other cases anti-cheat check can be fail in false case
    UnitMoveType move_type;

    static char const* const move_type_name[MAX_MOVE_TYPE] =
    {
        "Walk",
        "Run",
        "RunBack",
        "Swim",
        "SwimBack",
        "TurnRate",
        "Flight",
        "FlightBack",
        "PitchRate"
    };

    switch (opcode)
    {
        case CMSG_MOVE_FORCE_WALK_SPEED_CHANGE_ACK:        move_type = MOVE_WALK;        break;
        case CMSG_MOVE_FORCE_RUN_SPEED_CHANGE_ACK:         move_type = MOVE_RUN;         break;
        case CMSG_MOVE_FORCE_RUN_BACK_SPEED_CHANGE_ACK:    move_type = MOVE_RUN_BACK;    break;
        case CMSG_MOVE_FORCE_SWIM_SPEED_CHANGE_ACK:        move_type = MOVE_SWIM;        break;
        case CMSG_MOVE_FORCE_SWIM_BACK_SPEED_CHANGE_ACK:   move_type = MOVE_SWIM_BACK;   break;
        case CMSG_MOVE_FORCE_TURN_RATE_CHANGE_ACK:         move_type = MOVE_TURN_RATE;   break;
        case CMSG_MOVE_FORCE_FLIGHT_SPEED_CHANGE_ACK:      move_type = MOVE_FLIGHT;      break;
        case CMSG_MOVE_FORCE_FLIGHT_BACK_SPEED_CHANGE_ACK: move_type = MOVE_FLIGHT_BACK; break;
        case CMSG_MOVE_FORCE_PITCH_RATE_CHANGE_ACK:        move_type = MOVE_PITCH_RATE;  break;
        default:
            SF_LOG_ERROR("network", "WorldSession::HandleForceSpeedChangeAck: Unknown move type opcode: %u", opcode);
            return;
    }

    // skip all forced speed changes except last and unexpected
    // in run/mounted case used one ACK and it must be skipped. m_forced_speed_changes[MOVE_RUN] store both.
    if (_player->m_forced_speed_changes[move_type] > 0)
    {
        --_player->m_forced_speed_changes[move_type];
        if (_player->m_forced_speed_changes[move_type] > 0)
            return;
    }

    if (!_player->GetTransport() && fabs(_player->GetSpeed(move_type) - newspeed) > 0.01f)
    {
        if (_player->GetSpeed(move_type) > newspeed)         // must be greater - just correct
        {
            SF_LOG_ERROR("network", "%sSpeedChange player %s is NOT correct (must be %f instead %f), force set to correct value",
                move_type_name[move_type], _player->GetName().c_str(), _player->GetSpeed(move_type), newspeed);
            _player->SetSpeed(move_type, _player->GetSpeedRate(move_type), true);
        }
        else                                                // must be lesser - cheating
        {
            SF_LOG_DEBUG("misc", "Player %s from account id %u kicked for incorrect speed (must be %f instead %f)",
                _player->GetName().c_str(), _player->GetSession()->GetAccountId(), _player->GetSpeed(move_type), newspeed);
            _player->GetSession()->KickPlayer();
        }
    }
}

void WorldSession::HandleSetActiveMoverOpcode(WorldPacket& recvPacket)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_SET_ACTIVE_MOVER");

    ActiveMoverRequest request = ReadActiveMoverRequest(recvPacket);
    ObjectGuid guid = request.guid;

    ValidateActiveMoverGuid(_player, guid);
}

void WorldSession::HandleMoveNotActiveMover(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_MOVE_NOT_ACTIVE_MOVER");

    MovementInfo mi = ReadMovementInfoRequest(GetPlayer(), recvData);
    _player->m_movementInfo = mi;
}

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket& /*recvData*/)
{
    ObjectGuid guid = GetPlayer()->GetGUID();

    WorldPacket data(SMSG_MOUNTSPECIAL_ANIM, 1 + 8);

    data.WriteGuidMask(guid, 5, 7, 0, 3, 2, 1, 4, 6);
    data.WriteGuidBytes(guid, 7, 2, 0, 4, 5, 6, 1, 3);

    GetPlayer()->SendMessageToSet(&data, false);
}

void WorldSession::HandleMoveKnockBackAck(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_MOVE_KNOCK_BACK_ACK");

    MovementInfo movementInfo = ReadMovementInfoRequest(GetPlayer(), recvData);

    if (!ValidateKnockBackAck(_player->m_mover, movementInfo))
        return;

    _player->m_movementInfo = movementInfo;

    WorldPacket data(SMSG_MOVE_UPDATE_KNOCK_BACK, 66);
    _player->WriteMovementInfo(data);
    _player->SendMessageToSet(&data, false);
}

void WorldSession::HandleMoveHoverAck(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_MOVE_HOVER_ACK");

    ReadMoveHoverAckRequest(GetPlayer(), recvData);
}

void WorldSession::HandleMoveWaterWalkAck(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_MOVE_WATER_WALK_ACK");

    ReadMovementInfoRequest(GetPlayer(), recvData);

    // Temp disable until I get home to check the structure
    /*uint64 guid;                                            // guid - unused
    recvData.readPackGUID(guid);

    recvData.read_skip<uint32>();                          // unk

    MovementInfo movementInfo;
    GetPlayer()->ReadMovementInfo(recvData, &movementInfo);

    recvData.read_skip<uint32>();                          // unk2
    */
}

void WorldSession::HandleSummonResponseOpcode(WorldPacket& recvData)
{
    if (!_player->IsAlive() || _player->IsInCombat())
        return;

    SummonResponseRequest request = ReadSummonResponseRequest(recvData);

    _player->SummonIfPossible(request.accept);
}


void WorldSession::HandleMoveSetCanFlyAckOpcode(WorldPacket& recvData)
{
    // fly mode on/off
    SF_LOG_DEBUG("network", "WORLD: CMSG_MOVE_SET_CAN_FLY_ACK");

    MovementInfo movementInfo = ReadMoveSetCanFlyAckRequest(GetPlayer(), recvData);

    _player->m_mover->m_movementInfo.flags = movementInfo.GetMovementFlags();
}

void WorldSession::HandleSetCollisionHeightAck(WorldPacket& recvPacket)
{
    SF_LOG_DEBUG("network", "CMSG_MOVE_SET_COLLISION_HEIGHT_ACK");

    ReadSetCollisionHeightAckRequest(GetPlayer(), recvPacket);
}

void WorldSession::HandleMovementForceAck(WorldPacket& recvPacket)
{
    SF_LOG_DEBUG("network", "%s", recvPacket.GetOpcode() == CMSG_MOVE_APPLY_MOVEMENT_FORCE_ACK ? "CMSG_MOVE_APPLY_MOVEMENT_FORCE_ACK" : "CMSG_MOVE_REMOVE_MOVEMENT_FORCE_ACK");

    ReadMovementForceAckRequest(GetPlayer(), recvPacket);
}

void WorldSession::HandleMoveSetCanTransitionBetweenSwimAndFlyAck(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY_ACK");

    recvData.rfinish();
}

void WorldSession::HandleMoveSetCanTurnWhileFallingAck(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: CMSG_MOVE_SET_CAN_TURN_WHILE_FALLING_ACK");

    recvData.rfinish();
}
