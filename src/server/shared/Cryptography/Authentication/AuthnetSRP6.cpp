/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AuthnetSRP6.h"
#include "HMAC.h"
#include "Util.h"

#include <algorithm>
#include <cctype>
#include <functional>
#include <initializer_list>

#include <openssl/crypto.h>

namespace
{
    SkyFire::Crypto::SHA256::Digest DigestBigNumbers(std::initializer_list<BigNumber const*> values)
    {
        SkyFire::Crypto::SHA256 hash;
        for (BigNumber const* value : values)
        {
            std::vector<uint8> bytes = value->ToByteVector();
            hash.UpdateData(bytes);
        }

        hash.Finalize();
        return hash.GetDigest();
    }
}

using AuthnetSRP6 = SkyFire::Crypto::AuthnetSRP6;
using SHA256 = SkyFire::Crypto::SHA256;

std::array<uint8, 1> const AuthnetSRP6::Generator = { 2 };
std::array<uint8, AuthnetSRP6::EPHEMERAL_KEY_LENGTH> const AuthnetSRP6::Modulus =
{
    0xAB, 0x24, 0x43, 0x63, 0xA9, 0xC2, 0xA6, 0xC3, 0x3B, 0x37, 0xE4, 0x61, 0x84, 0x25, 0x9F, 0x8B,
    0x3F, 0xCB, 0x8A, 0x85, 0x27, 0xFC, 0x3D, 0x87, 0xBE, 0xA0, 0x54, 0xD2, 0x38, 0x5D, 0x12, 0xB7,
    0x61, 0x44, 0x2E, 0x83, 0xFA, 0xC2, 0x21, 0xD9, 0x10, 0x9F, 0xC1, 0x9F, 0xEA, 0x50, 0xE3, 0x09,
    0xA6, 0xE5, 0x5E, 0x23, 0xA7, 0x77, 0xEB, 0x00, 0xC7, 0xBA, 0xBF, 0xF8, 0x55, 0x8A, 0x0E, 0x80,
    0x2B, 0x14, 0x1A, 0xA2, 0xD4, 0x43, 0xA9, 0xD4, 0xAF, 0xAD, 0xB5, 0xE1, 0xF5, 0xAC, 0xA6, 0x13,
    0x1C, 0x69, 0x78, 0x64, 0x0B, 0x7B, 0xAF, 0x9C, 0xC5, 0x50, 0x31, 0x8A, 0x23, 0x08, 0x01, 0xA1,
    0xF5, 0xFE, 0x31, 0x32, 0x7F, 0xE2, 0x05, 0x82, 0xD6, 0x0B, 0xED, 0x4D, 0x55, 0x32, 0x41, 0x94,
    0x29, 0x6F, 0x55, 0x7D, 0xE3, 0x0F, 0x77, 0x19, 0xE5, 0x6C, 0x30, 0xEB, 0xDE, 0xF6, 0xA7, 0x86
};
BigNumber const AuthnetSRP6::_generator(AuthnetSRP6::Generator);
BigNumber const AuthnetSRP6::_modulus(AuthnetSRP6::Modulus);
BigNumber const AuthnetSRP6::_multiplier(DigestBigNumbers({ &AuthnetSRP6::_modulus, &AuthnetSRP6::_generator }));

AuthnetSRP6::RegistrationData AuthnetSRP6::MakeRegistrationData(std::string const& identity, std::string const& password)
{
    RegistrationData result;

    BigNumber salt;
    salt.SetRand(SALT_LENGTH * 8);
    result.salt = salt.ToByteArray<SALT_LENGTH>();

    SHA256::Digest const identityHash = SHA256::GetDigestOf(identity);
    SHA256::Digest const passwordHash = SHA256::GetDigestOf(ByteArrayToHexStr(identityHash), ":", password);
    BigNumber passwordValue(passwordHash);
    BigNumber x(DigestBigNumbers({ &salt, &passwordValue }));
    result.verifier = _generator.ModExp(x, _modulus).ToByteArray<VERIFIER_LENGTH>();

    return result;
}

