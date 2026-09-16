/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "HubProcessControl.h"

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iterator>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif

namespace
{
    bool ReadExact(uint64 handleValue, char* buffer, size_t length)
    {
        size_t consumed = 0;
        while (consumed < length)
        {
#ifdef _WIN32
            DWORD bytesRead = 0;
            if (!ReadFile(reinterpret_cast<HANDLE>(uintptr_t(handleValue)), buffer + consumed,
                DWORD(length - consumed), &bytesRead, nullptr) || bytesRead == 0)
                return false;
#else
            ssize_t const bytesRead = read(int(handleValue), buffer + consumed, length - consumed);
            if (bytesRead < 0)
            {
                if (errno == EINTR)
                    continue;
                return false;
            }

            if (bytesRead == 0)
                return false;
#endif
            consumed += size_t(bytesRead);
        }

        return true;
    }

    bool WriteAll(uint64 handleValue, char const* buffer, size_t length)
    {
        size_t consumed = 0;
        while (consumed < length)
        {
#ifdef _WIN32
            DWORD bytesWritten = 0;
            if (!WriteFile(reinterpret_cast<HANDLE>(uintptr_t(handleValue)), buffer + consumed,
                DWORD(length - consumed), &bytesWritten, nullptr) || bytesWritten == 0)
                return false;
#else
            ssize_t const bytesWritten = write(int(handleValue), buffer + consumed, length - consumed);
            if (bytesWritten < 0)
            {
                if (errno == EINTR)
                    continue;
                return false;
            }
#endif
            consumed += size_t(bytesWritten);
        }

        return true;
    }
}

Skyfire::HubControl::ChildChannel::ChildChannel()
    : _controlReadHandle(0), _statusWriteHandle(0)
{
}

Skyfire::HubControl::ChildChannel::~ChildChannel()
{
    Close();
}

bool Skyfire::HubControl::ChildChannel::Initialize(uint64 controlReadHandle, uint64 statusWriteHandle,
    std::string& error)
{
    if (!controlReadHandle || !statusWriteHandle)
    {
        error = "missing inherited control handles";
        return false;
    }

    char token[sizeof(LaunchToken) - 1] = { };
    if (!ReadExact(controlReadHandle, token, sizeof(token)) ||
        !std::equal(std::begin(token), std::end(token), std::begin(LaunchToken)))
    {
        error = "invalid hub startup authorization";
        return false;
    }

#ifndef _WIN32
    int const flags = fcntl(int(controlReadHandle), F_GETFL, 0);
    if (flags < 0 || fcntl(int(controlReadHandle), F_SETFL, flags | O_NONBLOCK) < 0)
    {
        error = std::string("unable to configure the hub control channel: ") + std::strerror(errno);
        return false;
    }
#endif

    _controlReadHandle = controlReadHandle;
    _statusWriteHandle = statusWriteHandle;
    return true;
}

bool Skyfire::HubControl::ChildChannel::SendStatus(char const* status) const
{
    if (!_statusWriteHandle)
        return false;

    std::string message(status);
    message.push_back('\n');
    return WriteAll(_statusWriteHandle, message.data(), message.size());
}

bool Skyfire::HubControl::ChildChannel::StopRequested()
{
    if (!_controlReadHandle)
        return true;

    char buffer[256];
#ifdef _WIN32
    DWORD available = 0;
    if (!PeekNamedPipe(reinterpret_cast<HANDLE>(uintptr_t(_controlReadHandle)), nullptr, 0, nullptr,
        &available, nullptr))
        return true;

    if (available)
    {
        DWORD bytesRead = 0;
        DWORD const requested = std::min<DWORD>(available, DWORD(sizeof(buffer)));
        if (!ReadFile(reinterpret_cast<HANDLE>(uintptr_t(_controlReadHandle)), buffer, requested,
            &bytesRead, nullptr) || bytesRead == 0)
            return true;
        _controlBuffer.append(buffer, bytesRead);
    }
#else
    while (true)
    {
        ssize_t const bytesRead = read(int(_controlReadHandle), buffer, sizeof(buffer));
        if (bytesRead > 0)
        {
            _controlBuffer.append(buffer, size_t(bytesRead));
            continue;
        }

        if (bytesRead == 0)
            return true;

        if (errno == EINTR)
            continue;
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            return true;
        break;
    }
#endif

    size_t lineEnd = 0;
    while ((lineEnd = _controlBuffer.find('\n')) != std::string::npos)
    {
        std::string const command = _controlBuffer.substr(0, lineEnd);
        _controlBuffer.erase(0, lineEnd + 1);
        if (command == "STOP")
            return true;
    }

    return false;
}

void Skyfire::HubControl::ChildChannel::Close()
{
#ifdef _WIN32
    if (_controlReadHandle)
        CloseHandle(reinterpret_cast<HANDLE>(uintptr_t(_controlReadHandle)));
    if (_statusWriteHandle)
        CloseHandle(reinterpret_cast<HANDLE>(uintptr_t(_statusWriteHandle)));
#else
    if (_controlReadHandle)
        close(int(_controlReadHandle));
    if (_statusWriteHandle)
        close(int(_statusWriteHandle));
#endif
    _controlReadHandle = 0;
    _statusWriteHandle = 0;
    _controlBuffer.clear();
}
