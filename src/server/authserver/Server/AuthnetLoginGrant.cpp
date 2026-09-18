/* Part of Project SkyFire. See LICENSE.md for copyright information. */
#include "AuthnetLoginGrant.h"
#include "Cluster/HandoffClient.h"
#include "Cluster/HandoffService.h"
#include <chrono>
#include <mutex>

namespace Skyfire::Authnet
{
namespace
{
    using namespace Skyfire::Cluster::Handoff;
    std::mutex LocalLock;
    MemoryStore LocalStore(RandomToken);
    Result Run(Operation action, uint32 account, std::string const& address, uint32 ttl,
        std::string const& selected, std::string* token)
    {
        Request request; request.Action = action; request.Ttl = ttl; request.Token = selected;
        request.Bind.Account = account; request.Bind.Address = address;
        request.Bind.Destination = "authnet"; request.Bind.Evidence = "-";
        if (Enabled()) return Call(request,token);
        std::lock_guard<std::mutex> guard(LocalLock);
        auto const now = std::uint64_t(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count());
        LocalStore.Cleanup(now);
        std::string result;
        auto status = Execute(LocalStore,request,now,result);
        if (token) *token = std::move(result);
        return status;
    }
}
bool IssueLoginGrant(uint32 account, std::string const& address, uint32 ttl)
{ return Run(Operation::Issue,account,address,ttl,"-",nullptr) == Result::Ok; }
bool HasLoginGrant(uint32 account, std::string const& address, std::string* token)
{ return Run(Operation::Inspect,account,address,0,"-",token) == Result::Ok; }
bool ConsumeLoginGrant(uint32 account, std::string const& address, std::string const& token)
{ return Run(Operation::Consume,account,address,0,token,nullptr) == Result::Ok; }
}
