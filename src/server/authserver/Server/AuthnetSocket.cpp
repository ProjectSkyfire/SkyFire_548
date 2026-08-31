/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AuthnetSocket.h"
#include "Authentication/BsnBitStream.h"
#include "Authentication/PasswordSrp.h"
#include "Auth/LoginIdentity.h"
#include "AuthDefines.h"
#include "Common.h"
#include "CryptoHash.h"
#include "CryptoRandom.h"
#include "Database/DatabaseEnv.h"
#include "HMAC.h"
#include "Log.h"
#include "NetworkAddress.h"
#include "RealmList.h"
#include "Timer.h"
#include "Util.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>

#include <algorithm>
#include <array>
#include <atomic>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iterator>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <thread>
#include <vector>

namespace
{
    constexpr size_t MaxCapturedBytes = 8192;

    constexpr uint32 InitialLoginCommand = 9;
    constexpr size_t MaxInitialIdentityBytes = 0x140;
    constexpr size_t ClientModeSwitchRequestLen = 2;
    constexpr size_t FirstEncryptedFollowupLen = 57;
    constexpr size_t EncryptedResourceLookupRequestLen = 27;
    constexpr size_t ResourceLookupBurstCount = 19;
    constexpr size_t AuthnetSecretBytes = 64;
    constexpr uint32 AuthnetDefaultWorldSessionTtlSeconds = 300;
    // Soft2 Success without Soft0 → client opens a second TCP to
    // .logon.battle.net (launcher DNS → authnet). SoftK must survive that hop.
    constexpr uint32 SoftFinishPendingTtlSeconds = 120;

    struct SoftFinishPendingSession
    {
        std::string remoteAddress;
        uint32 accountId = 0;
        std::string accountName;
        std::string loginIdentity;
        std::string worldToken;
        std::string clientOs;
        std::string clientLocale;
        uint32 localeId = 0;
        std::string os;
        SkyFire::Authnet::PasswordSrp::SessionKey sessionKey{};
        std::array<uint8, AuthnetSecretBytes> authnetSecret{};
        bool authnetSecretReady = false;
        std::chrono::steady_clock::time_point expiresAt{};
    };

    std::mutex g_softFinishPendingMutex;
    std::vector<SoftFinishPendingSession> g_softFinishPending;

    void PurgeExpiredSoftFinishPendingLocked(std::chrono::steady_clock::time_point now)
    {
        g_softFinishPending.erase(
            std::remove_if(g_softFinishPending.begin(), g_softFinishPending.end(),
                [now](SoftFinishPendingSession const& entry)
                {
                    return entry.expiresAt <= now;
                }),
            g_softFinishPending.end());
    }

    bool HasPendingSoftFinishForRemote(std::string const& remote)
    {
        std::lock_guard<std::mutex> lock(g_softFinishPendingMutex);
        PurgeExpiredSoftFinishPendingLocked(std::chrono::steady_clock::now());
        return std::any_of(g_softFinishPending.begin(), g_softFinishPending.end(),
            [&remote](SoftFinishPendingSession const& entry)
            {
                return entry.remoteAddress == remote;
            });
    }

    char constexpr PatchHttpResponseBody[] =
        "<patch><record program=\"WoW\" component=\"enUS\">"
        "http://127.0.0.1:1119/manifest;unused;internal;18414"
        "</record></patch>";
    char constexpr ManifestHttpResponseBody[] =
        "<config><versioninfo><version product=\"WoW\"><servers>"
        "<server id=\"authnet\" url=\"http://127.0.0.1:1119/wow-pod/\" />"
        "</servers></version></versioninfo></config>";
    char constexpr EmptyHttpResponse[] =
        "HTTP/1.1 204 No Content\r\n"
        "Content-Length: 0\r\n"
        "Connection: close\r\n"
        "\r\n";
    char constexpr NotFoundHttpResponse[] =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Length: 0\r\n"
        "Connection: close\r\n"
        "\r\n";

    constexpr uint8 InitialClientToServerCryptState[256] =
    {
        0x08, 0x27, 0x53, 0x31, 0x84, 0x32, 0xA8, 0x50, 0x22, 0x95, 0x41, 0xA4, 0x64, 0xB7, 0xA5, 0x46,
        0x6F, 0x28, 0x80, 0xFC, 0x62, 0x16, 0x60, 0x1F, 0xED, 0x12, 0xA7, 0xD6, 0x35, 0xD8, 0x63, 0xF8,
        0xEA, 0xA0, 0x1A, 0xAE, 0xE2, 0xE0, 0x3D, 0x43, 0x2F, 0x20, 0xBE, 0xCA, 0xF6, 0x76, 0x7D, 0x98,
        0xF9, 0x2D, 0x38, 0xDA, 0xA9, 0x6A, 0x07, 0xC7, 0x9C, 0x9F, 0x6B, 0xA3, 0x0D, 0xE9, 0x9D, 0x2C,
        0x7E, 0x5F, 0x04, 0x77, 0x1D, 0xEB, 0x5E, 0xDE, 0x19, 0x8A, 0xC0, 0x52, 0x67, 0xE5, 0xBA, 0xD7,
        0x21, 0xE1, 0x56, 0x59, 0xC3, 0x61, 0x69, 0x71, 0x2E, 0xB3, 0x06, 0x1E, 0x85, 0xB1, 0x93, 0x3C,
        0x5A, 0xAD, 0x86, 0xFA, 0x55, 0xC4, 0xAC, 0x8D, 0xE6, 0x3B, 0x0C, 0x88, 0x0F, 0x74, 0x13, 0x45,
        0xDD, 0xD3, 0xBF, 0x25, 0xD1, 0x40, 0x97, 0xBB, 0x4B, 0x4A, 0x83, 0x34, 0xA6, 0x78, 0x7F, 0xD9,
        0x0E, 0xAA, 0x8F, 0xE7, 0x44, 0x18, 0x51, 0x05, 0x23, 0xF4, 0x81, 0x02, 0x3A, 0x00, 0x39, 0x96,
        0xF2, 0x57, 0x26, 0xC5, 0x15, 0x5C, 0xC9, 0x5B, 0x70, 0x48, 0x0B, 0xCB, 0x17, 0x30, 0x37, 0x75,
        0xE8, 0xBC, 0xF1, 0xE4, 0xCC, 0xC8, 0xCF, 0x9A, 0x47, 0x99, 0xB6, 0x66, 0xC1, 0x5D, 0x36, 0x7C,
        0x73, 0xAB, 0xCD, 0x92, 0x6D, 0x90, 0xB8, 0x65, 0xA1, 0x3E, 0x8C, 0x8E, 0xD2, 0xAF, 0x14, 0x24,
        0xB2, 0xF7, 0x54, 0xDF, 0xCE, 0x68, 0x01, 0xC6, 0x9E, 0x3F, 0x89, 0xFE, 0xDC, 0x0A, 0x03, 0xEF,
        0xDB, 0x33, 0x87, 0xE3, 0x91, 0x4E, 0x8B, 0x1C, 0xEC, 0x7A, 0xB9, 0x7B, 0xD0, 0x79, 0x49, 0xEE,
        0xF0, 0x09, 0x2A, 0xC2, 0xFF, 0x94, 0x4F, 0x4D, 0x1B, 0x42, 0x6E, 0x4C, 0xF5, 0x58, 0xFD, 0xA2,
        0x6C, 0x9B, 0x72, 0xB5, 0xD4, 0xF3, 0xD5, 0x82, 0x11, 0x2B, 0xB4, 0x29, 0xBD, 0xFB, 0x10, 0xB0
    };

    constexpr uint8 InitialServerToClientCryptState[256] =
    {
        0xCB, 0x33, 0xE3, 0x08, 0x1E, 0x3E, 0xA6, 0x1C, 0x97, 0x2A, 0x43, 0xC6, 0x12, 0x10, 0xB2, 0xB8,
        0xC9, 0x84, 0x83, 0xCE, 0xFC, 0x3D, 0x4B, 0x95, 0x49, 0xA2, 0x46, 0xD8, 0x37, 0xB0, 0x05, 0x48,
        0xF9, 0xDD, 0x88, 0x5F, 0x69, 0x2F, 0xCF, 0xE1, 0x6C, 0xA1, 0x29, 0x1B, 0x5D, 0x7B, 0x6D, 0x64,
        0xD6, 0x76, 0xA5, 0x7E, 0xF8, 0x57, 0xAC, 0x7D, 0xE0, 0x00, 0x9F, 0x2E, 0x17, 0xA4, 0x32, 0x56,
        0x80, 0xAF, 0x5E, 0x53, 0x52, 0x96, 0xF7, 0xB3, 0x30, 0xEF, 0x82, 0xDF, 0x42, 0x27, 0x73, 0xC1,
        0xF3, 0xA0, 0x31, 0x2B, 0xBC, 0x60, 0xFE, 0x67, 0x94, 0x13, 0x92, 0x81, 0x3F, 0x3C, 0x6A, 0x15,
        0x25, 0xB1, 0xE7, 0xC3, 0xA3, 0x9B, 0x59, 0xD9, 0x72, 0x14, 0x51, 0x50, 0x39, 0x71, 0xD5, 0xCA,
        0xC2, 0x41, 0xAE, 0x8A, 0x4A, 0x26, 0x91, 0x35, 0x78, 0x0E, 0xAB, 0x11, 0xBE, 0x89, 0x70, 0x06,
        0x20, 0xA9, 0x9D, 0xFB, 0xD3, 0x75, 0x8B, 0x02, 0x6B, 0x16, 0xAA, 0xB7, 0x66, 0x9C, 0x98, 0xDB,
        0xE6, 0xFD, 0x28, 0x5A, 0x65, 0xEA, 0x0D, 0x85, 0xF4, 0x18, 0x09, 0xC5, 0x07, 0xCC, 0x62, 0x2D,
        0xA7, 0xAD, 0x63, 0xC7, 0xB5, 0x90, 0xE5, 0x86, 0xEE, 0xED, 0x6F, 0x4C, 0x5B, 0xE8, 0xDA, 0x87,
        0x4D, 0x55, 0xD7, 0x04, 0xE9, 0xD4, 0x77, 0xF5, 0xF1, 0x1A, 0x54, 0x93, 0x4F, 0x9E, 0x8C, 0x58,
        0x36, 0xFF, 0x24, 0x7F, 0x34, 0x5C, 0x38, 0xBD, 0x47, 0x01, 0x2C, 0x3B, 0xDC, 0x68, 0xC4, 0x8E,
        0xF2, 0xCD, 0x99, 0xFA, 0x0F, 0xBF, 0x74, 0x7C, 0xB9, 0x40, 0xD0, 0xD1, 0xA8, 0x0A, 0x0C, 0x1D,
        0xEC, 0x03, 0xB4, 0xE2, 0xEB, 0x6E, 0xBA, 0x23, 0xC8, 0xD2, 0x8F, 0x8D, 0x9A, 0xBB, 0x21, 0x7A,
        0x45, 0xC0, 0x3A, 0xE4, 0xDE, 0x44, 0x0B, 0x4E, 0x19, 0x22, 0x1F, 0xB6, 0xF0, 0x61, 0xF6, 0x79
    };

    struct ProbePacketHeader
    {
        bool decoded = false;
        uint32 command = 0;
        uint32 modeSwitch = 0;
        uint32 mode = 0;
        size_t bitPosition = 0;
    };

    struct InitialComponent
    {
        std::string program;
        std::string platform;
        uint32 build = 0;
    };

    struct InitialRequestInfo
    {
        ProbePacketHeader header;
        std::string program;
        std::string platform;
        std::string locale;
        std::vector<InitialComponent> components;
        bool hasIdentity = false;
        std::string identity;
        uint32 identityLength = 0;
        uint64 tailValue = 0;
        size_t bitLength = 0;
        size_t packetLength = 0;
    };

    struct ServiceRequestRecord
    {
        std::string program;
        std::string component;
        uint32 minor = 0;
        uint32 major = 0;
        uint32 build = 0;
    };

    struct ServiceRequestInfo
    {
        ProbePacketHeader header;
        std::vector<ServiceRequestRecord> records;
        uint32 selector = 0;
        std::string label;
        bool hasNestedHeader = false;
        ProbePacketHeader nestedHeader;
        uint32 requestId = 0;
        size_t bitLength = 0;
        size_t packetLength = 0;
    };

    struct ResourceLookupInfo
    {
        ProbePacketHeader header;
        uint32 marker = 0;
        uint32 requestId = 0;
        uint32 parentRaw = 0;
        uint32 optionalFlag = 0;
        uint32 requestType = 0;
        uint32 localeValue = 0;
        std::string locale;
        uint32 variant = 0;
        uint32 payloadFirst = 0;
        uint32 payloadSecond = 0;
        uint32 payloadWord = 0;
        size_t remainingBits = 0;
        size_t bitLength = 0;
        size_t packetLength = 0;
    };

    enum class ServiceKeyMode
    {
        ComponentMajor,
        ComponentMinor,
        ProgramMajor,
        ProgramMinor
    };

    struct ServiceResultProbeInfo
    {
        bool hasRecord = false;
        uint32 recordIndex = 0;
        char const* recordMode = "last";
        char const* field80Mode = "build";
        char const* blobMode = "zero";
        ServiceKeyMode keyMode = ServiceKeyMode::ComponentMajor;
        std::string program;
        std::string component;
        uint32 minor = 0;
        uint32 major = 0;
        uint32 build = 0;
        uint32 field80 = 0;
        std::vector<uint8> serviceKey;
        std::vector<uint8> blob;
    };

    bool StringEquals(char const* left, char const* right)
    {
        return left && std::strcmp(left, right) == 0;
    }

    bool StringEnabled(char const* value)
    {
        return value && value[0] &&
            !StringEquals(value, "0") &&
            !StringEquals(value, "false") &&
            !StringEquals(value, "no") &&
            !StringEquals(value, "none") &&
            !StringEquals(value, "skip") &&
            !StringEquals(value, "off");
    }

    char const* GetEnvOrDefault(char const* name, char const* fallback)
    {
        char const* value = std::getenv(name);
        return value && value[0] ? value : fallback;
    }

    // Default ON (session0_empty): Soft0 after Soft2 empty Soft-finishes on
    // the proof socket. Needs launcher Soft-mode B537E0→1 (A2057DF4 never
    // Set by Password/Wow). AUTHNET_POST_PROOF_SOFT0=none restores hop-TCP.
    bool Soft0Disabled()
    {
        char const* soft0Mode = GetEnvOrDefault("AUTHNET_POST_PROOF_SOFT0", "session0_empty");
        return StringEquals(soft0Mode, "none") || StringEquals(soft0Mode, "off") ||
            StringEquals(soft0Mode, "0") || StringEquals(soft0Mode, "false");
    }

    bool HasEnvValue(char const* name)
    {
        char const* value = std::getenv(name);
        return value && value[0];
    }

    uint32 GetEnvUInt32(char const* name, uint32 fallback);

    std::string GetConfiguredStartupWorldAccount()
    {
        std::string authSessionIdentity = GetEnvOrDefault("AUTHNET_STARTUP_WORLD_ACCOUNT", "A");
        if (authSessionIdentity.size() > MaxInitialIdentityBytes)
            authSessionIdentity.resize(MaxInitialIdentityBytes);

        return authSessionIdentity;
    }

    // Persist A{id} so each account has a unique world_account_token.
    // Soft AuthSession still sends the literal "A"; world matches the digest
    // against live session keys. AUTHNET_STARTUP_WORLD_ACCOUNT overrides.
    std::string BuildWorldAccountToken(uint32 accountId)
    {
        if (HasEnvValue("AUTHNET_STARTUP_WORLD_ACCOUNT"))
            return GetConfiguredStartupWorldAccount();
        if (!accountId)
            return "A";
        return std::string("A") + std::to_string(accountId);
    }

    uint32 GetAuthnetWorldSessionTtlSeconds()
    {
        uint32 ttl = GetEnvUInt32("AUTHNET_WORLD_SESSION_TTL_SECONDS", AuthnetDefaultWorldSessionTtlSeconds);
        return ttl ? ttl : AuthnetDefaultWorldSessionTtlSeconds;
    }

