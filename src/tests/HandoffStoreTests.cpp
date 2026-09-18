/* Part of Project SkyFire. See LICENSE.md for copyright information. */
#include "Cluster/HandoffService.h"
#include <iostream>
#include <stdexcept>
using namespace Skyfire::Cluster;
using namespace Skyfire::Cluster::Handoff;
namespace
{
    void Require(bool ok, char const* message) { if (!ok) throw std::runtime_error(message); }
    std::string NextToken()
    {
        static unsigned counter = 0;
        auto tail = std::to_string(++counter);
        return std::string(64-tail.size(),'0')+tail;
    }
    Node Live(std::string key, Service type, unsigned owner)
    {
        Node n; n.Key = std::move(key); n.Type = type; n.Owner = owner;
        n.Ready = true; n.Capabilities = 64; n.ExpiresAt = 100000; n.Realms = {1}; return n;
    }
    void Lifecycle()
    {
        MemoryStore store(NextToken,8);
        Binding binding{42,1,Purpose::World,"127.0.0.1","world:1",std::string(64,'a')};
        std::string token, another;
        Require(store.Issue(binding,1,100,token) == Result::Ok && IsToken(token),"Issue failed");
        auto wrong = binding; ++wrong.Account;
        Require(store.Consume(token,wrong,101) == Result::Denied,"Wrong account accepted");
        wrong = binding; ++wrong.Realm;
        Require(store.Consume(token,wrong,101) == Result::Denied,"Wrong realm accepted");
        wrong = binding; wrong.Destination = "world:2";
        Require(store.Consume(token,wrong,101) == Result::Denied,"Replacement process accepted old token");
        wrong = binding; wrong.Address = "127.0.0.2";
        Require(store.Consume(token,wrong,101) == Result::Denied,"Wrong client address accepted");
        wrong = binding; wrong.Evidence = std::string(64,'b');
        Require(store.Consume(token,wrong,101) == Result::Denied,"Wrong key fingerprint accepted");
        Require(store.Consume(token,binding,101) == Result::Ok,"Bound consume failed");
        Require(store.Consume(token,binding,102) == Result::Replay,"Replay accepted");
        Require(store.Issue(binding,1,200,another) == Result::Ok && another != token,"Token reused");
        Require(store.Consume(another,binding,1200) == Result::Expired,"Expiry boundary accepted");
        store.Cleanup(1200); Require(store.Size() == 0 && store.Status().Expired == 1,"Expiry cleanup wrong");
        Require(store.Issue(binding,1,1300,token) == Result::Ok,"Reissue failed");
        Require(store.Revoke(token,binding,1301) == Result::Ok,"Revoke failed");
        Require(store.Consume(token,binding,1302) == Result::Replay,"Revoked token accepted");
        Require(store.Issue(binding,1,1400,token) == Result::Ok,"Issue failed");
        Require(store.Issue(binding,1,1401,another) == Result::Ok,"Replacement issue failed");
        Require(store.Consume(token,binding,1402) == Result::Replay,"Superseded token accepted");
        store.Clear(); Require(store.Consume(another,binding,1403) == Result::Missing,"Hub restart retained token");
        MemoryStore bounded(NextToken,1);
        Require(bounded.Issue(binding,1,0,token) == Result::Ok,"Bounded issue failed");
        Require(bounded.Issue(binding,1,1,another) == Result::Capacity,"Unbounded storage");
        Require(bounded.Issue(binding,0,1,another) == Result::Invalid,"Zero TTL accepted");
        MemoryStore noEntropy([] { return std::string(); });
        Require(noEntropy.Issue(binding,1,0,token) == Result::Unavailable,"RNG failure accepted");
    }
    void CrossNode()
    {
        MemoryStore store(NextToken);
        std::vector<Node> nodes{Live("auth-a",Service::Auth,1),Live("auth-b",Service::Auth,2),Live("world",Service::World,3)};
        Request r; r.Action = Operation::Issue; r.Ttl = 60;
        r.Bind.Account = 42; r.Bind.Address = "127.0.0.1"; r.Bind.Evidence = "-";
        std::string token;
        Require(Authorize("auth-a",nodes,1,r) == Result::Ok && Execute(store,r,1,token) == Result::Ok,"Auth A could not issue");
        r.Action = Operation::Consume; r.Ttl = 0; r.Token = token;
        Require(Authorize("auth-b",nodes,2,r) == Result::Ok && Execute(store,r,2,token) == Result::Ok,"Cross-auth consume failed");
        Require(Execute(store,r,3,token) == Result::Replay,"Cross-auth replay accepted");
        Require(Authorize("world",nodes,3,r) == Result::Denied,"World consumed auth-only grant");
        r.Action = Operation::Issue; r.Ttl = 60; r.Token = "-"; r.Bind.Use = Purpose::World;
        r.Bind.Realm = 1; r.Bind.Evidence = std::string(64,'a');
        Require(Authorize("auth-a",nodes,4,r) == Result::Ok && Execute(store,r,4,token) == Result::Ok,"World handoff issue failed");
        r.Action = Operation::Consume; r.Ttl = 0; r.Token = token;
        Require(Authorize("auth-b",nodes,5,r) == Result::Denied,"Auth consumed world-only grant");
        nodes[2].Owner = 4;
        Require(Authorize("world",nodes,5,r) == Result::Ok && Execute(store,r,5,token) == Result::Denied,"World restart reused prior grant");
        nodes[2].Owner = 3;
        Require(Authorize("world",nodes,6,r) == Result::Ok && Execute(store,r,6,token) == Result::Ok,"World consume failed");
        nodes[2].Ready = false;
        Require(Authorize("world",nodes,7,r) == Result::Denied,"Unready caller accepted");
        r.Action = Operation::Issue;
        Require(Authorize("auth-a",nodes,7,r) == Result::Unavailable,"Offline realm accepted");
        nodes[2].Ready = true; nodes.push_back(Live("world-other",Service::World,5));
        Require(Authorize("auth-a",nodes,7,r) == Result::Unavailable,"Ambiguous realm accepted");
        Require(Authorize("unknown",nodes,7,r) == Result::Denied,"Unregistered certificate identity accepted");
    }
    void Wire()
    {
        Request r; r.Action = Operation::Issue; r.Ttl = 60; r.Bind.Account = 42;
        r.Bind.Address = "127.0.0.1"; r.Bind.Evidence = "-";
        auto wire = Encode(r).Bytes; Request decoded;
        Require(Decode(wire,decoded),"Valid request rejected");
        for (std::size_t i = 0; i < wire.size(); ++i)
            Require(!Decode(std::vector<std::uint8_t>(wire.begin(),wire.begin()+i),decoded),"Truncated request accepted");
        auto bad = wire; bad.push_back(0); Require(!Decode(bad,decoded),"Trailing data accepted");
        bad = wire; bad[0] = 2; Require(!Decode(bad,decoded),"Unknown subprotocol accepted");
        bad = wire; bad[1] = 0; Require(!Decode(bad,decoded),"Unknown operation accepted");
        r.Ttl = 901; Require(!Decode(Encode(r).Bytes,decoded),"Excessive TTL accepted");
    }
}
int main()
{
    try { Lifecycle(); CrossNode(); Wire(); std::cout << "Handoff binding, expiry, replay, revocation, cross-node authorization and wire checks passed.\n"; }
    catch (std::exception const& e) { std::cerr << e.what() << '\n'; return 1; }
}
