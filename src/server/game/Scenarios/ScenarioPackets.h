/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef ScenarioPackets_h__
#define ScenarioPackets_h__

#include "Common.h"
#include "WorldPacket.h"

#include <ctime>
#include <vector>

// Scenario packets of the 5.4.8 (18414) client. Field order and guid bit
// order follow the Pandaria 5.4.8 core (alexkulya), the only 18414 source
// that sends them.
namespace Skyfire::ScenarioPackets
{
    // One objective of the current step. The client matches CriteriaId against
    // Criteria.dbc and shows Quantity against the criteria tree's amount.
    struct CriteriaProgress
    {
        uint32 CriteriaId = 0;
        uint64 Quantity = 0;
        ObjectGuid PlayerGuid = 0;
        time_t Date = 0;                // written as packed game time
        uint32 TimeFromStart = 0;       // seconds since the scenario started
        uint32 TimeFromCreate = 0;      // seconds since the scenario started
        uint8 Flags = 0;                // 4 bits; bit 0 removes the entry
    };

    struct ScenarioState
    {
        uint32 WaveMax = 0;             // challenge waves, unused for 5.0 scenarios
        uint32 TimerDuration = 0;       // challenge timer in seconds
        uint32 Medal = 0;               // challenge medal
        uint32 ScenarioId = 0;          // Scenario.dbc
        uint32 WaveCurrent = 0;
        uint32 BonusObjectiveData = 0;
        int32 CurrentStep = 0;          // ScenarioStep.dbc Step, -1 before the first one
        std::vector<CriteriaProgress> Progress;
    };

    WorldPacket BuildScenarioState(ScenarioState const& state);
    WorldPacket BuildScenarioProgressUpdate(CriteriaProgress const& progress);
}

#endif // ScenarioPackets_h__
