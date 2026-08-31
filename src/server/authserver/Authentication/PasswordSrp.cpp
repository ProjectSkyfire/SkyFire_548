/*
 * Password.dll SRP-6a — see PasswordSrp.h.
 *
 * Password.dll BN import (62CE0 / 63000) and export (630C0 / 62D80) are
 * little-endian. Rdata constants (6E390 / 6E280 / 6E310) and SHA digests
 * loaded into BNs use the same LE convention. Wire B / second / A / proofS
 * are LE. Hash *inputs* (salt, I, NgHash, A/B byte buffers) stay in the
 * order stored on the props / wire.
 */

#include "PasswordSrp.h"
#include "CryptoHash.h"
#include "CryptoRandom.h"
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <algorithm>
#include <cstring>

namespace SkyFire::Authnet
{
namespace
{
    // Module N (unk_3896E390) — file byte order; BN load is LE (62CE0).
    uint8 const kModuleN[128] = {
        0xAB, 0x24, 0x43, 0x63, 0xA9, 0xC2, 0xA6, 0xC3, 0x3B, 0x37, 0xE4, 0x61, 0x84, 0x25, 0x9F, 0x8B,
        0x3F, 0xCB, 0x8A, 0x85, 0x27, 0xFC, 0x3D, 0x87, 0xBE, 0xA0, 0x54, 0xD2, 0x38, 0x5D, 0x12, 0xB7,
        0x61, 0x44, 0x2E, 0x83, 0xFA, 0xC2, 0x21, 0xD9, 0x10, 0x9F, 0xC1, 0x9F, 0xEA, 0x50, 0xE3, 0x09,
        0xA6, 0xE5, 0x5E, 0x23, 0xA7, 0x77, 0xEB, 0x00, 0xC7, 0xBA, 0xBF, 0xF8, 0x55, 0x8A, 0x0E, 0x80,
        0x2B, 0x14, 0x1A, 0xA2, 0xD4, 0x43, 0xA9, 0xD4, 0xAF, 0xAD, 0xB5, 0xE1, 0xF5, 0xAC, 0xA6, 0x13,
        0x1C, 0x69, 0x78, 0x64, 0x0B, 0x7B, 0xAF, 0x9C, 0xC5, 0x50, 0x31, 0x8A, 0x23, 0x08, 0x01, 0xA1,
        0xF5, 0xFE, 0x31, 0x32, 0x7F, 0xE2, 0x05, 0x82, 0xD6, 0x0B, 0xED, 0x4D, 0x55, 0x32, 0x41, 0x94,
        0x29, 0x6F, 0x55, 0x7D, 0xE3, 0x0F, 0x77, 0x19, 0xE5, 0x6C, 0x30, 0xEB, 0xDE, 0xF6, 0xA7, 0x86
    };

    // 629D0 N (unk_3896E280) — file byte order; BN load is LE (63000).
    uint8 const kProofN[128] = {
        0xA3, 0x48, 0xF5, 0xDC, 0xFB, 0x38, 0x2D, 0x0B, 0x88, 0xA3, 0xC9, 0x32, 0xE9, 0x31, 0x58, 0x15,
        0x6E, 0x98, 0x17, 0x73, 0x3B, 0xF0, 0xA4, 0x02, 0x8E, 0x0E, 0xC4, 0x32, 0x0B, 0x9B, 0xE1, 0x3A,
        0xDB, 0x6B, 0x4F, 0x04, 0x4C, 0x0E, 0xF1, 0x11, 0xD3, 0xFF, 0x27, 0x92, 0xED, 0xC4, 0x08, 0xFB,
        0x7B, 0x42, 0x4D, 0x49, 0xE5, 0x72, 0xEC, 0xB5, 0x6D, 0x9D, 0x2A, 0x96, 0x55, 0x85, 0x44, 0x45,
        0xED, 0x75, 0x68, 0x13, 0xAC, 0x7C, 0x18, 0xB7, 0xB6, 0xCC, 0xB8, 0x53, 0x79, 0x47, 0x73, 0x9B,
        0x19, 0x95, 0x5F, 0x8F, 0x18, 0xBA, 0x4F, 0xCA, 0xC5, 0xDD, 0xDF, 0xA1, 0xF1, 0xCD, 0x0F, 0x5D,
        0x6B, 0xA2, 0xBA, 0xEB, 0x6D, 0x51, 0x33, 0xA6, 0x9A, 0xC2, 0xBE, 0x34, 0x50, 0xDA, 0xA1, 0x70,
        0x2B, 0xB2, 0xDA, 0x7E, 0xA5, 0xFF, 0xA3, 0x67, 0x43, 0x10, 0x80, 0x24, 0x86, 0x63, 0x34, 0x8D
    };

