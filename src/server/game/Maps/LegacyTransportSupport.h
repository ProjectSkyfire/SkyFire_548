/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef LegacyTransportSupport_h__
#define LegacyTransportSupport_h__

#include "Define.h"

namespace LegacyTransport
{
    enum LegacyTransportFlags
    {
        LEGACY_TRANSPORT_FLAG_NONE = 0x00,
        LEGACY_TRANSPORT_FLAG_PRESERVE_PASSENGER_GAMEOBJECT_VISIBILITY = 0x01
    };

    struct LegacyTransportEntry
    {
        uint32 DbEntry;
        uint32 ClientEntry;
        uint32 MapId;
        uint32 SpawnMask;
        uint32 Flags;
    };

    struct LegacyTransportSpawnDiagnostic
    {
        uint32 DbGuid;
        uint32 DbEntry;
        uint32 ClientEntry;
        uint32 MapId;
        uint32 SpawnMode;
        float X;
        float Y;
        float Z;
        float O;
        uint32 AnimTime;
        uint32 AnimNodes;
    };

    void ClearLegacyTransportEntries();
    bool AddLegacyTransportEntry(LegacyTransportEntry const& entry);
    void LoadLegacyTransportEntries();

    bool IsLocalTransportDbEntry(uint32 dbEntry);
    bool IsDeeprunSubwayDbEntry(uint32 dbEntry);
    bool IsDeeprunSubwayClientEntry(uint32 clientEntry);
    uint32 GetClientEntryForDbEntry(uint32 dbEntry);
    uint32 GetAllowedSpawnMask(uint32 dbEntry, uint32 mapId, uint32 spawnMask);
    bool IsAllowedOnMap(uint32 dbEntry, uint32 mapId, uint32 spawnMode);
    bool ShouldPreservePassengerGameObjectVisibility(uint32 clientEntry);

    void LogRegisteredSpawn(LegacyTransportSpawnDiagnostic const& diagnostic);
    void LogMissingAnimationData(uint32 dbGuid, uint32 dbEntry, uint32 clientEntry);
    void LogCreateAttempt(LegacyTransportSpawnDiagnostic const& diagnostic);
    void LogMissingCreateAnimationData(uint32 dbGuid, uint32 dbEntry, uint32 clientEntry);
    void LogCreateFailure(LegacyTransportSpawnDiagnostic const& diagnostic);
    void LogCreateSuccess(LegacyTransportSpawnDiagnostic const& diagnostic, uint64 runtimeGuid, uint32 highGuid, uint32 lowGuid, uint32 timer, uint32 period, uint32 state, uint32 type, uint32 dynamicFlags);
    void LogNoRegisteredSpawns(uint32 mapId, uint32 spawnMode);
    void LogSpawnCount(uint32 count, uint32 mapId, uint32 spawnMode);
}

#endif // LegacyTransportSupport_h__
