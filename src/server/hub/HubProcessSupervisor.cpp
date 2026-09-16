/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "HubProcessSupervisor.h"

#include "Database/DatabaseEnv.h"
#include "Log.h"
#include "Platform/HubProcessControl.h"
#include "Platform/TimeUtils.h"

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <filesystem>
#include <sstream>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <csignal>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

namespace
{
    constexpr std::chrono::seconds StartupTimeout(30);
    constexpr std::chrono::seconds HeartbeatTimeout(15);

    bool WritePipe(uint64 handleValue, char const* message)
    {
        size_t const length = std::char_traits<char>::length(message);
        size_t consumed = 0;
        while (consumed < length)
        {
#ifdef _WIN32
            DWORD bytesWritten = 0;
            if (!WriteFile(reinterpret_cast<HANDLE>(uintptr_t(handleValue)), message + consumed,
                DWORD(length - consumed), &bytesWritten, nullptr) || bytesWritten == 0)
                return false;
#else
            ssize_t const bytesWritten = write(int(handleValue), message + consumed, length - consumed);
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

    std::filesystem::path ResolvePath(std::filesystem::path const& workingDirectory,
        std::string const& configuredPath)
    {
        std::filesystem::path path(configuredPath);
        if (path.is_relative())
            path = workingDirectory / path;
        return std::filesystem::absolute(path).lexically_normal();
    }
}

HubProcessSupervisor::HubProcessSupervisor()
    : _state(HubManagedProcessState::Stopped), _processId(0), _processHandle(0),
      _controlWriteHandle(0), _statusReadHandle(0), _lastExitCode(0), _ready(false)
{
}

HubProcessSupervisor::~HubProcessSupervisor()
{
    Stop();
}

bool HubProcessSupervisor::Start(std::string const& serviceKey, std::string& error)
{
    Update();
    if (IsActive())
    {
        error = "managed service '" + _serviceKey + "' is already " + GetStateName();
        return false;
    }

    auto const service = _services.find(serviceKey);
    if (service == _services.end())
    {
        error = "managed service '" + serviceKey + "' is not present in the database record cache";
        return false;
    }

    ManagedServiceDefinition const& definition = service->second;
    if (!definition.Enabled)
    {
        error = "managed service '" + serviceKey + "' is disabled";
        return false;
    }

    if (definition.ExecutablePath.empty())
    {
        error = "managed service '" + serviceKey + "' has no executable path";
        return false;
    }

    try
    {
        std::filesystem::path workingDirectory = definition.WorkingDirectory.empty()
            ? std::filesystem::current_path()
            : std::filesystem::path(definition.WorkingDirectory);
        if (workingDirectory.is_relative())
            workingDirectory = std::filesystem::absolute(workingDirectory);
        workingDirectory = workingDirectory.lexically_normal();

        std::filesystem::path executablePath = ResolvePath(workingDirectory, definition.ExecutablePath);
#ifdef _WIN32
        if (!std::filesystem::exists(executablePath) && executablePath.extension().empty())
            executablePath += ".exe";
#endif
        if (!std::filesystem::is_regular_file(executablePath))
        {
            error = "managed service executable does not exist: " + executablePath.string();
            return false;
        }

        std::filesystem::path configPath = ResolvePath(workingDirectory, definition.ConfigPath);
        if (!std::filesystem::is_regular_file(configPath))
        {
            error = "managed service configuration does not exist: " + configPath.string();
            return false;
        }

        if (!std::filesystem::is_directory(workingDirectory))
        {
            error = "managed service working directory does not exist: " + workingDirectory.string();
            return false;
        }

        _serviceKey = serviceKey;
        if (!Launch(executablePath.string(), configPath.string(), workingDirectory.string(), error))
        {
            _serviceKey.clear();
            return false;
        }
    }
    catch (std::filesystem::filesystem_error const& exception)
    {
        error = exception.what();
        return false;
    }

    _state = HubManagedProcessState::Starting;
    _ready = false;
    _lastExitCode = 0;
    _statusBuffer.clear();
    _startedAt = std::chrono::steady_clock::now();
    _lastHeartbeat = _startedAt;
    SF_LOG_INFO("server.hub", "Started managed service '%s' as process %llu.", _serviceKey.c_str(),
        static_cast<unsigned long long>(_processId));
    return true;
}

bool HubProcessSupervisor::ReloadDatabaseRecords(std::string& error)
{
    PreparedQueryResult result = HubDatabase.Query(
        HubDatabase.GetPreparedStatement(HUB_SEL_MANAGED_SERVICES));

    std::unordered_map<std::string, ManagedServiceDefinition> services;
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            std::string const serviceKey = fields[0].GetString();
            if (serviceKey.empty())
            {
                error = "hub_managed_services contains an empty service key";
                return false;
            }

            ManagedServiceDefinition definition;
            definition.Name = fields[1].GetString();
            definition.ExecutablePath = fields[2].GetString();
            definition.ConfigPath = fields[3].GetString();
            definition.WorkingDirectory = fields[4].GetString();
            definition.Enabled = fields[5].GetBool();
            services.emplace(serviceKey, std::move(definition));
        } while (result->NextRow());
    }

