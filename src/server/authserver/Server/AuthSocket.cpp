/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include <algorithm>

#include "AuthCodes.h"
#include "AuthPatchTransfer.h"
#include "AuthSocket.h"
#include "ByteBuffer.h"
#include "Common.h"
#include "CryptoRandom.h"
#include "CryptoHash.h"
#include "Configuration/Config.h"
#include "Database/DatabaseEnv.h"
#include "LoginIdentity.h"
#include "Log.h"
#include "NetworkAddress.h"
#include "openssl/crypto.h"
#include "RealmList.h"
#include "TOTP.h"

enum eAuthCmd
{
    AUTH_LOGON_CHALLENGE = 0x00,
    AUTH_LOGON_PROOF = 0x01,
    AUTH_RECONNECT_CHALLENGE = 0x02,
    AUTH_RECONNECT_PROOF = 0x03,
    REALM_LIST = 0x10,
    XFER_ACCEPT = 0x32,
    XFER_RESUME = 0x33,
    XFER_CANCEL = 0x34
};

enum eStatus
{
    STATUS_CONNECTED = 0,
    STATUS_AUTHED
};

// GCC has alternative #pragma pack(N) syntax. Old GCC versions do not support
// pack(push, N), and some platforms do not support it at all.
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

typedef struct AUTH_LOGON_CHALLENGE_C
{
    uint8   cmd;
    uint8   error;
    uint16  size;
    uint8   gamename[4];
    uint8   version1;
    uint8   version2;
    uint8   version3;
    uint16  build;
    uint8   platform[4];
    uint8   os[4];
    uint8   country[4];
    uint32  timezone_bias;
    uint32  ip;
    uint8   I_len;
    uint8   I[1];
} sAuthLogonChallenge_C;

typedef struct AUTH_LOGON_PROOF_C
{
    uint8   cmd;
    SkyFire::Crypto::SRP6::EphemeralKey A;
    SkyFire::Crypto::SHA1::Digest clientM;
    SkyFire::Crypto::SHA1::Digest crc_hash;
    uint8   number_of_keys;
    uint8   securityFlags;                                  // 0x00-0x04
} sAuthLogonProof_C;

typedef struct AUTH_LOGON_PROOF_S
{
    uint8   cmd;
    uint8   error;
    SkyFire::Crypto::SHA1::Digest M2;
    uint32  unk1;
    uint32  unk2;
    uint16  unk3;
} sAuthLogonProof_S;

typedef struct AUTH_LOGON_PROOF_S_OLD
{
    uint8   cmd;
    uint8   error;
    SkyFire::Crypto::SHA1::Digest M2;
    uint32  unk2;
} sAuthLogonProof_S_Old;

typedef struct AUTH_RECONNECT_PROOF_C
{
    uint8   cmd;
    uint8   R1[16];
    SkyFire::Crypto::SHA1::Digest R2, R3;
    uint8   number_of_keys;
} sAuthReconnectProof_C;

typedef struct AuthHandler
{
    eAuthCmd cmd;
    uint32 status;
    bool (AuthSocket::* handler)(void);
} AuthHandler;

// GCC has alternative #pragma pack() syntax. Old GCC versions do not support
// pack(pop), and some platforms do not support it at all.
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

const AuthHandler table[] =
{
    { AUTH_LOGON_CHALLENGE,     STATUS_CONNECTED, &AuthSocket::_HandleLogonChallenge    },
    { AUTH_LOGON_PROOF,         STATUS_CONNECTED, &AuthSocket::_HandleLogonProof        },
    { AUTH_RECONNECT_CHALLENGE, STATUS_CONNECTED, &AuthSocket::_HandleReconnectChallenge},
    { AUTH_RECONNECT_PROOF,     STATUS_CONNECTED, &AuthSocket::_HandleReconnectProof    },
    { REALM_LIST,               STATUS_AUTHED,    &AuthSocket::_HandleRealmList         },
    { XFER_ACCEPT,              STATUS_CONNECTED, &AuthSocket::_HandleXferAccept        },
    { XFER_RESUME,              STATUS_CONNECTED, &AuthSocket::_HandleXferResume        },
    { XFER_CANCEL,              STATUS_CONNECTED, &AuthSocket::_HandleXferCancel        }
};

#define AUTH_TOTAL_COMMANDS 8

