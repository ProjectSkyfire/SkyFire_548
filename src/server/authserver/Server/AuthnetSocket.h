/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_AUTHNETSOCKET_H
#define SF_AUTHNETSOCKET_H

#include "Authentication/AuthDefines.h"
#include "Authentication/PasswordSrp.h"
#include "Common.h"
#include "RealmSocket.h"
#include <array>
#include <boost/asio/steady_timer.hpp>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

class AuthnetSocket : public RealmSocket::Session
{
public:
    explicit AuthnetSocket(RealmSocket& socket);
    virtual ~AuthnetSocket();

    virtual void OnRead(void) override;
    virtual void OnAccept(void) override;
    virtual void OnClose(void) override;

private:
    RealmSocket& socket_;
    RealmSocket& socket(void) { return socket_; }

    void TrySendProbeResponse(size_t readOffset, size_t readSize);
    bool DecodeInitialRequest(void);
    void PrepareWorldSessionKey(std::string const& identity, std::string const& platform, std::string const& locale);
    void PersistAuthnetWorldSessionKey(uint32 connectionSeed, uint32 realmField, uint32 selectedRealmField, char const* reason);
    void PersistAuthnetWorldSessionKey(uint32 connectionSeed, uint32 realmField, char const* reason);
    void PersistAuthnetWorldSessionKey(char const* reason);
    void PersistAuthnetWorldSessionKey(char const* reason, uint32 connectionSeed);
    bool TryUpdateWorldSessionKeyFromSelectedRealm(std::vector<uint8> const& packet);
    uint32 GetAuthnetPreferredRealmField(void) const;
    void ProcessEncryptedClientBytes(size_t encryptedFollowupOffset);
    void ProcessPostProofPlaintext(void);
    void SendEncryptedRequestResult(uint32 requestId, std::vector<uint8> const* resourceKey = nullptr, uint32 resourceItemId = 0);
    void PushUnsolicitedRealmList(char const* reason);
    void SendMode2RealmUpdates(char const* reason);
    void SendMode2RealmListComplete(char const* reason);
    void SendMode2RealmListStartComplete(char const* reason);
    void ScheduleGetRealmsFollowupAfterConnectComplete();
    void CryptClientPayload(std::vector<uint8>& payload);
    void CryptServerPayload(std::vector<uint8>& payload);
    void InitSoftFinishCrypt(SkyFire::Authnet::PasswordSrp::SessionKey const& sessionKey);
    bool TrySendCachedGameAccount(char const* reason);
    void ScheduleRealmListFrames(std::vector<std::pair<std::string, std::vector<uint8>>> frames);
    void SendNextRealmListFrame();
    bool TryAdoptPendingSoftFinishSession();
    void PublishPendingSoftFinishSession();

    std::vector<uint8> _captured;
    size_t _encryptedBytesProcessed;
    size_t _initialRequestLen;
    size_t _soft2UplinkLen;
    size_t _postProofEncryptedOffset;
    uint32 _authnetAccountId;
    uint32 _authnetLocaleId;
    uint32 _authnetWorldConnectionSeed;
    uint32 _authnetWorldRealmField;
    uint32 _authnetSelectedRealmField;
    uint32 _authnetLoginCompleteRealmField;
    std::string _authnetAccountName;
    std::string _authnetOS;
    std::string _authnetWorldAccountToken;
    SessionKey _authnetWorldSessionKey;
    std::array<uint8, 64> _authnetSecret;
    bool _authnetWorldSessionKeyGenerated;
    bool _authnetWorldSessionKeyPersisted;
    bool _authnetSecretReady;
    bool _softFinishContinuation;
    uint8 _clientCryptState[256];
    uint8 _clientCryptI;
    uint8 _clientCryptJ;
    bool _clientCryptInitialized;
    uint8 _serverCryptState[256];
    uint8 _serverCryptI;
    uint8 _serverCryptJ;
    bool _serverCryptInitialized;
    std::vector<uint32> _answeredRequestIds;
    bool _responded;
    bool _httpResponded;
    bool _clientModeSwitchSeen;
    bool _followupLogged;
    bool _postSuccessBurstSeen;
    bool _moduleProofAnswered;
    bool _soft2EmptySeen;
    bool _mode1ConnectAnswered;
    bool _mode2LoginAnswered;
    bool _mode2Command2Answered;
    bool _mode2Command3Answered;
    bool _mode2RealmUpdatesSent;
    bool _connect4601CompleteSent;
    bool _mode2Command8Answered;
    bool _mode2Command8JoinAnswered;
    bool _mode2Command8JoinSuccess;
    bool _postLoginStatusSent;
    bool _mode1Command6Answered;
    bool _mode1Command7Answered;
    bool _softBounceSeen;
    std::string _loginIdentity;
    std::string _clientOs;
    std::string _clientLocale;
    uint32 _accountId;
    std::vector<std::pair<std::string, std::vector<uint8>>> _realmListFrames;
    size_t _realmListFrameIndex;
    std::unique_ptr<boost::asio::steady_timer> _realmListFrameTimer;
    std::optional<SkyFire::Authnet::PasswordSrp::Challenge> _passwordChallenge;
    std::optional<SkyFire::Authnet::PasswordSrp::SessionKey> _sessionKey;
};

#endif
