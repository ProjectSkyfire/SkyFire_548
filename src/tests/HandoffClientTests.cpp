/* Part of Project SkyFire. See LICENSE.md for copyright information. */
#include "Cluster/HandoffClient.h"
#include <iostream>
#include <set>
#include <stdexcept>
using namespace Skyfire::Cluster;
using namespace Skyfire::Cluster::Handoff;
namespace { void Require(bool ok, char const* message) { if (!ok) throw std::runtime_error(message); } }
int main(int argc, char** argv)
{
    try
    {
        std::set<std::string> tokens;
        for (unsigned i = 0; i < 512; ++i)
        { auto token = RandomToken(); Require(IsToken(token) && tokens.insert(token).second,"Invalid/duplicate random token"); }
        Require(Evidence("abc",3) == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad","Incorrect key fingerprint");
        Request r; r.Action = Operation::Issue; r.Ttl = 60; r.Bind.Account = 987654321;
        r.Bind.Address = "127.0.0.1"; r.Bind.Evidence = "-";
        Require(!Enabled() && Call(r) == Result::Unavailable,"Disabled client did not fail closed");
        if (argc == 6)
        {
            Require(std::string(argv[5]) == "--live","Expected --live integration mode");
            AgentOptions options; options.Enabled = options.HandoffEnabled = true;
            options.Host = "localhost"; options.Port = std::uint16_t(std::stoul(argv[1]));
            options.CA = argv[2]; options.Certificate = argv[3]; options.PrivateKey = argv[4];
            ConfigureClient(options);
            std::string token, diagnostic;
            auto result = Call(r,&token,&diagnostic);
            if (result != Result::Ok || !IsToken(token)) throw std::runtime_error("Live TLS client issue failed: "+diagnostic);
            r.Action = Operation::Consume; r.Ttl = 0; r.Token = token;
            Require(Call(r) == Result::Ok,"Live TLS client consume failed");
            Require(Call(r) == Result::Replay,"Live TLS client replay accepted");
            options.Certificate = "missing-client-certificate.pem"; ConfigureClient(options);
            Require(Call(r) == Result::Unavailable,"Missing certificate did not fail closed");
        }
        else Require(argc == 1,"Usage: handoff_client_tests [port ca certificate key --live]");
        std::cout << "Handoff client checks passed.\n";
    }
    catch (std::exception const& e) { std::cerr << e.what() << '\n'; return 1; }
}