namespace
{
    bool ReadAuthChallengeFrame(RealmSocket& socket, std::vector<uint8>& packet, uint16& remaining)
    {
        const size_t headerSize = 4;
        if (socket.GetAvailableBytes() < headerSize)
            return false;

        std::vector<uint8> header(headerSize);
        if (!socket.PeekBytes(&header[0], header.size()))
            return false;

        EndianConvertPtr<uint16>(&header[0]);
        remaining = ((sAuthLogonChallenge_C*)&header[0])->size;

        if (remaining < sizeof(sAuthLogonChallenge_C) - headerSize)
        {
            socket.DiscardBytes(headerSize);
            return false;
        }

        if (socket.GetAvailableBytes() < headerSize + remaining)
            return false;

        packet.resize(headerSize + remaining + 1);
        packet[packet.size() - 1] = 0;

        if (!socket.ReadBytes(&packet[0], packet.size() - 1))
            return false;

        EndianConvertPtr<uint16>(&packet[0]);
        return true;
    }
}

// Constructor - set the N and g values for SRP6
AuthSocket::AuthSocket(RealmSocket& socket) :
    socket_(socket), _authed(false), _build(0),
    _expversion(0), _accountSecurityLevel(AccountTypes::SEC_PLAYER)
{
}

// Close patch file descriptor before leaving
AuthSocket::~AuthSocket(void) { }

// Accept the connection
void AuthSocket::OnAccept(void)
{
    SF_LOG_DEBUG("server.authserver", "'%s:%d' Accepting connection",
        socket().getRemoteAddress().c_str(), socket().getRemotePort());
}

void AuthSocket::OnClose(void)
{
    SF_LOG_DEBUG("server.authserver", "AuthSocket::OnClose");
}

// Read the packet from the client
void AuthSocket::OnRead()
{
#define MAX_AUTH_LOGON_CHALLENGES_IN_A_ROW 3
    uint32 challengesInARow = 0;
    uint8 _cmd = 0;
    while (1)
    {
        if (!socket().PeekBytes(&_cmd, 1))
            return;

        if (_cmd == AUTH_LOGON_CHALLENGE)
        {
            ++challengesInARow;
            if (challengesInARow == MAX_AUTH_LOGON_CHALLENGES_IN_A_ROW)
            {
                SF_LOG_WARN("server.authserver", "Got %u AUTH_LOGON_CHALLENGE in a row from '%s', possible ongoing DoS",
                    challengesInARow, socket().getRemoteAddress().c_str());
                socket().Close();
                return;
            }
        }

        size_t i;

        // Circle through known commands and call the correct command handler
        for (i = 0; i < AUTH_TOTAL_COMMANDS; ++i)
        {
            if ((uint8)table[i].cmd == _cmd &&
                (table[i].status == STATUS_CONNECTED || (_authed && table[i].status == STATUS_AUTHED)))
            {
                SF_LOG_DEBUG("server.authserver", "Got data for cmd %u recv length %u",
                    (uint32)_cmd, (uint32)socket().GetAvailableBytes());

                if (!(*this.*table[i].handler)())
                {
                    SF_LOG_DEBUG("server.authserver", "Command handler failed for cmd %u recv length %u",
                        (uint32)_cmd, (uint32)socket().GetAvailableBytes());
                    return;
                }
                break;
            }
        }

        // Report unknown packets in the error log
        if (i == AUTH_TOTAL_COMMANDS)
        {
            SF_LOG_ERROR("server.authserver", "Got unknown packet from '%s'", socket().getRemoteAddress().c_str());
            socket().Close();
            return;
        }
    }
}

