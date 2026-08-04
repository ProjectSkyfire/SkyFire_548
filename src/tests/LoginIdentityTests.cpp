/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LoginIdentity.h"

#include <cstdlib>
#include <iostream>

namespace
{
bool Expect(bool condition, char const* message)
{
    if (condition)
        return true;

    std::cerr << message << '\n';
    return false;
}
}

int main()
{
    bool passed = true;

    Skyfire::Auth::LoginIdentity email =
        Skyfire::Auth::NormalizeLoginIdentity("ADMIN@PROJECTSKYFIRE.ORG");
    passed &= Expect(email.Valid, "Email login identity should be valid.");
    passed &= Expect(email.Kind == Skyfire::Auth::LoginIdentityKind::Email,
        "Email login identity should be classified as email.");
    passed &= Expect(email.Original == "ADMIN@PROJECTSKYFIRE.ORG",
        "Email login identity should preserve the SRP identity string.");
    passed &= Expect(email.Canonical == "admin@projectskyfire.org",
        "Email login identity should be lower-case canonical form.");

    Skyfire::Auth::LoginIdentity spacedEmail =
        Skyfire::Auth::NormalizeLoginIdentity("  Admin@ProjectSkyfire.Org  ");
    passed &= Expect(spacedEmail.Valid, "Trimmed email login identity should be valid.");
    passed &= Expect(spacedEmail.Canonical == "admin@projectskyfire.org",
        "Trimmed email login identity should canonicalize.");

    Skyfire::Auth::LoginIdentity legacy =
        Skyfire::Auth::NormalizeLoginIdentity("AlterEgo");
    passed &= Expect(legacy.Valid, "Legacy login identity should be valid.");
    passed &= Expect(legacy.Kind == Skyfire::Auth::LoginIdentityKind::LegacyUsername,
        "Legacy login identity should stay separate from email.");
    passed &= Expect(legacy.Canonical == "ALTEREGO",
        "Legacy login identity should match existing upper-case behavior.");

    Skyfire::Auth::LoginIdentity control =
        Skyfire::Auth::NormalizeLoginIdentity("bad\nname@example.org");
    passed &= Expect(!control.Valid, "Control characters should be rejected.");

    Skyfire::Auth::LoginIdentity malformed =
        Skyfire::Auth::NormalizeLoginIdentity("admin@");
    passed &= Expect(!malformed.Valid, "Malformed email identity should be rejected.");

    return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
