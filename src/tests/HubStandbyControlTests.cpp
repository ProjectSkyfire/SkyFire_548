/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "Platform/HubProcessControl.h"
#include <cstring>
#include <iostream>
#include <stdexcept>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace
{
    struct Pipe
    {
        uint64 Read = 0, Write = 0;
        Pipe()
        {
#ifdef _WIN32
            HANDLE read, write;
            if (!CreatePipe(&read, &write, nullptr, 0)) throw std::runtime_error("CreatePipe failed");
            Read = uint64(reinterpret_cast<uintptr_t>(read));
            Write = uint64(reinterpret_cast<uintptr_t>(write));
#else
            int handles[2];
            if (pipe(handles)) throw std::runtime_error("pipe failed");
            Read = handles[0]; Write = handles[1];
#endif
        }
        static void Close(uint64& handle)
        {
            if (!handle) return;
#ifdef _WIN32
            CloseHandle(reinterpret_cast<HANDLE>(uintptr_t(handle)));
#else
            close(int(handle));
#endif
            handle = 0;
        }
        ~Pipe() { Close(Read); Close(Write); }
        void Send(char const* value)
        {
            size_t length = std::strlen(value);
#ifdef _WIN32
            DWORD sent = 0;
            if (!WriteFile(reinterpret_cast<HANDLE>(uintptr_t(Write)), value, DWORD(length), &sent, nullptr) || sent != length)
                throw std::runtime_error("WriteFile failed");
#else
            if (write(int(Write), value, length) != ssize_t(length)) throw std::runtime_error("write failed");
#endif
        }
    };
}

int main()
{
    using namespace Skyfire::HubControl;
    Pipe commands, statuses;
    ChildChannel child;
    std::string error;
    commands.Send(LaunchToken);
    if (!child.Initialize(commands.Read, statuses.Write, error)) return 1;
    commands.Read = statuses.Write = 0; // Owned by the child channel now.
    int failed = 0;
    auto expect = [&](bool value, char const* text) { if (!value) { ++failed; std::cerr << text << '\n'; } };
    bool activate = false;
    commands.Send("COMMAND server shutdown 0\nACCOUNT ignored\nACTIVATE_STAN");
    expect(!child.StopRequested(nullptr, nullptr, &activate) && !activate, "Partial activation must not activate standby.");
    commands.Send("DBY_1\n");
    expect(!child.StopRequested(nullptr, nullptr, &activate) && activate, "Complete activation must reach the standby gate.");
    activate = false;
    commands.Send(ActivateCommand);
    expect(!child.StopRequested(), "An active child must ignore standby activation.");
    expect(!child.StopRequested(nullptr, nullptr, &activate) && !activate, "Ignored activation must not leak into a later poll.");
    commands.Send("ACTIVATE_STANDBY_2\n");
    expect(!child.StopRequested(nullptr, nullptr, &activate) && !activate, "Unknown activation versions must be ignored.");
    commands.Send("ACTIVATE_STANDBY_1\nSTOP\n");
    expect(child.StopRequested(nullptr, nullptr, &activate), "Stop must win over activation in the same batch.");
    Pipe::Close(commands.Write);
    expect(child.StopRequested(nullptr, nullptr, &activate), "Lost supervision must stop standby instead of promoting it.");
    return failed ? 1 : 0;
}
