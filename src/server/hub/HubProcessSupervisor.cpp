/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "HubBackupGuard.h"

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
    constexpr std::chrono::seconds ShutdownTimeout(10);

    bool WritePipe(uint64 handleValue, char const* message)
    {
        size_t const length = std::char_traits<char>::length(message);
        size_t consumed = 0;
        while (consumed < length)
        {
#ifdef _WIN32
            DWORD bytesWritten = 0;
            if (!WriteFile(reinterpret_cast<HANDLE>(uintptr_t(handleValue)), message + consumed,
                DWORD(length - consumed), &bytesWritten, nullptr) || !bytesWritten)
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

bool HubProcessSupervisor::IsWorldKey(std::string const& key)
{
    return key == "world" || (key.size() > 6 && key.compare(0, 6, "world-") == 0);
}

bool HubProcessSupervisor::HasActiveWorld() const
{
    return std::any_of(_services.begin(), _services.end(), [](auto const& service)
        { return IsWorldKey(service.first) && IsActive(service.second); });
}

bool HubProcessSupervisor::SaveWorldNode(std::string const& key, std::string const& name,
    std::string const& executable, std::string const& config, std::string const& directory, std::string& error)
{
    auto valid = [](std::string const& value, size_t max)
    {
        return !value.empty() && value.size() <= max && value.find_first_not_of(" ") != std::string::npos &&
            std::none_of(value.begin(), value.end(), [](unsigned char c) { return c < 32 || c == 127; });
    };
    if (!IsWorldKey(key) || key.size() > 64 || key.find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789-_") != std::string::npos ||
        !valid(name, 100) || !valid(executable, 1024) || !valid(config, 1024) || !valid(directory, 1024))
    {
        error = "Use world or world-<id> as the key and provide a name and valid paths.";
        return false;
    }
    auto found = _services.find(key);
    if (found != _services.end() && IsActive(found->second) &&
        (executable != found->second.Definition.ExecutablePath || config != found->second.Definition.ConfigPath ||
            directory != found->second.Definition.WorkingDirectory))
    {
        error = "Stop this world gracefully before changing its launch paths.";
        return false;
    }
    PreparedStatement* statement = HubDatabase.GetPreparedStatement(HUB_UPSERT_WORLD_SERVICE);
    statement->setString(0, key); statement->setString(1, name); statement->setString(2, executable);
    statement->setString(3, config); statement->setString(4, directory);
    HubDatabase.DirectExecute(statement);
    if (!ReloadDatabaseRecords(error)) return false;
    found = _services.find(key);
    if (found == _services.end() || found->second.Definition.Name != name ||
        found->second.Definition.ExecutablePath != executable || found->second.Definition.ConfigPath != config ||
        found->second.Definition.WorkingDirectory != directory)
    {
        error = "The world node could not be saved to the hub database.";
        return false;
    }
    return true;
}

HubProcessSupervisor::~HubProcessSupervisor()
{
    StopAll();
}

bool HubProcessSupervisor::Start(std::string const& serviceKey, std::string& error)
{
    Update();
    std::lock_guard<std::mutex> backupLock(HubBackupAdmission);
    if (HubBackupMaintenance() && !_backupLaunching) { error = "Backup recovery maintenance blocks service starts."; return false; }
    HubDatabase.DirectExecute("UPDATE hub_backup_worker SET services_stopped=0 WHERE id=1");
    auto service = _services.find(serviceKey);
    if (service == _services.end())
    {
        error = "managed service '" + serviceKey + "' is not present in the database record cache";
        return false;
    }

    ManagedServiceRuntime& runtime = service->second;
    if (IsActive(runtime))
    {
        error = "managed service '" + serviceKey + "' is already " + GetStateName(runtime.State);
        return false;
    }
    if (!runtime.Definition.Enabled)
    {
        error = "managed service '" + serviceKey + "' is disabled";
        return false;
    }
    if (runtime.Definition.ExecutablePath.empty())
    {
        error = "managed service '" + serviceKey + "' has no executable path";
        return false;
    }

    try
    {
        std::filesystem::path workingDirectory = runtime.Definition.WorkingDirectory.empty()
            ? std::filesystem::current_path() : std::filesystem::path(runtime.Definition.WorkingDirectory);
        if (workingDirectory.is_relative())
            workingDirectory = std::filesystem::absolute(workingDirectory);
        workingDirectory = workingDirectory.lexically_normal();

        std::filesystem::path executablePath = ResolvePath(workingDirectory, runtime.Definition.ExecutablePath);
#ifdef _WIN32
        if (!std::filesystem::exists(executablePath) && executablePath.extension().empty())
            executablePath += ".exe";
#endif
        if (!std::filesystem::is_regular_file(executablePath))
        {
            error = "managed service executable does not exist: " + executablePath.string();
            return false;
        }
        std::filesystem::path configPath = ResolvePath(workingDirectory, runtime.Definition.ConfigPath);
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
        if (IsWorldKey(serviceKey))
            for (auto const& other : _services)
                if (other.first != serviceKey && IsWorldKey(other.first) && IsActive(other.second))
                {
                    auto const& definition = other.second.Definition;
                    auto const otherDirectory = definition.WorkingDirectory.empty()
                        ? std::filesystem::current_path() : std::filesystem::path(definition.WorkingDirectory);
                    if (std::filesystem::equivalent(configPath, ResolvePath(otherDirectory, definition.ConfigPath)))
                    {
                        error = "This configuration is already used by active world node '" + other.first + "'.";
                        return false;
                    }
                }
        if (IsWorldKey(serviceKey) && (!_worldStartCheck || !_worldStartCheck(configPath.string(), error)))
        {
            if (error.empty()) error = "World start blocked: dependency checker is unavailable.";
            runtime.CommandResult = error;
            SF_LOG_ERROR("server.hub", "%s", error.c_str());
            return false;
        }
        if (!Launch(serviceKey, runtime, executablePath.string(), configPath.string(),
            workingDirectory.string(), error))
            return false;
    }
    catch (std::filesystem::filesystem_error const& exception)
    {
        error = exception.what();
        return false;
    }

    runtime.State = HubManagedProcessState::Starting;
    runtime.Ready = false;
    runtime.CanSendCommands = false;
    runtime.CanManageAccounts = false;
    runtime.CommandPending = false;
    runtime.SuppressRestart = false;
    runtime.BackupControlled = false;
    runtime.RestartPending = false;
    runtime.LastExitCode = 0;
    runtime.MetricsAvailable = false;
    runtime.LastTick = 0;
    runtime.StatusBuffer.clear();
    runtime.StartedAt = std::chrono::steady_clock::now();
    runtime.LastHeartbeat = runtime.StartedAt;
    SF_LOG_INFO("server.hub", "Started managed service '%s' as process %llu.", serviceKey.c_str(),
        static_cast<unsigned long long>(runtime.ProcessId));
    return true;
}

bool HubProcessSupervisor::Stop(std::string const& serviceKey, std::string& error)
{
    if (HubBackupMaintenance()) { error = "Backup maintenance blocks world launch configuration changes."; return false; }
    Update();
    auto service = _services.find(serviceKey);
    if (service == _services.end())
    {
        error = "managed service '" + serviceKey + "' is not present in the database record cache";
        return false;
    }
    ManagedServiceRuntime& runtime = service->second;
    if (!IsActive(runtime))
    {
        error = "managed service '" + serviceKey + "' is not running";
        return false;
    }
    if (runtime.State == HubManagedProcessState::Stopping)
    {
        error = "managed service '" + serviceKey + "' is already stopping";
        return false;
    }
    if (!WriteControl(runtime, Skyfire::HubControl::StopCommand))
    {
        error = "unable to send the stop command to managed service '" + serviceKey + "'";
        return false;
    }
    runtime.SuppressRestart = true;
    runtime.State = HubManagedProcessState::Stopping;
    runtime.StopRequestedAt = std::chrono::steady_clock::now();
    SF_LOG_INFO("server.hub", "Stop requested for managed service '%s'.", serviceKey.c_str());
    return true;
}

bool HubProcessSupervisor::SendWorldCommand(std::string command, std::string& error, std::string const& key)
{
    if (!_backupInternalCommand && HubBackupMaintenance()) { error = "Backup maintenance owns the world shutdown countdown."; return false; }
    size_t const first = command.find_first_not_of(" ");
    if (first != std::string::npos)
        command.erase(0, first);
    if (!command.empty() && command.front() == '.')
        command.erase(0, 1);
    if (command.empty() || command.find_first_not_of(" ") == std::string::npos ||
        command.size() > Skyfire::HubControl::MaxCommandLength ||
        std::any_of(command.begin(), command.end(), [](unsigned char c) { return c < 32 || c == 127; }))
    {
        error = "enter one command, up to 1024 bytes, without control characters";
        return false;
    }
    auto service = _services.find(key);
    if (!IsWorldKey(key) || service == _services.end() || !service->second.Definition.Enabled ||
        service->second.State != HubManagedProcessState::Running || !service->second.CanSendCommands)
    {
        error = "worldserver must be running under the hub with command support";
        return false;
    }
    ManagedServiceRuntime& runtime = service->second;
    if (runtime.CommandPending)
    {
        error = "a world command is still awaiting a response";
        return false;
    }
    if (!WriteControl(runtime, ("COMMAND " + command + '\n').c_str()))
    {
        error = "unable to send the world command";
        return false;
    }
    runtime.CommandPending = true;
    runtime.CommandResult = "Waiting for worldserver response...";
    return true;
}

bool HubProcessSupervisor::SendAccountRequest(std::string const& request,
    std::function<void(std::string const&)> callback, std::string& error)
{
    if (std::any_of(_services.begin(), _services.end(), [](auto const& service)
        { return bool(service.second.AccountCallback); }))
    {
        error = "An account change is already pending. Wait for its result before retrying.";
        return false;
    }
    auto world = std::find_if(_services.begin(), _services.end(), [](auto const& service)
    {
        return IsWorldKey(service.first) && service.second.State == HubManagedProcessState::Running &&
            service.second.CanManageAccounts && !service.second.CommandPending;
    });
    if (world == _services.end() || world->second.State != HubManagedProcessState::Running ||
        !world->second.CanManageAccounts || world->second.CommandPending)
    {
        error = "Worldserver is busy or lacks account administration support. No direct database fallback was attempted.";
        return false;
    }
    if (request.empty() || request.size() > 8192 || request.find_first_not_of("0123456789abcdef") != std::string::npos)
    {
        error = "Invalid account request.";
        return false;
    }
    if (!WriteControl(world->second, ("ACCOUNT " + request + '\n').c_str()))
    {
        error = "Worldserver control channel failed. Check the account state before retrying.";
        return false;
    }
    world->second.CommandPending = true;
    world->second.AccountCallback = std::move(callback);
    return true;
}

bool HubProcessSupervisor::ReloadDatabaseRecords(std::string& error)
{
    if (!_backupCycle.empty() && HubBackupMaintenance()) { error = "Backup maintenance blocks managed-service reloads."; return false; }
    PreparedQueryResult result = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_MANAGED_SERVICES));
    std::unordered_map<std::string, ManagedServiceDefinition> definitions;
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
            std::string const key = fields[0].GetString();
            if (key.empty())
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
            definitions.emplace(key, std::move(definition));
        } while (result->NextRow());
    }

    for (auto& definition : definitions)
        _services[definition.first].Definition = std::move(definition.second);
    for (auto service = _services.begin(); service != _services.end();)
    {
        if (definitions.find(service->first) == definitions.end() && !IsActive(service->second))
            service = _services.erase(service);
        else
            ++service;
    }
    SF_LOG_INFO("server.hub", "Cached %u managed service database record(s).", uint32(definitions.size()));
    return true;
}

