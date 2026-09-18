/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_HUB_WEB_SERVER_H
#define SKYFIRE_HUB_WEB_SERVER_H

#include "Define.h"
#include "Cluster/ClusterProtocol.h"
#include "Auth/AccountAdministration.h"
#include "Threading/BoostAsioThreadGroup.h"

#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <chrono>
#include <deque>
#include <future>
#include <memory>
#include <map>
#include <mutex>
#include <string>
#include <vector>

class HubWebSession;

struct HubWebManagedServiceStatus
{
    std::string Key;
    std::string Name;
    std::string State = "stopped";
    uint64 ProcessId = 0;
    int64 LastExitCode = 0;
    bool Enabled = false;
    bool CommandPending = false;
    bool CanSendCommands = false;
    std::string CommandResult;
    std::string ExecutablePath, ConfigPath, WorkingDirectory;
    bool IsWorld = false;
    uint64 UptimeSeconds = 0;
    bool MetricsAvailable = false;
    uint32 Players = 0;
    uint32 UpdateTimeMs = 0;
    int32 CpuBasisPoints = -1;
};

struct HubWebStatusSnapshot
{
    uint64 UptimeSeconds = 0;
    std::vector<HubWebManagedServiceStatus> Services;
    std::vector<Skyfire::Cluster::Node> ClusterNodes;
};

struct HubWebServiceCommand
{
    std::string ServiceKey;
    bool Start = false;
    bool Configure = false;
    std::string Name, ExecutablePath, ConfigPath, WorkingDirectory;
    std::string WorldCommand;
    std::string AccountRequest;
    std::shared_ptr<std::promise<Skyfire::Auth::AccountAdminReply>> AccountResult;
    std::shared_ptr<std::promise<std::string>> DispatchResult;
};

class HubWebServer
{
public:
    HubWebServer();
    ~HubWebServer();

    HubWebServer(HubWebServer const&) = delete;
    HubWebServer& operator=(HubWebServer const&) = delete;

    bool Open(std::string const& bindIp, uint16 port, std::string webRoot,
        bool allowRemote, uint32 sessionTimeoutSeconds);
    void Close();
    void UpdateStatus(HubWebStatusSnapshot const& status);
    bool PollServiceCommand(HubWebServiceCommand& command);

private:
    friend class HubWebSession;

    struct AuthenticatedSession
    {
        std::string Username;
        std::string CsrfToken;
        uint64 AccessFlags = 0;
        std::chrono::steady_clock::time_point ExpiresAt;
    };

    struct LoginAttempt
    {
        uint32 Failures = 0;
        std::chrono::steady_clock::time_point RetryAfter;
    };

    void AsyncAccept();
    std::string HandleRequest(std::string const& method, std::string const& target,
        std::map<std::string, std::string> const& headers, std::string const& body,
        std::string const& remoteAddress, bool remoteIsLoopback);
    std::string HandleLogin(std::string const& body, std::string const& remoteAddress,
        bool remoteIsLoopback);
    std::string HandleLogout(std::map<std::string, std::string> const& headers);
    std::string HandleStatus(std::map<std::string, std::string> const& headers);
    std::string HandleServiceCommand(std::string const& path,
        std::map<std::string, std::string> const& headers, std::string const& body);
    std::string HandleAccounts(std::string const& action,
        std::map<std::string, std::string> const& headers, std::string const& body);
    std::string ServeAsset(std::string const& target) const;
    bool FindSession(std::map<std::string, std::string> const& headers,
        AuthenticatedSession& session);
    void PurgeExpiredSessionsLocked(std::chrono::steady_clock::time_point now);

    Skyfire::Asio::IoContextThreadGroup _threadGroup;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::atomic<bool> _closed;
    std::string _webRoot;
    bool _allowRemote;
    uint32 _sessionTimeoutSeconds;

    std::mutex _statusMutex;
    HubWebStatusSnapshot _status;

    std::mutex _authMutex;
    std::map<std::string, AuthenticatedSession> _sessions;
    std::map<std::string, LoginAttempt> _loginAttempts;

    struct AccountJob
    {
        std::string OwnerToken;
        std::shared_future<Skyfire::Auth::AccountAdminReply> Result;
        std::chrono::steady_clock::time_point ExpiresAt;
    };
    std::mutex _accountJobsMutex;
    std::map<std::string, AccountJob> _accountJobs;

    std::mutex _commandMutex;
    std::deque<HubWebServiceCommand> _commands;
};

#endif
