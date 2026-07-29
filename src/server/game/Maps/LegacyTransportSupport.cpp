/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LegacyTransportSupport.h"

#include <algorithm>
#include <vector>

namespace LegacyTransport
{
    namespace
    {
        uint32 const SupportedLegacyTransportFlags = LEGACY_TRANSPORT_FLAG_PRESERVE_PASSENGER_GAMEOBJECT_VISIBILITY;

        typedef std::vector<LegacyTransportEntry> LegacyTransportEntryStore;

        LegacyTransportEntryStore& GetLegacyTransportEntries()
        {
            static LegacyTransportEntryStore entries;
            return entries;
        }

        LegacyTransportEntryStore::const_iterator FindByDbEntry(uint32 dbEntry)
        {
            LegacyTransportEntryStore const& entries = GetLegacyTransportEntries();
            return std::find_if(entries.begin(), entries.end(), [dbEntry](LegacyTransportEntry const& entry)
            {
                return entry.DbEntry == dbEntry;
            });
        }

        LegacyTransportEntryStore::const_iterator FindByClientEntry(uint32 clientEntry)
        {
            LegacyTransportEntryStore const& entries = GetLegacyTransportEntries();
            return std::find_if(entries.begin(), entries.end(), [clientEntry](LegacyTransportEntry const& entry)
            {
                return entry.ClientEntry == clientEntry;
            });
        }

        bool HasFlag(LegacyTransportEntry const& entry, uint32 flag)
        {
            return (entry.Flags & flag) != 0;
        }
    }

    void ClearLegacyTransportEntries()
    {
        GetLegacyTransportEntries().clear();
    }

    bool AddLegacyTransportEntry(LegacyTransportEntry const& entry)
    {
        if (!entry.DbEntry || !entry.ClientEntry || !entry.SpawnMask || (entry.Flags & ~SupportedLegacyTransportFlags))
            return false;

        LegacyTransportEntryStore& entries = GetLegacyTransportEntries();
        for (LegacyTransportEntryStore::const_iterator itr = entries.begin(); itr != entries.end(); ++itr)
        {
            if (itr->DbEntry == entry.DbEntry && itr->ClientEntry != entry.ClientEntry)
                return false;

            if (itr->DbEntry == entry.DbEntry && itr->MapId == entry.MapId && itr->SpawnMask == entry.SpawnMask)
                return false;
        }

        entries.push_back(entry);
        return true;
    }

    bool IsDeeprunSubwayDbEntry(uint32 dbEntry)
    {
        LegacyTransportEntryStore::const_iterator itr = FindByDbEntry(dbEntry);
        return itr != GetLegacyTransportEntries().end() && HasFlag(*itr, LEGACY_TRANSPORT_FLAG_PRESERVE_PASSENGER_GAMEOBJECT_VISIBILITY);
    }

    bool IsDeeprunSubwayClientEntry(uint32 clientEntry)
    {
        LegacyTransportEntryStore::const_iterator itr = FindByClientEntry(clientEntry);
        return itr != GetLegacyTransportEntries().end() && HasFlag(*itr, LEGACY_TRANSPORT_FLAG_PRESERVE_PASSENGER_GAMEOBJECT_VISIBILITY);
    }

    bool IsLocalTransportDbEntry(uint32 dbEntry)
    {
        return FindByDbEntry(dbEntry) != GetLegacyTransportEntries().end();
    }

    uint32 GetClientEntryForDbEntry(uint32 dbEntry)
    {
        LegacyTransportEntryStore::const_iterator itr = FindByDbEntry(dbEntry);
        return itr != GetLegacyTransportEntries().end() ? itr->ClientEntry : dbEntry;
    }

    uint32 GetAllowedSpawnMask(uint32 dbEntry, uint32 mapId, uint32 spawnMask)
    {
        uint32 allowedSpawnMask = 0;
        LegacyTransportEntryStore const& entries = GetLegacyTransportEntries();
        for (LegacyTransportEntryStore::const_iterator itr = entries.begin(); itr != entries.end(); ++itr)
            if (itr->DbEntry == dbEntry && itr->MapId == mapId)
                allowedSpawnMask |= itr->SpawnMask & spawnMask;

        return allowedSpawnMask;
    }

    bool IsAllowedOnMap(uint32 dbEntry, uint32 mapId, uint32 spawnMode)
    {
        if (spawnMode >= 32)
            return false;

        return (GetAllowedSpawnMask(dbEntry, mapId, 1u << spawnMode) & (1u << spawnMode)) != 0;
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
