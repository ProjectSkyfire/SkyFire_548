/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "darkmoon_faire.h"

#include <cmath>
#include <iostream>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool NearlyEqual(float left, float right, float tolerance = 0.001f)
    {
        return std::fabs(left - right) < tolerance;
    }

    bool TestEventEndTeleportGate()
    {
        bool passed = true;

        passed &= Expect(DarkmoonFaire::IsDarkmoonFaireEventEnd(false, DarkmoonFaire::GameEventId),
            "Darkmoon Faire should identify event 75 stop as its shutdown signal");
        passed &= Expect(!DarkmoonFaire::IsDarkmoonFaireEventEnd(true, DarkmoonFaire::GameEventId),
            "Darkmoon Faire should not identify event 75 start as its shutdown signal");
        passed &= Expect(DarkmoonFaire::ShouldTeleportOffIsland(false, DarkmoonFaire::GameEventId, DarkmoonFaire::IslandMapId),
            "Darkmoon Faire should teleport island players when event 75 stops");
        passed &= Expect(!DarkmoonFaire::ShouldTeleportOffIsland(true, DarkmoonFaire::GameEventId, DarkmoonFaire::IslandMapId),
            "Darkmoon Faire should not teleport island players when event 75 starts");
        passed &= Expect(!DarkmoonFaire::ShouldTeleportOffIsland(false, DarkmoonFaire::GameEventId + 1, DarkmoonFaire::IslandMapId),
            "Darkmoon Faire should ignore other game events");
        passed &= Expect(!DarkmoonFaire::ShouldTeleportOffIsland(false, DarkmoonFaire::GameEventId, 0),
            "Darkmoon Faire should ignore players who are already outside Darkmoon Island");
        passed &= Expect(DarkmoonFaire::ShouldTeleportOffIslandWhenEventInactive(false, DarkmoonFaire::IslandMapId),
            "Darkmoon Faire should teleport island players if they log in after the event ended");
        passed &= Expect(!DarkmoonFaire::ShouldTeleportOffIslandWhenEventInactive(true, DarkmoonFaire::IslandMapId),
            "Darkmoon Faire should allow island players while the event is active");
        passed &= Expect(!DarkmoonFaire::ShouldTeleportOffIslandWhenEventInactive(false, 1),
            "Darkmoon Faire inactive guard should ignore players outside Darkmoon Island");

        return passed;
    }

    bool TestEventEndReturnPositions()
    {
        bool passed = true;

        DarkmoonFaire::EventReturnPosition const& alliance = DarkmoonFaire::GetEventEndReturnPosition(false);
        DarkmoonFaire::EventReturnPosition const& horde = DarkmoonFaire::GetEventEndReturnPosition(true);

        passed &= Expect(alliance.MapId == 0,
            "Alliance players should return to the Elwynn Faire camp");
        passed &= Expect(horde.MapId == 1,
            "Horde players should return to the Mulgore Faire camp");
        passed &= Expect(NearlyEqual(alliance.X, -9547.84f) && NearlyEqual(alliance.Y, 85.0556f) && NearlyEqual(alliance.Z, 60.1021f),
            "Alliance return should land on the Goldshire portal spawn height");
        passed &= Expect(alliance.Z >= 60.0f,
            "Alliance return should be above the Goldshire terrain height");
        passed &= Expect(NearlyEqual(horde.X, -1460.82f) && NearlyEqual(horde.Y, 170.258f) && NearlyEqual(horde.Z, -7.44726f),
            "Horde return should land at the Mulgore portal cluster");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestEventEndTeleportGate();
    passed &= TestEventEndReturnPositions();

    return passed ? 0 : 1;
}