    std::string GetAuthnetOSFromPlatform(std::string const& platform)
    {
        std::string normalized(platform);
        std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char c)
        {
            return char(std::tolower(c));
        });

        if (normalized.find("win") != std::string::npos || normalized.find("wn") != std::string::npos)
            return "Win";

        if (normalized.find("mac") != std::string::npos || normalized.find("osx") != std::string::npos)
            return "OSX";

        if (platform.size() >= 3)
            return platform.substr(0, 3);

        return platform.empty() ? "Win" : platform;
    }

    std::string MaskSessionKey(SessionKey const& key)
    {
        std::string hex = ByteArrayToHexStr(key);
        if (hex.size() <= 16)
            return hex;

        return hex.substr(0, 8) + "..." + hex.substr(hex.size() - 8);
    }

    bool TryResolveAuthnetLoginIdentity(std::string const& identity, uint32& accountId, std::string& accountName)
    {
        Skyfire::Auth::LoginIdentity loginIdentity = Skyfire::Auth::NormalizeLoginIdentity(identity);

        PreparedStatement* stmt = nullptr;
        if (loginIdentity.Kind == Skyfire::Auth::LoginIdentityKind::Email)
        {
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_SESSIONKEY_BY_LOGIN_IDENTITY);
            stmt->setString(0, loginIdentity.Canonical);
            stmt->setString(1, loginIdentity.Canonical);
            stmt->setString(2, loginIdentity.Canonical);
        }
        else
        {
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_SESSIONKEY);
            stmt->setString(0, identity);
        }

        PreparedQueryResult result = LoginDatabase.Query(stmt);
        if (!result)
            return false;

        Field* fields = result->Fetch();
        accountId = fields[1].GetUInt32();
        accountName = loginIdentity.Kind == Skyfire::Auth::LoginIdentityKind::Email ? fields[3].GetString() : identity;

        return accountId != 0;
    }

    Realm const* FindAuthnetRealm(std::string const& preferredName)
    {
        sRealmList->UpdateIfNeed();

        if (!preferredName.empty())
        {
            for (RealmList::RealmMap::const_iterator itr = sRealmList->begin(); itr != sRealmList->end(); ++itr)
                if (itr->second.name == preferredName || itr->first == preferredName)
                    return &itr->second;
        }

        for (RealmList::RealmMap::const_iterator itr = sRealmList->begin(); itr != sRealmList->end(); ++itr)
            if (!itr->first.empty())
                return &itr->second;

        if (sRealmList->begin() != sRealmList->end())
            return &sRealmList->begin()->second;

        return nullptr;
    }

    Realm const* FindAuthnetRealmById(uint32 realmId)
    {
        if (!realmId)
            return nullptr;

        sRealmList->UpdateIfNeed();
        for (RealmList::RealmMap::const_iterator itr = sRealmList->begin(); itr != sRealmList->end(); ++itr)
            if (!itr->first.empty() && itr->second.m_ID == realmId)
                return &itr->second;

        return nullptr;
    }

    // Soft mode2 cmd8 Join (~16 B): Soft header + body; last 32 bits are the
    // RealmHandle id (live :65342 ends 00000001/02/03 for Maps/Main/Playerbots).
    bool TryParseMode2Command8JoinRealmId(std::vector<uint8> const& plain, uint32& realmId)
    {
        realmId = 0;
        if (plain.size() < 4)
            return false;

        Skyfire::Authnet::BitReader reader(plain.data(), plain.size());
        uint32 command = 0;
        uint32 modeSwitch = 0;
        uint32 mode = 0;
        if (!reader.ReadBits(6, command) || command != 8 ||
            !reader.ReadBits(1, modeSwitch) || !modeSwitch ||
            !reader.ReadBits(4, mode) || mode != 2)
            return false;

        size_t const idBit = plain.size() * 8 - 32;
        if (reader.BitPosition() > idBit)
            return false;

        reader.SetBitPosition(idBit);
        if (!reader.ReadUInt32(realmId) || !realmId)
            return false;

        return true;
    }

    // B39180 → B37290 uses *(conn+0x2538) as HMAC seed. Client fills that from
    // the first uint32 after Soft mode2 cmd8 header (live :60983 Join
    // 4872… → seed 0x769EDD73 matched AuthSession receivedDigest).
    bool TryParseMode2Command8JoinConnectionSeed(std::vector<uint8> const& plain, uint32& connectionSeed)
    {
        connectionSeed = 0;
        if (plain.size() < 6)
            return false;

        Skyfire::Authnet::BitReader reader(plain.data(), plain.size());
        uint32 command = 0;
        uint32 modeSwitch = 0;
        uint32 mode = 0;
        if (!reader.ReadBits(6, command) || command != 8 ||
            !reader.ReadBits(1, modeSwitch) || !modeSwitch ||
            !reader.ReadBits(4, mode) || mode != 2)
            return false;

        return reader.ReadUInt32(connectionSeed);
    }

    uint32 GetConfiguredAuthnetDefaultRealmField()
    {
        if (HasEnvValue("AUTHNET_MODE2_COMMAND8_FIELD"))
            return GetEnvUInt32("AUTHNET_MODE2_COMMAND8_FIELD", 0);

        std::string const preferredName = GetEnvOrDefault("AUTHNET_MODE2_COMMAND2_NAME", "");
        if (!preferredName.empty())
        {
            if (Realm const* realm = FindAuthnetRealm(preferredName))
                return realm->m_ID;
        }

        if (Realm const* realm = FindAuthnetRealm(""))
            return realm->m_ID;

        return 0;
    }

    uint32 ResolveAuthnetSelectedRealmField(uint32 accountId)
    {
        uint32 const configuredRealmField = GetConfiguredAuthnetDefaultRealmField();
        if (HasEnvValue("AUTHNET_MODE2_COMMAND8_FIELD") || HasEnvValue("AUTHNET_MODE2_COMMAND2_NAME"))
            return configuredRealmField;

        if (!accountId)
            return configuredRealmField;

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_AUTHNET_SELECTED_REALM);
        stmt->setUInt32(0, accountId);
        if (PreparedQueryResult result = LoginDatabase.Query(stmt))
        {
            uint32 const selectedRealmField = result->Fetch()[0].GetUInt32();
            if (selectedRealmField && FindAuthnetRealmById(selectedRealmField))
                return selectedRealmField;

            SF_LOG_INFO("server.authserver",
                "authnet probe: account %u saved selected_realm_id=%u is not in the active realm list; using configured default realm_field=%u.",
                accountId, selectedRealmField, configuredRealmField);
        }

        return configuredRealmField;
    }

    uint32 ResolveJoinRealmField(uint32 joinRealmId)
    {
        if (HasEnvValue("AUTHNET_MODE2_COMMAND8_FIELD"))
            return GetEnvUInt32("AUTHNET_MODE2_COMMAND8_FIELD", joinRealmId);

        if (joinRealmId)
            return joinRealmId;

        return GetConfiguredAuthnetDefaultRealmField();
    }

    bool IsAuthnetRealmJoinable(Realm const* realm);
    Realm const* FindFirstReachableAuthnetRealm(uint32 skipRealmId = 0);

    bool AllowAuthnetOfflineJoin()
    {
        char const* value = GetEnvOrDefault("AUTHNET_MODE2_COMMAND8_ALLOW_OFFLINE_JOIN", "0");
        return StringEquals(value, "1") || StringEquals(value, "true") ||
            StringEquals(value, "yes") || StringEquals(value, "on");
    }

    std::string FindAuthnetRealmNameById(uint32 realmId)
    {
        if (Realm const* realm = FindAuthnetRealmById(realmId))
            return realm->name;

        return {};
    }

    // Prefer env; else authnet_srp_password.txt next to cwd / known Server-auth dir.
    // Avoids "set env in wrong shell / old authserver still on 11190".
    void TrimAuthnetSrpPassword(std::string& line)
    {
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n' || line.back() == ' ' || line.back() == '\t'))
            line.pop_back();
        size_t start = 0;
        while (start < line.size() && (line[start] == ' ' || line[start] == '\t'))
            ++start;
        if (start)
            line.erase(0, start);
    }

    std::string LoadAuthnetSrpPassword(char const*& source)
    {
        source = "none";
        if (char const* env = std::getenv("AUTHNET_SRP_PASSWORD"); env && env[0])
        {
            std::string line = env;
            TrimAuthnetSrpPassword(line);
            if (!line.empty())
            {
                source = "env";
                return line;
            }
        }

        static char const* kPaths[] = {
            "authnet_srp_password.txt",
            "C:\\SkyFire_Files\\Server-auth\\authnet_srp_password.txt",
            "C:\\SkyFire_548\\.agents\\authnet_srp_password.txt"
        };
        for (char const* path : kPaths)
        {
            std::ifstream in(path, std::ios::in);
            if (!in)
                continue;
            std::string line;
            if (!std::getline(in, line))
                continue;
            TrimAuthnetSrpPassword(line);
            if (line.empty() || line[0] == '#')
                continue;
            source = path;
            return line;
        }
        return {};
    }

    int HexValue(char c)
    {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;

        return -1;
    }

    bool TryParseHexBytes(char const* text, size_t expectedBytes, std::vector<uint8>& bytes)
    {
        bytes.clear();
        if (!text)
            return false;

        size_t expectedChars = expectedBytes * 2;
        if (std::strlen(text) != expectedChars)
            return false;

        bytes.reserve(expectedBytes);
        for (size_t i = 0; i < expectedChars; i += 2)
        {
            int high = HexValue(text[i]);
            int low = HexValue(text[i + 1]);
            if (high < 0 || low < 0)
            {
                bytes.clear();
                return false;
            }

            bytes.push_back(uint8((high << 4) | low));
        }

        return true;
    }

    uint32 GetEnvUInt32(char const* name, uint32 fallback)
    {
        char const* value = std::getenv(name);
        if (!value || !value[0])
            return fallback;

        char* end = nullptr;
        unsigned long parsed = std::strtoul(value, &end, 0);
        return end && *end == '\0' ? uint32(parsed) : fallback;
    }

    std::string FourCCFromUInt32(uint32 value)
    {
        std::string text;
        text.reserve(4);

        for (int shift = 24; shift >= 0; shift -= 8)
        {
            char c = char((value >> shift) & 0xFF);
            text.push_back(c >= 0x20 && c <= 0x7E ? c : '.');
        }

        return text;
    }

    char const* ServiceKeyModeName(ServiceKeyMode mode)
    {
        switch (mode)
        {
            case ServiceKeyMode::ComponentMinor:
                return "component-minor";
            case ServiceKeyMode::ProgramMajor:
                return "program-major";
            case ServiceKeyMode::ProgramMinor:
                return "program-minor";
            case ServiceKeyMode::ComponentMajor:
            default:
                return "component-major";
        }
    }

    ServiceKeyMode GetServiceKeyMode()
    {
        char const* mode = GetEnvOrDefault("AUTHNET_SERVICE_KEY_MODE", "component-major");
        if (StringEquals(mode, "component-minor"))
            return ServiceKeyMode::ComponentMinor;
        if (StringEquals(mode, "program-major"))
            return ServiceKeyMode::ProgramMajor;
        if (StringEquals(mode, "program-minor"))
            return ServiceKeyMode::ProgramMinor;

        return ServiceKeyMode::ComponentMajor;
    }

    bool RecordMatches(ServiceRequestRecord const& record, char const* mode)
    {
        if (StringEquals(mode, "auth"))
            return record.program == "Auth" || record.component == "Lgon";
        if (StringEquals(mode, "conn"))
            return record.program == "Conn" || record.component == "Nul";
        if (StringEquals(mode, "wowr"))
            return record.program == "WoWR" || record.component == "SbRs";
        if (StringEquals(mode, "news"))
            return record.program == "Cach" && record.component == "News";
        if (StringEquals(mode, "svrs"))
            return record.program == "Cach" && record.component == "SvRs";

        return false;
    }

    ServiceRequestRecord const* SelectServiceRecord(ServiceRequestInfo const& request, char const* mode, uint32& selectedIndex)
    {
        if (request.records.empty())
            return nullptr;

        if (StringEquals(mode, "first"))
        {
            selectedIndex = 0;
            return &request.records[selectedIndex];
        }

        if (StringEquals(mode, "selected") && request.selector != 0 && request.selector <= request.records.size())
        {
            selectedIndex = request.selector - 1;
            return &request.records[selectedIndex];
        }

        for (size_t i = request.records.size(); i != 0; --i)
        {
            if (RecordMatches(request.records[i - 1], mode))
            {
                selectedIndex = uint32(i - 1);
                return &request.records[i - 1];
            }
        }

        selectedIndex = uint32(request.records.size() - 1);
        return &request.records[selectedIndex];
    }

    void StoreUInt16LE(uint8* target, uint32 value)
    {
        target[0] = uint8(value & 0xFF);
        target[1] = uint8((value >> 8) & 0xFF);
    }

    void StoreUInt32LE(uint8* target, uint32 value)
    {
        target[0] = uint8(value & 0xFF);
        target[1] = uint8((value >> 8) & 0xFF);
        target[2] = uint8((value >> 16) & 0xFF);
        target[3] = uint8((value >> 24) & 0xFF);
    }

    // Wow-64 B39180 Soft Join Success → B37290. HMAC key is Soft2 K64, never zeros.
    // worldKey[0:20] = HMAC-SHA1(SoftK64, "WoW\0" || connectionSeed || realmField)
    // worldKey[20:40] = HMAC-SHA1(SoftK64, "WoW\0" || realmField || connectionSeed)
    SessionKey BuildAuthnetWorldSessionKey(std::array<uint8, AuthnetSecretBytes> const& secret, uint32 connectionSeed, uint32 realmField)
    {
        auto buildDigestInput = [](uint32 first, uint32 second)
        {
            std::array<uint8, 12> input = { { 'W', 'o', 'W', 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
            StoreUInt32LE(input.data() + 4, first);
            StoreUInt32LE(input.data() + 8, second);
            return input;
        };

        auto firstDigest = SkyFire::Crypto::HMAC_SHA1::GetDigestOf(secret, buildDigestInput(connectionSeed, realmField));
        auto secondDigest = SkyFire::Crypto::HMAC_SHA1::GetDigestOf(secret, buildDigestInput(realmField, connectionSeed));

        SessionKey key = {};
        std::copy(firstDigest.begin(), firstDigest.end(), key.begin());
        std::copy(secondDigest.begin(), secondDigest.end(), key.begin() + firstDigest.size());
        return key;
    }

    void CopyFourCC(uint8* target, std::string const& value)
    {
        for (size_t i = 0; i < 4; ++i)
            target[i] = i < value.size() ? uint8(value[i]) : 0;
    }

    uint32 SelectServiceField80(ServiceRequestInfo const& request, ServiceRequestRecord const& record, char const* mode)
    {
        if (StringEquals(mode, "zero"))
            return 0;
        if (StringEquals(mode, "request"))
            return request.requestId;
        if (StringEquals(mode, "selector"))
            return request.selector;

        return record.build;
    }

    std::vector<uint8> BuildServicePayloadProbe(ServiceRequestInfo const& request, ServiceResultProbeInfo* probeInfo,
        char const* recordModeOverride = nullptr)
    {
        std::vector<uint8> payload(22, 0);

        // Post-proof service requests list Auth/Prf then Conn/Nul. "last" picks
        // Conn/Nul; prefer Auth/Prf unless the env explicitly overrides.
        char const* recordMode = (recordModeOverride && recordModeOverride[0])
            ? recordModeOverride
            : GetEnvOrDefault("AUTHNET_SERVICE_RECORD_MODE", "auth");
        char const* field80Mode = GetEnvOrDefault("AUTHNET_SERVICE_FIELD80", "build");
        char const* blobMode = GetEnvOrDefault("AUTHNET_SERVICE_BLOB_MODE", "zero");
        ServiceKeyMode keyMode = GetServiceKeyMode();

        uint32 selectedIndex = 0;
        ServiceRequestRecord const* record = SelectServiceRecord(request, recordMode, selectedIndex);
        if (!record)
        {
            if (probeInfo)
            {
                probeInfo->recordMode = recordMode;
                probeInfo->field80Mode = field80Mode;
                probeInfo->blobMode = blobMode;
                probeInfo->keyMode = keyMode;
                probeInfo->serviceKey.assign(payload.begin(), payload.begin() + 6);
                probeInfo->blob.assign(payload.begin() + 6, payload.end());
            }

            return payload;
        }

        std::string const& fourCC = keyMode == ServiceKeyMode::ProgramMajor || keyMode == ServiceKeyMode::ProgramMinor ?
            record->program : record->component;
        uint32 version = keyMode == ServiceKeyMode::ComponentMinor || keyMode == ServiceKeyMode::ProgramMinor ?
            record->minor : record->major;

        CopyFourCC(payload.data(), fourCC);
        StoreUInt16LE(&payload[4], version);

        if (StringEquals(blobMode, "record"))
        {
            StoreUInt32LE(&payload[6], record->build);
            StoreUInt32LE(&payload[10], request.requestId);
            StoreUInt16LE(&payload[14], record->major);
            StoreUInt16LE(&payload[16], record->minor);
            CopyFourCC(&payload[18], record->program);
        }

        uint32 field80 = SelectServiceField80(request, *record, field80Mode);

        if (probeInfo)
        {
            probeInfo->hasRecord = true;
            probeInfo->recordIndex = selectedIndex;
            probeInfo->recordMode = recordMode;
            probeInfo->field80Mode = field80Mode;
            probeInfo->blobMode = blobMode;
            probeInfo->keyMode = keyMode;
            probeInfo->program = record->program;
            probeInfo->component = record->component;
            probeInfo->minor = record->minor;
            probeInfo->major = record->major;
            probeInfo->build = record->build;
            probeInfo->field80 = field80;
            probeInfo->serviceKey.assign(payload.begin(), payload.begin() + 6);
            probeInfo->blob.assign(payload.begin() + 6, payload.end());
        }

        return payload;
    }

    bool TryParseHexBytesFlexible(char const* text, std::vector<uint8>& bytes, size_t maxBytes)
    {
        bytes.clear();
        if (!text || !*text)
            return false;

        size_t len = std::strlen(text);
        if ((len % 2) != 0 || (len / 2) > maxBytes)
            return false;

        bytes.reserve(len / 2);
        for (size_t i = 0; i < len; i += 2)
        {
            int high = HexValue(text[i]);
            int low = HexValue(text[i + 1]);
            if (high < 0 || low < 0)
            {
                bytes.clear();
                return false;
            }

            bytes.push_back(uint8((high << 4) | low));
        }

        return !bytes.empty();
    }

    static char const* kProp205Paths[] = {
        "authnet_prop205.txt",
        "C:\\SkyFire_Files\\Server-auth\\authnet_prop205.txt",
        "C:\\SkyFire_548\\.agents\\authnet_prop205.txt"
    };

    bool LoadAuthnetProp205(std::vector<uint8>& bytes, char const*& source)
    {
        // Soft2 629D0: challenge.second must be HMAC-SHA512(accountName, prop205)
        // before the client Generates a random prop205. SkyFire Launcher Soft
        // mode overwrites that buffer with kAuthnetFixedProp205; authserver
        // uses the same bytes unless an env/file override is set.
        source = "none";
        bytes.clear();
        if (TryParseHexBytesFlexible(std::getenv("AUTHNET_CLIENT_PROP205_HEX"), bytes, 128) &&
            bytes.size() == 128)
        {
            source = "env";
            return true;
        }

        char const* fileReuse = std::getenv("AUTHNET_PROP205_FILE_REUSE");
        if (fileReuse && (StringEquals(fileReuse, "1") || StringEquals(fileReuse, "yes") ||
                StringEquals(fileReuse, "true") || StringEquals(fileReuse, "on")))
        {
            for (char const* path : kProp205Paths)
            {
                std::ifstream in(path, std::ios::in);
                if (!in)
                    continue;
                std::string line;
                if (!std::getline(in, line))
                    continue;
                TrimAuthnetSrpPassword(line);
                if (line.empty() || line[0] == '#')
                    continue;
                if (TryParseHexBytesFlexible(line.c_str(), bytes, 128) && bytes.size() == 128)
                {
                    source = path;
                    return true;
                }
            }
        }

        bytes.assign(SkyFire::Authnet::PasswordSrp::kAuthnetFixedProp205,
            SkyFire::Authnet::PasswordSrp::kAuthnetFixedProp205 + SkyFire::Authnet::PasswordSrp::PROP205_LEN);
        source = "launcher_fixed";
        return true;
    }

    void SaveAuthnetProp205(SkyFire::Authnet::PasswordSrp::Prop205 const& prop205)
    {
        // Saved for inspection / forced-client experiments — not auto-loaded.
        std::string hex = ByteArrayToHexStr(
            std::vector<uint8>(prop205.begin(), prop205.end()));
        for (char const* path : kProp205Paths)
        {
            std::ofstream out(path, std::ios::out | std::ios::trunc);
            if (!out)
                continue;
            out << hex << '\n';
            if (out)
            {
                SF_LOG_INFO("server.authserver",
                    "authnet saved Soft2 prop205 to %s (inspection only; not reused on next login)",
                    path);
                return;
            }
        }
        SF_LOG_ERROR("server.authserver",
            "authnet failed to save Soft2 prop205 to any authnet_prop205.txt path");
    }

    std::string GetStartupAccountName()
    {
        // Password.dll Handle receives this E14340 "name" blob as its first
        // module message (len + bytes at account+0x28). ASCII "WoW1" is opcode
        // 0x57 → ReportError(101). Opcode 0 challenge layout (AuthModule+0x19a0):
        //   1 + 32 userSeed + 32 salt + 128 B + 128 secondChallenge = 321 bytes.
        // AUTHNET_STARTUP_ACCOUNT_PAYLOAD_HEX overrides the string name when set.
        std::vector<uint8> payload;
        if (TryParseHexBytesFlexible(std::getenv("AUTHNET_STARTUP_ACCOUNT_PAYLOAD_HEX"), payload, 0x3FF))
            return std::string(reinterpret_cast<char const*>(payload.data()), payload.size());

        std::string accountName = GetEnvOrDefault("AUTHNET_STARTUP_ACCOUNT_NAME", "WoW1");
        if (accountName.size() > 0x3FC)
            accountName.resize(0x3FC);

        return accountName;
    }

    // Client cmd=9 identity is the SRP userSeed / 629D0 HMAC key. Env knobs only
    // fill in when the packet had no identity (probes). Do not default to one
    // account — that made every other email fail Soft2 M1/629D0.
    std::string ResolveAuthnetChallengeIdentity(std::string const& loginIdentity)
    {
        if (!loginIdentity.empty())
        {
            if (HasEnvValue("AUTHNET_STARTUP_CHALLENGE_IDENTITY"))
            {
                char const* envIdentity = GetEnvOrDefault("AUTHNET_STARTUP_CHALLENGE_IDENTITY", "");
                if (envIdentity && envIdentity[0] && loginIdentity != envIdentity)
                    SF_LOG_INFO("server.authserver",
                        "authnet SRP using client identity='%s' (ignoring AUTHNET_STARTUP_CHALLENGE_IDENTITY='%s')",
                        loginIdentity.c_str(), envIdentity);
            }
            return loginIdentity;
        }

        if (HasEnvValue("AUTHNET_STARTUP_CHALLENGE_IDENTITY"))
            return GetEnvOrDefault("AUTHNET_STARTUP_CHALLENGE_IDENTITY", "");

        return {};
    }

    std::string ResolveAuthnetSrpAccountName(std::string const& identity)
    {
        if (!identity.empty())
        {
            if (HasEnvValue("AUTHNET_SRP_ACCOUNT_NAME"))
            {
                char const* envName = GetEnvOrDefault("AUTHNET_SRP_ACCOUNT_NAME", "");
                if (envName && envName[0] && identity != envName)
                    SF_LOG_INFO("server.authserver",
                        "authnet SRP HMAC accountName='%s' (ignoring AUTHNET_SRP_ACCOUNT_NAME='%s')",
                        identity.c_str(), envName);
            }
            return identity;
        }

        if (HasEnvValue("AUTHNET_SRP_ACCOUNT_NAME"))
            return GetEnvOrDefault("AUTHNET_SRP_ACCOUNT_NAME", "");

        return {};
    }

    // Build Password.dll opcode-0 challenge.
    // With AUTHNET_SRP_PASSWORD / authnet_srp_password.txt: real 1024-bit SRP (g=2, module N).
    // challenge.second uses kAuthnetFixedProp205 (launcher Soft hook) so Soft2
    // op=3 629D0 can pass. AUTHNET_CLIENT_PROP205_HEX overrides for debug.
    std::string BuildPlaceholderModuleChallenge(std::string const& loginIdentity,
        std::optional<SkyFire::Authnet::PasswordSrp::Challenge>* outChallenge = nullptr)
    {
        std::string identity = ResolveAuthnetChallengeIdentity(loginIdentity);
        std::string accountName = ResolveAuthnetSrpAccountName(identity);
        if (identity.empty() || accountName.empty())
            SF_LOG_ERROR("server.authserver",
                "authnet SRP challenge missing identity (login='%s' hmac='%s'); client cmd=9 must include the login email",
                identity.c_str(), accountName.c_str());
        char const* passwordSource = "none";
        std::string password = LoadAuthnetSrpPassword(passwordSource);

        SkyFire::Authnet::PasswordSrp::Prop205 prop205{};
        SkyFire::Authnet::PasswordSrp::Prop205 const* prop205Ptr = nullptr;
        char const* prop205Source = "none";
        std::vector<uint8> prop205Bytes;
        if (LoadAuthnetProp205(prop205Bytes, prop205Source))
        {
            std::memcpy(prop205.data(), prop205Bytes.data(), 128);
            prop205Ptr = &prop205;
        }

        SkyFire::Authnet::PasswordSrp::Salt salt{};
        SkyFire::Authnet::PasswordSrp::Salt const* saltPtr = nullptr;
        std::vector<uint8> saltBytes;
        if (TryParseHexBytesFlexible(std::getenv("AUTHNET_STARTUP_CHALLENGE_SALT_HEX"), saltBytes, 32) &&
            saltBytes.size() == 32)
        {
            std::memcpy(salt.data(), saltBytes.data(), 32);
            saltPtr = &salt;
        }

        if (!password.empty())
        {
            auto challenge = SkyFire::Authnet::PasswordSrp::MakeChallenge(
                identity, password, accountName, prop205Ptr, saltPtr);
            if (challenge)
            {
                SF_LOG_INFO("server.authserver",
                    "authnet SRP challenge built (password_source=%s password_len=%zu prop205=%s second_629d0=%s identity=%s)",
                    passwordSource, password.size(),
                    prop205Ptr ? prop205Source : "no",
                    challenge->secondMatchesProp205 ? "yes" : "no",
                    identity.c_str());
                std::string blob = SkyFire::Authnet::PasswordSrp::BuildChallengeMessage(*challenge);
                if (outChallenge)
                    *outChallenge = std::move(*challenge);
                return blob;
            }

            SF_LOG_ERROR("server.authserver",
                "authnet SRP MakeChallenge failed (password_source=%s prop205=%s) — falling back to placeholder B",
                passwordSource, prop205Ptr ? prop205Source : "no");
        }
        else
        {
            SF_LOG_INFO("server.authserver",
                "authnet SRP password missing (set AUTHNET_SRP_PASSWORD or write authnet_srp_password.txt next to authserver.exe)");
        }

        std::string blob;
        blob.resize(321, '\0');
        blob[0] = 0; // CHALLENGE

        uint8 userSeed[32] = {};
        size_t copyLen = std::min(identity.size(), sizeof(userSeed));
        std::memcpy(userSeed, identity.data(), copyLen);
        std::memcpy(&blob[1], userSeed, 32);

        if (saltPtr)
            std::memcpy(&blob[33], saltPtr->data(), 32);
        else
        {
            static uint8 const kDefaultSalt[32] = {
                0x16, 0x89, 0x1D, 0xED, 0x2A, 0xDB, 0x6C, 0x40, 0xB8, 0x7F, 0x0D, 0x7C, 0x25, 0x6E, 0x70, 0x9F,
                0x53, 0xAF, 0x59, 0xE0, 0x7F, 0x65, 0x74, 0x0E, 0xF3, 0x7B, 0x59, 0xD1, 0x74, 0x5C, 0xEB, 0xEF
            };
            std::memcpy(&blob[33], kDefaultSalt, 32);
        }

        for (size_t i = 65; i < 321; ++i)
            blob[i] = uint8(0xA5 ^ (i & 0xFF));
        blob[65 + 127] |= 0x01;
        blob[65 + 128 + 127] |= 0x01;

        if (outChallenge)
            outChallenge->reset();
        return blob;
    }

    std::vector<uint8> BuildStartupAccountKey()
    {
        // Default: Ovahlord x64 Password.dll ClientSideCache key (auth FourCC + SHA256).
        static char const* kDefaultKeyHex =
            "61757468000000000A3AFEE2CADE3A0E8B458C4B4660104CAC7FC50E2CA9BEF0D708942E77F15C1D";
        std::vector<uint8> key(40, 0);
        std::vector<uint8> configuredKey;
        if (TryParseHexBytes(std::getenv("AUTHNET_STARTUP_ACCOUNT_KEY"), key.size(), configuredKey))
            return configuredKey;
        if (TryParseHexBytes(kDefaultKeyHex, key.size(), configuredKey))
            return configuredKey;

        return key;
    }

    bool ShouldSendStartupAccountProbe(char const* mode)
    {
        return StringEnabled(mode);
    }

    bool IsStartupChallengePayload(char const* payloadMode)
    {
        return StringEquals(payloadMode, "challenge") || StringEquals(payloadMode, "srp0") ||
            StringEquals(payloadMode, "1") || StringEquals(payloadMode, "true");
    }

    // Soft cmd=2 (B56A50 → E2A4F0 → B56780): AuthModule feed without setting
    // Auth+9610. Same E14340 records as Soft type 0's account list. Client
    // proof uplink is Soft cmd=2 via B4F1A0 when 9610==0.
    std::vector<uint8> BuildStartupModuleFeedProbe(std::string const& accountName, std::vector<uint8> const& accountKey)
    {
        Skyfire::Authnet::BitWriter writer;
        writer.WriteBits(0x02, 6); // Soft cmd = module feed
        writer.WriteBits(1, 1);
        writer.WriteBits(0x00, 4);

        writer.WriteBits(1, 3); // one E14340 record
        writer.WriteBytes(accountKey.data(), accountKey.size());
        writer.WriteBits(uint32(accountName.size()), 10);
        // E14340 AlignToByte before name even when length is 0 (B29BA0).
        writer.WriteBytes(accountName.data(), accountName.size());
        writer.AlignToByte();
        return writer.Data();
    }

    // Soft cmd=2 client→server (B4F1A0 → E25220 → E14470): Soft header +
    // 3-bit count + (10-bit length + bytes) per record. Not E14340.
    struct SoftModuleUplinkInfo
    {
        uint32 command = 0;
        uint32 flag = 0;
        uint32 channel = 0;
        uint32 count = 0;
        std::vector<std::vector<uint8>> blobs;
        size_t packetLength = 0;
    };

    bool TryDecodeSoft2Empty(std::vector<uint8> const& packet, size_t& packetLength)
    {
        // Soft cmd=2 with 3-bit account/module count=0 (live post-proof: 4200).
        using namespace Skyfire::Authnet;
        BitReader reader(packet.data(), packet.size());
        uint32 command = 0;
        uint32 flag = 0;
        uint32 channel = 0;
        uint32 count = 0;
        if (!reader.ReadBits(6, command) || !reader.ReadBits(1, flag) ||
            !reader.ReadBits(4, channel) || !reader.ReadBits(3, count))
            return false;
        if (command != 2 || count != 0)
            return false;
        packetLength = ByteLengthForBits(reader.BitPosition());
        return packetLength > 0 && packetLength <= packet.size();
    }

    bool TryDecodeSoftModuleUplink(std::vector<uint8> const& packet, SoftModuleUplinkInfo& info)
    {
        using namespace Skyfire::Authnet;
        BitReader reader(packet.data(), packet.size());
        info = SoftModuleUplinkInfo();

        if (!reader.ReadBits(6, info.command) || !reader.ReadBits(1, info.flag) ||
            !reader.ReadBits(4, info.channel))
            return false;
        if (info.command != 2)
            return false;

        if (!reader.ReadBits(3, info.count) || info.count == 0 || info.count > 4)
            return false;

        info.blobs.reserve(info.count);
        for (uint32 i = 0; i < info.count; ++i)
        {
            uint32 length = 0;
            if (!reader.ReadBits(10, length) || length > 0x3FF)
                return false;

            std::vector<uint8> blob(length, 0);
            if (length != 0 && !reader.ReadBytes(blob.data(), length))
                return false;
            info.blobs.push_back(std::move(blob));
        }

        info.packetLength = ByteLengthForBits(reader.BitPosition());
        return info.packetLength > 0 && info.packetLength <= packet.size();
    }

    std::string BuildPlaceholderModuleServerProof()
    {
        // Password.dll opcode 3 (sub_38961D10): after opcode byte,
        //   32 bytes (server proof) + 128 bytes (key material).
        // Requires Auth state property 2462361631 == 2 (set by Soft2 client
        // proof). Wrong crypto → ReportError(103); missing state → #101.
        std::vector<uint8> configured;
        if (TryParseHexBytesFlexible(std::getenv("AUTHNET_MODULE_PROOF_REPLY_HEX"), configured, 0x3FF) &&
            !configured.empty())
            return std::string(configured.begin(), configured.end());

        std::string blob(1 + 32 + 128, '\0');
        blob[0] = 3;
        return blob;
    }

    std::vector<uint8> BuildSoftSession0WithAccount(std::vector<uint8> const& accountKey, std::string const& accountName)
    {
        Skyfire::Authnet::BitWriter writer;
        writer.WriteBits(0x00, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x00, 4);
        writer.WriteBits(0, 1); // session
        writer.WriteBits(1, 3);
        writer.WriteBytes(accountKey.data(), accountKey.size());
        writer.WriteBits(uint32(accountName.size()), 10);
        // E14340 always AlignToByte before name bytes (B29BA0), even when
        // length is 0 — omitting that shifts timeout/E14100 and Soft decode_fail.
        writer.WriteBytes(accountName.data(), accountName.size());
        writer.WriteBits(30000, 32);
        writer.WriteBits(0, 1);
        writer.WriteBits(0, 8);
        writer.AlignToByte();
        writer.WriteBits(0, 8);
        writer.AlignToByte();
        writer.WriteBits(0, 32);
        writer.WriteBits(0, 8);
        writer.WriteBits(0, 64);
        writer.WriteBits(0, 8);
        writer.WriteBits(0, 5);
        uint8 const stub[] = { 'A' };
        writer.WriteBytes(stub, sizeof(stub));
        writer.WriteBits(0, 64);
        writer.WriteBits(0, 32);
        writer.WriteBits(0, 1);
        writer.AlignToByte();
        return writer.Data();
    }

    std::vector<uint8> BuildSoftSession0Empty()
    {
        // Soft cmd=0 choice=0 with account count=0. Avoids reloading
        // Password.dll after Soft cmd=2 proof uplink (handlers already
        // destroyed — ChallengeHandler+0x432 clear, Soft-finish-safe).
        // B56890 → B537E0; Soft-mode launcher forces B537E0→1.
        Skyfire::Authnet::BitWriter writer;
        writer.WriteBits(0x00, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x00, 4);
        writer.WriteBits(0, 1); // session
        writer.WriteBits(0, 3); // no E14340 accounts
        writer.WriteBits(30000, 32);
        writer.WriteBits(0, 1);
        writer.WriteBits(0, 8);
        writer.AlignToByte();
        writer.WriteBits(0, 8);
        writer.AlignToByte();
        writer.WriteBits(0, 32);
        writer.WriteBits(0, 8);
        writer.WriteBits(0, 64);
        writer.WriteBits(0, 8);
        writer.WriteBits(0, 5);
        uint8 const stub[] = { 'A' };
        writer.WriteBytes(stub, sizeof(stub));
        writer.WriteBits(0, 64);
        writer.WriteBits(0, 32);
        writer.WriteBits(0, 1);
        writer.AlignToByte();
        return writer.Data();
    }

    std::vector<uint8> BuildModuleProofReplyProbe(char const*& responseMode,
        std::optional<SkyFire::Authnet::PasswordSrp::Challenge> const* challenge,
        std::vector<uint8> const* uplinkBlob,
        SkyFire::Authnet::PasswordSrp::SessionKey* outSessionKey = nullptr)
    {
        // Soft2 op=3 is the Password success path (state must be 2 from Soft2 proof).
        // Default "proof3" tries real SRP verify when AUTHNET_SRP_PASSWORD was used
        // for the challenge; falls back to zero M2 placeholder otherwise.
        responseMode = GetEnvOrDefault("AUTHNET_MODULE_PROOF_RESPONSE", "proof3");
        if (StringEquals(responseMode, "none") || StringEquals(responseMode, "skip") ||
            StringEquals(responseMode, "off"))
            return {};

        std::vector<uint8> accountKey = BuildStartupAccountKey();
        if (accountKey.size() != 40)
            accountKey.assign(40, 0);

        if (StringEquals(responseMode, "session0_empty") || StringEquals(responseMode, "soft0_empty"))
        {
            responseMode = "session0_empty";
            return BuildSoftSession0Empty();
        }

        if (StringEquals(responseMode, "session0") || StringEquals(responseMode, "soft0"))
        {
            responseMode = "session0";
            return BuildSoftSession0WithAccount(accountKey, std::string());
        }

        if (StringEquals(responseMode, "session0_name") || StringEquals(responseMode, "soft0_name"))
        {
            responseMode = "session0_name";
            std::string accountName = GetStartupAccountName();
            if (accountName.size() > 0x3FF)
                accountName.resize(0x3FF);
            return BuildSoftSession0WithAccount(accountKey, accountName);
        }

        responseMode = "proof3";
        if (challenge && *challenge && uplinkBlob)
        {
            SkyFire::Authnet::PasswordSrp::ClientProof client;
            if (SkyFire::Authnet::PasswordSrp::ParseClientProof(*uplinkBlob, client))
            {
                SkyFire::Authnet::PasswordSrp::Proof m2{};
                SkyFire::Authnet::PasswordSrp::SessionKey sessionKey{};
                char const* failReason = nullptr;
                SkyFire::Authnet::PasswordSrp::Proof expectedM1{};
                bool secondMismatch = false;
                if (auto op3 = SkyFire::Authnet::PasswordSrp::VerifyAndBuildOp3(
                        **challenge, client, m2, sessionKey, &failReason, &expectedM1,
                        &secondMismatch))
                {
                    if (secondMismatch)
                    {
                        SF_LOG_ERROR("server.authserver",
                            "authnet Soft2 629d0_mismatch (client will #103): "
                            "challenge.second/proofS not derived from this session prop205 "
                            "(launcher Soft mode must force kAuthnetFixedProp205; "
                            "AUTHNET_CLIENT_PROP205_HEX overrides must match the client)");
                    }
                    else if (outSessionKey)
                        *outSessionKey = sessionKey;

                    responseMode = secondMismatch ? "proof3_srp_629d0_mismatch" : "proof3_srp";
                    return BuildStartupModuleFeedProbe(*op3, accountKey);
                }

                SF_LOG_ERROR("server.authserver",
                    "authnet Soft2 SRP verify failed (%s) client_M1=%s expected_M1=%s salt=%s B=%s I=%s",
                    failReason ? failReason : "parse_or_unknown",
                    ByteArrayToHexStr(std::vector<uint8>(client.M1.begin(), client.M1.end())).c_str(),
                    failReason && std::strcmp(failReason, "M1_mismatch") == 0
                        ? ByteArrayToHexStr(std::vector<uint8>(expectedM1.begin(), expectedM1.end())).c_str()
                        : "-",
                    ByteArrayToHexStr(std::vector<uint8>((*challenge)->salt.begin(), (*challenge)->salt.end())).c_str(),
                    ByteArrayToHexStr(std::vector<uint8>((*challenge)->B.begin(), (*challenge)->B.end())).c_str(),
                    (*challenge)->srpUsername.c_str());
            }
            else
            {
                SF_LOG_ERROR("server.authserver",
                    "authnet Soft2 SRP verify failed (uplink_parse) blob_len=%zu",
                    uplinkBlob->size());
            }
        }
        else
        {
            SF_LOG_ERROR("server.authserver",
                "authnet Soft2 SRP verify skipped (challenge=%s uplink=%s)",
                (challenge && *challenge) ? "yes" : "no",
                uplinkBlob ? "yes" : "no");
        }

        return BuildStartupModuleFeedProbe(BuildPlaceholderModuleServerProof(), accountKey);
    }

    std::vector<uint8> BuildStartupResponseProbe(std::string const& loginIdentity,
        std::string* startupAccountName = nullptr,
        std::vector<uint8>* startupAccountKey = nullptr,
        std::optional<SkyFire::Authnet::PasswordSrp::Challenge>* outChallenge = nullptr)
    {
        // Soft cmd=0 body (Creep Layer E2BAF0 / B56F70 → B56CE0):
        //   Soft: cmd(6)=0, flag(1)=1, channel(4)=0
        //   choice(1): 0 = session (E2AE20 / B56CE0), 1 = result (E2A3C0)
        //
        // choice=0 ALWAYS sets Auth+9610 (finish). Module proof that stores
        // an outbound blob (1074=1) then asserts in B53E40. Password.dll
        // challenge therefore belongs on Soft cmd=2 (see
        // BuildStartupModuleFeedProbe), not embedded here.
        //
        // Soft cmd=2 body (E2A4F0): 3-bit count + E14340 records only — no
        // finish flag. Controlled by AUTHNET_STARTUP_SOFT_TYPE=2|module|auto.
        Skyfire::Authnet::BitWriter writer;
        char const* accountMode = GetEnvOrDefault("AUTHNET_STARTUP_ACCOUNT_RESPONSE", "1");
        bool const includeAccount = ShouldSendStartupAccountProbe(accountMode);
        char const* payloadMode = GetEnvOrDefault("AUTHNET_STARTUP_ACCOUNT_PAYLOAD", "challenge");
        std::string accountName;
        if (includeAccount)
        {
            if (IsStartupChallengePayload(payloadMode))
                accountName = BuildPlaceholderModuleChallenge(loginIdentity, outChallenge);
            else
                accountName = GetStartupAccountName();
        }
        std::vector<uint8> accountKey = includeAccount ? BuildStartupAccountKey() : std::vector<uint8>();

        if (startupAccountName)
            *startupAccountName = accountName;
        if (startupAccountKey)
            *startupAccountKey = accountKey;

        if (includeAccount && accountKey.size() != 40)
            accountKey.assign(40, 0);
        if (includeAccount && accountName.size() > 0x3FF)
            accountName.resize(0x3FF);

        char const* softTypeMode = GetEnvOrDefault("AUTHNET_STARTUP_SOFT_TYPE", "auto");
        bool useModuleFeed = StringEquals(softTypeMode, "2") || StringEquals(softTypeMode, "module") ||
            (StringEquals(softTypeMode, "auto") && includeAccount && IsStartupChallengePayload(payloadMode));
        if (useModuleFeed && includeAccount)
            return BuildStartupModuleFeedProbe(accountName, accountKey);

        writer.WriteBits(0x00, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x00, 4);

        writer.WriteBits(0, 1); // session variant

        // E2AAB0: account records
        writer.WriteBits(includeAccount ? 1 : 0, 3);
        if (includeAccount)
        {
            writer.WriteBytes(accountKey.data(), accountKey.size());
            writer.WriteBits(uint32(accountName.size()), 10);
            // E14340 AlignToByte before name even when length is 0 (B29BA0).
            writer.WriteBytes(accountName.data(), accountName.size());
        }

        // Optional<uint32> timeout (wire raw; client stores value|0x80000000)
        writer.WriteBits(30000, 32);
        writer.WriteBits(0, 1); // no E25E00 blob

        // E14100: two length-prefixed strings. B29E60 AlignToByte's before
        // the payload even when length is 0 — omit that and the rest of the
        // session body shifts and decode_fail (handler return 1).
        writer.WriteBits(0, 8);
        writer.AlignToByte();
        writer.WriteBits(0, 8);
        writer.AlignToByte();

        // Remainder of E2AE20
        writer.WriteBits(0, 32);
        writer.WriteBits(0, 8);
        writer.WriteBits(0, 64);
        writer.WriteBits(0, 8);
        writer.WriteBits(0, 5); // len N → read N+1 bytes
        uint8 const stub[] = { 'A' };
        writer.WriteBytes(stub, sizeof(stub));
        writer.WriteBits(0, 64);
        writer.WriteBits(0, 32);

        writer.WriteBits(0, 1); // no trailing list
        writer.AlignToByte();

        return writer.Data();
    }

    // Mode2 cmd0 S→C (B393D0 → E24300), answering empty C→S 4002:
    //   choice 1 → E23980 uint8 status (0 / 3..26 / 255 ok)
    //   choice 0 → E23BA0 7-bit count + entries (E235A0 handle + u16)
    // B398F0 registers cmd0/2/3/6/7/8 only — cmd1 has no handler.
    // Prior Larva-2315 name packing (401A…/411A…WoW1) was wrong and bounced.
    uint32 GetCachedGameAccountSoftCommand()
    {
        return GetEnvUInt32("AUTHNET_CGA_SOFT_COMMAND", 0) & 0x3F;
    }

    void WriteCgaSoftHeader(Skyfire::Authnet::BitWriter& writer)
    {
        writer.WriteBits(GetCachedGameAccountSoftCommand(), 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x02, 4);
    }

    std::string GetCachedGameAccountName()
    {
        char const* value = std::getenv("AUTHNET_GAME_ACCOUNT_NAME");
        if (value && *value)
            return std::string(value);
        return "WoW1";
    }

    void WriteGameAccountHandle(Skyfire::Authnet::BitWriter& writer)
    {
        // E235A0 + E23A90 (12-byte entry): u8, 12-bit pad, u8, u32, u16.
        // Same handle trailing shape as mode2 cmd2 detail probe.
        // byte0 default 1 = US-like region. List alone (:64118) stores accounts
        // at BattlenetLogin state 2; Soft Soft mode2 cmd3 follows to advance.
        uint32 const byte0 = GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_BYTE0",
            GetEnvUInt32("AUTHNET_MODE2_COMMAND2_KEY_BYTE0", 1)) & 0xFF;
        uint32 const byte1 = GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_BYTE1",
            GetEnvUInt32("AUTHNET_MODE2_COMMAND2_KEY_BYTE1", 0)) & 0xFF;
        uint32 const value32 = GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_VALUE32",
            GetEnvUInt32("AUTHNET_MODE2_COMMAND2_KEY_VALUE32", 1));
        uint32 const flags = GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_FLAGS", 0) & 0xFFFF;

        writer.WriteBits(byte0, 8);
        writer.WriteBits(0, 12);
        writer.WriteBits(byte1, 8);
        writer.WriteUInt32(value32);
        writer.WriteBits(flags, 16);
    }

    std::vector<uint8> BuildLoginAuthStatusProbe(uint32 status)
    {
        // E24300 choice=1 + E23980 status byte. Wire: 400a00 for status 0.
        Skyfire::Authnet::BitWriter writer;

        WriteCgaSoftHeader(writer);
        writer.WriteBits(1, 1); // choice = status
        writer.WriteBits(status & 0xFF, 8);
        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildLoginAuthEmptyListProbe()
    {
        // E24300 choice=0 + E23BA0 count=0. Wire: 400200.
        Skyfire::Authnet::BitWriter writer;

        WriteCgaSoftHeader(writer);
        writer.WriteBits(0, 1); // choice = list
        writer.WriteBits(0, 7); // account count
        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildLoginAuthSingleListProbe()
    {
        // Choice0 + one Handle. List alone parks BattlenetLogin at state 2
        // (:64118 / :61112). Follow with Soft Soft mode2 cmd3 (empty) so
        // B39020 advances to state 3 / RESPONSE_CONNECTED.
        Skyfire::Authnet::BitWriter writer;

        WriteCgaSoftHeader(writer);
        writer.WriteBits(0, 1); // choice = list
        writer.WriteBits(1, 7); // one account
        WriteGameAccountHandle(writer);
        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildLoginAuthProbe(uint32 status, char const*& responseMode)
    {
        // Default single-list so COP_CONNECT can select an account and
        // advance to COP_GET_REALMS / 4601.
        responseMode = GetEnvOrDefault("AUTHNET_MODE2_COMMAND0_RESPONSE", "single-list");
        if (StringEquals(responseMode, "none") || StringEquals(responseMode, "skip") || StringEquals(responseMode, "off"))
            return {};

        if (StringEquals(responseMode, "status"))
            return BuildLoginAuthStatusProbe(status);

        if (StringEquals(responseMode, "empty-list"))
            return BuildLoginAuthEmptyListProbe();

        responseMode = "single-list";
        return BuildLoginAuthSingleListProbe();
    }

    bool ShouldPushCachedGameAccountAfterResources()
    {
        // Default OFF. B53940 never enables Soft mode2 receive, so unsolicited
        // CGA (mode2 cmd0) Soft-bounces. Playtests :54235 (mid-PFTY) and
        // :56463 (after 19 lookups) both quoted 402A…WoW1 in the next uplink
        // and never sent solicited 4002/4601. Answer CGA only on 4002 or as
        // pre-CGA before a solicited 4601.
        char const* value = GetEnvOrDefault("AUTHNET_PUSH_CACHED_GAMEACCOUNT_AFTER_RESOURCES", "0");
        return StringEquals(value, "1") || StringEquals(value, "true") ||
            StringEquals(value, "yes") || StringEquals(value, "on");
    }

    bool ShouldPushCachedGameAccountAfterSoftFinish()
    {
        // Default OFF. Playtest :54235: unsolicited CGA after Soft-finish RC4
        // Soft-bounces — next uplink quotes CGA plain (402A…WoW1) plus our
        // resource-result plains, then Cach/News service storm. COP_CONNECT
        // still REALM_LIST_FAILED ~400ms after AUTHENTICATED. Keep off;
        // AUTHNET_PUSH_CACHED_GAMEACCOUNT_AFTER_SOFT_FINISH=1 to re-probe.
        char const* value = GetEnvOrDefault("AUTHNET_PUSH_CACHED_GAMEACCOUNT_AFTER_SOFT_FINISH", "0");
        return StringEquals(value, "1") || StringEquals(value, "true") ||
            StringEquals(value, "yes") || StringEquals(value, "on");
    }

    bool ShouldPushRealmListAfterService()
    {
        // Relay evidence: client fails COP_CONNECT ~780ms after CachedGameAccount
        // with no further S->C, then sends a dying service blob; replies after
        // that never reach the client. Default off — push after resources.
        char const* value = GetEnvOrDefault("AUTHNET_PUSH_REALMLIST_AFTER_SERVICE", "0");
        return StringEquals(value, "1") || StringEquals(value, "true") ||
            StringEquals(value, "yes") || StringEquals(value, "on");
    }

    char const* GetRealmListPushAfterLoginAuthMode()
    {
        // Default OFF. Unsolicited Soft mode1 cmd6 Start is not a valid
        // receive command after B53940: cmd6 is enabled on the TX bitmask
        // (offset 344, mode 1) so the client can send RequestRealmList
        // (4601). Incoming Start has no mode1 handler table and bounces.
        // Mode0 cmd4 IS enabled on the RX bitmask (offset 728).
        // A83990 (COP_GET_REALMS) on BattlenetLogin is A7C410 and does
        // send solicited 4601. Glue still draws WowSvcs REALM_INFO, which
        // only fills from Grunt A63990 or from BN_REALM via A777B0.
        // AUTHNET_PUSH_REALMLIST_AFTER_LOGINAUTH=1 restores Start+Update+Complete.
        // =updates sends only mode0 cmd4 RealmUpdate (RX-bit probe).
        return GetEnvOrDefault("AUTHNET_PUSH_REALMLIST_AFTER_LOGINAUTH", "0");
    }

    bool ShouldPushRealmListAfterLoginAuth()
    {
        char const* value = GetRealmListPushAfterLoginAuthMode();
        if (StringEquals(value, "0") || StringEquals(value, "false") ||
            StringEquals(value, "no") || StringEquals(value, "none") ||
            StringEquals(value, "off"))
            return false;
        return true;
    }

    bool ShouldPushRealmListStartComplete()
    {
        char const* value = GetRealmListPushAfterLoginAuthMode();
        return StringEquals(value, "1") || StringEquals(value, "true") ||
            StringEquals(value, "yes") || StringEquals(value, "on") ||
            StringEquals(value, "start-complete") || StringEquals(value, "full");
    }

    bool ShouldPushRealmListAfterResources()
    {
        // Default OFF. Unsolicited Soft mode1 cmd6 Start/Complete and Soft
        // mode0 cmd4 RealmUpdate are Soft-bounced the same way as CGA
        // (client quotes them after a 9300 trailer in the next uplink). That
        // leaves the socket half-alive and spins a Cach/News service storm.
        // Answer Soft mode1 cmd6 only when the client sends RequestRealmList.
        char const* value = GetEnvOrDefault("AUTHNET_PUSH_REALMLIST_AFTER_RESOURCES", "0");
        return StringEquals(value, "1") || StringEquals(value, "true") ||
            StringEquals(value, "yes") || StringEquals(value, "on");
    }

    bool ShouldPushRealmListUpdates()
    {
        // RealmUpdate is a separate Soft frame (default mode1 cmd6 method 0),
        // not part of RealmList::Result. Mode0 cmd4 is Licenses, not names.
        char const* value = GetEnvOrDefault("AUTHNET_PUSH_REALMLIST_UPDATES", "1");
        return !StringEquals(value, "0") && !StringEquals(value, "false") &&
            !StringEquals(value, "no") && !StringEquals(value, "none") &&
            !StringEquals(value, "off");
    }

    bool ShouldSendBurstMode2Response()
    {
        // Default OFF. Same unsolicited Soft mode2 CGA as after-resources;
        // B53940 does not enable mode2 receive.
        char const* value = GetEnvOrDefault("AUTHNET_SEND_BURST_MODE2_RESPONSE", "0");
        return StringEquals(value, "1") || StringEquals(value, "true") ||
            StringEquals(value, "yes") || StringEquals(value, "on");
    }

    uint32 GetMode2Command0DelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND0_DELAY_MS", 0);
    }

    uint32 GetMode2Command0Status()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND0_STATUS", 0);
    }

    char const* GetMode2Command2ResponseMode()
    {
        // Default none. Unsolicited cmd2 with realmlist push stormed (:61112).
        // List CGA alone stores accounts (BattlenetLogin state 2); proceed is
        // Soft Soft mode2 cmd3 (B39680 → B39020), not cmd2 detail.
        return GetEnvOrDefault("AUTHNET_MODE2_COMMAND2_RESPONSE", "none");
    }

    char const* GetMode2Command3ResponseMode()
    {
        // Mode2 cmd3 body is empty (E22C30 stub). B39020 posts method 1
        // Result=1 Complete and enables Soft TX cmd8/9. Default: send after
        // CGA (once BN Updates are out). 4601 Complete is separate
        // (AUTHNET_MODE2_COMMAND3_ON_4601, default none) — a second Complete
        // closes Soft. after-join sends it only once Join was CHOICE 0 ACKed.
        return GetEnvOrDefault("AUTHNET_MODE2_COMMAND3_RESPONSE", "empty");
    }

    bool ShouldSendMode2Command3EmptyProbe(char const* mode)
    {
        return StringEquals(mode, "empty") || StringEquals(mode, "1") ||
            StringEquals(mode, "true") || StringEquals(mode, "yes") ||
            StringEquals(mode, "on");
    }

    bool ShouldSendMode2Command2DetailProbe(char const* mode)
    {
        return StringEquals(mode, "detail") || StringEquals(mode, "full") ||
            StringEquals(mode, "1") || StringEquals(mode, "true") || StringEquals(mode, "yes");
    }

    uint32 GetMode2Command2DelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND2_DELAY_MS", 0);
    }

    char const* GetMode2Command8ResponseMode()
    {
        return GetEnvOrDefault("AUTHNET_MODE2_COMMAND8_RESPONSE", "none");
    }

    bool ShouldSendMode2Command8StructuredProbe(char const* mode)
    {
        return StringEquals(mode, "structured") || StringEquals(mode, "empty-structured") ||
            StringEquals(mode, "1") || StringEquals(mode, "true") || StringEquals(mode, "yes");
    }

    uint32 GetMode2Command8DelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND8_DELAY_MS", 0);
    }

    // Client sends Soft mode2 cmd8 (~16 B) immediately after after-cga Complete
    // (B39020 enables TX 8/9). Leaving it unanswered kills Soft before Join
    // (:49402 closed at 918 B). :63405 CHOICE 1 status (480A01) bounced.
    // :65342 empty-structured (CHOICE 0, 0 routes) kept Soft alive and
    // showed the realm list, but Join retries forever with no game address.
    // Default structured: CHOICE 0 Success + 1 IPv4:port so B39180 /
    // B383D0 can hand the client a world endpoint. empty-structured still
    // Soft-ACKs without a route. status is the failed Failure ACK.
    char const* GetMode2Command8RequestResponseMode()
    {
        return GetEnvOrDefault("AUTHNET_MODE2_COMMAND8_REQUEST_RESPONSE", "structured");
    }

    bool IsMode2Command8SuccessResponse(char const* mode)
    {
        return StringEquals(mode, "structured") || StringEquals(mode, "empty-structured");
    }

    uint32 GetMode2Command8RequestDelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND8_REQUEST_DELAY_MS", 0);
    }

    uint32 GetMode2Command8RequestGapMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND8_REQUEST_GAP_MS", 0);
    }

    // Gap between separately-aligned Soft cmd6 frames. Default 0: queue every
    // frame in OnRead so a 4601+FIN / Cancel still flushes Start+Updates+
    // Complete (_closeWhenWritesFlush). A 25 ms timer lost the rest when
    // the glue Cancel closed the socket after only Start (pktlog 022613).
    // AUTHNET_REALMLIST_FRAME_DELAY_MS=25 restores the staggered recvs.
    uint32 GetRealmListFrameDelayMs()
    {
        return GetEnvUInt32("AUTHNET_REALMLIST_FRAME_DELAY_MS", 0);
    }

    // First solicited 4601 is from A82B10 while COP id is still 2 (CONNECT).
    // Empty Complete there → REALM_NOT_FOUND → 3A57F0 GET_REALMS (COP id 9).
    // Delay before Updates+Complete so A85240 sees COP==9 (REALM_LIST_SUCCESS).
    uint32 GetGetRealmsFollowupDelayMs()
    {
        return GetEnvUInt32("AUTHNET_GET_REALMS_FOLLOWUP_DELAY_MS", 200);
    }

    uint32 GetMode2Command8ListCount(char const* name)
    {
        return std::min<uint32>(GetEnvUInt32(name, 0), 16);
    }

    uint32 GetMode2Command8List6Count()
    {
        if (HasEnvValue("AUTHNET_MODE2_COMMAND8_LIST6_COUNT"))
            return GetMode2Command8ListCount("AUTHNET_MODE2_COMMAND8_LIST6_COUNT");

        if (StringEquals(GetMode2Command8ResponseMode(), "empty-structured") ||
            StringEquals(GetMode2Command8RequestResponseMode(), "empty-structured"))
            return 0;

        return 1;
    }

    bool TryParseIPv4AddressBytes(std::string const& address, std::array<uint8, 4>& bytes)
    {
        size_t offset = 0;
        for (size_t index = 0; index < bytes.size(); ++index)
        {
            if (offset >= address.size())
                return false;

            char* end = nullptr;
            unsigned long value = std::strtoul(address.c_str() + offset, &end, 10);
            if (!end || end == address.c_str() + offset || value > 255)
                return false;

            bytes[index] = uint8(value);
            offset = size_t(end - address.c_str());
            if (index + 1 < bytes.size())
            {
                if (offset >= address.size() || address[offset] != '.')
                    return false;
                ++offset;
            }
        }

        return offset == address.size();
    }

    // Prefer LocalAddress for loopback clients (same idea as AuthSocket realmlist).
    Skyfire::Net::Address const& ChooseRealmAddressForClient(Realm const& realm, bool clientIsLoopback);

    std::vector<uint8> BuildMode2Command8DefaultRouteList(uint32 entryCount,
        Realm const* selectedRealm = nullptr, bool clientIsLoopback = false)
    {
        std::vector<uint8> bytes(6 * entryCount, 0);
        if (!entryCount)
            return bytes;

        Realm const* realm = selectedRealm ? selectedRealm :
            FindAuthnetRealm(GetEnvOrDefault("AUTHNET_MODE2_COMMAND2_NAME", ""));
        std::string defaultAddress = "127.0.0.1";
        uint32 defaultPort = 8085;
        if (realm)
        {
            Skyfire::Net::Address const& chosen = ChooseRealmAddressForClient(*realm, clientIsLoopback);
            if (!chosen.GetHost().empty())
            {
                defaultAddress = chosen.GetHost();
                if (chosen.GetPort())
                    defaultPort = chosen.GetPort();
            }
            else if (!realm->ExternalAddress.GetHost().empty())
            {
                defaultAddress = realm->ExternalAddress.GetHost();
                if (realm->ExternalAddress.GetPort())
                    defaultPort = realm->ExternalAddress.GetPort();
            }
            else if (!realm->LocalAddress.GetHost().empty())
            {
                defaultAddress = realm->LocalAddress.GetHost();
                if (realm->LocalAddress.GetPort())
                    defaultPort = realm->LocalAddress.GetPort();
            }
        }

        std::array<uint8, 4> address = { 127, 0, 0, 1 };
        TryParseIPv4AddressBytes(GetEnvOrDefault("AUTHNET_MODE2_COMMAND8_ROUTE_ADDRESS",
            GetEnvOrDefault("AUTHNET_MODE2_COMMAND2_ENDPOINT_ADDRESS", defaultAddress.c_str())), address);

        uint32 const port = GetEnvUInt32("AUTHNET_MODE2_COMMAND8_ROUTE_PORT",
            GetEnvUInt32("AUTHNET_MODE2_COMMAND2_PORT", defaultPort)) & 0xFFFF;

        for (uint32 index = 0; index < entryCount; ++index)
        {
            size_t const offset = size_t(index) * 6;
            bytes[offset] = address[0];
            bytes[offset + 1] = address[1];
            bytes[offset + 2] = address[2];
            bytes[offset + 3] = address[3];
            bytes[offset + 4] = uint8(port >> 8);
            bytes[offset + 5] = uint8(port);
        }

        SF_LOG_INFO("server.authserver",
            "authnet Soft Join Soft-ACK route list6 count=%u ip=%u.%u.%u.%u port=%u loopback_client=%s realm=%s",
            entryCount, address[0], address[1], address[2], address[3], port,
            clientIsLoopback ? "yes" : "no",
            realm ? realm->name.c_str() : "(none)");

        return bytes;
    }

    std::vector<uint8> GetConfiguredListBytes(char const* name, size_t entrySize, uint32 entryCount)
    {
        std::vector<uint8> bytes(entrySize * entryCount, 0);
        std::vector<uint8> configuredBytes;
        if (!bytes.empty() && TryParseHexBytes(std::getenv(name), bytes.size(), configuredBytes))
            return configuredBytes;

        return bytes;
    }

    std::vector<uint8> BuildLoginGameAccountDetailProbe()
    {
        Skyfire::Authnet::BitWriter writer;

        uint32 const keyByte0 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_KEY_BYTE0",
            GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_BYTE0", 1)) & 0xFF;
        uint32 const keyByte1 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_KEY_BYTE1",
            GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_BYTE1", 0)) & 0xFF;
        uint32 const keyValue32 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_KEY_VALUE32",
            GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_VALUE32", 1));
        uint32 const field418 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FIELD418", 0);
        uint32 const field41C = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FIELD41C", 0);
        uint32 const field420 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FIELD420", 0) & 0xFF;
        uint32 const field424 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FIELD424", 0);
        uint32 const finalByte = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FINAL_BYTE", 0) & 0xFF;

        std::string accountName = GetEnvOrDefault("AUTHNET_MODE2_COMMAND2_NAME", "WoW1");
        if (accountName.size() > 0x3FC)
            accountName.resize(0x3FC);

        writer.WriteBits(0x02, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x02, 4);

        writer.WriteBits(1, 1);
        writer.WriteUInt32(field41C);
        writer.WriteUInt32(field424);
        writer.WriteBits(field420, 8);
        writer.WriteBits(0, 19);
        writer.WriteUInt32(field418 + 0x80000000u);
        writer.WriteBits(uint32(accountName.size()), 10);
        writer.WriteBytes(accountName.data(), accountName.size());
        writer.WriteBits(0, 1);
        writer.WriteBits(finalByte, 8);

        writer.WriteBits(keyByte0, 8);
        writer.WriteBits(0, 12);
        writer.WriteBits(keyByte1, 8);
        writer.WriteUInt32(keyValue32);
        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildMode2Command8StructuredProbe(
        Realm const* selectedRealm = nullptr, bool clientIsLoopback = false,
        SessionKey const* worldSessionKeyOrNull = nullptr, bool emptyRoutes = false,
        uint32 fieldOverride = 0)
    {
        Skyfire::Authnet::BitWriter writer;

        uint32 fieldValue = fieldOverride ? fieldOverride :
            (selectedRealm ? selectedRealm->m_ID : 0);
        if (HasEnvValue("AUTHNET_MODE2_COMMAND8_FIELD"))
            fieldValue = GetEnvUInt32("AUTHNET_MODE2_COMMAND8_FIELD", fieldValue);
        uint32 const list6Count = emptyRoutes ? 0 : GetMode2Command8List6Count();
        uint32 const list18Count = GetMode2Command8ListCount("AUTHNET_MODE2_COMMAND8_LIST18_COUNT");
        std::vector<uint8> list6 = BuildMode2Command8DefaultRouteList(list6Count, selectedRealm, clientIsLoopback);
        std::vector<uint8> configuredList6;
        if (!list6.empty() && TryParseHexBytes(std::getenv("AUTHNET_MODE2_COMMAND8_LIST6_HEX"), list6.size(), configuredList6))
            list6 = configuredList6;

        std::vector<uint8> list18 = GetConfiguredListBytes("AUTHNET_MODE2_COMMAND8_LIST18_HEX", 18, list18Count);

        writer.WriteBits(0x08, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x02, 4);

        writer.WriteBits(0, 1); // Success

        // Mode2 Soft-ACK Success has no raw SessionKey (E24200: field+lists).
        // Client derives 40 bytes in B37290 from SoftK64. Opt-in raw key before
        // field mis-parses list6 — keep OFF.
        if (worldSessionKeyOrNull &&
            StringEnabled(GetEnvOrDefault("AUTHNET_MODE2_COMMAND8_INCLUDE_SESSION_KEY", "0")))
        {
            writer.WriteBytes(worldSessionKeyOrNull->data(), worldSessionKeyOrNull->size());
        }

        writer.WriteUInt32(fieldValue);

        writer.WriteBits(list6Count, 5);
        if (!list6.empty())
            writer.WriteBytes(list6.data(), list6.size());

        writer.WriteBits(list18Count, 5);
        if (!list18.empty())
            writer.WriteBytes(list18.data(), list18.size());

        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildMode2Command8StatusProbe()
    {
        Skyfire::Authnet::BitWriter writer;

        // CHOICE 1 Failure. Live :63405 (480A01) bounced 445 B then closed.
        // AUTHNET_MODE2_COMMAND8_STATUS=0 still takes the Failure path.
        uint32 const status = GetEnvUInt32("AUTHNET_MODE2_COMMAND8_STATUS", 1) & 0xFF;

        writer.WriteBits(0x08, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x02, 4);

        writer.WriteBits(1, 1);
        writer.WriteBits(status, 8);
        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildEmptyModeCommand(uint32 command, uint32 mode)
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(command, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(mode, 4);

        return writer.Data();
    }

    std::vector<uint8> BuildMode1Command1Status(uint16 status)
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x01, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x01, 4);
        writer.WriteBits(status, 16);

        return writer.Data();
    }

    bool TryGetPostLoginMode1Command1Status(uint16& status)
    {
        char const* value = std::getenv("AUTHNET_MODE1_COMMAND1_POST_LOGIN_STATUS");
        if (!value || !value[0] || StringEquals(value, "none") || StringEquals(value, "skip") || StringEquals(value, "off"))
            return false;

        char* end = nullptr;
        unsigned long parsed = std::strtoul(value, &end, 0);
        if (!end || *end != '\0' || parsed > 0xFFFF)
            return false;

        status = uint16(parsed);
        return true;
    }

    char const* GetMode1Command6ResponseMode()
    {
        // Solicited 4601 is Soft mode1 cmd6 TX (RequestRealmList). Mode1
        // table 62F2B0 has RX handlers for cmds 0-4 only; S->C cmd6 is
        // error 10 in B76960 and never reaches A777B0. Names come from
        // mode2 cmd2 (B39530 → B38E30 → method 0 RealmUpdate). Default
        // mode2-update sends those Updates then empty mode2 cmd3 Complete
        // (B39020 method 1 Result=1). AUTHNET_MODE1_COMMAND6_RESPONSE=cmd6
        // restores the old mode1 Start/Update/Complete frames. none disables.
        return GetEnvOrDefault("AUTHNET_MODE1_COMMAND6_RESPONSE", "mode2-update");
    }

    uint16 GetMode1Command6Status()
    {
        return uint16(GetEnvUInt32("AUTHNET_MODE1_COMMAND6_STATUS", 0) & 0xFFFF);
    }

    uint32 GetMode1Command6FailureReason()
    {
        return GetEnvUInt32("AUTHNET_MODE1_COMMAND6_FAILURE_REASON", 0) & 0xFF;
    }

    uint32 GetRealmUpdateSoftCommand()
    {
        // Used only by AUTHNET_MODE1_COMMAND6_RESPONSE=cmd6. Default 4601
        // replies are mode2 cmd2 (see BuildMode2Command2RealmUpdate).
        return GetEnvUInt32("AUTHNET_MODE1_REALMUPDATE_COMMAND", 6) & 0x3F;
    }

    uint32 GetRealmUpdateSoftMode()
    {
        return GetEnvUInt32("AUTHNET_REALMUPDATE_SOFT_MODE", 1) & 0xF;
    }

    uint32 GetRealmListWowMethodBits()
    {
        // Type 2302 Larva::Invoke::WoW is an 8-bit CHOICE (A78ED0 a2[1]).
        // 0=RealmUpdate, 1=RealmList, 2=JoinResponse. Solicited cmd6 used
        // to omit this and send only 2547's 2-bit Result — GET_REALMS
        // completed with an empty viewer. AUTHNET_REALMLIST_WOW_METHOD_BITS=0
        // restores that body.
        return GetEnvUInt32("AUTHNET_REALMLIST_WOW_METHOD_BITS", AUTHNET_WOW_METHOD_BITS) & 0xFF;
    }

    uint32 GetRealmListWowServiceBits()
    {
        // C->S 4601 is Soft header only. A75010 still encodes type 1620, so
        // the S->C pair on command 6 is type 2292 (8-bit Larva service).
        // A78ED0 only fills BN_REALM when *a2==9 (WoW). Without tag 9 the
        // 2302 method byte is read as the 2292 service (1=Authentication)
        // and A777B0 never runs — 2026-08-30 189 B bit-contiguous reply,
        // names on the wire, empty viewer. AUTHNET_REALMLIST_WOW_SERVICE_BITS=0
        // omits the tag (2302-only).
        return GetEnvUInt32("AUTHNET_REALMLIST_WOW_SERVICE_BITS", AUTHNET_INVOKE_CHOICE_BITS) & 0xFF;
    }

    // Soft cmd6 is the 4601 RPC: one 2292 object per recv. A combined
    // Start+Updates+Complete blob lets the waiter consume Start and drop
    // the rest. Packet 9 Header after Soft-finish is Soft cmd 9, not
    // B236E0. Default 0: separate cmd6 frames. Set
    // AUTHNET_REALMLIST_HEADER_INVOKE=1 only to retry Header m_id 5.
    bool ShouldSendRealmListHeaderInvoke()
    {
        char const* value = GetEnvOrDefault("AUTHNET_REALMLIST_HEADER_INVOKE", "0");
        return !StringEquals(value, "0") && !StringEquals(value, "false") &&
            !StringEquals(value, "no") && !StringEquals(value, "off") &&
            !StringEquals(value, "none");
    }

    void WriteLarvaWowInvokePrefix(Skyfire::Authnet::BitWriter& writer, uint32 wowMethod)
    {
        uint32 const serviceBits = GetRealmListWowServiceBits();
        if (serviceBits)
            writer.WriteBits(AuthnetLarvaInvoke::WoW, serviceBits);
        uint32 const methodBits = GetRealmListWowMethodBits();
        if (methodBits)
            writer.WriteBits(wowMethod, methodBits);
    }

    std::vector<uint8> BuildMode1Command6Status(uint16 status)
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x06, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x01, 4);
        writer.WriteBits(status, 16);

        return writer.Data();
    }

    // Battlenet::Event::Larva::Invoke::WoW::RealmList::Result (type 2547):
    //   CHOICE 2 bits: 0=Start, 1=Complete, 2=Failure
    // Start (2723): ARRAY ToonCountEntryList, 7-bit count 0..64
    // Complete (2724): empty struct
    // Failure (2725): u8 m_reasonCode
    // 2026-08-30 024431 / :56077: Event m_id 5 stuffed after the Soft
    // header (4629…576F57) — five SMSG, names on the wire, still empty.
    // B38510 delivers Header+2292 in-process via vtable+112; mode1 has
    // no cmd6 RX handler, so that Event prefix never reaches A7E890.
    // Soft cmd6 + Larva tag 9 only (no Event header).
    void WriteSoftCmd6LarvaWow(Skyfire::Authnet::BitWriter& writer, uint32 wowMethod)
    {
        writer.WriteBits(0x06, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x01, 4);
        WriteLarvaWowInvokePrefix(writer, wowMethod);
    }

    void WriteMode1Command6Header(Skyfire::Authnet::BitWriter& writer)
    {
        WriteSoftCmd6LarvaWow(writer, AuthnetLarvaWow::RealmList);
    }

    // Shared region/site for Start toonCounts and RealmUpdate handles.
    void GetRealmHandleIds(uint32& region, uint32& site)
    {
        region = GetEnvUInt32("AUTHNET_REALM_REGION", 1) & 0xFF;
        site = GetEnvUInt32("AUTHNET_REALM_SITE", 1) & 0xFF;
    }

    std::vector<uint8> FinishSoftPacket(Skyfire::Authnet::BitWriter& writer)
    {
        writer.AlignToByte();
        return writer.Data();
    }

    std::vector<Realm> CollectAuthnetRealms()
    {
        std::vector<Realm> realms;
        char const* singleName = std::getenv("AUTHNET_REALM_NAME");
        if (singleName && singleName[0] && StringEquals(GetEnvOrDefault("AUTHNET_REALMUPDATE_SOURCE", "list"), "env"))
        {
            Realm stub;
            stub.name = singleName;
            stub.m_ID = GetEnvUInt32("AUTHNET_REALM_ID", 1);
            stub.icon = uint8(GetEnvUInt32("AUTHNET_REALM_TYPE", 0) & 0xFF);
            stub.timezone = uint8(GetEnvUInt32("AUTHNET_REALM_CATEGORY", 1) & 0xFF);
            stub.flag = REALM_FLAG_NONE;
            stub.populationLevel = 0.0f;
            realms.push_back(stub);
            return realms;
        }

        for (RealmList::RealmMap::const_iterator i = sRealmList->begin(); i != sRealmList->end(); ++i)
        {
            if (i->first.empty())
                continue;
            realms.push_back(i->second);
            if (realms.size() >= 16)
                break;
        }

        if (realms.empty())
        {
            Realm stub;
            stub.name = "SkyFire";
            stub.m_ID = 1;
            stub.icon = 0;
            stub.timezone = 1;
            stub.flag = REALM_FLAG_NONE;
            stub.populationLevel = 0.0f;
            stub.ExternalAddress = Skyfire::Net::Address("127.0.0.1", 8085);
            stub.LocalAddress = stub.ExternalAddress;
            realms.push_back(stub);
        }

        return realms;
    }

    // Mode2 cmd0 CHOICE 0 list. B38BE0 copies these E235A0 handles into
    // RealmList Start (method 1 Result=0). CGA already sends one fake
    // account on 4002. Do not send this again on 4601: playtest 115001
    // closed at 902 B, stuck on SUCCESS, Cancel restarted GET_REALMS
    // with no window. Empty cmd3 Complete (B39020) is the 4601 reply.
    std::vector<uint8> BuildMode2Command0RealmListStart()
    {
        Skyfire::Authnet::BitWriter writer;
        WriteCgaSoftHeader(writer);
        writer.WriteBits(0, 1);

        std::vector<Realm> const realms = CollectAuthnetRealms();
        uint32 const count = uint32(std::min<size_t>(realms.size(), 64));
        writer.WriteBits(count, 7);

        uint32 region = 0;
        uint32 site = 0;
        GetRealmHandleIds(region, site);
        for (uint32 i = 0; i < count; ++i)
        {
            writer.WriteBits(region, 8);
            writer.WriteBits(0, 12);
            writer.WriteBits(site, 8);
            writer.WriteUInt32(realms[i].m_ID ? realms[i].m_ID : 1);
            writer.WriteBits(0, 16);
        }
        writer.AlignToByte();
        return writer.Data();
    }

    void WriteToonCountEntry(Skyfire::Authnet::BitWriter& writer, Realm const& realm, uint16 toonCount)
    {
        uint32 region = 0;
        uint32 site = 0;
        GetRealmHandleIds(region, site);
        uint32 const realmId = GetEnvUInt32("AUTHNET_REALM_ID", realm.m_ID ? realm.m_ID : 1);

        writer.WriteBits(region, 8);
        writer.WriteBits(site, 8);
        writer.WriteUInt32(realmId);
        writer.WriteBits(toonCount, 16);
    }

    void WriteMode1RealmListStart(Skyfire::Authnet::BitWriter& writer)
    {
        // RealmList::Result::Start: ARRAY ToonCountEntryList (7-bit count).
        // Same realm set as RealmUpdate so handles match before Complete.
        WriteMode1Command6Header(writer);
        writer.WriteBits(0, 2); // Start

        std::vector<Realm> const realms = CollectAuthnetRealms();
        uint32 const count = uint32(std::min<size_t>(realms.size(), 64));
        writer.WriteBits(count, 7);
        for (uint32 i = 0; i < count; ++i)
            WriteToonCountEntry(writer, realms[i], 0);
    }

    void WriteMode1RealmListComplete(Skyfire::Authnet::BitWriter& writer)
    {
        WriteMode1Command6Header(writer);
        writer.WriteBits(1, 2); // Complete
    }

    std::vector<uint8> BuildMode1Command6RealmListStart()
    {
        Skyfire::Authnet::BitWriter writer;
        WriteMode1RealmListStart(writer);
        return FinishSoftPacket(writer);
    }

    std::vector<uint8> BuildMode1Command6RealmListComplete()
    {
        Skyfire::Authnet::BitWriter writer;
        WriteMode1RealmListComplete(writer);
        return FinishSoftPacket(writer);
    }

    std::vector<uint8> BuildMode1Command6RealmListFailure(uint32 reason)
    {
        Skyfire::Authnet::BitWriter writer;
        WriteMode1Command6Header(writer);
        writer.WriteBits(2, 2); // Failure
        writer.WriteBits(reason & 0xFF, 8);
        writer.AlignToByte();
        return writer.Data();
    }

    void WriteFloatBits(Skyfire::Authnet::BitWriter& writer, float value)
    {
        uint32 bits = 0;
        std::memcpy(&bits, &value, sizeof(bits));
        writer.WriteUInt32(bits);
    }

    // Prefer LocalAddress for loopback clients (same idea as AuthSocket realmlist).
    Skyfire::Net::Address const& ChooseRealmAddressForClient(Realm const& realm, bool clientIsLoopback)
    {
        if (clientIsLoopback && !realm.LocalAddress.GetHost().empty())
            return realm.LocalAddress;
        if (!realm.ExternalAddress.GetHost().empty())
            return realm.ExternalAddress;
        return realm.LocalAddress;
    }

    bool ProbeTcpIPv4(std::string const& host, uint16 port, uint32 timeoutMs)
    {
        if (host.empty() || !port)
            return false;
        if (!timeoutMs)
            timeoutMs = 1;

        try
        {
            boost::system::error_code parseEc;
            boost::asio::ip::address_v4 const addr = boost::asio::ip::make_address_v4(host, parseEc);
            if (parseEc)
                return false;

            boost::asio::io_context io;
            boost::asio::ip::tcp::socket socket(io);
            boost::asio::steady_timer timer(io);
            boost::asio::ip::tcp::endpoint const endpoint(addr, port);
            boost::system::error_code connectEc = boost::asio::error::would_block;

            socket.open(endpoint.protocol());
            socket.async_connect(endpoint, [&](boost::system::error_code const& ec)
            {
                connectEc = ec;
                timer.cancel();
            });
            timer.expires_after(std::chrono::milliseconds(timeoutMs));
            timer.async_wait([&](boost::system::error_code const& ec)
            {
                if (ec)
                    return;
                boost::system::error_code ignored;
                socket.close(ignored);
            });
            io.run();
            return !connectEc;
        }
        catch (...)
        {
            return false;
        }
    }

    std::mutex g_authnetReachMu;
    std::map<uint32, std::pair<bool, uint32>> g_authnetReachCache; // id -> {up, getMSTime}
    std::set<uint32> g_authnetReachInFlight;

    void StoreAuthnetRealmReachable(uint32 realmId, bool up)
    {
        if (!realmId)
            return;
        std::lock_guard<std::mutex> lock(g_authnetReachMu);
        g_authnetReachCache[realmId] = { up, getMSTime() };
    }

    bool LookupAuthnetRealmReachable(uint32 realmId, bool& up)
    {
        up = true;
        if (!realmId)
            return false;
        uint32 const ttlMs = GetEnvUInt32("AUTHNET_REALM_REACHABILITY_CACHE_MS", 30000);
        std::lock_guard<std::mutex> lock(g_authnetReachMu);
        auto it = g_authnetReachCache.find(realmId);
        if (it == g_authnetReachCache.end())
            return false;
        if (getMSTimeDiff(it->second.second, getMSTime()) > ttlMs)
            return false;
        up = it->second.first;
        return true;
    }

    bool ProbeAuthnetRealmWorldPort(Realm const& realm)
    {
        if (realm.name.empty() || !realm.m_ID)
            return false;

        uint32 const timeoutMs = GetEnvUInt32("AUTHNET_REALM_REACHABILITY_TIMEOUT_MS", 200);
        std::string const localHost = realm.LocalAddress.GetHost();
        uint16 const localPort = uint16(realm.LocalAddress.GetPort());
        std::string const extHost = realm.ExternalAddress.GetHost();
        uint16 const extPort = uint16(realm.ExternalAddress.GetPort());
        bool up = ProbeTcpIPv4(localHost, localPort, timeoutMs);
        if (!up && (extHost != localHost || extPort != localPort))
            up = ProbeTcpIPv4(extHost, extPort, timeoutMs);
        StoreAuthnetRealmReachable(realm.m_ID, up);

        SF_LOG_INFO("server.authserver",
            "authnet realm probe id=%u name=%s local=%s:%u external=%s:%u reachable=%s",
            realm.m_ID, realm.name.c_str(),
            realm.LocalAddress.GetHost().c_str(), uint32(realm.LocalAddress.GetPort()),
            realm.ExternalAddress.GetHost().c_str(), uint32(realm.ExternalAddress.GetPort()),
            up ? "yes" : "no");
        return up;
    }

    void StartAuthnetRealmReachabilityRefresh()
    {
        std::vector<Realm> const realms = CollectAuthnetRealms();
        for (Realm const& realm : realms)
        {
            if (realm.name.empty() || !realm.m_ID)
                continue;

            bool cachedUp = false;
            if (LookupAuthnetRealmReachable(realm.m_ID, cachedUp))
                continue;

            {
                std::lock_guard<std::mutex> lock(g_authnetReachMu);
                if (g_authnetReachInFlight.count(realm.m_ID))
                    continue;
                g_authnetReachInFlight.insert(realm.m_ID);
            }

            std::thread([realm]()
            {
                ProbeAuthnetRealmWorldPort(realm);
                std::lock_guard<std::mutex> lock(g_authnetReachMu);
                g_authnetReachInFlight.erase(realm.m_ID);
            }).detach();
        }
    }

    bool AuthnetReachabilityReady()
    {
        {
            std::lock_guard<std::mutex> lock(g_authnetReachMu);
            if (!g_authnetReachInFlight.empty())
                return false;
        }

        std::vector<Realm> const realms = CollectAuthnetRealms();
        for (Realm const& realm : realms)
        {
            if (realm.name.empty() || !realm.m_ID)
                continue;
            bool up = false;
            if (!LookupAuthnetRealmReachable(realm.m_ID, up))
                return false;
        }
        return true;
    }

    // CGA (4002) must go out first. Waiting here is only for the first
    // RealmUpdates so Maps/Playerbots are not painted as online (yellow)
    // while their public-IP probes are still in flight.
    void WaitAuthnetRealmReachability()
    {
        uint32 const timeoutMs = GetEnvUInt32("AUTHNET_REALM_REACHABILITY_WAIT_MS", 500);
        StartAuthnetRealmReachabilityRefresh();
        uint32 const start = getMSTime();
        while (!AuthnetReachabilityReady())
        {
            if (getMSTimeDiff(start, getMSTime()) >= timeoutMs)
            {
                SF_LOG_INFO("server.authserver",
                    "authnet realm probe wait timed out after %u ms", timeoutMs);
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void ApplyAuthnetRealmListFromProbes(bool unknownIsOffline = false)
    {
        for (RealmList::RealmMap::const_iterator itr = sRealmList->begin(); itr != sRealmList->end(); ++itr)
        {
            if (itr->first.empty() || !itr->second.m_ID)
                continue;
            bool up = false;
            if (!LookupAuthnetRealmReachable(itr->second.m_ID, up))
            {
                if (unknownIsOffline)
                    sRealmList->SetRealmOffline(itr->second.m_ID, true);
                continue;
            }
            sRealmList->SetRealmOffline(itr->second.m_ID, !up);
        }
    }

    bool IsAuthnetRealmWorldOffline(Realm const& realm)
    {
        if (realm.name.empty())
            return true;
        return (realm.flag & REALM_FLAG_OFFLINE) || (realm.flag & REALM_FLAG_INVALID);
    }

    bool IsAuthnetRealmJoinable(Realm const* realm)
    {
        if (!realm || realm->name.empty())
            return false;
        if (IsAuthnetRealmWorldOffline(*realm))
            return false;
        return true;
    }

    Realm const* FindFirstReachableAuthnetRealm(uint32 skipRealmId)
    {
        // Do not UpdateIfNeed here: Join already holds Realm* into m_realms, and
        // UpdateRealms() clears that map (empty names / dangling route pointers).
        for (RealmList::RealmMap::const_iterator itr = sRealmList->begin(); itr != sRealmList->end(); ++itr)
        {
            if (!IsAuthnetRealmJoinable(&itr->second) || itr->second.m_ID == skipRealmId)
                continue;
            if (ProbeAuthnetRealmWorldPort(itr->second))
                return &itr->second;
        }
        return nullptr;
    }

    uint32 GetAuthnetRealmInfoFlags(Realm const& realm)
    {
        if (HasEnvValue("AUTHNET_REALM_INFO_FLAGS"))
            return GetEnvUInt32("AUTHNET_REALM_INFO_FLAGS", 0) & 0xFF;
        return uint32(realm.flag) & 0xFF;
    }

    uint32 GetAuthnetRealmOfflineStateFlags(Realm const& realm)
    {
        if (HasEnvValue("AUTHNET_REALM_STATE_FLAGS"))
            return GetEnvUInt32("AUTHNET_REALM_STATE_FLAGS", 0) & 0xFF;

        return (realm.flag & REALM_FLAG_OFFLINE) ? 0x02u : 0u;
    }

    // Soft mode1 cmd7: Creep JoinRealm (1676) → Larva JoinResponse (2383).
    // Success (2726): SessionKey fixed 40 bytes + RealmAddressList (v4/v6).
    std::vector<uint8> BuildMode1JoinResponseSuccess(
        SkyFire::Authnet::PasswordSrp::SessionKey const* sessionKeyOrNull,
        bool clientIsLoopback)
    {
        Skyfire::Authnet::BitWriter writer;
        writer.WriteBits(0x07, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x01, 4);
        writer.WriteBits(0, 1); // Success

        uint8 sessionKey[40] = {};
        if (sessionKeyOrNull)
        {
            size_t const n = std::min(sizeof(sessionKey), sessionKeyOrNull->size());
            std::memcpy(sessionKey, sessionKeyOrNull->data(), n);
        }
        writer.WriteBytes(sessionKey, sizeof(sessionKey));

        uint8 addrBytes[4] = { 127, 0, 0, 1 };
        uint16 port = 8085;
        for (RealmList::RealmMap::const_iterator i = sRealmList->begin(); i != sRealmList->end(); ++i)
        {
            if (i->first.empty())
                continue;
            Skyfire::Net::Address const& chosen = ChooseRealmAddressForClient(i->second, clientIsLoopback);
            uint32 ipNet = 0;
            if (!chosen.GetHost().empty())
                ipNet = chosen.ToIPv4NetworkOrder();
            else
                ipNet = Skyfire::Net::ToIPv4NetworkOrder("127.0.0.1");
            std::memcpy(addrBytes, &ipNet, 4);
            port = uint16(chosen.GetPort() ? chosen.GetPort() :
                (i->second.ExternalAddress.GetPort() ? i->second.ExternalAddress.GetPort() : 8085));
            break;
        }
        uint8 portBytes[2] = { uint8((port >> 8) & 0xFF), uint8(port & 0xFF) };

        writer.WriteBits(1, 5); // v4 count
        writer.WriteBytes(addrBytes, 4);
        writer.WriteBytes(portBytes, 2);
        writer.WriteBits(0, 5); // v6 count
        writer.AlignToByte();
        return writer.Data();
    }

    // RealmInfo.m_privilegedData (optional): VersionString (5-bit len 0..23) +
    // ConfigId u32 + IP4::AddressPort (4-byte address + 2-byte port).
    void WriteRealmPrivilegedData(Skyfire::Authnet::BitWriter& writer, Realm const& realm,
        bool clientIsLoopback)
    {
        std::string version = GetEnvOrDefault("AUTHNET_REALM_VERSION", "5.4.8");
        if (version.size() > 23)
            version.resize(23);

        uint32 configId = GetEnvUInt32("AUTHNET_REALM_CONFIG_ID",
            realm.gamebuild ? realm.gamebuild : 18414);

        char const* hostOverride = std::getenv("AUTHNET_REALM_ADDRESS");
        Skyfire::Net::Address const& chosen = ChooseRealmAddressForClient(realm, clientIsLoopback);
        uint16 port = uint16(GetEnvUInt32("AUTHNET_REALM_PORT",
            chosen.GetPort() ? chosen.GetPort() :
            (realm.ExternalAddress.GetPort() ? realm.ExternalAddress.GetPort() : 8085)) & 0xFFFF);

        uint32 ipNet = 0;
        if (hostOverride && hostOverride[0])
            ipNet = Skyfire::Net::ToIPv4NetworkOrder(hostOverride);
        else if (!chosen.GetHost().empty())
            ipNet = chosen.ToIPv4NetworkOrder();
        else
            ipNet = Skyfire::Net::ToIPv4NetworkOrder("127.0.0.1");

        uint8 addrBytes[4];
        std::memcpy(addrBytes, &ipNet, 4);
        // Port as 2-byte network order (kind-4 fixed field size 2).
        uint8 portBytes[2] = { uint8((port >> 8) & 0xFF), uint8(port & 0xFF) };

        writer.WriteBits(1, 1); // privilegedData present
        writer.WriteBits(uint32(version.size()), 5);
        writer.WriteBytes(version.data(), version.size());
        writer.WriteUInt32(configId);
        writer.WriteBytes(addrBytes, 4);
        writer.WriteBytes(portBytes, 2);
    }

    // Default OFF. :62799 coalesced Updates+Complete (173 B, names on the
    // wire) still tore the session down (902 B captured, then GET /agent).
    // A777B0's privileged branch asserts (AEC060) if the nested copy is
    // incomplete; E23CE0 failure is B76960 error 2 (vtable+88 close). Glue
    // names come from RealmInfo+24; Join is mode1 cmd7, not this optional.
    bool ShouldIncludeRealmPrivilegedData()
    {
        char const* value = GetEnvOrDefault("AUTHNET_REALM_PRIVILEGED", "0");
        return StringEquals(value, "1") || StringEquals(value, "true") ||
            StringEquals(value, "yes") || StringEquals(value, "on");
    }

    // Larva::Invoke::WoW::RealmUpdate body after the 2292 prefix: Handle +
    // Result Update + RealmInfo. No Soft header, no AlignToByte.
    void WriteRealmUpdateBody(Skyfire::Authnet::BitWriter& writer, Realm const& realm, bool clientIsLoopback)
    {
        uint32 region = 0;
        uint32 site = 0;
        GetRealmHandleIds(region, site);
        uint32 const realmId = GetEnvUInt32("AUTHNET_REALM_ID", realm.m_ID ? realm.m_ID : 1);
        uint32 const infoFlags = GetAuthnetRealmInfoFlags(realm);
        uint32 const stateFlags = GetAuthnetRealmOfflineStateFlags(realm);
        int32 const realmType = int32(GetEnvUInt32("AUTHNET_REALM_TYPE", realm.icon));
        uint32 const category = GetEnvUInt32("AUTHNET_REALM_CATEGORY", realm.timezone);

        std::string name = GetEnvOrDefault("AUTHNET_REALM_NAME", realm.name.c_str());
        if (name.empty())
            name = "SkyFire";
        if (name.size() > 0xFF)
            name.resize(0xFF);

        writer.WriteBits(region, 8);
        writer.WriteBits(site, 8);
        writer.WriteUInt32(realmId);
        writer.WriteBits(0, 1); // Result Update
        writer.WriteBits(infoFlags, 8);
        writer.WriteBits(uint32(name.size()), 8);
        writer.WriteBytes(name.data(), name.size());
        writer.WriteUInt32(uint32(realmType));
        writer.WriteUInt32(category);
        writer.WriteBits(stateFlags, 8);
        WriteFloatBits(writer, realm.populationLevel);
        if (ShouldIncludeRealmPrivilegedData())
            WriteRealmPrivilegedData(writer, realm, clientIsLoopback);
        else
            writer.WriteBits(0, 1);
    }

    // Packet 9 channel 1 + Header m_id 5 destClass 0 dest FourCC WoW, then
    // type 2292 tag 9 + WoW method. B236E0 → A7E890 → A78ED0.
    void WriteHeaderLarvaWow(Skyfire::Authnet::BitWriter& writer, uint32 wowMethod)
    {
        Skyfire::Authnet::WritePacketHeader(writer, AuthnetWire::PacketLogin, 1, AuthnetWire::ChannelEvent);
        Skyfire::Authnet::WriteCompleteEventHeader(writer, 5,
            AUTHNET_DESTINATION_CLASS_NONE, AUTHNET_SUBSCRIBER_FOURCC_WOW,
            AUTHNET_SUBSCRIBER_FOURCC_WOW);
        writer.WriteBits(AuthnetLarvaInvoke::WoW, AUTHNET_INVOKE_CHOICE_BITS);
        writer.WriteBits(wowMethod, AUTHNET_WOW_METHOD_BITS);
    }

    void WriteHeaderRealmListStart(Skyfire::Authnet::BitWriter& writer)
    {
        WriteHeaderLarvaWow(writer, AuthnetLarvaWow::RealmList);
        writer.WriteBits(0, 2); // Start
        std::vector<Realm> const realms = CollectAuthnetRealms();
        uint32 const count = uint32(std::min<size_t>(realms.size(), 64));
        writer.WriteBits(count, 7);
        for (uint32 i = 0; i < count; ++i)
            WriteToonCountEntry(writer, realms[i], 0);
    }

    void WriteHeaderRealmListComplete(Skyfire::Authnet::BitWriter& writer)
    {
        WriteHeaderLarvaWow(writer, AuthnetLarvaWow::RealmList);
        writer.WriteBits(1, 2); // Complete
    }

    void WriteHeaderRealmUpdate(Skyfire::Authnet::BitWriter& writer, Realm const& realm, bool clientIsLoopback)
    {
        WriteHeaderLarvaWow(writer, AuthnetLarvaWow::RealmUpdate);
        WriteRealmUpdateBody(writer, realm, clientIsLoopback);
    }

    void WriteMode1RealmUpdate(Skyfire::Authnet::BitWriter& writer, Realm const& realm, bool clientIsLoopback)
    {
        if (GetRealmUpdateSoftCommand() == 6 && GetRealmUpdateSoftMode() == 1)
            WriteSoftCmd6LarvaWow(writer, AuthnetLarvaWow::RealmUpdate);
        else
        {
            writer.WriteBits(GetRealmUpdateSoftCommand(), 6);
            writer.WriteBits(1, 1);
            writer.WriteBits(GetRealmUpdateSoftMode(), 4);
            WriteLarvaWowInvokePrefix(writer, AuthnetLarvaWow::RealmUpdate);
        }
        WriteRealmUpdateBody(writer, realm, clientIsLoopback);
    }

    std::vector<uint8> BuildMode1RealmUpdate(Realm const& realm, bool clientIsLoopback)
    {
        Skyfire::Authnet::BitWriter writer;
        WriteMode1RealmUpdate(writer, realm, clientIsLoopback);
        return FinishSoftPacket(writer);
    }

    std::vector<uint8> BuildMode1RealmListStartComplete(bool clientIsLoopback)
    {
        Skyfire::Authnet::BitWriter writer;
        std::vector<Realm> const realms = CollectAuthnetRealms();
        if (ShouldSendRealmListHeaderInvoke())
        {
            // Unsolicited Header Invoke fills BN_REALM before the 4601 RPC
            // returns. Cmd6 is one Complete so the waiter does not eat Start
            // and drop Updates.
            WriteHeaderRealmListStart(writer);
            for (size_t i = 0; i < realms.size(); ++i)
                WriteHeaderRealmUpdate(writer, realms[i], clientIsLoopback);
            WriteHeaderRealmListComplete(writer);
            WriteMode1RealmListComplete(writer);
        }
        else
        {
            WriteMode1RealmListStart(writer);
            for (size_t i = 0; i < realms.size(); ++i)
                WriteMode1RealmUpdate(writer, realms[i], clientIsLoopback);
            WriteMode1RealmListComplete(writer);
        }
        return FinishSoftPacket(writer);
    }

    std::vector<std::vector<uint8>> BuildMode1RealmUpdatesFromRealmList(bool clientIsLoopback)
    {
        WaitAuthnetRealmReachability();
        ApplyAuthnetRealmListFromProbes(true);
        std::vector<std::vector<uint8>> packets;
        std::vector<Realm> const realms = CollectAuthnetRealms();
        for (size_t i = 0; i < realms.size(); ++i)
            packets.push_back(BuildMode1RealmUpdate(realms[i], clientIsLoopback));
        return packets;
    }

    // Soft mode2 cmd2 (E24C40): 1-bit Update, E24620 RealmInfo, then E235A0
    // Handle (8-bit region, 12-bit skip, 8-bit site, 32-bit id). B38E30
    // copies that into a WoW method-0 Invoke and B38510 delivers it.
    void WriteMode2Command2RealmUpdate(Skyfire::Authnet::BitWriter& writer, Realm const& realm,
        bool clientIsLoopback)
    {
        uint32 region = 0;
        uint32 site = 0;
        GetRealmHandleIds(region, site);
        uint32 const realmId = GetEnvUInt32("AUTHNET_REALM_ID", realm.m_ID ? realm.m_ID : 1);
        uint32 const infoFlags = GetAuthnetRealmInfoFlags(realm);
        uint32 const stateFlags = GetAuthnetRealmOfflineStateFlags(realm);
        int32 const realmType = int32(GetEnvUInt32("AUTHNET_REALM_TYPE", realm.icon));
        uint32 const category = GetEnvUInt32("AUTHNET_REALM_CATEGORY", realm.timezone);

        std::string name = GetEnvOrDefault("AUTHNET_REALM_NAME", realm.name.c_str());
        if (name.empty())
            name = "SkyFire";
        if (name.size() > 0x3FC)
            name.resize(0x3FC);

        writer.WriteBits(0x02, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x02, 4);

        writer.WriteBits(1, 1); // Update
        writer.WriteUInt32(category);
        WriteFloatBits(writer, realm.populationLevel);
        writer.WriteBits(stateFlags, 8);
        writer.WriteBits(0, 19);
        writer.WriteUInt32(uint32(realmType) + 0x80000000u);
        writer.WriteBits(uint32(name.size()), 10);
        writer.WriteBytes(name.data(), name.size());
        if (ShouldIncludeRealmPrivilegedData())
            WriteRealmPrivilegedData(writer, realm, clientIsLoopback);
        else
            writer.WriteBits(0, 1);
        writer.WriteBits(infoFlags, 8);

        writer.WriteBits(region, 8);
        writer.WriteBits(0, 12);
        writer.WriteBits(site, 8);
        writer.WriteUInt32(realmId);
    }

    std::vector<uint8> BuildMode2Command2RealmUpdate(Realm const& realm, bool clientIsLoopback)
    {
        Skyfire::Authnet::BitWriter writer;
        WriteMode2Command2RealmUpdate(writer, realm, clientIsLoopback);
        return FinishSoftPacket(writer);
    }

    std::vector<std::vector<uint8>> BuildMode2RealmUpdatesFromRealmList(bool clientIsLoopback)
    {
        WaitAuthnetRealmReachability();
        ApplyAuthnetRealmListFromProbes(true);
        std::vector<std::vector<uint8>> packets;
        std::vector<Realm> const realms = CollectAuthnetRealms();
        for (size_t i = 0; i < realms.size(); ++i)
            packets.push_back(BuildMode2Command2RealmUpdate(realms[i], clientIsLoopback));
        return packets;
    }

    std::vector<uint8> BuildEmptyRequestResult(uint32 requestId)
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x09, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x0B, 4);
        writer.WriteBits(0, 16);
        writer.WriteBits(0, 16);
        writer.WriteUInt32(requestId);
        writer.WriteBits(0, 6);
        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildResourceRequestResult(uint32 requestId, std::vector<uint8> const* resourceKey, uint32 resourceItemId)
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x09, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x0B, 4);
        writer.WriteBits(0, 16);
        writer.WriteBits(0, 16);
        writer.WriteUInt32(requestId);

        bool hasResourceItem = resourceKey && resourceKey->size() == 40;
        writer.WriteBits(hasResourceItem ? 1 : 0, 6);
        if (hasResourceItem)
        {
            writer.WriteBytes(resourceKey->data(), resourceKey->size());
            writer.WriteBits(0, 27);
            writer.WriteUInt32(resourceItemId + 0x80000000u);
        }

        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildServiceResultPayload(ServiceRequestInfo const& request, ServiceResultProbeInfo* probeInfo,
        char const* recordModeOverride = nullptr)
    {
        Skyfire::Authnet::BitWriter writer;

        // Optional service blob: the bit after the leading uint32 is present.
        // Writing the 22-byte key with present=0 shifts every later field (client
        // hangs after a structurally "successful" send). Live post-proof wants
        // present=1 + Auth/Prf (see AUTHNET_SERVICE_RECORD_MODE).
        char const* presentMode = GetEnvOrDefault("AUTHNET_SERVICE_RESULT_PRESENT", "1");
        bool present = !StringEquals(presentMode, "0") && !StringEquals(presentMode, "false") &&
            !StringEquals(presentMode, "off") && !StringEquals(presentMode, "none");

        // Soft-finish client sends Soft cmd9 mode1 for this request. Answering
        // Soft cmd3 mode11 (legacy C303) after CachedGameAccount produced
        // REALM_LIST_FAILED. Default match the request Soft header (C901…).
        // AUTHNET_SERVICE_RESULT_SOFT=c303 restores the old Soft cmd3 mode11.
        char const* softMode = GetEnvOrDefault("AUTHNET_SERVICE_RESULT_SOFT", "c901");
        if (StringEquals(softMode, "c303") || StringEquals(softMode, "cmd3") || StringEquals(softMode, "mode11"))
        {
            writer.WriteBits(0x03, 6);
            writer.WriteBits(1, 1);
            writer.WriteBits(0x0B, 4);
        }
        else
        {
            writer.WriteBits(0x09, 6);
            writer.WriteBits(1, 1);
            writer.WriteBits(0x01, 4);
        }
        writer.WriteUInt32(0);
        writer.WriteBits(present ? 1 : 0, 1);

        std::vector<uint8> servicePayloadProbe = BuildServicePayloadProbe(request, probeInfo, recordModeOverride);
        if (present)
            writer.WriteBytes(servicePayloadProbe.data(), servicePayloadProbe.size());
        writer.WriteBits(0, 9);
        writer.WriteUInt32(probeInfo ? probeInfo->field80 : 0);
        writer.WriteBits(0, 1);
        writer.WriteUInt32(request.requestId);

        writer.AlignToByte();
        return writer.Data();
    }

    void ApplyStreamCipher(uint8 state[256], uint8& i, uint8& j, std::vector<uint8>& payload)
    {
        for (size_t pos = 0; pos < payload.size(); ++pos)
        {
            ++i;
            uint8 si = state[i];
            j = uint8(j + si);
            uint8 sj = state[j];
            state[i] = sj;
            state[j] = si;
            payload[pos] ^= state[uint8(state[i] + state[j])];
        }
    }

    // Soft finish B53940 → B75150/B751B0 → C175F0: RC4 key =
    // HMAC-SHA256(sessionKey64, directionSalt16). Salts from unk_14100DD10/DD20.
    constexpr uint8 SoftFinishClientToServerSalt[16] = {
        0x68, 0xE0, 0xC7, 0x2E, 0xDD, 0xD6, 0xD2, 0xF3,
        0x1E, 0x5A, 0xB1, 0x55, 0xB1, 0x8B, 0x63, 0x1E
    };
    constexpr uint8 SoftFinishServerToClientSalt[16] = {
        0xDE, 0xA9, 0x65, 0xAE, 0x54, 0x3A, 0x1E, 0x93,
        0x9E, 0x69, 0x0C, 0xAA, 0x68, 0xDE, 0x78, 0x39
    };

    void InitRC4State(uint8 state[256], uint8& i, uint8& j, uint8 const* key, size_t keyLen)
    {
        for (uint32 n = 0; n < 256; ++n)
            state[n] = uint8(n);

        uint8 acc = 0;
        for (uint32 n = 0; n < 256; ++n)
        {
            acc = uint8(acc + state[n] + key[n % keyLen]);
            uint8 tmp = state[n];
            state[n] = state[acc];
            state[acc] = tmp;
        }

        i = 0;
        j = 0;
    }

    ProbePacketHeader DecodeProbePacketHeader(std::vector<uint8> const& packet)
    {
        ProbePacketHeader header;
        Skyfire::Authnet::BitReader reader(packet.data(), packet.size());

        if (!reader.ReadBits(6, header.command) || !reader.ReadBits(1, header.modeSwitch))
            return header;

        if (header.modeSwitch && !reader.ReadBits(4, header.mode))
            return header;

        header.bitPosition = reader.BitPosition();
        header.decoded = true;
        return header;
    }

    bool TryDecodePostSuccessRequestId(std::vector<uint8> const& packet, uint32& requestId)
    {
        Skyfire::Authnet::BitReader reader(packet.data(), packet.size());

        ProbePacketHeader header;
        if (!reader.ReadBits(6, header.command) || !reader.ReadBits(1, header.modeSwitch))
            return false;

        if (!header.modeSwitch || !reader.ReadBits(4, header.mode))
            return false;

        if (header.command != 9 || header.mode != 11)
            return false;

        uint32 requestCheck = 0;
        if (!reader.ReadBits(31, requestCheck) || !reader.ReadUInt32(requestId))
            return false;

        return true;
    }

    bool TryDecodeSelectedRealmSeed(std::vector<uint8> const& packet, uint32& connectionSeed)
    {
        Skyfire::Authnet::BitReader reader(packet.data(), packet.size());

        ProbePacketHeader header;
        if (!reader.ReadBits(6, header.command) || !reader.ReadBits(1, header.modeSwitch))
            return false;

        if (!header.modeSwitch || !reader.ReadBits(4, header.mode))
            return false;

        if (header.command != 8 || header.mode != 2)
            return false;

        return reader.ReadUInt32(connectionSeed);
    }

    bool TryDecodeResourceLookup(std::vector<uint8> const& packet, ResourceLookupInfo& info)
    {
        using namespace Skyfire::Authnet;

        BitReader reader(packet.data(), packet.size());
        if (!reader.ReadBits(6, info.header.command) || !reader.ReadBits(1, info.header.modeSwitch))
            return false;

        if (info.header.modeSwitch && !reader.ReadBits(4, info.header.mode))
            return false;

        info.header.bitPosition = reader.BitPosition();
        info.header.decoded = true;

        if (info.header.command != 9 || !info.header.modeSwitch || info.header.mode != 11)
            return false;

        uint32 requestTypeMinusOne = 0;
        if (!reader.ReadBits(31, info.marker) || !reader.ReadUInt32(info.requestId) ||
            !reader.ReadUInt32(info.parentRaw) || !reader.ReadBits(1, info.optionalFlag) ||
            !reader.ReadBits(6, requestTypeMinusOne) || !reader.ReadUInt32(info.localeValue) ||
            !reader.ReadBits(1, info.variant))
            return false;

        info.requestType = requestTypeMinusOne + 1;
        info.locale = FourCCFromUInt32(info.localeValue);

        if (info.variant == 1)
        {
            if (!reader.ReadUInt32(info.payloadFirst) || !reader.ReadUInt32(info.payloadSecond))
                return false;
        }
        else
        {
            if (!reader.ReadBits(16, info.payloadWord))
                return false;
        }

        info.remainingBits = reader.RemainingBits();
        info.bitLength = reader.BitPosition();
        info.packetLength = (info.bitLength + 7) / 8;
        return true;
    }

    bool TryGetConfiguredResourceResult(ResourceLookupInfo const& lookup, std::vector<uint8>& resourceKey, uint32& resourceItemId, std::string& reason)
    {
        char const* mode = GetEnvOrDefault("AUTHNET_RESOURCE_RESULT_MODE", "empty");
        if (!StringEquals(mode, "single"))
            return false;

        char const* payload0Filter = std::getenv("AUTHNET_RESOURCE_RESULT_PAYLOAD0");
        if (payload0Filter && payload0Filter[0] && FourCCFromUInt32(lookup.payloadFirst) != payload0Filter)
        {
            reason = "payload0-filter";
            return false;
        }

        char const* localeFilter = std::getenv("AUTHNET_RESOURCE_RESULT_LOCALE");
        if (localeFilter && localeFilter[0] && lookup.locale != localeFilter)
        {
            reason = "locale-filter";
            return false;
        }

        if (!TryParseHexBytes(std::getenv("AUTHNET_RESOURCE_RESULT_KEY"), 40, resourceKey))
        {
            reason = "bad-key";
            return false;
        }

        resourceItemId = GetEnvUInt32("AUTHNET_RESOURCE_RESULT_ID", 0);
        reason = "single";
        return true;
    }

    bool TryReadPacketHeader(Skyfire::Authnet::BitReader& reader, ProbePacketHeader& header)
    {
        if (!reader.ReadBits(6, header.command) || !reader.ReadBits(1, header.modeSwitch))
            return false;

        if (header.modeSwitch && !reader.ReadBits(4, header.mode))
            return false;

        header.bitPosition = reader.BitPosition();
        header.decoded = true;
        return true;
    }

    bool TryDecodeServiceRequest(std::vector<uint8> const& packet, ServiceRequestInfo& info)
    {
        using namespace Skyfire::Authnet;

        BitReader reader(packet.data(), packet.size());
        if (!TryReadPacketHeader(reader, info.header))
            return false;

        if (info.header.command != 9 || !info.header.modeSwitch || info.header.mode != 1)
            return false;

        uint32 recordCount = 0;
        if (!reader.ReadBits(6, recordCount) || recordCount > 0x28)
            return false;

        info.records.clear();
        info.records.reserve(recordCount);

        for (uint32 i = 0; i < recordCount; ++i)
        {
            ServiceRequestRecord record;
            if (!reader.ReadFourCC(record.component) || !reader.ReadUInt32(record.build) ||
                !reader.ReadBits(16, record.major) || !reader.ReadFourCC(record.program) ||
                !reader.ReadBits(16, record.minor))
                return false;

            info.records.push_back(record);
        }

        uint32 selectorMinusOne = 0;
        if (!reader.ReadBits(4, selectorMinusOne))
            return false;

        info.selector = selectorMinusOne + 1;

        uint32 labelLength = 0;
        if (!reader.ReadBits(8, labelLength) || labelLength > 0x80)
            return false;

        reader.AlignToByte();
        if (labelLength != 0)
        {
            info.label.resize(labelLength);
            if (!reader.ReadBytes(&info.label[0], labelLength))
            {
                info.label.clear();
                return false;
            }
        }

        uint32 hasNestedHeader = 0;
        if (!reader.ReadBits(1, hasNestedHeader))
            return false;

        info.hasNestedHeader = hasNestedHeader != 0;
        if (info.hasNestedHeader && !TryReadPacketHeader(reader, info.nestedHeader))
            return false;

        if (!reader.ReadUInt32(info.requestId))
            return false;

        info.bitLength = reader.BitPosition();
        info.packetLength = (info.bitLength + 7) / 8;
        return true;
    }

    // Soft bounce trailer: 9200 or 9300 + quoted server Soft plains
    // (Start 4601 / Update 4400 / CGA). Live :56778/:56795 used 9200.
    // Answering further service requests fuels a Cach/News storm.
    bool LooksLikeSoftFrameBounce(std::vector<uint8> const& plain)
    {
        // :56463 quoted CGA+Start+Updates inside a service request with no
        // 9300 trailer. Treat any service-sized uplink that embeds our Soft
        // plains as a bounce so we do not answer Cach/News.
        // Quoted CGA plains: Hard status 400a00, empty-list 400200, single-list
        // 400201…, plus legacy Larva-name guesses (401A…/411A…/402A…WoW1).
        static uint8 const cgaStatus[] = { 0x40, 0x0A, 0x00 };
        static uint8 const cgaEmpty[] = { 0x40, 0x02, 0x00 };
        static uint8 const cgaList[] = { 0x40, 0x02, 0x01 };
        static uint8 const cgaBody[] = { 0x57, 0x6F, 0x57, 0x31, 0x01, 0x03, 0x57, 0x6F, 0x57, 0x31 };
        static uint8 const cgaCmd0[] = { 0x40, 0x1A, 0x01, 0x57, 0x6F, 0x57 };
        static uint8 const cgaCmd1[] = { 0x41, 0x1A, 0x01, 0x57, 0x6F, 0x57 };
        static uint8 const cgaNameOld[] = { 0x40, 0x2A, 0x00, 0x57, 0x6F, 0x57 };
        static uint8 const realmStart[] = { 0x46, 0x01, 0x03 };
        static uint8 const realmUpdate[] = { 0x44, 0x00, 0x01, 0x01 };
        static uint8 const realmComplete[] = { 0x46, 0x09 };
        // :63405 quoted CHOICE 1 cmd8 Failure (480A01) inside a 445 B
        // Auth/Lgon service storm, then closed. Do not answer that uplink.
        static uint8 const cmd8StatusFail[] = { 0x48, 0x0A, 0x01 };
        static uint8 const cmd8StatusZero[] = { 0x48, 0x0A, 0x00 };
        if (std::search(plain.begin(), plain.end(), cgaStatus, cgaStatus + sizeof(cgaStatus)) != plain.end() ||
            std::search(plain.begin(), plain.end(), cgaEmpty, cgaEmpty + sizeof(cgaEmpty)) != plain.end() ||
            std::search(plain.begin(), plain.end(), cgaList, cgaList + sizeof(cgaList)) != plain.end() ||
            std::search(plain.begin(), plain.end(), cgaBody, cgaBody + sizeof(cgaBody)) != plain.end() ||
            std::search(plain.begin(), plain.end(), cgaCmd0, cgaCmd0 + sizeof(cgaCmd0)) != plain.end() ||
            std::search(plain.begin(), plain.end(), cgaCmd1, cgaCmd1 + sizeof(cgaCmd1)) != plain.end() ||
            std::search(plain.begin(), plain.end(), cgaNameOld, cgaNameOld + sizeof(cgaNameOld)) != plain.end() ||
            std::search(plain.begin(), plain.end(), realmStart, realmStart + sizeof(realmStart)) != plain.end() ||
            std::search(plain.begin(), plain.end(), realmUpdate, realmUpdate + sizeof(realmUpdate)) != plain.end() ||
            std::search(plain.begin(), plain.end(), realmComplete, realmComplete + sizeof(realmComplete)) != plain.end() ||
            std::search(plain.begin(), plain.end(), cmd8StatusFail, cmd8StatusFail + sizeof(cmd8StatusFail)) != plain.end() ||
            std::search(plain.begin(), plain.end(), cmd8StatusZero, cmd8StatusZero + sizeof(cmd8StatusZero)) != plain.end())
            return true;

        static uint8 const bounceTag93[2] = { 0x93, 0x00 };
        static uint8 const bounceTag92[2] = { 0x92, 0x00 };
        auto bounce = std::search(plain.begin(), plain.end(), bounceTag92, bounceTag92 + 2);
        if (bounce == plain.end())
            bounce = std::search(plain.begin(), plain.end(), bounceTag93, bounceTag93 + 2);
        if (bounce == plain.end())
            return false;

        auto const after = bounce + 2;
        static uint8 const cgaStatusHdr[2] = { 0x40, 0x0A };
        static uint8 const cgaCmd0hdr[2] = { 0x40, 0x1A };
        static uint8 const cgaCmd1hdr[2] = { 0x41, 0x1A };
        static uint8 const cgaOld[2] = { 0x40, 0x2A };
        if (std::search(after, plain.end(), realmStart, realmStart + sizeof(realmStart)) != plain.end())
            return true;
        if (std::search(after, plain.end(), realmUpdate, realmUpdate + sizeof(realmUpdate)) != plain.end())
            return true;
        if (std::search(after, plain.end(), cgaStatusHdr, cgaStatusHdr + 2) != plain.end())
            return true;
        if (std::search(after, plain.end(), cgaCmd0hdr, cgaCmd0hdr + 2) != plain.end())
            return true;
        if (std::search(after, plain.end(), cgaCmd1hdr, cgaCmd1hdr + 2) != plain.end())
            return true;
        if (std::search(after, plain.end(), cgaOld, cgaOld + 2) != plain.end())
            return true;
        if (std::search(after, plain.end(), cmd8StatusFail, cmd8StatusFail + sizeof(cmd8StatusFail)) != plain.end())
            return true;
        if (std::search(after, plain.end(), cmd8StatusZero, cmd8StatusZero + sizeof(cmd8StatusZero)) != plain.end())
            return true;
        return false;
    }

    void TrimTrailingNulls(std::string& value)
    {
        while (!value.empty() && value[value.size() - 1] == '\0')
            value.erase(value.size() - 1);
    }

    bool TryDecodeInitialRequest(std::vector<uint8> const& captured, InitialRequestInfo& info)
    {
        using namespace Skyfire::Authnet;

        BitReader reader(captured.data(), captured.size());

        if (!reader.ReadBits(6, info.header.command) || !reader.ReadBits(1, info.header.modeSwitch))
            return false;

        if (info.header.modeSwitch && !reader.ReadBits(4, info.header.mode))
            return false;

        info.header.bitPosition = reader.BitPosition();
        info.header.decoded = true;

        if (info.header.command != InitialLoginCommand || !info.header.modeSwitch)
            return false;

        uint32 componentCount = 0;
        if (!reader.ReadFourCC(info.program) || !reader.ReadFourCC(info.platform) ||
            !reader.ReadFourCC(info.locale) || !reader.ReadBits(6, componentCount))
            return false;

        info.components.clear();
        info.components.reserve(componentCount);

        for (uint32 i = 0; i < componentCount; ++i)
        {
            InitialComponent component;
            if (!reader.ReadFourCC(component.program) || !reader.ReadFourCC(component.platform) ||
                !reader.ReadUInt32(component.build))
                return false;

            info.components.push_back(component);
        }

        uint32 hasIdentity = 0;
        if (!reader.ReadBits(1, hasIdentity))
            return false;

        info.hasIdentity = hasIdentity != 0;
        if (info.hasIdentity)
        {
            uint32 identityLengthMinus3 = 0;
            if (!reader.ReadBits(9, identityLengthMinus3))
                return false;

            info.identityLength = identityLengthMinus3 + 3;
            if (info.identityLength > MaxInitialIdentityBytes)
                return false;

            if (!reader.ReadString(info.identityLength, info.identity))
                return false;

            TrimTrailingNulls(info.identity);
        }

        if (!reader.ReadUInt64(info.tailValue))
            return false;

        info.bitLength = reader.BitPosition();
        info.packetLength = ByteLengthForBits(info.bitLength);
        return true;
    }

    bool CapturedStartsWith(std::vector<uint8> const& bytes, char const* prefix)
    {
        size_t prefixLen = std::strlen(prefix);
        return bytes.size() >= prefixLen && std::memcmp(bytes.data(), prefix, prefixLen) == 0;
    }

    bool LooksLikeHttpRequest(std::vector<uint8> const& bytes)
    {
        return CapturedStartsWith(bytes, "GET ") ||
            CapturedStartsWith(bytes, "HEAD ") ||
            CapturedStartsWith(bytes, "POST ") ||
            CapturedStartsWith(bytes, "OPTIONS ");
    }

    bool TryGetHttpRequestTarget(std::vector<uint8> const& bytes, std::string& target)
    {
        std::vector<uint8>::const_iterator methodEnd = std::find(bytes.begin(), bytes.end(), ' ');
        if (methodEnd == bytes.end())
            return false;

        std::vector<uint8>::const_iterator targetStart = methodEnd + 1;
        std::vector<uint8>::const_iterator targetEnd = std::find(targetStart, bytes.end(), ' ');
        if (targetEnd == bytes.end() || targetStart == targetEnd)
            return false;

        target.assign(targetStart, targetEnd);
        return true;
    }

    bool IsSafeLocalHttpFileName(std::string const& fileName)
    {
        if (fileName.empty() || fileName.find("..") != std::string::npos)
            return false;

        return std::all_of(fileName.begin(), fileName.end(), [](char c)
        {
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                (c >= '0' && c <= '9') || c == '.' || c == '-' || c == '_';
        });
    }

    bool TryLoadLocalWowPodFile(std::string const& target, std::string& body, std::string& filePath)
    {
        char constexpr Prefix[] = "/wow-pod/";
        size_t constexpr PrefixLen = sizeof(Prefix) - 1;

        if (target.compare(0, PrefixLen, Prefix) != 0)
            return false;

        std::string fileName = target.substr(PrefixLen);
        size_t queryOffset = fileName.find('?');
        if (queryOffset != std::string::npos)
            fileName.erase(queryOffset);

        if (!IsSafeLocalHttpFileName(fileName))
            return false;

        filePath = "authnet/wow-pod/" + fileName;

        std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
        if (!file)
            return false;

        body.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        return true;
    }

    std::string BuildHttpResponse(char const* contentType, std::string const& body)
    {
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: ";
        response += contentType;
        response += "\r\nContent-Length: ";
        response += std::to_string(body.size());
        response += "\r\nConnection: close\r\n\r\n";
        response += body;
        return response;
    }

    std::string BuildHttpResponse(char const* contentType, char const* body)
    {
        return BuildHttpResponse(contentType, std::string(body));
    }
}

