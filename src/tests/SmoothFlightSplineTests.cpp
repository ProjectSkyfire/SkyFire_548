/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Common.h"
#include "DatabaseEnv.h"
#include "MoveSpline.h"
#include "WaypointManager.h"
#include <cmath>
#include <iostream>

// Process globals normally supplied by worldserver/Main.cpp. No DB is opened.
uint32 realmID = 0;
DatabaseWorkerPool<WorldDatabaseConnection> WorldDatabase;
DatabaseWorkerPool<CharacterDatabaseConnection> CharacterDatabase;
DatabaseWorkerPool<LoginDatabaseConnection> LoginDatabase;
std::map<uint32, std::string> realmNameStore;

int main()
{
    Movement::MoveSplineInitArgs args;
    args.flags.EnableFlying();
    args.flags.EnableCatmullRom();
    args.flags.cyclic = true;
    args.velocity = 8.0f;
    args.durationOverride = 12000;
    for (int i = 0; i < 16; ++i)
    {
        float angle = 2.0f * float(M_PI) * i / 16.0f;
        args.path.push_back(G3D::Vector3(50.0f * std::cos(angle), 30.0f * std::sin(angle), 150.0f));
    }
    Movement::MoveSpline leader;
    leader.Initialize(args);
    G3D::Vector3 offset(12.0f, -8.0f, 2.0f);
    for (auto& point : args.path)
        point += offset;
    Movement::MoveSpline follower;
    follower.Initialize(args);
    if (leader.Duration() != 12000 || follower.Duration() != 12000)
        return 1;
    for (int lap = 0; lap < 100; ++lap)
    {
        for (int tick = 0; tick < 120; ++tick)
        {
            auto previous = leader.ComputePosition();
            leader.updateState(100);
            follower.updateState(100);
            auto current = leader.ComputePosition();
            G3D::Vector3 delta = follower.ComputePosition() - current;
            if (leader.Finalized() || follower.Finalized() || (delta - offset).length() > 0.02f ||
                (current - previous).length() > 5.0f)
            {
                std::cerr << "Cyclic flight discontinuity or formation drift\n";
                return 1;
            }
        }
    }
    auto initial = leader.ComputePosition();
    leader.updateState(12000);
    if ((leader.ComputePosition() - initial).length() > 0.01f || leader.Finalized())
        return 1;

    WaypointData nodes[4] = {};
    WaypointPath path;
    for (int i = 0; i < 4; ++i)
    {
        nodes[i].x = float(i);
        nodes[i].SetMoveFlags(65);
        path.push_back(&nodes[i]);
    }
    if (!IsSmoothCyclicWaypointPath(path) || !nodes[0].run)
        return 1;
    nodes[2].delay = 100;
    if (IsSmoothCyclicWaypointPath(path))
        return 1;
    nodes[2].delay = 0;
    nodes[1].event_id = 42;
    if (IsSmoothCyclicWaypointPath(path))
        return 1;
    nodes[1].event_id = 0;
    nodes[0].SetMoveFlags(1);
    if (IsSmoothCyclicWaypointPath(path) || nodes[0].smoothCyclic || !nodes[0].run)
        return 1;
    nodes[0].SetMoveFlags(2);
    if (nodes[0].smoothCyclic || !nodes[0].run)
        return 1;
    nodes[0].SetMoveFlags(64);
    if (!nodes[0].smoothCyclic || nodes[0].run)
        return 1;
    std::cout << "Smooth cyclic seam, 100-lap formation timing, and explicit opt-in checks passed\n";
    return 0;
}
