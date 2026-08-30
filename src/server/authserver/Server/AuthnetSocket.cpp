/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AuthnetSocket.h"
#include "Auth/LoginIdentity.h"
#include "Authentication/BsnBitStream.h"
#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "HMAC.h"
#include "Log.h"
#include "RealmList.h"
#include "Util.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <thread>
#include <utility>
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
    constexpr uint32 MaxAuthnetRealmRoutes = 16;
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

    std::string GetEnvStringOrDefault(char const* name, std::string const& fallback)
    {
        char const* value = std::getenv(name);
        return value && value[0] ? value : fallback;
    }

    bool HasEnvValue(char const* name)
    {
        char const* value = std::getenv(name);
        return value && value[0];
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

    uint64 GetEnvUInt64(char const* name, uint64 fallback)
    {
        char const* value = std::getenv(name);
        if (!value || !value[0])
            return fallback;

        char* end = nullptr;
        unsigned long long parsed = std::strtoull(value, &end, 0);
        return end && *end == '\0' ? uint64(parsed) : fallback;
    }

    void WriteUInt64(Skyfire::Authnet::BitWriter& writer, uint64 value)
    {
        writer.WriteUInt32(uint32(value >> 32));
        writer.WriteUInt32(uint32(value));
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

        if (sRealmList->begin() != sRealmList->end())
            return &sRealmList->begin()->second;

        return nullptr;
    }

    uint32 GetAuthnetRealmCategory(Realm const* realm)
    {
        if (HasEnvValue("AUTHNET_MODE2_COMMAND2_REALM_CATEGORY"))
            return GetEnvUInt32("AUTHNET_MODE2_COMMAND2_REALM_CATEGORY", 1) & 0xFF;

        uint32 const category = realm ? realm->timezone : 0;
        return (category ? category : 1) & 0xFF;
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

    char const* AuthnetPacketName(uint32 mode, uint32 command, uint32 modeSwitch)
    {
        if (!modeSwitch)
            return "InlineControl";

        switch (mode)
        {
            case 0:
                switch (command)
                {
                    case 0:
                        return "InitialLoginResponseCandidate";
                    case 9:
                        return "InitialLoginRequest";
                    default:
                        return "Mode0Unknown";
                }
            case 1:
                switch (command)
                {
                    case 0:
                        return "SessionKeepalive";
                    case 1:
                        return "LoginStatus";
                    case 2:
                        return "ConnectRealmListResultCandidate";
                    case 3:
                        return "ConnectRealmListStatusCandidate";
                    case 4:
                        return "ConnectRealmListTicketCandidate";
                    case 5:
                        return "PostLoginTransition";
                    case 6:
                        return "RealmListCancelOrDisconnect";
                    case 9:
                        return "ServiceLookupRequest";
                    default:
                        return "Mode1Unknown";
                }
            case 2:
                switch (command)
                {
                    case 0:
                        return "GameAccountList";
                    case 2:
                        return "GameAccountDetail";
                    case 3:
                        return "GameAccountServicesReadyCandidate";
                    case 6:
                        return "GameAccountStateUpdateCandidate";
                    case 7:
                        return "GameAccountReconnectCompleteCandidate";
                    case 8:
                        return "GameAccountLoginComplete";
                    default:
                        return "Mode2Unknown";
                }
            case 11:
                switch (command)
                {
                    case 3:
                        return "ServiceMethodResult";
                    case 4:
                        return "ServiceLookupResult";
                    case 7:
                        return "ServiceNotify";
                    case 8:
                        return "ServiceState";
                    case 9:
                        return "ServiceLookupRequest";
                    case 10:
                        return "ServiceComplete";
                    default:
                        return "Mode11Unknown";
                }
            default:
                return "UnknownMode";
        }
    }

    char const* AuthnetPacketName(ProbePacketHeader const& header)
    {
        return header.decoded ? AuthnetPacketName(header.mode, header.command, header.modeSwitch) : "Undecoded";
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

    std::vector<uint8> BuildServicePayloadProbe(ServiceRequestInfo const& request, ServiceResultProbeInfo* probeInfo)
    {
        std::vector<uint8> payload(22, 0);

        char const* recordMode = GetEnvOrDefault("AUTHNET_SERVICE_RECORD_MODE", "last");
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

    char const* GetServiceResponseMode()
    {
        return GetEnvOrDefault("AUTHNET_SERVICE_RESPONSE_MODE", "method-result");
    }

    bool IsServiceResponseModeDisabled(char const* mode)
    {
        return StringEquals(mode, "none") || StringEquals(mode, "skip") || StringEquals(mode, "off");
    }

    char const* GetResourceResultMode()
    {
        return GetEnvOrDefault("AUTHNET_RESOURCE_RESULT_MODE", "empty");
    }

    bool IsResourceResultModeDisabled(char const* mode)
    {
        return StringEquals(mode, "none") || StringEquals(mode, "skip") || StringEquals(mode, "off");
    }

    std::vector<uint8> BuildServiceLookupResultBody(ServiceRequestInfo const& request, std::vector<uint8> const& servicePayloadProbe)
    {
        std::vector<uint8> body(32, 0);

        std::vector<uint8> configuredBody;
        if (TryParseHexBytes(std::getenv("AUTHNET_SERVICE_LOOKUP_RESULT_HEX"), body.size(), configuredBody))
            return configuredBody;

        char const* bodyMode = GetEnvOrDefault("AUTHNET_SERVICE_LOOKUP_RESULT_BODY", "service-payload");
        if (StringEquals(bodyMode, "zero"))
            return body;

        if (StringEquals(bodyMode, "request-id"))
        {
            StoreUInt32LE(body.data(), request.requestId);
            return body;
        }

        if (StringEquals(bodyMode, "service-payload") || StringEquals(bodyMode, "record"))
        {
            size_t const copySize = std::min(body.size(), servicePayloadProbe.size());
            if (copySize != 0)
                std::copy(servicePayloadProbe.begin(), servicePayloadProbe.begin() + copySize, body.begin());
            return body;
        }

        return body;
    }

    std::string GetStartupAccountName()
    {
        std::string accountName = GetEnvOrDefault("AUTHNET_STARTUP_ACCOUNT_NAME", "WoW1");
        if (accountName.size() > 0x3FC)
            accountName.resize(0x3FC);

        return accountName;
    }

    std::vector<uint8> BuildStartupAccountKey()
    {
        std::vector<uint8> key(40, 0);
        std::vector<uint8> configuredKey;
        if (TryParseHexBytes(std::getenv("AUTHNET_STARTUP_ACCOUNT_KEY"), key.size(), configuredKey))
            return configuredKey;

        return key;
    }

    bool ShouldSendStartupAccountProbe(char const* mode)
    {
        return StringEnabled(mode);
    }

    bool ShouldGenerateWorldSessionKey()
    {
        return StringEnabled(std::getenv("AUTHNET_GENERATE_WORLD_SESSION_KEY"));
    }

    std::string GetConfiguredStartupWorldAccount()
    {
        std::string authSessionIdentity = GetEnvOrDefault("AUTHNET_STARTUP_WORLD_ACCOUNT", "A");
        if (authSessionIdentity.size() > MaxInitialIdentityBytes)
            authSessionIdentity.resize(MaxInitialIdentityBytes);

        return authSessionIdentity;
    }

    std::string BuildWorldAccountToken(uint32 /*accountId*/)
    {
        return GetConfiguredStartupWorldAccount();
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

    std::vector<uint8> BuildStartupResponseProbe(std::string const& worldAccountToken = std::string(),
        std::string* startupAccountName = nullptr,
        std::vector<uint8>* startupAccountKey = nullptr,
        std::string* worldAccountIdentity = nullptr)
    {
        Skyfire::Authnet::BitWriter writer;
        char const* accountMode = GetEnvOrDefault("AUTHNET_STARTUP_ACCOUNT_RESPONSE", "none");
        bool const includeAccount = ShouldSendStartupAccountProbe(accountMode);
        std::string accountName = includeAccount ? GetStartupAccountName() : std::string();
        std::vector<uint8> accountKey = includeAccount ? BuildStartupAccountKey() : std::vector<uint8>();

        if (startupAccountName)
            *startupAccountName = accountName;
        if (startupAccountKey)
            *startupAccountKey = accountKey;

        std::string authSessionIdentity = worldAccountToken.empty() ? GetConfiguredStartupWorldAccount() : worldAccountToken;

        if (worldAccountIdentity)
            *worldAccountIdentity = authSessionIdentity;

        writer.WriteBits(0x00, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x00, 4);

        writer.WriteBits(0, 1);
        writer.WriteBits(includeAccount ? 1 : 0, 3);
        if (includeAccount)
        {
            writer.WriteBytes(accountKey.data(), accountKey.size());
            writer.WriteBits(uint32(accountName.size()), 10);
            writer.WriteBytes(accountName.data(), accountName.size());
        }

        writer.WriteUInt32(0x8000C350);
        writer.WriteBits(0, 1);
        writer.WriteBits(0, 8);
        writer.AlignToByte();
        writer.WriteBits(0, 8);
        writer.AlignToByte();
        writer.WriteUInt32(0);
        writer.WriteBits(0, 8);
        writer.WriteUInt32(0);
        writer.WriteUInt32(0);
        writer.WriteBits(0, 8);
        writer.WriteBits(0, 5);
        writer.WriteBytes(authSessionIdentity.data(), authSessionIdentity.size());
        writer.WriteUInt32(0);
        writer.WriteUInt32(0);
        writer.WriteUInt32(0);
        writer.WriteBits(0, 1);

        return writer.Data();
    }

    std::vector<uint8> BuildLoginAuthStatusProbe(uint32 status)
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x00, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x02, 4);
        writer.WriteBits(1, 1);
        writer.WriteBits(status, 8);

        return writer.Data();
    }

    std::vector<uint8> BuildLoginAuthEmptyListProbe()
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x00, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x02, 4);
        writer.WriteBits(0, 1);
        writer.WriteBits(0, 7);
        writer.AlignToByte();

        return writer.Data();
    }

    uint32 GetAuthnetDefaultRealmField();
    std::vector<uint8> BuildLoginAuthRealmListProbe();

    std::vector<uint8> BuildLoginAuthSingleListProbe()
    {
        Skyfire::Authnet::BitWriter writer;

        uint32 const byte0 = GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_BYTE0", 0) & 0xFF;
        uint32 const byte1 = GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_BYTE1", 0) & 0xFF;
        uint32 const value32 = GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_VALUE32", GetAuthnetDefaultRealmField());
        uint32 const word16 = GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_WORD16", 0) & 0xFFFF;

        writer.WriteBits(0x00, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x02, 4);
        writer.WriteBits(0, 1);
        writer.WriteBits(1, 7);
        writer.WriteBits(byte0, 8);
        writer.WriteBits(0, 12);
        writer.WriteBits(byte1, 8);
        writer.WriteUInt32(value32);
        writer.WriteBits(word16, 16);
        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildLoginAuthProbe(uint32 status, char const*& responseMode)
    {
        responseMode = GetEnvOrDefault("AUTHNET_MODE2_COMMAND0_RESPONSE", "status");
        if (StringEquals(responseMode, "none") || StringEquals(responseMode, "skip") || StringEquals(responseMode, "off"))
            return {};

        if (StringEquals(responseMode, "empty-list"))
            return BuildLoginAuthEmptyListProbe();

        if (StringEquals(responseMode, "single-list"))
            return BuildLoginAuthSingleListProbe();

        if (StringEquals(responseMode, "realm-list") || StringEquals(responseMode, "realms") || StringEquals(responseMode, "multi-list"))
            return BuildLoginAuthRealmListProbe();

        responseMode = "status";
        return BuildLoginAuthStatusProbe(status);
    }

    bool ShouldSendBurstMode2Response()
    {
        char const* value = GetEnvOrDefault("AUTHNET_SEND_BURST_MODE2_RESPONSE", "1");
        return !StringEquals(value, "0") && !StringEquals(value, "false") && !StringEquals(value, "no");
    }

    uint32 GetMode2Command0DelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND0_DELAY_MS", 0);
    }

    bool ShouldSendMode2FollowupsBeforeCommand0()
    {
        return StringEnabled(std::getenv("AUTHNET_MODE2_FOLLOWUPS_BEFORE_COMMAND0"));
    }

    uint32 GetMode2Command0Status()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND0_STATUS", 0);
    }

    char const* GetMode2Command2ResponseMode()
    {
        return GetEnvOrDefault("AUTHNET_MODE2_COMMAND2_RESPONSE", "none");
    }

    bool ShouldSendMode2Command2DetailProbe(char const* mode)
    {
        return StringEquals(mode, "detail") || StringEquals(mode, "full") ||
            StringEquals(mode, "realms") || StringEquals(mode, "realm-list") ||
            StringEquals(mode, "realm-details") || StringEquals(mode, "multi-list") ||
            StringEquals(mode, "1") || StringEquals(mode, "true") || StringEquals(mode, "yes");
    }

    bool ShouldSendMode2Command2RealmDetails(char const* mode)
    {
        return StringEquals(mode, "realms") || StringEquals(mode, "realm-list") ||
            StringEquals(mode, "realm-details") || StringEquals(mode, "multi-list");
    }

    uint32 GetMode2Command2DelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND2_DELAY_MS", 0);
    }

    bool ShouldSendMode2EmptyCommandProbe(char const* mode)
    {
        return StringEquals(mode, "empty") || StringEquals(mode, "1") ||
            StringEquals(mode, "true") || StringEquals(mode, "yes");
    }

    char const* GetMode2Command3ResponseMode()
    {
        return GetEnvOrDefault("AUTHNET_MODE2_COMMAND3_RESPONSE", "none");
    }

    uint32 GetMode2Command3DelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND3_DELAY_MS", 0);
    }

    char const* GetMode2Command6ResponseMode()
    {
        return GetEnvOrDefault("AUTHNET_MODE2_COMMAND6_RESPONSE", "none");
    }

    bool ShouldSendMode2Command6StateProbe(char const* mode)
    {
        return StringEquals(mode, "state") || StringEquals(mode, "structured") ||
            StringEquals(mode, "1") || StringEquals(mode, "true") || StringEquals(mode, "yes");
    }

    uint32 GetMode2Command6DelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND6_DELAY_MS", 0);
    }

    char const* GetMode2Command6Position()
    {
        return GetEnvOrDefault("AUTHNET_MODE2_COMMAND6_POSITION", "after-command8");
    }

    char const* GetMode2Command7ResponseMode()
    {
        return GetEnvOrDefault("AUTHNET_MODE2_COMMAND7_RESPONSE", "none");
    }

    uint32 GetMode2Command7DelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND7_DELAY_MS", 0);
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

    char const* GetMode2Command8StatusResponseMode()
    {
        return GetEnvOrDefault("AUTHNET_MODE2_COMMAND8_STATUS_RESPONSE", "none");
    }

    bool ShouldSendMode2Command8StatusBeforeStructured(char const* mode)
    {
        return StringEquals(mode, "before") || StringEquals(mode, "before-structured") || StringEquals(mode, "both");
    }

    bool ShouldSendMode2Command8StatusAfterStructured(char const* mode)
    {
        return StringEquals(mode, "after") || StringEquals(mode, "after-structured") ||
            StringEquals(mode, "status") || StringEquals(mode, "both") ||
            StringEquals(mode, "1") || StringEquals(mode, "true") || StringEquals(mode, "yes");
    }

    uint32 GetMode2Command8DelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND8_DELAY_MS", 0);
    }

    uint32 GetMode2Command8RepeatCount()
    {
        return std::min<uint32>(GetEnvUInt32("AUTHNET_MODE2_COMMAND8_REPEAT_COUNT", 0), 4);
    }

    uint32 GetMode2Command8RepeatDelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND8_REPEAT_DELAY_MS", 0);
    }

    bool ShouldSendMode2Command8RepeatAsync()
    {
        return StringEnabled(std::getenv("AUTHNET_MODE2_COMMAND8_REPEAT_ASYNC"));
    }

    uint32 GetMode2Command8PostCommand6Count()
    {
        return std::min<uint32>(GetEnvUInt32("AUTHNET_MODE2_COMMAND8_POST_COMMAND6_COUNT", 0), 4);
    }

    uint32 GetMode2Command8PostCommand6DelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND8_POST_COMMAND6_DELAY_MS", 0);
    }

    uint32 GetMode2Command8PostCommand6GapMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND8_POST_COMMAND6_GAP_MS", 0);
    }

    char const* GetMode2Command8RequestResponseMode()
    {
        return GetEnvOrDefault("AUTHNET_MODE2_COMMAND8_REQUEST_RESPONSE", "structured");
    }

    uint32 GetMode2Command8RequestDelayMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND8_REQUEST_DELAY_MS", 0);
    }

    uint32 GetMode2Command8RequestGapMs()
    {
        return GetEnvUInt32("AUTHNET_MODE2_COMMAND8_REQUEST_GAP_MS", 0);
    }

    uint32 GetMode2Command8ListCount(char const* name)
    {
        return std::min<uint32>(GetEnvUInt32(name, 0), MaxAuthnetRealmRoutes);
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

    struct AuthnetRealmRoute
    {
        uint32 realmId = 0;
        std::string name;
        std::string host = "127.0.0.1";
        std::array<uint8, 4> address = {{ 127, 0, 0, 1 }};
        uint16 port = 8085;
        uint32 category = 1;
    };

    std::vector<AuthnetRealmRoute> BuildAuthnetRealmRoutes()
    {
        std::vector<AuthnetRealmRoute> routes;
        sRealmList->UpdateIfNeed();
        routes.reserve(std::min<size_t>(sRealmList->size(), MaxAuthnetRealmRoutes));

        for (RealmList::RealmMap::const_iterator itr = sRealmList->begin(); itr != sRealmList->end(); ++itr)
        {
            if (routes.size() >= MaxAuthnetRealmRoutes)
                break;

            Realm const& realm = itr->second;
            AuthnetRealmRoute route;
            route.realmId = realm.m_ID;
            route.name = realm.name;
            route.host = realm.ExternalAddress.GetHost();
            route.port = uint16(realm.ExternalAddress.GetPort() & 0xFFFF);
            route.category = GetAuthnetRealmCategory(&realm);
            TryParseIPv4AddressBytes(route.host, route.address);
            routes.push_back(route);
        }

        return routes;
    }

    uint32 GetAuthnetDefaultRealmField(std::vector<AuthnetRealmRoute> const& routes)
    {
        if (HasEnvValue("AUTHNET_MODE2_COMMAND8_FIELD"))
            return GetEnvUInt32("AUTHNET_MODE2_COMMAND8_FIELD", 0);

        std::string const preferredName = GetEnvStringOrDefault("AUTHNET_MODE2_COMMAND2_NAME", "");
        if (!preferredName.empty())
        {
            if (Realm const* realm = FindAuthnetRealm(preferredName))
                return realm->m_ID;
        }

        if (!routes.empty())
            return routes.front().realmId;

        return 0;
    }

    uint32 GetAuthnetDefaultRealmField()
    {
        return GetAuthnetDefaultRealmField(BuildAuthnetRealmRoutes());
    }

    std::string FindAuthnetRealmNameById(uint32 realmId)
    {
        sRealmList->UpdateIfNeed();
        for (RealmList::RealmMap::const_iterator itr = sRealmList->begin(); itr != sRealmList->end(); ++itr)
            if (itr->second.m_ID == realmId)
                return itr->second.name;

        return {};
    }

    uint32 GetMode2Command8List6Count(std::vector<AuthnetRealmRoute> const& routes)
    {
        if (HasEnvValue("AUTHNET_MODE2_COMMAND8_LIST6_COUNT"))
            return GetMode2Command8ListCount("AUTHNET_MODE2_COMMAND8_LIST6_COUNT");

        if (StringEquals(GetMode2Command8ResponseMode(), "empty-structured"))
            return 0;

        if (!routes.empty())
            return std::min<uint32>(uint32(routes.size()), MaxAuthnetRealmRoutes);

        return 1;
    }

    uint32 GetMode2Command8List6Count()
    {
        return GetMode2Command8List6Count(BuildAuthnetRealmRoutes());
    }

    std::vector<uint8> BuildMode2Command8DefaultRouteList(uint32 entryCount, std::vector<AuthnetRealmRoute> const& routes)
    {
        std::vector<uint8> bytes(6 * entryCount, 0);
        if (!entryCount)
            return bytes;

        std::array<uint8, 4> overrideAddress = {{ 127, 0, 0, 1 }};
        std::string const configuredAddress = GetEnvStringOrDefault("AUTHNET_MODE2_COMMAND8_ROUTE_ADDRESS",
            GetEnvStringOrDefault("AUTHNET_MODE2_COMMAND2_ENDPOINT_ADDRESS", ""));
        bool const useOverrideAddress = !configuredAddress.empty() && TryParseIPv4AddressBytes(configuredAddress, overrideAddress);
        bool const useOverridePort = HasEnvValue("AUTHNET_MODE2_COMMAND8_ROUTE_PORT") || HasEnvValue("AUTHNET_MODE2_COMMAND2_PORT");
        uint16 const overridePort = uint16(GetEnvUInt32("AUTHNET_MODE2_COMMAND8_ROUTE_PORT",
            GetEnvUInt32("AUTHNET_MODE2_COMMAND2_PORT", 8085)) & 0xFFFF);

        for (uint32 index = 0; index < entryCount; ++index)
        {
            AuthnetRealmRoute const* route = nullptr;
            if (!routes.empty())
                route = &routes[std::min<size_t>(index, routes.size() - 1)];

            std::array<uint8, 4> const& address = useOverrideAddress ? overrideAddress : (route ? route->address : overrideAddress);
            uint16 const port = useOverridePort ? overridePort : (route ? route->port : uint16(8085));

            size_t const offset = size_t(index) * 6;
            bytes[offset] = address[0];
            bytes[offset + 1] = address[1];
            bytes[offset + 2] = address[2];
            bytes[offset + 3] = address[3];
            bytes[offset + 4] = uint8(port >> 8);
            bytes[offset + 5] = uint8(port);
        }

        return bytes;
    }

    std::vector<uint8> BuildLoginAuthRealmListProbe()
    {
        Skyfire::Authnet::BitWriter writer;
        std::vector<AuthnetRealmRoute> const routes = BuildAuthnetRealmRoutes();

        uint32 const byte0 = GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_BYTE0", 0) & 0xFF;
        uint32 const byte1 = GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_BYTE1", 0) & 0xFF;
        uint32 const word16 = GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_WORD16", 0) & 0xFFFF;
        uint32 const entryCount = routes.empty() ? 1 : std::min<uint32>(uint32(routes.size()), MaxAuthnetRealmRoutes);

        writer.WriteBits(0x00, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x02, 4);
        writer.WriteBits(0, 1);
        writer.WriteBits(entryCount, 7);

        for (uint32 index = 0; index < entryCount; ++index)
        {
            uint32 const value32 = routes.empty() ? GetAuthnetDefaultRealmField(routes) : routes[index].realmId;
            writer.WriteBits(byte0, 8);
            writer.WriteBits(0, 12);
            writer.WriteBits(byte1, 8);
            writer.WriteUInt32(value32);
            writer.WriteBits(word16, 16);
        }

        writer.AlignToByte();
        return writer.Data();
    }

    std::vector<uint8> GetConfiguredListBytes(char const* name, size_t entrySize, uint32 entryCount)
    {
        std::vector<uint8> bytes(entrySize * entryCount, 0);
        std::vector<uint8> configuredBytes;
        if (!bytes.empty() && TryParseHexBytes(std::getenv(name), bytes.size(), configuredBytes))
            return configuredBytes;

        return bytes;
    }

    std::vector<uint8> BuildLoginGameAccountDetailProbe(AuthnetRealmRoute const* route = nullptr)
    {
        Skyfire::Authnet::BitWriter writer;

        std::vector<AuthnetRealmRoute> fallbackRoutes;
        if (!route)
            fallbackRoutes = BuildAuthnetRealmRoutes();

        uint32 const defaultRealmField = route ? route->realmId : GetAuthnetDefaultRealmField(fallbackRoutes);
        uint32 const keyByte0 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_KEY_BYTE0",
            GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_BYTE0", 0)) & 0xFF;
        uint32 const keyByte1 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_KEY_BYTE1",
            GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_BYTE1", 0)) & 0xFF;
        uint32 const keyValue32 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_KEY_VALUE32",
            GetEnvUInt32("AUTHNET_MODE2_COMMAND0_ENTRY_VALUE32", defaultRealmField));
        uint32 const field420 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FIELD420", 0) & 0xFF;
        uint32 const field424 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FIELD424", 0);
        uint32 const finalByte = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FINAL_BYTE", 0) & 0xFF;
        bool const hasEndpoint = StringEnabled(std::getenv("AUTHNET_MODE2_COMMAND2_HAS_ENDPOINT"));
        uint32 const endpointField450 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FIELD450", 0);
        uint32 const endpointField454 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FIELD454", 0);

        std::string configuredName = GetEnvStringOrDefault("AUTHNET_MODE2_COMMAND2_NAME", "");
        Realm const* realm = route ? nullptr : FindAuthnetRealm(configuredName);
        uint32 const realmCategory = route ? route->category : GetAuthnetRealmCategory(realm);
        uint32 const field418 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FIELD418", 0);
        uint32 const field41C = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FIELD41C", realmCategory);
        uint32 const endpointPort = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_PORT",
            route ? route->port : (realm ? realm->ExternalAddress.GetPort() : 8085)) & 0xFFFF;
        uint32 const endpointByte458 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FIELD458", endpointPort >> 8) & 0xFF;
        uint32 const endpointByte459 = GetEnvUInt32("AUTHNET_MODE2_COMMAND2_FIELD459", endpointPort) & 0xFF;

        std::string accountName = route ? route->name : GetEnvStringOrDefault("AUTHNET_MODE2_COMMAND2_NAME",
            realm ? realm->name : "WoW1");
        if (accountName.size() > 0x3FC)
            accountName.resize(0x3FC);

        std::string endpointAddress = route ? route->host : GetEnvStringOrDefault("AUTHNET_MODE2_COMMAND2_ENDPOINT_ADDRESS",
            realm ? realm->ExternalAddress.GetHost() : "127.0.0.1");
        if (endpointAddress.size() > 0x17)
            endpointAddress.resize(0x17);

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
        writer.WriteBits(hasEndpoint ? 1 : 0, 1);
        if (hasEndpoint)
        {
            uint8 endpointFields[6] =
            {
                uint8(endpointField454 & 0xFF),
                uint8((endpointField454 >> 8) & 0xFF),
                uint8((endpointField454 >> 16) & 0xFF),
                uint8((endpointField454 >> 24) & 0xFF),
                uint8(endpointByte458),
                uint8(endpointByte459)
            };

            writer.WriteBits(uint32(endpointAddress.size()), 5);
            writer.WriteBytes(endpointAddress.data(), endpointAddress.size());
            writer.WriteUInt32(endpointField450);
            writer.WriteBytes(endpointFields, sizeof(endpointFields));
        }
        writer.WriteBits(finalByte, 8);

        writer.WriteBits(keyByte0, 8);
        writer.WriteBits(0, 12);
        writer.WriteBits(keyByte1, 8);
        writer.WriteUInt32(keyValue32);
        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildMode2Command6StateProbe()
    {
        Skyfire::Authnet::BitWriter writer;

        uint32 const accountByte0 = GetEnvUInt32("AUTHNET_MODE2_COMMAND6_ACCOUNT_BYTE0", 0) & 0xFF;
        uint32 const accountField4 = GetEnvUInt32("AUTHNET_MODE2_COMMAND6_ACCOUNT_FIELD4", 0);
        uint32 const accountField8 = GetEnvUInt32("AUTHNET_MODE2_COMMAND6_ACCOUNT_FIELD8", 0);
        uint32 const serviceStateField0 = GetEnvUInt32("AUTHNET_MODE2_COMMAND6_SERVICE_FIELD0", 0);
        uint64 const serviceStateField8 = GetEnvUInt64("AUTHNET_MODE2_COMMAND6_SERVICE_FIELD8", 0);
        uint64 const stateToken64 = GetEnvUInt64("AUTHNET_MODE2_COMMAND6_STATE_TOKEN64", 0);
        uint32 const stateField8 = GetEnvUInt32("AUTHNET_MODE2_COMMAND6_STATE_FIELD8", 0);
        uint32 const stateByte0 = GetEnvUInt32("AUTHNET_MODE2_COMMAND6_STATE_BYTE0", 0) & 0xFF;
        uint32 const stateField4 = GetEnvUInt32("AUTHNET_MODE2_COMMAND6_STATE_FIELD4", 0);

        std::string accountBlobText = GetEnvOrDefault("AUTHNET_MODE2_COMMAND6_ACCOUNT_BLOB_TEXT", "");
        if (accountBlobText.size() > 0x17)
            accountBlobText.resize(0x17);

        std::vector<uint8> accountBlob(accountBlobText.begin(), accountBlobText.end());
        std::vector<uint8> configuredBlob;
        if (TryParseHexBytes(std::getenv("AUTHNET_MODE2_COMMAND6_ACCOUNT_BLOB_HEX"), accountBlob.size(), configuredBlob))
            accountBlob = configuredBlob;

        std::vector<uint8> accountBlobSuffix = { 0, 0 };
        std::string accountBlobSuffixText = GetEnvOrDefault("AUTHNET_MODE2_COMMAND6_ACCOUNT_BLOB_SUFFIX_TEXT", "");
        if (accountBlobSuffixText.size() >= accountBlobSuffix.size())
            std::copy_n(accountBlobSuffixText.begin(), accountBlobSuffix.size(), accountBlobSuffix.begin());

        std::vector<uint8> configuredBlobSuffix;
        if (TryParseHexBytes(std::getenv("AUTHNET_MODE2_COMMAND6_ACCOUNT_BLOB_SUFFIX_HEX"), accountBlobSuffix.size(), configuredBlobSuffix))
            accountBlobSuffix = configuredBlobSuffix;

        writer.WriteBits(0x06, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x02, 4);

        writer.WriteBits(accountByte0, 8);
        writer.WriteUInt32(accountField4);
        writer.WriteUInt32(accountField8);
        writer.WriteBits(uint32(accountBlob.size()), 7);
        accountBlob.insert(accountBlob.end(), accountBlobSuffix.begin(), accountBlobSuffix.end());
        writer.WriteBytes(accountBlob.data(), accountBlob.size());

        writer.WriteBits(0, 21);
        WriteUInt64(writer, serviceStateField8);
        writer.WriteUInt32(serviceStateField0);

        WriteUInt64(writer, stateToken64);
        writer.WriteUInt32(stateField8);
        writer.WriteBits(stateByte0, 8);
        writer.WriteUInt32(stateField4);
        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildMode2Command8StructuredProbe()
    {
        Skyfire::Authnet::BitWriter writer;
        std::vector<AuthnetRealmRoute> const routes = BuildAuthnetRealmRoutes();

        uint32 const fieldValue = GetAuthnetDefaultRealmField(routes);
        uint32 const list6Count = GetMode2Command8List6Count(routes);
        uint32 const list18Count = GetMode2Command8ListCount("AUTHNET_MODE2_COMMAND8_LIST18_COUNT");
        std::vector<uint8> list6 = BuildMode2Command8DefaultRouteList(list6Count, routes);
        std::vector<uint8> configuredList6;
        if (!list6.empty() && TryParseHexBytes(std::getenv("AUTHNET_MODE2_COMMAND8_LIST6_HEX"), list6.size(), configuredList6))
            list6 = configuredList6;

        std::vector<uint8> list18 = GetConfiguredListBytes("AUTHNET_MODE2_COMMAND8_LIST18_HEX", 18, list18Count);

        writer.WriteBits(0x08, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x02, 4);

        writer.WriteBits(0, 1);
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

        uint32 const status = GetEnvUInt32("AUTHNET_MODE2_COMMAND8_STATUS", 0) & 0xFF;

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

    std::vector<uint8> BuildMode1Command2Empty()
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x02, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x01, 4);
        writer.WriteBits(0, 1);
        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildMode1Command2Result(uint32 field0, uint32 field1)
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x02, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x01, 4);
        writer.WriteBits(1, 1);
        writer.WriteUInt32(field0);
        writer.WriteUInt32(field1);
        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> BuildMode1Command3Value(uint32 value)
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x03, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x01, 4);
        writer.WriteUInt32(value);
        writer.AlignToByte();

        return writer.Data();
    }

    std::vector<uint8> GetConfiguredFixedBytes(char const* name, size_t length)
    {
        std::vector<uint8> bytes(length, 0);
        std::vector<uint8> configuredBytes;
        if (TryParseHexBytes(std::getenv(name), bytes.size(), configuredBytes))
            return configuredBytes;

        return bytes;
    }

    std::vector<uint8> BuildMode1Command4Pair()
    {
        Skyfire::Authnet::BitWriter writer;

        std::vector<uint8> first = GetConfiguredFixedBytes("AUTHNET_MODE1_COMMAND6_KEY0", 6);
        std::vector<uint8> second = GetConfiguredFixedBytes("AUTHNET_MODE1_COMMAND6_KEY1", 6);

        writer.WriteBits(0x04, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x01, 4);
        writer.WriteBytes(second.data(), second.size());
        writer.WriteBytes(first.data(), first.size());
        writer.AlignToByte();

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
        return GetEnvOrDefault("AUTHNET_MODE1_COMMAND6_RESPONSE", "none");
    }

    uint16 GetMode1Command6Status()
    {
        return uint16(GetEnvUInt32("AUTHNET_MODE1_COMMAND6_STATUS", 6) & 0xFFFF);
    }

    char const* GetPostLoginMode1ResponseMode()
    {
        return GetEnvOrDefault("AUTHNET_POST_LOGIN_MODE1_SEQUENCE", "none");
    }

    bool ShouldSendMode1SequenceOnModeSwitch(char const* phase)
    {
        char const* mode = GetEnvOrDefault("AUTHNET_SEND_MODE1_SEQUENCE_ON_MODE_SWITCH", "none");
        if (StringEquals(mode, phase))
            return true;

        return StringEnabled(mode) && StringEquals(phase, "after-mode2");
    }

    bool ShouldSendMode1SequenceOnMode2Command0()
    {
        return StringEnabled(GetEnvOrDefault("AUTHNET_SEND_MODE1_SEQUENCE_ON_MODE2_COMMAND0", "1"));
    }

    bool IsMode1ProbeDisabled(char const* responseMode)
    {
        return StringEquals(responseMode, "none") || StringEquals(responseMode, "skip") ||
            StringEquals(responseMode, "off");
    }

    bool AddMode1ProbeResponse(char const* responseMode, uint16 status, uint32 field0, uint32 field1, uint32 value,
        std::vector<std::vector<uint8>>& responses, std::vector<std::string>& responseLabels)
    {
        auto addResponse = [&responses, &responseLabels](char const* label, std::vector<uint8> response)
        {
            responses.push_back(std::move(response));
            responseLabels.push_back(label);
        };

        if (StringEquals(responseMode, "empty"))
        {
            addResponse("empty", BuildEmptyModeCommand(6, 1));
            return true;
        }

        if (StringEquals(responseMode, "command2-empty") || StringEquals(responseMode, "c2-empty"))
        {
            addResponse("command2-empty", BuildMode1Command2Empty());
            return true;
        }

        if (StringEquals(responseMode, "command2-result") || StringEquals(responseMode, "c2-result"))
        {
            addResponse("command2-result", BuildMode1Command2Result(field0, field1));
            return true;
        }

        if (StringEquals(responseMode, "command3-value") || StringEquals(responseMode, "command3-status") ||
            StringEquals(responseMode, "c3-value") || StringEquals(responseMode, "c3-status"))
        {
            addResponse("command3-value", BuildMode1Command3Value(value));
            return true;
        }

        if (StringEquals(responseMode, "command4-pair") || StringEquals(responseMode, "command4-ticket") ||
            StringEquals(responseMode, "c4-pair") || StringEquals(responseMode, "c4-ticket"))
        {
            addResponse("command4-pair", BuildMode1Command4Pair());
            return true;
        }

        if (StringEquals(responseMode, "status"))
        {
            addResponse("status", BuildMode1Command1Status(status));
            return true;
        }

        return false;
    }

    void BuildMode1ProbeResponses(char const* responseMode, uint16 status, uint32 field0, uint32 field1, uint32 value,
        std::vector<std::vector<uint8>>& responses, std::vector<std::string>& responseLabels)
    {
        if (AddMode1ProbeResponse(responseMode, status, field0, field1, value, responses, responseLabels))
            return;

        if (StringEquals(responseMode, "command4-command2") || StringEquals(responseMode, "c4-c2"))
        {
            AddMode1ProbeResponse("command4-pair", status, field0, field1, value, responses, responseLabels);
            AddMode1ProbeResponse("command2-empty", status, field0, field1, value, responses, responseLabels);
            return;
        }

        if (StringEquals(responseMode, "command4-command3") || StringEquals(responseMode, "c4-c3"))
        {
            AddMode1ProbeResponse("command4-pair", status, field0, field1, value, responses, responseLabels);
            AddMode1ProbeResponse("command3-value", status, field0, field1, value, responses, responseLabels);
            return;
        }

        if (StringEquals(responseMode, "command3-command4") || StringEquals(responseMode, "c3-c4"))
        {
            AddMode1ProbeResponse("command3-value", status, field0, field1, value, responses, responseLabels);
            AddMode1ProbeResponse("command4-pair", status, field0, field1, value, responses, responseLabels);
            return;
        }

        if (StringEquals(responseMode, "command2-command3") || StringEquals(responseMode, "c2-c3"))
        {
            AddMode1ProbeResponse("command2-result", status, field0, field1, value, responses, responseLabels);
            AddMode1ProbeResponse("command3-value", status, field0, field1, value, responses, responseLabels);
            return;
        }

        if (StringEquals(responseMode, "command3-command2") || StringEquals(responseMode, "c3-c2"))
        {
            AddMode1ProbeResponse("command3-value", status, field0, field1, value, responses, responseLabels);
            AddMode1ProbeResponse("command2-result", status, field0, field1, value, responses, responseLabels);
            return;
        }

        AddMode1ProbeResponse("status", status, field0, field1, value, responses, responseLabels);
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

    std::vector<uint8> BuildServiceResultPayload(uint32 requestId, std::vector<uint8> const& servicePayloadProbe, uint32 field80);

    std::vector<uint8> BuildServiceResultPayload(ServiceRequestInfo const& request, ServiceResultProbeInfo* probeInfo)
    {
        std::vector<uint8> servicePayloadProbe = BuildServicePayloadProbe(request, probeInfo);
        return BuildServiceResultPayload(request.requestId, servicePayloadProbe, probeInfo ? probeInfo->field80 : 0);
    }

    std::vector<uint8> BuildServiceResultPayload(uint32 requestId, std::vector<uint8> const& servicePayloadProbe, uint32 field80)
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x03, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x0B, 4);
        writer.WriteUInt32(0);
        writer.WriteBits(0, 1);

        writer.WriteBytes(servicePayloadProbe.data(), servicePayloadProbe.size());
        writer.WriteBits(0, 9);
        writer.WriteUInt32(field80);
        writer.WriteBits(0, 1);
        writer.WriteUInt32(requestId);

        writer.AlignToByte();
        return writer.Data();
    }

    std::vector<uint8> BuildServiceLookupResultPayload(uint32 requestId, std::vector<uint8> const& lookupBody)
    {
        Skyfire::Authnet::BitWriter writer;

        writer.WriteBits(0x04, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x0B, 4);
        writer.WriteUInt32(requestId);
        writer.WriteBytes(lookupBody.data(), lookupBody.size());
        writer.AlignToByte();
        return writer.Data();
    }

    std::vector<uint8> BuildServiceCompletePayload(uint32 requestId, std::vector<uint8> const& servicePayloadProbe)
    {
        Skyfire::Authnet::BitWriter writer;
        char const* variantMode = GetEnvOrDefault("AUTHNET_SERVICE_COMPLETE_VARIANT", "zero40");

        writer.WriteBits(0x0A, 6);
        writer.WriteBits(1, 1);
        writer.WriteBits(0x0B, 4);

        if (StringEquals(variantMode, "short16") || StringEquals(variantMode, "word16") ||
            StringEquals(variantMode, "variant1"))
        {
            writer.WriteBits(1, 1);
            writer.WriteBits(GetEnvUInt32("AUTHNET_SERVICE_COMPLETE_WORD", 0) & 0xFFFF, 16);
            writer.WriteUInt32(requestId);
            writer.AlignToByte();
            return writer.Data();
        }

        std::vector<uint8> body(40, 0);
        std::vector<uint8> configuredBody;
        if (TryParseHexBytes(std::getenv("AUTHNET_SERVICE_COMPLETE_BODY_HEX"), body.size(), configuredBody))
            body = configuredBody;
        else
        {
            char const* bodyMode = GetEnvOrDefault("AUTHNET_SERVICE_COMPLETE_BODY", "zero");
            if (StringEquals(bodyMode, "service-payload") || StringEquals(bodyMode, "record"))
            {
                size_t const copySize = std::min(body.size(), servicePayloadProbe.size());
                if (copySize != 0)
                    std::copy(servicePayloadProbe.begin(), servicePayloadProbe.begin() + copySize, body.begin());
            }
        }

        if (char const* tag = std::getenv("AUTHNET_SERVICE_COMPLETE_TAG"))
            body[8] = uint8(GetEnvUInt32("AUTHNET_SERVICE_COMPLETE_TAG", uint32(uint8(tag[0]))) & 0xFF);

        writer.WriteBits(0, 1);
        writer.WriteBytes(body.data(), body.size());
        writer.WriteUInt32(requestId);
        writer.AlignToByte();
        return writer.Data();
    }

    bool ShouldSendPostLoginServiceResult()
    {
        return StringEnabled(std::getenv("AUTHNET_POST_LOGIN_SERVICE_RESULT"));
    }

    std::vector<uint8> GetPostLoginServiceResultPayload()
    {
        std::vector<uint8> payload(22, 0);
        payload[0] = 'N';
        payload[1] = 'u';
        payload[2] = 'l';
        payload[4] = 1;

        std::vector<uint8> configuredPayload;
        if (TryParseHexBytes(std::getenv("AUTHNET_POST_LOGIN_SERVICE_RESULT_PAYLOAD"), payload.size(), configuredPayload))
            return configuredPayload;

        return payload;
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

    struct SelectedRealmRequestInfo
    {
        uint32 connectionSeed = 0;
        uint32 realmField = 0;
        bool hasRealmField = false;
    };

    bool SkipBits(Skyfire::Authnet::BitReader& reader, size_t bitCount)
    {
        while (bitCount > 0)
        {
            uint32 ignored = 0;
            uint32 const chunk = std::min<uint32>(uint32(bitCount), 32);
            if (!reader.ReadBits(chunk, ignored))
                return false;

            bitCount -= chunk;
        }

        return true;
    }

    bool TryDecodeSelectedRealmRequest(std::vector<uint8> const& packet, SelectedRealmRequestInfo& info)
    {
        Skyfire::Authnet::BitReader reader(packet.data(), packet.size());

        ProbePacketHeader header;
        if (!reader.ReadBits(6, header.command) || !reader.ReadBits(1, header.modeSwitch))
            return false;

        if (!header.modeSwitch || !reader.ReadBits(4, header.mode))
            return false;

        if (header.command != 8 || header.mode != 2)
            return false;

        if (!reader.ReadUInt32(info.connectionSeed))
            return false;

        size_t const remainingBits = reader.RemainingBits();
        if (remainingBits >= 32)
        {
            if (!SkipBits(reader, remainingBits - 32))
                return false;

            if (reader.ReadUInt32(info.realmField))
                info.hasRealmField = true;
        }

        return true;
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

    size_t ByteLengthForBits(size_t bitLength)
    {
        return (bitLength + 7) / 8;
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
    socket_(socket), _encryptedBytesProcessed(0), _initialRequestLen(0), _authnetAccountId(0),
    _authnetLocaleId(0), _authnetWorldConnectionSeed(0), _authnetWorldRealmField(0),
    _authnetWorldSessionKey(), _authnetSecret(), _authnetWorldSessionKeyGenerated(false),
    _authnetWorldSessionKeyPersisted(false), _clientCryptI(0), _clientCryptJ(0),
    _clientCryptInitialized(false), _serverCryptI(0), _serverCryptJ(0), _serverCryptInitialized(false),
    _responded(false), _httpResponded(false), _clientModeSwitchSeen(false), _followupLogged(false),
    _postSuccessBurstSeen(false), _mode1ConnectAnswered(false), _mode2LoginAnswered(false),
    _mode2Command2Answered(false), _mode2Command3Answered(false), _mode2Command6Answered(false),
    _mode2Command7Answered(false), _mode2Command8Answered(false), _mode2Command8PostCommand6Scheduled(false),
    _postLoginServiceResultSent(false), _postLoginStatusSent(false), _postLoginMode1SequenceSent(false),
    _mode1Command6Answered(false)
{
}

AuthnetSocket::~AuthnetSocket(void) { }

void AuthnetSocket::OnAccept(void)
{
    SF_LOG_INFO("server.authserver", "'%s:%d' authnet passive probe: connection accepted",
        socket().getRemoteAddress().c_str(), socket().getRemotePort());
}

void AuthnetSocket::OnClose(void)
{
    SF_LOG_INFO("server.authserver", "'%s:%d' authnet passive probe: connection closed, %zu byte(s) captured total, responded=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), _captured.size(),
        _responded ? "yes" : "no");
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
        "'%s:%d' authnet decode: initial_header name=%s command=%u mode_switch=%u mode=%u header_bits=%zu program=%s platform=%s locale=%s components=%zu",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
        AuthnetPacketName(request.header),
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

    if (request.hasIdentity)
        PrepareWorldSessionKey(request.identity, request.platform, request.locale);

    return true;
}

void AuthnetSocket::PrepareWorldSessionKey(std::string const& identity, std::string const& platform, std::string const& locale)
{
    if (!ShouldGenerateWorldSessionKey())
        return;

    uint32 accountId = 0;
    std::string accountName;
    if (!TryResolveAuthnetLoginIdentity(identity, accountId, accountName))
    {
        SF_LOG_ERROR("server.authserver", "'%s:%d' authnet probe: world session key generation requested but identity '%s' did not resolve to an account.",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), identity.c_str());
        return;
    }

    _authnetAccountId = accountId;
    _authnetAccountName = accountName;
    _authnetWorldAccountToken = BuildWorldAccountToken(accountId);
    _authnetLocaleId = GetLocaleByName(locale);
    _authnetOS = GetAuthnetOSFromPlatform(platform);
    _authnetSecret.fill(0);

    PersistAuthnetWorldSessionKey(0, GetAuthnetDefaultRealmField(), "initial-login");
}

void AuthnetSocket::PersistAuthnetWorldSessionKey(uint32 connectionSeed, uint32 realmField, char const* reason)
{
    if (!_authnetAccountId || _authnetWorldAccountToken.empty())
    {
        SF_LOG_ERROR("server.authserver", "'%s:%d' authnet probe: cannot persist world session key before account context is ready.",
            socket().getRemoteAddress().c_str(), socket().getRemotePort());
        return;
    }

    _authnetWorldConnectionSeed = connectionSeed;
    _authnetWorldRealmField = realmField;
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

    uint32 const sessionTtl = GetAuthnetWorldSessionTtlSeconds();
    stmt = LoginDatabase.GetPreparedStatement(LOGIN_REP_AUTHNET_WORLD_SESSION);
    stmt->setUInt32(0, _authnetAccountId);
    stmt->setString(1, _authnetWorldAccountToken);
    stmt->setBinary(2, _authnetWorldSessionKey);
    stmt->setBinary(3, _authnetSecret);
    stmt->setUInt32(4, connectionSeed);
    stmt->setUInt32(5, realmField);
    stmt->setString(6, remoteAddress);
    stmt->setUInt32(7, _authnetLocaleId);
    stmt->setString(8, _authnetOS);
    stmt->setUInt32(9, sessionTtl);
    LoginDatabase.DirectExecute(stmt);

    _authnetWorldSessionKeyPersisted = true;

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: persisted %s world session key for account %u (%s), token=%s, seed=%u, realm_field=%u, ttl=%u, key=%s.",
        remoteAddress.c_str(), socket().getRemotePort(), reason ? reason : "authnet",
        _authnetAccountId, _authnetAccountName.c_str(), _authnetWorldAccountToken.c_str(),
        connectionSeed, realmField, sessionTtl, MaskSessionKey(_authnetWorldSessionKey).c_str());
}

bool AuthnetSocket::TryUpdateWorldSessionKeyFromSelectedRealm(std::vector<uint8> const& packet)
{
    if (!ShouldGenerateWorldSessionKey())
        return false;

    SelectedRealmRequestInfo selectedRealm;
    if (!TryDecodeSelectedRealmRequest(packet, selectedRealm))
    {
        SF_LOG_ERROR("server.authserver", "'%s:%d' authnet probe: selected realm request did not contain a decodable connection seed, request=%s.",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), ByteArrayToHexStr(packet).c_str());
        return false;
    }

    uint32 const selectedRealmField = selectedRealm.hasRealmField ? selectedRealm.realmField : GetAuthnetDefaultRealmField();
    uint32 const keyRealmField = GetAuthnetDefaultRealmField();
    std::string const selectedRealmName = FindAuthnetRealmNameById(selectedRealmField);
    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: selected realm request seed=%u selected_realm_field=%u selected_realm=%s key_realm_field=%u source=%s.",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
        selectedRealm.connectionSeed, selectedRealmField, selectedRealmName.empty() ? "<unknown>" : selectedRealmName.c_str(), keyRealmField,
        selectedRealm.hasRealmField ? "packet" : "default");

    PersistAuthnetWorldSessionKey(selectedRealm.connectionSeed, keyRealmField, "selected-realm");
    return true;
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
    std::lock_guard<std::mutex> lock(_serverCryptMutex);

    if (!_serverCryptInitialized)
    {
        std::memcpy(_serverCryptState, InitialServerToClientCryptState, sizeof(_serverCryptState));
        _serverCryptI = 0;
        _serverCryptJ = 0;
        _serverCryptInitialized = true;
    }

    ApplyStreamCipher(_serverCryptState, _serverCryptI, _serverCryptJ, payload);
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

