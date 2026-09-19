/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_HUB_CONTROL_PROTOCOL_H
#define SKYFIRE_HUB_CONTROL_PROTOCOL_H
#include "Cluster/ClusterProtocol.h"
#include <map>
#include <string>
#include <cstdint>
namespace Skyfire::Control
{
    constexpr std::uint64_t View = 1, Operate = 4, Administrator = 15, Remote = 16, Recovery = 32;
    inline bool RequestId(std::string const& id)
    { return id.size() == 32 && id.find_first_not_of("0123456789abcdef") == std::string::npos; }
    inline char const* Role(std::uint64_t flags)
    {
        if ((flags & Administrator) == Administrator) return "administrator";
        if (flags & Recovery) return "recovery";
        return flags & Operate ? "operator" : "viewer";
    }
    struct Command { std::string Id, Action, Target, WorldCommand, ClusterAction; bool Start = false; };
    inline bool Decode(std::map<std::string,std::string> const& form, Command& command)
    {
        auto value = [&](char const* key) { auto found = form.find(key); return found == form.end() ? std::string() : found->second; };
        command = {}; command.Id = value("id"); command.Action = value("action"); command.Target = value("target");
        if (!RequestId(command.Id) || !Cluster::ValidKey(command.Target)) return false;
        bool const schedule = command.Action == "world.shutdown" || command.Action == "world.restart";
        if (form.size() != (schedule ? 4u : 3u)) return false;
        if (command.Action == "cluster.drain" || command.Action == "cluster.disable" || command.Action == "cluster.enable")
            command.ClusterAction = command.Action.substr(8);
        else if (command.Action == "service.start" || command.Action == "service.stop") command.Start = command.Action == "service.start";
        else if (schedule)
        {
            auto seconds = value("seconds");
            if (seconds.empty() || seconds.size() > 5 || seconds.find_first_not_of("0123456789") != std::string::npos ||
                (seconds.size() > 1 && seconds[0] == '0') || std::stoul(seconds) > 86400) return false;
            command.WorldCommand = "server " + command.Action.substr(6) + " " + seconds;
        }
        else if (command.Action == "world.cancel-shutdown") command.WorldCommand = "server shutdown cancel";
        else if (command.Action == "world.cancel-restart") command.WorldCommand = "server restart cancel";
        else return false;
        return true;
    }
}
#endif
