/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "HubControlProtocol.h"
#include "HubBackupSchedule.h"
#include <iostream>
#include <stdexcept>
using namespace Skyfire::Control;
namespace
{
    void Check(bool ok, char const* message) { if (!ok) throw std::runtime_error(message); }
}
int main()
{
    try
    {
        Command decoded;
        std::map<std::string,std::string> form = {{"id",std::string(32,'a')},{"action","world.restart"},{"target","world-a"},{"seconds","300"}};
        Check(Decode(form,decoded) && decoded.WorldCommand == "server restart 300", "Restart must use the graceful command channel");
        for (auto bad : {"-1","86401","001","1.5","300; stop","300\nshutdown","999999999999999999999"})
        { form["seconds"] = bad; Check(!Decode(form,decoded),"Invalid countdown accepted"); }
        form["seconds"] = "0"; Check(Decode(form,decoded),"Immediate graceful shutdown unavailable");
        form["seconds"] = "86400"; Check(Decode(form,decoded),"Maximum countdown unavailable");
        form.erase("seconds");
        for (auto action : {"cluster.drain","cluster.disable","cluster.enable","service.start","service.stop","world.cancel-restart","world.cancel-shutdown"})
        { form["action"] = action; Check(Decode(form,decoded),"Allowlisted action missing"); }
        for (auto action : {"shell","sql","process.exec","file.read","memory.write","server restart 0"})
        { form["action"] = action; Check(!Decode(form,decoded),"Arbitrary operation accepted"); }
        form["action"] = "service.start";
        for (auto target : {"../world","world;shutdown","C:\\server.exe","world\nstop",""})
        { form["target"] = target; Check(!Decode(form,decoded),"Arbitrary target accepted"); }
        form["target"] = "world-a"; form["path"] = "server.exe";
        Check(!Decode(form,decoded),"Extra parameter accepted"); form.erase("path");
        form["id"] = std::string(32,'A'); Check(!Decode(form,decoded),"Noncanonical identifier accepted");
        Check(std::string(Role(17)) == "viewer" && std::string(Role(21)) == "operator" &&
              std::string(Role(31)) == "administrator" && std::string(Role(49)) == "recovery", "Role mapping changed");
        Skyfire::Backup::Schedule schedule;
        std::map<std::string,std::string> backup = {{"id",std::string(32,'a')},{"target","auth"},{"enabled","1"},
            {"mode","interval"},{"intervalMinutes","30"},{"minuteOfDay","0"},{"weekday","0"},{"revision","0"}};
        Check(Skyfire::Backup::Decode(backup,schedule),"Valid interval rejected");
        for (auto bad : {"0","14","10081","015","-1","true","99999999999"})
        { backup["intervalMinutes"] = bad; Check(!Skyfire::Backup::Decode(backup,schedule),"Invalid backup interval accepted"); }
        backup["intervalMinutes"] = "60"; backup["mode"] = "daily"; backup["minuteOfDay"] = "1439";
        Check(Skyfire::Backup::Decode(backup,schedule),"Daily UTC schedule rejected");
        backup["timeZone"]="server";
        Check(Skyfire::Backup::Decode(backup,schedule) && schedule.TimeZone=="server","Server time zone rejected");
        backup["timeZone"]="CST"; Check(!Skyfire::Backup::Decode(backup,schedule),"Ambiguous fixed time-zone abbreviation accepted");
        backup.erase("timeZone");
        backup["minuteOfDay"] = "1440"; Check(!Skyfire::Backup::Decode(backup,schedule),"Invalid time accepted");
        backup["minuteOfDay"] = "180"; backup["mode"] = "weekly"; backup["weekday"] = "6";
        Check(Skyfire::Backup::Decode(backup,schedule),"Weekly UTC schedule rejected");
        backup["weekday"] = "7"; Check(!Skyfire::Backup::Decode(backup,schedule),"Invalid weekday accepted");
        backup["weekday"] = "0"; backup["target"] = "../database";
        Check(!Skyfire::Backup::Decode(backup,schedule),"Arbitrary database accepted");
        backup["target"] = "world"; backup["cron"] = "* * * * *";
        Check(!Skyfire::Backup::Decode(backup,schedule),"Unknown schedule field accepted");
        std::cout << "Control protocol checks passed.\n";
        return 0;
    }
    catch (std::exception const& error) { std::cerr << error.what() << '\n'; return 1; }
}
