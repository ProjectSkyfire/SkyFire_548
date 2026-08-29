/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_AUTHNETSOCKET_H
#define SF_AUTHNETSOCKET_H

#include "Common.h"
#include "RealmSocket.h"

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
    void ProcessEncryptedClientBytes(size_t encryptedFollowupOffset);
    void SendEncryptedRequestResult(uint32 requestId, std::vector<uint8> const* resourceKey = nullptr, uint32 resourceItemId = 0);
    void CryptClientPayload(std::vector<uint8>& payload);
    void CryptServerPayload(std::vector<uint8>& payload);

    std::vector<uint8> _captured;
    size_t _encryptedBytesProcessed;
    size_t _initialRequestLen;
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
    bool _mode1ConnectAnswered;
    bool _mode2LoginAnswered;
    bool _mode2Command2Answered;
    bool _mode2Command8Answered;
    bool _postLoginStatusSent;
    bool _mode1Command6Answered;
};

#endif
