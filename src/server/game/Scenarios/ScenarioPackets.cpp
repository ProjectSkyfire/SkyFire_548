/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "ScenarioPackets.h"

namespace Skyfire::ScenarioPackets
{
    // The client reads Quantity with the same masked byte encoding as a guid.
    WorldPacket BuildScenarioState(ScenarioState const& state)
    {
        WorldPacket data(SMSG_SCENARIO_STATE, 4 * 7 + 3 + state.Progress.size() * 40);

        data << uint32(state.WaveMax);
        data << uint32(state.TimerDuration);
        data << uint32(state.Medal);
        data << uint32(state.ScenarioId);
        data << uint32(state.WaveCurrent);
        data << uint32(state.BonusObjectiveData);
        data << int32(state.CurrentStep);

        data.WriteBits(state.Progress.size(), 19);
        data.WriteBit(false);                   // unnamed in the 18414 sources, always sent clear

        ByteBuffer bytes;
        for (CriteriaProgress const& progress : state.Progress)
        {
            ObjectGuid quantity = progress.Quantity;
            ObjectGuid const& player = progress.PlayerGuid;

            data.WriteGuidMask(player, 3, 5);
            data.WriteGuidMask(quantity, 2);
            data.WriteGuidMask(player, 6);
            data.WriteGuidMask(quantity, 7);
            data.WriteGuidMask(player, 1, 7, 0);
            data.WriteGuidMask(quantity, 3);
            data.WriteGuidMask(player, 4);
            data.WriteGuidMask(quantity, 0, 5, 1, 4);
            data.WriteGuidMask(player, 2);
            data.WriteGuidMask(quantity, 6);
            data.WriteBits(progress.Flags, 4);

            bytes.WriteGuidBytes(quantity, 3, 7, 5);
            bytes << uint32(progress.TimeFromStart);
            bytes.WriteGuidBytes(quantity, 2);
            bytes << uint32(progress.TimeFromCreate);
            bytes.WriteGuidBytes(quantity, 6, 4);
            bytes.AppendPackedTime(progress.Date);
            bytes.WriteGuidBytes(quantity, 1);
            bytes.WriteGuidBytes(player, 3, 2, 1, 5, 4, 7);
            bytes.WriteGuidBytes(quantity, 0);
            bytes.WriteGuidBytes(player, 6, 0);
            bytes << uint32(progress.CriteriaId);
        }

        data.WriteBit(false);                   // unnamed in the 18414 sources, always sent clear
        data.FlushBits();

        if (!bytes.empty())
            data.append(bytes);

        return data;
    }

    WorldPacket BuildScenarioProgressUpdate(CriteriaProgress const& progress)
    {
        ObjectGuid quantity = progress.Quantity;
        ObjectGuid const& player = progress.PlayerGuid;

        WorldPacket data(SMSG_SCENARIO_PROGRESS_UPDATE, 3 + 8 + 8 + 4 * 4);

        data.WriteGuidMask(player, 6, 4, 5);
        data.WriteGuidMask(quantity, 1, 5, 6, 7, 4);
        data.WriteGuidMask(player, 1);
        data.WriteGuidMask(quantity, 0);
        data.WriteGuidMask(player, 2, 3);
        data.WriteGuidMask(quantity, 2, 3);
        data.WriteGuidMask(player, 0);
        data.WriteBits(progress.Flags, 4);
        data.WriteGuidMask(player, 7);
        data.FlushBits();

        data.WriteGuidBytes(quantity, 5, 2);
        data.WriteGuidBytes(player, 7, 4);
        data.WriteGuidBytes(quantity, 4, 0);
        data.WriteGuidBytes(player, 1, 2);
        data << uint32(progress.TimeFromStart);
        data.WriteGuidBytes(quantity, 3);
        data << uint32(progress.CriteriaId);
        data.WriteGuidBytes(quantity, 1);
        data.AppendPackedTime(progress.Date);
        data << uint32(progress.TimeFromCreate);
        data.WriteGuidBytes(player, 0, 3, 6);
        data.WriteGuidBytes(quantity, 7, 6);
        data.WriteGuidBytes(player, 5);

        return data;
    }
}
