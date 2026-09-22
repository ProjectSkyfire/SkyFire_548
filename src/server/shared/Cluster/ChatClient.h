/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_CHAT_CLIENT_H
#define SKYFIRE_CHAT_CLIENT_H
#include "ChatPresence.h"
#include "ClusterAgent.h"
namespace Skyfire::Chat
{
    // Configure before world tick starts; Stop after its thread has joined.
    bool StartClient(Cluster::AgentOptions options, std::string& error);
    void StopClient();
    bool ClientEnabled();
    void PublishPresence(std::vector<PlayerPresence> players);
}
#endif
