/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_AUTHSOCKET_H
#define SF_AUTHSOCKET_H

#include "CryptoHash.h"
#include "Common.h"
#include "NetworkAddress.h"
#include "RealmSocket.h"
#include "SRP6.h"

struct Realm;

// Handle login commands
class AuthSocket : public RealmSocket::Session
{
public:
    const static int s_BYTE_SIZE = 32;

    AuthSocket(RealmSocket& socket);
    virtual ~AuthSocket(void);

    virtual void OnRead(void);
    virtual void OnAccept(void);
    virtual void OnClose(void);

    static Skyfire::Net::Address const& GetAddressForClient(Realm const& realm, Skyfire::Net::Address const& clientAddr);

    bool _HandleLogonChallenge();
    bool _HandleLogonProof();
    bool _HandleReconnectChallenge();
    bool _HandleReconnectProof();
    bool _HandleRealmList();

    //data transfer handle for patch
    bool _HandleXferResume();
    bool _HandleXferCancel();
    bool _HandleXferAccept();

private:
    RealmSocket& socket_;
    RealmSocket& socket(void) { return socket_; }

    std::optional<SkyFire::Crypto::SRP6> _srp6;
    SessionKey _sessionKey = {};
    std::array<uint8, 16> _reconnectProof = {};

    bool _authed;

    std::string _login;
    std::string _accountName;
    std::string _tokenKey;
    uint32 _accountId = 0;

    // Since GetLocaleByName() is _NOT_ bijective, we have to store the locale as a string. Otherwise we can't differ
    // between enUS and enGB, which is important for the patch system
    std::string _localizationName;
    std::string _os;
    uint16 _build;
    uint8 _expversion;
    AccountTypes _accountSecurityLevel;
};

#endif
