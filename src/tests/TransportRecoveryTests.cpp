/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "TransportRecovery.h"
#include <iostream>
#include <limits>
#include <vector>

struct Frame
{
    bool stop;
    bool IsStopFrame() const { return stop; }
};

int main()
{
    using namespace Skyfire::TransportRecovery;
    std::vector<Frame> route = {{true}, {false}, {true}, {false}};
    std::vector<Frame> patrol = {{false}, {false}};
    std::vector<Frame> singleStop = {{false}, {true}, {false}};
    if (NextStop(route, 0) != &route[2] || NextStop(route, 1) != &route[2] ||
        NextStop(route, 2) != &route[0] || NextStop(route, 3) != &route[0] ||
        NextStop(route, 0, true) != &route[0] || NextStop(route, 2, true) != &route[2] ||
        NextStop(route, 1, true) != &route[2] ||
        NextStop(route, 4) || NextStop(std::vector<Frame>(), 0) ||
        NextStop(patrol, 0) || NextStop(patrol, 1, true) ||
        NextStop(singleStop, 0) != &singleStop[1] || NextStop(singleStop, 1) != &singleStop[1] ||
        NextStop(singleStop, 2) != &singleStop[1])
    {
        std::cerr << "Next arrival selection failed\n";
        return 1;
    }
    float nan = std::numeric_limits<float>::quiet_NaN();
    float inf = std::numeric_limits<float>::infinity();
    if (!ValidOffset(-250, 250, 0, 0) || ValidOffset(-251, 0, 0, 0) ||
        ValidOffset(0, 251, 0, 0) || ValidOffset(0, 0, -251, 0) ||
        ValidOffset(nan, 0, 0, 0) || ValidOffset(0, inf, 0, 0) ||
        ValidOffset(0, 0, nan, 0) || ValidOffset(0, 0, 0, inf))
    {
        std::cerr << "Passenger offset validation failed\n";
        return 1;
    }
    return 0;
}
