/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "DraconicMending.h"
#include <iostream>
#include <limits>

int main()
{
    using namespace Skyfire::DraconicMending;
    Encounter success;
    Encounter failed;
    Encounter abandoned;
    Encounter timeout;
    Encounter returning;
    Encounter unattended;
    Encounter stalled;
    if (success.Update(1000, true, false, 5) != Result::Running ||
        success.Update(1000, true, true, 3) != Result::Recovered ||
        success.Update(1000, true, true, 5) != Result::Recovered ||
        failed.Update(1000, true, true, 0) != Result::Failed ||
        abandoned.Update(29999, false, false, 5) != Result::Running ||
        abandoned.Update(1, false, false, 5) != Result::Abandoned ||
        abandoned.Update(1, true, true, 5) != Result::Abandoned ||
        timeout.Update(239999, true, false, 5) != Result::Running ||
        timeout.Update(1, true, false, 5) != Result::TimedOut ||
        returning.Update(29000, false, false, 5) != Result::Running ||
        returning.Update(1000, true, false, 5) != Result::Running ||
        returning.Update(29000, false, false, 5) != Result::Running ||
        unattended.Update(1000, false, true, 5) != Result::Running ||
        stalled.Update(std::numeric_limits<std::uint32_t>::max(), true, true, 5) != Result::TimedOut)
    {
        std::cerr << "Draconic Mending encounter lifecycle failed\n";
        return 1;
    }
    return 0;
}
