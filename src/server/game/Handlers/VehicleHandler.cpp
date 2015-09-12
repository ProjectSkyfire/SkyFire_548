/*
 * Copyright (C) 2011-2015 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2015 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Vehicle.h"
#include "Player.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "MovementStructures.h"

void WorldSession::HandleDismissControlledVehicle(WorldPacket &recvData)
{
    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_DISMISS_CONTROLLED_VEHICLE");

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
    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_CHANGE_SEATS_ON_CONTROLLED_VEHICLE");

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
        TC_LOG_ERROR("network", "HandleChangeSeatsOnControlledVehicle, Opcode: %u, Player %u tried to switch seats but current seatflags %u don't permit that.",
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
            // more research before enable it !!
            float x, y, z;
            int8 seatId;
            ObjectGuid playerGUID;
            ObjectGuid accessoryGUID;
            ObjectGuid transportGUID;

            recvData >> seatId >> y >> z >> x;

            accessoryGUID[2] = recvData.ReadBit();
            accessoryGUID[1] = recvData.ReadBit();
            auto bit32 = !recvData.ReadBit();
            playerGUID[6] = recvData.ReadBit();
            playerGUID[0] = recvData.ReadBit();
            playerGUID[2] = recvData.ReadBit();
            accessoryGUID[7] = recvData.ReadBit();
            playerGUID[7] = recvData.ReadBit();
            playerGUID[1] = recvData.ReadBit();
            accessoryGUID[5] = recvData.ReadBit();
            accessoryGUID[3] = recvData.ReadBit();
            auto hasTransport = recvData.ReadBit();
            auto bit28 = !recvData.ReadBit();
            accessoryGUID[6] = recvData.ReadBit();
            accessoryGUID[4] = recvData.ReadBit();
            playerGUID[4] = recvData.ReadBit();
            uint32 unkcounter = recvData.ReadBits(22);
            accessoryGUID[0] = recvData.ReadBit();

            auto unkBit0 = !recvData.ReadBit();
            playerGUID[5] = recvData.ReadBit();
            auto bit172 = recvData.ReadBit();
            auto bit148 = recvData.ReadBit();
            auto unkBit1 = !recvData.ReadBit();
            auto bit140 = recvData.ReadBit();
            auto bit24 = !recvData.ReadBit();
            auto bit168 = !recvData.ReadBit();
            auto bit149 = recvData.ReadBit();
            playerGUID[3] = recvData.ReadBit();
            auto unkBit2 = !recvData.ReadBit();

            auto bit92 = false;
            auto bit100 = false;
            if (hasTransport)
            {
                transportGUID[6] = recvData.ReadBit();
                transportGUID[0] = recvData.ReadBit();
                transportGUID[3] = recvData.ReadBit();
                transportGUID[7] = recvData.ReadBit();
                bit92 = recvData.ReadBit();
                bit100 = recvData.ReadBit();
                transportGUID[4] = recvData.ReadBit();
                transportGUID[1] = recvData.ReadBit();
                transportGUID[2] = recvData.ReadBit();
                transportGUID[5] = recvData.ReadBit();
            }

            if (bit24)
                recvData.ReadBits(30);

            if (bit28)
                recvData.ReadBits(13);

            auto bit136 = false;
            if (bit140)
                bit136 = recvData.ReadBit();

            recvData.read_skip(sizeof(uint32) * unkcounter);

            recvData.ReadByteSeq(playerGUID[6]);
            recvData.ReadByteSeq(accessoryGUID[5]);
            recvData.ReadByteSeq(playerGUID[2]);
            recvData.ReadByteSeq(playerGUID[5]);
            recvData.ReadByteSeq(accessoryGUID[4]);
            recvData.ReadByteSeq(accessoryGUID[7]);
            recvData.ReadByteSeq(playerGUID[3]);
            recvData.ReadByteSeq(playerGUID[7]);
            recvData.ReadByteSeq(accessoryGUID[1]);
            recvData.ReadByteSeq(accessoryGUID[3]);
            recvData.ReadByteSeq(accessoryGUID[2]);
            recvData.ReadByteSeq(playerGUID[4]);
            recvData.ReadByteSeq(playerGUID[1]);
            recvData.ReadByteSeq(accessoryGUID[6]);
            recvData.ReadByteSeq(accessoryGUID[0]);
            recvData.ReadByteSeq(playerGUID[0]);

            if (bit168)
                recvData.read_skip<uint32>();

            if (bit140)
            {
                recvData.read_skip<float>();

                if (bit136)
                {
                    recvData.read_skip<float>();
                    recvData.read_skip<float>();
                    recvData.read_skip<float>();
                }
                recvData.read_skip<uint32>();
            }

            if (hasTransport)
            {
                recvData.read_skip<uint32>();
                recvData.ReadByteSeq(transportGUID[6]);
                recvData.read_skip<float>();
                recvData.ReadByteSeq(transportGUID[4]);
                recvData.ReadByteSeq(transportGUID[7]);
                recvData.read_skip<float>();
                recvData.ReadByteSeq(transportGUID[5]);
                recvData.read_skip<uint8>();
                recvData.read_skip<float>();

                if (bit100)
                    recvData.read_skip<uint32>();

                recvData.ReadByteSeq(transportGUID[1]);
                recvData.ReadByteSeq(transportGUID[2]);
                recvData.ReadByteSeq(transportGUID[0]);

                if (bit92)
                    recvData.read_skip<uint32>();

                recvData.read_skip<float>();
                recvData.ReadByteSeq(transportGUID[3]);
            }

            if (unkBit0)
                recvData.read_skip<float>();

            if (unkBit1)
                recvData.read_skip<float>();

            if (unkBit2)
                recvData.read_skip<float>();

            if (bit32)
                recvData.read_skip<uint32>();
            
            if (!accessoryGUID)
                GetPlayer()->ChangeSeat(-1, seatId > 0); // prev/next
            else if (Unit* vehUnit = Unit::GetUnit(*GetPlayer(), accessoryGUID))
            {
                if (Vehicle* vehicle = vehUnit->GetVehicleKit())
                    if (vehicle->HasEmptySeat(seatId))
                        vehUnit->HandleSpellClick(GetPlayer(), seatId);
            }
            break;
        }
        case CMSG_REQUEST_VEHICLE_SWITCH_SEAT:
        {
            uint64 guid;        // current vehicle guid
            recvData.readPackGUID(guid);

            int8 seatId;
            recvData >> seatId;

            if (vehicle_base->GetGUID() == guid)
                GetPlayer()->ChangeSeat(seatId);
            else if (Unit* vehUnit = Unit::GetUnit(*GetPlayer(), guid))
                if (Vehicle* vehicle = vehUnit->GetVehicleKit())
                    if (vehicle->HasEmptySeat(seatId))
                        vehUnit->HandleSpellClick(GetPlayer(), seatId);
            break;
        }
        default:
            break;
    }
}

void WorldSession::HandleEnterPlayerVehicle(WorldPacket& recvData)
{
    ObjectGuid Guid;
    Guid[5] = recvData.ReadBit();
    Guid[7] = recvData.ReadBit();
    Guid[3] = recvData.ReadBit();
    Guid[0] = recvData.ReadBit();
    Guid[2] = recvData.ReadBit();
    Guid[4] = recvData.ReadBit();
    Guid[6] = recvData.ReadBit();
    Guid[1] = recvData.ReadBit();

    recvData.ReadByteSeq(Guid[5]);
    recvData.ReadByteSeq(Guid[3]);
    recvData.ReadByteSeq(Guid[1]);
    recvData.ReadByteSeq(Guid[2]);
    recvData.ReadByteSeq(Guid[7]);
    recvData.ReadByteSeq(Guid[0]);
    recvData.ReadByteSeq(Guid[6]);
    recvData.ReadByteSeq(Guid[4]);

    if (Player* player = ObjectAccessor::FindPlayer(Guid))
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
        TC_LOG_ERROR("network", "HandleEjectPassenger: Player %u is not in a vehicle!", GetPlayer()->GetGUIDLow());
        return;
    }

    uint64 guid;
    data >> guid;

    if (IS_PLAYER_GUID(guid))
    {
        Player* player = ObjectAccessor::FindPlayer(guid);
        if (!player)
        {
            TC_LOG_ERROR("network", "Player %u tried to eject player %u from vehicle, but the latter was not found in world!", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        if (!player->IsOnVehicle(vehicle->GetBase()))
        {
            TC_LOG_ERROR("network", "Player %u tried to eject player %u, but they are not in the same vehicle", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(player);
        ASSERT(seat);
        if (seat->IsEjectable())
            player->ExitVehicle();
        else
            TC_LOG_ERROR("network", "Player %u attempted to eject player %u from non-ejectable seat.", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
    }

    else if (IS_CREATURE_GUID(guid))
    {
        Unit* unit = ObjectAccessor::GetUnit(*_player, guid);
        if (!unit) // creatures can be ejected too from player mounts
        {
            TC_LOG_ERROR("network", "Player %u tried to eject creature guid %u from vehicle, but the latter was not found in world!", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
            return;
        }

        if (!unit->IsOnVehicle(vehicle->GetBase()))
        {
            TC_LOG_ERROR("network", "Player %u tried to eject unit %u, but they are not in the same vehicle", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
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
            TC_LOG_ERROR("network", "Player %u attempted to eject creature GUID %u from non-ejectable seat.", GetPlayer()->GetGUIDLow(), GUID_LOPART(guid));
    }
    else
        TC_LOG_ERROR("network", "HandleEjectPassenger: Player %u tried to eject invalid GUID " UI64FMTD, GetPlayer()->GetGUIDLow(), guid);
}

void WorldSession::HandleRequestVehicleExit(WorldPacket& /*recvData*/)
{
    TC_LOG_DEBUG("network", "WORLD: Recvd CMSG_REQUEST_VEHICLE_EXIT");

    if (Vehicle* vehicle = GetPlayer()->GetVehicle())
    {
        if (VehicleSeatEntry const* seat = vehicle->GetSeatForPassenger(GetPlayer()))
        {
            if (seat->CanEnterOrExit())
                GetPlayer()->ExitVehicle();
            else
                TC_LOG_ERROR("network", "Player %u tried to exit vehicle, but seatflags %u (ID: %u) don't permit that.",
                GetPlayer()->GetGUIDLow(), seat->m_ID, seat->m_flags);
        }
    }
}
