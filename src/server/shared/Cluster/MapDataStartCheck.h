/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_MAP_DATA_START_CHECK_H
#define SKYFIRE_MAP_DATA_START_CHECK_H
#include "MapDataDirectory.h"
#include <istream>
#include <map>
#include <regex>
#include <set>
#include <sstream>

namespace Skyfire::Cluster::MapData
{
    // Read independently: never replace the running hub's ConfigMgr settings.
    inline bool CheckWorldStart(std::istream& input, std::vector<Node> const& nodes,
        std::uint64_t now, std::string& error)
    {
        auto reject = [&](std::string const& why) { error = "World start blocked: " + why; return false; };
        if (!input) return reject("cannot read world configuration.");
        auto trim = [](std::string value)
        {
            auto first = value.find_first_not_of(" \t\r\n");
            return first == std::string::npos ? std::string() : value.substr(first, value.find_last_not_of(" \t\r\n") - first + 1);
        };
        std::map<std::string,std::string> settings;
        std::string line;
        while (std::getline(input,line))
        {
            char quote = 0;
            for (std::size_t i = 0; i < line.size(); ++i)
            {
                char c = line[i];
                if ((c == '\'' || c == '"') && (!i || line[i-1] != '\\'))
                { if (!quote) quote = c; else if (quote == c) quote = 0; }
                else if (!quote && (c == '#' || c == ';')) { line.resize(i); break; }
            }
            auto equals = line.find('='); if (equals == std::string::npos) continue;
            auto key = trim(line.substr(0,equals));
            if (key != "MapData.Enable" && key != "MapData.Sources") continue;
            auto value = trim(line.substr(equals+1));
            if (value.size() >= 2 && (value.front() == '\'' || value.front() == '"') && value.back() == value.front())
                value = value.substr(1,value.size()-2);
            if (!settings.emplace(key,value).second) return reject("duplicate map configuration setting.");
        }
        auto enabled = settings["MapData.Enable"];
        if (enabled.empty() || enabled == "0" || enabled == "false" || enabled == "FALSE" || enabled == "no" || enabled == "NO") return true;
        if (enabled != "1" && enabled != "true" && enabled != "TRUE" && enabled != "yes" && enabled != "YES")
            return reject("invalid MapData.Enable.");
        std::istringstream sources(settings["MapData.Sources"]);
        std::set<std::string> providers; std::set<unsigned> maps;
        std::regex format("([A-Za-z0-9_.-]{1,64})=([0-9]{1,4}(,[0-9]{1,4}){0,511})");
        std::string source;
        while (sources >> source)
        {
            std::smatch match;
            if (!std::regex_match(source,match,format) || !providers.insert(match[1].str()).second)
                return reject("invalid or duplicate map provider assignment.");
            std::istringstream ids(match[2].str()); std::string id;
            while (std::getline(ids,id,','))
                if (!maps.insert(unsigned(std::stoul(id))).second) return reject("duplicate map assignment.");
            if (Resolve(match[1].str(),nodes,now).Bytes.at(1) != 1)
                return reject("required mapserver '" + match[1].str() + "' is offline, unready, drained, disabled or full. Start it and retry.");
        }
        if (providers.empty() || providers.size() > 16 || maps.size() > 512)
            return reject("configure 1..16 map providers and 1..512 map IDs.");
        return true;
    }
}
#endif
