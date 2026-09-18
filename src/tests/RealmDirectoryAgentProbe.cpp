/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "Cluster/ClusterAgent.h"
#include "Cluster/RealmDirectory.h"
#include "Config.h"
#include <iostream>
#include <stdexcept>
#include <thread>

// Exercise the real async agent without loading production configuration or log appenders.
std::string ConfigMgr::GetStringDefault(char const*, std::string const& value) { return value; }
bool ConfigMgr::GetBoolDefault(char const*, bool value) { return value; }
int ConfigMgr::GetIntDefault(char const*, int value) { return value; }
std::string const& ConfigMgr::GetFilename() { return _filename; }

int main(int argc, char** argv)
{
    using namespace Skyfire::Cluster;
    if (argc != 5) { std::cerr << "Expected port CA certificate private-key\n"; return 1; }
    try
    {
        AgentOptions options; options.Enabled = options.HandoffEnabled = options.RealmDirectoryEnabled = true;
        options.Host = "localhost"; options.Port = std::uint16_t(std::stoul(argv[1]));
        options.CA = argv[2]; options.Certificate = argv[3]; options.PrivateKey = argv[4];
        auto& node = options.Advertisement; node.Key = node.Name = "auth-agent"; node.Address = "127.0.0.1";
        node.Port = 1121; node.Build = 18414; node.Capabilities = 208; node.Type = Service::Auth;
        std::vector<Realms::Query> queries;
        for (unsigned id = 1; id <= 20; ++id) queries.push_back({id,18414});
        Realms::Client.SetQueries(queries);
        Agent agent; std::string error;
        if (!agent.Start(options,[] { return AgentSample{true,0}; },error)) throw std::runtime_error(error);
        auto wait = [&](Realms::Status state)
        {
            auto deadline = Realms::Now() + 15000;
            while (Realms::Now() < deadline)
            {
                auto now = Realms::Now();
                if (Realms::Client.Read({1,18414},now).State == state && Realms::Client.Read({2,18414},now).State == state) return;
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
            throw std::runtime_error("Agent cache did not reach expected state");
        };
        wait(Realms::Status::Ready);
        auto owner = Realms::Client.Read({2,18414},Realms::Now()).Destination;
        std::cout << "READY" << std::endl;
        wait(Realms::Status::Offline); std::cout << "OFFLINE" << std::endl;
        wait(Realms::Status::Ready);
        if (owner == Realms::Client.Read({2,18414},Realms::Now()).Destination) throw std::runtime_error("Old world owner retained after restart");
        agent.Stop();
        if (Realms::Client.Read({1,18414},Realms::Now()).State != Realms::Status::Offline) throw std::runtime_error("Agent stop retained routes");
        std::cout << "RECOVERED" << std::endl;
    }
    catch (std::exception const& error) { std::cerr << error.what() << '\n'; return 1; }
}
