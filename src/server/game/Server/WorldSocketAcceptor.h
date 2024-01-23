/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/** \addtogroup u2w User to World Communication
 *  @{
 *  \file WorldSocketMgr.h
 */

#ifndef SF_WORLDSOCKETACCEPTOR_H
#define SF_WORLDSOCKETACCEPTOR_H

#include "Common.h"

#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>

#include "WorldSocket.h"

class WorldSocketAcceptor : public ACE_Acceptor<WorldSocket, ACE_SOCK_Acceptor>
{
public:
    WorldSocketAcceptor(void) { }
    virtual ~WorldSocketAcceptor(void)
    {
        if (reactor())
            reactor()->cancel_timer(this, 1);
    }

protected:
    virtual int handle_timeout(const ACE_Time_Value& /*current_time*/, const void* /*act = 0*/)
    {
        SF_LOG_DEBUG("misc", "Resuming acceptor");
        reactor()->cancel_timer(this, 1);
        return reactor()->register_handler(this, ACE_Event_Handler::ACCEPT_MASK);
    }

    virtual int handle_accept_error(void)
    {
#if defined(ENFILE) && defined(EMFILE)
        if (errno == ENFILE || errno == EMFILE)
        {
            SF_LOG_ERROR("misc", "Out of file descriptors, suspending incoming connections for 10 seconds");
            reactor()->remove_handler(this, ACE_Event_Handler::ACCEPT_MASK | ACE_Event_Handler::DONT_CALL);
            reactor()->schedule_timer(this, NULL, ACE_Time_Value(10));
        }
#endif
        return 0;
    }
};

#endif /* __WORLDSOCKETACCEPTOR_H_ */
/// @}
