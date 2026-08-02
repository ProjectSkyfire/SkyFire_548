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

    bool NearlyEqual(float left, float right)
    {
        return std::fabs(left - right) < 0.001f;
    }

    bool TestTonkChallengeScoringRules()
    {
        bool passed = true;

        passed &= Expect(DarkmoonTonkChallenge::IsTonkTarget(NPC_TONK_CANNON_TARGET),
            "Tonk Challenge cannon targets should count toward quest credit");
        passed &= Expect(!DarkmoonTonkChallenge::IsTonkTarget(NPC_DARKMOON_STEAM_TONK),
            "Tonk Challenge player vehicles should not count as targets");
        passed &= Expect(DarkmoonTonkChallenge::IsTonkHitSpell(SPELL_DARKMOON_TONK_SHOT),
            "Tonk Challenge should accept the Tonk vehicle shot spell");
        passed &= Expect(!DarkmoonTonkChallenge::IsTonkHitSpell(SPELL_DARKMOON_TONK_CONTROLLER),
            "Tonk Challenge should not treat the controller summon as a target hit");
        passed &= Expect(DarkmoonTonkChallenge::ClampTargetScore(29, 2) == TONK_CHALLENGE_TARGET_COUNT,
            "Tonk Challenge score should clamp at the daily quest target count");
        passed &= Expect(DarkmoonTonkChallenge::IsCompleteScore(TONK_CHALLENGE_TARGET_COUNT),
            "Tonk Challenge should end when all targets are destroyed");
        passed &= Expect(!DarkmoonTonkChallenge::IsCompleteScore(TONK_CHALLENGE_TARGET_COUNT - 1),
            "Tonk Challenge should continue below the target count");

        return passed;
    }

    bool TestTonkChallengeTargetPlacements()
    {
        bool passed = true;

        DarkmoonTonkChallenge::TargetPosition const& startPosition = DarkmoonTonkChallenge::GetTonkStartPosition();
        DarkmoonTonkChallenge::TargetPosition const* targets = DarkmoonTonkChallenge::GetTargetPositions();

        passed &= Expect(DarkmoonTonkChallenge::MapId == 974,
            "Tonk Challenge should run on Darkmoon Island");
        passed &= Expect(NearlyEqual(startPosition.X, -4129.767578f) && NearlyEqual(startPosition.Y, 6321.413086f) && NearlyEqual(startPosition.Z, 13.116933f),
            "Tonk Challenge player vehicle should spawn at the fenced start position");
        passed &= Expect(DarkmoonTonkChallenge::IsTargetInsideArena(startPosition),
            "Tonk Challenge player vehicle start should stay inside the booth arena bounds");

        passed &= Expect(DarkmoonTonkChallenge::TargetPositionCount == 18,
            "Tonk Challenge should use the full target set from the imported game script");
        passed &= Expect(NearlyEqual(targets[0].X, -4140.77f) && NearlyEqual(targets[0].Y, 6315.62f),
            "Tonk Challenge first target should match the northern target placement");
        passed &= Expect(NearlyEqual(targets[5].X, -4134.84f) && NearlyEqual(targets[5].Y, 6293.24f),
            "Tonk Challenge sixth target should match the southern target placement");
        passed &= Expect(NearlyEqual(targets[17].X, -4127.59f) && NearlyEqual(targets[17].Y, 6313.02f),
            "Tonk Challenge last target should match the eastern target placement");

        for (uint32 i = 0; i < DarkmoonTonkChallenge::TargetPositionCount; ++i)
            passed &= Expect(DarkmoonTonkChallenge::IsTargetInsideArena(targets[i]),
                "Tonk Challenge targets should stay inside the booth arena bounds");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestTonkChallengeScoringRules();
    passed &= TestTonkChallengeTargetPlacements();

    return passed ? 0 : 1;
}