bool HubProcessSupervisor::Launch(std::string const& serviceKey, ManagedServiceRuntime& runtime,
    std::string const& executablePath, std::string const& configPath,
    std::string const& workingDirectory, std::string& error)
{
#ifdef _WIN32
    SECURITY_ATTRIBUTES security = { sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };
    HANDLE childControlRead = nullptr, parentControlWrite = nullptr;
    HANDLE parentStatusRead = nullptr, childStatusWrite = nullptr;
    auto closePipes = [&]()
    {
        if (childControlRead) CloseHandle(childControlRead);
        if (parentControlWrite) CloseHandle(parentControlWrite);
        if (parentStatusRead) CloseHandle(parentStatusRead);
        if (childStatusWrite) CloseHandle(childStatusWrite);
    };
    if (!CreatePipe(&childControlRead, &parentControlWrite, &security, 0) ||
        !CreatePipe(&parentStatusRead, &childStatusWrite, &security, 0) ||
        !SetHandleInformation(parentControlWrite, HANDLE_FLAG_INHERIT, 0) ||
        !SetHandleInformation(parentStatusRead, HANDLE_FLAG_INHERIT, 0))
    {
        error = "unable to create supervision pipes (Windows error " + std::to_string(GetLastError()) + ')';
        closePipes();
        return false;
    }
    std::filesystem::path const executable(executablePath), config(configPath), working(workingDirectory);
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
        error = "unable to start managed service '" + serviceKey + "' (Windows error " +
            std::to_string(GetLastError()) + ')';
        closePipes();
        return false;
    }
    CloseHandle(processInfo.hThread);
    CloseHandle(childControlRead);
    CloseHandle(childStatusWrite);
    runtime.ProcessId = processInfo.dwProcessId;
    runtime.ProcessHandle = uint64(reinterpret_cast<uintptr_t>(processInfo.hProcess));
    runtime.ControlWriteHandle = uint64(reinterpret_cast<uintptr_t>(parentControlWrite));
    runtime.StatusReadHandle = uint64(reinterpret_cast<uintptr_t>(parentStatusRead));
