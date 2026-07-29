/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LegacyTransportSupport.h"

#include <iostream>

namespace
{
    bool AddLegacyTransport(uint32 dbEntry, uint32 clientEntry, uint32 mapId, uint32 spawnMask, uint32 flags = 0)
    {
        LegacyTransport::LegacyTransportEntry entry = { dbEntry, clientEntry, mapId, spawnMask, flags };
        return LegacyTransport::AddLegacyTransportEntry(entry);
    }

    bool Expect(bool condition, char const* message)
    {
        if (condition)
            return true;

        std::cerr << message << '\n';
        return false;
    }

    bool TestLegacyTransportRegistryStartsEmpty()
    {
        bool passed = true;

        LegacyTransport::ClearLegacyTransportEntries();

        passed &= Expect(!LegacyTransport::IsLocalTransportDbEntry(176080),
            "Legacy transport registry should not allow Deeprun entries before data is loaded");
        passed &= Expect(LegacyTransport::GetClientEntryForDbEntry(176080) == 176080,
            "Legacy transport registry should not remap Deeprun entries before data is loaded");
        passed &= Expect(!LegacyTransport::ShouldPreservePassengerGameObjectVisibility(218203),
            "Legacy transport registry should not preserve Deeprun station visibility before data is loaded");

        return passed;
    }

    bool LoadKnownWorkingLegacyTransports()
    {
        bool passed = true;

        LegacyTransport::ClearLegacyTransportEntries();

        struct LegacyTransportSeedRow
        {
            uint32 DbEntry;
            uint32 ClientEntry;
            uint32 MapId;
            uint32 SpawnMask;
            uint32 Flags;
        };

        LegacyTransportSeedRow const rows[] =
        {
            { 176080, 218203, 369, 1, LegacyTransport::LEGACY_TRANSPORT_FLAG_PRESERVE_PASSENGER_GAMEOBJECT_VISIBILITY },
            { 176081, 218204, 369, 1, LegacyTransport::LEGACY_TRANSPORT_FLAG_PRESERVE_PASSENGER_GAMEOBJECT_VISIBILITY },
            { 176082, 218205, 369, 1, LegacyTransport::LEGACY_TRANSPORT_FLAG_PRESERVE_PASSENGER_GAMEOBJECT_VISIBILITY },
            { 176083, 218206, 369, 1, LegacyTransport::LEGACY_TRANSPORT_FLAG_PRESERVE_PASSENGER_GAMEOBJECT_VISIBILITY },
            { 176084, 218207, 369, 1, LegacyTransport::LEGACY_TRANSPORT_FLAG_PRESERVE_PASSENGER_GAMEOBJECT_VISIBILITY },
            { 176085, 218208, 369, 1, LegacyTransport::LEGACY_TRANSPORT_FLAG_PRESERVE_PASSENGER_GAMEOBJECT_VISIBILITY },
            { 4170, 4170, 1, 1, 0 },
            { 4171, 4171, 1, 1, 0 },
            { 11898, 11898, 1, 1, 0 },
            { 11898, 11898, 47, 2, 0 },
            { 11899, 11899, 1, 1, 0 },
            { 11899, 11899, 47, 2, 0 },
            { 20649, 20649, 0, 1, 0 },
            { 20650, 20650, 0, 1, 0 },
            { 20651, 20651, 0, 1, 0 },
            { 20652, 20652, 0, 1, 0 },
            { 20653, 20653, 0, 1, 0 },
            { 20654, 20654, 0, 1, 0 },
            { 20655, 20655, 0, 1, 0 },
            { 20656, 20656, 0, 1, 0 },
            { 20657, 20657, 0, 1, 0 },
            { 47296, 47296, 1, 1, 0 },
            { 47297, 47297, 1, 1, 0 },
            { 85556, 85556, 90, 2, 0 },
            { 149045, 149045, 0, 1, 0 },
            { 149045, 149045, 530, 1, 0 },
            { 149045, 149045, 571, 1, 0 },
            { 149046, 149046, 0, 1, 0 },
            { 149046, 149046, 530, 1, 0 },
            { 149046, 149046, 571, 1, 0 },
            { 152614, 152614, 0, 1, 0 },
            { 152614, 152614, 1, 1, 0 },
            { 152614, 152614, 530, 1, 0 },
            { 152614, 152614, 571, 1, 0 },
            { 183169, 183169, 530, 1, 0 },
            { 183177, 183177, 530, 1, 0 },
            { 183202, 183202, 530, 1, 0 },
            { 183203, 183203, 530, 1, 0 },
            { 183407, 183407, 548, 16, 0 },
            { 183490, 183490, 548, 16, 0 },
            { 183788, 183788, 554, 6, 0 },
            { 184330, 184330, 530, 1, 0 },
            { 186452, 186452, 571, 1, 0 },
            { 186452, 186452, 575, 6, 0 },
            { 186758, 186758, 571, 1, 0 },
            { 186759, 186759, 571, 1, 0 },
            { 186760, 186760, 571, 1, 0 },
            { 186761, 186761, 571, 1, 0 },
            { 186762, 186762, 571, 1, 0 },
            { 188136, 188136, 571, 1, 0 },
            { 188360, 188360, 571, 1, 0 },
            { 188360, 188360, 575, 6, 0 },
            { 188428, 188428, 571, 1, 0 },
            { 188514, 188514, 571, 1, 0 },
            { 188514, 188514, 578, 6, 0 },
            { 188515, 188515, 571, 1, 0 },
            { 188516, 188516, 571, 1, 0 },
            { 188517, 188517, 571, 1, 0 },
            { 188521, 188521, 571, 1, 0 },
            { 189991, 189991, 571, 1, 0 },
            { 190118, 190118, 571, 1, 0 },
            { 190118, 190118, 575, 6, 0 },
            { 190119, 190119, 571, 1, 0 },
            { 190119, 190119, 575, 6, 0 },
            { 190587, 190587, 571, 1, 0 },
            { 190587, 190587, 575, 6, 0 },
            { 191276, 191276, 571, 1, 0 },
            { 191277, 191277, 571, 1, 0 },
            { 191278, 191278, 571, 1, 0 },
            { 191329, 191329, 571, 1, 0 },
            { 192588, 192588, 571, 1, 0 },
            { 192717, 192717, 571, 1, 0 },
            { 194675, 194675, 603, 8, 0 },
            { 196837, 196837, 1, 1, 0 },
            { 196840, 196840, 631, 8, 0 },
            { 202220, 202220, 631, 8, 0 },
            { 202234, 202234, 631, 8, 0 },
            { 204243, 204243, 1, 1, 0 },
            { 204244, 204244, 1, 1, 0 },
            { 204245, 204245, 1, 1, 0 },
            { 204246, 204246, 1, 1, 0 },
            { 205079, 205079, 0, 1, 0 },
            { 205080, 205080, 0, 1, 0 },
            { 206608, 206608, 1, 1, 0 },
            { 206609, 206609, 1, 1, 0 },
            { 206610, 206610, 1, 1, 0 },
            { 207547, 207547, 644, 6, 0 },
            { 207889, 207889, 1, 1, 0 },
            { 212975, 212975, 870, 1, 0 },
            { 212976, 212976, 870, 1, 0 },
            { 219175, 219175, 1136, 16632, 0 },
            { 219176, 219176, 1136, 16632, 0 },
            { 219177, 219177, 1136, 16632, 0 },
            { 220364, 220364, 1136, 16632, 0 }
        };

        for (LegacyTransportSeedRow const& row : rows)
            passed &= AddLegacyTransport(row.DbEntry, row.ClientEntry, row.MapId, row.SpawnMask, row.Flags);

        return passed;
    }

