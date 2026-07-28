/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LegacyTransportSupport.h"

namespace LegacyTransport
{
    namespace
    {
        bool IsDeeprunSubwayMap(uint32 mapId, uint32 spawnMode)
        {
            return mapId == 369 && spawnMode == 0;
        }

        bool IsThunderBluffMesaElevatorMap(uint32 mapId, uint32 spawnMode)
        {
            return mapId == 1 && spawnMode == 0;
        }

        bool IsThunderBluffMesaElevatorDbEntry(uint32 dbEntry)
        {
            switch (dbEntry)
            {
                case 4170:
                case 4171:
                case 11898:
                case 11899:
                case 47296:
                case 47297:
                    return true;
                default:
                    return false;
            }
        }
    }

    bool IsDeeprunSubwayDbEntry(uint32 dbEntry)
    {
        switch (dbEntry)
        {
            case 176080:
            case 176081:
            case 176082:
            case 176083:
            case 176084:
            case 176085:
                return true;
            default:
                return false;
        }
    }

    bool IsDeeprunSubwayClientEntry(uint32 clientEntry)
    {
        switch (clientEntry)
        {
            case 218203:
            case 218204:
            case 218205:
            case 218206:
            case 218207:
            case 218208:
                return true;
            default:
                return false;
        }
    }

    bool IsLocalTransportDbEntry(uint32 dbEntry)
    {
        return IsDeeprunSubwayDbEntry(dbEntry) || IsThunderBluffMesaElevatorDbEntry(dbEntry);
    }

    uint32 GetClientEntryForDbEntry(uint32 dbEntry)
    {
        switch (dbEntry)
        {
            case 176080:
                return 218203;
            case 176081:
                return 218204;
            case 176082:
                return 218205;
            case 176083:
                return 218206;
            case 176084:
                return 218207;
            case 176085:
                return 218208;
            default:
                return dbEntry;
        }
    }

    uint32 GetAllowedSpawnMask(uint32 dbEntry, uint32 mapId, uint32 spawnMask)
    {
        uint32 allowedSpawnMask = 0;
        for (uint8 i = 0; spawnMask != 0; ++i, spawnMask >>= 1)
            if ((spawnMask & 1) && IsAllowedOnMap(dbEntry, mapId, i))
                allowedSpawnMask |= 1u << i;

        return allowedSpawnMask;
    }

    bool IsAllowedOnMap(uint32 dbEntry, uint32 mapId, uint32 spawnMode)
    {
        if (IsDeeprunSubwayDbEntry(dbEntry))
            return IsDeeprunSubwayMap(mapId, spawnMode);

        if (IsThunderBluffMesaElevatorDbEntry(dbEntry))
            return IsThunderBluffMesaElevatorMap(mapId, spawnMode);

        return false;
    }

    bool ShouldPreservePassengerGameObjectVisibility(uint32 clientEntry)
    {
        return IsDeeprunSubwayClientEntry(clientEntry);
    }

    void LogRegisteredSpawn(LegacyTransportSpawnDiagnostic const&) { }
    void LogMissingAnimationData(uint32, uint32, uint32) { }
    void LogCreateAttempt(LegacyTransportSpawnDiagnostic const&) { }
    void LogMissingCreateAnimationData(uint32, uint32, uint32) { }
    void LogCreateFailure(LegacyTransportSpawnDiagnostic const&) { }
    void LogCreateSuccess(LegacyTransportSpawnDiagnostic const&, uint64, uint32, uint32, uint32, uint32, uint32, uint32, uint32) { }
    void LogNoRegisteredSpawns(uint32, uint32) { }
    void LogSpawnCount(uint32, uint32, uint32) { }
}
