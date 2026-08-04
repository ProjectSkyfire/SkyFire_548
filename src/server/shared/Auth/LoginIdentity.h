/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_LOGIN_IDENTITY_H
#define SF_LOGIN_IDENTITY_H

#include <string>
#include <string_view>

namespace Skyfire::Auth
{
    enum class LoginIdentityKind
    {
        Invalid,
        LegacyUsername,
        Email
    };

    struct LoginIdentity
    {
        LoginIdentityKind Kind = LoginIdentityKind::Invalid;
        std::string Original;
        std::string Canonical;
        bool Valid = false;
    };

    LoginIdentity NormalizeLoginIdentity(std::string_view input);
    char const* GetLoginIdentityKindName(LoginIdentityKind kind);
}

#endif
