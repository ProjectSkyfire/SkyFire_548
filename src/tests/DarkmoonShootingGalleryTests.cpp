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

    bool TestShootingGalleryScoringRules()
    {
        bool passed = true;

        passed &= Expect(DarkmoonShootingGallery::IsTargetCreditEntry(NPC_SHOOTING_GALLERY_TARGET),
            "Shooting Gallery active targets should count toward quest credit");
        passed &= Expect(DarkmoonShootingGallery::CanAwardTargetCredit(NPC_SHOOTING_GALLERY_TARGET, true),
            "Shooting Gallery active target state should award credit");
        passed &= Expect(!DarkmoonShootingGallery::CanAwardTargetCredit(NPC_SHOOTING_GALLERY_TARGET, false),
            "Shooting Gallery inactive target state should be visual only");
        passed &= Expect(DarkmoonShootingGallery::GetHitCreditCount(0) == 2,
            "Shooting Gallery immediate hits should award quick-shot credit");
        passed &= Expect(DarkmoonShootingGallery::GetHitCreditCount(999) == 2,
            "Shooting Gallery hits inside the quick-shot window should award two credits");
        passed &= Expect(DarkmoonShootingGallery::GetHitCreditCount(1001) == 1,
            "Shooting Gallery hits after the quick-shot window should award one credit");
        passed &= Expect(DarkmoonShootingGallery::ClampTargetScore(24, 2) == 25,
            "Shooting Gallery score should clamp at the daily quest target count");
        passed &= Expect(SPELL_SHOOTING_GALLERY_TARGET_INDICATOR == 102341,
            "Shooting Gallery active targets should use the client-visible target indicator spell");
        passed &= Expect(DarkmoonShootingGallery::IsCompleteScore(25),
            "Shooting Gallery should finish as soon as the score reaches the target count");
        passed &= Expect(!DarkmoonShootingGallery::IsCompleteScore(24),
            "Shooting Gallery should continue below the target count");
        passed &= Expect(ACTION_SHOOTING_GALLERY_COMPLETE_GAME != ACTION_SHOOTING_GALLERY_END_GAME,
            "Shooting Gallery completed score should use the aura-removing end path");

        return passed;
    }

    bool TestShootingGalleryTargetPlacements()
    {
        bool passed = true;

        DarkmoonShootingGallery::TargetPosition const* targets = DarkmoonShootingGallery::GetTargetPositions();

        passed &= Expect(DarkmoonShootingGallery::TargetPositionCount == 3,
            "Shooting Gallery should use the three visible booth targets");
        passed &= Expect(NearlyEqual(targets[0].X, -4068.08f) && NearlyEqual(targets[0].Y, 6353.26f),
            "Shooting Gallery first target should match the right booth target");
        passed &= Expect(NearlyEqual(targets[1].X, -4069.91f) && NearlyEqual(targets[1].Y, 6355.00f),
            "Shooting Gallery second target should match the center booth target");
        passed &= Expect(NearlyEqual(targets[2].X, -4072.09f) && NearlyEqual(targets[2].Y, 6356.73f),
            "Shooting Gallery third target should match the left booth target");

        for (uint32 i = 0; i < DarkmoonShootingGallery::TargetPositionCount; ++i)
            passed &= Expect(DarkmoonShootingGallery::HasVisibleTargetHeight(targets[i]),
                "Shooting Gallery targets should spawn above the booth counter");

        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestShootingGalleryScoringRules();
    passed &= TestShootingGalleryTargetPlacements();

    return passed ? 0 : 1;
}