AuthnetSocket::AuthnetSocket(RealmSocket& socket) :
    socket_(socket), _encryptedBytesProcessed(0), _initialRequestLen(0), _soft2UplinkLen(0),
    _postProofEncryptedOffset(0), _authnetAccountId(0), _authnetLocaleId(0),
    _authnetWorldConnectionSeed(0), _authnetWorldRealmField(0),
    _authnetSelectedRealmField(0), _authnetLoginCompleteRealmField(0),
    _authnetWorldSessionKey(), _authnetSecret(), _authnetWorldSessionKeyGenerated(false),
    _authnetWorldSessionKeyPersisted(false), _authnetSecretReady(false),
    _softFinishContinuation(false), _clientCryptI(0), _clientCryptJ(0),
    _clientCryptInitialized(false), _serverCryptI(0), _serverCryptJ(0), _serverCryptInitialized(false),
    _responded(false), _httpResponded(false), _clientModeSwitchSeen(false), _followupLogged(false),
    _postSuccessBurstSeen(false), _moduleProofAnswered(false), _soft2EmptySeen(false),
    _mode1ConnectAnswered(false),
    _mode2LoginAnswered(false), _mode2Command2Answered(false), _mode2Command3Answered(false),
    _mode2RealmUpdatesSent(false), _connect4601CompleteSent(false),
    _mode2Command8Answered(false), _mode2Command8JoinAnswered(false),
    _mode2Command8JoinSuccess(false),
    _postLoginStatusSent(false), _mode1Command6Answered(false), _mode1Command7Answered(false),
    _softBounceSeen(false), _accountId(0), _realmListFrameIndex(0)
{
}

