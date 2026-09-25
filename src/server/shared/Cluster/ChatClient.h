/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_CHAT_CLIENT_H
#define SKYFIRE_CHAT_CLIENT_H
#include "ChatPresence.h"
#include "ChatWhisper.h"
#include "ChatRouting.h"
#include "ClusterAgent.h"
#include <chrono>
namespace Skyfire::Chat
{
    // Configure before world tick starts; Stop after its thread has joined.
    bool StartClient(Cluster::AgentOptions options, std::string& error);
    void StopClient();
    bool WhispersEnabled();
    bool QueueWhisper(Whisper message);
    struct WhisperResult { Whisper Message; bool Success = false; std::chrono::steady_clock::time_point Deadline; };
    std::vector<WhisperResult> TakeWhisperResults();
    struct RouteResult
    {
        std::uint32_t Account = 0;
        std::uint8_t Lane = 0;
        std::uint64_t Sequence = 0;
        bool Success = false;
        std::vector<RouteMember> Recipients;
        std::chrono::steady_clock::time_point Deadline;
    };
    bool RoutingEnabled();
    std::uint64_t QueueRoute(AudienceProjection projection, RoutedMessage message);
    std::vector<RouteResult> TakeRouteResults();
    bool ClientEnabled();
    void PublishPresence(std::vector<PlayerPresence> players);
}
#endif
