/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "Corpse.h"
#include "Player.h"
#include "SpellAuras.h"
#include "MapManager.h"
#include "Transport.h"
#include "Battleground.h"
#include "WaypointMovementGenerator.h"
#include "InstanceSaveMgr.h"
#include "ObjectMgr.h"
#include "MovementStructures.h"
#include "BattlePetMgr.h"

#define MOVEMENT_PACKET_TIME_DELAY 0

void WorldSession::HandleMoveWorldportAckOpcode(WorldPacket & /*recvData*/)
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

    ObjectGuid guid;
    uint32 flags, time;
    recvPacket >> time >> flags;

    guid[0] = recvPacket.ReadBit();
    guid[7] = recvPacket.ReadBit();
    guid[3] = recvPacket.ReadBit();
    guid[5] = recvPacket.ReadBit();
    guid[4] = recvPacket.ReadBit();
    guid[6] = recvPacket.ReadBit();
    guid[1] = recvPacket.ReadBit();
    guid[2] = recvPacket.ReadBit();

    recvPacket.ReadByteSeq(guid[4]);
    recvPacket.ReadByteSeq(guid[1]);
    recvPacket.ReadByteSeq(guid[6]);
    recvPacket.ReadByteSeq(guid[7]);
    recvPacket.ReadByteSeq(guid[0]);
    recvPacket.ReadByteSeq(guid[2]);
    recvPacket.ReadByteSeq(guid[5]);
    recvPacket.ReadByteSeq(guid[3]);

    SF_LOG_DEBUG("network", "Guid " UI64FMTD, uint64(guid));
    SF_LOG_DEBUG("network", "Flags %u, time %u", flags, time/IN_MILLISECONDS);

    Player* plMover = _player->m_mover->ToPlayer();

    if (!plMover || !plMover->IsBeingTeleportedNear())
        return;

    if (guid != plMover->GetGUID())
        return;

    plMover->SetSemaphoreTeleportNear(false);

    uint32 old_zone = plMover->GetZoneId();

    WorldLocation const& dest = plMover->GetTeleportDest();

    plMover->UpdatePosition(dest, true);

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

    // ignore, waiting processing in WorldSession::HandleMoveWorldportAckOpcode and WorldSession::HandleMoveTeleportAck
    if (plrMover && plrMover->IsBeingTeleported())
    {
        recvPacket.rfinish();                     // prevent warnings spam
        return;
    }

    /* extract packet */
    MovementInfo movementInfo;
    GetPlayer()->ReadMovementInfo(recvPacket, &movementInfo);

    // prevent tampered movement data
    if (movementInfo.guid != mover->GetGUID())
    {
        SF_LOG_ERROR("network", "HandleMovementOpcodes: guid error");
        return;
    }
    if (!movementInfo.pos.IsPositionValid())
    {
        SF_LOG_ERROR("network", "HandleMovementOpcodes: Invalid Position");
        return;
    }

    /* handle special cases */
    if (movementInfo.transport.guid)
    {
        // transports size limited
        // (also received at zeppelin leave by some reason with t_* as absolute in continent coordinates, can be safely skipped)
        if (movementInfo.transport.pos.GetPositionX() > 50 || movementInfo.transport.pos.GetPositionY() > 50 || movementInfo.transport.pos.GetPositionZ() > 50)
        {
            recvPacket.rfinish();                 // prevent warnings spam
            return;
        }

        if (!Skyfire::IsValidMapCoord(movementInfo.pos.GetPositionX() + movementInfo.transport.pos.GetPositionX(), movementInfo.pos.GetPositionY() + movementInfo.transport.pos.GetPositionY(),
            movementInfo.pos.GetPositionZ() + movementInfo.transport.pos.GetPositionZ(), movementInfo.pos.GetOrientation() + movementInfo.transport.pos.GetOrientation()))
        {
            recvPacket.rfinish();                 // prevent warnings spam
            return;
        }

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
                plrMover->m_transport->RemovePassenger(plrMover);
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

    /*----------------------*/
    /* process position-change */
    // this is almost never true (not sure why it is sometimes, but it is), normally use mover->IsVehicle()
    if (mover->GetVehicle())
    {
        mover->SetOrientation(movementInfo.pos.GetOrientation());
        return;
    }

    mover->UpdatePosition(movementInfo.pos);

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

void WorldSession::HandleForceSpeedChangeAck(WorldPacket &recvData)
{
    uint32 opcode = recvData.GetOpcode();

    recvData.read_skip<uint32>(); // SplineID?

    /* extract packet */
    MovementInfo movementInfo;
    static MovementStatusElements const speedElement = MSEExtraFloat;
    Movement::ExtraMovementStatusElement extras(&speedElement);
    GetPlayer()->ReadMovementInfo(recvData, &movementInfo, &extras);

    // now can skip not our packet
    if (_player->GetGUID() != movementInfo.guid)
    {
        recvData.rfinish();                   // prevent warnings spam
        return;
    }

    float newspeed = extras.Data.floatData;
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

    ObjectGuid guid;

    recvPacket.ReadBit();

    guid[3] = recvPacket.ReadBit();
    guid[0] = recvPacket.ReadBit();
    guid[2] = recvPacket.ReadBit();
    guid[1] = recvPacket.ReadBit();
    guid[5] = recvPacket.ReadBit();
    guid[4] = recvPacket.ReadBit();
    guid[7] = recvPacket.ReadBit();
    guid[6] = recvPacket.ReadBit();

    recvPacket.ReadByteSeq(guid[3]);
    recvPacket.ReadByteSeq(guid[4]);
    recvPacket.ReadByteSeq(guid[5]);
    recvPacket.ReadByteSeq(guid[2]);
    recvPacket.ReadByteSeq(guid[7]);
    recvPacket.ReadByteSeq(guid[0]);
    recvPacket.ReadByteSeq(guid[1]);
    recvPacket.ReadByteSeq(guid[6]);

    if (GetPlayer()->IsInWorld())
    {
        if (_player->m_mover->GetGUID() != guid)
            SF_LOG_ERROR("network", "HandleSetActiveMoverOpcode: incorrect mover guid: mover is " UI64FMTD " (%s - Entry: %u) and should be " UI64FMTD, uint64(guid), GetLogNameForGuid(guid), GUID_ENPART(guid), _player->m_mover->GetGUID());
    }
}

void WorldSession::HandleMoveNotActiveMover(WorldPacket &recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_MOVE_NOT_ACTIVE_MOVER");

    MovementInfo mi;
    GetPlayer()->ReadMovementInfo(recvData, &mi);
    _player->m_movementInfo = mi;
}

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket& /*recvData*/)
{
    ObjectGuid guid = GetPlayer()->GetGUID();

    WorldPacket data(SMSG_MOUNTSPECIAL_ANIM, 1 + 8);

    data.WriteBit(guid[5]);
    data.WriteBit(guid[7]);
    data.WriteBit(guid[0]);
    data.WriteBit(guid[3]);
    data.WriteBit(guid[2]);
    data.WriteBit(guid[1]);
    data.WriteBit(guid[4]);
    data.WriteBit(guid[6]);

    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[2]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[1]);
    data.WriteByteSeq(guid[3]);

    GetPlayer()->SendMessageToSet(&data, false);
}

