/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRESERVER_GAMEOBJECT_TRANSPORT_TIMING_H
#define SKYFIRESERVER_GAMEOBJECT_TRANSPORT_TIMING_H

#include "Define.h"
#include "SharedDefines.h"

namespace Skyfire
{
namespace GameObjects
{
    inline bool UsesControlledTransportProgress(uint32 gameObjectType, uint32 transportPeriod)
    {
        return gameObjectType == GAMEOBJECT_TYPE_MO_TRANSPORT ||
            (gameObjectType == GAMEOBJECT_TYPE_TRANSPORT && transportPeriod != 0);
    }

    inline uint32 GetLegacyTransportEndpoint(uint32 transportPeriod)
    {
        return transportPeriod ? transportPeriod - 1 : 0;
    }

    inline uint32 ClampLegacyTransportPathProgress(uint32 pathProgress, uint32 transportPeriod)
    {
        if (!transportPeriod)
            return pathProgress;

        uint32 const endpoint = GetLegacyTransportEndpoint(transportPeriod);
        return pathProgress > endpoint ? endpoint : pathProgress;
    }

    inline uint32 WrapTransportPathProgress(uint32 pathProgress, uint32 transportPeriod)
    {
        return transportPeriod ? pathProgress % transportPeriod : pathProgress;
    }

    inline uint32 AdvanceTransportPathProgress(uint32 pathProgress, uint32 diff, uint32 transportPeriod)
    {
        if (!transportPeriod)
            return pathProgress + diff;

        uint32 const endpoint = GetLegacyTransportEndpoint(transportPeriod);
        if (pathProgress >= endpoint || diff >= endpoint - pathProgress)
            return endpoint;

        return pathProgress + diff;
    }

    inline uint32 GetLegacyTransportTravelTime(uint32 sourceProgress, uint32 targetProgress, uint32 transportPeriod)
    {
        if (!transportPeriod)
            return targetProgress >= sourceProgress ? targetProgress - sourceProgress : sourceProgress - targetProgress;

        if (targetProgress >= sourceProgress)
            return targetProgress - sourceProgress;

        return transportPeriod - sourceProgress + targetProgress;
    }

    inline uint32 GetLegacyTransportTransitionTime(uint32 sourceProgress, uint32 targetProgress)
    {
        return targetProgress >= sourceProgress ? targetProgress - sourceProgress : sourceProgress - targetProgress;
    }

    inline uint32 GetLegacyTransportReverseTravelTime(uint32 sourceProgress, uint32 targetProgress, uint32 transportPeriod)
    {
        if (!transportPeriod)
            return GetLegacyTransportTransitionTime(sourceProgress, targetProgress);

        if (sourceProgress >= targetProgress)
            return sourceProgress - targetProgress;

        return sourceProgress + transportPeriod - targetProgress;
    }

    inline uint32 SetLegacyTransportStoppedFlag(uint32 dynamicFlags, bool stopped)
    {
        if (stopped)
            return dynamicFlags | uint32(GO_DYNFLAG_LO_STOPPED);

        return dynamicFlags & ~uint32(GO_DYNFLAG_LO_STOPPED);
    }

    inline uint32 GetInitialLegacyTransportState(bool /*startOpen*/, bool hasStopFrames)
    {
        if (hasStopFrames)
            return 25;

        return 24;
    }

    inline uint32 InterpolateLegacyTransportPathProgress(uint32 sourceProgress, uint32 targetProgress, uint32 transportPeriod, uint32 elapsedTime, uint32 travelTime, bool invertedMovement = false)
    {
        if (!travelTime || elapsedTime >= travelTime)
            return targetProgress;

        uint32 const distance = invertedMovement ?
            GetLegacyTransportReverseTravelTime(sourceProgress, targetProgress, transportPeriod) :
            GetLegacyTransportTravelTime(sourceProgress, targetProgress, transportPeriod);
        uint32 const advanced = uint32((uint64(distance) * uint64(elapsedTime)) / uint64(travelTime));

        if (invertedMovement)
            return WrapTransportPathProgress(sourceProgress + transportPeriod - advanced, transportPeriod);

        return WrapTransportPathProgress(sourceProgress + advanced, transportPeriod);
    }

    inline uint32 GetInitialLegacyTransportPathProgress(bool hasStopFrames, uint32 transportPeriod, uint32 now)
    {
        return hasStopFrames ? 0 : WrapTransportPathProgress(now, transportPeriod);
    }

    inline uint32 GetTransportDynamicPathProgress(uint32 gameObjectType, uint32 transportPeriod, uint32 pathProgress)
    {
        if (gameObjectType == GAMEOBJECT_TYPE_TRANSPORT)
            return ClampLegacyTransportPathProgress(pathProgress, transportPeriod);

        if (gameObjectType == GAMEOBJECT_TYPE_MO_TRANSPORT)
            return WrapTransportPathProgress(pathProgress, transportPeriod);

        return pathProgress;
    }

}
}

#endif
