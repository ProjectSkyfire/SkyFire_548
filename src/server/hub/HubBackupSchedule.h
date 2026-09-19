/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_HUB_BACKUP_SCHEDULE_H
#define SKYFIRE_HUB_BACKUP_SCHEDULE_H
#include <cstdint>
#include <map>
#include <string>
namespace Skyfire::Backup
{
    struct Schedule
    {
        std::string Id, Target, Mode;
        std::uint32_t Enabled = 0, IntervalMinutes = 60, MinuteOfDay = 0, Weekday = 0, Revision = 0;
    };
    inline bool Decode(std::map<std::string,std::string> const& form, Schedule& schedule)
    {
        if (form.size() != 8) return false;
        auto value = [&](char const* name) { auto it = form.find(name); return it == form.end() ? std::string() : it->second; };
        schedule.Id = value("id"); schedule.Target = value("target"); schedule.Mode = value("mode");
        if (schedule.Id.size() != 32 || schedule.Id.find_first_not_of("0123456789abcdef") != std::string::npos ||
            (schedule.Target != "auth" && schedule.Target != "characters" && schedule.Target != "world" && schedule.Target != "hub")) return false;
        auto number = [&](char const* key, std::uint32_t max, std::uint32_t& result)
        {
            auto text = value(key);
            if (text.empty() || text.size() > 10 || (text.size() > 1 && text[0] == '0') ||
                text.find_first_not_of("0123456789") != std::string::npos) return false;
            auto parsed = std::stoull(text);
            if (parsed > max) return false;
            result = static_cast<std::uint32_t>(parsed); return true;
        };
        if (!number("enabled",1,schedule.Enabled) || !number("intervalMinutes",10080,schedule.IntervalMinutes) ||
            !number("minuteOfDay",1439,schedule.MinuteOfDay) || !number("weekday",6,schedule.Weekday) ||
            !number("revision",4294967294u,schedule.Revision)) return false;
        if (schedule.Mode == "interval") return schedule.IntervalMinutes >= 15 && !schedule.MinuteOfDay && !schedule.Weekday;
        if (schedule.Mode == "daily") return schedule.IntervalMinutes == 60 && !schedule.Weekday;
        return schedule.Mode == "weekly" && schedule.IntervalMinutes == 60;
    }
}
#endif
