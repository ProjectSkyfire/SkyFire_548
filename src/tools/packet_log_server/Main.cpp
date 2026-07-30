/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include <atomic>
#include <chrono>
#include <csignal>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

namespace
{
std::atomic<bool> running(true);

void Stop(int)
{
    running = false;
}

void PrintUsage(char const* executable)
{
    std::cout
        << "Usage: " << executable << " [control-file]\n"
        << "\n"
        << "Creates a packet log control file while this process is running.\n"
        << "Configured servers write per-session packet logs when they see that file.\n"
        << "\n"
        << "Default control-file: packetlogserver.active\n";
}

bool WriteControlFile(std::filesystem::path const& controlFile)
{
    std::error_code error;
    std::filesystem::path parent = controlFile.parent_path();
    if (!parent.empty())
    {
        std::filesystem::create_directories(parent, error);
        if (error)
        {
            std::cerr << "Failed to create control file directory " << parent.string()
                << ": " << error.message() << "\n";
            return false;
        }
    }

    std::ofstream file(controlFile, std::ios::out | std::ios::trunc);
    if (!file)
    {
        std::cerr << "Failed to create control file " << controlFile.string() << "\n";
        return false;
    }

    file << "SkyFire packet log server active\n";
    file << "StartedUnixTime=" << static_cast<long long>(std::time(nullptr)) << "\n";
    return true;
}
}

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        std::string arg = argv[1];
        if (arg == "-h" || arg == "--help" || arg == "/?")
        {
            PrintUsage(argv[0]);
            return 0;
        }
    }

    std::filesystem::path controlFile = argc > 1 ? argv[1] : "packetlogserver.active";

    if (!WriteControlFile(controlFile))
        return 1;

    std::signal(SIGINT, Stop);
    std::signal(SIGTERM, Stop);

    std::cout << "SkyFire packet log server active.\n";
    std::cout << "Control file: " << std::filesystem::absolute(controlFile).string() << "\n";
    std::cout << "Press Ctrl+C to stop packet logging.\n";

    while (running)
        std::this_thread::sleep_for(std::chrono::milliseconds(250));

    std::error_code error;
    std::filesystem::remove(controlFile, error);
    if (error)
    {
        std::cerr << "Failed to remove control file " << controlFile.string()
            << ": " << error.message() << "\n";
        return 1;
    }

    std::cout << "SkyFire packet log server stopped.\n";
    return 0;
}
