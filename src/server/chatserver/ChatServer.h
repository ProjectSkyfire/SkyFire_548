/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_CHAT_SERVER_H
#define SKYFIRE_CHAT_SERVER_H
#include "Cluster/ClusterAgent.h"
#include <memory>
#include <set>

namespace Skyfire::Chat
{
    struct Options
    {
        std::string Address;
        std::uint16_t Port = 54940;
        unsigned MaxConnections = 32;
        unsigned TimeoutSeconds = 5;
        std::set<std::string> WorldKeys;
        std::set<std::uint32_t> Realms;
    };
    class Server
    {
    public:
        Server();
        ~Server();
        bool Open(Options options, Cluster::AgentOptions const& tls, std::string& error);
        void Update();
        void Stop();
        Cluster::ChatMetrics Metrics() const;
    private:
        struct State;
        std::unique_ptr<State> _state;
    };
}
#endif
