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

    void TrySendProbeResponse(void);

    std::vector<uint8> _captured;
    bool _responded;
};

#endif
