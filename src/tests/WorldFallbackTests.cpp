/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "Platform/WorldOwnershipLock.h"
#include "HubWorldFallbackPolicy.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#ifndef _WIN32
#include <sys/wait.h>
#endif

namespace
{
    int Child(std::filesystem::path const& executable, std::filesystem::path const& path, bool crash = false)
    {
#ifdef _WIN32
        std::wstring command = L"\"" + executable.wstring() + (crash ? L"\" --crash \"" : L"\" --try \"") + path.wstring() + L"\"";
        STARTUPINFOW startup{}; startup.cb = sizeof(startup);
        PROCESS_INFORMATION process{};
        if (!CreateProcessW(executable.c_str(), command.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &startup, &process)) return -1;
        DWORD code = 255;
        if (WaitForSingleObject(process.hProcess, 10000) == WAIT_OBJECT_0) GetExitCodeProcess(process.hProcess, &code);
        else { TerminateProcess(process.hProcess, 255); WaitForSingleObject(process.hProcess, INFINITE); }
        CloseHandle(process.hThread); CloseHandle(process.hProcess);
        return int(code);
#else
        auto pid = fork();
        if (pid < 0) return -1;
        if (!pid) { execl(executable.c_str(), executable.c_str(), crash ? "--crash" : "--try", path.c_str(), nullptr); std::_Exit(255); }
        int status = 0;
        if (waitpid(pid, &status, 0) != pid || !WIFEXITED(status)) return -1;
        return WEXITSTATUS(status);
#endif
    }
}

int main(int argc, char** argv)
{
    if (argc == 3)
    {
        WorldOwnershipLock lock;
        std::string error;
        bool acquired = lock.Acquire(argv[2], error);
        if (std::string(argv[1]) == "--crash") std::_Exit(acquired ? 0 : 10);
        return acquired ? 0 : 10;
    }
    int failed = 0;
    auto expect = [&](bool value, char const* message) { if (!value) { ++failed; std::cerr << message << '\n'; } };
    using namespace Skyfire::Fallback;
    expect(StartInStandby({true,false,false,false,true,true,false}), "A secondary must prepare without taking the primary's writer ownership.");
    expect(!StartInStandby({true,false,false,false,false,false,false}), "The initial primary must start active.");
    expect(StartInStandby({true,false,false,false,false,true,false}), "Starting the secondary first must not implicitly promote it.");
    expect(StartInStandby({true,false,false,false,true,false,true}), "The former primary must be able to return as standby.");
    expect(!StartInStandby({true,false,true,false,false,true,true}), "Restart/backup must preserve the promoted secondary's active role.");
    expect(StartInStandby({true,false,true,true,false,false,false}), "Restart/backup must preserve the primary's standby role.");
    expect(!StartInStandby({true,true,false,true,false,true,false}), "Explicit cold promotion must activate, not wait for another promotion.");
    expect(!StartInStandby({false,false,false,false,true,true,false}), "Cold mode must retain exclusive startup behavior.");
    ExitObservation good{true, true, true, false, false, false, 1};
    expect(MayAutomaticallyPromote(good), "Confirmed crash of a ready fenced owner should permit automatic promotion.");
    for (int scenario = 0; scenario < 8; ++scenario)
    {
        auto observation = good;
        switch (scenario)
        {
            case 0: observation.Confirmed = false; break; // Lost heartbeat is not exit.
            case 1: observation.PreviouslyReady = false; break; // Failed bootstrap.
            case 2: observation.OwnershipConfirmed = false; break; // Older binary.
            case 3: observation.Expected = true; break;
            case 4: observation.Maintenance = true; break;
            case 5: observation.ShuttingDown = true; break;
            case 6: observation.Code = 0; break;
            case 7: observation.Code = 2; break; // Soft restart must retain its node.
        }
        expect(!MayAutomaticallyPromote(observation), "Unsafe or intentional exit must not cause promotion.");
    }
    auto const executable = std::filesystem::absolute(argv[0]);
    DegradationWatch watch;
    expect(!watch.Observe(1000, true, 600, 500, 15), "One slow tick must not promote.");
    expect(!watch.Observe(10000, true, 600, 500, 15), "A short delay must not promote.");
    expect(!watch.Observe(11000, true, 100, 500, 15), "Recovery must reset the overload interval.");
    expect(!watch.Observe(12000, true, 600, 500, 15), "A new overload episode starts its own interval.");
    expect(watch.Observe(27000, true, 600, 500, 15), "Sustained fresh overload should request a graceful switchover.");
    expect(!watch.Observe(28000, true, 600, 500, 15), "Do not repeatedly request the same overload episode.");
    expect(!watch.Observe(29000, false, 600, 500, 15), "Stale metrics cannot authorize a preventive save.");
    expect(!watch.Observe(60000, true, 600, 500, 15), "Fresh reports must rebuild the interval after a stale gap.");
    expect(!watch.Observe(90000, true, 600, 0, 15), "A zero threshold disables preventive switching.");
    auto const directory = std::filesystem::temp_directory_path() / ("skyfire-fallback-" + std::to_string(
#ifdef _WIN32
        GetCurrentProcessId()
#else
        getpid()
#endif
    ));
    if (!std::filesystem::create_directory(directory)) return 1;
    auto const path = directory / "realm.lock";
    {
        WorldOwnershipLock owner;
        std::string error;
        expect(owner.Acquire(path.string(), error), "First world should acquire the local ownership lock.");
        expect(Child(executable, path) == 10, "A separate process must not acquire held realm ownership.");
        expect(Child(executable, directory / "other.lock") == 0, "A different realm lock should remain independent.");
    }
    expect(Child(executable, path) == 0, "Ownership must be available after graceful process cleanup.");
    expect(Child(executable, path, true) == 0, "Crash fixture must acquire ownership.");
    expect(Child(executable, path) == 0, "OS must release ownership after abnormal process termination.");
    expect(Child(executable, directory / "missing" / "realm.lock") == 10, "Inaccessible lock path must fail closed.");
    std::filesystem::remove_all(directory);
    return failed ? 1 : 0;
}
