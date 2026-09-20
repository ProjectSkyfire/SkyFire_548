/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_HUB_WORLD_FALLBACK_POLICY_H
#define SKYFIRE_HUB_WORLD_FALLBACK_POLICY_H
#include <cstdint>
namespace Skyfire::Fallback
{
    struct ExitObservation
    {
        bool Confirmed = false;
        bool PreviouslyReady = false;
        bool OwnershipConfirmed = false;
        bool Expected = false;
        bool Maintenance = false;
        bool ShuttingDown = false;
        std::int64_t Code = 0;
    };
    inline bool MayAutomaticallyPromote(ExitObservation const& exit)
    {
        return exit.Confirmed && exit.PreviouslyReady && exit.OwnershipConfirmed &&
            !exit.Expected && !exit.Maintenance && !exit.ShuttingDown && exit.Code != 0 && exit.Code != 2;
    }

    // Only sustained fresh world-tick reports qualify. One request per overload episode.
    class DegradationWatch
    {
    public:
        bool Observe(std::uint64_t nowMs, bool eligible, std::uint32_t updateMs,
            std::uint32_t thresholdMs, std::uint32_t durationSeconds)
        {
            if (!eligible || !thresholdMs || updateMs < thresholdMs)
            { _tracking = _attempted = false; return false; }
            if (!_tracking || nowMs < _since)
            { _since = nowMs; _tracking = true; _attempted = false; }
            if (_attempted || nowMs - _since < std::uint64_t(durationSeconds) * 1000) return false;
            _attempted = true;
            return true;
        }
    private:
        bool _tracking = false, _attempted = false;
        std::uint64_t _since = 0;
    };
}
#endif
