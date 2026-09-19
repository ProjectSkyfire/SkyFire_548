/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "HubControlProtocol.h"
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
        std::cout << "Control protocol checks passed.\n";
        return 0;
    }
    catch (std::exception const& error) { std::cerr << error.what() << '\n'; return 1; }
}
