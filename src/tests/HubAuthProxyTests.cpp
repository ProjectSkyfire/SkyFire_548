/* Part of Project SkyFire. See LICENSE.md for copyright information. */
#include "HubAuthProxy.h"
#include "Network/ProxyProtocol.h"
#include <boost/asio/write.hpp>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

using boost::asio::ip::tcp;
using Skyfire::Cluster::Node;
namespace
{
    void Require(bool ok, char const* message) { if (!ok) throw std::runtime_error(message); }
    Node Auth(std::string key, unsigned short port)
    {
        Node n; n.Key = std::move(key); n.Name = n.Key; n.Type = Skyfire::Cluster::Service::Auth;
        n.Address = "127.0.0.1"; n.Port = port; n.Build = 18414; n.Capabilities = 16;
        n.Owner = 1; n.Ready = true; n.ExpiresAt = UINT64_MAX;
        return n;
    }
    void Selection()
    {
        Skyfire::Cluster::AuthRouting policy; policy.PreserveClientIP = false;
        policy.Weights = {{"a",3},{"b",1}};
        std::vector<Node> nodes{Auth("a",1001),Auth("b",1002)};
        unsigned a = 0, b = 0;
        Node choice;
        for (unsigned i = 0; i < 200; ++i)
        {
            Require(policy.Select(nodes, 1, {}, choice), "No eligible weighted route");
            (choice.Key == "a" ? a : b)++; policy.Release(choice);
        }
        Require(a == 150 && b == 50, "Weighted distribution is not 3:1");
        nodes[0].Ready = false; nodes[1].Capacity = 1;
        Require(policy.Select(nodes,1,{},choice) && choice.Key == "b", "Not-ready node selected");
        Require(!policy.Select(nodes,1,{},choice), "Pending reservation exceeded capacity");
        policy.Release(nodes[1]); nodes[1].Load = 1;
        Require(!policy.Select(nodes,1,{},choice), "Reported backend capacity ignored");
        nodes[1].Load = 0; nodes[1].ExpiresAt = 1;
        Require(!policy.Select(nodes,1,{},choice), "Expired lease selected");
        nodes[1].ExpiresAt = 100; nodes[1].Capabilities = 0;
        Require(!policy.Select(nodes,1,{},choice), "Authnet routed to legacy endpoint");
        nodes[1].Capabilities = 48;
        Require(!policy.Select(nodes,1,{},choice), "Raw stream routed to PROXY-only endpoint");
        policy.PreserveClientIP = true;
        Require(policy.Select(nodes,1,{},choice), "PROXY capability not recognized"); policy.Release(choice);
        Require(!policy.Select(nodes,1,{"b"},choice), "Retry selected a failed node twice");
        nodes[1].Build = 123;
        Require(!policy.Select(nodes,1,{},choice), "Wrong client build selected");
        nodes[1].Build = 18414; nodes[1].Type = Skyfire::Cluster::Service::World;
        Require(!policy.Select(nodes,1,{},choice), "Authentication routed to a world node");
        nodes[1].Type = Skyfire::Cluster::Service::Auth;
        Require(policy.Select(nodes,1,{},choice), "Failed to reserve old owner");
        auto old = choice; nodes[1].Owner = 2;
        Require(policy.Select(nodes,1,{},choice), "New owner inherited an old process reservation");
        policy.Release(old);
        Require(!policy.Select(nodes,1,{},choice), "Old owner release removed a new reservation");
        policy.Release(nodes[1]);
    }
    void HeaderValidation()
    {
        tcp::endpoint endpoint;
        Require(Skyfire::Net::ParseProxyHeader("PROXY TCP4 192.0.2.1 192.0.2.2 1234 1118\r\n",endpoint) &&
            endpoint.address().to_string() == "192.0.2.1" && endpoint.port() == 1234, "Valid IPv4 header rejected");
        Require(Skyfire::Net::ParseProxyHeader("PROXY TCP6 2001:db8::1 2001:db8::2 1234 1118\r\n",endpoint), "Valid IPv6 header rejected");
        for (auto const& bad : {"PROXY UNKNOWN\r\n", "PROXY TCP4 ::1 ::1 1234 1118\r\n",
            "PROXY TCP4 127.0.0.1 127.0.0.1 0 1118\r\n", "PROXY TCP4 127.0.0.1 127.0.0.1 1234 65536\r\n",
            "PROXY TCP4 127.0.0.1 127.0.0.1 1234 1118\n", "PROXY TCP4 127.0.0.1 127.0.0.1 1234 1118 extra\r\n"})
            Require(!Skyfire::Net::ParseProxyHeader(bad,endpoint), "Malformed PROXY header accepted");
        Require(!Skyfire::Net::ParseProxyHeader(std::string(109,'x')+"\r\n",endpoint), "Oversized header accepted");
        std::vector<boost::asio::ip::address> peers;
        Require(Skyfire::Net::ParseProxyPeers("127.0.0.1 ::1",peers) && peers.size() == 2, "Trusted peer parsing failed");
        peers.clear(); Require(!Skyfire::Net::ParseProxyPeers("0.0.0.0",peers), "Wildcard proxy trust accepted");
    }
    struct EchoPeer : std::enable_shared_from_this<EchoPeer>
    {
        std::shared_ptr<tcp::socket> Socket;
        std::array<char,4096> Buffer{};
        std::string Received;
        tcp::endpoint Source;
        bool Drop = false, HasIdentity = false;
        explicit EchoPeer(std::shared_ptr<tcp::socket> socket, bool drop) : Socket(std::move(socket)), Drop(drop) { }
        void Read()
        {
            auto self = shared_from_this();
            Socket->async_read_some(boost::asio::buffer(Buffer),[self](boost::system::error_code ec,std::size_t count)
            {
                if (ec) { boost::system::error_code ignored; self->Socket->close(ignored); return; }
                self->Received.append(self->Buffer.data(),count);
                if (self->Drop) { boost::system::error_code ignored; self->Socket->close(ignored); return; }
                boost::asio::async_write(*self->Socket,boost::asio::buffer(self->Buffer.data(),count),
                    [self](boost::system::error_code error,std::size_t) { if (!error) self->Read(); });
            });
        }
    };
    struct EchoServer
    {
        boost::asio::io_context& Io;
        tcp::acceptor Acceptor;
        std::vector<std::shared_ptr<EchoPeer>> Peers;
        bool Proxy, Drop;
        EchoServer(boost::asio::io_context& io, bool proxy = false, bool drop = false)
            : Io(io), Acceptor(io,tcp::endpoint(boost::asio::ip::address_v4::loopback(),0)), Proxy(proxy), Drop(drop) { Accept(); }
        unsigned short Port() const { return Acceptor.local_endpoint().port(); }
        void Accept()
        {
            Acceptor.async_accept([this](boost::system::error_code ec,tcp::socket socket)
            {
                if (ec) return;
                auto peer = std::make_shared<EchoPeer>(std::make_shared<tcp::socket>(std::move(socket)),Drop);
                Peers.push_back(peer);
                if (Proxy) Skyfire::Net::ProxyHeaderReader::Start(peer->Socket,[peer](bool ok,tcp::endpoint source)
                { peer->HasIdentity = ok; peer->Source = source; if (ok) peer->Read(); });
                else peer->Read();
                Accept();
            });
        }
    };
    struct Harness
    {
        boost::asio::io_context Io;
        HubAuthProxy Proxy;
        std::vector<Node> Nodes;
        unsigned short LastClientPort = 0;
        void Open(bool identity = false, unsigned limit = 16)
        {
            HubAuthProxyOptions options; options.Port = 0; options.PreserveClientIP = identity;
            options.MaxConnections = limit;
            std::string error; Require(Proxy.Open(options,error),error.c_str());
        }
        void Pump(std::function<bool()> done)
        {
            auto deadline = std::chrono::steady_clock::now()+std::chrono::seconds(5);
            while (!done() && std::chrono::steady_clock::now()<deadline)
            {
                Proxy.Update(Nodes); Io.restart(); Io.poll();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            Require(done(),"Transport test timed out");
        }
        std::shared_ptr<tcp::socket> Connect()
        {
            auto client = std::make_shared<tcp::socket>(Io);
            client->connect(tcp::endpoint(boost::asio::ip::address_v4::loopback(),Proxy.Status().Port));
            return client;
        }
        std::string Exchange(std::string payload)
        {
            auto client = Connect();
            LastClientPort = client->local_endpoint().port();
            bool ended = false, written = false;
            std::string result;
            std::array<char,4096> buffer{};
            std::function<void()> read;
            read = [&]
            {
                client->async_read_some(boost::asio::buffer(buffer),[&](boost::system::error_code ec,std::size_t n)
                { result.append(buffer.data(),n); if (ec) ended = true; else read(); });
            };
            read();
            boost::asio::async_write(*client,boost::asio::buffer(payload),[&](boost::system::error_code,std::size_t)
            { written = true; boost::system::error_code ec; client->shutdown(tcp::socket::shutdown_send,ec); });
            Pump([&] { return ended && written; });
            return result;
        }
    };
    void Transport()
    {
        Harness h; EchoServer a(h.Io), b(h.Io); h.Open();
        h.Nodes = {Auth("a",a.Port()),Auth("b",b.Port())};
        std::string binary(100000,'x'); for (std::size_t i = 0; i < binary.size(); ++i) binary[i] = char(i % 256);
        Require(h.Exchange(binary) == binary,"Binary stream/backpressure/half-close lost bytes");
        Require(h.Exchange("second") == "second","Second backend exchange failed");
        Require(a.Peers.size() == 1 && b.Peers.size() == 1,"Connections did not distribute across backends");
        Require(h.Proxy.Status().ClientBytes == binary.size()+6 && h.Proxy.Status().BackendBytes == binary.size()+6,"Byte counters incorrect");

        auto pinned = h.Connect();
        h.Pump([&] { return a.Peers.size()+b.Peers.size() == 3; });
        h.Nodes[0].Ready = false; h.Nodes[1].Ready = false;
        Require(h.Exchange("reject") == "","Not-ready backend received new client");
        boost::asio::write(*pinned,boost::asio::buffer("pinned",6));
        std::array<char,6> reply{}; bool received = false;
        boost::asio::async_read(*pinned,boost::asio::buffer(reply),[&](boost::system::error_code ec,std::size_t n)
        { Require(!ec && n == 6,"Pinned connection dropped after readiness changed"); received = true; });
        h.Pump([&] { return received; });
        Require(std::string(reply.data(),6) == "pinned","Pinned stream moved or corrupted");
        boost::system::error_code ignored; pinned->close(ignored);
        h.Proxy.Close();
    }
    void RetryAndNoReplay()
    {
        Harness h; EchoServer good(h.Io); h.Open();
        tcp::acceptor unavailable(h.Io,tcp::endpoint(boost::asio::ip::address_v4::loopback(),0));
        auto deadPort = unavailable.local_endpoint().port(); unavailable.close();
        h.Nodes = {Auth("a-dead",deadPort),Auth("b-good",good.Port())};
        Require(h.Exchange("retry-on-connect") == "retry-on-connect","Safe connect fallback failed");
        Require(h.Proxy.Status().Retries == 1 && h.Proxy.Status().ConnectFailures == 1,"Connect retry counters wrong");
        h.Proxy.Close();

        Harness pinned; EchoServer drop(pinned.Io,false,true), spare(pinned.Io); pinned.Open();
        pinned.Nodes = {Auth("a-drop",drop.Port()),Auth("b-spare",spare.Port())};
        Require(pinned.Exchange("must-not-replay").empty(),"Dropping backend returned unexpected bytes");
        Require(spare.Peers.empty() && pinned.Proxy.Status().Retries == 0 && !drop.Peers[0]->Received.empty(),
            "Client bytes replayed to another backend after connect");

        Harness loop; EchoServer healthy(loop.Io); loop.Open();
        loop.Nodes = {Auth("a-self",loop.Proxy.Status().Port),Auth("b-healthy",healthy.Port())};
        Require(loop.Exchange("avoid-self-loop") == "avoid-self-loop" && loop.Proxy.Status().ConnectFailures == 1,
            "Ingress self-route was not excluded before connecting");
    }
    void ClientIdentityAndLimits()
    {
        Harness h; EchoServer backend(h.Io,true); h.Open(true,1);
        auto node = Auth("proxy",backend.Port()); node.Capabilities |= 32; h.Nodes = {node};
        Require(h.Exchange("client-protocol") == "client-protocol","Identity header leaked into client stream");
        Require(backend.Peers.size() == 1 && backend.Peers[0]->HasIdentity && backend.Peers[0]->Source.address().is_loopback() &&
            backend.Peers[0]->Source.port() == h.LastClientPort && backend.Peers[0]->Received == "client-protocol",
            "Client identity not restored before protocol bytes");
        h.Pump([&] { return h.Proxy.Status().Active == 0; });
        auto held = h.Connect(); h.Pump([&] { return backend.Peers.size() == 2; });
        Require(h.Exchange("over-limit").empty() && backend.Peers.size() == 2,"Listener connection limit ignored");
        boost::system::error_code ec; held->close(ec);
    }
    void HeaderFailurePaths()
    {
        for (auto const& payload : {std::string("not-a-proxy-header\n"), std::string(108,'x'), std::string("PROXY TCP4 ")})
        {
            boost::asio::io_context io;
            tcp::acceptor acceptor(io,tcp::endpoint(boost::asio::ip::address_v4::loopback(),0));
            tcp::socket client(io);
            client.connect(acceptor.local_endpoint());
            auto server = std::make_shared<tcp::socket>(io); acceptor.accept(*server);
            bool completed = false, accepted = false;
            Skyfire::Net::ProxyHeaderReader::Start(server,[&](bool ok,tcp::endpoint) { completed = true; accepted = ok; });
            boost::asio::write(client,boost::asio::buffer(payload));
            io.run_for(std::chrono::seconds(6));
            Require(completed && !accepted && !server->is_open(),"Malformed, oversized or stalled PROXY header survived its deadline");
        }
    }
}
int main()
{
    try
    {
        Selection(); HeaderValidation(); Transport(); RetryAndNoReplay(); ClientIdentityAndLimits(); HeaderFailurePaths();
        std::cout << "Auth ingress tests passed: weights, readiness/leases/capacity, protocol isolation, binary forwarding, half-close, pinning, safe retry, no replay, client identity and listener limits.\n";
        return 0;
    }
    catch (std::exception const& error) { std::cerr << error.what() << '\n'; return 1; }
}