AuthnetSocket::~AuthnetSocket(void)
{
    if (_realmListFrameTimer)
        _realmListFrameTimer->cancel();
}

void AuthnetSocket::OnAccept(void)
{
    // Live 51891: GET /agent after 50ms, so any greeting poisoned Agent.
    // After pending is published, Agent is still 77-byte HTTP — do not log
    // those as hop candidates. Real Sunken is the empty TCP (OnClose).
}

void AuthnetSocket::PublishPendingSoftFinishSession()
{
    if (!_sessionKey || !_authnetSecretReady)
    {
        SF_LOG_ERROR("server.authserver",
            "'%s:%d' authnet probe: cannot publish Soft-finish pending; SoftK64 missing",
            socket().getRemoteAddress().c_str(), socket().getRemotePort());
        return;
    }

    SoftFinishPendingSession pending;
    pending.remoteAddress = socket().getRemoteAddress();
    pending.accountId = _authnetAccountId ? _authnetAccountId : _accountId;
    pending.accountName = _authnetAccountName;
    pending.loginIdentity = _loginIdentity;
    pending.worldToken = _authnetWorldAccountToken.empty()
        ? BuildWorldAccountToken(pending.accountId) : _authnetWorldAccountToken;
    pending.clientOs = _clientOs;
    pending.clientLocale = _clientLocale;
    pending.localeId = _authnetLocaleId;
    pending.os = _authnetOS;
    pending.sessionKey = *_sessionKey;
    pending.authnetSecret = _authnetSecret;
    pending.authnetSecretReady = _authnetSecretReady;
    pending.expiresAt = std::chrono::steady_clock::now() +
        std::chrono::seconds(SoftFinishPendingTtlSeconds);

    {
        std::lock_guard<std::mutex> lock(g_softFinishPendingMutex);
        PurgeExpiredSoftFinishPendingLocked(std::chrono::steady_clock::now());
        // One pending Soft finish per client IP.
        g_softFinishPending.erase(
            std::remove_if(g_softFinishPending.begin(), g_softFinishPending.end(),
                [&pending](SoftFinishPendingSession const& entry)
                {
                    return entry.remoteAddress == pending.remoteAddress;
                }),
            g_softFinishPending.end());
        g_softFinishPending.push_back(pending);
    }

    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet probe: published Soft-finish pending account=%u token=%s ttl=%u",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
        pending.accountId, pending.worldToken.c_str(), SoftFinishPendingTtlSeconds);
}