    // 629D0 k (unk_3896E310) — file byte order; BN load is LE.
    uint8 const kProofK[128] = {
        0x71, 0x68, 0x3A, 0x6C, 0xFF, 0x38, 0xF8, 0x55, 0x80, 0x38, 0xFF, 0x6A, 0x2D, 0x4C, 0x30, 0xF9,
        0x52, 0xE9, 0xCB, 0xB0, 0x2F, 0x7D, 0xDE, 0x3A, 0x23, 0xFF, 0xCD, 0xC1, 0x8B, 0xD9, 0xC4, 0x4E,
        0x58, 0xA6, 0xD5, 0x68, 0xDC, 0x30, 0xF9, 0xB1, 0x56, 0xEB, 0x92, 0x1B, 0x94, 0xD3, 0x10, 0x6F,
        0x2C, 0x9F, 0x55, 0x17, 0x02, 0x93, 0xC5, 0x56, 0xB6, 0xC0, 0xFC, 0x30, 0x2B, 0xFA, 0x8D, 0xA0,
        0x4B, 0xCD, 0xF0, 0xD7, 0x8D, 0x96, 0xB8, 0xE9, 0x6B, 0xF8, 0xAD, 0x89, 0x52, 0xFC, 0xF3, 0xA6,
        0x48, 0x8E, 0xFE, 0x58, 0x16, 0xA2, 0xAA, 0xA3, 0xD9, 0xCE, 0xCB, 0xFF, 0x39, 0xC5, 0x6A, 0x26,
        0x1A, 0x00, 0x9A, 0x3D, 0x8D, 0x06, 0x54, 0x6E, 0xAE, 0x8A, 0xF2, 0x67, 0x57, 0xA4, 0x5F, 0xCF,
        0x15, 0x3A, 0xB9, 0xCE, 0x3D, 0xEC, 0xCA, 0x3B, 0x54, 0x7D, 0x9E, 0xAB, 0x7B, 0x25, 0x68, 0x42
    };

    char const kHexDigits[] = "0123456789ABCDEF";

    using SHA256 = SkyFire::Crypto::SHA256;

    PasswordSrp::SessionKey SHA256Interleave(std::vector<uint8> const& S)
    {
        // Password.dll sub_389612F0 — SHA256 even/odd split → 64-byte K.
        size_t len = S.size();
        size_t offset = 0;
        if (len & 1)
        {
            ++offset;
            --len;
        }
        size_t half = len / 2;
        if (half > 128)
            half = 128;

        std::array<uint8, 128> even{}, odd{};
        for (size_t i = 0; i < half; ++i)
            even[i] = S[offset + 2 * i];
        for (size_t i = 0; i < half; ++i)
            odd[i] = S[offset + 2 * i + 1];

        SHA256::Digest const h0 = SHA256::GetDigestOf(even.data(), half);
        SHA256::Digest const h1 = SHA256::GetDigestOf(odd.data(), half);

        PasswordSrp::SessionKey K{};
        for (size_t i = 0; i < 32; ++i)
        {
            K[2 * i] = h0[i];
            K[2 * i + 1] = h1[i];
        }
        return K;
    }

