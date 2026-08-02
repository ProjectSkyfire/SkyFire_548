/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "darkmoon_faire.h"

#include <cmath>
#include <cstring>
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

    bool TestCannonLandingScoringRules()
    {
        bool passed = true;

        passed &= Expect(DarkmoonCannon::GetLandingCredit(0.0f) == 3,
            "Darkmoon Cannon should award three points for a center landing");
        passed &= Expect(DarkmoonCannon::GetLandingCredit(DarkmoonCannon::BullseyeDistance) == 3,
            "Darkmoon Cannon should include the center edge in the three-point band");
        passed &= Expect(DarkmoonCannon::GetLandingCredit(DarkmoonCannon::BullseyeDistance + 0.001f) == 1,
            "Darkmoon Cannon should award one point just outside the center band");
        passed &= Expect(DarkmoonCannon::GetLandingCredit(DarkmoonCannon::NearDistance) == 1,
            "Darkmoon Cannon should include the target water edge in the one-point band");
        passed &= Expect(DarkmoonCannon::GetLandingCredit(DarkmoonCannon::NearDistance + 0.001f) == 0,
            "Darkmoon Cannon should not award quest credit outside the near band");
        passed &= Expect(DarkmoonCannon::ClampCannonScore(4, DarkmoonCannon::GetLandingCredit(DarkmoonCannon::NearDistance + 0.001f)) == 4,
            "Darkmoon Cannon miss landings should leave the score unchanged");
        passed &= Expect(DarkmoonCannon::ClampCannonScore(4, 3) == DARKMOON_CANNON_TARGET_POINTS,
            "Darkmoon Cannon score should clamp at the daily quest target count");
        passed &= Expect(DarkmoonCannon::IsCompleteScore(DARKMOON_CANNON_TARGET_POINTS),
            "Darkmoon Cannon should complete at the target point count");
        passed &= Expect(!DarkmoonCannon::IsCompleteScore(DARKMOON_CANNON_TARGET_POINTS - 1),
            "Darkmoon Cannon should remain incomplete below the target point count");

        return passed;
    }

    float DistanceFromCannonTarget(float x, float y)
    {
        DarkmoonCannon::CannonPosition const& target = DarkmoonCannon::GetTargetPosition();
        return std::hypot(x - target.X, y - target.Y);
    }

    bool TestCannonLandingGpsSamples()
    {
        bool passed = true;

        passed &= Expect(DarkmoonCannon::GetLandingCredit(DistanceFromCannonTarget(-4479.484863f, 6222.407227f)) == 3,
            "Darkmoon Cannon target center GPS should score three points");
        passed &= Expect(DarkmoonCannon::GetLandingCredit(DistanceFromCannonTarget(-4499.085938f, 6224.813965f)) == 1,
            "Darkmoon Cannon past-target GPS should score one point");
        passed &= Expect(DarkmoonCannon::GetLandingCredit(DistanceFromCannonTarget(-4480.541992f, 6206.239258f)) == 1,
            "Darkmoon Cannon left-target GPS should score one point");
        passed &= Expect(DarkmoonCannon::GetLandingCredit(DistanceFromCannonTarget(-4474.148926f, 6237.091797f)) == 1,
            "Darkmoon Cannon right-target GPS should score one point");
        passed &= Expect(DarkmoonCannon::GetLandingCredit(DistanceFromCannonTarget(-4454.482422f, 6232.762695f)) == 1,
            "Darkmoon Cannon ahead-target GPS should score one point");
        passed &= Expect(DarkmoonCannon::GetLandingCredit(DistanceFromCannonTarget(-4440.18f, 6211.33f)) == 0,
            "Darkmoon Cannon old scorer GPS should not score against the corrected target");

        return passed;
    }

    bool TestSharedGameTokenMessage()
    {
        bool passed = true;

        passed &= Expect(std::strcmp(DarkmoonFaire::GetMissingGameTokenText(), "You need a Darkmoon Game Token to play this game.") == 0,
            "Darkmoon Faire mini-games should share one clear missing-token message");

        return passed;
    }

    bool TestCannonPositions()
    {
        bool passed = true;

        DarkmoonCannon::CannonPosition const& launch = DarkmoonCannon::GetLaunchPosition();
        DarkmoonCannon::CannonPosition const& source = DarkmoonCannon::GetLaunchSourcePosition();
        DarkmoonCannon::CannonPosition const& target = DarkmoonCannon::GetTargetPosition();
        DarkmoonCannon::CannonPosition const& returnPosition = DarkmoonCannon::GetReturnPosition();

        passed &= Expect(DarkmoonCannon::MapId == 974,
            "Darkmoon Cannon should run on Darkmoon Island");
        passed &= Expect(NearlyEqual(launch.X, -4018.15f) && NearlyEqual(launch.Y, 6299.57f),
            "Darkmoon Cannon launch position should match the cannon mouth");
        passed &= Expect(NearlyEqual(target.X, -4479.484863f) && NearlyEqual(target.Y, 6222.407227f),
            "Darkmoon Cannon target position should match the target center");
        passed &= Expect(NearlyEqual(DarkmoonCannon::LaunchHorizontalSpeed, 55.0f)
            && NearlyEqual(DarkmoonCannon::LaunchVerticalSpeed, 48.2f),
            "Darkmoon Cannon launch speeds should match the cannon spell");
        passed &= Expect(std::hypot(launch.X - source.X, launch.Y - source.Y) > 0.1f,
            "Darkmoon Cannon launch source should not overlap the player position");
        float const sourceAngle = std::atan2(launch.Y - source.Y, launch.X - source.X);
        float const targetAngle = std::atan2(target.Y - launch.Y, target.X - launch.X);
        passed &= Expect(NearlyEqual(sourceAngle, targetAngle, 0.01f),
            "Darkmoon Cannon launch source should knock the player toward the target");
        passed &= Expect(NearlyEqual(returnPosition.X, -4019.0f) && NearlyEqual(returnPosition.Y, 6286.58f),
            "Darkmoon Cannon return position should place the player back near the booth");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestCannonLandingScoringRules();
    passed &= TestCannonLandingGpsSamples();
    passed &= TestSharedGameTokenMessage();
    passed &= TestCannonPositions();

    return passed ? 0 : 1;
}