bool AuthnetSocket::TryAdoptPendingSoftFinishSession()
{
    if (_softFinishContinuation || _sessionKey)
        return false;

    SoftFinishPendingSession pending;
    bool found = false;
    {
        std::lock_guard<std::mutex> lock(g_softFinishPendingMutex);
        auto const now = std::chrono::steady_clock::now();
        PurgeExpiredSoftFinishPendingLocked(now);
        std::string const remote = socket().getRemoteAddress();
        for (auto it = g_softFinishPending.begin(); it != g_softFinishPending.end(); ++it)
        {
            if (it->remoteAddress != remote)
                continue;
            pending = *it;
            g_softFinishPending.erase(it);
            found = true;
            break;
        }
    }

    if (!found)
        return false;

    _softFinishContinuation = true;
    _sessionKey = pending.sessionKey;
    _authnetSecret = pending.authnetSecret;
    _authnetSecretReady = pending.authnetSecretReady;
    _authnetAccountId = pending.accountId;
    _accountId = pending.accountId;
    _authnetAccountName = pending.accountName;
    _loginIdentity = pending.loginIdentity;
    _authnetWorldAccountToken = pending.worldToken;
    _clientOs = pending.clientOs;
    _clientLocale = pending.clientLocale;
    _authnetLocaleId = pending.localeId;
    _authnetOS = pending.os;
    _moduleProofAnswered = true; // Soft2 already completed on the first TCP
    // Do not InitSoftFinishCrypt yet — the second TCP may start with a
    // plaintext Soft cmd=9. RC4 starts on 4501 like the same-socket path.

    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet probe: adopted Soft-finish continuation account=%u token=%s SoftK ready",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
        _authnetAccountId, _authnetWorldAccountToken.c_str());
    return true;
}

