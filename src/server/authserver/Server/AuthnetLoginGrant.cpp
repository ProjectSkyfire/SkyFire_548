/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AuthnetLoginGrant.h"

#include <ctime>
#include <mutex>
#include <unordered_map>

namespace Skyfire::Authnet
{
namespace
{
    struct LoginGrant
    {
        std::string RemoteAddress;
        time_t ExpiresAt = 0;
    };

    std::mutex LoginGrantMutex;
    std::unordered_map<uint32, LoginGrant> LoginGrants;

    time_t GetNow()
    {
        return std::time(nullptr);
    }

    void CleanupExpiredLoginGrants(time_t now)
    {
        for (auto itr = LoginGrants.begin(); itr != LoginGrants.end();)
        {
            if (itr->second.ExpiresAt <= now)
                itr = LoginGrants.erase(itr);
            else
                ++itr;
        }
    }
}

bool IssueLoginGrant(uint32 accountId, std::string const& remoteAddress, uint32 ttlSeconds)
{
    if (!accountId || remoteAddress.empty() || !ttlSeconds)
        return false;

    std::lock_guard<std::mutex> lock(LoginGrantMutex);
    time_t const now = GetNow();
    CleanupExpiredLoginGrants(now);

    LoginGrants[accountId] = { remoteAddress, now + ttlSeconds };
    return true;
}

bool HasLoginGrant(uint32 accountId, std::string const& remoteAddress)
{
    if (!accountId || remoteAddress.empty())
        return false;

    std::lock_guard<std::mutex> lock(LoginGrantMutex);
    time_t const now = GetNow();
    CleanupExpiredLoginGrants(now);

    auto itr = LoginGrants.find(accountId);
    if (itr == LoginGrants.end())
        return false;

    if (itr->second.RemoteAddress != remoteAddress)
        return false;

    return true;
}

bool ConsumeLoginGrant(uint32 accountId, std::string const& remoteAddress)
{
    if (!HasLoginGrant(accountId, remoteAddress))
        return false;

    std::lock_guard<std::mutex> lock(LoginGrantMutex);
    auto itr = LoginGrants.find(accountId);
    if (itr == LoginGrants.end() || itr->second.RemoteAddress != remoteAddress)
        return false;

    LoginGrants.erase(itr);
    return true;
}
}
