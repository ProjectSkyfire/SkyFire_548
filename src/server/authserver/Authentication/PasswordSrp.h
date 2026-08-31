/*
 * Password.dll (bnet AuthModule) SRP-6a helpers for Soft cmd=2 challenge / op=3.
 * Constants and formulas from PasswordSafe IDA (CreateModule → 61080, proof 61430,
 * op3 61D10 / 61220 / 629D0). Password is uppercased (6CC70) before x.
 */

#ifndef SF_PASSWORD_SRP_H
#define SF_PASSWORD_SRP_H

#include "BigNumber.h"
#include "Define.h"
#include <array>
#include <optional>
#include <string>
#include <vector>

namespace SkyFire::Authnet
{
    class PasswordSrp
    {
    public:
        static constexpr size_t SALT_LEN = 32;
        static constexpr size_t USER_SEED_LEN = 32;
        static constexpr size_t EPHEMERAL_LEN = 128;
        static constexpr size_t PROOF_LEN = 32;
        static constexpr size_t SESSION_KEY_LEN = 64;
        static constexpr size_t PROP205_LEN = 128;

        using Salt = std::array<uint8, SALT_LEN>;
        using UserSeed = std::array<uint8, USER_SEED_LEN>;
        using Ephemeral = std::array<uint8, EPHEMERAL_LEN>;
        using Proof = std::array<uint8, PROOF_LEN>;
        using SessionKey = std::array<uint8, SESSION_KEY_LEN>;
        using Prop205 = std::array<uint8, PROP205_LEN>;

        // SHA256("SkyFire.Authnet.FixedProp205.v1") repeated 4 times.
        // Must match SkyFireLauncher AuthnetProp205Hook.FixedBytes.
        static constexpr uint8 kAuthnetFixedProp205[PROP205_LEN] = {
            0x29, 0x4A, 0x1F, 0xEC, 0xB9, 0xCB, 0x9C, 0x7D, 0xF5, 0xA1, 0x94, 0xBD, 0x6C, 0x3E, 0xA9, 0xEB,
            0x74, 0xBD, 0x87, 0xD8, 0x4B, 0xF5, 0xD7, 0x11, 0x6E, 0xE9, 0x06, 0x1B, 0xDC, 0x43, 0x8B, 0x36,
            0x29, 0x4A, 0x1F, 0xEC, 0xB9, 0xCB, 0x9C, 0x7D, 0xF5, 0xA1, 0x94, 0xBD, 0x6C, 0x3E, 0xA9, 0xEB,
            0x74, 0xBD, 0x87, 0xD8, 0x4B, 0xF5, 0xD7, 0x11, 0x6E, 0xE9, 0x06, 0x1B, 0xDC, 0x43, 0x8B, 0x36,
            0x29, 0x4A, 0x1F, 0xEC, 0xB9, 0xCB, 0x9C, 0x7D, 0xF5, 0xA1, 0x94, 0xBD, 0x6C, 0x3E, 0xA9, 0xEB,
            0x74, 0xBD, 0x87, 0xD8, 0x4B, 0xF5, 0xD7, 0x11, 0x6E, 0xE9, 0x06, 0x1B, 0xDC, 0x43, 0x8B, 0x36,
            0x29, 0x4A, 0x1F, 0xEC, 0xB9, 0xCB, 0x9C, 0x7D, 0xF5, 0xA1, 0x94, 0xBD, 0x6C, 0x3E, 0xA9, 0xEB,
            0x74, 0xBD, 0x87, 0xD8, 0x4B, 0xF5, 0xD7, 0x11, 0x6E, 0xE9, 0x06, 0x1B, 0xDC, 0x43, 0x8B, 0x36
        };

        // Module SRP group (unk_3896E390): g=2, 1024-bit N.
        // All module BNs (62CE0/63000) are little-endian.
        static BigNumber const& N();
        static BigNumber const& g();
        // k = SHA256(N_file || g) LE-loaded (module+128).
        static BigNumber const& k();
        // NgHash = SHA256(g) xor SHA256(N) (32 bytes, module+0).
        static Proof const& NgHash();
        // 629D0 group (unk_3896E280 / g=4 / unk_3896E310).
        static BigNumber const& ProofN();
        static BigNumber const& ProofG();
        static BigNumber const& ProofK();

        static std::string HexUsernameFromSeed(UserSeed const& seed);
        static UserSeed UserSeedFromIdentity(std::string const& identity);

        // HMAC-SHA512(key=accountName, data=prop205) → 64 bytes → BN (629D0).
        static BigNumber HmacUsernameProp205(std::string const& accountName, Prop205 const& prop205);

        // second = (g^S * k^H) mod ProofN, with S < ProofN/2 and QR(second).
        static bool BuildSecondField(std::string const& accountName, Prop205 const& prop205,
            Ephemeral& outSecond, Ephemeral& outS);

        struct Challenge
        {
            UserSeed userSeed{};
            Salt salt{};
            Ephemeral B{};          // little-endian on wire (63000)
            Ephemeral second{};     // little-endian on wire (63000)
            Ephemeral proofS{};     // private S for Soft2 op=3 128-byte field
            bool secondMatchesProp205 = false; // true if second/proofS satisfy 629D0
            std::string srpUsername; // 64-char hex of userSeed
            std::string accountName; // HMAC / Auth prop 1509466876
            BigNumber b;            // server ephemeral secret
            BigNumber v;            // password verifier
        };

        struct ClientProof
        {
            Ephemeral A{};    // little-endian on wire
            Proof M1{};
            Prop205 prop205{};
        };

        static bool ParseClientProof(std::vector<uint8> const& blob, ClientProof& out);
        static std::string BuildChallengeMessage(Challenge const& ch);
        static std::string BuildServerProofMessage(Proof const& M2, Ephemeral const& proofS);

        // password = cleartext from RequestPassword; accountName for 629D0 HMAC.
        static std::optional<Challenge> MakeChallenge(std::string const& identity,
            std::string const& password, std::string const& accountName,
            Prop205 const* prop205OrNull, Salt const* saltOrNull = nullptr);

        // True when challenge.second == (g^proofS * k^H(account, prop205)) mod ProofN.
        static bool SecondMatchesProp205(Challenge const& ch, Prop205 const& prop205);

        // Verify Soft2 uplink and produce Soft2 op=3 payload (1+32+128).
        // On failure, failReason is a short tag; expectedM1 is filled on M1_mismatch.
        // secondMismatchOut is set when M1 ok but 629D0 will fail (#103).
        static std::optional<std::string> VerifyAndBuildOp3(Challenge const& ch,
            ClientProof const& client, Proof& outM2, SessionKey& outK,
            char const** failReason = nullptr, Proof* expectedM1 = nullptr,
            bool* secondMismatchOut = nullptr);
    };
}

#endif
