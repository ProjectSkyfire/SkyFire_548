/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_CHARACTER_METRICS_H
#define SKYFIRE_CHARACTER_METRICS_H
#include "ClusterProtocol.h"
namespace Skyfire::Cluster
{
    constexpr Message CharacterMetricsType = Message(10);
    inline bool DecodeCharacterMetrics(std::vector<std::uint8_t> const& bytes, CharacterMetrics& metrics)
    {
        Reader input(bytes); std::uint8_t version;
        return input.U8(version) && version == 1 && input.U32(metrics.Uptime) &&
            input.U32(metrics.CpuBasisPoints) && metrics.CpuBasisPoints <= 10000 &&
            input.U32(metrics.MemoryMiB) && input.U32(metrics.Requests) && input.U32(metrics.Failures) &&
            input.U32(metrics.Reads) && input.U32(metrics.Writes) && input.U32(metrics.Transactions) &&
            input.U32(metrics.Connections) && metrics.Connections <= 128 &&
            input.U32(metrics.Pending) && metrics.Pending <= 128 && input.U32(metrics.LatencyUs) &&
            input.U32(metrics.LastCommitAge) && input.U32(metrics.DatabaseReady) && metrics.DatabaseReady <= 1 && input.End();
    }
}
#endif