// Logon Challenge command handler
bool AuthSocket::_HandleLogonChallenge()
{
    SF_LOG_DEBUG("server.authserver", "Entering _HandleLogonChallenge");
    std::vector<uint8> buf;
    uint16 remaining = 0;
    if (!ReadAuthChallengeFrame(socket(), buf, remaining))
        return false;

    SF_LOG_DEBUG("server.authserver", "[AuthChallenge] got header, body is %#04x bytes", remaining);
    sAuthLogonChallenge_C* ch = (sAuthLogonChallenge_C*)&buf[0];

    SF_LOG_DEBUG("server.authserver", "[AuthChallenge] got full packet, %#04x bytes", ch->size);
    SF_LOG_DEBUG("server.authserver", "[AuthChallenge] name(%d): '%s'", ch->I_len, ch->I);

    // BigEndian code, nop in little endian case
    // size already converted
    EndianConvertPtr<uint32>(&ch->gamename[0]);
    EndianConvert(ch->build);
    EndianConvertPtr<uint32>(&ch->platform[0]);
    EndianConvertPtr<uint32>(&ch->os[0]);
    EndianConvertPtr<uint32>(&ch->country[0]);
    EndianConvert(ch->timezone_bias);
    EndianConvert(ch->ip);

    ByteBuffer pkt;

    _login = (const char*)ch->I;
    _accountId = 0;
    _accountName.clear();

    Skyfire::Auth::LoginIdentity const loginIdentity = Skyfire::Auth::NormalizeLoginIdentity(_login);
    SF_LOG_DEBUG("server.authserver", "[AuthChallenge] login identity kind: %s",
        Skyfire::Auth::GetLoginIdentityKindName(loginIdentity.Kind));

    socket().SetPacketLogAccountName(_login);
    _build = ch->build;
    _expversion = uint8(AuthHelper::IsPostBCAcceptedClientBuild(_build) ? POST_BC_EXP_FLAG :
        (AuthHelper::IsPreBCAcceptedClientBuild(_build) ? PRE_BC_EXP_FLAG : NO_VALID_EXP_FLAG));
    _os = (const char*)ch->os;

    if (_os.size() > 4)
        return false;

    // Restore string order as its byte order is reversed
    std::reverse(_os.begin(), _os.end());

    pkt << uint8(AUTH_LOGON_CHALLENGE);
    pkt << uint8(0x00);

    // Verify that this IP is not in the ip_banned table
    LoginDatabase.Execute(LoginDatabase.GetPreparedStatement(LOGIN_DEL_EXPIRED_IP_BANS));

    std::string const& ip_address = socket().getRemoteAddress();
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_IP_BANNED);
    stmt->setString(0, ip_address);
    PreparedQueryResult result = LoginDatabase.Query(stmt);
    if (result)
    {
        pkt << uint8(AuthResult::WOW_FAIL_BANNED);
        SF_LOG_DEBUG("server.authserver", "'%s:%d' [AuthChallenge] Banned ip tries to login!",
            socket().getRemoteAddress().c_str(), socket().getRemotePort());
    }
    else
    {
        // Get the account details from the account table
        // No SQL injection (prepared statement)
        if (loginIdentity.Kind == Skyfire::Auth::LoginIdentityKind::Email)
        {
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_LOGONCHALLENGE_BY_LOGIN_IDENTITY);
            stmt->setString(0, loginIdentity.Canonical);
            stmt->setString(1, loginIdentity.Canonical);
            stmt->setString(2, loginIdentity.Canonical);
        }
        else
        {
            stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_LOGONCHALLENGE);
            stmt->setString(0, _login);
        }

        PreparedQueryResult res2 = LoginDatabase.Query(stmt);
        if (res2)
        {
            Field* fields = res2->Fetch();
            _accountId = fields[0].GetUInt32();
            _accountName = fields[8].GetString();

            if (loginIdentity.Kind == Skyfire::Auth::LoginIdentityKind::Email)
                SF_LOG_DEBUG("server.authserver", "[AuthChallenge] email identity '%s' resolved to account '%s' (%u)",
                    _login.c_str(), _accountName.c_str(), _accountId);

            // If the IP is 'locked', check that the player comes indeed from the correct IP address
            bool locked = false;
            if (fields[1].GetUInt8() == 1)                  // if ip is locked
            {
                SF_LOG_DEBUG("server.authserver", "[AuthChallenge] Account '%s' is locked to IP - '%s'",
                    _login.c_str(), fields[3].GetCString());
                SF_LOG_DEBUG("server.authserver", "[AuthChallenge] Player address is '%s'", ip_address.c_str());

                if (strcmp(fields[3].GetCString(), ip_address.c_str()) != 0)
                {
                    SF_LOG_DEBUG("server.authserver", "[AuthChallenge] Account IP differs");
                    pkt << uint8(AuthResult::WOW_FAIL_LOCKED_ENFORCED);
                    locked = true;
                }
                else
                    SF_LOG_DEBUG("server.authserver", "[AuthChallenge] Account IP matches");
            }
            else
            {
                SF_LOG_DEBUG("server.authserver", "[AuthChallenge] Account '%s' is not locked to ip", _login.c_str());
                std::string accountCountry = fields[2].GetString();
                if (accountCountry.empty() || accountCountry == "00")
                {
                    SF_LOG_DEBUG("server.authserver", "[AuthChallenge] Account '%s' is not locked to country",
                        _login.c_str());
                }

                if (!accountCountry.empty())
                {
                    uint32 ip = Skyfire::Net::ToIPv4NetworkOrder(ip_address);
                    EndianConvertReverse(ip);

                    stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_LOGON_COUNTRY);
                    stmt->setUInt32(0, ip);
                    if (PreparedQueryResult sessionCountryQuery = LoginDatabase.Query(stmt))
                    {
                        std::string loginCountry = (*sessionCountryQuery)[0].GetString();
                        SF_LOG_DEBUG("server.authserver",
                            "[AuthChallenge] Account '%s' is locked to country: '%s' Player country is '%s'",
                            _login.c_str(), accountCountry.c_str(), loginCountry.c_str());
                        if (loginCountry != accountCountry)
                        {
                            SF_LOG_DEBUG("server.authserver", "[AuthChallenge] Account country differs.");
                            pkt << uint8(AuthResult::WOW_FAIL_UNLOCKABLE_LOCK);
                            locked = true;
                        }
                        else
                            SF_LOG_DEBUG("server.authserver", "[AuthChallenge] Account country matches");
                    }
                    else
                        SF_LOG_DEBUG("server.authserver", "[AuthChallenge] IP2NATION Table empty");
                }
            }

            if (!locked)
            {
                //set expired bans to inactive
                LoginDatabase.DirectExecute(LoginDatabase.GetPreparedStatement(LOGIN_UPD_EXPIRED_ACCOUNT_BANS));

                // If the account is banned, reject the logon attempt
                stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_BANNED);
                stmt->setUInt32(0, _accountId);
                PreparedQueryResult banresult = LoginDatabase.Query(stmt);
                if (banresult)
                {
                    if ((*banresult)[0].GetUInt32() == (*banresult)[1].GetUInt32())
                    {
                        pkt << uint8(AuthResult::WOW_FAIL_BANNED);
                        SF_LOG_DEBUG("server.authserver", "'%s:%d' [AuthChallenge] Banned account %s tried to login!",
                            socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str());
                    }
                    else
                    {
                        pkt << uint8(AuthResult::WOW_FAIL_SUSPENDED);
                        SF_LOG_DEBUG("server.authserver",
                            "'%s:%d' [AuthChallenge] Temporarily banned account %s tried to login!",
                            socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str());
                    }
                }
                else
                {
                    // GRUNT SRP proofs are bound to the client-sent identity string.
                    // Do not replace email logins with the legacy username here.
                    _srp6.emplace(_login, fields[5].GetBinary<SkyFire::Crypto::SRP6::SALT_LENGTH>(),
                        fields[6].GetBinary<SkyFire::Crypto::SRP6::VERIFIER_LENGTH>());

                    BigNumber unk3;
                    unk3.SetRand(16 * 8);

                    // Fill the response packet with the result
                    if (AuthHelper::IsAcceptedClientBuild(_build))
                        pkt << uint8(AuthResult::WOW_SUCCESS);
                    else
                        pkt << uint8(AuthResult::WOW_FAIL_VERSION_INVALID);

                    // B may be calculated < 32B so we force minimal length to 32B
                    pkt.append(_srp6->B);
                    pkt << uint8(1);
                    pkt.append(_srp6->g);
                    pkt << uint8(32);
                    pkt.append(_srp6->N);
                    pkt.append(_srp6->s);
                    pkt.append(unk3.ToByteArray<16>());
                    uint8 securityFlags = 0;

                    // Check if token is used
                    _tokenKey = fields[7].GetString();
                    if (!_tokenKey.empty())
                        securityFlags = 4;

                    pkt << uint8(securityFlags);            // security flags (0x0...0x04)

                    if (securityFlags & 0x01)               // PIN input
                    {
                        pkt << uint32(0);
                        pkt << uint64(0) << uint64(0);      // 16 bytes hash?
                    }

                    if (securityFlags & 0x02)               // Matrix input
                    {
                        pkt << uint8(0);
                        pkt << uint8(0);
                        pkt << uint8(0);
                        pkt << uint8(0);
                        pkt << uint64(0);
                    }

                    if (securityFlags & 0x04)               // Security token input
                        pkt << uint8(1);

                    AccountTypes secLevel = AccountTypes(fields[4].GetUInt8());
                    _accountSecurityLevel = secLevel <= AccountTypes::SEC_ADMINISTRATOR ?
                        AccountTypes(secLevel) : AccountTypes::SEC_ADMINISTRATOR;

                    _localizationName.resize(4);
                    for (int i = 0; i < 4; ++i)
                        _localizationName[i] = ch->country[4 - i - 1];

                    SF_LOG_DEBUG("server.authserver",
                        "'%s:%d' [AuthChallenge] account %s is using '%c%c%c%c' locale (%u)",
                        socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str(),
                        ch->country[3], ch->country[2], ch->country[1], ch->country[0],
                        GetLocaleByName(_localizationName));
                }
            }
        }
        else                                                //no account
            pkt << uint8(AuthResult::WOW_FAIL_UNKNOWN_ACCOUNT);
    }

    socket().QueueSend(pkt.contents(), pkt.size());
    return true;
}

