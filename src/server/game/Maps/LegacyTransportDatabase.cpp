/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "DatabaseEnv.h"
#include "LegacyTransportSupport.h"
#include "Log.h"
#include "Timer.h"

namespace LegacyTransport
{
    void LoadLegacyTransportEntries()
    {
        uint32 oldMSTime = getMSTime();

        ClearLegacyTransportEntries();

        QueryResult result = WorldDatabase.Query("SELECT `entry`, `transport_entry`, `map`, `spawnMask`, `flags` FROM `transport_legacy`");
        if (!result)
        {
            SF_LOG_INFO("server.loading", ">> Loaded 0 legacy local transports. DB table `transport_legacy` is empty.");
            return;
        }

        uint32 count = 0;
        do
        {
            Field* fields = result->Fetch();
            LegacyTransportEntry entry =
            {
                fields[0].GetUInt32(),
                fields[1].GetUInt32(),
                fields[2].GetUInt32(),
                fields[3].GetUInt32(),
                fields[4].GetUInt32()
            };

            if (!AddLegacyTransportEntry(entry))
            {
                SF_LOG_ERROR("sql.sql", "Table `transport_legacy` has invalid or duplicate legacy transport row for entry %u, transport_entry %u, map %u, spawnMask %u, flags %u. Skipped.",
                    entry.DbEntry, entry.ClientEntry, entry.MapId, entry.SpawnMask, entry.Flags);
                continue;
            }

            ++count;
        } while (result->NextRow());

        SF_LOG_INFO("server.loading", ">> Loaded %u legacy local transports in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }
}
