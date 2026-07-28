/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LegacyTransportSupport.h"

#include <iostream>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (condition)
            return true;

        std::cerr << message << '\n';
        return false;
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

    passed &= TestThunderBluffMesaElevatorAllowlist();
    passed &= TestDeeprunCompatibilityRules();

    return passed ? 0 : 1;
}
