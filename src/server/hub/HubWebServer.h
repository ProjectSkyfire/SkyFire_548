/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_HUB_WEB_SERVER_H
#define SKYFIRE_HUB_WEB_SERVER_H

#include "Define.h"
#include "Cluster/ClusterProtocol.h"
#include "HubAuthProxy.h"
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
    std::string RestartState, RestartMessage;
    bool RestartActive = false;
    std::vector<HubWebManagedServiceStatus> Services;
    std::vector<Skyfire::Cluster::Node> ClusterNodes;
    std::vector<HubAuthProxyStatus> AuthIngress;
};

struct HubWebServiceCommand
{
    std::string ServiceKey;
    bool Start = false;
    bool RestartAll = false;
    bool Configure = false;
    std::string Name, ExecutablePath, ConfigPath, WorkingDirectory;
    std::string WorldCommand;
    std::string AccountRequest;
    std::string ClusterAction, Actor, CorrelationId, ControlAction;
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
        bool allowRemote, uint32 sessionTimeoutSeconds, std::string controlToken = "");
    void Close();
    void UpdateStatus(HubWebStatusSnapshot const& status);
    bool PollServiceCommand(HubWebServiceCommand& command);
    void CompleteControlCommand(HubWebServiceCommand const& command, bool accepted);

private:
    friend class HubWebSession;

    struct AuthenticatedSession
    {
        std::string Username;
        std::string CredentialHash;
        bool Remote = false;
        std::string CsrfToken;
        uint64 AccessFlags = 0;
        std::chrono::steady_clock::time_point ExpiresAt;
    };

    struct LoginAttempt
    {
        uint32 Failures = 0;
        std::chrono::steady_clock::time_point RetryAfter;
    };

    std::string HandleControl(std::string const& method, std::string const& path,
        std::map<std::string,std::string> const& headers, std::string const& body, bool loopback);
    std::string HandleBackupSchedule(std::string const& method, std::map<std::string,std::string> const& headers,
        std::string const& body, bool remote = false);
    std::string BackupSchedules(bool canEdit);
    std::string BackupJobs(bool administrator);
    std::string HandleBackupJobs(std::string const& method, std::map<std::string,std::string> const& headers,
        std::string const& body, bool remote = false);
    std::string ControlStatus(AuthenticatedSession const& session);
    std::string ControlCommand(std::map<std::string,std::string> const& headers, std::string const& body);
    bool AuditControl(std::string const& id, std::string const& phase, std::string const& actor,
        std::string const& action, std::string const& target, std::string const& outcome);
    std::string _controlToken;
    struct ControlResult { std::string Owner, State; std::chrono::steady_clock::time_point ExpiresAt; };
    std::mutex _controlMutex, _controlAdmissionMutex;
    std::map<std::string,ControlResult> _controlResults;
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
    std::string HandleClusterCommand(std::string const& path, std::map<std::string,std::string> const& headers);
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
    std::chrono::steady_clock::time_point _backupHealthAt{};
    bool _backupServicesStopped = false;

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
