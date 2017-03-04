/*
 * Copyright (C) 2011-2017 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2017 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2017 MaNGOS <https://www.getmangos.eu/>
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

#include "Common.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "Path.h"
#include "MovementStructures.h"
#include "WaypointMovementGenerator.h"

void WorldSession::HandleTaxiNodeStatusQueryOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_TAXI_NODE_STATUS_QUERY");
        
    ObjectGuid guid;

    recvData.ReadGuidMask(guid, 0, 1, 6, 4, 5, 2, 3, 7);
    recvData.ReadGuidBytes(guid, 4, 1, 5, 0, 2, 7, 6, 3);
    
    SendTaxiStatus(guid);
}

void WorldSession::SendTaxiStatus(uint64 guid)
{
    // cheating checks
    Creature* unit = GetPlayer()->GetMap()->GetCreature(guid);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WorldSession::SendTaxiStatus - Unit (GUID: %u) not found.", uint32(GUID_LOPART(guid)));
        return;
    }

    uint32 curloc = sObjectMgr->GetNearestTaxiNode(unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ(), unit->GetMapId(), GetPlayer()->GetTeam());

    // not found nearest
    if (curloc == 0)
        return;

    SF_LOG_DEBUG("network", "WORLD: current location %u ", curloc);

    ObjectGuid Guid = guid;
    WorldPacket data(SMSG_TAXI_NODE_STATUS, 9);

    data.WriteBit(Guid[6]);
    data.WriteBit(Guid[2]);
    data.WriteBit(Guid[7]);
    data.WriteBit(Guid[5]);
    data.WriteBit(Guid[4]);
    data.WriteBit(Guid[1]); 
    data.WriteBits(!GetPlayer()->m_taxi.IsTaximaskNodeKnown(curloc), 2);
    data.WriteBit(Guid[3]);
    data.WriteBit(Guid[0]);

    data.WriteByteSeq(Guid[0]);
    data.WriteByteSeq(Guid[5]);
    data.WriteByteSeq(Guid[2]);
    data.WriteByteSeq(Guid[1]);
    data.WriteByteSeq(Guid[4]);
    data.WriteByteSeq(Guid[6]);
    data.WriteByteSeq(Guid[7]);
    data.WriteByteSeq(Guid[3]);

    SendPacket(&data);

    SF_LOG_DEBUG("network", "WORLD: Sent SMSG_TAXI_NODE_STATUS");
}

void WorldSession::HandleTaxiQueryAvailableNodes(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_TAXI_QUERY_AVAILABLE_NODES");

    ObjectGuid guid;

    recvData.ReadGuidMask(guid, 7, 1, 0, 4, 2, 5, 6, 3);
    recvData.ReadGuidBytes(guid, 0, 3, 7, 5, 2, 6, 4, 1);

    // cheating checks
    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!unit)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleTaxiQueryAvailableNodes - Unit (GUID: %u) not found or you can't interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // unknown taxi node case
    if (SendLearnNewTaxiNode(unit))
        return;

    // known taxi node case
    SendTaxiMenu(unit);
}

void WorldSession::SendTaxiMenu(Creature* unit)
{
    // find current node
    uint32 curloc = sObjectMgr->GetNearestTaxiNode(unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ(), unit->GetMapId(), GetPlayer()->GetTeam());
    if (!curloc)
        return;

    bool lastTaxiCheaterState = GetPlayer()->isTaxiCheater();
    if (unit->GetEntry() == 29480)
        GetPlayer()->SetTaxiCheater(true); // Grimwing in Ebon Hold, special case. NOTE: Not perfect, Zul'Aman should not be included according to WoWhead, and I think taxicheat includes it.

    SF_LOG_DEBUG("network", "WORLD: CMSG_TAXI_NODE_STATUS_QUERY %u ", curloc);
    ObjectGuid Guid = unit->GetGUID();

    WorldPacket data(SMSG_SHOW_TAXI_NODES, (4 + 8 + 4 + 4 * 4));
    data.WriteBit(1); //unk
    data.WriteGuidMask(Guid, 3, 0, 4, 2, 1, 7, 6, 5);
    data.WriteBits(TaxiMaskSize, 24);

    data.WriteGuidBytes(Guid, 0, 3);
    data << uint32(curloc);
    data.WriteGuidBytes(Guid, 5, 2, 6, 1, 7, 4);

    GetPlayer()->m_taxi.AppendTaximaskTo(data, GetPlayer()->isTaxiCheater());
    SendPacket(&data);

    SF_LOG_DEBUG("network", "WORLD: Sent SMSG_SHOW_TAXI_NODES");

    GetPlayer()->SetTaxiCheater(lastTaxiCheaterState);
}

void WorldSession::SendDoFlight(uint32 mountDisplayId, uint32 path, uint32 pathNode)
{
    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    while (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
        GetPlayer()->GetMotionMaster()->MovementExpired(false);

    if (mountDisplayId)
        GetPlayer()->Mount(mountDisplayId);

    GetPlayer()->GetMotionMaster()->MoveTaxiFlight(path, pathNode);
}

bool WorldSession::SendLearnNewTaxiNode(Creature* unit)
{
    // find current node
    uint32 curloc = sObjectMgr->GetNearestTaxiNode(unit->GetPositionX(), unit->GetPositionY(), unit->GetPositionZ(), unit->GetMapId(), GetPlayer()->GetTeam());

    if (curloc == 0)
        return true; 
    // `true` send to avoid WorldSession::SendTaxiMenu call with one more curlock seartch with same false result.

    if (GetPlayer()->m_taxi.SetTaximaskNode(curloc))
    {
        WorldPacket msg(SMSG_NEW_TAXI_PATH, 0);
        SendPacket(&msg);

        WorldPacket update(SMSG_TAXI_NODE_STATUS, 9);
        update.WriteBit(1);
        ObjectGuid guid = unit->GetGUID();
        
        update.WriteGuidMask(guid, 1, 7, 6, 5, 4, 0, 2, 3);
        update.WriteGuidBytes(guid, 4, 2, 3, 6, 7, 0, 1, 5);

        SendPacket(&update);

        return true;
    }
    else
        return false;
}

void WorldSession::SendDiscoverNewTaxiNode(uint32 nodeid)
{
    if (GetPlayer()->m_taxi.SetTaximaskNode(nodeid))
    {
        WorldPacket msg(SMSG_NEW_TAXI_PATH, 0);
        SendPacket(&msg);
    }
}

void WorldSession::HandleActivateTaxiExpressOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_ACTIVATE_TAXI_EXPRESS");

    ObjectGuid guid;
    uint32 node_count;

    recvData.ReadGuidMask(guid, 6, 7);
    node_count = recvData.ReadBits(22);
    printf("nodes_count [%u]\n", node_count);
    recvData.ReadGuidMask(guid, 2, 0, 4, 3, 1, 5);
    recvData.ReadGuidBytes(guid, 2, 7, 1);

    std::vector<uint32> nodes;

    for (uint32 i = 0; i < node_count; ++i)
    {
        uint32 node;
        recvData >> node;
        nodes.push_back(node);
    }

    recvData.ReadGuidBytes(guid, 0, 5, 3, 6, 4);

    Creature* npc = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!npc)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleActivateTaxiExpressOpcode - Unit (GUID: %u) not found or you can't interact with it.", uint32(GUID_LOPART(guid)));
        return;
    }

    SF_LOG_DEBUG("network", "WORLD: Received CMSG_ACTIVATE_TAXI_EXPRESS from %d to %d", nodes.front(), nodes.back());

    GetPlayer()->ActivateTaxiPathTo(nodes, npc);
}

void WorldSession::HandleMoveSplineDoneOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_MOVE_SPLINE_DONE");

    uint32 SplineID;
    recvData >> SplineID;
    MovementInfo movementInfo;                              // used only for proper packet read
    _player->ReadMovementInfo(recvData, &movementInfo);

    // in taxi flight packet received in 2 case:
    // 1) end taxi path in far (multi-node) flight
    // 2) switch from one map to other in case multim-map taxi path
    // we need process only (1)

    uint32 curDest = GetPlayer()->m_taxi.GetTaxiDestination();
    if (!curDest)
        return;

    TaxiNodesEntry const* curDestNode = sTaxiNodesStore.LookupEntry(curDest);

    // far teleport case
    if (curDestNode && curDestNode->map_id != GetPlayer()->GetMapId())
    {
        if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
        {
            // short preparations to continue flight
            FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());

            flight->SetCurrentNodeAfterTeleport();
            TaxiPathNodeEntry const& node = flight->GetPath()[flight->GetCurrentNode()];
            flight->SkipCurrentNode();

            GetPlayer()->TeleportTo(curDestNode->map_id, node.x, node.y, node.z, GetPlayer()->GetOrientation());
        }
        return;
    }

    uint32 destinationnode = GetPlayer()->m_taxi.NextTaxiDestination();
    if (destinationnode > 0)                              // if more destinations to go
    {
        // current source node for next destination
        uint32 sourcenode = GetPlayer()->m_taxi.GetTaxiSource();

        // Add to taximask middle hubs in taxicheat mode (to prevent having player with disabled taxicheat and not having back flight path)
        if (GetPlayer()->isTaxiCheater())
        {
            if (GetPlayer()->m_taxi.SetTaximaskNode(sourcenode))
            {
                WorldPacket data(SMSG_NEW_TAXI_PATH, 0);
                _player->GetSession()->SendPacket(&data);
            }
        }

        SF_LOG_DEBUG("network", "WORLD: Taxi has to go from %u to %u", sourcenode, destinationnode);

        uint32 mountDisplayId = sObjectMgr->GetTaxiMountDisplayId(sourcenode, GetPlayer()->GetTeam());

        uint32 path, cost;
        sObjectMgr->GetTaxiPath(sourcenode, destinationnode, path, cost);

        if (path && mountDisplayId)
            SendDoFlight(mountDisplayId, path, 1);        // skip start fly node
        else
            GetPlayer()->m_taxi.ClearTaxiDestinations();    // clear problematic path and next
        return;
    }
    else
        GetPlayer()->m_taxi.ClearTaxiDestinations();        // not destinations, clear source node

    GetPlayer()->CleanupAfterTaxiFlight();
    GetPlayer()->SetFallInformation(0, GetPlayer()->GetPositionZ());
    if (GetPlayer()->pvpInfo.IsHostile)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);
}

void WorldSession::HandleActivateTaxiOpcode(WorldPacket& recvData)
{
    SF_LOG_DEBUG("network", "WORLD: Received CMSG_ACTIVATE_TAXI");

    ObjectGuid guid;
    std::vector<uint32> nodes;
    nodes.resize(2);

    recvData >> nodes[1] >> nodes[0];
    recvData.ReadGuidMask(guid, 4, 0, 1, 2, 5, 6, 7, 3);
    recvData.ReadGuidBytes(guid, 1, 0, 6, 5, 2, 4, 3, 7);

    SF_LOG_DEBUG("network", "WORLD: Received CMSG_ACTIVATE_TAXI from %d to %d", nodes[0], nodes[1]);
    Creature* npc = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_FLIGHTMASTER);
    if (!npc)
    {
        SF_LOG_DEBUG("network", "WORLD: HandleActivateTaxiOpcode - Unit (GUID: %u) not found or you can't interact with it.", uint32(GUID_LOPART(guid)));
        return;
    }

    GetPlayer()->ActivateTaxiPathTo(nodes, npc);
}

void WorldSession::SendActivateTaxiReply(ActivateTaxiReply reply)
{
    ObjectGuid guid(_player->GetGUID());

    WorldPacket data(SMSG_ACTIVATE_TAXI_REPLY, 1 + 1 + 8);
    data.WriteGuidMask(guid, 2, 7);
    data.WriteBit(!reply);
    data.WriteGuidMask(guid, 0, 3, 6, 5, 1, 4);
    data.FlushBits();
    data.WriteGuidBytes(guid, 1, 5, 7, 4, 2, 6, 3, 0);

    SendPacket(&data);

    SF_LOG_DEBUG("network", "WORLD: Sent SMSG_ACTIVATE_TAXI_REPLY");
}
