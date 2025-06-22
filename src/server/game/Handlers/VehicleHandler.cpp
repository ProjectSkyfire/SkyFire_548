/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Log.h"
#include "MovementStructures.h"
#include "ObjectAccessor.h"
#include "Opcodes.h"
#include "Player.h"
#include "Vehicle.h"
#include "WorldPacket.h"
#include "WorldSession.h"

void WorldSession::HandleDismissControlledVehicle(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_DISMISS_CONTROLLED_VEHICLE");

    uint64 vehicleGUID = _player->GetCharmGUID();

    if (!vehicleGUID)                                       // something wrong here...
    {
        recvData.rfinish();                                // prevent warnings spam
        return;
    }

    MovementInfo mi;
    _player->ReadMovementInfo(recvData, &mi);

    _player->m_movementInfo = mi;

    _player->ExitVehicle();
}

void WorldSession::HandleChangeSeatsOnControlledVehicle(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE");

    Unit* vehicle_base = GetPlayer()->GetVehicleBase();
    if (!vehicle_base)
    {
        recvData.rfinish();                                // prevent warnings spam
        return;
    }

    VehicleSeatEntry const* seat = GetPlayer()->GetVehicle()->GetSeatForPassenger(GetPlayer());
    if (!seat->CanSwitchFromSeat())
    {
        recvData.rfinish();                                // prevent warnings spam
        SF_LOG_ERROR("network", "HandleChangeSeatsOnControlledVehicle, Opcode: %u, Player %u tried to switch seats but current seatflags %u don't permit that.",
            recvData.GetOpcode(), GetPlayer()->GetGUIDLow(), seat->m_flags);
        return;
    }

    switch (recvData.GetOpcode())
    {
        case CMSG_REQUEST_VEHICLE_PREV_SEAT:
            GetPlayer()->ChangeSeat(-1, false);
            break;
        case CMSG_REQUEST_VEHICLE_NEXT_SEAT:
            GetPlayer()->ChangeSeat(-1, true);
            break;
        case CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE:
        {
            static MovementStatusElements const accessoryGuid[] =
            {
                MSEExtraInt8,
                MSEHasGuidByte2,
                MSEHasGuidByte1,
                MSEHasGuidByte7,
                MSEHasGuidByte5,
                MSEHasGuidByte3,
                MSEHasGuidByte6,
                MSEHasGuidByte4,
                MSEHasGuidByte0,
                MSEGuidByte5,
                MSEGuidByte4,
                MSEGuidByte7,
                MSEGuidByte1,
                MSEGuidByte3,
                MSEGuidByte2,
                MSEGuidByte6,
                MSEGuidByte0,
            };

            Movement::ExtraMovementStatusElement extra(accessoryGuid);
            MovementInfo movementInfo;
            GetPlayer()->ReadMovementInfo(recvData, &movementInfo, &extra);
            vehicle_base->m_movementInfo = movementInfo;

            uint64 accessory = extra.Data.guid;
            int8 seatId = extra.Data.byteData;

            if (vehicle_base->GetGUID() != movementInfo.guid)
                return;

            if (!accessory)
                GetPlayer()->ChangeSeat(-1, seatId > 0); // prev/next
            else if (Unit* vehUnit = Unit::GetUnit(*GetPlayer(), accessory))
            {
                if (Vehicle* vehicle = vehUnit->GetVehicleKit())
                    if (vehicle->HasEmptySeat(seatId))
                        vehUnit->HandleSpellClick(GetPlayer(), seatId);
            }
            break;
        }
        case CMSG_REQUEST_VEHICLE_SWITCH_SEAT:
        {
            int8 seatId;
            ObjectGuid VehicleGUID;        // current vehicle guid
            recvData >> seatId;
            recvData.ReadGuidMask(VehicleGUID, 7, 3, 0, 1, 6, 4, 5, 2);
            recvData.ReadGuidBytes(VehicleGUID, 5, 0, 2, 3, 7, 4, 6, 1);

            if (vehicle_base->GetGUID() == VehicleGUID)
                GetPlayer()->ChangeSeat(seatId);
            else if (Unit* vehUnit = Unit::GetUnit(*GetPlayer(), VehicleGUID))
                if (Vehicle* vehicle = vehUnit->GetVehicleKit())
                    if (vehicle->HasEmptySeat(seatId))
                        vehUnit->HandleSpellClick(GetPlayer(), seatId);
            break;
        }
        default:
            break;
    }
}

void WorldSession::HandleRideVehicleInteract(WorldPacket& recvData)
{
    ObjectGuid VehicleGUID;
    recvData.ReadGuidMask(VehicleGUID, 5, 7, 3, 0, 2, 4, 6, 1);
    recvData.ReadGuidBytes(VehicleGUID, 5, 3, 1, 2, 7, 0, 6, 4);

    if (Player* player = ObjectAccessor::FindPlayer(VehicleGUID))
    {
        if (!player->GetVehicleKit())
            return;
        if (!player->IsInRaidWith(_player))
            return;
        if (!player->IsWithinDistInMap(_player, INTERACTION_DISTANCE))
            return;

        _player->EnterVehicle(player);
    }
}