void AuthnetSocket::OnClose(void)
{
    if (_realmListFrameIndex < _realmListFrames.size())
        SF_LOG_INFO("server.authserver",
            "'%s:%d' authnet probe: connection closed with realmlist frames left %zu/%zu",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(),
            _realmListFrameIndex, _realmListFrames.size());

    if (_realmListFrameTimer)
        _realmListFrameTimer->cancel();

    if (_captured.empty() && !_responded)
    {
        if (HasPendingSoftFinishForRemote(socket().getRemoteAddress()))
            SF_LOG_INFO("server.authserver",
                "'%s:%d' authnet probe: empty TCP closed while Soft-finish pending (hop connected but sent no bytes)",
                socket().getRemoteAddress().c_str(), socket().getRemotePort());
        return;
    }

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet passive probe: connection closed, %zu byte(s) captured total, responded=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), _captured.size(),
        _responded ? "yes" : "no");
}

void AuthnetSocket::ScheduleRealmListFrames(std::vector<std::pair<std::string, std::vector<uint8>>> frames)
{
    _realmListFrames.clear();
    _realmListFrameIndex = 0;
    for (size_t i = 0; i < frames.size(); ++i)
    {
        SF_LOG_INFO("server.authserver",
            "'%s:%d' authnet probe: queue realmlist frame[%zu] %s plain=%s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), i,
            frames[i].first.c_str(), ByteArrayToHexStr(frames[i].second).c_str());
        CryptServerPayload(frames[i].second);
        _realmListFrames.push_back(std::move(frames[i]));
    }

    SendNextRealmListFrame();
}

void AuthnetSocket::SendNextRealmListFrame()
{
    if (_realmListFrameIndex >= _realmListFrames.size())
        return;

    std::pair<std::string, std::vector<uint8>> const& frame = _realmListFrames[_realmListFrameIndex++];
    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet probe: sending encrypted mode1 %s encrypted=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
        frame.first.c_str(), ByteArrayToHexStr(frame.second).c_str());
    socket().QueueSend(reinterpret_cast<char const*>(frame.second.data()), frame.second.size());

    if (_realmListFrameIndex >= _realmListFrames.size())
        return;

    uint32 const delayMs = GetRealmListFrameDelayMs();
    if (delayMs == 0)
    {
        SendNextRealmListFrame();
        return;
    }

    if (!_realmListFrameTimer)
        _realmListFrameTimer.reset(new boost::asio::steady_timer(socket().GetExecutor()));

    _realmListFrameTimer->expires_after(std::chrono::milliseconds(delayMs));
    std::shared_ptr<RealmSocket> sock = socket().shared_from_this();
    _realmListFrameTimer->async_wait([this, sock](boost::system::error_code const& error)
    {
        if (error)
            return;
        SendNextRealmListFrame();
    });
}

void AuthnetSocket::OnRead(void)
{
    size_t available = socket().GetAvailableBytes();
    if (available == 0)
        return;

    if (_captured.size() + available > MaxCapturedBytes)
        available = MaxCapturedBytes - _captured.size();

    if (available == 0)
    {
        SF_LOG_ERROR("server.authserver", "'%s:%d' authnet passive probe: hit the %zu byte capture cap, closing",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), MaxCapturedBytes);
        socket().Close();
        return;
    }

    size_t offset = _captured.size();
    _captured.resize(offset + available);
    socket().ReadBytes(_captured.data() + offset, available);

    std::vector<uint8> newBytes(_captured.begin() + offset, _captured.end());

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet passive probe: %zu new byte(s), %zu total so far, new=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), available, _captured.size(),
        ByteArrayToHexStr(newBytes).c_str());

    TrySendProbeResponse(offset, available);
}

bool AuthnetSocket::DecodeInitialRequest(void)
{
    if (_initialRequestLen != 0)
        return true;

    InitialRequestInfo request;
    if (!TryDecodeInitialRequest(_captured, request))
        return false;

    _initialRequestLen = request.packetLength;

    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet decode: initial_header command=%u mode_switch=%u mode=%u header_bits=%zu program=%s platform=%s locale=%s components=%zu",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
        request.header.command, request.header.modeSwitch, request.header.mode, request.header.bitPosition,
        request.program.c_str(), request.platform.c_str(), request.locale.c_str(), request.components.size());

    for (size_t i = 0; i < request.components.size(); ++i)
    {
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet decode: component[%u] = %s.%s.%u",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(),
            uint32(i), request.components[i].program.c_str(), request.components[i].platform.c_str(),
            request.components[i].build);
    }

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet decode: has_identity=%s identity_len=%u identity=%s tail_value=" UI64FMTD " initial_request_len=%zu initial_bits=%zu",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
        request.hasIdentity ? "yes" : "no", request.identityLength, request.identity.c_str(),
        request.tailValue, _initialRequestLen, request.bitLength);

    if (request.hasIdentity && !request.identity.empty())
        _loginIdentity = request.identity;
    _clientLocale = request.locale.empty() ? "enUS" : request.locale;
    // Auth.account.os is Win/OSX; Soft platform is Wn64/OSX.
    if (request.platform == "Wn64" || request.platform == "Win" || request.platform == "Wn32")
        _clientOs = "Win";
    else if (request.platform == "OSX" || request.platform == "Mc64")
        _clientOs = "OSX";
    else
        _clientOs = request.platform.empty() ? "Win" : request.platform;

    if (request.hasIdentity)
        PrepareWorldSessionKey(request.identity, request.platform, request.locale);

    return true;
}

void AuthnetSocket::PrepareWorldSessionKey(std::string const& identity, std::string const& platform, std::string const& locale)
{
    // Soft path: only capture account context here. Do not derive/persist a
    // world key until Soft2 K64 is known — HMAC with a zero secret is wrong.
    uint32 accountId = 0;
    std::string accountName;
    if (!TryResolveAuthnetLoginIdentity(identity, accountId, accountName))
    {
        SF_LOG_ERROR("server.authserver", "'%s:%d' authnet probe: identity '%s' did not resolve to an account.",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), identity.c_str());
        return;
    }

    _authnetAccountId = accountId;
    _accountId = accountId;
    _authnetAccountName = accountName;
    _authnetWorldAccountToken = BuildWorldAccountToken(accountId);
    _authnetLocaleId = GetLocaleByName(locale);
    _authnetOS = GetAuthnetOSFromPlatform(platform);
    _loginIdentity = identity;
    _clientLocale = locale;
    if (_clientOs.empty())
        _clientOs = _authnetOS;

    _authnetSelectedRealmField = ResolveAuthnetSelectedRealmField(accountId);
    _authnetLoginCompleteRealmField = _authnetSelectedRealmField;
}

void AuthnetSocket::PersistAuthnetWorldSessionKey(uint32 connectionSeed, uint32 realmField, char const* reason)
{
    PersistAuthnetWorldSessionKey(connectionSeed, realmField, realmField, reason);
}

void AuthnetSocket::PersistAuthnetWorldSessionKey(uint32 connectionSeed, uint32 realmField, uint32 selectedRealmField, char const* reason)
{
    if (!_authnetAccountId || _authnetWorldAccountToken.empty())
    {
        SF_LOG_ERROR("server.authserver", "'%s:%d' authnet probe: cannot persist world session key before account context is ready.",
            socket().getRemoteAddress().c_str(), socket().getRemotePort());
        return;
    }

    if (!_authnetSecretReady)
    {
        SF_LOG_INFO("server.authserver",
            "'%s:%d' authnet probe: skip world session_key persist (%s); SoftK64 secret not ready",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), reason ? reason : "authnet");
        return;
    }

    _authnetWorldConnectionSeed = connectionSeed;
    _authnetWorldRealmField = realmField;
    if (selectedRealmField)
        _authnetSelectedRealmField = selectedRealmField;
    _authnetWorldSessionKey = BuildAuthnetWorldSessionKey(_authnetSecret, connectionSeed, realmField);
    _authnetWorldSessionKeyGenerated = true;

    std::string const remoteAddress = socket().getRemoteAddress();

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_LOGONPROOF_BY_ID);
    stmt->setBinary(0, _authnetWorldSessionKey);
    stmt->setString(1, remoteAddress);
    stmt->setUInt32(2, _authnetLocaleId);
    stmt->setString(3, _authnetOS);
    stmt->setUInt32(4, _authnetAccountId);
    LoginDatabase.DirectExecute(stmt);

    // Do not DELETE expired rows on Soft persist: soft2-proof and join Soft-ACK
    // both REPLACE the same PK while an async table-wide DELETE races them →
    // InnoDB 1213. World lookup already filters expires_at > UNIX_TIMESTAMP().

    uint32 const sessionTtl = GetAuthnetWorldSessionTtlSeconds();
    stmt = LoginDatabase.GetPreparedStatement(LOGIN_REP_AUTHNET_WORLD_SESSION);
    stmt->setUInt32(0, _authnetAccountId);
    stmt->setString(1, _authnetWorldAccountToken);
    stmt->setBinary(2, _authnetWorldSessionKey);
    stmt->setBinary(3, _authnetSecret);
    stmt->setUInt32(4, connectionSeed);
    stmt->setUInt32(5, realmField);
    stmt->setUInt32(6, _authnetSelectedRealmField);
    stmt->setString(7, remoteAddress);
    stmt->setUInt32(8, _authnetLocaleId);
    stmt->setString(9, _authnetOS);
    stmt->setUInt32(10, sessionTtl);
    LoginDatabase.DirectExecute(stmt);

    _accountId = _authnetAccountId;
    _authnetWorldSessionKeyPersisted = true;

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: persisted %s world session key for account %u (%s), token=%s, seed=%u, realm_field=%u, selected_realm_field=%u, ttl=%u, key=%s.",
        remoteAddress.c_str(), socket().getRemotePort(), reason ? reason : "authnet",
        _authnetAccountId, _authnetAccountName.c_str(), _authnetWorldAccountToken.c_str(),
        connectionSeed, realmField, _authnetSelectedRealmField, sessionTtl, MaskSessionKey(_authnetWorldSessionKey).c_str());
}

bool AuthnetSocket::TryUpdateWorldSessionKeyFromSelectedRealm(std::vector<uint8> const& packet)
{
    uint32 connectionSeed = 0;
    if (!TryDecodeSelectedRealmSeed(packet, connectionSeed) &&
        !TryParseMode2Command8JoinConnectionSeed(packet, connectionSeed))
    {
        SF_LOG_ERROR("server.authserver", "'%s:%d' authnet probe: selected realm request did not contain a decodable connection seed, request=%s.",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), ByteArrayToHexStr(packet).c_str());
        return false;
    }

    uint32 joinRealmId = 0;
    TryParseMode2Command8JoinRealmId(packet, joinRealmId);
    uint32 const previousRealmField = GetAuthnetPreferredRealmField();
    uint32 const selectedRealmField = ResolveJoinRealmField(joinRealmId ? joinRealmId : previousRealmField);
    std::string const selectedRealmName = FindAuthnetRealmNameById(selectedRealmField);
    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet probe: selected realm request seed=%u selected_realm_field=%u selected_realm=%s previous_realm_field=%u source=%s.",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
        connectionSeed, selectedRealmField, selectedRealmName.empty() ? "<unknown>" : selectedRealmName.c_str(),
        previousRealmField, joinRealmId ? "packet" : "default");

    PersistAuthnetWorldSessionKey(connectionSeed, selectedRealmField, selectedRealmField, "selected-realm");
    return _authnetWorldSessionKeyPersisted;
}

uint32 AuthnetSocket::GetAuthnetPreferredRealmField() const
{
    return _authnetSelectedRealmField ? _authnetSelectedRealmField : GetConfiguredAuthnetDefaultRealmField();
}

void AuthnetSocket::CryptClientPayload(std::vector<uint8>& payload)
{
    if (!_clientCryptInitialized)
    {
        std::memcpy(_clientCryptState, InitialClientToServerCryptState, sizeof(_clientCryptState));
        _clientCryptI = 0;
        _clientCryptJ = 0;
        _clientCryptInitialized = true;
    }

    ApplyStreamCipher(_clientCryptState, _clientCryptI, _clientCryptJ, payload);
}

void AuthnetSocket::CryptServerPayload(std::vector<uint8>& payload)
{
    if (!_serverCryptInitialized)
    {
        std::memcpy(_serverCryptState, InitialServerToClientCryptState, sizeof(_serverCryptState));
        _serverCryptI = 0;
        _serverCryptJ = 0;
        _serverCryptInitialized = true;
    }

    ApplyStreamCipher(_serverCryptState, _serverCryptI, _serverCryptJ, payload);
}

void AuthnetSocket::InitSoftFinishCrypt(SkyFire::Authnet::PasswordSrp::SessionKey const& sessionKey)
{
    // Soft finish B53940 → B75150 (DD10) / B751B0 (DD20) → C175F0:
    // RC4_KSA(HMAC-SHA256(K64, salt16)). Live capture: DD20 decrypts C→S
    // (cmd=9 mode=11); DD10 is S→C. AUTHNET_SOFT_FINISH_CRYPT_SWAP=1 flips.
    bool swap = StringEquals(GetEnvOrDefault("AUTHNET_SOFT_FINISH_CRYPT_SWAP", "0"), "1") ||
        StringEquals(GetEnvOrDefault("AUTHNET_SOFT_FINISH_CRYPT_SWAP", "0"), "true");

    // Default: client decrypt ← DD20, server encrypt ← DD10.
    uint8 const* c2sSalt = swap ? SoftFinishClientToServerSalt : SoftFinishServerToClientSalt;
    uint8 const* s2cSalt = swap ? SoftFinishServerToClientSalt : SoftFinishClientToServerSalt;

    auto c2sKey = SkyFire::Crypto::HMAC_SHA256::GetDigestOf(sessionKey, c2sSalt, 16);
    auto s2cKey = SkyFire::Crypto::HMAC_SHA256::GetDigestOf(sessionKey, s2cSalt, 16);

    InitRC4State(_clientCryptState, _clientCryptI, _clientCryptJ, c2sKey.data(), c2sKey.size());
    InitRC4State(_serverCryptState, _serverCryptI, _serverCryptJ, s2cKey.data(), s2cKey.size());
    _clientCryptInitialized = true;
    _serverCryptInitialized = true;
    _encryptedBytesProcessed = 0;

    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet probe: Soft-finish RC4 from SRP K (swap=%s) c2s=%s s2c=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
        swap ? "yes" : "no",
        ByteArrayToHexStr(std::vector<uint8>(c2sKey.begin(), c2sKey.end())).c_str(),
        ByteArrayToHexStr(std::vector<uint8>(s2cKey.begin(), s2cKey.end())).c_str());
}

void AuthnetSocket::ProcessPostProofPlaintext(void)
{
    // After Soft2 op=3 + Soft0 finish (B537E0→B53940) the client emits Soft2
    // empty (4200) then Creep cmd=5 mode=1 (4501) RC4 mode-switch, then
    // encrypted follow-up. Without Soft0 it used to send Soft2 empty / 4001 /
    // cmd9 service and never entered encryption.
    if (!_moduleProofAnswered)
        return;
    if (_soft2UplinkLen == 0 && !_softFinishContinuation)
        return;

    if (_clientModeSwitchSeen && _postProofEncryptedOffset != 0)
    {
        ProcessEncryptedClientBytes(_postProofEncryptedOffset);
        return;
    }

    size_t offset = _initialRequestLen + _soft2UplinkLen;
    while (offset < _captured.size())
    {
        std::vector<uint8> slice(_captured.begin() + offset, _captured.end());
        size_t emptyLen = 0;
        if (TryDecodeSoft2Empty(slice, emptyLen))
        {
            if (!_soft2EmptySeen)
            {
                _soft2EmptySeen = true;
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: post-proof Soft2 empty (count=0) len=%zu plain=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    emptyLen, ByteArrayToHexStr(std::vector<uint8>(slice.begin(), slice.begin() + emptyLen)).c_str());

                // Soft0 empty sets Auth+9610 then B537E0 → B53940 Soft-finish.
                // Stock B537E0 Gets A2057DF4 (never Set); Soft-mode launcher
                // forces B537E0→1. Soft-finish (B53E40) also asserts if any
                // ChallengeHandler+0x432 (!=0) — Soft2 flush after uplink
                // clears those; do not arm Soft-finish mid Soft2.
                // AUTHNET_POST_PROOF_SOFT0=none → hop TCP instead.
                if (!Soft0Disabled())
                {
                    char const* soft0Mode = GetEnvOrDefault("AUTHNET_POST_PROOF_SOFT0", "session0_empty");
                    std::vector<uint8> soft0;
                    if (StringEquals(soft0Mode, "session0") || StringEquals(soft0Mode, "soft0"))
                        soft0 = BuildSoftSession0WithAccount(BuildStartupAccountKey(), std::string());
                    else
                        soft0 = BuildSoftSession0Empty();

                    SF_LOG_INFO("server.authserver",
                        "'%s:%d' authnet probe: sending post-proof Soft0 %s len=%zu plain=%s",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                        soft0Mode, soft0.size(), ByteArrayToHexStr(soft0).c_str());
                    socket().QueueSend(reinterpret_cast<char const*>(soft0.data()), soft0.size());
                }
                else
                {
                    PublishPendingSoftFinishSession();
                    SF_LOG_INFO("server.authserver",
                        "'%s:%d' authnet probe: Soft0 skipped; published Soft-finish pending for second TCP (.logon.battle.net via launcher DNS)",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort());
                }
            }
            offset += emptyLen;
            continue;
        }

        ProbePacketHeader header = DecodeProbePacketHeader(slice);
        if (!header.decoded || !header.modeSwitch)
            break;

        // 4501: Creep cmd=5 mode=1 — RC4 mode-switch after Soft finish.
        if (header.command == 5 && header.mode == 1 &&
            slice.size() >= ClientModeSwitchRequestLen)
        {
            _clientModeSwitchSeen = true;
            _postProofEncryptedOffset = offset + ClientModeSwitchRequestLen;
            if (_sessionKey)
                InitSoftFinishCrypt(*_sessionKey);
            else
            {
                SF_LOG_ERROR("server.authserver",
                    "'%s:%d' authnet probe: Soft-finish mode switch without SRP session key; "
                    "falling back to static RC4 tables",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort());
            }

            SF_LOG_INFO("server.authserver",
                "'%s:%d' authnet probe: post-proof plaintext mode switch (cmd=5 mode=1) "
                "encrypted_offset=%zu remaining=%zu session_key=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                _postProofEncryptedOffset, _captured.size() - _postProofEncryptedOffset,
                _sessionKey ? "yes" : "no");
            ProcessEncryptedClientBytes(_postProofEncryptedOffset);
            // Beat COP_CONNECT timeout: push CGA as soon as Soft-finish RC4 is
            // live (and any co-packet resource lookups are answered), before the
            // full PFTY burst / solicited 4002.
            if (ShouldPushCachedGameAccountAfterSoftFinish())
                TrySendCachedGameAccount("after-soft-finish");
            return;
        }

        // 4001: Creep cmd=0 mode=1 connect (pre-Soft0 Creep path).
        if (header.command == 0 && header.mode == 1 &&
            slice.size() >= ClientModeSwitchRequestLen)
        {
            if (!_mode1ConnectAnswered)
            {
                _mode1ConnectAnswered = true;
                std::vector<uint8> response = BuildEmptyModeCommand(0, 1);
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: sending plaintext mode1 command0 ack plain=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    ByteArrayToHexStr(response).c_str());
                socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
            }
            offset += ClientModeSwitchRequestLen;
            continue;
        }

        // Mode2 login probe (cmd=0 mode=2) — may appear later; answer like encrypted path.
        if (header.command == 0 && header.mode == 2 &&
            slice.size() >= ClientModeSwitchRequestLen)
        {
            if (!_mode2LoginAnswered)
            {
                char const* responseMode = nullptr;
                uint32 status = GetMode2Command0Status();
                std::vector<uint8> response = BuildLoginAuthProbe(status, responseMode);
                if (response.empty())
                {
                    SF_LOG_INFO("server.authserver",
                        "'%s:%d' authnet probe: plaintext mode2 command0 disabled by AUTHNET_MODE2_COMMAND0_RESPONSE=%s",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(), responseMode);
                }
                else
                {
                    _mode2LoginAnswered = true;
                    SF_LOG_INFO("server.authserver",
                        "'%s:%d' authnet probe: sending plaintext mode2 command0 %s response status=%u plain=%s",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                        responseMode, status, ByteArrayToHexStr(response).c_str());
                    socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
                }
            }
            offset += ClientModeSwitchRequestLen;
            continue;
        }

        // cmd=9 mode=1: service request (same body as encrypted path).
        ServiceRequestInfo serviceRequest;
        if (TryDecodeServiceRequest(slice, serviceRequest))
        {
            SF_LOG_INFO("server.authserver",
                "'%s:%d' authnet probe: post-proof plaintext service request id=%u records=%zu selector=%u label_len=%zu packet_len=%zu bits=%zu",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), serviceRequest.requestId,
                serviceRequest.records.size(), serviceRequest.selector, serviceRequest.label.size(),
                serviceRequest.packetLength, serviceRequest.bitLength);

            for (size_t i = 0; i < serviceRequest.records.size(); ++i)
            {
                ServiceRequestRecord const& record = serviceRequest.records[i];
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: post-proof service record[%u]=%s/%s minor=%u major=%u build=%u",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), uint32(i),
                    record.program.c_str(), record.component.c_str(), record.minor, record.major,
                    record.build);
            }

            if (std::find(_answeredRequestIds.begin(), _answeredRequestIds.end(), serviceRequest.requestId) ==
                _answeredRequestIds.end())
            {
                _answeredRequestIds.push_back(serviceRequest.requestId);

                // Auth/Prf 205 + zero blob on this socket precedes the 444-byte
                // close, which destroys B7FBA0's Sunken child (live 58053:
                // server Close() on 4001 aborted the hop before it could
                // speak). Answer Conn/Nul instead so Creep stays on this TCP
                // without that empty proof blob.
                char const* recordMode = (!_softFinishContinuation && Soft0Disabled())
                    ? "conn" : nullptr;
                ServiceResultProbeInfo resultProbe;
                std::vector<uint8> response = BuildServiceResultPayload(
                    serviceRequest, &resultProbe, recordMode);
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: sending plaintext service result id=%u found=%s record_index=%u/%zu "
                    "record_mode=%s key_mode=%s key=%s blob_mode=%s field80_mode=%s field80=%u source=%s/%s "
                    "minor=%u major=%u build=%u plain=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), serviceRequest.requestId,
                    resultProbe.hasRecord ? "yes" : "no", resultProbe.recordIndex, serviceRequest.records.size(),
                    resultProbe.recordMode, ServiceKeyModeName(resultProbe.keyMode),
                    ByteArrayToHexStr(resultProbe.serviceKey).c_str(), resultProbe.blobMode,
                    resultProbe.field80Mode, resultProbe.field80,
                    resultProbe.program.c_str(), resultProbe.component.c_str(), resultProbe.minor,
                    resultProbe.major, resultProbe.build, ByteArrayToHexStr(response).c_str());
                socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
            }

            offset += serviceRequest.packetLength;
            continue;
        }

        if (header.command == 9)
        {
            SF_LOG_INFO("server.authserver",
                "'%s:%d' authnet probe: post-proof plaintext Creep command9 mode=%u (%zu byte(s)) plain=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                header.mode, slice.size(), ByteArrayToHexStr(slice).c_str());
            break;
        }

        SF_LOG_INFO("server.authserver",
            "'%s:%d' authnet probe: post-proof plaintext Creep command=%u mode=%u (%zu byte(s)) plain=%s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(),
            header.command, header.mode, slice.size(), ByteArrayToHexStr(slice).c_str());
        break;
    }
}