#else
    int controlPipe[2] = { -1, -1 }, statusPipe[2] = { -1, -1 };
    if (pipe(controlPipe) != 0 || pipe(statusPipe) != 0)
    {
        error = "unable to create supervision pipes";
        if (controlPipe[0] >= 0) close(controlPipe[0]);
        if (controlPipe[1] >= 0) close(controlPipe[1]);
        if (statusPipe[0] >= 0) close(statusPipe[0]);
        if (statusPipe[1] >= 0) close(statusPipe[1]);
        return false;
    }
    pid_t const child = fork();
    if (child < 0)
    {
        error = "unable to fork managed service '" + serviceKey + "'";
        close(controlPipe[0]); close(controlPipe[1]); close(statusPipe[0]); close(statusPipe[1]);
        return false;
    }
    if (child == 0)
    {
        close(controlPipe[1]); close(statusPipe[0]);
        if (chdir(workingDirectory.c_str()) != 0) _exit(126);
        std::string const controlHandle = std::to_string(controlPipe[0]);
        std::string const statusHandle = std::to_string(statusPipe[1]);
        execl(executablePath.c_str(), executablePath.c_str(), "-c", configPath.c_str(),
            "--hub-control-read", controlHandle.c_str(), "--hub-status-write", statusHandle.c_str(),
            static_cast<char*>(nullptr));
        _exit(127);
    }
    close(controlPipe[0]); close(statusPipe[1]);
    int const flags = fcntl(statusPipe[0], F_GETFL, 0);
    if (flags >= 0) (void)fcntl(statusPipe[0], F_SETFL, flags | O_NONBLOCK);
    runtime.ProcessId = uint64(child);
    runtime.ControlWriteHandle = uint64(controlPipe[1]);
    runtime.StatusReadHandle = uint64(statusPipe[0]);