void WorldSession::HandleEjectPassenger(WorldPacket& data)
{
    Vehicle* vehicle = _player->GetVehicleKit();
    if (!vehicle)
    {
        data.rfinish();                                     // prevent warnings spam
        SF_LOG_ERROR("network", "HandleEjectPassenger: Player %u is not in a vehicle!", GetPlayer()->GetGUIDLow());
        return;
    }

    ObjectGuid PassengerGUID;
    data.ReadGuidMask(PassengerGUID, 4, 2, 1, 6, 5, 0, 7, 3);
    data.ReadGuidBytes(PassengerGUID, 2, 7, 0, 6, 4, 3, 5, 1);

    if (IS_PLAYER_GUID(PassengerGUID))
    {
        Player* player = ObjectAccessor::FindPlayer(PassengerGUID);
        if (!player)
        {
            SF_LOG_ERROR("network", "Player %u tried to eject player %u from vehicle, but the latter was not found in world!", GetPlayer()->GetGUIDLow(), GUID_LOPART(PassengerGUID));
            return;
        }

        if (!player->IsOnVehicle(vehicle->GetBase()))
        {
            SF_LOG_ERROR("network", "Player %u tried to eject player %u, but they are not in the same vehicle", GetPlayer()->GetGUIDLow(), GUID_LOPART(PassengerGUID));
            return;
        }

        VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(player);
        ASSERT(seat);
        if (seat->IsEjectable())
            player->ExitVehicle();
        else
            SF_LOG_ERROR("network", "Player %u attempted to eject player %u from non-ejectable seat.", GetPlayer()->GetGUIDLow(), GUID_LOPART(PassengerGUID));
    }

    else if (IS_CREATURE_GUID(PassengerGUID))
    {
        Unit* unit = ObjectAccessor::GetUnit(*_player, PassengerGUID);
        if (!unit) // creatures can be ejected too from player mounts
        {
            SF_LOG_ERROR("network", "Player %u tried to eject creature guid %u from vehicle, but the latter was not found in world!", GetPlayer()->GetGUIDLow(), GUID_LOPART(PassengerGUID));
            return;
        }

        if (!unit->IsOnVehicle(vehicle->GetBase()))
        {
            SF_LOG_ERROR("network", "Player %u tried to eject unit %u, but they are not in the same vehicle", GetPlayer()->GetGUIDLow(), GUID_LOPART(PassengerGUID));
            return;
        }

        VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(unit);
        ASSERT(seat);
        if (seat->IsEjectable())
        {
            ASSERT(GetPlayer() == vehicle->GetBase());
            unit->ExitVehicle();
        }
        else
            SF_LOG_ERROR("network", "Player %u attempted to eject creature GUID %u from non-ejectable seat.", GetPlayer()->GetGUIDLow(), GUID_LOPART(PassengerGUID));
    }
    else
        SF_LOG_ERROR("network", "HandleEjectPassenger: Player %u tried to eject invalid GUID %u", GetPlayer()->GetGUIDLow(), GUID_LOPART(PassengerGUID));
}

void WorldSession::HandleRequestVehicleExit(WorldPacket& /*recvData*/)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_REQUEST_VEHICLE_EXIT");

    if (Vehicle* vehicle = GetPlayer()->GetVehicle())
    {
        if (VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(GetPlayer()))
        {
            if (seat->CanEnterOrExit())
                GetPlayer()->ExitVehicle();
            else
                SF_LOG_ERROR("network", "Player %u tried to exit vehicle, but seatflags %u (ID: %u) don't permit that.",
                    GetPlayer()->GetGUIDLow(), seat->m_ID, seat->m_flags);
        }
    }
}

void WorldSession::HandleMoveSetVehicleRecAck(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Recvd CMSG_SET_VEHICLE_REC_ID_ACK");

    static MovementStatusElements const vehicleIdExtra = MSEExtraInt32;
    Movement::ExtraMovementStatusElement extras(&vehicleIdExtra);
    MovementInfo movementInfo;
    GetPlayer()->ReadMovementInfo(recvData, &movementInfo, &extras);

    uint32 vehicleId = extras.Data.extraInt32Data;

    ObjectGuid vehicleGuid = GetPlayer()->GetGUID();
    WorldPacket data(SMSG_SET_VEHICLE_REC_ID, 8 + 4);
    data.WriteGuidMask(vehicleGuid, 5, 7, 2, 1, 4, 0, 3, 6);
    data.WriteGuidBytes(vehicleGuid, 5, 7, 4, 6, 2, 1, 3, 0);
    data << uint32(vehicleId);
    GetPlayer()->SendMessageToSet(&data, true);
}