bool AuthnetSocket::TrySendMode2Command0Probe(char const* trigger, bool sendFollowups)
{
    char const* responseMode = nullptr;
    uint32 status = GetMode2Command0Status();
    std::vector<uint8> response = BuildLoginAuthProbe(status, responseMode);

    if (response.empty())
    {
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: %s-triggered %s (mode2 command0) response disabled by AUTHNET_MODE2_COMMAND0_RESPONSE=%s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
            AuthnetPacketName(2, 0, 1), responseMode);
        return false;
    }

    if (uint32 delayMs = GetMode2Command0DelayMs())
    {
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: delaying %s-triggered %s (mode2 command0) %s response by %u ms",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
            AuthnetPacketName(2, 0, 1), responseMode, delayMs);
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }

    _mode2LoginAnswered = true;

    bool const sendFollowupsBeforeCommand0 = sendFollowups && ShouldSendMode2FollowupsBeforeCommand0();
    if (sendFollowupsBeforeCommand0)
        SendMode2LoginFollowups(trigger);

    std::vector<uint8> plainResponse = response;
    CryptServerPayload(response);

    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted %s-triggered %s (mode2 command0) %s response status=%u plain=%s encrypted=%s",
        socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
        AuthnetPacketName(2, 0, 1), responseMode, status,
        ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

    socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());

    if (sendFollowups && !sendFollowupsBeforeCommand0)
        SendMode2LoginFollowups(trigger);

    return true;
}