// Logon Proof command handler
bool AuthSocket::_HandleLogonProof()
{
    SF_LOG_DEBUG("server.authserver", "Entering _HandleLogonProof");
    sAuthLogonProof_C lp;

    if (!socket().PeekBytes(&lp, sizeof(sAuthLogonProof_C)))
        return false;

    bool needsToken = (lp.securityFlags & 0x04) || !_tokenKey.empty();
    uint8 tokenSize = 0;
    size_t requiredBytes = sizeof(sAuthLogonProof_C);

    if (needsToken)
    {
        if (!socket().PeekBytes(&tokenSize, sizeof(tokenSize), requiredBytes))
            return false;

        requiredBytes += sizeof(tokenSize) + tokenSize;
        if (socket().GetAvailableBytes() < requiredBytes)
            return false;
    }

    if (!socket().ReadBytes(&lp, sizeof(sAuthLogonProof_C)))
        return false;

    // If the client has no valid version
    if (_expversion == NO_VALID_EXP_FLAG)
    {
        // Check if we have the appropriate patch on the disk
        SF_LOG_DEBUG("network", "Client with invalid version, patching is not implemented");
        socket().Close();
        return true;
    }

    if (std::optional<SessionKey> K = _srp6->VerifyChallengeResponse(lp.A, lp.clientM))
    {
         _sessionKey = *K;

        SF_LOG_DEBUG("server.authserver", "'%s:%d' User '%s' successfully authenticated",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str());

        // Update the sessionkey, last_ip, last login time and reset number of failed logins
        // in the account table for this account.
        // No SQL injection (escaped user name) and IP address as received by socket

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_LOGONPROOF_BY_ID);
        stmt->setBinary(0, _sessionKey);
        stmt->setString(1, socket().getRemoteAddress().c_str());
        stmt->setUInt32(2, GetLocaleByName(_localizationName));
        stmt->setString(3, _os);
        stmt->setUInt32(4, _accountId);
        LoginDatabase.DirectExecute(stmt);

        // Finish SRP6 and send the final result to the client
        SkyFire::Crypto::SHA1::Digest M2 = SkyFire::Crypto::SRP6::GetSessionVerifier(lp.A, lp.clientM, _sessionKey);

        // Check auth token
        if (needsToken)
        {
            uint8 size = 0;
            if (!socket().ReadBytes(&size, sizeof(size)))
                return false;

            std::vector<char> token(size + 1);
            token[size] = '\0';
            if (!socket().ReadBytes(&token[0], size))
                return false;

            unsigned int validToken = TOTP::GenerateToken(_tokenKey);
            unsigned int incomingToken = atoi(&token[0]);
            if (validToken != incomingToken)
            {
                char data[] = { AUTH_LOGON_PROOF, uint8(AuthResult::WOW_FAIL_UNKNOWN_ACCOUNT), 3, 0 };
                socket().QueueSend(data, sizeof(data));
                return false;
            }
        }

        if (_expversion & POST_BC_EXP_FLAG)                 // 2.x and 3.x clients
        {
            sAuthLogonProof_S proof;
            proof.M2 = M2;
            proof.cmd = AUTH_LOGON_PROOF;
            proof.error = 0;
            proof.unk1 = 0x00800000;    // Accountflags. 0x01 = GM, 0x08 = Trial, 0x00800000 = Pro pass.
            proof.unk2 = 0x00;          // SurveyId
            proof.unk3 = 0x00;
            socket().QueueSend(&proof, sizeof(proof));
        }
        else
        {
            sAuthLogonProof_S_Old proof;
            proof.M2 = M2;
            proof.cmd = AUTH_LOGON_PROOF;
            proof.error = 0;
            proof.unk2 = 0x00;
            socket().QueueSend(&proof, sizeof(proof));
        }

        _authed = true;
    }
    else
    {
        char data[4] = { AUTH_LOGON_PROOF, uint8(AuthResult::WOW_FAIL_UNKNOWN_ACCOUNT), 3, 0 };
        socket().QueueSend(data, sizeof(data));

        SF_LOG_DEBUG("server.authserver",
            "'%s:%d' [AuthChallenge] account %s tried to login with invalid password!",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str());

        uint32 MaxWrongPassCount = sConfigMgr->GetIntDefault("WrongPass.MaxCount", 0);
        if (MaxWrongPassCount > 0)
        {
            //Increment number of failed logins by one and if it reaches the limit temporarily ban that account or IP
            PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(_accountId ? LOGIN_UPD_FAILEDLOGINS_BY_ID : LOGIN_UPD_FAILEDLOGINS);
            if (_accountId)
                stmt->setUInt32(0, _accountId);
            else
                stmt->setString(0, _login);
            LoginDatabase.Execute(stmt);

            stmt = LoginDatabase.GetPreparedStatement(_accountId ? LOGIN_SEL_FAILEDLOGINS_BY_ID : LOGIN_SEL_FAILEDLOGINS);
            if (_accountId)
                stmt->setUInt32(0, _accountId);
            else
                stmt->setString(0, _login);

            if (PreparedQueryResult loginfail = LoginDatabase.Query(stmt))
            {
                uint32 failed_logins = (*loginfail)[1].GetUInt32();

                if (failed_logins >= MaxWrongPassCount)
                {
                    uint32 WrongPassBanTime = sConfigMgr->GetIntDefault("WrongPass.BanTime", 600);
                    bool WrongPassBanType = sConfigMgr->GetBoolDefault("WrongPass.BanType", false);

                    if (WrongPassBanType)
                    {
                        uint32 acc_id = (*loginfail)[0].GetUInt32();
                        stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_ACCOUNT_AUTO_BANNED);
                        stmt->setUInt32(0, acc_id);
                        stmt->setUInt32(1, WrongPassBanTime);
                        LoginDatabase.Execute(stmt);

                        SF_LOG_DEBUG("server.authserver",
                            "'%s:%d' [AuthChallenge] account %s got banned for '%u' seconds "
                            "because it failed to authenticate '%u' times",
                            socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str(),
                            WrongPassBanTime, failed_logins);
                    }
                    else
                    {
                        stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_IP_AUTO_BANNED);
                        stmt->setString(0, socket().getRemoteAddress());
                        stmt->setUInt32(1, WrongPassBanTime);
                        LoginDatabase.Execute(stmt);

                        SF_LOG_DEBUG("server.authserver",
                            "'%s:%d' [AuthChallenge] IP %s got banned for '%u' seconds "
                            "because account %s failed to authenticate '%u' times",
                            socket().getRemoteAddress().c_str(), socket().getRemotePort(),
                            socket().getRemoteAddress().c_str(), WrongPassBanTime, _login.c_str(), failed_logins);
                    }
                }
            }
        }
    }

    return true;
}