void AuthnetSocket::PersistAuthnetWorldSessionKey(char const* reason)
{
    uint32 connectionSeed = _authnetWorldConnectionSeed;
    if (HasEnvValue("AUTHNET_JOIN_CONNECTION_SEED"))
        connectionSeed = GetEnvUInt32("AUTHNET_JOIN_CONNECTION_SEED", connectionSeed);
    PersistAuthnetWorldSessionKey(reason, connectionSeed);
}

void AuthnetSocket::PersistAuthnetWorldSessionKey(char const* reason, uint32 connectionSeed)
{
    if (!_sessionKey)
    {
        SF_LOG_INFO("server.authserver",
            "'%s:%d' authnet probe: skip world session_key persist (%s); Soft session key missing",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), reason);
        return;
    }

    // Soft K is SHA256-interleave 64 bytes. Client AuthSession uses the 40-byte
    // Join world key from B37290 (HMAC-SHA1 halves), not SoftK[0:40].
    _authnetSecret.fill(0);
    std::memcpy(_authnetSecret.data(), _sessionKey->data(),
        std::min(_authnetSecret.size(), _sessionKey->size()));
    _authnetSecretReady = true;

    if (!_authnetAccountId)
    {
        std::string identity = _loginIdentity;
        if (identity.empty() && _passwordChallenge)
            identity = _passwordChallenge->accountName;
        if (identity.empty() && HasEnvValue("AUTHNET_STARTUP_CHALLENGE_IDENTITY"))
            identity = GetEnvOrDefault("AUTHNET_STARTUP_CHALLENGE_IDENTITY", "");

        uint32 accountId = 0;
        std::string accountName;
        if (!TryResolveAuthnetLoginIdentity(identity, accountId, accountName))
        {
            SF_LOG_ERROR("server.authserver",
                "'%s:%d' authnet probe: cannot persist world session_key (%s); no account for identity='%s'",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), reason, identity.c_str());
            return;
        }

        _authnetAccountId = accountId;
        _accountId = accountId;
        _authnetAccountName = accountName;
        _loginIdentity = identity;
    }

    if (_authnetWorldAccountToken.empty())
        _authnetWorldAccountToken = BuildWorldAccountToken(_authnetAccountId ? _authnetAccountId : _accountId);
    if (!_authnetLocaleId)
        _authnetLocaleId = uint32(GetLocaleByName(_clientLocale.empty() ? "enUS" : _clientLocale));
    if (_authnetOS.empty())
        _authnetOS = _clientOs.empty() ? "Win" : _clientOs;

    if (HasEnvValue("AUTHNET_JOIN_CONNECTION_SEED"))
        connectionSeed = GetEnvUInt32("AUTHNET_JOIN_CONNECTION_SEED", connectionSeed);
    uint32 const realmField = ResolveJoinRealmField(GetAuthnetPreferredRealmField());
    if (!connectionSeed && !HasEnvValue("AUTHNET_JOIN_CONNECTION_SEED"))
    {
        SF_LOG_INFO("server.authserver",
            "'%s:%d' authnet probe: skip world session_key persist (%s); connection seed is 0 until Join",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), reason);
        return;
    }
    PersistAuthnetWorldSessionKey(connectionSeed, realmField, realmField, reason);
}

void AuthnetSocket::SendEncryptedRequestResult(uint32 requestId, std::vector<uint8> const* resourceKey, uint32 resourceItemId)
{
    if (std::find(_answeredRequestIds.begin(), _answeredRequestIds.end(), requestId) != _answeredRequestIds.end())
        return;

    _answeredRequestIds.push_back(requestId);

    std::vector<uint8> response = resourceKey && resourceKey->size() == 40 ?
        BuildResourceRequestResult(requestId, resourceKey, resourceItemId) :
        BuildEmptyRequestResult(requestId);
    std::vector<uint8> plainResponse = response;
    CryptServerPayload(response);

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted request result id=%u items=%u item_id=%u key=%s plain=%s encrypted=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), requestId,
        resourceKey && resourceKey->size() == 40 ? 1u : 0u, resourceItemId,
        resourceKey && resourceKey->size() == 40 ? ByteArrayToHexStr(*resourceKey).c_str() : "",
        ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

    socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
}

bool AuthnetSocket::TrySendCachedGameAccount(char const* reason)
{
    if (_mode2LoginAnswered || !_serverCryptInitialized)
        return false;

    char const* responseMode = nullptr;
    uint32 status = GetMode2Command0Status();
    std::vector<uint8> response = BuildLoginAuthProbe(status, responseMode);
    if (response.empty())
        return false;

    _mode2LoginAnswered = true;
    std::vector<uint8> plainResponse = response;
    CryptServerPayload(response);
    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet probe: push-cga (%s) mode2 command0 %s status=%u account=%s plain=%s encrypted=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), reason,
        responseMode ? responseMode : "?", status, GetCachedGameAccountName().c_str(),
        ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());
    socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());

    if (ShouldPushRealmListAfterLoginAuth())
        PushUnsolicitedRealmList(reason);
    return true;
}

void AuthnetSocket::SendMode2RealmUpdates(char const* reason)
{
    if (_mode2RealmUpdatesSent)
        return;
    _mode2RealmUpdatesSent = true;

    bool const clientIsLoopback = Skyfire::Net::Address(socket().getRemoteAddress(), 0).IsLoopback();
    std::vector<std::vector<uint8>> updates = BuildMode2RealmUpdatesFromRealmList(clientIsLoopback);
    std::vector<Realm> const realms = CollectAuthnetRealms();
    std::vector<uint8> blob;
    blob.reserve(64 * updates.size());
    for (size_t i = 0; i < updates.size(); ++i)
        blob.insert(blob.end(), updates[i].begin(), updates[i].end());

    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet probe: mode2 command2 realm updates (%s) bytes=%zu realms=%zu",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), reason,
        blob.size(), updates.size());
    for (size_t i = 0; i < updates.size(); ++i)
    {
        char const* name = (i < realms.size()) ? realms[i].name.c_str() : "?";
        uint32 const realmId = (i < realms.size()) ? realms[i].m_ID : 0;
        bool const offline = (i < realms.size()) && IsAuthnetRealmWorldOffline(realms[i]);
        uint32 const infoFlags = (i < realms.size()) ? GetAuthnetRealmInfoFlags(realms[i]) : 0;
        uint32 const stateFlags = (i < realms.size()) ? GetAuthnetRealmOfflineStateFlags(realms[i]) : 0;
        SF_LOG_INFO("server.authserver",
            "'%s:%d' authnet probe: mode2 command2 realmupdate[%zu] name=%s id=%u offline=%s infoFlags=0x%02X stateFlags=0x%02X plain=%s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(),
            i, name, realmId, offline ? "yes" : "no", infoFlags, stateFlags,
            ByteArrayToHexStr(updates[i]).c_str());
    }

    std::vector<uint8> plainBlob = blob;
    CryptServerPayload(blob);
    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet probe: sending encrypted mode2 realm updates (%s) plain=%s encrypted=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), reason,
        ByteArrayToHexStr(plainBlob).c_str(), ByteArrayToHexStr(blob).c_str());
    socket().QueueSend(reinterpret_cast<char const*>(blob.data()), blob.size());
}

void AuthnetSocket::SendMode2RealmListComplete(char const* reason)
{
    std::vector<uint8> complete = BuildEmptyModeCommand(3, 2);
    std::vector<uint8> plainComplete = complete;
    CryptServerPayload(complete);
    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet probe: mode2 command3 realmlist complete (%s) plain=%s encrypted=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), reason,
        ByteArrayToHexStr(plainComplete).c_str(), ByteArrayToHexStr(complete).c_str());
    socket().QueueSend(reinterpret_cast<char const*>(complete.data()), complete.size());
}

void AuthnetSocket::ScheduleGetRealmsFollowupAfterConnectComplete()
{
    // Coalesce Updates+Complete into one TCP write after the delay so Soft RX
    // fills BN_REALM before the second Complete Event runs A75420/A85240.
    bool const clientIsLoopback = Skyfire::Net::Address(socket().getRemoteAddress(), 0).IsLoopback();
    std::vector<std::vector<uint8>> updates = BuildMode2RealmUpdatesFromRealmList(clientIsLoopback);
    std::vector<uint8> blob;
    blob.reserve(64 * updates.size() + 8);
    for (size_t i = 0; i < updates.size(); ++i)
        blob.insert(blob.end(), updates[i].begin(), updates[i].end());
    std::vector<uint8> complete = BuildEmptyModeCommand(3, 2);
    blob.insert(blob.end(), complete.begin(), complete.end());

    uint32 const delayMs = GetGetRealmsFollowupDelayMs();
    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet probe: schedule get-realms followup delay_ms=%u updates=%zu bytes=%zu",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
        delayMs, updates.size(), blob.size());
    for (size_t i = 0; i < updates.size(); ++i)
        SF_LOG_INFO("server.authserver",
            "'%s:%d' authnet probe: get-realms followup realmupdate[%zu] plain=%s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(),
            i, ByteArrayToHexStr(updates[i]).c_str());

    std::vector<std::pair<std::string, std::vector<uint8>>> frames;
    frames.push_back(std::make_pair(
        std::string("mode2 updates+complete (get-realms-followup)"), std::move(blob)));

    _realmListFrames.clear();
    _realmListFrameIndex = 0;
    for (size_t i = 0; i < frames.size(); ++i)
    {
        CryptServerPayload(frames[i].second);
        _realmListFrames.push_back(std::move(frames[i]));
    }

    if (!_realmListFrameTimer)
        _realmListFrameTimer.reset(new boost::asio::steady_timer(socket().GetExecutor()));

    _realmListFrameTimer->expires_after(std::chrono::milliseconds(delayMs));
    std::shared_ptr<RealmSocket> sock = socket().shared_from_this();
    _realmListFrameTimer->async_wait([this, sock](boost::system::error_code const& error)
    {
        if (error)
            return;
        _mode2RealmUpdatesSent = true;
        SendNextRealmListFrame();
    });
}

void AuthnetSocket::SendMode2RealmListStartComplete(char const* reason)
{
    std::vector<uint8> start = BuildMode2Command0RealmListStart();
    std::vector<uint8> complete = BuildEmptyModeCommand(3, 2);
    std::vector<uint8> blob = start;
    blob.insert(blob.end(), complete.begin(), complete.end());

    CryptServerPayload(blob);
    SF_LOG_INFO("server.authserver",
        "'%s:%d' authnet probe: mode2 command0 realmlist start + command3 complete (%s) start=%s complete=%s encrypted=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), reason,
        ByteArrayToHexStr(start).c_str(), ByteArrayToHexStr(complete).c_str(),
        ByteArrayToHexStr(blob).c_str());
    socket().QueueSend(reinterpret_cast<char const*>(blob.data()), blob.size());
}

void AuthnetSocket::PushUnsolicitedRealmList(char const* reason)
{
    if (_mode1Command6Answered)
        return;

    _mode1Command6Answered = true;

    bool const clientIsLoopback = Skyfire::Net::Address(socket().getRemoteAddress(), 0).IsLoopback();
    uint32 const frameDelayMs = GetRealmListFrameDelayMs();
    bool firstFrame = true;

    auto sendEncrypted = [&](char const* label, std::vector<uint8> response)
    {
        if (!firstFrame && frameDelayMs)
            std::this_thread::sleep_for(std::chrono::milliseconds(frameDelayMs));
        firstFrame = false;

        std::vector<uint8> plainResponse = response;
        CryptServerPayload(response);
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: push-realmlist (%s) %s plain=%s encrypted=%s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), reason, label,
            ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());
        socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
    };

    bool const sendUpdates = ShouldPushRealmListUpdates();
    bool const sendStartComplete = ShouldPushRealmListStartComplete();
    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: pushing realmlist reason=%s start_complete=%s updates=%s frame_delay_ms=%u loopback=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), reason,
        sendStartComplete ? "yes" : "no", sendUpdates ? "yes" : "no",
        frameDelayMs, clientIsLoopback ? "yes" : "no");
    if (sendStartComplete && sendUpdates && frameDelayMs == 0)
    {
        sendEncrypted("mode1 command6 realmlist start-complete",
            BuildMode1RealmListStartComplete(clientIsLoopback));
        return;
    }
    if (sendStartComplete)
        sendEncrypted("mode1 command6 realmlist-start", BuildMode1Command6RealmListStart());

    if (sendUpdates)
    {
        std::vector<std::vector<uint8>> updates = BuildMode1RealmUpdatesFromRealmList(clientIsLoopback);
        for (size_t i = 0; i < updates.size(); ++i)
        {
            char label[64];
            std::snprintf(label, sizeof(label), "mode%u command%u realmupdate[%zu]",
                GetRealmUpdateSoftMode(), GetRealmUpdateSoftCommand(), i);
            sendEncrypted(label, std::move(updates[i]));
        }
    }

    if (sendStartComplete)
        sendEncrypted("mode1 command6 realmlist-complete", BuildMode1Command6RealmListComplete());
}

