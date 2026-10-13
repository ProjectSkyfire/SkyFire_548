/* This file is part of Project SkyFire. See LICENSE.md for Copyright information. */
#ifndef SKYFIRE_PVP_TEAM_STATE_H
#define SKYFIRE_PVP_TEAM_STATE_H
#include <cstddef>
#include <initializer_list>

namespace Skyfire
{
inline bool HasTeamVacancy(std::size_t war, std::size_t invites, std::size_t maximum, bool reserved)
{
    std::size_t used = war + invites;
    if (reserved && used) --used;
    return used < maximum;
}

// Move only this player's entries, including their existing invitation/kick
// deadlines. All callers validate capacity and group membership beforehand.
template<class Sets, class Timers, class Guid>
void TransferTeamRecords(Sets* zone, Sets* queue, Sets* war, Timers* invites, Timers* kicks,
    Guid guid, unsigned from, unsigned to)
{
    if (from == to || from > 1 || to > 1) return;
    for (Sets* sets : {zone, queue, war})
        if (sets[from].erase(guid)) sets[to].insert(guid);
    for (Timers* timers : {invites, kicks})
    {
        auto itr = timers[from].find(guid);
        if (itr != timers[from].end())
        {
            timers[to][guid] = itr->second;
            timers[from].erase(itr);
        }
    }
}
}
#endif
