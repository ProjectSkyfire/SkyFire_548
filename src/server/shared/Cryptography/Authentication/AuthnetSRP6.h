/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _AUTHNET_SRP6_H
#define _AUTHNET_SRP6_H

#include "BigNumber.h"
#include "CryptoHash.h"
#include "Define.h"

#include <array>
#include <optional>
#include <string>

namespace SkyFire::Crypto
{
    class AuthnetSRP6
    {
    public:
        static constexpr size_t SALT_LENGTH = 32;
        static constexpr size_t VERIFIER_LENGTH = 128;
        static constexpr size_t EPHEMERAL_KEY_LENGTH = 128;
        static constexpr size_t SESSION_KEY_LENGTH = 64;

        using Salt = std::array<uint8, SALT_LENGTH>;
        using Verifier = std::array<uint8, VERIFIER_LENGTH>;
        using EphemeralKey = std::array<uint8, EPHEMERAL_KEY_LENGTH>;
        using SessionKey = std::array<uint8, SESSION_KEY_LENGTH>;
        using Proof = SHA256::Digest;

        struct RegistrationData
        {
            Salt salt;
            Verifier verifier;
        };

        // identity and password must be passed through Utf8ToUpperOnlyLatin first.
        static RegistrationData MakeRegistrationData(std::string const& identity, std::string const& password);
        static Proof GetSessionVerifier(EphemeralKey const& A, Proof const& clientProof, SessionKey const& sessionKey);

        AuthnetSRP6(std::string const& identity, Salt const& salt, Verifier const& verifier);
        std::optional<SessionKey> VerifyChallengeResponse(EphemeralKey const& A, Proof const& clientProof);

        SHA256::Digest const& GetIdentityHash() const { return _identityHash; }
        Salt const& GetSalt() const { return _salt; }
        EphemeralKey const& GetServerPublicValue() const { return _serverPublicValue; }

    private:
        static BigNumber MakePrivateValue();
        static EphemeralKey CalculateServerPublicValue(BigNumber const& privateValue, BigNumber const& verifier);
        static SessionKey SHA256Interleave(EphemeralKey const& sharedSecret);

        static std::array<uint8, 1> const Generator;
        static std::array<uint8, EPHEMERAL_KEY_LENGTH> const Modulus;
        static BigNumber const _generator;
        static BigNumber const _modulus;
        static BigNumber const _multiplier;

        bool _used = false;
        SHA256::Digest const _identityHash;
        BigNumber const _privateValue;
        BigNumber const _verifier;
        Salt const _salt;
        EphemeralKey const _serverPublicValue;
    };

    class AuthnetPasswordProof
    {
    public:
        static constexpr size_t VALUE_LENGTH = 128;

        using Value = std::array<uint8, VALUE_LENGTH>;

        explicit AuthnetPasswordProof(std::string const& privateKeyHex);

        bool IsValid() const { return _valid; }
        Value const& GetChallenge() const { return _challenge; }
        Value const& GetPublicKey() const { return _publicKey; }
        Value Sign(std::string const& identity, Value const& clientChallenge) const;

    private:
        static std::array<uint8, 1> const Generator;
        static Value const Modulus;
        static BigNumber const _generator;
        static BigNumber const _modulus;
        static BigNumber const _order;

        BigNumber _privateKey;
        BigNumber _nonce;
        Value _challenge = { };
        Value _publicKey = { };
        bool _valid = false;
    };
}

#endif