void AuthnetSocket::ProcessEncryptedClientBytes(size_t encryptedFollowupOffset)
{
    if (_captured.size() <= encryptedFollowupOffset + _encryptedBytesProcessed)
        return;

    size_t offset = encryptedFollowupOffset + _encryptedBytesProcessed;
    std::vector<uint8> plain(_captured.begin() + offset, _captured.end());
    _encryptedBytesProcessed += plain.size();

    CryptClientPayload(plain);

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: decrypted post-success byte(s) offset=%zu len=%zu plain=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), offset, plain.size(),
        ByteArrayToHexStr(plain).c_str());

    if (plain.size() % EncryptedResourceLookupRequestLen == 0)
    {
        StartAuthnetRealmReachabilityRefresh();

        for (size_t pos = 0; pos < plain.size(); pos += EncryptedResourceLookupRequestLen)
        {
            std::vector<uint8> packet(plain.begin() + pos, plain.begin() + pos + EncryptedResourceLookupRequestLen);
            ResourceLookupInfo lookup;
            if (TryDecodeResourceLookup(packet, lookup))
            {
                std::string payloadFirst = FourCCFromUInt32(lookup.payloadFirst);
                std::string payloadSecond = FourCCFromUInt32(lookup.payloadSecond);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: localized resource lookup id=%u marker=0x%08X parent=0x%08X optional=%u type=%u locale=%s variant=%u payload0=%s payload1=%s payload0_raw=0x%08X payload1_raw=0x%08X word=0x%04X remaining_bits=%zu packet_len=%zu bits=%zu",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), lookup.requestId,
                    lookup.marker, lookup.parentRaw, lookup.optionalFlag, lookup.requestType,
                    lookup.locale.c_str(), lookup.variant, payloadFirst.c_str(), payloadSecond.c_str(),
                    lookup.payloadFirst, lookup.payloadSecond, lookup.payloadWord, lookup.remainingBits,
                    lookup.packetLength, lookup.bitLength);

                std::vector<uint8> resourceKey;
                uint32 resourceItemId = 0;
                std::string resourceResultReason;
                bool hasResourceItem = TryGetConfiguredResourceResult(lookup, resourceKey, resourceItemId, resourceResultReason);
                if (!resourceResultReason.empty())
                {
                    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: configured resource result id=%u decision=%s items=%u item_id=%u key=%s",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                        lookup.requestId, resourceResultReason.c_str(), hasResourceItem ? 1u : 0u,
                        resourceItemId, hasResourceItem ? ByteArrayToHexStr(resourceKey).c_str() : "");
                }

                SendEncryptedRequestResult(lookup.requestId, hasResourceItem ? &resourceKey : nullptr, resourceItemId);
                continue;
            }

            uint32 requestId = 0;
            if (TryDecodePostSuccessRequestId(packet, requestId))
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: decoded post-success request id=%u",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), requestId);
                SendEncryptedRequestResult(requestId);
            }
        }

        // Soft-finish resource burst done. Do not push unsolicited CGA or
        // realmlist (Soft-bounced on :54235 / :56463). Wait for 4002 / 4601.
        if (_encryptedBytesProcessed >= EncryptedResourceLookupRequestLen * ResourceLookupBurstCount)
        {
            if (ShouldPushCachedGameAccountAfterResources())
                TrySendCachedGameAccount("after-resources");
            else
            {
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: resource burst complete (%zu B); waiting for solicited 4002/4601",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    _encryptedBytesProcessed);
            }

            if (ShouldPushRealmListAfterResources())
                PushUnsolicitedRealmList("after-resources");
        }

        return;
    }

    std::vector<std::vector<uint8>> softFrames;
    if (plain.size() == 4)
    {
        ProbePacketHeader h0 = DecodeProbePacketHeader(plain);
        std::vector<uint8> rest(plain.begin() + 2, plain.end());
        ProbePacketHeader h1 = DecodeProbePacketHeader(rest);
        if (h0.decoded && h0.modeSwitch && h0.command == 0 && h0.mode == 2 &&
            h1.decoded && h1.modeSwitch && h1.command == 6 && h1.mode == 1)
        {
            SF_LOG_INFO("server.authserver",
                "'%s:%d' authnet probe: coalesced 4002+4601 (%zu B); answering CGA then RequestRealmList",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), plain.size());
            softFrames.emplace_back(plain.begin(), plain.begin() + 2);
            softFrames.emplace_back(std::move(rest));
        }
    }
    if (softFrames.empty())
        softFrames.push_back(plain);

    for (std::vector<uint8> const& softFrame : softFrames)
    {
    std::vector<uint8> const& plain = softFrame;

    ProbePacketHeader header = DecodeProbePacketHeader(plain);
    if (header.decoded)
    {
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: decrypted post-success header command=%u mode_switch=%u mode=%u header_bits=%zu",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(),
            header.command, header.modeSwitch, header.mode, header.bitPosition);

        ServiceRequestInfo serviceRequest;
        if (TryDecodeServiceRequest(plain, serviceRequest))
        {
            if (!_softBounceSeen && LooksLikeSoftFrameBounce(plain))
            {
                _softBounceSeen = true;
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: Soft bounce trailer detected (9200/9300 + quoted Soft frames); ignoring further service replies",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort());
            }

            if (_softBounceSeen)
                return;

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: service request id=%u records=%zu selector=%u label_len=%zu nested_header=%s packet_len=%zu bits=%zu",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), serviceRequest.requestId,
                serviceRequest.records.size(), serviceRequest.selector, serviceRequest.label.size(),
                serviceRequest.hasNestedHeader ? "yes" : "no", serviceRequest.packetLength,
                serviceRequest.bitLength);

            size_t const logRecords = std::min<size_t>(serviceRequest.records.size(), 4);
            for (size_t i = 0; i < logRecords; ++i)
            {
                ServiceRequestRecord const& record = serviceRequest.records[i];
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: service request record[%u]=%s/%s minor=%u major=%u build=%u",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), uint32(i),
                    record.program.c_str(), record.component.c_str(), record.minor, record.major,
                    record.build);
            }
            if (serviceRequest.records.size() > logRecords)
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: service request ... %zu more record(s) omitted",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    serviceRequest.records.size() - logRecords);
            }

            if (std::find(_answeredRequestIds.begin(), _answeredRequestIds.end(), serviceRequest.requestId) != _answeredRequestIds.end())
                return;

            _answeredRequestIds.push_back(serviceRequest.requestId);

            ServiceResultProbeInfo resultProbe;
            std::vector<uint8> response = BuildServiceResultPayload(serviceRequest, &resultProbe);
            std::vector<uint8> plainResponse = response;
            CryptServerPayload(response);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: service result source id=%u found=%s record_index=%u/%zu record_mode=%s key_mode=%s key=%s blob_mode=%s blob=%s field80_mode=%s field80=%u source=%s/%s minor=%u major=%u build=%u",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), serviceRequest.requestId,
                resultProbe.hasRecord ? "yes" : "no", resultProbe.recordIndex, serviceRequest.records.size(),
                resultProbe.recordMode, ServiceKeyModeName(resultProbe.keyMode),
                ByteArrayToHexStr(resultProbe.serviceKey).c_str(), resultProbe.blobMode,
                ByteArrayToHexStr(resultProbe.blob).c_str(), resultProbe.field80Mode, resultProbe.field80,
                resultProbe.program.c_str(), resultProbe.component.c_str(), resultProbe.minor,
                resultProbe.major, resultProbe.build);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted service result payload id=%u plain=%s encrypted=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), serviceRequest.requestId,
                ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

            socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());

            if (ShouldPushRealmListAfterService())
                PushUnsolicitedRealmList("after-service");

            return;
        }

        if (plain.size() == ClientModeSwitchRequestLen &&
            header.command == 0 && header.modeSwitch && header.mode == 1)
        {
            _mode1ConnectAnswered = true;

            std::vector<uint8> response = BuildEmptyModeCommand(0, 1);
            std::vector<uint8> plainResponse = response;
            CryptServerPayload(response);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted empty mode1 command0 response plain=%s encrypted=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

            socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
        }

        if (plain.size() == ClientModeSwitchRequestLen &&
            header.command == 0 && header.modeSwitch && header.mode == 2)
        {
            if (_mode2LoginAnswered)
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: mode2 command0 observed after CachedGameAccount already sent",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort());
            }
            else
            {
            char const* responseMode = nullptr;
            uint32 status = GetMode2Command0Status();
            std::vector<uint8> response = BuildLoginAuthProbe(status, responseMode);

            if (response.empty())
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: mode2 command0 response disabled by AUTHNET_MODE2_COMMAND0_RESPONSE=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), responseMode);
                return;
            }

            if (uint32 delayMs = GetMode2Command0DelayMs())
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: delaying mode2 command0 %s response by %u ms",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), responseMode, delayMs);
                std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
            }

            _mode2LoginAnswered = true;

            std::vector<uint8> plainResponse = response;
            CryptServerPayload(response);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted mode2 command0 %s response status=%u account=%s plain=%s encrypted=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                responseMode, status, GetCachedGameAccountName().c_str(),
                ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

            socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
            } // !_mode2LoginAnswered

            if (_mode2LoginAnswered)
            {
            char const* command2Mode = GetMode2Command2ResponseMode();
            if (!_mode2Command2Answered && ShouldSendMode2Command2DetailProbe(command2Mode))
            {
                _mode2Command2Answered = true;

                if (uint32 detailDelayMs = GetMode2Command2DelayMs())
                {
                    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: delaying mode2 command2 %s response by %u ms",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(), command2Mode,
                        detailDelayMs);
                    std::this_thread::sleep_for(std::chrono::milliseconds(detailDelayMs));
                }

                std::vector<uint8> detailResponse = BuildLoginGameAccountDetailProbe();
                std::vector<uint8> plainDetailResponse = detailResponse;
                CryptServerPayload(detailResponse);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted mode2 command2 %s response plain=%s encrypted=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), command2Mode,
                    ByteArrayToHexStr(plainDetailResponse).c_str(), ByteArrayToHexStr(detailResponse).c_str());

                socket().QueueSend(reinterpret_cast<char const*>(detailResponse.data()), detailResponse.size());
            }

            // Fill BN_REALM before after-cga Complete so Event 5 / A75420
            // copies names immediately. 035752 sent Complete first (empty
            // fill) then Updates — Cancel revealed names but Soft died on
            // the later 4601 Complete (918 B). Updates-then-Complete leaves
            // state 3 with names; CONNECT's A7C410 can event-5 + A85240
            // without a second Complete that tears Soft down.
            SendMode2RealmUpdates("after-cga");
            char const* command3Mode = GetMode2Command3ResponseMode();
            if (!_mode2Command3Answered && ShouldSendMode2Command3EmptyProbe(command3Mode))
            {
                _mode2Command3Answered = true;
                SendMode2RealmListComplete("after-cga");
            }

            char const* command8Mode = GetMode2Command8ResponseMode();
            if (!_mode2Command8Answered && ShouldSendMode2Command8StructuredProbe(command8Mode))
            {
                _mode2Command8Answered = true;

                if (uint32 command8DelayMs = GetMode2Command8DelayMs())
                {
                    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: delaying mode2 command8 %s response by %u ms",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(), command8Mode,
                        command8DelayMs);
                    std::this_thread::sleep_for(std::chrono::milliseconds(command8DelayMs));
                }

                std::vector<uint8> command8Response = BuildMode2Command8StructuredProbe();
                std::vector<uint8> plainCommand8Response = command8Response;
                CryptServerPayload(command8Response);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted mode2 command8 %s response field=%u list6=%u list18=%u plain=%s encrypted=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), command8Mode,
                    GetEnvUInt32("AUTHNET_MODE2_COMMAND8_FIELD", 0),
                    GetMode2Command8List6Count(),
                    GetMode2Command8ListCount("AUTHNET_MODE2_COMMAND8_LIST18_COUNT"),
                    ByteArrayToHexStr(plainCommand8Response).c_str(), ByteArrayToHexStr(command8Response).c_str());

                socket().QueueSend(reinterpret_cast<char const*>(command8Response.data()), command8Response.size());
            }

            uint16 postLoginStatus = 0;
            if (!_postLoginStatusSent && TryGetPostLoginMode1Command1Status(postLoginStatus))
            {
                _postLoginStatusSent = true;

                std::vector<uint8> statusResponse = BuildMode1Command1Status(postLoginStatus);
                std::vector<uint8> plainStatusResponse = statusResponse;
                CryptServerPayload(statusResponse);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted post-login mode1 command1 status=%u plain=%s encrypted=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), postLoginStatus,
                    ByteArrayToHexStr(plainStatusResponse).c_str(), ByteArrayToHexStr(statusResponse).c_str());

                socket().QueueSend(reinterpret_cast<char const*>(statusResponse.data()), statusResponse.size());
            }

            if (ShouldPushRealmListAfterLoginAuth())
                PushUnsolicitedRealmList("after-loginauth");
            else
            {
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: skipping unsolicited realmlist after LoginAuth (AUTHNET_PUSH_REALMLIST_AFTER_LOGINAUTH=%s); waiting for solicited 4601",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    GetRealmListPushAfterLoginAuthMode());
            }
            } // if (_mode2LoginAnswered) follow-ups
        }

        // Soft mode2 cmd8 JoinRealm (solicited). After-cga Complete enables Soft
        // TX 8/9; client sends ~16 B immediately. Ignoring it closes Soft at
        // 918 B before a usable Join (:49402). Reply here; do not rely on the
        // unsolicited AUTHNET_MODE2_COMMAND8_RESPONSE probe.
        // A new click always re-answers: leftover Join Success to an offline
        // realm used to stick the client on "logging into game server".
        if (plain.size() > ClientModeSwitchRequestLen &&
            header.command == 8 && header.modeSwitch && header.mode == 2)
        {
            char const* responseMode = GetMode2Command8RequestResponseMode();
            if (StringEquals(responseMode, "none") || StringEquals(responseMode, "skip") ||
                StringEquals(responseMode, "off"))
            {
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: selected realm Soft mode2 command8 request observed; response disabled by AUTHNET_MODE2_COMMAND8_REQUEST_RESPONSE=%s plain=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    responseMode, ByteArrayToHexStr(plain).c_str());
                return;
            }

            std::vector<std::vector<uint8>> responses;
            std::vector<std::string> responseLabels;
            auto addResponse = [&responses, &responseLabels](char const* label, std::vector<uint8> response)
            {
                responses.push_back(std::move(response));
                responseLabels.push_back(label);
            };

            bool const clientIsLoopback = Skyfire::Net::Address(socket().getRemoteAddress(), 0).IsLoopback();
            uint32 joinRealmId = 0;
            uint32 joinConnectionSeed = 0;
            Realm joinRealmCopy;
            Realm routeRealmCopy;
            Realm const* joinRealm = nullptr;
            TryParseMode2Command8JoinRealmId(plain, joinRealmId);
            TryParseMode2Command8JoinConnectionSeed(plain, joinConnectionSeed);
            ApplyAuthnetRealmListFromProbes();
            if (joinRealmId)
                joinRealm = FindAuthnetRealmById(joinRealmId);
            else
                joinRealm = FindAuthnetRealm(GetEnvOrDefault("AUTHNET_MODE2_COMMAND2_NAME", ""));
            if (joinRealm)
            {
                joinRealmCopy = *joinRealm;
                joinRealm = &joinRealmCopy;
            }

            bool const unknownRealm = joinRealmId && !joinRealm;
            bool clickedDown = unknownRealm ||
                (joinRealm && !IsAuthnetRealmJoinable(joinRealm) && !AllowAuthnetOfflineJoin());
            if (!clickedDown && joinRealm && !AllowAuthnetOfflineJoin() &&
                !ProbeAuthnetRealmWorldPort(*joinRealm))
            {
                clickedDown = true;
                sRealmList->SetRealmOffline(joinRealmCopy.m_ID, true);
                joinRealmCopy.flag = RealmFlags(uint8(joinRealmCopy.flag) | REALM_FLAG_OFFLINE);
            }
            Realm const* routeRealm = joinRealm;
            uint32 hmacRealmId = joinRealmId ? joinRealmId : (joinRealm ? joinRealm->m_ID : 0);
            if (joinRealm)
            {
                routeRealmCopy = *joinRealm;
                routeRealm = &routeRealmCopy;
                hmacRealmId = routeRealmCopy.m_ID;
            }

            _mode2Command8Answered = true;
            _mode2Command8JoinAnswered = true;

            // A second Join Success (dead realm, or redirect that looks like
            // Main) commits COP_CONNECT and the client sends 4601 then closes
            // Soft. Grey unreachable realms in Updates; do not Success them.
            if (clickedDown)
            {
                _mode2Command8JoinSuccess = false;
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: refusing Join to unreachable realm id=%u (%s); not sending Success",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    joinRealmId, joinRealm ? joinRealm->name.c_str() : "<unknown>");
                addResponse("mode2-command8-empty", BuildEmptyModeCommand(8, 2));
            }
            else
            {
            _mode2Command8JoinSuccess = routeRealm && IsMode2Command8SuccessResponse(responseMode);

            // Persist B37290-derived world key before Soft-ACK so world AuthSession
            // matches the client Join Success Event key (seed + Success field).
            if (_mode2Command8JoinSuccess && _sessionKey)
            {
                TryUpdateWorldSessionKeyFromSelectedRealm(plain);
                PersistAuthnetWorldSessionKey(joinConnectionSeed, hmacRealmId, hmacRealmId, "join-soft-ack");
            }

            SessionKey const* worldKeyForJoin = nullptr;
            SessionKey softK40{};
            if (_authnetWorldSessionKeyPersisted)
                worldKeyForJoin = &_authnetWorldSessionKey;
            else if (_sessionKey)
            {
                std::memcpy(softK40.data(), _sessionKey->data(),
                    std::min(softK40.size(), _sessionKey->size()));
                worldKeyForJoin = &softK40;
            }

            if (StringEquals(responseMode, "status"))
                addResponse("mode2-command8-status", BuildMode2Command8StatusProbe());
            else if (StringEquals(responseMode, "empty"))
                addResponse("mode2-command8-empty", BuildEmptyModeCommand(8, 2));
            else if (IsMode2Command8SuccessResponse(responseMode))
                addResponse(StringEquals(responseMode, "empty-structured")
                    ? "mode2-command8-empty-structured" : "mode2-command8-structured",
                    BuildMode2Command8StructuredProbe(routeRealm, clientIsLoopback, worldKeyForJoin,
                        false, hmacRealmId));
            else
            {
                // Unknown value: keep CHOICE 0 Success (structured), not
                // CHOICE 1 status — that bounced.
                _mode2Command8JoinSuccess = true;
                addResponse("mode2-command8-structured",
                    BuildMode2Command8StructuredProbe(routeRealm, clientIsLoopback, worldKeyForJoin,
                        false, hmacRealmId));
            }
            }

            uint32 const delayMs = GetMode2Command8RequestDelayMs();
            uint32 const gapMs = GetMode2Command8RequestGapMs();
            if (delayMs)
            {
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: delaying selected-realm Soft mode2 command8 %s response by %u ms",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    responseMode, delayMs);
                std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
            }

            for (size_t i = 0; i < responses.size(); ++i)
            {
                if (gapMs && i != 0)
                    std::this_thread::sleep_for(std::chrono::milliseconds(gapMs));

                std::vector<uint8> response = responses[i];
                std::vector<uint8> plainResponse = response;
                CryptServerPayload(response);

                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: sending encrypted selected-realm Soft mode2 command8 %s response %u/%u join_success=%s realm_id=%u realm=%s seed=%u field=%u list6=%u list18=%u request=%s plain=%s encrypted=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    responseLabels[i].c_str(), uint32(i + 1), uint32(responses.size()),
                    _mode2Command8JoinSuccess ? "yes" : "no",
                    hmacRealmId,
                    routeRealm ? routeRealm->name.c_str() : "?",
                    joinConnectionSeed,
                    hmacRealmId,
                    GetMode2Command8List6Count(),
                    GetMode2Command8ListCount("AUTHNET_MODE2_COMMAND8_LIST18_COUNT"),
                    ByteArrayToHexStr(plain).c_str(),
                    ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

                socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
            }

            // Unsolicited Soft mode1 JoinResponse after Mode2 Soft-ACK Soft-bounces.
            // World key is derived client-side (B37290); Mode1 handoff stays opt-in.
            if (_mode2Command8JoinSuccess &&
                StringEnabled(GetEnvOrDefault("AUTHNET_MODE2_JOIN_MODE1_RESPONSE", "0")))
            {
                SkyFire::Authnet::PasswordSrp::SessionKey joinKey{};
                bool haveJoinKey = false;
                if (_authnetWorldSessionKeyPersisted)
                {
                    std::memcpy(joinKey.data(), _authnetWorldSessionKey.data(),
                        std::min(joinKey.size(), _authnetWorldSessionKey.size()));
                    haveJoinKey = true;
                }
                else if (_sessionKey)
                {
                    joinKey = *_sessionKey;
                    haveJoinKey = true;
                }

                if (haveJoinKey)
                {
                    _mode1Command7Answered = true;
                    bool const loopback =
                        Skyfire::Net::Address(socket().getRemoteAddress(), 0).IsLoopback();
                    std::vector<uint8> joinResponse =
                        BuildMode1JoinResponseSuccess(&joinKey, loopback);
                    std::vector<uint8> plainJoin = joinResponse;
                    CryptServerPayload(joinResponse);
                    SF_LOG_INFO("server.authserver",
                        "'%s:%d' authnet probe: Soft mode2 Join Soft-ACK → Mode1 JoinResponse Success (opt-in) key_source=%s plain=%s encrypted=%s",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                        _authnetWorldSessionKeyPersisted ? "authnet-session" : "soft-k",
                        ByteArrayToHexStr(plainJoin).c_str(),
                        ByteArrayToHexStr(joinResponse).c_str());
                    socket().QueueSend(reinterpret_cast<char const*>(joinResponse.data()),
                        joinResponse.size());
                }
            }

            return;
        }

        if (plain.size() == ClientModeSwitchRequestLen &&
            header.command == 6 && header.modeSwitch && header.mode == 1)
        {
            // Logout → Change Realm, and COP_CONNECT retry after a bad Join,
            // send 4601 as the first solicited cmd6 (after-cga never set
            // _mode1Command6Answered). Reset Join so the next click is fresh.
            bool const replayRealmList = _mode1Command6Answered || _mode2Command8JoinAnswered;
            _mode1Command6Answered = true;

            char const* responseMode = GetMode1Command6ResponseMode();
            if (StringEquals(responseMode, "none") || StringEquals(responseMode, "skip") || StringEquals(responseMode, "off"))
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: mode1 command6 realmlist request observed; response disabled by AUTHNET_MODE1_COMMAND6_RESPONSE=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), responseMode);
                return;
            }

            // Clear Join Success so a later click can Join fresh, and re-push
            // Updates because SendMode2RealmUpdates is one-shot.
            if (replayRealmList)
            {
                _mode2Command8Answered = false;
                _mode2Command8JoinAnswered = false;
                _mode2Command8JoinSuccess = false;
                _mode2RealmUpdatesSent = false;
                _connect4601CompleteSent = false;
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: mode1 command6 replaying realm list after Change Realm / logout",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort());
            }

            auto sendEncrypted = [&](char const* label, std::vector<uint8> response)
            {
                std::vector<uint8> plainResponse = response;
                CryptServerPayload(response);
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted mode1 %s plain=%s encrypted=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), label,
                    ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());
                socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
            };

            // RequestRealmList without CachedGameAccount (playtest :51501) →
            // REALM_LIST_FAILED immediately. Answer CGA first when missing.
            if (!_mode2LoginAnswered)
            {
                char const* cgaMode = nullptr;
                std::vector<uint8> cga = BuildLoginAuthProbe(GetMode2Command0Status(), cgaMode);
                if (!cga.empty())
                {
                    _mode2LoginAnswered = true;
                    sendEncrypted("command6-pre-cga mode2 command0", std::move(cga));
                    SF_LOG_INFO("server.authserver",
                        "'%s:%d' authnet probe: CachedGameAccount sent before solicited RequestRealmList (%s)",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                        cgaMode ? cgaMode : "?");
                }
            }

            if (StringEquals(responseMode, "status"))
            {
                sendEncrypted("command6 status", BuildMode1Command6Status(GetMode1Command6Status()));
            }
            else if (StringEquals(responseMode, "complete"))
            {
                sendEncrypted("command6 realmlist-complete", BuildMode1Command6RealmListComplete());
            }
            else if (StringEquals(responseMode, "failure"))
            {
                sendEncrypted("command6 realmlist-failure", BuildMode1Command6RealmListFailure(GetMode1Command6FailureReason()));
            }
            else if (StringEquals(responseMode, "empty"))
            {
                sendEncrypted("command6 empty", BuildEmptyModeCommand(6, 1));
            }
            else if (StringEquals(responseMode, "cmd6") || StringEquals(responseMode, "start-complete"))
            {
                // Debug only. Mode1 cmd6 has no RX handler (B76960 error 10).
                responseMode = "cmd6";
                bool const clientIsLoopback = Skyfire::Net::Address(socket().getRemoteAddress(), 0).IsLoopback();
                std::vector<std::pair<std::string, std::vector<uint8>>> frames;
                frames.push_back(std::make_pair(std::string("command6 realmlist-start"),
                    BuildMode1Command6RealmListStart()));
                std::vector<std::vector<uint8>> updates = BuildMode1RealmUpdatesFromRealmList(clientIsLoopback);
                for (size_t i = 0; i < updates.size(); ++i)
                {
                    char label[64];
                    std::snprintf(label, sizeof(label), "mode%u command%u realmupdate[%zu]",
                        GetRealmUpdateSoftMode(), GetRealmUpdateSoftCommand(), i);
                    frames.push_back(std::make_pair(std::string(label), std::move(updates[i])));
                }
                frames.push_back(std::make_pair(std::string("command6 realmlist-complete"),
                    BuildMode1Command6RealmListComplete()));
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: mode1 command6 realmlist frames=%zu delay_ms=%u (no RX handler)",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    frames.size(), GetRealmListFrameDelayMs());
                ScheduleRealmListFrames(std::move(frames));
            }
            else
            {
                // mode2-update (default): after-cga Updates+Complete filled
                // BN_REALM. A second Complete on 4601 closes Soft (:58268 /
                // 035752 / :50620 at 966 B). Join was already Soft-ACKed
                // (CHOICE 0 Success, empty-structured). Keep Soft alive:
                // do not Complete again. AUTHNET_MODE2_COMMAND3_ON_4601=
                // after-join sends Complete only after that Join Success.
                responseMode = "mode2-update";
                SendMode2RealmUpdates("solicited-4601");
                char const* command3Mode = GetEnvOrDefault("AUTHNET_MODE2_COMMAND3_ON_4601", "none");
                bool const completeAfterJoin = StringEquals(command3Mode, "after-join") &&
                    _mode2Command8JoinSuccess;
                if (ShouldSendMode2Command3EmptyProbe(command3Mode) ||
                    StringEquals(command3Mode, "complete") || completeAfterJoin)
                {
                    _connect4601CompleteSent = true;
                    SendMode2RealmListComplete("solicited-4601");
                }
                else
                {
                    SF_LOG_INFO("server.authserver",
                        "'%s:%d' authnet probe: Soft Soft-ACK 4601 without Complete (AUTHNET_MODE2_COMMAND3_ON_4601=%s) join_acked=%s join_success=%s; after-cga Complete already sent",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                        command3Mode,
                        _mode2Command8JoinAnswered ? "yes" : "no",
                        _mode2Command8JoinSuccess ? "yes" : "no");
                }
            }

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: mode1 command6 realmlist reply mode=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), responseMode);
        }

        // Soft mode1 cmd7: Creep JoinRealm → Larva JoinResponse (session key +
        // realm AddressPort). B53940 enables Soft mode1 cmd7 with cmd6.
        if (!_mode1Command7Answered && header.decoded && header.command == 7 &&
            header.modeSwitch && header.mode == 1)
        {
            _mode1Command7Answered = true;
            bool const clientIsLoopback = Skyfire::Net::Address(socket().getRemoteAddress(), 0).IsLoopback();
            std::vector<uint8> response = BuildMode1JoinResponseSuccess(
                _sessionKey ? &*_sessionKey : nullptr, clientIsLoopback);
            std::vector<uint8> plainResponse = response;
            CryptServerPayload(response);
            SF_LOG_INFO("server.authserver",
                "'%s:%d' authnet probe: Soft mode1 command7 JoinRealm → JoinResponse Success session_key=%s plain=%s encrypted=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                _sessionKey ? "yes" : "no",
                ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());
            socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
        }
    }
    }

    // Soft-finish: after 19 resource lookups, optional late CGA / realmlist.
    if (_encryptedBytesProcessed >= EncryptedResourceLookupRequestLen * ResourceLookupBurstCount)
    {
        if (ShouldPushCachedGameAccountAfterResources())
            TrySendCachedGameAccount("after-resources-tail");

        if (ShouldPushRealmListAfterResources())
            PushUnsolicitedRealmList("after-resources-tail");
    }
}

void AuthnetSocket::TrySendProbeResponse(size_t readOffset, size_t readSize)
{
    if (_httpResponded)
        return;

    if (LooksLikeHttpRequest(_captured))
    {
        if (CapturedStartsWith(_captured, "POST /patch "))
        {
            std::string response = BuildHttpResponse("text/xml", PatchHttpResponseBody);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: patch HTTP request received, sending local manifest pointer and closing",
                socket().getRemoteAddress().c_str(), socket().getRemotePort());

            socket().QueueSend(response.data(), response.size(), true);
            _httpResponded = true;
            _responded = true;
            return;
        }

        if (CapturedStartsWith(_captured, "GET /manifest "))
        {
            std::string response = BuildHttpResponse("text/xml", ManifestHttpResponseBody);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: manifest HTTP request received, sending local version info and closing",
                socket().getRemoteAddress().c_str(), socket().getRemotePort());

            socket().QueueSend(response.data(), response.size(), true);
            _httpResponded = true;
            _responded = true;
            return;
        }

        std::string target;
        if (TryGetHttpRequestTarget(_captured, target) && target.compare(0, 9, "/wow-pod/") == 0)
        {
            std::string body;
            std::string filePath;
            if (TryLoadLocalWowPodFile(target, body, filePath))
            {
                std::string response = BuildHttpResponse("application/octet-stream", body);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: local file request %s served from %s, %zu byte(s)",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    target.c_str(), filePath.c_str(), body.size());

                socket().QueueSend(response.data(), response.size(), true);
            }
            else
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: local file request %s not found",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), target.c_str());

                socket().QueueSend(NotFoundHttpResponse, sizeof(NotFoundHttpResponse) - 1, true);
            }

            _httpResponded = true;
            _responded = true;
            return;
        }

        // GET /agent (and other probe GETs) spam after LOGIN_BAD_SERVER_PROOF /
        // #103 — client retries ~20/sec. Answer quietly; do not INFO-log each hit.
        {
            static std::atomic<uint32> httpNoiseCount{0};
            static std::atomic<time_t> httpNoiseWindow{0};
            time_t const now = time(nullptr);
            uint32 const n = ++httpNoiseCount;
            time_t window = httpNoiseWindow.load();
            if (window == 0 || now - window >= 5)
            {
                httpNoiseWindow.store(now);
                httpNoiseCount.store(0);
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: HTTP noise on authnet port (e.g. GET /agent) — answered quietly (%u hit(s) in window); #103/prop205 mismatch usually causes this storm",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), n);
            }
        }

        socket().QueueSend(EmptyHttpResponse, sizeof(EmptyHttpResponse) - 1, true);
        _httpResponded = true;
        _responded = true;
        return;
    }

    // Live 60523: B7FBA0 hop is a fresh Auth login (same 89-byte cmd=9),
    // not Soft-finish. session0 empty → immediate close. Adopt SoftK only
    // if this socket starts at 4501 (no new identity request).
    if (!_softFinishContinuation)
    {
        ProbePacketHeader const header = DecodeProbePacketHeader(_captured);
        if (header.decoded && header.command == 5 && header.modeSwitch && header.mode == 1)
            TryAdoptPendingSoftFinishSession();
    }

    if (_softFinishContinuation)
    {
        _responded = true;
        ProcessPostProofPlaintext();
        return;
    }

    if (!DecodeInitialRequest())
        return;

    if (!_responded)
    {
        std::string startupAccountName;
        std::vector<uint8> startupAccountKey;
        std::vector<uint8> response = BuildStartupResponseProbe(_loginIdentity, &startupAccountName, &startupAccountKey, &_passwordChallenge);
        std::string startupAccountKeyHex = startupAccountKey.empty() ? std::string() : ByteArrayToHexStr(startupAccountKey);
        std::string responseHex = ByteArrayToHexStr(response);
        std::string startupPayloadHex = startupAccountName.empty()
            ? std::string()
            : ByteArrayToHexStr(std::vector<uint8>(startupAccountName.begin(), startupAccountName.end()));

        uint32 softCmd = response.empty() ? 0u : (response[0] & 0x3F);
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: initial request detected, sending startup response candidate %zu-byte response soft_cmd=%u account_count=%u account_payload_len=%zu srp_challenge=%s account_payload=%s account_key=%s plain=%s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), response.size(), softCmd,
            startupAccountName.empty() ? 0u : 1u, startupAccountName.size(),
            _passwordChallenge ? "yes" : "no",
            startupPayloadHex.c_str(),
            startupAccountKeyHex.c_str(), responseHex.c_str());

        socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
        _responded = true;
        StartAuthnetRealmReachabilityRefresh();
    }

    // Soft cmd=2 proof uplink (B4F1A0) arrives plaintext after Soft challenge —
    // not a mode-switch / encrypted follow-up.
    if (_responded && !_moduleProofAnswered && _captured.size() > _initialRequestLen)
    {
        std::vector<uint8> followup(_captured.begin() + _initialRequestLen, _captured.end());
        SoftModuleUplinkInfo uplink;
        if (TryDecodeSoftModuleUplink(followup, uplink) &&
            followup.size() >= uplink.packetLength)
        {
            _moduleProofAnswered = true;
            _soft2UplinkLen = uplink.packetLength;

            std::vector<uint8> const* firstBlob = uplink.blobs.empty() ? nullptr : &uplink.blobs[0];
            for (size_t i = 0; i < uplink.blobs.size(); ++i)
            {
                std::vector<uint8> const& blob = uplink.blobs[i];
                uint32 opcode = blob.empty() ? 0u : blob[0];
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: Soft cmd=2 module uplink count=%u blob[%zu] len=%zu op=%u head=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    uplink.count, i, blob.size(), opcode,
                    ByteArrayToHexStr(blob).c_str());

                // Log/save prop205 for forced same-session experiments. Do not
                // expect file reuse on the next login (CryptGenRandom).
                SkyFire::Authnet::PasswordSrp::ClientProof parsed;
                if (SkyFire::Authnet::PasswordSrp::ParseClientProof(blob, parsed))
                {
                    bool const secondOk = _passwordChallenge &&
                        SkyFire::Authnet::PasswordSrp::SecondMatchesProp205(
                            *_passwordChallenge, parsed.prop205);
                    SF_LOG_INFO("server.authserver",
                        "'%s:%d' authnet probe: Soft2 proof A=%s M1=%s prop205=%s second_629d0=%s",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                        ByteArrayToHexStr(std::vector<uint8>(parsed.A.begin(), parsed.A.end())).c_str(),
                        ByteArrayToHexStr(std::vector<uint8>(parsed.M1.begin(), parsed.M1.end())).c_str(),
                        ByteArrayToHexStr(std::vector<uint8>(parsed.prop205.begin(), parsed.prop205.end())).c_str(),
                        secondOk ? "match" : "mismatch");
                    SaveAuthnetProp205(parsed.prop205);
                }
            }

            char const* replyMode = nullptr;
            SkyFire::Authnet::PasswordSrp::SessionKey sessionKey{};
            std::vector<uint8> reply = BuildModuleProofReplyProbe(replyMode, &_passwordChallenge, firstBlob, &sessionKey);
            if (reply.empty())
            {
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: Soft cmd=2 module uplink observed; reply disabled by AUTHNET_MODULE_PROOF_RESPONSE=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), replyMode);
            }
            else
            {
                if (StringEquals(replyMode, "proof3_srp"))
                {
                    _sessionKey = sessionKey;
                    PersistAuthnetWorldSessionKey("soft2-proof");
                    // Soft0 is off by default: Soft finish hops to a second TCP.
                    // Publish SoftK now so adoption does not race Soft2 empty.
                    if (Soft0Disabled())
                        PublishPendingSoftFinishSession();
                }

                uint32 softCmd = reply[0] & 0x3F;
                SF_LOG_INFO("server.authserver",
                    "'%s:%d' authnet probe: Soft cmd=2 module uplink → sending %zu-byte %s reply soft_cmd=%u plain=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    reply.size(), replyMode, softCmd, ByteArrayToHexStr(reply).c_str());
                socket().QueueSend(reinterpret_cast<char const*>(reply.data()), reply.size());
            }
        }
    }

    if (_moduleProofAnswered)
        ProcessPostProofPlaintext();

    // Legacy path: mode-switch immediately after initial request (no Soft2).
    // Soft2 finish uses ProcessPostProofPlaintext + _postProofEncryptedOffset —
    // do not decrypt from _initialRequestLen+2 (that is Soft2 uplink).
    if (!_moduleProofAnswered && !_clientModeSwitchSeen &&
        _captured.size() >= _initialRequestLen + ClientModeSwitchRequestLen)
    {
        // Soft cmd=2 uplinks also start with Soft bits that look like
        // Creep command=2 — only treat true 2-byte mode-switch frames.
        if (_captured.size() == _initialRequestLen + ClientModeSwitchRequestLen ||
            (_captured.size() > _initialRequestLen + ClientModeSwitchRequestLen &&
             _captured[_initialRequestLen] != 0x42))
        {
        std::vector<uint8> followup(_captured.begin() + _initialRequestLen,
            _captured.begin() + _initialRequestLen + ClientModeSwitchRequestLen);
        ProbePacketHeader header = DecodeProbePacketHeader(followup);

        if (header.decoded)
        {
            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: client mode switch header command=%u mode_switch=%u mode=%u header_bits=%zu",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                header.command, header.modeSwitch, header.mode, header.bitPosition);
        }

        if (header.decoded && header.command == 5 && header.modeSwitch && header.mode == 1)
        {
            _clientModeSwitchSeen = true;

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: client mode switch detected, waiting for encrypted follow-up",
                socket().getRemoteAddress().c_str(), socket().getRemotePort());
        }
        }
    }

    size_t encryptedFollowupOffset = _postProofEncryptedOffset != 0
        ? _postProofEncryptedOffset
        : (_initialRequestLen + (_clientModeSwitchSeen ? ClientModeSwitchRequestLen : 0));
    if (_clientModeSwitchSeen && !_moduleProofAnswered)
        ProcessEncryptedClientBytes(encryptedFollowupOffset);

    if (!_followupLogged && !_moduleProofAnswered &&
        _captured.size() >= encryptedFollowupOffset + FirstEncryptedFollowupLen)
    {
        std::vector<uint8> followup(_captured.begin() + encryptedFollowupOffset,
            _captured.begin() + encryptedFollowupOffset + FirstEncryptedFollowupLen);

        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: first encrypted post-success bytes detected: %s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), ByteArrayToHexStr(followup).c_str());

        _followupLogged = true;
    }

    size_t resourceLookupBurstBytes = EncryptedResourceLookupRequestLen * ResourceLookupBurstCount;
    size_t postResourceLookupOffset = encryptedFollowupOffset + resourceLookupBurstBytes;

    if (_clientModeSwitchSeen && !_postSuccessBurstSeen && !_moduleProofAnswered)
    {
        size_t encryptedFollowupBytes = _captured.size() > encryptedFollowupOffset ? _captured.size() - encryptedFollowupOffset : 0;
        if (encryptedFollowupBytes >= resourceLookupBurstBytes)
        {
            _postSuccessBurstSeen = true;

            if (!ShouldSendBurstMode2Response())
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: post-success lookup burst reached %zu byte(s), burst mode2 response disabled",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), encryptedFollowupBytes);
                return;
            }

            char const* responseMode = nullptr;
            uint32 status = GetMode2Command0Status();
            std::vector<uint8> response = BuildLoginAuthProbe(status, responseMode);

            if (response.empty())
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: post-success lookup burst reached %zu byte(s), mode2 command0 response disabled by AUTHNET_MODE2_COMMAND0_RESPONSE=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), encryptedFollowupBytes,
                    responseMode);
                return;
            }

            if (uint32 delayMs = GetMode2Command0DelayMs())
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: post-success lookup burst reached %zu byte(s), delaying mode2 command0 %s response by %u ms",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), encryptedFollowupBytes,
                    responseMode, delayMs);
                std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
            }

            ProbePacketHeader header = DecodeProbePacketHeader(response);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: post-success lookup burst reached %zu byte(s), sending %zu-byte mode2 command0 %s candidate: %s, header_ok=%s command=%u mode_switch=%u mode=%u",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), encryptedFollowupBytes,
                response.size(), responseMode, ByteArrayToHexStr(response).c_str(), header.decoded ? "yes" : "no",
                header.command, header.modeSwitch, header.mode);

            std::vector<uint8> plainResponse = response;
            CryptServerPayload(response);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted burst-complete mode2 command0 %s response status=%u plain=%s encrypted=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                responseMode, status, ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

            socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
        }
    }

    if (_postSuccessBurstSeen && readOffset >= postResourceLookupOffset && readSize != ClientModeSwitchRequestLen)
    {
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: post-success request length %zu observed after burst",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), readSize);
    }
}