AuthnetSRP6::Proof AuthnetSRP6::GetSessionVerifier(
    EphemeralKey const& A, Proof const& clientProof, SessionKey const& sessionKey)
{
    BigNumber clientPublicValue(A);
    BigNumber proof(clientProof);
    BigNumber key(sessionKey);
    return DigestBigNumbers({ &clientPublicValue, &proof, &key });
}

BigNumber AuthnetSRP6::MakePrivateValue()
{
    BigNumber value;
    value.SetRand(EPHEMERAL_KEY_LENGTH * 8);
    return value;
}

AuthnetSRP6::EphemeralKey AuthnetSRP6::CalculateServerPublicValue(
    BigNumber const& privateValue, BigNumber const& verifier)
{
    return (((verifier * _multiplier) + _generator.ModExp(privateValue, _modulus)) % _modulus)
        .ToByteArray<EPHEMERAL_KEY_LENGTH>();
}

AuthnetSRP6::SessionKey AuthnetSRP6::SHA256Interleave(EphemeralKey const& sharedSecret)
{
    std::array<uint8, EPHEMERAL_KEY_LENGTH / 2> even;
    std::array<uint8, EPHEMERAL_KEY_LENGTH / 2> odd;
    for (size_t i = 0; i < even.size(); ++i)
    {
        even[i] = sharedSecret[i * 2];
        odd[i] = sharedSecret[i * 2 + 1];
    }

    SHA256::Digest const evenHash = SHA256::GetDigestOf(even);
    SHA256::Digest const oddHash = SHA256::GetDigestOf(odd);

    SessionKey key;
    for (size_t i = 0; i < SHA256::DIGEST_LENGTH; ++i)
    {
        key[i * 2] = evenHash[i];
        key[i * 2 + 1] = oddHash[i];
    }

    return key;
}

AuthnetSRP6::AuthnetSRP6(std::string const& identity, Salt const& salt, Verifier const& verifier)
    : _identityHash(SHA256::GetDigestOf(identity)), _privateValue(MakePrivateValue()), _verifier(verifier),
      _salt(salt), _serverPublicValue(CalculateServerPublicValue(_privateValue, _verifier))
{
}

std::optional<AuthnetSRP6::SessionKey> AuthnetSRP6::VerifyChallengeResponse(
    EphemeralKey const& A, Proof const& clientProof)
{
    ASSERT(!_used);
    _used = true;

    BigNumber clientPublicValue(A);
    if ((clientPublicValue % _modulus).isZero())
        return std::nullopt;

    BigNumber serverPublicValue(_serverPublicValue);
    BigNumber u(DigestBigNumbers({ &clientPublicValue, &serverPublicValue }));
    EphemeralKey const sharedSecret = ((clientPublicValue * _verifier.ModExp(u, _modulus)) % _modulus)
        .ModExp(_privateValue, _modulus).ToByteArray<EPHEMERAL_KEY_LENGTH>();
    SessionKey const sessionKey = SHA256Interleave(sharedSecret);

    SHA256::Digest const modulusHash = DigestBigNumbers({ &_modulus });
    SHA256::Digest const generatorHash = DigestBigNumbers({ &_generator });
    SHA256::Digest modulusGeneratorHash;
    std::transform(modulusHash.begin(), modulusHash.end(), generatorHash.begin(), modulusGeneratorHash.begin(),
        std::bit_xor<>());

    SHA256::Digest const identityHash = SHA256::GetDigestOf(ByteArrayToHexStr(_identityHash));
    BigNumber salt(_salt);
    BigNumber key(sessionKey);

    SHA256 proofHash;
    proofHash.UpdateData(modulusGeneratorHash);
    proofHash.UpdateData(identityHash);
    for (BigNumber const* value : { &salt, &clientPublicValue, &serverPublicValue, &key })
    {
        std::vector<uint8> bytes = value->ToByteVector();
        proofHash.UpdateData(bytes);
    }
    proofHash.Finalize();

    if (CRYPTO_memcmp(proofHash.GetDigest().data(), clientProof.data(), clientProof.size()) != 0)
        return std::nullopt;

    return sessionKey;
}

