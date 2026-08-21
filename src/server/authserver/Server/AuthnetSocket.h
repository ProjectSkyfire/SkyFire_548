/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_AUTHNETSOCKET_H
#define SF_AUTHNETSOCKET_H

#include "Common.h"
#include "RealmSocket.h"

// Passive probe for authnet (the launcher-facing Battle.net-style login
// path). Accumulates and logs everything a real connection sends, across
// as many read events as it takes, rather than assuming a specific frame
// boundary - an earlier version tried to interpret a length-prefixed
// header and close immediately after, which risked (and in practice did)
// truncate a message that arrived across more than one read. Still never
// sends a response, so the client can't progress to the later,
// credential-bearing RPC call. See the authnet roadmap and its client
// login probe research note.
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

    std::vector<uint8> _captured;
};

#endif
