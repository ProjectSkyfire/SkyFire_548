/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScenarioMgr.h"
#include "DBCStores.h"
#include "Opcodes.h"
#include "Player.h"
#include "WorldPacket.h"

uint32 ScenarioMgr::GetScenarioIdForMap(uint32 map) const
{
    for (uint32 i = 0; i < sLFGDungeonStore.GetNumRows(); ++i)
        if (LFGDungeonEntry const* dungeon = sLFGDungeonStore.LookupEntry(i))
            if (uint32(dungeon->m_ContinentID) == map && dungeon->m_ScenarioID)
                return dungeon->m_ScenarioID;

    return 0;
}

uint32 ScenarioMgr::GetCriteriaIdForScenarioStep(uint32 map, uint32 step) const
{
    uint32 scenarioId = GetScenarioIdForMap(map);
    for (uint32 i = 0; i < sScenarioStepStore.GetNumRows(); ++i)
        if (ScenarioStepEntry const* scenarioStep = sScenarioStepStore.LookupEntry(i))
            if (scenarioStep->ScenarioID == scenarioId && scenarioStep->Step == step)
                return scenarioStep->CriteriaTreeID;

    return 0;
}

void ScenarioMgr::SendScenarioState(Player* player, uint32 map, int32 step, uint32 startTime, uint32 miscCriteriaTreeId, uint32 scenarioId, uint32 bonusData)
{
    if (!player)
        return;

    // The client needs an empty state before the first step.
    if (step == 0)
        SendScenarioState(player, map, -1, 0, miscCriteriaTreeId, scenarioId);

    uint32 criteriaTreeId = miscCriteriaTreeId ? miscCriteriaTreeId : GetCriteriaIdForScenarioStep(map, step);

    ObjectGuid playerGuid = player->GetGUID();
    ObjectGuid unkGuid = 0;

    ByteBuffer bytesData;

    WorldPacket data(SMSG_SCENARIO_STATE);
    data << uint32(0);                                      // challenge max wave
    data << uint32(0);                                      // challenge timer
    data << uint32(0);                                      // challenge medal
    data << uint32(scenarioId ? scenarioId : GetScenarioIdForMap(map));
    data << uint32(0);                                      // challenge current wave
    data << uint32(bonusData);
    data << uint32(step);
    data.WriteBits(criteriaTreeId ? 1 : 0, 19);
    data.WriteBit(false);

    if (criteriaTreeId)
    {
        data.WriteBit(playerGuid[3]);
        data.WriteBit(playerGuid[5]);
        data.WriteBit(unkGuid[2]);
        data.WriteBit(playerGuid[6]);
        data.WriteBit(unkGuid[7]);
        data.WriteBit(playerGuid[1]);
        data.WriteBit(playerGuid[7]);
        data.WriteBit(playerGuid[0]);
        data.WriteBit(unkGuid[3]);
        data.WriteBit(playerGuid[4]);
        data.WriteBit(unkGuid[0]);
        data.WriteBit(unkGuid[5]);
        data.WriteBit(unkGuid[1]);
        data.WriteBit(unkGuid[4]);
        data.WriteBit(playerGuid[2]);
        data.WriteBit(unkGuid[6]);
        data.WriteBits(0, 4);

        bytesData.WriteByteSeq(unkGuid[3]);
        bytesData.WriteByteSeq(unkGuid[7]);
        bytesData.WriteByteSeq(unkGuid[5]);
        bytesData << uint32(startTime);
        bytesData.WriteByteSeq(unkGuid[2]);
        bytesData << uint32(startTime);
        bytesData.WriteByteSeq(unkGuid[6]);
        bytesData.WriteByteSeq(unkGuid[4]);
        bytesData.AppendPackedTime(time(NULL));
        bytesData.WriteByteSeq(unkGuid[1]);
        bytesData.WriteByteSeq(playerGuid[3]);
        bytesData.WriteByteSeq(playerGuid[2]);
        bytesData.WriteByteSeq(playerGuid[1]);
        bytesData.WriteByteSeq(playerGuid[5]);
        bytesData.WriteByteSeq(playerGuid[4]);
        bytesData.WriteByteSeq(playerGuid[7]);
        bytesData.WriteByteSeq(unkGuid[0]);
        bytesData.WriteByteSeq(playerGuid[6]);
        bytesData.WriteByteSeq(playerGuid[0]);
        bytesData << uint32(criteriaTreeId);
    }

    data.WriteBit(false);
    data.FlushBits();

    if (criteriaTreeId)
        data.append(bytesData);

    player->SendDirectMessage(&data);

    if (step <= 0 || !criteriaTreeId)
        return;

    WorldPacket progress(SMSG_SCENARIO_PROGRESS_UPDATE);
    progress.WriteBit(unkGuid[6]);
    progress.WriteBit(unkGuid[4]);
    progress.WriteBit(unkGuid[5]);
    progress.WriteBit(playerGuid[1]);
    progress.WriteBit(playerGuid[5]);
    progress.WriteBit(playerGuid[6]);
    progress.WriteBit(playerGuid[7]);
    progress.WriteBit(playerGuid[4]);
    progress.WriteBit(unkGuid[1]);
    progress.WriteBit(playerGuid[0]);
    progress.WriteBit(unkGuid[2]);
    progress.WriteBit(unkGuid[3]);
    progress.WriteBit(playerGuid[2]);
    progress.WriteBit(playerGuid[3]);
    progress.WriteBit(unkGuid[0]);
    progress.WriteBits(0, 4);
    progress.WriteBit(unkGuid[7]);

    progress.WriteByteSeq(playerGuid[5]);
    progress.WriteByteSeq(playerGuid[2]);
    progress.WriteByteSeq(unkGuid[7]);
    progress.WriteByteSeq(unkGuid[4]);
    progress.WriteByteSeq(playerGuid[4]);
    progress.WriteByteSeq(playerGuid[0]);
    progress.WriteByteSeq(unkGuid[1]);
    progress.WriteByteSeq(unkGuid[2]);
    progress << uint32(startTime);
    progress.WriteByteSeq(playerGuid[3]);
    progress << uint32(criteriaTreeId);
    progress.WriteByteSeq(playerGuid[1]);
    progress.AppendPackedTime(time(NULL));
    progress << uint32(startTime);
    progress.WriteByteSeq(unkGuid[0]);
    progress.WriteByteSeq(unkGuid[3]);
    progress.WriteByteSeq(unkGuid[6]);
    progress.WriteByteSeq(playerGuid[7]);
    progress.WriteByteSeq(playerGuid[6]);
    progress.WriteByteSeq(unkGuid[5]);

    player->SendDirectMessage(&progress);
}
