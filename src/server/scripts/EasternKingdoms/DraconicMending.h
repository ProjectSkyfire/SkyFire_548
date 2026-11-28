/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_DRACONIC_MENDING_H
#define SKYFIRE_DRACONIC_MENDING_H

#include <cstdint>

namespace Skyfire
{
namespace DraconicMending
{
    enum class Result { Running, Recovered, Failed, Abandoned, TimedOut };

    class Encounter
    {
    public:
        Result Update(std::uint32_t diff, bool hasPlayers, bool recovered, unsigned livingMenders)
        {
            if (_result != Result::Running)
                return _result;
            _elapsed += diff;
            _absent = hasPlayers ? 0 : _absent + diff;
            if (_absent >= 30000)
                _result = Result::Abandoned;
            else if (_elapsed >= 240000)
                _result = Result::TimedOut;
            else if (!livingMenders)
                _result = Result::Failed;
            else if (hasPlayers && recovered)
                _result = Result::Recovered;
            return _result;
        }

    private:
        std::uint64_t _elapsed = 0;
        std::uint64_t _absent = 0;
        Result _result = Result::Running;
    };
}
}

#endif