    bool TestThunderBluffMesaElevatorAllowlist()
    {
        bool passed = true;

        uint32 const thunderBluffMesaElevators[] = { 4170, 4171, 11898, 11899, 47296, 47297 };
        for (uint32 dbEntry : thunderBluffMesaElevators)
        {
            passed &= Expect(LegacyTransport::IsLocalTransportDbEntry(dbEntry),
                "Thunder Bluff Mesa Elevator entries should be treated as local transports");
            passed &= Expect(LegacyTransport::GetClientEntryForDbEntry(dbEntry) == dbEntry,
                "Thunder Bluff Mesa Elevator entries should use their database entry as the client entry");
            passed &= Expect(LegacyTransport::IsAllowedOnMap(dbEntry, 1, 0),
                "Thunder Bluff Mesa Elevator entries should be allowed on Kalimdor normal spawn mode");
            passed &= Expect(LegacyTransport::GetAllowedSpawnMask(dbEntry, 1, 1) == 1,
                "Thunder Bluff Mesa Elevator entries should retain normal-map spawn mask bit");
            passed &= Expect(!LegacyTransport::IsAllowedOnMap(dbEntry, 369, 0),
                "Thunder Bluff Mesa Elevator entries should not be allowed on Deeprun");
            passed &= Expect(!LegacyTransport::IsAllowedOnMap(dbEntry, 1, 1),
                "Thunder Bluff Mesa Elevator entries should not be allowed on alternate Kalimdor spawn modes");
        }

        return passed;
    }

    bool TestUndercityElevatorAllowlist()
    {
        bool passed = true;

        uint32 const undercityElevators[] = { 20649, 20650, 20651, 20652, 20653, 20654, 20655, 20656, 20657 };
        for (uint32 dbEntry : undercityElevators)
        {
            passed &= Expect(LegacyTransport::IsLocalTransportDbEntry(dbEntry),
                "Undercity Elevator entries should be treated as local transports");
            passed &= Expect(LegacyTransport::GetClientEntryForDbEntry(dbEntry) == dbEntry,
                "Undercity Elevator entries should use their database entry as the client entry");
            passed &= Expect(LegacyTransport::IsAllowedOnMap(dbEntry, 0, 0),
                "Undercity Elevator entries should be allowed on Eastern Kingdoms normal spawn mode");
            passed &= Expect(LegacyTransport::GetAllowedSpawnMask(dbEntry, 0, 1) == 1,
                "Undercity Elevator entries should retain normal-map spawn mask bit");
            passed &= Expect(!LegacyTransport::IsAllowedOnMap(dbEntry, 1, 0),
                "Undercity Elevator entries should not be allowed on Kalimdor");
            passed &= Expect(!LegacyTransport::IsAllowedOnMap(dbEntry, 0, 1),
                "Undercity Elevator entries should not be allowed on alternate Eastern Kingdoms spawn modes");
        }

        return passed;
    }

