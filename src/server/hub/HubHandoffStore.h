/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_HUB_HANDOFF_STORE_H
#define SKYFIRE_HUB_HANDOFF_STORE_H

#include "Cluster/HandoffStore.h"
#include <memory>

// Serialized on the hub loop. Database UTC is authoritative; caller monotonic
// timestamps are deliberately unused by this persistent implementation.
class HubHandoffStore final : public Skyfire::Cluster::Handoff::Store
{
public:
    HubHandoffStore();
    ~HubHandoffStore() override;
    bool Open(std::string const& connectionString);
    void Close();
    Skyfire::Cluster::Handoff::Result Issue(Skyfire::Cluster::Handoff::Binding const&, std::uint32_t ttl,
        std::uint64_t now, std::string& token) override;
    Skyfire::Cluster::Handoff::Result Inspect(Skyfire::Cluster::Handoff::Binding const&, std::uint64_t now,
        std::string& token) override;
    Skyfire::Cluster::Handoff::Result Consume(std::string const&, Skyfire::Cluster::Handoff::Binding const&,
        std::uint64_t now) override;
    Skyfire::Cluster::Handoff::Result Revoke(std::string const&, Skyfire::Cluster::Handoff::Binding const&,
        std::uint64_t now) override;
    void Cleanup(std::uint64_t now) override;
    Skyfire::Cluster::Handoff::Counters Status() const;
private:
    struct State;
    std::unique_ptr<State> _state;
};

#endif
