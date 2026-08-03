/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "SpellChargePersistence.h"

#include <iostream>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    bool TestActiveRuntimeChargeBuildsPersistedState()
    {
        Skyfire::SpellCharges::PersistedState state;

        bool const persisted = Skyfire::SpellCharges::BuildPersistedState(1, 25000, 15000, 10000, 1000, state);

        bool passed = true;
        passed &= Expect(persisted, "Active runtime charges should be persisted");
        passed &= Expect(state.ConsumedCharges == 1, "Persisted consumed charge count should be preserved");
        passed &= Expect(state.ResetTime == 1015, "Persisted reset time should be absolute game time");
        passed &= Expect(state.BaseRegenTime == 15000, "Persisted base regen time should remain in milliseconds");
        return passed;
    }

    bool TestElapsedRuntimeChargeIsNotPersisted()
    {
        Skyfire::SpellCharges::PersistedState state;

        bool const persisted = Skyfire::SpellCharges::BuildPersistedState(1, 9000, 15000, 10000, 1000, state);

        return Expect(!persisted, "Fully restored runtime charges should not be persisted");
    }

    bool TestOfflineRechargeAdvancesLoadedState()
    {
        Skyfire::SpellCharges::RuntimeState state;

        bool const loaded = Skyfire::SpellCharges::BuildRuntimeState(2, 1005, 10000, 1011, 50000, state);

        bool passed = true;
        passed &= Expect(loaded, "Partially restored saved charges should load");
        passed &= Expect(state.ConsumedCharges == 1, "One elapsed recharge should reduce the consumed count");
        passed &= Expect(state.CurrentResetTime == 54000, "Next runtime reset should be relative to current getMSTime");
        passed &= Expect(state.BaseRegenTime == 10000, "Runtime regen time should remain in milliseconds");
        return passed;
    }

    bool TestFullyRestoredSavedChargeIsSkipped()
    {
        Skyfire::SpellCharges::RuntimeState state;

        bool const loaded = Skyfire::SpellCharges::BuildRuntimeState(2, 1005, 10000, 1025, 50000, state);

        return Expect(!loaded, "Fully restored saved charges should not load into runtime state");
    }

    bool TestRegenSecondsRoundsUp()
    {
        return Expect(Skyfire::SpellCharges::GetRegenSeconds(1501) == 2,
            "Millisecond regeneration should round up when stored as seconds");
    }
}

int main()
{
    bool passed = true;

    passed &= TestActiveRuntimeChargeBuildsPersistedState();
    passed &= TestElapsedRuntimeChargeIsNotPersisted();
    passed &= TestOfflineRechargeAdvancesLoadedState();
    passed &= TestFullyRestoredSavedChargeIsSkipped();
    passed &= TestRegenSecondsRoundsUp();

    return passed ? 0 : 1;
}
