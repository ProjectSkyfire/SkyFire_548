/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_CLUSTER_AGENT_H
#define SKYFIRE_CLUSTER_AGENT_H
#include "ClusterProtocol.h"
#include <functional>
#include <memory>
#include <thread>
namespace Skyfire::Cluster
{
    struct AgentSample
    {
        bool Ready = false;
        std::uint32_t Load = 0;
        ChatMetrics Chat{};
    };
    struct AgentOptions
    {
        bool Enabled = false;
        bool HandoffEnabled = false;
        bool RealmDirectoryEnabled = false;
        std::string Host, Certificate, PrivateKey, CA, CRL;
        std::uint16_t Port = 9100;
        Node Advertisement;
    };
    // Configuration is read once on the main thread. Relative TLS paths use the config directory.
    bool LoadAgentOptions(Node advertisement, AgentOptions& options, std::string& error);
    class Agent
    {
    public:
        Agent() = default;
        ~Agent();
        Agent(Agent const&) = delete;
        Agent& operator=(Agent const&) = delete;
        bool Start(AgentOptions options, std::function<AgentSample()> sample, std::string& error);
        void Stop();
        bool IsRegistered() const;
    private:
        struct State;
        std::shared_ptr<State> _state;
        std::thread _thread;
    };
}
#endif
