/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_AUTH_TOTP_H
#define SF_AUTH_TOTP_H

#include "Define.h"

#include <optional>
#include <string>

namespace Skyfire::Auth::TOTP
{
    constexpr uint32 DEFAULT_DIGITS = 6;
    constexpr uint32 DEFAULT_TIME_STEP = 30;
    constexpr uint32 DEFAULT_SECRET_BYTES = 20;

    struct ValidationResult
    {
        bool Success = false;
        uint64 TimeStep = 0;
    };

    std::string GenerateSecret();
    bool IsBase32SecretValid(std::string const& secret);

    std::optional<uint32> GenerateToken(std::string const& secret, uint64 timeStep, uint32 digits = DEFAULT_DIGITS);
    std::optional<uint32> GenerateCurrentToken(std::string const& secret, uint32 digits = DEFAULT_DIGITS);

    ValidationResult ValidateToken(
        std::string const& secret,
        std::string const& token,
        uint32 window,
        uint64 lastUsedStep = 0,
        uint64 currentTimeStep = UINT64_MAX,
        uint32 digits = DEFAULT_DIGITS,
        bool allowLastUsedStepReplay = false);

    ValidationResult ValidateLoginToken(
        std::string const& secret,
        std::string const& token,
        uint32 window,
        uint64 currentTimeStep = UINT64_MAX,
        uint32 digits = DEFAULT_DIGITS);
}

#endif
