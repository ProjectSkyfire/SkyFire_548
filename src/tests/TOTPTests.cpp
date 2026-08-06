/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Auth/TOTP.h"

#include <cstdlib>
#include <iostream>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }
}

int main()
{
    bool passed = true;

    std::string const secret = "GEZDGNBVGY3TQOJQGEZDGNBVGY3TQOJQ";
    std::optional<uint32> token = Skyfire::Auth::TOTP::GenerateToken(secret, 1, 8);
    passed &= Expect(token && *token == 94287082, "TOTP should match the RFC 6238 SHA1 vector.");

    passed &= Expect(!Skyfire::Auth::TOTP::GenerateToken("BAD@", 1, 6),
        "Invalid Base32 secrets should fail cleanly.");

    Skyfire::Auth::TOTP::ValidationResult accepted =
        Skyfire::Auth::TOTP::ValidateToken(secret, "287082", 1, 0, 1);
    passed &= Expect(accepted.Success, "A current 6-digit token should validate.");
    passed &= Expect(accepted.TimeStep == 1, "Accepted validation should report the matched time step.");

    Skyfire::Auth::TOTP::ValidationResult replayed =
        Skyfire::Auth::TOTP::ValidateToken(secret, "287082", 1, 1, 1);
    passed &= Expect(!replayed.Success, "The same or older time step should not be reusable.");

    Skyfire::Auth::TOTP::ValidationResult sameStepRetry =
        Skyfire::Auth::TOTP::ValidateToken(secret, "287082", 1, 1, 1, Skyfire::Auth::TOTP::DEFAULT_DIGITS, true);
    passed &= Expect(sameStepRetry.Success && sameStepRetry.TimeStep == 1,
        "Same-step retry should validate when replay of the last used time step is explicitly allowed.");

    Skyfire::Auth::TOTP::ValidationResult olderStepRetry =
        Skyfire::Auth::TOTP::ValidateToken(secret, "287082", 1, 2, 1, Skyfire::Auth::TOTP::DEFAULT_DIGITS, true);
    passed &= Expect(!olderStepRetry.Success,
        "Allowing same-step retry should still reject tokens older than the last used time step.");

    Skyfire::Auth::TOTP::ValidationResult loginToken =
        Skyfire::Auth::TOTP::ValidateLoginToken(secret, "287082", 1, 1);
    passed &= Expect(loginToken.Success && loginToken.TimeStep == 1,
        "Login token validation should accept a valid current token without replay-state checks.");

    return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