// Reconnect Challenge command handler
bool AuthSocket::_HandleReconnectChallenge()
{
    SF_LOG_DEBUG("server.authserver", "Entering _HandleReconnectChallenge");
    std::vector<uint8> buf;
    uint16 remaining = 0;
    if (!ReadAuthChallengeFrame(socket(), buf, remaining))
        return false;

    SF_LOG_DEBUG("server.authserver", "[ReconnectChallenge] got header, body is %#04x bytes", remaining);
    sAuthLogonChallenge_C* ch = (sAuthLogonChallenge_C*)&buf[0];

    SF_LOG_DEBUG("server.authserver", "[ReconnectChallenge] got full packet, %#04x bytes", ch->size);
    SF_LOG_DEBUG("server.authserver", "[ReconnectChallenge] name(%d): '%s'", ch->I_len, ch->I);

    _login = (const char*)ch->I;
    _accountId = 0;
    _accountName.clear();
    socket().SetPacketLogAccountName(_login);

    Skyfire::Auth::LoginIdentity const loginIdentity = Skyfire::Auth::NormalizeLoginIdentity(_login);
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
        stmt->setString(0, _login);
    }

    PreparedQueryResult result = LoginDatabase.Query(stmt);

    // Stop if the account is not found
    if (!result)
    {
        SF_LOG_ERROR("server.authserver",
            "'%s:%d' [ERROR] user %s tried to login and we cannot find his session key in the database.",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str());
        socket().Close();
        return false;
    }

    // Reinitialize build, expansion and the account securitylevel
    _build = ch->build;
    _expversion = uint8(AuthHelper::IsPostBCAcceptedClientBuild(_build) ? POST_BC_EXP_FLAG :
        (AuthHelper::IsPreBCAcceptedClientBuild(_build) ? PRE_BC_EXP_FLAG : NO_VALID_EXP_FLAG));
    _os = (const char*)ch->os;

    if (_os.size() > 4)
        return false;

    // Restore string order as its byte order is reversed
    std::reverse(_os.begin(), _os.end());

    Field* fields = result->Fetch();
    _accountId = fields[1].GetUInt32();
    if (loginIdentity.Kind == Skyfire::Auth::LoginIdentityKind::Email)
        _accountName = fields[3].GetString();
    else
        _accountName = _login;

    AccountTypes secLevel = AccountTypes(fields[2].GetUInt8());
    _accountSecurityLevel = secLevel <= AccountTypes::SEC_ADMINISTRATOR ?
        AccountTypes(secLevel) : AccountTypes::SEC_ADMINISTRATOR;

    _sessionKey = fields[0].GetBinary<SESSION_KEY_LENGTH>();
    SkyFire::Crypto::GetRandomBytes(_reconnectProof);

    // Sending response
    ByteBuffer pkt;
    pkt << uint8(AUTH_RECONNECT_CHALLENGE);
    pkt << uint8(0x00);
    pkt.append(_reconnectProof);                            // 16 bytes random
    pkt << uint64(0x00) << uint64(0x00);                    // 16 bytes zeros
    socket().QueueSend(pkt.contents(), pkt.size());
    return true;
}

