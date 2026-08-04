/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "LoginIdentity.h"

#include <algorithm>
#include <cctype>

namespace Skyfire::Auth
{
namespace
{
    constexpr size_t MaxLegacyUsernameBytes = 16;
    constexpr size_t MaxEmailBytes = 64;

    std::string TrimAscii(std::string_view input)
    {
        size_t begin = 0;
        while (begin < input.size() && std::isspace(static_cast<unsigned char>(input[begin])))
            ++begin;

        size_t end = input.size();
        while (end > begin && std::isspace(static_cast<unsigned char>(input[end - 1])))
            --end;

        return std::string(input.substr(begin, end - begin));
    }

    bool HasControlCharacter(std::string const& value)
    {
        return std::any_of(value.begin(), value.end(), [](unsigned char c)
        {
            return c < 0x20 || c == 0x7F;
        });
    }

    std::string LowerAscii(std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c)
        {
            return char(std::tolower(c));
        });

        return value;
    }

    std::string UpperLegacyUsername(std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c)
        {
            return char(std::toupper(c));
        });

        return value;
    }

    bool IsConservativeEmailShape(std::string const& value)
    {
        size_t at = value.find('@');
        if (at == std::string::npos || at == 0 || at + 1 >= value.size())
            return false;

        if (value.find('@', at + 1) != std::string::npos)
            return false;

        std::string_view domain(value.data() + at + 1, value.size() - at - 1);
        size_t dot = domain.find('.');
        return dot != std::string_view::npos && dot != 0 && dot + 1 < domain.size();
    }
}

LoginIdentity NormalizeLoginIdentity(std::string_view input)
{
    LoginIdentity identity;
    identity.Original = TrimAscii(input);

    if (identity.Original.empty() || HasControlCharacter(identity.Original))
        return identity;

    if (identity.Original.find('@') != std::string::npos)
    {
        if (identity.Original.size() > MaxEmailBytes || !IsConservativeEmailShape(identity.Original))
            return identity;

        identity.Kind = LoginIdentityKind::Email;
        identity.Canonical = LowerAscii(identity.Original);
        identity.Valid = true;
        return identity;
    }

    if (identity.Original.size() > MaxLegacyUsernameBytes)
        return identity;

    identity.Canonical = UpperLegacyUsername(identity.Original);
    if (identity.Canonical.empty())
        return identity;

    identity.Kind = LoginIdentityKind::LegacyUsername;
    identity.Valid = true;
    return identity;
}

char const* GetLoginIdentityKindName(LoginIdentityKind kind)
{
    switch (kind)
    {
        case LoginIdentityKind::LegacyUsername:
            return "legacy_username";
        case LoginIdentityKind::Email:
            return "email";
        case LoginIdentityKind::Invalid:
            break;
    }

    return "invalid";
}
}