    _services.swap(services);
    SF_LOG_INFO("server.hub", "Cached %u managed service database record(s).", uint32(_services.size()));
    return true;
}

bool HubProcessSupervisor::Launch(std::string const& executablePath, std::string const& configPath,
    std::string const& workingDirectory, std::string& error)
{
#ifdef _WIN32
    SECURITY_ATTRIBUTES security = { sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };
    HANDLE childControlRead = nullptr;
    HANDLE parentControlWrite = nullptr;
    HANDLE parentStatusRead = nullptr;
    HANDLE childStatusWrite = nullptr;

    auto closePipeHandles = [&]()
    {
        if (childControlRead)
            CloseHandle(childControlRead);
        if (parentControlWrite)
            CloseHandle(parentControlWrite);
        if (parentStatusRead)
            CloseHandle(parentStatusRead);
        if (childStatusWrite)
            CloseHandle(childStatusWrite);
    };

    if (!CreatePipe(&childControlRead, &parentControlWrite, &security, 0) ||
        !CreatePipe(&parentStatusRead, &childStatusWrite, &security, 0) ||
        !SetHandleInformation(parentControlWrite, HANDLE_FLAG_INHERIT, 0) ||
        !SetHandleInformation(parentStatusRead, HANDLE_FLAG_INHERIT, 0))
    {
        error = "unable to create authnet supervision pipes (Windows error " +
            std::to_string(GetLastError()) + ')';
        closePipeHandles();
        return false;
    }

    std::filesystem::path const executable(executablePath);
    std::filesystem::path const config(configPath);
    std::filesystem::path const working(workingDirectory);
    std::wostringstream command;
    command << L'\"' << executable.wstring() << L"\" -c \"" << config.wstring()
        << L"\" --hub-control-read " << reinterpret_cast<uintptr_t>(childControlRead)
        << L" --hub-status-write " << reinterpret_cast<uintptr_t>(childStatusWrite);
    std::wstring commandText = command.str();
    std::vector<wchar_t> commandBuffer(commandText.begin(), commandText.end());
    commandBuffer.push_back(L'\0');

    STARTUPINFOW startupInfo = { };
    startupInfo.cb = sizeof(startupInfo);
    PROCESS_INFORMATION processInfo = { };
    if (!CreateProcessW(executable.wstring().c_str(), commandBuffer.data(), nullptr, nullptr, TRUE,
        CREATE_NEW_CONSOLE, nullptr, working.wstring().c_str(), &startupInfo, &processInfo))
    {
        error = "unable to start authnet process (Windows error " + std::to_string(GetLastError()) + ')';
        closePipeHandles();
        return false;
    }

    CloseHandle(processInfo.hThread);
    CloseHandle(childControlRead);
    childControlRead = nullptr;
    CloseHandle(childStatusWrite);
    childStatusWrite = nullptr;

    _processId = processInfo.dwProcessId;
    _processHandle = uint64(reinterpret_cast<uintptr_t>(processInfo.hProcess));
    _controlWriteHandle = uint64(reinterpret_cast<uintptr_t>(parentControlWrite));
    _statusReadHandle = uint64(reinterpret_cast<uintptr_t>(parentStatusRead));
    parentControlWrite = nullptr;
    parentStatusRead = nullptr;
#else
    int controlPipe[2] = { -1, -1 };
    int statusPipe[2] = { -1, -1 };
    if (pipe(controlPipe) != 0 || pipe(statusPipe) != 0)
    {
        error = "unable to create authnet supervision pipes";
        if (controlPipe[0] >= 0)
            close(controlPipe[0]);
        if (controlPipe[1] >= 0)
            close(controlPipe[1]);
        if (statusPipe[0] >= 0)
            close(statusPipe[0]);
        if (statusPipe[1] >= 0)
            close(statusPipe[1]);
        return false;
    }

    pid_t const child = fork();
    if (child < 0)
    {
        error = "unable to fork authnet process";
        close(controlPipe[0]);
        close(controlPipe[1]);
        close(statusPipe[0]);
        close(statusPipe[1]);
        return false;
    }

    if (child == 0)
    {
        close(controlPipe[1]);
        close(statusPipe[0]);
        if (chdir(workingDirectory.c_str()) != 0)
            _exit(126);

        std::string const controlHandle = std::to_string(controlPipe[0]);
        std::string const statusHandle = std::to_string(statusPipe[1]);
        execl(executablePath.c_str(), executablePath.c_str(), "-c", configPath.c_str(),
            "--hub-control-read", controlHandle.c_str(), "--hub-status-write", statusHandle.c_str(),
            static_cast<char*>(nullptr));
        _exit(127);
    }

    close(controlPipe[0]);
    close(statusPipe[1]);
    int const flags = fcntl(statusPipe[0], F_GETFL, 0);
    if (flags >= 0)
        (void)fcntl(statusPipe[0], F_SETFL, flags | O_NONBLOCK);

    _processId = uint64(child);
    _processHandle = 0;
    _controlWriteHandle = uint64(controlPipe[1]);
    _statusReadHandle = uint64(statusPipe[0]);
#endif

    if (!WritePipe(_controlWriteHandle, Skyfire::HubControl::LaunchToken))
    {
        error = "unable to authorize the authnet child process";
#ifdef _WIN32
        TerminateProcess(reinterpret_cast<HANDLE>(uintptr_t(_processHandle)), 1);
#else
        kill(pid_t(_processId), SIGKILL);
        (void)waitpid(pid_t(_processId), nullptr, 0);
#endif
        CloseHandles();
        _processId = 0;
        return false;
    }

    return true;
}