// Reconnect Proof command handler
bool AuthSocket::_HandleReconnectProof()
{
    SF_LOG_DEBUG("server.authserver", "Entering _HandleReconnectProof");
    // Read the packet
    sAuthReconnectProof_C lp;
    if (!socket().ReadBytes(&lp, sizeof(sAuthReconnectProof_C)))
        return false;

    if (_login.empty())
        return false;

    BigNumber t1;
    t1.SetBinary(lp.R1, 16);

    SkyFire::Crypto::SHA1 sha;
    sha.UpdateData(_login);
    sha.UpdateData(t1.ToByteArray<16>());
    sha.UpdateData(_reconnectProof);
    sha.UpdateData(_sessionKey);
    sha.Finalize();

    if (sha.GetDigest() == lp.R2)
    {
        // Sending response
        ByteBuffer pkt;
        pkt << uint8(AUTH_RECONNECT_PROOF);
        pkt << uint8(0x00);
        pkt << uint16(0x00);                               // 2 bytes zeros
        socket().QueueSend(pkt.contents(), pkt.size());
        _authed = true;
        return true;
    }
    else
    {
        SF_LOG_ERROR("server.authserver", "'%s:%d' [ERROR] user %s tried to login, but session is invalid.",
            socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str());
        socket().Close();
        return false;
    }
}

