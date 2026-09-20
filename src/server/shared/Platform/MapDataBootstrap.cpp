/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "MapDataBootstrap.h"
#include "Config.h"
#include "Log.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <regex>
#include <random>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#else
#include <spawn.h>
#include <sys/wait.h>
#include <signal.h>
#include <cerrno>
extern char** environ;
#endif

namespace
{
    std::string Utf8(std::filesystem::path const& path)
    {
        auto bytes = path.generic_u8string();
        return std::string(bytes.begin(), bytes.end());
    }
    bool Run(std::vector<std::string> const& args, unsigned timeout)
    {
        auto started = std::chrono::steady_clock::now();
        auto deadline = started + std::chrono::seconds(timeout);
        auto nextReport = started + std::chrono::seconds(5);
        auto report = [&]()
        {
            auto now = std::chrono::steady_clock::now();
            if (now >= nextReport)
            {
                SF_LOG_INFO("server.loading", "Preparing map cache (%u seconds elapsed); checking providers and cached assets.",
                    unsigned(std::chrono::duration_cast<std::chrono::seconds>(now - started).count()));
                nextReport = now + std::chrono::seconds(5);
            }
        };
#ifdef _WIN32
        // Quote every argument using the Windows CRT backslash/quote rules.
        std::wstring command;
        for (auto const& arg : args)
        {
            std::wstring value = std::filesystem::u8path(arg).wstring();
            command += L"\"";
            unsigned slashes = 0;
            for (wchar_t c : value)
            {
                if (c == L'\\') { ++slashes; continue; }
                command.append(c == L'"' ? slashes * 2 + 1 : slashes, L'\\');
                command += c; slashes = 0;
            }
            command.append(slashes * 2, L'\\'); command += L"\" ";
        }
        STARTUPINFOW startup{}; startup.cb = sizeof(startup);
        PROCESS_INFORMATION process{};
        if (!CreateProcessW(nullptr, command.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW,
            nullptr, nullptr, &startup, &process)) return false;
        CloseHandle(process.hThread);
        bool finished = false;
        do
        {
            DWORD result = WaitForSingleObject(process.hProcess, 1000);
            if (result == WAIT_OBJECT_0) { finished = true; break; }
            if (result == WAIT_FAILED) break;
            report();
        } while (std::chrono::steady_clock::now() < deadline);
        if (!finished) { TerminateProcess(process.hProcess, 1); WaitForSingleObject(process.hProcess, INFINITE); }
        DWORD code = 1; GetExitCodeProcess(process.hProcess, &code); CloseHandle(process.hProcess);
        return finished && code == 0;
#else
        std::vector<char*> argv;
        for (auto const& arg : args) argv.push_back(const_cast<char*>(arg.c_str()));
        argv.push_back(nullptr);
        pid_t child;
        if (posix_spawnp(&child, argv[0], nullptr, nullptr, argv.data(), environ)) return false;
        int status = 0;
        for (;;)
        {
            pid_t result = waitpid(child, &status, WNOHANG);
            if (result == child) return WIFEXITED(status) && WEXITSTATUS(status) == 0;
            if (result < 0 && errno != EINTR) return false;
            if (std::chrono::steady_clock::now() >= deadline)
            {
                kill(child, SIGKILL);
                while (waitpid(child, &status, 0) < 0 && errno == EINTR) { }
                return false;
            }
            report();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#endif
    }
}

bool PrepareMapData(std::set<std::uint32_t>& maps, std::string& root, std::string& error)
{
    if (!sConfigMgr->GetBoolDefault("MapData.Enable", false)) return true;
    try
    {
        auto config = std::filesystem::absolute(sConfigMgr->GetFilename());
        auto path = [&](char const* key, char const* fallback)
        {
            auto value = std::filesystem::u8path(sConfigMgr->GetStringDefault(key, fallback));
            return (value.is_absolute() ? value : config.parent_path() / value).lexically_normal();
        };
        std::istringstream assignments(sConfigMgr->GetStringDefault("MapData.Sources", ""));
        std::set<std::string> providers;
        std::string assignment;
        std::regex format("([A-Za-z0-9_.-]{1,64})=([0-9]{1,4}(,[0-9]{1,4}){0,511})");
        while (assignments >> assignment)
        {
            std::smatch match;
            if (!std::regex_match(assignment, match, format) || !providers.insert(match[1].str()).second)
                throw std::runtime_error("Invalid or duplicate MapData.Sources provider");
            std::istringstream values(match[2].str()); std::string value;
            while (std::getline(values, value, ','))
                if (!maps.insert(std::stoul(value)).second) throw std::runtime_error("Duplicate map assignment");
        }
        if (maps.empty() || maps.size() > 512 || providers.size() > 16)
            throw std::runtime_error("Configure 1..16 map providers and 1..512 maps");
        int timeout = sConfigMgr->GetIntDefault("MapData.StartupTimeout", 1800);
        if (timeout < 1 || timeout > 86400) throw std::runtime_error("Invalid map bootstrap timeout");
        auto python = std::filesystem::u8path(sConfigMgr->GetStringDefault("MapData.Python", "python3"));
        if (python.has_parent_path() && !python.is_absolute()) python = config.parent_path() / python;
        std::random_device random; std::ostringstream receiptName; receiptName << "receipt-" << std::hex << std::setfill('0');
        for (unsigned i = 0; i < 4; ++i) receiptName << std::setw(8) << std::uint32_t(random());
        SF_LOG_INFO("server.loading", "Preparing map data cache; unchanged verified assets will be reused unless full verification is configured.");
        if (!Run({Utf8(python), Utf8(path("MapData.Bootstrap", "mapserver/fetch_maps.py")),
            "--world-config", Utf8(config), "--receipt", receiptName.str()}, unsigned(timeout)))
            throw std::runtime_error("Map bootstrap failed or timed out; run fetch_maps.py manually for details");
        auto cache = path("MapData.CachePath", "map-cache");
        auto receipt = cache / receiptName.str();
        std::ifstream marker(receipt); std::string snapshot;
        if (!(marker >> snapshot) || !std::regex_match(snapshot, std::regex("[0-9a-f]{64}")))
            throw std::runtime_error("Missing or invalid map cache marker");
        if (sConfigMgr->GetBoolDefault("MapData.FullData", false))
        {
            std::string mode;
            if (!(marker >> mode) || mode != "full-data")
                throw std::runtime_error("Bootstrap did not verify a full data snapshot; update the Python helper");
        }
        marker.close(); std::filesystem::remove(receipt);
        auto selected = cache / snapshot;
        if (!std::filesystem::is_directory(selected) || std::filesystem::canonical(selected) != selected)
            throw std::runtime_error("Invalid map cache directory");
        root = Utf8(selected) + "/";
        SF_LOG_INFO("server.loading", "Map data cache ready for %u assigned maps.", unsigned(maps.size()));
        return true;
    }
    catch (std::exception const& exception) { error = exception.what(); return false; }
}