void HubProcessSupervisor::Update()
{
    if (!IsActive())
        return;

#ifdef _WIN32
    DWORD exitCode = STILL_ACTIVE;
    if (!GetExitCodeProcess(reinterpret_cast<HANDLE>(uintptr_t(_processHandle)), &exitCode) ||
        exitCode != STILL_ACTIVE)
    {
        MarkExited(exitCode == STILL_ACTIVE ? -1 : int64(exitCode));
        return;
    }
#else
    int status = 0;
    pid_t const result = waitpid(pid_t(_processId), &status, WNOHANG);
    if (result == pid_t(_processId))
    {
        int64 exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status);
        MarkExited(exitCode);
        return;
    }
#endif

    ReadStatusMessages();

    auto const now = std::chrono::steady_clock::now();
    if (_state == HubManagedProcessState::Starting && now - _startedAt > StartupTimeout)
        _state = HubManagedProcessState::Unresponsive;
    else if (_ready && now - _lastHeartbeat > HeartbeatTimeout &&
        _state != HubManagedProcessState::Stopping)
        _state = HubManagedProcessState::Unresponsive;
}

void HubProcessSupervisor::ReadStatusMessages()
{
    if (!_statusReadHandle)
        return;

    char buffer[512];
#ifdef _WIN32
    while (true)
    {
        DWORD available = 0;
        if (!PeekNamedPipe(reinterpret_cast<HANDLE>(uintptr_t(_statusReadHandle)), nullptr, 0,
            nullptr, &available, nullptr) || !available)
            break;

        DWORD bytesRead = 0;
        DWORD const requested = std::min<DWORD>(available, DWORD(sizeof(buffer)));
        if (!ReadFile(reinterpret_cast<HANDLE>(uintptr_t(_statusReadHandle)), buffer, requested,
            &bytesRead, nullptr) || bytesRead == 0)
            break;
        _statusBuffer.append(buffer, bytesRead);
    }
#else
    while (true)
    {
        ssize_t const bytesRead = read(int(_statusReadHandle), buffer, sizeof(buffer));
        if (bytesRead > 0)
        {
            _statusBuffer.append(buffer, size_t(bytesRead));
            continue;
        }
        if (bytesRead < 0 && errno == EINTR)
            continue;
        break;
    }
#endif

    size_t lineEnd = 0;
    while ((lineEnd = _statusBuffer.find('\n')) != std::string::npos)
    {
        std::string const message = _statusBuffer.substr(0, lineEnd);
        _statusBuffer.erase(0, lineEnd + 1);
        ProcessStatusMessage(message);
    }
}

