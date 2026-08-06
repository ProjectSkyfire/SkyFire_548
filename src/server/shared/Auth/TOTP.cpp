/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Auth/TOTP.h"

#include "CryptoRandom.h"
#include "HMAC.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <ctime>
#include <vector>

namespace
{
    char constexpr BASE32_ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

    std::optional<std::vector<uint8>> DecodeBase32(std::string const& encoded)
    {
        std::vector<uint8> result;
        int buffer = 0;
        int bitsLeft = 0;
        bool seenPadding = false;

        for (char raw : encoded)
        {
            unsigned char ch = static_cast<unsigned char>(raw);
            if (std::isspace(ch) || ch == '-')
                continue;

            if (ch == '=')
            {
                seenPadding = true;
                continue;
            }

            if (seenPadding)
                return std::nullopt;

            if (ch == '0')
                ch = 'O';
            else if (ch == '1')
                ch = 'L';
            else if (ch == '8')
                ch = 'B';

            int value = -1;
            if (ch >= 'A' && ch <= 'Z')
                value = ch - 'A';
            else if (ch >= 'a' && ch <= 'z')
                value = ch - 'a';
            else if (ch >= '2' && ch <= '7')
                value = ch - '2' + 26;

            if (value < 0)
                return std::nullopt;

            buffer = (buffer << 5) | value;
            bitsLeft += 5;
            if (bitsLeft >= 8)
            {
                result.push_back(uint8((buffer >> (bitsLeft - 8)) & 0xFF));
                bitsLeft -= 8;
                buffer &= (1 << bitsLeft) - 1;
            }
        }

        if (result.empty())
            return std::nullopt;

        return result;
    }

    std::string EncodeBase32(uint8 const* data, size_t length)
    {
        std::string encoded;
        int buffer = 0;
        int bitsLeft = 0;

        for (size_t i = 0; i < length; ++i)
        {
            buffer = (buffer << 8) | data[i];
            bitsLeft += 8;

            while (bitsLeft >= 5)
            {
                encoded.push_back(BASE32_ALPHABET[(buffer >> (bitsLeft - 5)) & 0x1F]);
                bitsLeft -= 5;
            }

            buffer &= (1 << bitsLeft) - 1;
        }

        if (bitsLeft > 0)
            encoded.push_back(BASE32_ALPHABET[(buffer << (5 - bitsLeft)) & 0x1F]);

        return encoded;
    }

    uint32 Pow10(uint32 digits)
    {
        uint32 value = 1;
        for (uint32 i = 0; i < digits; ++i)
            value *= 10;

        return value;
    }

    uint64 CurrentTimeStep()
    {
        return uint64(std::time(nullptr)) / Skyfire::Auth::TOTP::DEFAULT_TIME_STEP;
    }

    std::optional<std::string> NormalizeToken(std::string const& token, uint32 digits)
    {
        if (token.length() != digits)
            return std::nullopt;

        if (!std::all_of(token.begin(), token.end(), [](char ch)
        {
            return std::isdigit(static_cast<unsigned char>(ch)) != 0;
        }))
            return std::nullopt;

        return token;
    }

    std::string FormatToken(uint32 token, uint32 digits)
    {
        std::string value = std::to_string(token);
        if (value.length() < digits)
            value.insert(value.begin(), digits - value.length(), '0');

        return value;
    }

    bool ConstantTimeEquals(std::string const& left, std::string const& right)
    {
        size_t const length = std::max(left.length(), right.length());
        unsigned char diff = static_cast<unsigned char>(left.length() ^ right.length());

        for (size_t i = 0; i < length; ++i)
        {
            unsigned char l = i < left.length() ? static_cast<unsigned char>(left[i]) : 0;
            unsigned char r = i < right.length() ? static_cast<unsigned char>(right[i]) : 0;
            diff |= l ^ r;
        }

        return diff == 0;
    }
}

namespace Skyfire::Auth::TOTP
{
    std::string GenerateSecret()
    {
        std::array<uint8, DEFAULT_SECRET_BYTES> secret = SkyFire::Crypto::GetRandomBytes<DEFAULT_SECRET_BYTES>();
        return EncodeBase32(secret.data(), secret.size());
    }

    bool IsBase32SecretValid(std::string const& secret)
    {
        return DecodeBase32(secret).has_value();
    }

    std::optional<uint32> GenerateToken(std::string const& secret, uint64 timeStep, uint32 digits)
    {
        if (digits < 6 || digits > 8)
            return std::nullopt;

        std::optional<std::vector<uint8>> key = DecodeBase32(secret);
        if (!key)
            return std::nullopt;

        std::array<uint8, 8> challenge = {};
        for (int i = 7; i >= 0; --i)
        {
            challenge[i] = uint8(timeStep & 0xFF);
            timeStep >>= 8;
        }

        SkyFire::Crypto::HMAC_SHA1::Digest hmac = SkyFire::Crypto::HMAC_SHA1::GetDigestOf(*key, challenge);
        uint32 offset = hmac[19] & 0x0F;
        uint32 binary =
            ((uint32(hmac[offset]) & 0x7F) << 24) |
            (uint32(hmac[offset + 1]) << 16) |
            (uint32(hmac[offset + 2]) << 8) |
            uint32(hmac[offset + 3]);

        return binary % Pow10(digits);
    }

    std::optional<uint32> GenerateCurrentToken(std::string const& secret, uint32 digits)
    {
        return GenerateToken(secret, CurrentTimeStep(), digits);
    }

    ValidationResult ValidateToken(
        std::string const& secret,
        std::string const& token,
        uint32 window,
        uint64 lastUsedStep,
        uint64 currentTimeStep,
        uint32 digits,
        bool allowLastUsedStepReplay)
    {
        std::optional<std::string> normalizedToken = NormalizeToken(token, digits);
        if (!normalizedToken)
            return {};

        if (currentTimeStep == UINT64_MAX)
            currentTimeStep = CurrentTimeStep();

        int64 const signedCurrentStep = int64(currentTimeStep);
        int64 const signedWindow = int64(window);
        for (int64 offset = -signedWindow; offset <= signedWindow; ++offset)
        {
            int64 candidateStep = signedCurrentStep + offset;
            if (candidateStep < 0)
                continue;

            uint64 step = uint64(candidateStep);
            if (lastUsedStep && (allowLastUsedStepReplay ? step < lastUsedStep : step <= lastUsedStep))
                continue;

            std::optional<uint32> generated = GenerateToken(secret, step, digits);
            if (!generated)
                return {};

            if (ConstantTimeEquals(FormatToken(*generated, digits), *normalizedToken))
                return { true, step };
        }

        return {};
    }

    ValidationResult ValidateLoginToken(
        std::string const& secret,
        std::string const& token,
        uint32 window,
        uint64 currentTimeStep,
        uint32 digits)
    {
        return ValidateToken(secret, token, window, 0, currentTimeStep, digits);
    }
}