void AuthnetSocket::SendMode2LoginFollowups(char const* trigger)
{
    auto sendMode2EmptyCommand = [this, trigger](uint32 command, char const* responseMode, uint32 delayMs, bool& answered)
    {
        if (answered || !ShouldSendMode2EmptyCommandProbe(responseMode))
            return;

        answered = true;

        if (delayMs)
        {
            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: delaying %s-triggered %s (mode2 command%u) %s response by %u ms",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
                AuthnetPacketName(2, command, 1), command, responseMode, delayMs);
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }

        std::vector<uint8> emptyResponse = BuildEmptyModeCommand(command, 2);
        std::vector<uint8> plainEmptyResponse = emptyResponse;
        CryptServerPayload(emptyResponse);

        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted %s-triggered %s (mode2 command%u) %s response plain=%s encrypted=%s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
            AuthnetPacketName(2, command, 1), command, responseMode,
            ByteArrayToHexStr(plainEmptyResponse).c_str(), ByteArrayToHexStr(emptyResponse).c_str());

        socket().QueueSend(reinterpret_cast<char const*>(emptyResponse.data()), emptyResponse.size());
    };

    auto sendMode2Command6 = [this, trigger]()
    {
        char const* responseMode = GetMode2Command6ResponseMode();
        if (_mode2Command6Answered || !ShouldSendMode2Command6StateProbe(responseMode))
            return;

        _mode2Command6Answered = true;

        if (uint32 delayMs = GetMode2Command6DelayMs())
        {
            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: delaying %s-triggered %s (mode2 command6) %s response by %u ms",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
                AuthnetPacketName(2, 6, 1), responseMode, delayMs);
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }

        std::vector<uint8> stateResponse = BuildMode2Command6StateProbe();
        std::vector<uint8> plainStateResponse = stateResponse;
        CryptServerPayload(stateResponse);

        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted %s-triggered %s (mode2 command6) %s response position=%s plain=%s encrypted=%s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
            AuthnetPacketName(2, 6, 1), responseMode, GetMode2Command6Position(),
            ByteArrayToHexStr(plainStateResponse).c_str(), ByteArrayToHexStr(stateResponse).c_str());

        socket().QueueSend(reinterpret_cast<char const*>(stateResponse.data()), stateResponse.size());
    };

    char const* command2Mode = GetMode2Command2ResponseMode();
    if (!_mode2Command2Answered && ShouldSendMode2Command2DetailProbe(command2Mode))
    {
        _mode2Command2Answered = true;

        if (uint32 detailDelayMs = GetMode2Command2DelayMs())
        {
            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: delaying %s-triggered %s (mode2 command2) %s response by %u ms",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
                AuthnetPacketName(2, 2, 1), command2Mode, detailDelayMs);
            std::this_thread::sleep_for(std::chrono::milliseconds(detailDelayMs));
        }

        std::vector<AuthnetRealmRoute> detailRoutes;
        if (ShouldSendMode2Command2RealmDetails(command2Mode))
            detailRoutes = BuildAuthnetRealmRoutes();

        auto sendCommand2Detail = [this, trigger, command2Mode](AuthnetRealmRoute const* route)
        {
            std::vector<uint8> detailResponse = BuildLoginGameAccountDetailProbe(route);
            std::vector<uint8> plainDetailResponse = detailResponse;
            CryptServerPayload(detailResponse);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted %s-triggered %s (mode2 command2) %s response realm_id=%u realm=%s plain=%s encrypted=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
                AuthnetPacketName(2, 2, 1), command2Mode,
                route ? route->realmId : 0, route ? route->name.c_str() : "<default>",
                ByteArrayToHexStr(plainDetailResponse).c_str(), ByteArrayToHexStr(detailResponse).c_str());

            socket().QueueSend(reinterpret_cast<char const*>(detailResponse.data()), detailResponse.size());
        };

        if (detailRoutes.empty())
            sendCommand2Detail(nullptr);
        else
            for (AuthnetRealmRoute const& route : detailRoutes)
                sendCommand2Detail(&route);
    }

    if (StringEquals(GetMode2Command6Position(), "after-command2"))
        sendMode2Command6();

    sendMode2EmptyCommand(3, GetMode2Command3ResponseMode(), GetMode2Command3DelayMs(), _mode2Command3Answered);

    if (StringEquals(GetMode2Command6Position(), "after-command3"))
        sendMode2Command6();

    char const* command8Mode = GetMode2Command8ResponseMode();
    if (!_mode2Command8Answered && ShouldSendMode2Command8StructuredProbe(command8Mode))
    {
        _mode2Command8Answered = true;
        char const* command8StatusMode = GetMode2Command8StatusResponseMode();

        auto sendMode2Command8Status = [&](char const* position)
        {
            std::vector<uint8> statusResponse = BuildMode2Command8StatusProbe();
            std::vector<uint8> plainStatusResponse = statusResponse;
            CryptServerPayload(statusResponse);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted %s-triggered %s (mode2 command8) status %s response status=%u plain=%s encrypted=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
                AuthnetPacketName(2, 8, 1), position,
                GetEnvUInt32("AUTHNET_MODE2_COMMAND8_STATUS", 0) & 0xFF,
                ByteArrayToHexStr(plainStatusResponse).c_str(), ByteArrayToHexStr(statusResponse).c_str());

            socket().QueueSend(reinterpret_cast<char const*>(statusResponse.data()), statusResponse.size());
        };

        if (uint32 command8DelayMs = GetMode2Command8DelayMs())
        {
            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: delaying %s-triggered %s (mode2 command8) %s response by %u ms",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
                AuthnetPacketName(2, 8, 1), command8Mode, command8DelayMs);
            std::this_thread::sleep_for(std::chrono::milliseconds(command8DelayMs));
        }

        if (ShouldSendMode2Command8StatusBeforeStructured(command8StatusMode))
            sendMode2Command8Status("before-structured");

        std::vector<uint8> command8Response = BuildMode2Command8StructuredProbe();
        std::vector<uint8> plainCommand8Response = command8Response;
        CryptServerPayload(command8Response);

        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted %s-triggered %s (mode2 command8) %s response field=%u list6=%u list18=%u plain=%s encrypted=%s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
            AuthnetPacketName(2, 8, 1), command8Mode,
            GetAuthnetDefaultRealmField(),
            GetMode2Command8List6Count(),
            GetMode2Command8ListCount("AUTHNET_MODE2_COMMAND8_LIST18_COUNT"),
            ByteArrayToHexStr(plainCommand8Response).c_str(), ByteArrayToHexStr(command8Response).c_str());

        socket().QueueSend(reinterpret_cast<char const*>(command8Response.data()), command8Response.size());

        if (ShouldSendMode2Command8StatusAfterStructured(command8StatusMode))
            sendMode2Command8Status("after-structured");

        uint32 const repeatCount = GetMode2Command8RepeatCount();
        uint32 const repeatDelayMs = GetMode2Command8RepeatDelayMs();
        if (repeatCount && ShouldSendMode2Command8RepeatAsync())
        {
            std::shared_ptr<RealmSocket> delayedSocket = socket().shared_from_this();
            std::string remoteAddress = socket().getRemoteAddress();
            uint16 remotePort = socket().getRemotePort();
            std::string command8ModeText = command8Mode;
            std::string triggerText = trigger ? trigger : "unknown";

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: scheduling asynchronous %s-triggered %s (mode2 command8) %s repeat count=%u delay_ms=%u",
                remoteAddress.c_str(), remotePort, triggerText.c_str(), AuthnetPacketName(2, 8, 1),
                command8ModeText.c_str(), repeatCount, repeatDelayMs);

            std::thread([this, delayedSocket, remoteAddress, remotePort, command8ModeText, triggerText, repeatCount, repeatDelayMs]()
            {
                for (uint32 repeatIndex = 0; repeatIndex < repeatCount; ++repeatIndex)
                {
                    if (repeatDelayMs)
                        std::this_thread::sleep_for(std::chrono::milliseconds(repeatDelayMs));

                    std::vector<uint8> repeatCommand8Response = BuildMode2Command8StructuredProbe();
                    std::vector<uint8> plainRepeatCommand8Response = repeatCommand8Response;
                    CryptServerPayload(repeatCommand8Response);

                    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted asynchronous %s-triggered %s (mode2 command8) %s repeat %u/%u response field=%u list6=%u list18=%u plain=%s encrypted=%s",
                        remoteAddress.c_str(), remotePort, triggerText.c_str(),
                        AuthnetPacketName(2, 8, 1), command8ModeText.c_str(), repeatIndex + 1, repeatCount,
                        GetAuthnetDefaultRealmField(),
                        GetMode2Command8List6Count(),
                        GetMode2Command8ListCount("AUTHNET_MODE2_COMMAND8_LIST18_COUNT"),
                        ByteArrayToHexStr(plainRepeatCommand8Response).c_str(), ByteArrayToHexStr(repeatCommand8Response).c_str());

                    delayedSocket->QueueSend(reinterpret_cast<char const*>(repeatCommand8Response.data()), repeatCommand8Response.size());
                }
            }).detach();
        }
        else
        {
            for (uint32 repeatIndex = 0; repeatIndex < repeatCount; ++repeatIndex)
            {
                if (repeatDelayMs)
                {
                    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: delaying %s-triggered %s (mode2 command8) repeat %u/%u by %u ms",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
                        AuthnetPacketName(2, 8, 1), repeatIndex + 1, repeatCount, repeatDelayMs);
                    std::this_thread::sleep_for(std::chrono::milliseconds(repeatDelayMs));
                }

                std::vector<uint8> repeatCommand8Response = BuildMode2Command8StructuredProbe();
                std::vector<uint8> plainRepeatCommand8Response = repeatCommand8Response;
                CryptServerPayload(repeatCommand8Response);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted %s-triggered %s (mode2 command8) %s repeat %u/%u response field=%u list6=%u list18=%u plain=%s encrypted=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
                    AuthnetPacketName(2, 8, 1), command8Mode, repeatIndex + 1, repeatCount,
                    GetAuthnetDefaultRealmField(),
                    GetMode2Command8List6Count(),
                    GetMode2Command8ListCount("AUTHNET_MODE2_COMMAND8_LIST18_COUNT"),
                    ByteArrayToHexStr(plainRepeatCommand8Response).c_str(), ByteArrayToHexStr(repeatCommand8Response).c_str());

                socket().QueueSend(reinterpret_cast<char const*>(repeatCommand8Response.data()), repeatCommand8Response.size());
            }
        }
    }

    if (StringEquals(GetMode2Command6Position(), "after-command8"))
        sendMode2Command6();

    uint32 const postCommand6Count = GetMode2Command8PostCommand6Count();
    if (!_mode2Command8PostCommand6Scheduled && postCommand6Count &&
        ShouldSendMode2Command8StructuredProbe(command8Mode))
    {
        _mode2Command8PostCommand6Scheduled = true;

        for (uint32 repeatIndex = 0; repeatIndex < postCommand6Count; ++repeatIndex)
        {
            std::vector<uint8> plainResponse = BuildMode2Command8StructuredProbe();

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: queued delayed %s-triggered %s (mode2 command8) %s post-command6 %u/%u response field=%u list6=%u list18=%u plain=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
                AuthnetPacketName(2, 8, 1), command8Mode, repeatIndex + 1, postCommand6Count,
                GetAuthnetDefaultRealmField(),
                GetMode2Command8List6Count(),
                GetMode2Command8ListCount("AUTHNET_MODE2_COMMAND8_LIST18_COUNT"),
                ByteArrayToHexStr(plainResponse).c_str());
        }

        uint32 const delayMs = GetMode2Command8PostCommand6DelayMs();
        uint32 const gapMs = GetMode2Command8PostCommand6GapMs();
        std::shared_ptr<RealmSocket> delayedSocket = socket().shared_from_this();
        std::string remoteAddress = socket().getRemoteAddress();
        uint16 remotePort = socket().getRemotePort();
        std::string command8ModeText = command8Mode ? command8Mode : "";

        std::thread([this, delayedSocket, postCommand6Count, delayMs, gapMs, remoteAddress,
            remotePort, command8ModeText]() mutable
        {
            if (delayMs)
                std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));

            for (uint32 i = 0; i < postCommand6Count; ++i)
            {
                std::vector<uint8> response = BuildMode2Command8StructuredProbe();
                std::vector<uint8> plainResponse = response;
                CryptServerPayload(response);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending delayed post-command6 %s (mode2 command8) %s response %u/%u delay_ms=%u gap_ms=%u field=%u list6=%u list18=%u plain=%s encrypted=%s",
                    remoteAddress.c_str(), remotePort, AuthnetPacketName(2, 8, 1),
                    command8ModeText.c_str(), i + 1, postCommand6Count, delayMs, gapMs,
                    GetAuthnetDefaultRealmField(),
                    GetMode2Command8List6Count(),
                    GetMode2Command8ListCount("AUTHNET_MODE2_COMMAND8_LIST18_COUNT"),
                    ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

                delayedSocket->QueueSend(reinterpret_cast<char const*>(response.data()), response.size());

                if (gapMs && i + 1 < postCommand6Count)
                    std::this_thread::sleep_for(std::chrono::milliseconds(gapMs));
            }
        }).detach();
    }

    sendMode2EmptyCommand(7, GetMode2Command7ResponseMode(), GetMode2Command7DelayMs(), _mode2Command7Answered);
}