    bool IsQuadraticResidue(BigNumber const& value, BigNumber const& modulus)
    {
        // value^((N-1)/2) ≡ 1 (mod N)
        BigNumber exp(modulus);
        BigNumber two(2);
        exp /= two; // integer divide → (N-1)/2 for odd N
        BigNumber legendre = value.ModExp(exp, modulus);
        return BN_is_one(legendre.BN()) == 1;
    }
}

BigNumber const& PasswordSrp::N()
{
    static BigNumber n = [] {
        BigNumber v;
        v.SetBinary(kModuleN, 128, true);
        return v;
    }();
    return n;
}

BigNumber const& PasswordSrp::g()
{
    static BigNumber g = BigNumber(uint32(2));
    return g;
}

BigNumber const& PasswordSrp::k()
{
    static BigNumber kMul = [] {
        // 61080: SHA256(N_file || g_file) then 62CE0 LE-load of digest.
        uint8 buf[129];
        std::memcpy(buf, kModuleN, 128);
        buf[128] = 2;
        SHA256::Digest digest = SHA256::GetDigestOf(buf, sizeof(buf));
        BigNumber v;
        v.SetBinary(digest.data(), int32(digest.size()), true);
        return v;
    }();
    return kMul;
}

PasswordSrp::Proof const& PasswordSrp::NgHash()
{
    static Proof ng = [] {
        // 61080: SHA256(g_file) xor SHA256(N_file) — raw file bytes.
        uint8 gByte = 2;
        SHA256::Digest gHash = SHA256::GetDigestOf(&gByte, 1);
        SHA256::Digest nHash = SHA256::GetDigestOf(kModuleN, 128);
        Proof out{};
        for (size_t i = 0; i < out.size(); ++i)
            out[i] = gHash[i] ^ nHash[i];
        return out;
    }();
    return ng;
}

BigNumber const& PasswordSrp::ProofN()
{
    static BigNumber n = [] {
        BigNumber v;
        v.SetBinary(kProofN, 128, true);
        return v;
    }();
    return n;
}

BigNumber const& PasswordSrp::ProofG()
{
    static BigNumber g = BigNumber(uint32(4));
    return g;
}

BigNumber const& PasswordSrp::ProofK()
{
    static BigNumber kConst = [] {
        BigNumber v;
        v.SetBinary(kProofK, 128, true);
        return v;
    }();
    return kConst;
}

std::string PasswordSrp::HexUsernameFromSeed(UserSeed const& seed)
{
    std::string out;
    out.resize(seed.size() * 2);
    for (size_t i = 0; i < seed.size(); ++i)
    {
        out[2 * i] = kHexDigits[seed[i] >> 4];
        out[2 * i + 1] = kHexDigits[seed[i] & 0xF];
    }
    return out;
}

PasswordSrp::UserSeed PasswordSrp::UserSeedFromIdentity(std::string const& identity)
{
    UserSeed seed{};
    size_t n = std::min(identity.size(), seed.size());
    std::memcpy(seed.data(), identity.data(), n);
    return seed;
}

BigNumber PasswordSrp::HmacUsernameProp205(std::string const& accountName, Prop205 const& prop205)
{
    // sub_38962810 → digest; 63000 LE-loads into BN (629D0).
    unsigned int outLen = 64;
    uint8 out[64];
    HMAC(EVP_sha512(),
        accountName.data(), int(accountName.size()),
        prop205.data(), prop205.size(),
        out, &outLen);

    BigNumber bn;
    bn.SetBinary(out, int32(outLen), true);
    return bn;
}

bool PasswordSrp::BuildSecondField(std::string const& accountName, Prop205 const& prop205,
    Ephemeral& outSecond, Ephemeral& outS)
{
    BigNumber const& n = ProofN();
    BigNumber halfN(n);
    halfN /= BigNumber(uint32(2));

    BigNumber H = HmacUsernameProp205(accountName, prop205);
    BigNumber kH = ProofK().ModExp(H, n);
    // g=4 is a square ⇒ g^S is always QR; second=g^S*k^H is QR iff k^H is.
    if (!IsQuadraticResidue(kH, n))
        return false;

    for (int attempt = 0; attempt < 64; ++attempt)
    {
        SkyFire::Crypto::GetRandomBytes(outS);
        BigNumber S;
        S.SetBinary(outS.data(), int32(outS.size()), true);
        S %= n;
        if (S.isZero() || BN_cmp(S.BN(), halfN.BN()) >= 0)
            continue;

        outS = S.ToByteArray<EPHEMERAL_LEN>(true);

        BigNumber gS = ProofG().ModExp(S, n);
        BigNumber second = (gS * kH) % n;
        if (second.isZero())
            continue;

        outSecond = second.ToByteArray<EPHEMERAL_LEN>(true);
        return true;
    }
    return false;
}

bool PasswordSrp::ParseClientProof(std::vector<uint8> const& blob, ClientProof& out)
{
    // Soft2 uplink module blob: op=2 + A(128 LE) + M1(32) + prop205(128).
    if (blob.size() < 1 + EPHEMERAL_LEN + PROOF_LEN + PROP205_LEN)
        return false;
    if (blob[0] != 2)
        return false;

    std::memcpy(out.A.data(), &blob[1], EPHEMERAL_LEN);
    std::memcpy(out.M1.data(), &blob[1 + EPHEMERAL_LEN], PROOF_LEN);
    std::memcpy(out.prop205.data(), &blob[1 + EPHEMERAL_LEN + PROOF_LEN], PROP205_LEN);
    return true;
}

std::string PasswordSrp::BuildChallengeMessage(Challenge const& ch)
{
    std::string blob(1 + USER_SEED_LEN + SALT_LEN + EPHEMERAL_LEN + EPHEMERAL_LEN, '\0');
    blob[0] = 0;
    std::memcpy(&blob[1], ch.userSeed.data(), USER_SEED_LEN);
    std::memcpy(&blob[1 + USER_SEED_LEN], ch.salt.data(), SALT_LEN);
    std::memcpy(&blob[1 + USER_SEED_LEN + SALT_LEN], ch.B.data(), EPHEMERAL_LEN);
    std::memcpy(&blob[1 + USER_SEED_LEN + SALT_LEN + EPHEMERAL_LEN], ch.second.data(), EPHEMERAL_LEN);
    return blob;
}

std::string PasswordSrp::BuildServerProofMessage(Proof const& M2, Ephemeral const& proofS)
{
    std::string blob(1 + PROOF_LEN + EPHEMERAL_LEN, '\0');
    blob[0] = 3;
    std::memcpy(&blob[1], M2.data(), PROOF_LEN);
    std::memcpy(&blob[1 + PROOF_LEN], proofS.data(), EPHEMERAL_LEN);
    return blob;
}

std::optional<PasswordSrp::Challenge> PasswordSrp::MakeChallenge(std::string const& identity,
    std::string const& password, std::string const& accountName,
    Prop205 const* prop205OrNull, Salt const* saltOrNull)
{
    if (password.empty() || accountName.empty())
        return std::nullopt;

    Challenge ch;
    ch.userSeed = UserSeedFromIdentity(identity);
    ch.srpUsername = HexUsernameFromSeed(ch.userSeed);
    ch.accountName = accountName;

    if (saltOrNull)
        ch.salt = *saltOrNull;
    else
        SkyFire::Crypto::GetRandomBytes(ch.salt);

    // x = SHA256(salt || SHA256(I || ":" || password)); 63000 LE-loads digest.
    // Password.dll 622B0 runs 6CC70 (toupper) on the password before 61430.
    std::string passwordUpper = password;
    for (char& c : passwordUpper)
    {
        if (c >= 'a' && c <= 'z')
            c = char(c - 32);
    }
    SHA256::Digest inner = SHA256::GetDigestOf(ch.srpUsername, std::string(":"), passwordUpper);
    SHA256::Digest xDigest = SHA256::GetDigestOf(ch.salt, inner);
    BigNumber x;
    x.SetBinary(xDigest.data(), int32(xDigest.size()), true);

    ch.v = g().ModExp(x, N());
    ch.b.SetRand(256);
    // B = (k*v + g^b) mod N
    BigNumber B = ((k() * ch.v) + g().ModExp(ch.b, N())) % N();
    if ((B % N()).isZero())
        return std::nullopt;
    ch.B = B.ToByteArray<EPHEMERAL_LEN>(true);
    ch.secondMatchesProp205 = false;

    if (prop205OrNull)
    {
        if (BuildSecondField(accountName, *prop205OrNull, ch.second, ch.proofS))
            ch.secondMatchesProp205 = true;
        else
        {
            // prop205/HMAC may not yield a QR second (629D0) for this
            // accountName; still emit a real SRP B so Soft2 M1/M2 can be
            // tested. Log second_629d0=no in that case.
            prop205OrNull = nullptr;
        }
    }

    if (!ch.secondMatchesProp205)
    {
        // Without matching prop205, emit a random QR second so challenge still
        // parses. Soft2 op=3 629D0 fails unless launcher Soft mode (or an env
        // override) used the same 128 bytes in BuildSecondField.
        bool found = false;
        for (int i = 0; i < 64; ++i)
        {
            SkyFire::Crypto::GetRandomBytes(ch.second);
            BigNumber second;
            second.SetBinary(ch.second.data(), int32(ch.second.size()), true);
            second %= ProofN();
            if (!second.isZero() && IsQuadraticResidue(second, ProofN()))
            {
                ch.second = second.ToByteArray<EPHEMERAL_LEN>(true);
                SkyFire::Crypto::GetRandomBytes(ch.proofS);
                found = true;
                break;
            }
        }
        if (!found)
        {
            SkyFire::Crypto::GetRandomBytes(ch.second);
            SkyFire::Crypto::GetRandomBytes(ch.proofS);
        }
    }

    return ch;
}

std::optional<std::string> PasswordSrp::VerifyAndBuildOp3(Challenge const& ch,
    ClientProof const& client, Proof& outM2, SessionKey& outK,
    char const** failReason, Proof* expectedM1, bool* secondMismatchOut)
{
    if (failReason)
        *failReason = nullptr;

    BigNumber A;
    A.SetBinary(client.A.data(), int32(client.A.size()), true);
    if ((A % N()).isZero())
    {
        if (failReason)
            *failReason = "A_mod_N_zero";
        return std::nullopt;
    }

    BigNumber B;
    B.SetBinary(ch.B.data(), int32(ch.B.size()), true);

    // u = SHA256(A_bytes || B_bytes) using on-wire byte order; LE-load digest.
    SHA256::Digest uDigest = SHA256::GetDigestOf(client.A, ch.B);
    BigNumber u;
    u.SetBinary(uDigest.data(), int32(uDigest.size()), true);

    // S = (A * v^u)^b mod N — export little-endian significant bytes (62D80).
    BigNumber S = (A * ch.v.ModExp(u, N())).ModExp(ch.b, N());
    std::vector<uint8> Sbytes = S.ToByteVector(0, true);
    outK = SHA256Interleave(Sbytes);

    // M1 = SHA256(NgHash || SHA256(I) || salt || A || B || K)
    SHA256::Digest Ihash = SHA256::GetDigestOf(ch.srpUsername);
    Proof ourM1 = SHA256::GetDigestOf(NgHash(), Ihash, ch.salt, client.A, ch.B, outK);
    if (ourM1 != client.M1)
    {
        if (failReason)
            *failReason = "M1_mismatch";
        if (expectedM1)
            *expectedM1 = ourM1;
        return std::nullopt;
    }

    // M2 = SHA256(A || M1 || K)
    outM2 = SHA256::GetDigestOf(client.A, client.M1, outK);

    Ephemeral proofS = ch.proofS;
    if (secondMismatchOut)
        *secondMismatchOut = false;
    if (!ch.proofS.empty() && !SecondMatchesProp205(ch, client.prop205))
    {
        // Challenge second was not derived from this session's prop205.
        // Still send M2; client ReportError(103) on 629D0. Stock Wow
        // CryptGenRandoms prop205 after the challenge is already on the wire;
        // SkyFire Launcher Soft mode overwrites it with kAuthnetFixedProp205.
        if (secondMismatchOut)
            *secondMismatchOut = true;
    }

    return BuildServerProofMessage(outM2, proofS);
}

bool PasswordSrp::SecondMatchesProp205(Challenge const& ch, Prop205 const& prop205)
{
    if (ch.proofS.empty())
        return false;

    BigNumber H = HmacUsernameProp205(ch.accountName, prop205);
    BigNumber Sbn;
    Sbn.SetBinary(ch.proofS.data(), int32(ch.proofS.size()), true);
    BigNumber expect = (ProofG().ModExp(Sbn, ProofN()) * ProofK().ModExp(H, ProofN())) % ProofN();
    BigNumber second;
    second.SetBinary(ch.second.data(), int32(ch.second.size()), true);
    return BN_cmp(expect.BN(), second.BN()) == 0;
}
}