using AuthnetPasswordProof = SkyFire::Crypto::AuthnetPasswordProof;

std::array<uint8, 1> const AuthnetPasswordProof::Generator = { 4 };
AuthnetPasswordProof::Value const AuthnetPasswordProof::Modulus =
{
    0xA3, 0x48, 0xF5, 0xDC, 0xFB, 0x38, 0x2D, 0x0B, 0x88, 0xA3, 0xC9, 0x32, 0xE9, 0x31, 0x58, 0x15,
    0x6E, 0x98, 0x17, 0x73, 0x3B, 0xF0, 0xA4, 0x02, 0x8E, 0x0E, 0xC4, 0x32, 0x0B, 0x9B, 0xE1, 0x3A,
    0xDB, 0x6B, 0x4F, 0x04, 0x4C, 0x0E, 0xF1, 0x11, 0xD3, 0xFF, 0x27, 0x92, 0xED, 0xC4, 0x08, 0xFB,
    0x7B, 0x42, 0x4D, 0x49, 0xE5, 0x72, 0xEC, 0xB5, 0x6D, 0x9D, 0x2A, 0x96, 0x55, 0x85, 0x44, 0x45,
    0xED, 0x75, 0x68, 0x13, 0xAC, 0x7C, 0x18, 0xB7, 0xB6, 0xCC, 0xB8, 0x53, 0x79, 0x47, 0x73, 0x9B,
    0x19, 0x95, 0x5F, 0x8F, 0x18, 0xBA, 0x4F, 0xCA, 0xC5, 0xDD, 0xDF, 0xA1, 0xF1, 0xCD, 0x0F, 0x5D,
    0x6B, 0xA2, 0xBA, 0xEB, 0x6D, 0x51, 0x33, 0xA6, 0x9A, 0xC2, 0xBE, 0x34, 0x50, 0xDA, 0xA1, 0x70,
    0x2B, 0xB2, 0xDA, 0x7E, 0xA5, 0xFF, 0xA3, 0x67, 0x43, 0x10, 0x80, 0x24, 0x86, 0x63, 0x34, 0x8D
};
BigNumber const AuthnetPasswordProof::_generator(AuthnetPasswordProof::Generator);
BigNumber const AuthnetPasswordProof::_modulus(AuthnetPasswordProof::Modulus);
BigNumber const AuthnetPasswordProof::_order(AuthnetPasswordProof::_modulus / BigNumber(2));

AuthnetPasswordProof::AuthnetPasswordProof(std::string const& privateKeyHex)
{
    if (privateKeyHex.empty() || privateKeyHex.size() > VALUE_LENGTH * 2 ||
        !std::all_of(privateKeyHex.begin(), privateKeyHex.end(), [](unsigned char value) { return std::isxdigit(value) != 0; }))
        return;

    BigNumber configuredPrivateKey(privateKeyHex);
    _privateKey = configuredPrivateKey % _order;
    if (_privateKey.isZero())
        return;

    do
    {
        _nonce.SetRand(VALUE_LENGTH * 8);
        _nonce %= _order;
    } while (_nonce.isZero());

    _challenge = _generator.ModExp(_nonce, _modulus).ToByteArray<VALUE_LENGTH>();
    _publicKey = _generator.ModExp(_privateKey, _modulus).ToByteArray<VALUE_LENGTH>();
    _valid = true;
}

AuthnetPasswordProof::Value AuthnetPasswordProof::Sign(
    std::string const& identity, Value const& clientChallenge) const
{
    ASSERT(_valid);

    SkyFire::Crypto::HMAC_SHA512::Digest const digest =
        SkyFire::Crypto::HMAC_SHA512::GetDigestOf(identity, clientChallenge);
    BigNumber challengeValue(digest);
    BigNumber privateContribution = (_privateKey * challengeValue) % _order;
    BigNumber signature = (_nonce + _order - privateContribution) % _order;
    return signature.ToByteArray<VALUE_LENGTH>();
}