void HubProcessSupervisor::ProcessStatusMessage(std::string const& message)
{
    auto const now = std::chrono::steady_clock::now();
    if (message == Skyfire::HubControl::StartingMessage)
    {
        _state = HubManagedProcessState::Starting;
        _lastHeartbeat = now;
    }
    else if (message == Skyfire::HubControl::ReadyMessage)
    {
        _ready = true;
        _state = HubManagedProcessState::Running;
        _lastHeartbeat = now;
        SF_LOG_INFO("server.hub", "Managed service '%s' reported ready.", _serviceKey.c_str());
    }
    else if (message == Skyfire::HubControl::HeartbeatMessage)
    {
        _lastHeartbeat = now;
        if (_ready && _state != HubManagedProcessState::Stopping)
            _state = HubManagedProcessState::Running;
    }
    else if (message == Skyfire::HubControl::StoppingMessage)
        _state = HubManagedProcessState::Stopping;
}

void HubProcessSupervisor::Stop()
{
    Update();
    if (!IsActive())
        return;

    _state = HubManagedProcessState::Stopping;
    (void)WriteControl(Skyfire::HubControl::StopCommand);

#ifdef _WIN32
    HANDLE const process = reinterpret_cast<HANDLE>(uintptr_t(_processHandle));
    DWORD waitResult = WaitForSingleObject(process, 10000);
    if (waitResult == WAIT_TIMEOUT)
    {
        SF_LOG_ERROR("server.hub", "Managed service '%s' did not stop; terminating it.", _serviceKey.c_str());
        TerminateProcess(process, 1);
        (void)WaitForSingleObject(process, 5000);
    }

    DWORD exitCode = 1;
    (void)GetExitCodeProcess(process, &exitCode);
    MarkExited(exitCode == STILL_ACTIVE ? -1 : int64(exitCode));
#else
    for (uint32 attempt = 0; attempt < 100; ++attempt)
    {
        int status = 0;
        pid_t const result = waitpid(pid_t(_processId), &status, WNOHANG);
        if (result == pid_t(_processId))
        {
            MarkExited(WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status));
            return;
        }
        Skyfire::SleepForMilliseconds(100);
    }

    SF_LOG_ERROR("server.hub", "Managed service '%s' did not stop; terminating it.", _serviceKey.c_str());
    kill(pid_t(_processId), SIGKILL);
    int status = 0;
    (void)waitpid(pid_t(_processId), &status, 0);
    MarkExited(WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status));
#endif
}

bool HubProcessSupervisor::WriteControl(char const* message)
{
    return _controlWriteHandle && WritePipe(_controlWriteHandle, message);
}

bool HubProcessSupervisor::IsActive() const
{
    return _state == HubManagedProcessState::Starting || _state == HubManagedProcessState::Running ||
        _state == HubManagedProcessState::Unresponsive || _state == HubManagedProcessState::Stopping;
}

char const* HubProcessSupervisor::GetStateName() const
{
    switch (_state)
    {
        case HubManagedProcessState::Stopped: return "stopped";
        case HubManagedProcessState::Starting: return "starting";
        case HubManagedProcessState::Running: return "running";
        case HubManagedProcessState::Unresponsive: return "unresponsive";
        case HubManagedProcessState::Stopping: return "stopping";
        case HubManagedProcessState::Exited: return "exited";
        default: return "unknown";
    }
}

void HubProcessSupervisor::MarkExited(int64 exitCode)
{
    SF_LOG_INFO("server.hub", "Managed service '%s' process %llu exited with code %lld.",
        _serviceKey.c_str(), static_cast<unsigned long long>(_processId), static_cast<long long>(exitCode));
    _lastExitCode = exitCode;
    _state = HubManagedProcessState::Exited;
    _ready = false;
    CloseHandles();
}

void HubProcessSupervisor::CloseHandles()
{
#ifdef _WIN32
    if (_processHandle)
        CloseHandle(reinterpret_cast<HANDLE>(uintptr_t(_processHandle)));
    if (_controlWriteHandle)
        CloseHandle(reinterpret_cast<HANDLE>(uintptr_t(_controlWriteHandle)));
    if (_statusReadHandle)
        CloseHandle(reinterpret_cast<HANDLE>(uintptr_t(_statusReadHandle)));
#else
    if (_controlWriteHandle)
        close(int(_controlWriteHandle));
    if (_statusReadHandle)
        close(int(_statusReadHandle));
#endif
    _processHandle = 0;
    _controlWriteHandle = 0;
    _statusReadHandle = 0;
    _statusBuffer.clear();
}
