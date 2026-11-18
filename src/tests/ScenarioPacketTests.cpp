/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

// Byte layout of the 5.4.8 (18414) scenario packets. The expected bytes come
// from an independent model of the Pandaria 5.4.8 core's writers
// (InstanceScript::SendScenarioState / SendScenarioProgressUpdate), with the
// packed date spliced in at its position because it depends on the local zone.

#include "ScenarioPackets.h"

#include <iostream>
#include <vector>

using namespace Skyfire::ScenarioPackets;

namespace
{
    time_t const TestDate = 1763100000; // 2025-11-14, any fixed instant works

    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << "FAIL: " << message << std::endl;

        return condition;
    }

    std::vector<uint8> PackedDate()
    {
        ByteBuffer buffer;
        buffer.AppendPackedTime(TestDate);
        return std::vector<uint8>(buffer.contents(), buffer.contents() + buffer.size());
    }

    std::vector<uint8> Join(std::vector<uint8> before, std::vector<uint8> const& after)
    {
        std::vector<uint8> date = PackedDate();
        before.insert(before.end(), date.begin(), date.end());
        before.insert(before.end(), after.begin(), after.end());
        return before;
    }

    bool ExpectBytes(WorldPacket const& packet, std::vector<uint8> const& expected, char const* message)
    {
        std::vector<uint8> actual(packet.contents(), packet.contents() + packet.size());
        if (actual == expected)
            return true;

        std::cerr << "FAIL: " << message << std::endl << "  expected:";
        for (uint8 b : expected)
            std::cerr << ' ' << std::hex << uint32(b);
        std::cerr << std::endl << "  actual:  ";
        for (uint8 b : actual)
            std::cerr << ' ' << std::hex << uint32(b);
        std::cerr << std::dec << std::endl;
        return false;
    }

    bool TestStateBeforeFirstStep()
    {
        ScenarioState state;
        state.ScenarioId = 58;
        state.CurrentStep = -1;

        WorldPacket packet = BuildScenarioState(state);

        bool passed = Expect(packet.GetOpcode() == SMSG_SCENARIO_STATE, "state packet should use SMSG_SCENARIO_STATE");
        passed &= ExpectBytes(packet, {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00 },
            "state without objectives: seven fields, step -1, zero count and two clear bits");
        return passed;
    }

    bool TestStateWithOneObjective()
    {
        ScenarioState state;
        state.ScenarioId = 58;
        state.CurrentStep = 2;

        CriteriaProgress progress;
        progress.CriteriaId = 19336;
        progress.Quantity = 1;
        progress.PlayerGuid = UI64LIT(0x1234);
        progress.Date = TestDate;
        progress.TimeFromStart = 65;
        progress.TimeFromCreate = 65;
        state.Progress.push_back(progress);

        WorldPacket packet = BuildScenarioState(state);

        return ExpectBytes(packet, Join({
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x52, 0x00, 0x00, 0x41, 0x00,
            0x00, 0x00, 0x41, 0x00, 0x00, 0x00 },
            { 0x13, 0x00, 0x35, 0x88, 0x4B, 0x00, 0x00 }),
            "state with one objective: count 1, quantity masked like a guid, criteria id last");
    }

    bool TestProgressUpdate()
    {
        CriteriaProgress progress;
        progress.CriteriaId = 20810;
        progress.Quantity = 0x0102;
        progress.PlayerGuid = UI64LIT(0x0700000000001234);
        progress.Date = TestDate;
        progress.TimeFromStart = 7;
        progress.TimeFromCreate = 7;
        progress.Flags = 1;

        WorldPacket packet = BuildScenarioProgressUpdate(progress);

        bool passed = Expect(packet.GetOpcode() == SMSG_SCENARIO_PROGRESS_UPDATE,
            "progress packet should use SMSG_SCENARIO_PROGRESS_UPDATE");
        passed &= ExpectBytes(packet, Join({
            0x10, 0xC2, 0x30, 0x06, 0x03, 0x13, 0x07, 0x00, 0x00, 0x00, 0x4A, 0x51,
            0x00, 0x00, 0x00 },
            { 0x07, 0x00, 0x00, 0x00, 0x35 }),
            "progress update: masks, flags, then interleaved quantity and player bytes");
        return passed;
    }
}

int main()
{
    bool passed = true;
    passed &= TestStateBeforeFirstStep();
    passed &= TestStateWithOneObjective();
    passed &= TestProgressUpdate();

    if (!passed)
        return 1;

    std::cout << "scenario packet tests passed" << std::endl;
    return 0;
}