#endif
    if (!WritePipe(runtime.ControlWriteHandle, Skyfire::HubControl::LaunchToken))
    {
        error = "unable to authorize managed service '" + serviceKey + "'";
        ForceStop(serviceKey, runtime);
        return false;
    }
    return true;
}

void HubProcessSupervisor::Update()
{
    for (auto& service : _services)
        Update(service.first, service.second);
    for (auto& service : _services)
        if (service.second.RestartPending)
        {
            service.second.RestartPending = false;
            std::string error;
            if (!_shuttingDown && !Start(service.first, error))
                SF_LOG_ERROR("server.hub", "Unable to restart '%s': %s.", service.first.c_str(), error.c_str());
        }
}

void HubProcessSupervisor::Update(std::string const& key, ManagedServiceRuntime& runtime)
{
    if (!IsActive(runtime))
        return;
#ifdef _WIN32
    DWORD exitCode = STILL_ACTIVE;
    if (!GetExitCodeProcess(reinterpret_cast<HANDLE>(uintptr_t(runtime.ProcessHandle)), &exitCode) || exitCode != STILL_ACTIVE)
    {
        MarkExited(key, runtime, exitCode == STILL_ACTIVE ? -1 : int64(exitCode));
        return;
    }
#else
    int status = 0;
    pid_t const result = waitpid(pid_t(runtime.ProcessId), &status, WNOHANG);
    if (result == pid_t(runtime.ProcessId))
    {
        MarkExited(key, runtime, WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status));
        return;
    }
