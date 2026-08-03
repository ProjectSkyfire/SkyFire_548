/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_SPELL_CHARGE_PERSISTENCE_H
#define SF_SPELL_CHARGE_PERSISTENCE_H

#include "Define.h"

namespace Skyfire
{
namespace SpellCharges
{
    struct PersistedState
    {
        PersistedState() : ConsumedCharges(0), ResetTime(0), BaseRegenTime(0) { }

        uint8 ConsumedCharges;
        uint32 ResetTime;
        uint32 BaseRegenTime;
    };

    struct RuntimeState
    {
        RuntimeState() : ConsumedCharges(0), CurrentResetTime(0), BaseRegenTime(0) { }

        uint8 ConsumedCharges;
        uint32 CurrentResetTime;
        uint32 BaseRegenTime;
    };

    inline uint32 GetRegenSeconds(uint32 baseRegenTime)
    {
        if (!baseRegenTime)
            return 0;

        return (baseRegenTime + 999) / 1000;
    }

    inline uint8 AdvanceRuntimeState(uint8 consumedCharges, uint32& resetTime, uint32 now, uint32 baseRegenTime)
    {
        if (!consumedCharges || !baseRegenTime)
            return consumedCharges;

        while (consumedCharges && (int32)(now - resetTime) >= 0)
        {
            --consumedCharges;
            resetTime += baseRegenTime;
        }

        return consumedCharges;
    }

    inline uint8 AdvancePersistedState(uint8 consumedCharges, uint32& resetTime, uint32 now, uint32 baseRegenTime)
    {
        uint32 const regenSeconds = GetRegenSeconds(baseRegenTime);
        if (!consumedCharges || !regenSeconds)
            return consumedCharges;

        while (consumedCharges && resetTime <= now)
        {
            --consumedCharges;
            resetTime += regenSeconds;
        }

        return consumedCharges;
    }

    inline bool BuildPersistedState(uint8 consumedCharges, uint32 currentResetTime, uint32 baseRegenTime, uint32 nowMs, uint32 now, PersistedState& state)
    {
        if (!consumedCharges || !baseRegenTime)
            return false;

        consumedCharges = AdvanceRuntimeState(consumedCharges, currentResetTime, nowMs, baseRegenTime);
        if (!consumedCharges)
            return false;

        uint64 const remainingMs = uint32(currentResetTime - nowMs);
        uint64 remainingSeconds = (remainingMs + 999) / 1000;
        if (remainingSeconds > uint64(0xFFFFFFFF) - now)
            remainingSeconds = uint64(0xFFFFFFFF) - now;

        state.ConsumedCharges = consumedCharges;
        state.ResetTime = now + uint32(remainingSeconds);
        state.BaseRegenTime = baseRegenTime;
        return true;
    }

    inline bool BuildRuntimeState(uint8 consumedCharges, uint32 resetTime, uint32 baseRegenTime, uint32 now, uint32 nowMs, RuntimeState& state)
    {
        if (!consumedCharges || !baseRegenTime)
            return false;

        consumedCharges = AdvancePersistedState(consumedCharges, resetTime, now, baseRegenTime);
        if (!consumedCharges)
            return false;

        uint64 remainingMs = uint64(resetTime - now) * 1000;
        if (remainingMs > 0x7FFFFFFF)
            remainingMs = 0x7FFFFFFF;

        state.ConsumedCharges = consumedCharges;
        state.CurrentResetTime = nowMs + uint32(remainingMs);
        state.BaseRegenTime = baseRegenTime;
        return true;
    }
}
}

#endif