Skyfire::Net::Address const& AuthSocket::GetAddressForClient(Realm const& realm, Skyfire::Net::Address const& clientAddr)
{
    // Attempt to send best address for client
    if (clientAddr.IsLoopback())
    {
        // Try guessing if realm is also connected locally
        if (realm.LocalAddress.IsLoopback() || realm.ExternalAddress.IsLoopback())
            return clientAddr;

        // Assume that user connecting from the machine that authserver is located on
        // has all realms available in his local network
        return realm.LocalAddress;
    }

    // Check if connecting client is in the same network
    if (IsIPAddrInNetwork(realm.LocalAddress, clientAddr, realm.LocalSubnetMask))
        return realm.LocalAddress;

    // Return external IP
    return realm.ExternalAddress;
}

// Realm List command handler
bool AuthSocket::_HandleRealmList()
{
    SF_LOG_DEBUG("server.authserver", "Entering _HandleRealmList");
    if (socket().GetAvailableBytes() < 5)
        return false;

    socket().DiscardBytes(5);

    // Get the user id (else close the connection)
    // No SQL injection (prepared statement)
    uint32 id = _accountId;
    PreparedStatement* stmt = nullptr;
    PreparedQueryResult result;
    if (!id)
    {
        stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_ID_BY_NAME);
        stmt->setString(0, _login);
        result = LoginDatabase.Query(stmt);
        if (!result)
        {
            SF_LOG_ERROR("server.authserver",
                "'%s:%d' [ERROR] user %s tried to login but we cannot find him in the database.",
                socket().getRemoteAddress().c_str(), socket().getRemotePort(), _login.c_str());
            socket().Close();
            return false;
        }

        Field* fields = result->Fetch();
        id = fields[0].GetUInt32();
    }

    // Update realm list if need
    sRealmList->UpdateIfNeed();

    Skyfire::Net::Address clientAddr(socket().getRemoteAddress(), socket().getRemotePort());

    // Circle through realms in the RealmList and construct the return packet,
    // including the number of user characters in each realm.
    ByteBuffer pkt;

    size_t RealmListSize = 0;
    for (RealmList::RealmMap::const_iterator i = sRealmList->begin(); i != sRealmList->end(); ++i)
    {
        const Realm& realm = i->second;
        // don't work with realms which not compatible with the client
        bool okBuild = ((_expversion & POST_BC_EXP_FLAG) && realm.gamebuild == _build) ||
            ((_expversion & PRE_BC_EXP_FLAG) && !AuthHelper::IsPreBCAcceptedClientBuild(realm.gamebuild));

        // No SQL injection. id of realm is controlled by the database.
        uint32 flag = realm.flag;
        RealmBuildInfo const* buildInfo = AuthHelper::GetBuildInfo(realm.gamebuild);
        if (!okBuild)
        {
            if (!buildInfo)
                continue;

            flag |= REALM_FLAG_OFFLINE | REALM_FLAG_SPECIFYBUILD;   // tell the client what build the realm is for
        }

        if (!buildInfo)
            flag &= ~REALM_FLAG_SPECIFYBUILD;

        std::string name = i->first;
        if (_expversion & PRE_BC_EXP_FLAG && flag & REALM_FLAG_SPECIFYBUILD)
        {
            std::ostringstream ss;
            ss << name << " (" << buildInfo->MajorVersion << '.' << buildInfo->MinorVersion << '.'
                << buildInfo->BugfixVersion << ')';
            name = ss.str();
        }

        // We don't need the port number from which client connects with but the realm's port
        clientAddr.SetPort(realm.ExternalAddress.GetPort());

        uint8 lock = (realm.allowedSecurityLevel > _accountSecurityLevel) ? 1 : 0;

        uint8 AmountOfCharacters = 0;
        stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_NUM_CHARS_ON_REALM);
        stmt->setUInt32(0, realm.m_ID);
        stmt->setUInt32(1, id);
        result = LoginDatabase.Query(stmt);
        if (result)
            AmountOfCharacters = (*result)[0].GetUInt8();

        pkt << realm.icon;                                  // realm type
        if (_expversion & POST_BC_EXP_FLAG)                 // only 2.x and 3.x clients
            pkt << lock;                                    // if 1, then realm locked
        pkt << uint8(flag);                                 // RealmFlags
        pkt << name;
        pkt << GetAddressString(GetAddressForClient(realm, clientAddr));
        pkt << realm.populationLevel;
        pkt << AmountOfCharacters;
        pkt << realm.timezone;                              // realm category
        if (_expversion & POST_BC_EXP_FLAG)                 // 2.x and 3.x clients
            pkt << uint8(realm.m_ID);                       // VirtualRealmID
        else
            pkt << uint8(0x0);                              // 1.12.1 and 1.12.2 clients

        if (_expversion & POST_BC_EXP_FLAG && flag & REALM_FLAG_SPECIFYBUILD)
        {
            pkt << uint8(buildInfo->MajorVersion);
            pkt << uint8(buildInfo->MinorVersion);
            pkt << uint8(buildInfo->BugfixVersion);
            pkt << uint16(buildInfo->Build);
        }

        ++RealmListSize;
    }

    if (_expversion & POST_BC_EXP_FLAG)                     // 2.x and 3.x clients
    {
        pkt << uint8(0x10);
        pkt << uint8(0x00);
    }
    else                                                    // 1.12.1 and 1.12.2 clients
    {
        pkt << uint8(0x00);
        pkt << uint8(0x02);
    }

    // make a ByteBuffer which stores the RealmList's size
    ByteBuffer RealmListSizeBuffer;
    RealmListSizeBuffer << uint32(0);
    if (_expversion & POST_BC_EXP_FLAG)                     // only 2.x and 3.x clients
        RealmListSizeBuffer << uint16(RealmListSize);
    else
        RealmListSizeBuffer << uint32(RealmListSize);

    ByteBuffer hdr;
    hdr << uint8(REALM_LIST);
    hdr << uint16(pkt.size() + RealmListSizeBuffer.size());
    hdr.append(RealmListSizeBuffer);                        // append RealmList's size buffer
    hdr.append(pkt);                                        // append realms in the realmlist

    socket().QueueSend(hdr.contents(), hdr.size());

    return true;
}