#endif
    ReadStatusMessages(key, runtime);
    auto const now = std::chrono::steady_clock::now();
    if (!IsWorldKey(key) && !runtime.BackupControlled && runtime.State == HubManagedProcessState::Stopping && now - runtime.StopRequestedAt > ShutdownTimeout)
        ForceStop(key, runtime);
    else if (runtime.State == HubManagedProcessState::Starting && now - runtime.StartedAt > StartupTimeout)
        runtime.State = HubManagedProcessState::Unresponsive;
    else if (runtime.Ready && now - runtime.LastHeartbeat > HeartbeatTimeout &&
        runtime.State != HubManagedProcessState::Stopping)
        runtime.State = HubManagedProcessState::Unresponsive;
}

void HubProcessSupervisor::ReadStatusMessages(std::string const& key, ManagedServiceRuntime& runtime)
{
    if (!runtime.StatusReadHandle)
        return;
    char buffer[512];
#ifdef _WIN32
    while (true)
    {
        DWORD available = 0;
        if (!PeekNamedPipe(reinterpret_cast<HANDLE>(uintptr_t(runtime.StatusReadHandle)), nullptr, 0,
            nullptr, &available, nullptr) || !available)
            break;
        DWORD bytesRead = 0;
        DWORD const requested = std::min<DWORD>(available, DWORD(sizeof(buffer)));
        if (!ReadFile(reinterpret_cast<HANDLE>(uintptr_t(runtime.StatusReadHandle)), buffer, requested,
            &bytesRead, nullptr) || !bytesRead)
            break;
        runtime.StatusBuffer.append(buffer, bytesRead);
    }
#else
    while (true)
    {
        ssize_t const bytesRead = read(int(runtime.StatusReadHandle), buffer, sizeof(buffer));
        if (bytesRead > 0) { runtime.StatusBuffer.append(buffer, size_t(bytesRead)); continue; }
        if (bytesRead < 0 && errno == EINTR) continue;
        break;
    }
#endif
    size_t lineEnd = 0;
    while ((lineEnd = runtime.StatusBuffer.find('\n')) != std::string::npos)
    {
        std::string const message = runtime.StatusBuffer.substr(0, lineEnd);
        runtime.StatusBuffer.erase(0, lineEnd + 1);
        ProcessStatusMessage(key, runtime, message);
    }
}