    bool TestShattrathElevatorAllowlist()
    {
        bool passed = true;

        uint32 const shattrathElevators[] = { 183169, 183202, 183203 };
        for (uint32 dbEntry : shattrathElevators)
        {
            passed &= Expect(LegacyTransport::IsLocalTransportDbEntry(dbEntry),
                "Shattrath Elevator entries should be treated as local transports");
            passed &= Expect(LegacyTransport::GetClientEntryForDbEntry(dbEntry) == dbEntry,
                "Shattrath Elevator entries should use their database entry as the client entry");
            passed &= Expect(LegacyTransport::IsAllowedOnMap(dbEntry, 530, 0),
                "Shattrath Elevator entries should be allowed on Outland normal spawn mode");
            passed &= Expect(LegacyTransport::GetAllowedSpawnMask(dbEntry, 530, 1) == 1,
                "Shattrath Elevator entries should retain normal-map spawn mask bit");
            passed &= Expect(!LegacyTransport::IsAllowedOnMap(dbEntry, 0, 0),
                "Shattrath Elevator entries should not be allowed on Eastern Kingdoms");
            passed &= Expect(!LegacyTransport::IsAllowedOnMap(dbEntry, 530, 1),
                "Shattrath Elevator entries should not be allowed on alternate Outland spawn modes");
        }

        return passed;
    }

    bool TestExpandedElevatorTransportRows()
    {
        bool passed = true;

        struct ExpectedLegacyTransportRow
        {
            uint32 DbEntry;
            uint32 MapId;
            uint32 SpawnMask;
        };

        ExpectedLegacyTransportRow const expectedRows[] =
        {
            { 85556, 90, 2 },
            { 11898, 47, 2 },
            { 152614, 571, 1 },
            { 183177, 530, 1 },
            { 186452, 575, 6 },
            { 188514, 578, 6 },
            { 194675, 603, 8 },
            { 212975, 870, 1 },
            { 219175, 1136, 16632 }
        };

        for (ExpectedLegacyTransportRow const& row : expectedRows)
        {
            passed &= Expect(LegacyTransport::IsLocalTransportDbEntry(row.DbEntry),
                "Expanded elevator transport entries should be treated as local transports");
            passed &= Expect(LegacyTransport::GetClientEntryForDbEntry(row.DbEntry) == row.DbEntry,
                "Expanded elevator transport entries should use their database entry as the client entry");
            passed &= Expect(LegacyTransport::GetAllowedSpawnMask(row.DbEntry, row.MapId, row.SpawnMask) == row.SpawnMask,
                "Expanded elevator transport entries should retain their configured spawn mask bits");
        }

        passed &= Expect(LegacyTransport::IsAllowedOnMap(219175, 1136, 3),
            "Orgrimmar elevator entries should allow spawn mode bits from the phased spawn mask");
        passed &= Expect(!LegacyTransport::IsAllowedOnMap(219175, 1136, 2),
            "Orgrimmar elevator entries should not allow spawn mode bits outside the configured mask");

        return passed;
    }

    bool TestDeeprunCompatibilityRules()
    {
        bool passed = true;

        passed &= Expect(LegacyTransport::IsLocalTransportDbEntry(176080),
            "Deeprun database entries should remain local transports");
        passed &= Expect(LegacyTransport::GetClientEntryForDbEntry(176080) == 218203,
            "Deeprun database entries should still remap to client-facing entries");
        passed &= Expect(LegacyTransport::IsAllowedOnMap(176080, 369, 0),
            "Deeprun database entries should remain allowed on the Deeprun map");
        passed &= Expect(!LegacyTransport::IsAllowedOnMap(176080, 1, 0),
            "Deeprun database entries should not be allowed on Kalimdor");
        passed &= Expect(LegacyTransport::ShouldPreservePassengerGameObjectVisibility(218203),
            "Deeprun client entries should preserve passenger-side station gameobject visibility");
        passed &= Expect(!LegacyTransport::ShouldPreservePassengerGameObjectVisibility(4170),
            "Mesa Elevator entries should not use Deeprun passenger visibility preservation");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestLegacyTransportRegistryStartsEmpty();
    passed &= Expect(LoadKnownWorkingLegacyTransports(),
        "Known working legacy transport records should load into the registry");
    passed &= TestThunderBluffMesaElevatorAllowlist();
    passed &= TestUndercityElevatorAllowlist();
    passed &= TestShattrathElevatorAllowlist();
    passed &= TestExpandedElevatorTransportRows();
    passed &= TestDeeprunCompatibilityRules();

    return passed ? 0 : 1;
}
