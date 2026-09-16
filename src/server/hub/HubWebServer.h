/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SKYFIRE_HUB_WEB_SERVER_H
#define SKYFIRE_HUB_WEB_SERVER_H

#include "Define.h"
#include "Threading/BoostAsioThreadGroup.h"

#include <atomic>
#include <boost/asio/ip/tcp.hpp>
#include <chrono>
#include <map>
#include <mutex>
#include <string>

class HubWebSession;

struct HubWebStatusSnapshot
{
    uint64 UptimeSeconds = 0;
    std::string AuthnetState = "stopped";
    uint64 AuthnetProcessId = 0;
    int64 AuthnetLastExitCode = 0;
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

private:
    friend class HubWebSession;

    struct AuthenticatedSession
    {
        std::string Username;
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
};

#endif
