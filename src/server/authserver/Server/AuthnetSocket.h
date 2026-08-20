/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_AUTHNETSOCKET_H
#define SF_AUTHNETSOCKET_H

#include "Common.h"
#include "RealmSocket.h"

// Passive probe for authnet (the launcher-facing Battle.net-style login
// path). Logs the first frame's header bytes from a real connection and
// closes - this build's exact Header field layout (service_id/method_id/
// token/size) isn't confirmed precisely enough yet to safely determine
// where the header ends and a body begins, so this deliberately does not
// try to read past the header or synthesize a response. See the authnet
// roadmap and its client login probe research note.
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

    bool _loggedHeader;
};

#endif