bool AuthnetSocket::TrySendPostLoginMode1Sequence(char const* trigger)
{
    char const* postLoginMode1Mode = GetPostLoginMode1ResponseMode();
    if (_postLoginMode1SequenceSent || IsMode1ProbeDisabled(postLoginMode1Mode))
        return false;

    _postLoginMode1SequenceSent = true;

    uint16 status = uint16(GetEnvUInt32("AUTHNET_POST_LOGIN_MODE1_STATUS", GetMode1Command6Status()) & 0xFFFF);
    uint32 field0 = GetEnvUInt32("AUTHNET_POST_LOGIN_MODE1_FIELD0", GetEnvUInt32("AUTHNET_MODE1_COMMAND6_FIELD0", 0));
    uint32 field1 = GetEnvUInt32("AUTHNET_POST_LOGIN_MODE1_FIELD1", GetEnvUInt32("AUTHNET_MODE1_COMMAND6_FIELD1", 0));
    uint32 value = GetEnvUInt32("AUTHNET_POST_LOGIN_MODE1_VALUE", GetEnvUInt32("AUTHNET_MODE1_COMMAND6_VALUE", status));
    uint32 delayMs = GetEnvUInt32("AUTHNET_POST_LOGIN_MODE1_DELAY_MS", 0);
    uint32 gapMs = GetEnvUInt32("AUTHNET_POST_LOGIN_MODE1_GAP_MS", 0);
    std::vector<std::vector<uint8>> responses;
    std::vector<std::string> responseLabels;

    BuildMode1ProbeResponses(postLoginMode1Mode, status, field0, field1, value, responses, responseLabels);

    if (delayMs || gapMs)
    {
        std::shared_ptr<RealmSocket> delayedSocket = socket().shared_from_this();
        std::string remoteAddress = socket().getRemoteAddress();
        uint16 remotePort = socket().getRemotePort();
        std::string postLoginMode1ModeText = postLoginMode1Mode ? postLoginMode1Mode : "";

        for (size_t i = 0; i < responses.size(); ++i)
        {
            ProbePacketHeader responseHeader = DecodeProbePacketHeader(responses[i]);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: queued encrypted %s-triggered post-login %s %s response %u/%u mode=%s delay_ms=%u gap_ms=%u status=%u field0=%u field1=%u value=%u plain=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
                responseHeader.decoded ? AuthnetPacketName(responseHeader) : "UndecodedMode1Probe",
                responseLabels[i].c_str(), uint32(i + 1), uint32(responses.size()),
                postLoginMode1Mode, delayMs, gapMs, status, field0, field1, value,
                ByteArrayToHexStr(responses[i]).c_str());
        }

        std::thread([this, delayedSocket, responses = std::move(responses),
            responseLabels = std::move(responseLabels), delayMs, gapMs, remoteAddress, remotePort,
            trigger, postLoginMode1ModeText, status, field0, field1, value]() mutable
        {
            if (delayMs)
                std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));

            for (size_t i = 0; i < responses.size(); ++i)
            {
                std::vector<uint8> response = responses[i];
                std::vector<uint8> plainResponse = response;
                ProbePacketHeader responseHeader = DecodeProbePacketHeader(plainResponse);
                CryptServerPayload(response);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending delayed encrypted %s-triggered post-login %s %s response %u/%u mode=%s delay_ms=%u gap_ms=%u status=%u field0=%u field1=%u value=%u plain=%s encrypted=%s",
                    remoteAddress.c_str(), remotePort, trigger,
                    responseHeader.decoded ? AuthnetPacketName(responseHeader) : "UndecodedMode1Probe",
                    responseLabels[i].c_str(), uint32(i + 1), uint32(responses.size()),
                    postLoginMode1ModeText.c_str(), delayMs, gapMs, status, field0, field1, value,
                    ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

                delayedSocket->QueueSend(reinterpret_cast<char const*>(response.data()), response.size());

                if (gapMs && i + 1 < responses.size())
                    std::this_thread::sleep_for(std::chrono::milliseconds(gapMs));
            }
        }).detach();
    }
    else
    {
        for (size_t i = 0; i < responses.size(); ++i)
        {
            std::vector<uint8> response = responses[i];
            std::vector<uint8> plainResponse = response;
            ProbePacketHeader responseHeader = DecodeProbePacketHeader(plainResponse);
            CryptServerPayload(response);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted %s-triggered post-login %s %s response %u/%u mode=%s delay_ms=%u gap_ms=%u status=%u field0=%u field1=%u value=%u plain=%s encrypted=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), trigger,
                responseHeader.decoded ? AuthnetPacketName(responseHeader) : "UndecodedMode1Probe",
                responseLabels[i].c_str(), uint32(i + 1), uint32(responses.size()),
                postLoginMode1Mode, delayMs, gapMs, status, field0, field1, value,
                ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

            socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
        }
    }

    return true;
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

                char const* resourceResultMode = GetResourceResultMode();
                if (IsResourceResultModeDisabled(resourceResultMode))
                {
                    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: localized resource result id=%u disabled by AUTHNET_RESOURCE_RESULT_MODE=%s",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                        lookup.requestId, resourceResultMode);
                    continue;
                }

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

                char const* resourceResultMode = GetResourceResultMode();
                if (IsResourceResultModeDisabled(resourceResultMode))
                {
                    SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: post-success request id=%u result disabled by AUTHNET_RESOURCE_RESULT_MODE=%s",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(), requestId, resourceResultMode);
                    continue;
                }

                SendEncryptedRequestResult(requestId);
            }
        }

        return;
    }

    ProbePacketHeader header = DecodeProbePacketHeader(plain);
    if (header.decoded)
    {
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: decrypted post-success header name=%s command=%u mode_switch=%u mode=%u header_bits=%zu",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(),
            AuthnetPacketName(header),
            header.command, header.modeSwitch, header.mode, header.bitPosition);

        ServiceRequestInfo serviceRequest;
        if (TryDecodeServiceRequest(plain, serviceRequest))
        {
            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: service request name=%s id=%u records=%zu selector=%u label_len=%zu nested_header=%s packet_len=%zu bits=%zu",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                AuthnetPacketName(serviceRequest.header), serviceRequest.requestId,
                serviceRequest.records.size(), serviceRequest.selector, serviceRequest.label.size(),
                serviceRequest.hasNestedHeader ? "yes" : "no", serviceRequest.packetLength,
                serviceRequest.bitLength);

            for (size_t i = 0; i < serviceRequest.records.size(); ++i)
            {
                ServiceRequestRecord const& record = serviceRequest.records[i];
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: service request record[%u]=%s/%s minor=%u major=%u build=%u",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), uint32(i),
                    record.program.c_str(), record.component.c_str(), record.minor, record.major,
                    record.build);
            }

            if (std::find(_answeredRequestIds.begin(), _answeredRequestIds.end(), serviceRequest.requestId) != _answeredRequestIds.end())
                return;

            _answeredRequestIds.push_back(serviceRequest.requestId);

            ServiceResultProbeInfo resultProbe;
            std::vector<uint8> servicePayloadProbe = BuildServicePayloadProbe(serviceRequest, &resultProbe);
            std::vector<uint8> lookupBody = BuildServiceLookupResultBody(serviceRequest, servicePayloadProbe);
            std::vector<std::vector<uint8>> responses;
            std::vector<std::string> responseLabels;
            char const* serviceResponseMode = GetServiceResponseMode();

            auto addResponse = [&responses, &responseLabels](char const* label, std::vector<uint8> response)
            {
                responses.push_back(std::move(response));
                responseLabels.push_back(label);
            };

            if (StringEquals(serviceResponseMode, "lookup-result") || StringEquals(serviceResponseMode, "lookup") ||
                StringEquals(serviceResponseMode, "command4") || StringEquals(serviceResponseMode, "c4"))
            {
                addResponse("lookup-result", BuildServiceLookupResultPayload(serviceRequest.requestId, lookupBody));
            }
            else if (StringEquals(serviceResponseMode, "complete") || StringEquals(serviceResponseMode, "complete-result") ||
                StringEquals(serviceResponseMode, "command10") || StringEquals(serviceResponseMode, "c10"))
            {
                addResponse("complete-result", BuildServiceCompletePayload(serviceRequest.requestId, servicePayloadProbe));
            }
            else if (StringEquals(serviceResponseMode, "lookup-method") || StringEquals(serviceResponseMode, "command4-command3") ||
                StringEquals(serviceResponseMode, "c4-c3"))
            {
                addResponse("lookup-result", BuildServiceLookupResultPayload(serviceRequest.requestId, lookupBody));
                addResponse("method-result", BuildServiceResultPayload(serviceRequest.requestId, servicePayloadProbe, resultProbe.field80));
            }
            else if (StringEquals(serviceResponseMode, "lookup-complete") || StringEquals(serviceResponseMode, "command4-command10") ||
                StringEquals(serviceResponseMode, "c4-c10"))
            {
                addResponse("lookup-result", BuildServiceLookupResultPayload(serviceRequest.requestId, lookupBody));
                addResponse("complete-result", BuildServiceCompletePayload(serviceRequest.requestId, servicePayloadProbe));
            }
            else if (StringEquals(serviceResponseMode, "method-complete") || StringEquals(serviceResponseMode, "command3-command10") ||
                StringEquals(serviceResponseMode, "c3-c10"))
            {
                addResponse("method-result", BuildServiceResultPayload(serviceRequest.requestId, servicePayloadProbe, resultProbe.field80));
                addResponse("complete-result", BuildServiceCompletePayload(serviceRequest.requestId, servicePayloadProbe));
            }
            else if (StringEquals(serviceResponseMode, "lookup-method-complete") ||
                StringEquals(serviceResponseMode, "command4-command3-command10") ||
                StringEquals(serviceResponseMode, "c4-c3-c10"))
            {
                addResponse("lookup-result", BuildServiceLookupResultPayload(serviceRequest.requestId, lookupBody));
                addResponse("method-result", BuildServiceResultPayload(serviceRequest.requestId, servicePayloadProbe, resultProbe.field80));
                addResponse("complete-result", BuildServiceCompletePayload(serviceRequest.requestId, servicePayloadProbe));
            }
            else if (StringEquals(serviceResponseMode, "method-lookup-complete") ||
                StringEquals(serviceResponseMode, "command3-command4-command10") ||
                StringEquals(serviceResponseMode, "c3-c4-c10"))
            {
                addResponse("method-result", BuildServiceResultPayload(serviceRequest.requestId, servicePayloadProbe, resultProbe.field80));
                addResponse("lookup-result", BuildServiceLookupResultPayload(serviceRequest.requestId, lookupBody));
                addResponse("complete-result", BuildServiceCompletePayload(serviceRequest.requestId, servicePayloadProbe));
            }
            else if (StringEquals(serviceResponseMode, "method-lookup") || StringEquals(serviceResponseMode, "command3-command4") ||
                StringEquals(serviceResponseMode, "c3-c4"))
            {
                addResponse("method-result", BuildServiceResultPayload(serviceRequest.requestId, servicePayloadProbe, resultProbe.field80));
                addResponse("lookup-result", BuildServiceLookupResultPayload(serviceRequest.requestId, lookupBody));
            }
            else if (!IsServiceResponseModeDisabled(serviceResponseMode))
            {
                addResponse("method-result", BuildServiceResultPayload(serviceRequest.requestId, servicePayloadProbe, resultProbe.field80));
            }

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: service result source id=%u found=%s record_index=%u/%zu record_mode=%s key_mode=%s key=%s blob_mode=%s blob=%s field80_mode=%s field80=%u source=%s/%s minor=%u major=%u build=%u",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), serviceRequest.requestId,
                resultProbe.hasRecord ? "yes" : "no", resultProbe.recordIndex, serviceRequest.records.size(),
                resultProbe.recordMode, ServiceKeyModeName(resultProbe.keyMode),
                ByteArrayToHexStr(resultProbe.serviceKey).c_str(), resultProbe.blobMode,
                ByteArrayToHexStr(resultProbe.blob).c_str(), resultProbe.field80Mode, resultProbe.field80,
                resultProbe.program.c_str(), resultProbe.component.c_str(), resultProbe.minor,
                resultProbe.major, resultProbe.build);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: service response mode=%s lookup_body=%s response_count=%zu",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), serviceResponseMode,
                ByteArrayToHexStr(lookupBody).c_str(), responses.size());

            for (size_t i = 0; i < responses.size(); ++i)
            {
                std::vector<uint8> plainResponse = responses[i];
                std::vector<uint8> response = responses[i];
                CryptServerPayload(response);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted service %s payload %zu/%zu id=%u plain=%s encrypted=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), responseLabels[i].c_str(), i + 1,
                    responses.size(), serviceRequest.requestId, ByteArrayToHexStr(plainResponse).c_str(),
                    ByteArrayToHexStr(response).c_str());

                socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
            }
            return;
        }

        if (plain.size() == ClientModeSwitchRequestLen &&
            header.command == 0 && header.modeSwitch && header.mode == 1)
        {
            _mode1ConnectAnswered = true;

            std::vector<uint8> response = BuildEmptyModeCommand(0, 1);
            std::vector<uint8> plainResponse = response;
            CryptServerPayload(response);

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted %s (mode1 command0) response plain=%s encrypted=%s",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                AuthnetPacketName(1, 0, 1),
                ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

            socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());

            if (StringEnabled(std::getenv("AUTHNET_SEND_MODE1_SEQUENCE_ON_COMMAND0")))
                TrySendPostLoginMode1Sequence("mode1 command0");
        }

        if (plain.size() == ClientModeSwitchRequestLen &&
            header.command == 0 && header.modeSwitch && header.mode == 2)
        {
            if (!TrySendMode2Command0Probe("mode2 command0", true))
                return;

            if (!_postLoginServiceResultSent && ShouldSendPostLoginServiceResult())
            {
                _postLoginServiceResultSent = true;

                uint32 requestId = GetEnvUInt32("AUTHNET_POST_LOGIN_SERVICE_RESULT_ID", 1);
                uint32 field80 = GetEnvUInt32("AUTHNET_POST_LOGIN_SERVICE_RESULT_FIELD80", 0);
                std::vector<uint8> servicePayload = GetPostLoginServiceResultPayload();
                std::vector<uint8> serviceResultResponse = BuildServiceResultPayload(requestId, servicePayload, field80);
                std::vector<uint8> plainServiceResultResponse = serviceResultResponse;
                CryptServerPayload(serviceResultResponse);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted post-login %s (mode11 command3) id=%u field80=%u service_payload=%s plain=%s encrypted=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    AuthnetPacketName(11, 3, 1), requestId, field80,
                    ByteArrayToHexStr(servicePayload).c_str(),
                    ByteArrayToHexStr(plainServiceResultResponse).c_str(), ByteArrayToHexStr(serviceResultResponse).c_str());

                socket().QueueSend(reinterpret_cast<char const*>(serviceResultResponse.data()), serviceResultResponse.size());
            }

            uint16 postLoginStatus = 0;
            if (!_postLoginStatusSent && TryGetPostLoginMode1Command1Status(postLoginStatus))
            {
                _postLoginStatusSent = true;

                std::vector<uint8> statusResponse = BuildMode1Command1Status(postLoginStatus);
                std::vector<uint8> plainStatusResponse = statusResponse;
                CryptServerPayload(statusResponse);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted post-login %s (mode1 command1) status=%u plain=%s encrypted=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    AuthnetPacketName(1, 1, 1), postLoginStatus,
                    ByteArrayToHexStr(plainStatusResponse).c_str(), ByteArrayToHexStr(statusResponse).c_str());

                socket().QueueSend(reinterpret_cast<char const*>(statusResponse.data()), statusResponse.size());
            }

            if (ShouldSendMode1SequenceOnMode2Command0())
                TrySendPostLoginMode1Sequence("mode2 command0");
        }

        if (plain.size() > ClientModeSwitchRequestLen &&
            header.command == 8 && header.modeSwitch && header.mode == 2)
        {
            TryUpdateWorldSessionKeyFromSelectedRealm(plain);

            char const* responseMode = GetMode2Command8RequestResponseMode();
            if (StringEquals(responseMode, "none") || StringEquals(responseMode, "skip") ||
                StringEquals(responseMode, "off"))
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: selected realm %s (mode2 command8) request observed; response disabled by AUTHNET_MODE2_COMMAND8_REQUEST_RESPONSE=%s plain=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    AuthnetPacketName(header), responseMode, ByteArrayToHexStr(plain).c_str());
                return;
            }

            std::vector<std::vector<uint8>> responses;
            std::vector<std::string> responseLabels;
            auto addResponse = [&responses, &responseLabels](char const* label, std::vector<uint8> response)
            {
                responses.push_back(std::move(response));
                responseLabels.push_back(label);
            };

            if (StringEquals(responseMode, "status"))
                addResponse("mode2-command8-status", BuildMode2Command8StatusProbe());
            else if (StringEquals(responseMode, "empty"))
                addResponse("mode2-command8-empty", BuildEmptyModeCommand(8, 2));
            else if (StringEquals(responseMode, "state") || StringEquals(responseMode, "command6") ||
                StringEquals(responseMode, "mode2-command6"))
                addResponse("mode2-command6-state", BuildMode2Command6StateProbe());
            else if (StringEquals(responseMode, "mode1-status"))
                addResponse("mode1-status", BuildMode1Command1Status(GetMode1Command6Status()));
            else if (StringEquals(responseMode, "mode1-sequence"))
            {
                TrySendPostLoginMode1Sequence("selected-realm command8");
                return;
            }
            else if (StringEquals(responseMode, "structured-state"))
            {
                addResponse("mode2-command8-structured", BuildMode2Command8StructuredProbe());
                addResponse("mode2-command6-state", BuildMode2Command6StateProbe());
            }
            else if (StringEquals(responseMode, "state-structured"))
            {
                addResponse("mode2-command6-state", BuildMode2Command6StateProbe());
                addResponse("mode2-command8-structured", BuildMode2Command8StructuredProbe());
            }
            else
                addResponse("mode2-command8-structured", BuildMode2Command8StructuredProbe());

            uint32 const delayMs = GetMode2Command8RequestDelayMs();
            uint32 const gapMs = GetMode2Command8RequestGapMs();
            if (delayMs)
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: delaying selected-realm %s (mode2 command8) %s response by %u ms",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    AuthnetPacketName(header), responseMode, delayMs);
                std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
            }

            for (size_t i = 0; i < responses.size(); ++i)
            {
                if (gapMs && i != 0)
                    std::this_thread::sleep_for(std::chrono::milliseconds(gapMs));

                std::vector<uint8> response = responses[i];
                std::vector<uint8> plainResponse = response;
                CryptServerPayload(response);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted selected-realm %s (mode2 command8) %s response %u/%u request=%s plain=%s encrypted=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    AuthnetPacketName(header), responseLabels[i].c_str(), uint32(i + 1),
                    uint32(responses.size()), ByteArrayToHexStr(plain).c_str(),
                    ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

                socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
            }

            return;
        }

        if (plain.size() == ClientModeSwitchRequestLen &&
            header.command == 6 && header.modeSwitch && header.mode == 1)
        {
            _mode1Command6Answered = true;

            char const* responseMode = GetMode1Command6ResponseMode();
            if (IsMode1ProbeDisabled(responseMode))
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: %s (mode1 command6) observed; response disabled by AUTHNET_MODE1_COMMAND6_RESPONSE=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    AuthnetPacketName(1, 6, 1), responseMode);
                return;
            }

            uint16 status = GetMode1Command6Status();
            uint32 field0 = GetEnvUInt32("AUTHNET_MODE1_COMMAND6_FIELD0", 0);
            uint32 field1 = GetEnvUInt32("AUTHNET_MODE1_COMMAND6_FIELD1", 0);
            uint32 value = GetEnvUInt32("AUTHNET_MODE1_COMMAND6_VALUE", status);
            std::vector<std::vector<uint8>> responses;
            std::vector<std::string> responseLabels;

            BuildMode1ProbeResponses(responseMode, status, field0, field1, value, responses, responseLabels);

            for (size_t i = 0; i < responses.size(); ++i)
            {
                std::vector<uint8> response = responses[i];
                std::vector<uint8> plainResponse = response;
                CryptServerPayload(response);

                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: sending encrypted %s (mode1 command6) %s response %u/%u status=%u field0=%u field1=%u value=%u plain=%s encrypted=%s",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                    AuthnetPacketName(1, 6, 1), responseLabels[i].c_str(),
                    uint32(i + 1), uint32(responses.size()),
                    status, field0, field1, value,
                    ByteArrayToHexStr(plainResponse).c_str(), ByteArrayToHexStr(response).c_str());

                socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
            }
        }
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

        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: HTTP request received on authnet port, sending empty HTTP response and closing",
            socket().getRemoteAddress().c_str(), socket().getRemotePort());

        socket().QueueSend(EmptyHttpResponse, sizeof(EmptyHttpResponse) - 1, true);
        _httpResponded = true;
        _responded = true;
        return;
    }

    if (!DecodeInitialRequest())
        return;

    if (!_responded)
    {
        std::string startupAccountName;
        std::vector<uint8> startupAccountKey;
        std::string worldAccountIdentity;
        std::vector<uint8> response = BuildStartupResponseProbe(_authnetWorldAccountToken, &startupAccountName, &startupAccountKey,
            &worldAccountIdentity);
        std::string startupAccountKeyHex = startupAccountKey.empty() ? std::string() : ByteArrayToHexStr(startupAccountKey);
        std::string responseHex = ByteArrayToHexStr(response);

        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: initial request detected, sending startup response candidate %zu-byte response account_count=%u account_name=%s account_key=%s auth_session_identity=%s generated_key=%u persisted_key=%u plain=%s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), response.size(),
            startupAccountName.empty() ? 0u : 1u, startupAccountName.c_str(),
            startupAccountKeyHex.c_str(), worldAccountIdentity.c_str(),
            _authnetWorldSessionKeyGenerated ? 1u : 0u, _authnetWorldSessionKeyPersisted ? 1u : 0u,
            responseHex.c_str());

        socket().QueueSend(reinterpret_cast<char const*>(response.data()), response.size());
        _responded = true;
    }

    if (!_clientModeSwitchSeen && _captured.size() >= _initialRequestLen + ClientModeSwitchRequestLen)
    {
        std::vector<uint8> followup(_captured.begin() + _initialRequestLen,
            _captured.begin() + _initialRequestLen + ClientModeSwitchRequestLen);
        ProbePacketHeader header = DecodeProbePacketHeader(followup);

        if (header.decoded)
        {
            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: client mode switch header name=%s command=%u mode_switch=%u mode=%u header_bits=%zu",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                AuthnetPacketName(header),
                header.command, header.modeSwitch, header.mode, header.bitPosition);
        }

        if (header.decoded && header.command == 5 && header.modeSwitch && header.mode == 1)
        {
            _clientModeSwitchSeen = true;

            SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: client mode switch detected, waiting for encrypted follow-up",
                socket().getRemoteAddress().c_str(), socket().getRemotePort());

            if (ShouldSendMode1SequenceOnModeSwitch("before-mode2"))
                TrySendPostLoginMode1Sequence("mode-switch-before-mode2");

            if (StringEnabled(std::getenv("AUTHNET_SEND_MODE2_ON_MODE_SWITCH")))
            {
                bool const sendFollowups = StringEnabled(GetEnvOrDefault("AUTHNET_SEND_MODE2_ON_MODE_SWITCH_FOLLOWUPS", "1"));
                TrySendMode2Command0Probe("mode-switch", sendFollowups);
            }

            if (ShouldSendMode1SequenceOnModeSwitch("after-mode2"))
                TrySendPostLoginMode1Sequence("mode-switch-after-mode2");
        }
    }

    size_t encryptedFollowupOffset = _initialRequestLen + (_clientModeSwitchSeen ? ClientModeSwitchRequestLen : 0);
    if (_clientModeSwitchSeen)
        ProcessEncryptedClientBytes(encryptedFollowupOffset);

    if (!_followupLogged && _captured.size() >= encryptedFollowupOffset + FirstEncryptedFollowupLen)
    {
        std::vector<uint8> followup(_captured.begin() + encryptedFollowupOffset,
            _captured.begin() + encryptedFollowupOffset + FirstEncryptedFollowupLen);

        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: first encrypted post-success bytes detected: %s",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), ByteArrayToHexStr(followup).c_str());

        _followupLogged = true;
    }

    size_t resourceLookupBurstBytes = EncryptedResourceLookupRequestLen * ResourceLookupBurstCount;
    size_t postResourceLookupOffset = encryptedFollowupOffset + resourceLookupBurstBytes;

    if (_clientModeSwitchSeen && !_postSuccessBurstSeen)
    {
        size_t encryptedFollowupBytes = _captured.size() > encryptedFollowupOffset ? _captured.size() - encryptedFollowupOffset : 0;
        if (encryptedFollowupBytes >= resourceLookupBurstBytes)
        {
            _postSuccessBurstSeen = true;

            if (!ShouldSendBurstMode2Response())
            {
                SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: post-success lookup burst reached %zu byte(s), burst %s (mode2 command0) response disabled",
                    socket().getRemoteAddress().c_str(), socket().getRemotePort(), encryptedFollowupBytes,
                    AuthnetPacketName(2, 0, 1));
                return;
            }

            TrySendMode2Command0Probe("burst-complete", StringEnabled(std::getenv("AUTHNET_SEND_BURST_MODE2_FOLLOWUPS")));
        }
    }

    if (_postSuccessBurstSeen && readOffset >= postResourceLookupOffset && readSize != ClientModeSwitchRequestLen)
    {
        SF_LOG_INFO("server.authserver", "'%s:%d' authnet probe: post-success request length %zu observed after burst",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), readSize);
    }
}