void HubProcessSupervisor::ProcessStatusMessage(std::string const& key, ManagedServiceRuntime& runtime,
    std::string const& message)
{
    auto const now = std::chrono::steady_clock::now();
    if (message == Skyfire::HubControl::StartingMessage)
    {
        runtime.State = HubManagedProcessState::Starting;
        runtime.LastHeartbeat = now;
    }
    else if (message == Skyfire::HubControl::ReadyMessage || message == Skyfire::HubControl::WorldReadyMessage || message == Skyfire::HubControl::AccountReadyMessage)
    {
        runtime.CanSendCommands = IsWorldKey(key) && message != Skyfire::HubControl::ReadyMessage;
        runtime.CanManageAccounts = IsWorldKey(key) && message == Skyfire::HubControl::AccountReadyMessage;
        runtime.Ready = true;
        runtime.State = HubManagedProcessState::Running;
        runtime.LastHeartbeat = now;
        SF_LOG_INFO("server.hub", "Managed service '%s' reported ready.", key.c_str());
    }
    else if (message == Skyfire::HubControl::HeartbeatMessage)
    {
        runtime.LastHeartbeat = now;
        if (runtime.Ready && runtime.State != HubManagedProcessState::Stopping)
            runtime.State = HubManagedProcessState::Running;
    }
    else if (IsWorldKey(key) && message.compare(0, 8, "METRICS ") == 0)
    {
        std::istringstream input(message.substr(8));
        uint32 players, updateTime;
        uint64 tick;
        int32 cpu;
        std::string extra;
        if (input >> players >> updateTime >> tick >> cpu && !(input >> extra) && cpu >= -1 && cpu <= 10000 && tick != runtime.LastTick)
        {
            runtime.Players = players;
            runtime.UpdateTimeMs = updateTime;
            runtime.CpuBasisPoints = cpu;
            runtime.LastTick = tick;
            runtime.LastMetrics = now;
            runtime.MetricsAvailable = true;
        }
    }
    else if (message.compare(0, 7, "RESULT ") == 0 && runtime.CommandPending)
    {
        runtime.CommandPending = false;
        if (runtime.AccountCallback)
        {
            auto callback = std::move(runtime.AccountCallback);
            callback(message.substr(7));
            return;
        }
        runtime.CommandResult = message.substr(7);
        std::printf("%s: %s\n", key.c_str(), runtime.CommandResult.c_str());
        std::fflush(stdout);
    }
    else if (message == Skyfire::HubControl::StoppingMessage)
    {
        if (runtime.State != HubManagedProcessState::Stopping)
            runtime.StopRequestedAt = now;
        runtime.State = HubManagedProcessState::Stopping;
    }
}

void HubProcessSupervisor::StopAll()
{
    _shuttingDown = true;
    for (auto& service : _services)
    {
        std::string ignored;
        if (IsActive(service.second) && service.second.State != HubManagedProcessState::Stopping)
            (void)Stop(service.first, ignored);
    }
    auto const deadline = std::chrono::steady_clock::now() + ShutdownTimeout;
    while (true)
    {
        Update();
        bool active = false;
        for (auto& service : _services)
        {
            // World saves and database draining must finish, even during hub exit.
            if (!IsWorldKey(service.first) && IsActive(service.second) &&
                std::chrono::steady_clock::now() >= deadline)
                ForceStop(service.first, service.second);
            active = active || IsActive(service.second);
        }
        if (!active)
            return;
        Skyfire::SleepForMilliseconds(100);
    }
}

HubManagedServiceStatus HubProcessSupervisor::GetStatus(std::string const& key) const
{
    HubManagedServiceStatus status;
    status.Key = key;
    status.Name = key;
    auto const found = _services.find(key);
    if (found == _services.end())
        return status;
    status.Name = found->second.Definition.Name;
    status.State = found->second.State;
    status.ProcessId = found->second.ProcessId;
    status.LastExitCode = found->second.LastExitCode;
    status.Enabled = found->second.Definition.Enabled;
    status.CanSendCommands = found->second.CanSendCommands;
    status.CommandPending = found->second.CommandPending;
    status.CommandResult = found->second.CommandResult;
    status.IsWorld = IsWorldKey(key);
    status.ExecutablePath = found->second.Definition.ExecutablePath;
    status.ConfigPath = found->second.Definition.ConfigPath;
    status.WorkingDirectory = found->second.Definition.WorkingDirectory;
    status.CpuBasisPoints = found->second.CpuBasisPoints;
    if (IsActive(found->second))
        status.UptimeSeconds = uint64(std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - found->second.StartedAt).count());
    status.MetricsAvailable = found->second.State == HubManagedProcessState::Running &&
        found->second.MetricsAvailable && std::chrono::steady_clock::now() - found->second.LastMetrics <= HeartbeatTimeout;
    status.Players = found->second.Players;
    status.UpdateTimeMs = found->second.UpdateTimeMs;
    return status;
}

