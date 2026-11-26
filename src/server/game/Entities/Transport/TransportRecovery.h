/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_TRANSPORT_RECOVERY_H
#define SKYFIRE_TRANSPORT_RECOVERY_H

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace Skyfire
{
namespace TransportRecovery
{
    // Explicit shore recovery for patrol routes which never have a stop frame.
    constexpr std::uint32_t NoStopArrivalNode = 0xFFFFFFFFu;

    inline bool ValidOffset(float x, float y, float z, float o)
    {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(z) && std::isfinite(o) &&
            std::fabs(x) <= 250.0f && std::fabs(y) <= 250.0f && std::fabs(z) <= 250.0f;
    }

    // Search forward, including wraparound. Never substitute a later configured
    // stop when the actual next stop has no safe landing point.
    template<class Frames>
    typename Frames::value_type const* NextStop(Frames const& frames, std::size_t current, bool docked = false)
    {
        if (current >= frames.size())
            return nullptr;
        if (docked && frames[current].IsStopFrame())
            return &frames[current];
        for (std::size_t distance = 1; distance <= frames.size(); ++distance)
        {
            auto const& frame = frames[(current + distance) % frames.size()];
            if (frame.IsStopFrame())
                return &frame;
        }
        return nullptr;
    }
}
}
#endif