// Resume patch transfer
bool AuthSocket::_HandleXferResume()
{
    SF_LOG_DEBUG("server.authserver", "Entering _HandleXferResume");

    switch (Skyfire::Auth::EvaluatePatchTransferRequest(Skyfire::Auth::PatchTransferAction::Resume, socket().GetAvailableBytes()))
    {
        case Skyfire::Auth::PatchTransferDecision::NeedResumeOffset:
            SF_LOG_ERROR("server.authserver", "Error while resuming patch transfer (wrong packet)");
            return false;
        case Skyfire::Auth::PatchTransferDecision::Unsupported:
            SF_LOG_ERROR("server.authserver", "Client requested unsupported patch transfer resume");
            socket().Close();
            return false;
        case Skyfire::Auth::PatchTransferDecision::Cancel:
            break;
    }

    return false;
}

// Cancel patch transfer
bool AuthSocket::_HandleXferCancel()
{
    SF_LOG_DEBUG("server.authserver", "Entering _HandleXferCancel");

    if (Skyfire::Auth::EvaluatePatchTransferRequest(Skyfire::Auth::PatchTransferAction::Cancel, socket().GetAvailableBytes()) !=
        Skyfire::Auth::PatchTransferDecision::Cancel)
        return false;

    // Close and delete the socket
    socket().DiscardBytes(1);                                      // clear input buffer
    socket().Close();

    return true;
}

// Accept patch transfer
bool AuthSocket::_HandleXferAccept()
{
    SF_LOG_DEBUG("server.authserver", "Entering _HandleXferAccept");

    if (Skyfire::Auth::EvaluatePatchTransferRequest(Skyfire::Auth::PatchTransferAction::Accept, socket().GetAvailableBytes()) ==
        Skyfire::Auth::PatchTransferDecision::Unsupported)
    {
        SF_LOG_ERROR("server.authserver", "Client requested unsupported patch transfer accept");
        socket().Close();
        return false;
    }

    return false;
}