std::vector<HubManagedServiceStatus> HubProcessSupervisor::GetStatuses() const
{
    std::vector<HubManagedServiceStatus> statuses;
    statuses.reserve(_services.size());
    for (auto const& service : _services)
        statuses.push_back(GetStatus(service.first));
    std::sort(statuses.begin(), statuses.end(), [](HubManagedServiceStatus const& left,
        HubManagedServiceStatus const& right) { return left.Key < right.Key; });
    return statuses;
}

bool HubProcessSupervisor::WriteControl(ManagedServiceRuntime const& runtime, char const* message)
{
    return runtime.ControlWriteHandle && WritePipe(runtime.ControlWriteHandle, message);
}

bool HubProcessSupervisor::IsActive(ManagedServiceRuntime const& runtime)
{
    return runtime.State == HubManagedProcessState::Starting || runtime.State == HubManagedProcessState::Running ||
        runtime.State == HubManagedProcessState::Unresponsive || runtime.State == HubManagedProcessState::Stopping;
}

char const* HubProcessSupervisor::GetStateName(HubManagedProcessState state)
{
    switch (state)
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

void HubProcessSupervisor::MarkExited(std::string const& key, ManagedServiceRuntime& runtime, int64 exitCode)
{
    SF_LOG_INFO("server.hub", "Managed service '%s' process %llu exited with code %lld.", key.c_str(),
        static_cast<unsigned long long>(runtime.ProcessId), static_cast<long long>(exitCode));
    ReadStatusMessages(key, runtime);
    runtime.RestartPending = IsWorldKey(key) && exitCode == Skyfire::HubControl::WorldRestartExitCode &&
        !_shuttingDown && !runtime.SuppressRestart;
    if (runtime.CommandPending)
        runtime.CommandResult = "Worldserver exited before returning a command result.";
    if (runtime.AccountCallback)
    {
        auto callback = std::move(runtime.AccountCallback);
        callback("ERROR ACCOUNT 503 {\"error\":\"Worldserver exited before acknowledging the change. Check account state before retrying.\"}");
    }
    runtime.CommandPending = false;
    runtime.CanSendCommands = false;
    runtime.LastExitCode = exitCode;
    runtime.State = HubManagedProcessState::Exited;
    runtime.Ready = false;
    CloseHandles(runtime);
}

void HubProcessSupervisor::ForceStop(std::string const& key, ManagedServiceRuntime& runtime)
{
    SF_LOG_ERROR("server.hub", "Managed service '%s' did not stop cleanly; terminating it.", key.c_str());
#ifdef _WIN32
    HANDLE const process = reinterpret_cast<HANDLE>(uintptr_t(runtime.ProcessHandle));
    if (process)
    {
        TerminateProcess(process, 1);
        (void)WaitForSingleObject(process, 5000);
    }
    DWORD exitCode = 1;
    if (process)
        (void)GetExitCodeProcess(process, &exitCode);
    MarkExited(key, runtime, exitCode == STILL_ACTIVE ? -1 : int64(exitCode));
#else
    if (runtime.ProcessId)
    {
        kill(pid_t(runtime.ProcessId), SIGKILL);
        int status = 0;
        (void)waitpid(pid_t(runtime.ProcessId), &status, 0);
        MarkExited(key, runtime, WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status));
    }
#endif
}

void HubProcessSupervisor::CloseHandles(ManagedServiceRuntime& runtime)
{
#ifdef _WIN32
    if (runtime.ProcessHandle) CloseHandle(reinterpret_cast<HANDLE>(uintptr_t(runtime.ProcessHandle)));
    if (runtime.ControlWriteHandle) CloseHandle(reinterpret_cast<HANDLE>(uintptr_t(runtime.ControlWriteHandle)));
    if (runtime.StatusReadHandle) CloseHandle(reinterpret_cast<HANDLE>(uintptr_t(runtime.StatusReadHandle)));
#else
    if (runtime.ControlWriteHandle) close(int(runtime.ControlWriteHandle));
    if (runtime.StatusReadHandle) close(int(runtime.StatusReadHandle));
#endif
    runtime.ProcessId = 0;
    runtime.ProcessHandle = 0;
    runtime.ControlWriteHandle = 0;
    runtime.StatusReadHandle = 0;
    runtime.StatusBuffer.clear();
}
