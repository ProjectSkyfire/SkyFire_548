/* Part of Project SkyFire. See LICENSE.md for copyright information. */
#ifndef SKYFIRE_HANDOFF_CLIENT_H
#define SKYFIRE_HANDOFF_CLIENT_H
#include "ClusterAgent.h"
#include "HandoffProtocol.h"
namespace Skyfire::Cluster::Handoff
{
    // Configure once before accepting game connections. Disabled mode uses local adapters.
    void ConfigureClient(AgentOptions const& options);
    bool Enabled();
    Result Call(Request const& request, std::string* token = nullptr, std::string* diagnostic = nullptr);
    std::string RandomToken();
    std::string Evidence(void const* bytes, std::size_t size);
}
#endif