void WorldSession::HandleMoveKnockBackAck(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_MOVE_KNOCK_BACK_ACK");

    MovementInfo movementInfo;
    GetPlayer()->ReadMovementInfo(recvData, &movementInfo);

    if (_player->m_mover->GetGUID() != movementInfo.guid)
        return;

    _player->m_movementInfo = movementInfo;

    WorldPacket data(SMSG_MOVE_UPDATE_KNOCK_BACK, 66);
    _player->WriteMovementInfo(data);
    _player->SendMessageToSet(&data, false);
}

void WorldSession::HandleMoveHoverAck(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_MOVE_HOVER_ACK");

    uint64 guid;                                            // guid - unused
    recvData.readPackGUID(guid);

    recvData.read_skip<uint32>();                          // unk

    MovementInfo movementInfo;
    GetPlayer()->ReadMovementInfo(recvData, &movementInfo);

    recvData.read_skip<uint32>();                          // unk2
}

void WorldSession::HandleMoveWaterWalkAck(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "CMSG_MOVE_WATER_WALK_ACK");

    uint64 guid;                                            // guid - unused
    recvData.readPackGUID(guid);

    recvData.read_skip<uint32>();                          // unk

    MovementInfo movementInfo;
    GetPlayer()->ReadMovementInfo(recvData, &movementInfo);

    recvData.read_skip<uint32>();                          // unk2
}

void WorldSession::HandleSummonResponseOpcode(WorldPacket& recvData)
{
    if (!_player->IsAlive() || _player->IsInCombat())
        return;

    ObjectGuid SummonerGUID;

    SummonerGUID[1] = recvData.ReadBit();
    SummonerGUID[3] = recvData.ReadBit();
    SummonerGUID[5] = recvData.ReadBit();
    SummonerGUID[2] = recvData.ReadBit();
    bool Accept = recvData.ReadBit();
    SummonerGUID[7] = recvData.ReadBit();
    SummonerGUID[0] = recvData.ReadBit();
    SummonerGUID[4] = recvData.ReadBit();
    SummonerGUID[6] = recvData.ReadBit();

    recvData.ReadByteSeq(SummonerGUID[0]);
    recvData.ReadByteSeq(SummonerGUID[1]);
    recvData.ReadByteSeq(SummonerGUID[6]);
    recvData.ReadByteSeq(SummonerGUID[3]);
    recvData.ReadByteSeq(SummonerGUID[5]);
    recvData.ReadByteSeq(SummonerGUID[4]);
    recvData.ReadByteSeq(SummonerGUID[2]);
    recvData.ReadByteSeq(SummonerGUID[7]);

    _player->SummonIfPossible(Accept);
}

void WorldSession::HandleSetCollisionHeightAck(WorldPacket& recvPacket)
{
    SF_LOG_DEBUG("network", "CMSG_MOVE_SET_COLLISION_HEIGHT_ACK");

    static MovementStatusElements const heightElement = MSEExtraFloat;
    Movement::ExtraMovementStatusElement extra(&heightElement);
    MovementInfo movementInfo;
    GetPlayer()->ReadMovementInfo(recvPacket, &movementInfo, &extra);
}
