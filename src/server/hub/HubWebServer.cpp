/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "HubWebServer.h"
#include "HubProcessSupervisor.h"

#include "Cryptography/CryptoRandom.h"
#include "Database/DatabaseEnv.h"
#include "Log.h"
#include "Network/BoostAsioUtils.h"
#include "Utilities/Util.h"

#include <algorithm>
#include <array>
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <memory>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace
{
    constexpr size_t MaximumHeaderBytes = 32 * 1024;
    constexpr size_t MaximumBodyBytes = 16 * 1024;
    constexpr uint32 MaximumLoginFailures = 5;
    constexpr std::chrono::seconds LoginLockout(30);

    std::string Trim(std::string value)
    {
        auto const notSpace = [](unsigned char character) { return !std::isspace(character); };
        value.erase(value.begin(), std::find_if(value.begin(), value.end(), notSpace));
        value.erase(std::find_if(value.rbegin(), value.rend(), notSpace).base(), value.end());
        return value;
    }

    std::string ToLower(std::string value)
    {
        std::transform(value.begin(), value.end(), value.begin(),
            [](unsigned char character) { return char(std::tolower(character)); });
        return value;
    }

    std::string JsonEscape(std::string const& value)
    {
        std::ostringstream output;
        for (unsigned char character : value)
        {
            switch (character)
            {
                case '\\': output << "\\\\"; break;
                case '"': output << "\\\""; break;
                case '\b': output << "\\b"; break;
                case '\f': output << "\\f"; break;
                case '\n': output << "\\n"; break;
                case '\r': output << "\\r"; break;
                case '\t': output << "\\t"; break;
                default:
                    if (character < 0x20)
                        output << "\\u" << std::hex << std::setw(4) << std::setfill('0') << uint32(character);
                    else
                        output << character;
                    break;
            }
        }
        return output.str();
    }

    int HexValue(char character)
    {
        if (character >= '0' && character <= '9')
            return character - '0';
        if (character >= 'a' && character <= 'f')
            return character - 'a' + 10;
        if (character >= 'A' && character <= 'F')
            return character - 'A' + 10;
        return -1;
    }

    std::string UrlDecode(std::string const& value)
    {
        std::string decoded;
        decoded.reserve(value.size());
        for (size_t index = 0; index < value.size(); ++index)
        {
            if (value[index] == '+')
                decoded.push_back(' ');
            else if (value[index] == '%' && index + 2 < value.size())
            {
                int const high = HexValue(value[index + 1]);
                int const low = HexValue(value[index + 2]);
                if (high >= 0 && low >= 0)
                {
                    decoded.push_back(char((high << 4) | low));
                    index += 2;
                }
                else
                    decoded.push_back(value[index]);
            }
            else
                decoded.push_back(value[index]);
        }
        return decoded;
    }

    std::map<std::string, std::string> ParseForm(std::string const& body)
    {
        std::map<std::string, std::string> values;
        size_t start = 0;
        while (start <= body.size())
        {
            size_t const end = body.find('&', start);
            std::string const pair = body.substr(start, end == std::string::npos ? std::string::npos : end - start);
            size_t const equals = pair.find('=');
            if (equals != std::string::npos)
                values[UrlDecode(pair.substr(0, equals))] = UrlDecode(pair.substr(equals + 1));
            if (end == std::string::npos)
                break;
            start = end + 1;
        }
        return values;
    }

    bool DecodeHex(std::string const& value, std::vector<uint8>& decoded)
    {
        if (value.empty() || value.size() % 2 != 0)
            return false;
        decoded.resize(value.size() / 2);
        for (size_t index = 0; index < decoded.size(); ++index)
        {
            int const high = HexValue(value[index * 2]);
            int const low = HexValue(value[index * 2 + 1]);
            if (high < 0 || low < 0)
                return false;
            decoded[index] = uint8((high << 4) | low);
        }
        return true;
    }

    bool VerifyPassword(std::string const& password, std::string const& encodedHash)
    {
        std::vector<std::string> parts;
        size_t start = 0;
        while (start <= encodedHash.size())
        {
            size_t const end = encodedHash.find('$', start);
            parts.push_back(encodedHash.substr(start, end == std::string::npos ? std::string::npos : end - start));
            if (end == std::string::npos)
                break;
            start = end + 1;
        }

        if (parts.size() != 5 || !parts[0].empty() || parts[1] != "pbkdf2-sha256")
            return false;

        uint32 iterations = 0;
        try
        {
            size_t parsed = 0;
            unsigned long const value = std::stoul(parts[2], &parsed);
            if (parsed != parts[2].size() || value < 100000 || value > 2000000)
                return false;
            iterations = uint32(value);
        }
        catch (...)
        {
            return false;
        }

        std::vector<uint8> salt;
        std::vector<uint8> expected;
        if (!DecodeHex(parts[3], salt) || !DecodeHex(parts[4], expected) || expected.empty())
            return false;

        std::vector<uint8> actual(expected.size());
        if (PKCS5_PBKDF2_HMAC(password.data(), int(password.size()), salt.data(), int(salt.size()),
            int(iterations), EVP_sha256(), int(actual.size()), actual.data()) != 1)
            return false;

        return CRYPTO_memcmp(actual.data(), expected.data(), expected.size()) == 0;
    }

    std::string ReasonPhrase(int status)
    {
        switch (status)
        {
            case 200: return "OK";
            case 202: return "Accepted";
            case 204: return "No Content";
            case 400: return "Bad Request";
            case 401: return "Unauthorized";
            case 403: return "Forbidden";
            case 404: return "Not Found";
            case 405: return "Method Not Allowed";
            case 409: return "Conflict";
            case 413: return "Payload Too Large";
            case 429: return "Too Many Requests";
            case 503: return "Service Unavailable";
            default: return "Internal Server Error";
        }
    }

    std::string MakeResponse(int status, std::string const& contentType, std::string const& body,
        std::string const& extraHeaders = std::string())
    {
        std::ostringstream response;
        response << "HTTP/1.1 " << status << ' ' << ReasonPhrase(status) << "\r\n"
                 << "Content-Type: " << contentType << "\r\n"
                 << "Content-Length: " << body.size() << "\r\n"
                 << "Connection: close\r\n"
                 << "Cache-Control: no-store\r\n"
                 << "Content-Security-Policy: default-src 'self'; script-src 'self'; style-src 'self'; img-src 'self'; object-src 'none'; frame-ancestors 'none'; base-uri 'none'; form-action 'self'\r\n"
                 << "Referrer-Policy: no-referrer\r\n"
                 << "X-Content-Type-Options: nosniff\r\n"
                 << "X-Frame-Options: DENY\r\n"
                 << extraHeaders << "\r\n" << body;
        return response.str();
    }

    std::string GetCookie(std::map<std::string, std::string> const& headers, std::string const& name)
    {
        auto const header = headers.find("cookie");
        if (header == headers.end())
            return std::string();

        size_t start = 0;
        while (start < header->second.size())
        {
            size_t const end = header->second.find(';', start);
            std::string const pair = Trim(header->second.substr(start,
                end == std::string::npos ? std::string::npos : end - start));
            size_t const equals = pair.find('=');
            if (equals != std::string::npos && pair.substr(0, equals) == name)
                return pair.substr(equals + 1);
            if (end == std::string::npos)
                break;
            start = end + 1;
        }
        return std::string();
    }

    std::string GetContentType(std::string const& path)
    {
        if (path.size() >= 5 && path.substr(path.size() - 5) == ".html")
            return "text/html; charset=utf-8";
        if (path.size() >= 4 && path.substr(path.size() - 4) == ".css")
            return "text/css; charset=utf-8";
        if (path.size() >= 3 && path.substr(path.size() - 3) == ".js")
            return "application/javascript; charset=utf-8";
        if (path.size() >= 4 && path.substr(path.size() - 4) == ".png")
            return "image/png";
        if (path.size() >= 4 && path.substr(path.size() - 4) == ".ico")
            return "image/x-icon";
        return "application/octet-stream";
    }
}

class HubWebSession : public std::enable_shared_from_this<HubWebSession>
{
public:
    HubWebSession(HubWebServer& server, boost::asio::ip::tcp::socket socket,
        std::string remoteAddress, bool remoteIsLoopback)
        : _server(server), _socket(std::move(socket)), _buffer(MaximumHeaderBytes),
          _remoteAddress(std::move(remoteAddress)), _remoteIsLoopback(remoteIsLoopback)
    {
    }

    void Start()
    {
        std::shared_ptr<HubWebSession> self = shared_from_this();
        boost::asio::async_read_until(_socket, _buffer, "\r\n\r\n",
            [self](boost::system::error_code const& error, size_t)
            {
                if (!error)
                    self->ReadHeaders();
            });
    }

private:
    void ReadHeaders()
    {
        std::istream input(&_buffer);
        std::string requestLine;
        std::getline(input, requestLine);
        if (!requestLine.empty() && requestLine.back() == '\r')
            requestLine.pop_back();

        std::istringstream line(requestLine);
        std::string version;
        line >> _method >> _target >> version;
        if (_method.empty() || _target.empty() || version.compare(0, 5, "HTTP/") != 0)
        {
            Write(MakeResponse(400, "application/json", "{\"error\":\"invalid request\"}"));
            return;
        }

        std::string headerLine;
        while (std::getline(input, headerLine) && headerLine != "\r")
        {
            if (!headerLine.empty() && headerLine.back() == '\r')
                headerLine.pop_back();
            size_t const colon = headerLine.find(':');
            if (colon != std::string::npos)
                _headers[ToLower(Trim(headerLine.substr(0, colon)))] = Trim(headerLine.substr(colon + 1));
        }

        size_t contentLength = 0;
        auto const lengthHeader = _headers.find("content-length");
        if (lengthHeader != _headers.end())
        {
            try
            {
                size_t parsed = 0;
                contentLength = std::stoul(lengthHeader->second, &parsed);
                if (parsed != lengthHeader->second.size())
                    throw std::invalid_argument("content length");
            }
            catch (...)
            {
                Write(MakeResponse(400, "application/json", "{\"error\":\"invalid content length\"}"));
                return;
            }
        }

        if (contentLength > MaximumBodyBytes)
        {
            Write(MakeResponse(413, "application/json", "{\"error\":\"request body too large\"}"));
            return;
        }

        if (_buffer.size() >= contentLength)
        {
            ReadBody(contentLength);
            return;
        }

        std::shared_ptr<HubWebSession> self = shared_from_this();
        boost::asio::async_read(_socket, _buffer, boost::asio::transfer_exactly(contentLength - _buffer.size()),
            [self, contentLength](boost::system::error_code const& error, size_t)
            {
                if (!error)
                    self->ReadBody(contentLength);
            });
    }

    void ReadBody(size_t contentLength)
    {
        std::string body(contentLength, '\0');
        if (contentLength)
        {
            std::istream input(&_buffer);
            input.read(&body[0], std::streamsize(contentLength));
        }
        Write(_server.HandleRequest(_method, _target, _headers, body, _remoteAddress, _remoteIsLoopback));
    }

    void Write(std::string response)
    {
        _response.reset(new std::string(std::move(response)));
        std::shared_ptr<HubWebSession> self = shared_from_this();
        boost::asio::async_write(_socket, boost::asio::buffer(*_response),
            [self](boost::system::error_code const&, size_t)
            {
                Skyfire::Net::CloseTcpSocket(self->_socket);
            });
    }

    HubWebServer& _server;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::streambuf _buffer;
    std::string _remoteAddress;
    bool _remoteIsLoopback;
    std::string _method;
    std::string _target;
    std::map<std::string, std::string> _headers;
    std::shared_ptr<std::string> _response;
};

HubWebServer::HubWebServer()
    : _threadGroup(), _acceptor(_threadGroup.GetIoContext()), _closed(true),
      _allowRemote(false), _sessionTimeoutSeconds(1800)
{
}

HubWebServer::~HubWebServer()
{
    Close();
}

bool HubWebServer::Open(std::string const& bindIp, uint16 port, std::string webRoot,
    bool allowRemote, uint32 sessionTimeoutSeconds)
{
    if (!Skyfire::Net::OpenTcpAcceptor(_threadGroup.GetIoContext(), _acceptor, port, bindIp,
        "server.hub", "hub web console"))
        return false;

    _webRoot = std::move(webRoot);
    while (!_webRoot.empty() && (_webRoot.back() == '/' || _webRoot.back() == '\\'))
        _webRoot.pop_back();
    _allowRemote = allowRemote;
    _sessionTimeoutSeconds = sessionTimeoutSeconds;
    _closed = false;
    AsyncAccept();

    if (_threadGroup.Start(1) == -1)
    {
        Close();
        return false;
    }

    SF_LOG_INFO("server.hub", "Hub web console listening on http://%s:%u.", bindIp.c_str(), port);
    return true;
}

void HubWebServer::Close()
{
    bool expected = false;
    if (!_closed.compare_exchange_strong(expected, true))
        return;

    Skyfire::Net::CloseTcpAcceptor(_acceptor);
    _threadGroup.StopAndJoin();

    std::lock_guard<std::mutex> lock(_authMutex);
    _sessions.clear();
    _loginAttempts.clear();

    std::lock_guard<std::mutex> commandLock(_commandMutex);
    _commands.clear();
}

void HubWebServer::UpdateStatus(HubWebStatusSnapshot const& status)
{
    std::lock_guard<std::mutex> lock(_statusMutex);
    _status = status;
}

bool HubWebServer::PollServiceCommand(HubWebServiceCommand& command)
{
    std::lock_guard<std::mutex> lock(_commandMutex);
    if (_commands.empty())
        return false;
    command = std::move(_commands.front());
    _commands.pop_front();
    return true;
}

void HubWebServer::AsyncAccept()
{
    if (!_acceptor.is_open())
        return;

    std::shared_ptr<boost::asio::ip::tcp::socket> socket(
        new boost::asio::ip::tcp::socket(_threadGroup.GetIoContext()));
    _acceptor.async_accept(*socket,
        [this, socket](boost::system::error_code const& error)
        {
            if (!_closed && !error)
            {
                boost::system::error_code endpointError;
                boost::asio::ip::tcp::endpoint const endpoint = socket->remote_endpoint(endpointError);
                std::string const address = endpointError ? std::string("<unknown>") : endpoint.address().to_string();
                bool const loopback = !endpointError && endpoint.address().is_loopback();
                std::make_shared<HubWebSession>(*this, std::move(*socket), address, loopback)->Start();
            }
            else if (!_closed && error != boost::asio::error::operation_aborted)
                SF_LOG_ERROR("server.hub", "Hub web console accept failed with error %d.", error.value());

            if (!_closed)
                AsyncAccept();
        });
}

std::string HubWebServer::HandleRequest(std::string const& method, std::string const& target,
    std::map<std::string, std::string> const& headers, std::string const& body,
    std::string const& remoteAddress, bool remoteIsLoopback)
{
    if (!remoteIsLoopback && !_allowRemote)
        return MakeResponse(403, "application/json", "{\"error\":\"remote web access is disabled\"}");

    std::string path = target.substr(0, target.find('?'));
    if (path == "/api/v1/session" && method == "POST")
        return HandleLogin(body, remoteAddress, remoteIsLoopback);
    if (path == "/api/v1/logout" && method == "POST")
        return HandleLogout(headers);
    if (path == "/api/v1/status" && method == "GET")
        return HandleStatus(headers);
    if (path.compare(0,16,"/api/v1/cluster/") == 0 && method == "POST")
        return HandleClusterCommand(path,headers);
    if (path.compare(0, 17, "/api/v1/accounts/") == 0 && method == "POST")
        return HandleAccounts(path.substr(17), headers, body);
    if (path.compare(0, 17, "/api/v1/services/") == 0 && method == "POST")
        return HandleServiceCommand(path, headers, body);
    if (path.compare(0, 8, "/api/v1/") == 0)
        return MakeResponse(404, "application/json", "{\"error\":\"endpoint not found\"}");
    if (method != "GET")
        return MakeResponse(405, "application/json", "{\"error\":\"method not allowed\"}");
    return ServeAsset(path);
}

std::string HubWebServer::HandleLogin(std::string const& body, std::string const& remoteAddress,
    bool remoteIsLoopback)
{
    std::map<std::string, std::string> const form = ParseForm(body);
    auto const usernameValue = form.find("username");
    auto const passwordValue = form.find("password");
    if (usernameValue == form.end() || passwordValue == form.end() ||
        usernameValue->second.empty() || passwordValue->second.empty())
        return MakeResponse(400, "application/json", "{\"error\":\"username and password are required\"}");

    auto const now = std::chrono::steady_clock::now();
    {
        std::lock_guard<std::mutex> lock(_authMutex);
        LoginAttempt const& attempt = _loginAttempts[remoteAddress];
        if (attempt.Failures >= MaximumLoginFailures && now < attempt.RetryAfter)
            return MakeResponse(429, "application/json", "{\"error\":\"too many login attempts\"}");
    }

    PreparedStatement* statement = HubDatabase.GetPreparedStatement(HUB_SEL_ADMIN_BY_USERNAME);
    statement->setString(0, usernameValue->second);
    PreparedQueryResult result = HubDatabase.Query(statement);

    bool authenticated = false;
    uint64 accessFlags = 0;
    if (result)
    {
        Field* fields = result->Fetch();
        accessFlags = fields[3].GetUInt64();
        authenticated = fields[4].GetUInt8() != 0 &&
            VerifyPassword(passwordValue->second, fields[2].GetString()) &&
            (remoteIsLoopback || HubAdminCanLoginRemotely(accessFlags));
    }

    if (!authenticated)
    {
        std::lock_guard<std::mutex> lock(_authMutex);
        LoginAttempt& attempt = _loginAttempts[remoteAddress];
        ++attempt.Failures;
        if (attempt.Failures >= MaximumLoginFailures)
            attempt.RetryAfter = now + LoginLockout;
        return MakeResponse(401, "application/json", "{\"error\":\"invalid credentials or access denied\"}");
    }

    std::string const token = ByteArrayToHexStr(SkyFire::Crypto::GetRandomBytes<32>());
    {
        std::lock_guard<std::mutex> lock(_authMutex);
        PurgeExpiredSessionsLocked(now);
        _loginAttempts.erase(remoteAddress);
        AuthenticatedSession session;
        session.Username = usernameValue->second;
        session.CsrfToken = ByteArrayToHexStr(SkyFire::Crypto::GetRandomBytes<32>());
        session.AccessFlags = accessFlags;
        session.ExpiresAt = now + std::chrono::seconds(_sessionTimeoutSeconds);
        _sessions[token] = session;
    }

    PreparedStatement* updateLogin = HubDatabase.GetPreparedStatement(HUB_UPD_ADMIN_LAST_LOGIN);
    updateLogin->setString(0, usernameValue->second);
    HubDatabase.Execute(updateLogin);

    std::ostringstream cookie;
    cookie << "Set-Cookie: hub_session=" << token << "; Path=/; HttpOnly; SameSite=Strict; Max-Age="
           << _sessionTimeoutSeconds << "\r\n";
    return MakeResponse(200, "application/json",
        "{\"authenticated\":true,\"username\":\"" + JsonEscape(usernameValue->second) + "\"}", cookie.str());
}

std::string HubWebServer::HandleLogout(std::map<std::string, std::string> const& headers)
{
    std::string const token = GetCookie(headers, "hub_session");
    if (!token.empty())
    {
        std::lock_guard<std::mutex> lock(_authMutex);
        _sessions.erase(token);
    }
    return MakeResponse(204, "application/json", std::string(),
        "Set-Cookie: hub_session=; Path=/; HttpOnly; SameSite=Strict; Max-Age=0\r\n");
}

std::string HubWebServer::HandleStatus(std::map<std::string, std::string> const& headers)
{
    AuthenticatedSession session;
    if (!FindSession(headers, session) || !(session.AccessFlags & HUB_ADMIN_ACCESS_VIEW_STATUS))
        return MakeResponse(401, "application/json", "{\"error\":\"authentication required\"}");

    HubWebStatusSnapshot status;
    {
        std::lock_guard<std::mutex> lock(_statusMutex);
        status = _status;
    }

    std::ostringstream json;
    json << "{\"authenticated\":true,\"username\":\"" << JsonEscape(session.Username)
         << "\",\"csrfToken\":\"" << session.CsrfToken
         << "\",\"canOperateServices\":"
         << ((session.AccessFlags & HUB_ADMIN_ACCESS_OPERATE_NODES) ? "true" : "false")
         << ",\"canSendWorldCommands\":"
         << (((session.AccessFlags & HUB_ADMIN_ACCESS_ALL_LOCAL) == HUB_ADMIN_ACCESS_ALL_LOCAL) ? "true" : "false")
         << ",\"accountsEnabled\":" << (Skyfire::Auth::AccountAdministration::IsEnabled() ? "true" : "false")
         << ",\"uptimeSeconds\":" << status.UptimeSeconds << ",\"components\":["
         << "{\"key\":\"hub\",\"name\":\"Hub Runtime\",\"status\":\"online\",\"detail\":\"Control process is running\"},"
         << "{\"key\":\"database\",\"name\":\"Hub Database\",\"status\":\"online\",\"detail\":\"Connection pool is active\"},"
         << "{\"key\":\"web\",\"name\":\"Web Console\",\"status\":\"online\",\"detail\":\"Secure session is active\"}";

    for (HubWebManagedServiceStatus const& service : status.Services)
    {
        std::string health = "offline";
        if (service.State == "running")
            health = "online";
        else if (service.State == "starting" || service.State == "stopping" ||
            service.State == "unresponsive")
            health = "issue";

        json << ",{\"key\":\"" << JsonEscape(service.Key) << "\",\"name\":\""
             << JsonEscape(service.Name) << "\",\"status\":\"" << health
             << "\",\"detail\":\"" << JsonEscape(service.Enabled ? service.State : "disabled");
        if (service.ProcessId)
            json << " | PID " << service.ProcessId;
        if (service.State == "exited")
            json << " | Exit " << service.LastExitCode;
        json << "\",\"isWorld\":" << (service.IsWorld ? "true" : "false")
             << ",\"uptimeSeconds\":" << service.UptimeSeconds
             << ",\"metricsAvailable\":" << (service.MetricsAvailable ? "true" : "false")
             << ",\"players\":" << service.Players << ",\"updateTimeMs\":" << service.UpdateTimeMs
             << ",\"cpuPercent\":" << (service.CpuBasisPoints < 0 ? "null" : std::to_string(service.CpuBasisPoints / 100.0));
        if ((session.AccessFlags & HUB_ADMIN_ACCESS_ALL_LOCAL) == HUB_ADMIN_ACCESS_ALL_LOCAL)
            json << ",\"executablePath\":\"" << JsonEscape(service.ExecutablePath)
                 << "\",\"configPath\":\"" << JsonEscape(service.ConfigPath)
                 << "\",\"workingDirectory\":\"" << JsonEscape(service.WorkingDirectory) << "\"";
        json << ",\"managed\":true,\"enabled\":" << (service.Enabled ? "true" : "false")
             << ",\"state\":\"" << JsonEscape(service.State) << "\""
             << ",\"canSendCommands\":" << (service.CanSendCommands ? "true" : "false")
             << ",\"commandPending\":" << (service.CommandPending ? "true" : "false")
             << ",\"commandResult\":\"" << JsonEscape(
                 (session.AccessFlags & HUB_ADMIN_ACCESS_ALL_LOCAL) == HUB_ADMIN_ACCESS_ALL_LOCAL ? service.CommandResult : "") << "\"}";
    }
    for (auto const& ingress : status.AuthIngress)
    {
        if (!ingress.Enabled) continue;
        json << ",{\"key\":\"ingress:" << JsonEscape(ingress.Name) << "\",\"name\":\"" << JsonEscape(ingress.Name)
             << "\",\"status\":\"" << (ingress.AvailableNodes ? "online" : "issue") << "\",\"detail\":\"" << JsonEscape(ingress.Address) << ':' << ingress.Port
             << " | eligible nodes " << ingress.AvailableNodes << " | backoff " << ingress.BackoffNodes
             << " | active " << ingress.Active << " | routed " << ingress.Routed << " | rejected " << ingress.Rejected
             << " | connect failures " << ingress.ConnectFailures << " | retries " << ingress.Retries
             << " | stream failures " << ingress.StreamFailures << " | unavailable " << ingress.NoBackend
             << (ingress.LastRejection.empty() ? "" : " | last rejection: "+JsonEscape(ingress.LastRejection)) << "\",\"managed\":false"
             << ",\"routing\":{\"active\":" << ingress.Active << ",\"accepted\":" << ingress.Accepted
             << ",\"routed\":" << ingress.Routed << ",\"rejected\":" << ingress.Rejected << ",\"attempts\":" << ingress.Attempts
             << ",\"connectFailures\":" << ingress.ConnectFailures << ",\"retries\":" << ingress.Retries
             << ",\"streamFailures\":" << ingress.StreamFailures << ",\"clientBytes\":" << ingress.ClientBytes
             << ",\"backendBytes\":" << ingress.BackendBytes << ",\"noBackend\":" << ingress.NoBackend
             << ",\"limitRejected\":" << ingress.LimitRejected << ",\"withdrawn\":" << ingress.Withdrawn
             << ",\"backoffNodes\":" << ingress.BackoffNodes << ",\"availableNodes\":" << ingress.AvailableNodes
             << ",\"lastRejection\":\"" << JsonEscape(ingress.LastRejection) << "\"}}";
    }
    for (auto const& node : status.ClusterNodes)
    {
        uint64 connections = 0;
        for (auto const& ingress : status.AuthIngress)
        {
            auto count = ingress.ConnectionsByNode.find(node.Key);
            if (count != ingress.ConnectionsByNode.end()) connections += count->second;
        }
        json << ",{\"key\":\"cluster:" << JsonEscape(node.Key) << "\",\"name\":\"" << JsonEscape(node.Name)
             << "\",\"status\":\"" << (!node.Live ? "offline" : node.Ready && node.Admin == Skyfire::Cluster::Administration::Enabled ? "online" : "issue")
             << "\",\"detail\":\"Cluster " << (node.Type == Skyfire::Cluster::Service::Auth ? ((node.Capabilities & 16) ? "authnet" : "auth") : "world")
             << " | " << (!node.Live ? "offline" : node.Ready ? "ready" : "not ready")
             << " | policy " << Skyfire::Cluster::AdministrationName(node.Admin);
        if (!node.Address.empty()) json << " | " << JsonEscape(node.Address) << ':' << node.Port;
        if (node.Live) json << " | load " << node.Load << '/' << node.Capacity;
        if (node.Type == Skyfire::Cluster::Service::Auth) json << " | hub connections " << connections;
        if (!node.Realms.empty())
        {
            json << " | realms";
            for (auto realm : node.Realms) json << ' ' << realm;
        }
        json << "\",\"managed\":false,\"clusterKey\":\"" << JsonEscape(node.Key)
             << "\",\"clusterCanAdmin\":" << (node.Type == Skyfire::Cluster::Service::Auth ? "true" : "false")
             << ",\"live\":" << (node.Live ? "true" : "false") << ",\"hubConnections\":" << connections
             << ",\"adminState\":\"" << Skyfire::Cluster::AdministrationName(node.Admin) << "\"}";
    }
    json << "]}";
    return MakeResponse(200, "application/json", json.str());
}

std::string HubWebServer::HandleClusterCommand(std::string const& path, std::map<std::string,std::string> const& headers)
{
    AuthenticatedSession session;
    if (!FindSession(headers,session)) return MakeResponse(401,"application/json","{\"error\":\"Authentication required.\"}");
    if (!(session.AccessFlags & HUB_ADMIN_ACCESS_OPERATE_NODES))
        return MakeResponse(403,"application/json","{\"error\":\"Node operation access is required.\"}");
    auto csrf = headers.find("x-hub-csrf");
    if (csrf == headers.end() || csrf->second != session.CsrfToken)
        return MakeResponse(403,"application/json","{\"error\":\"Invalid request token.\"}");
    auto const route = path.substr(16);
    auto const slash = route.find('/');
    std::string const key = route.substr(0,slash);
    std::string const action = slash == std::string::npos ? "" : route.substr(slash+1);
    if (!Skyfire::Cluster::ValidKey(key) || (action != "enable" && action != "disable" && action != "drain"))
        return MakeResponse(400,"application/json","{\"error\":\"Invalid node key or cluster action.\"}");
    HubWebServiceCommand command;
    command.ServiceKey = key; command.ClusterAction = action; command.Actor = session.Username;
    command.DispatchResult = std::make_shared<std::promise<std::string>>();
    auto result = command.DispatchResult->get_future();
    {
        std::lock_guard<std::mutex> lock(_commandMutex);
        if (_commands.size() >= 64) return MakeResponse(409,"application/json","{\"error\":\"Command queue is busy.\"}");
        _commands.push_back(std::move(command));
    }
    if (result.wait_for(std::chrono::seconds(2)) == std::future_status::ready)
    {
        auto const error = result.get();
        if (!error.empty()) return MakeResponse(409,"application/json","{\"error\":\""+JsonEscape(error)+"\"}");
        return MakeResponse(200,"application/json","{\"saved\":true}");
    }
    return MakeResponse(202,"application/json","{\"accepted\":true}");
}

std::string HubWebServer::HandleServiceCommand(std::string const& path,
    std::map<std::string, std::string> const& headers, std::string const& body)
{
    AuthenticatedSession session;
    if (!FindSession(headers, session))
        return MakeResponse(401, "application/json", "{\"error\":\"authentication required\"}");
    if (!(session.AccessFlags & HUB_ADMIN_ACCESS_OPERATE_NODES))
        return MakeResponse(403, "application/json", "{\"error\":\"service operation access is required\"}");

    auto const csrf = headers.find("x-hub-csrf");
    if (csrf == headers.end() || csrf->second != session.CsrfToken)
        return MakeResponse(403, "application/json", "{\"error\":\"invalid request token\"}");

    std::string const route = path.substr(17);
    size_t const slash = route.find('/');
    if (slash == std::string::npos || slash == 0 || route.find('/', slash + 1) != std::string::npos)
        return MakeResponse(404, "application/json", "{\"error\":\"endpoint not found\"}");
    std::string const serviceKey = route.substr(0, slash);
    std::string const action = route.substr(slash + 1);
    if (action != "start" && action != "stop" && action != "configure" && action != "command")
        return MakeResponse(404, "application/json", "{\"error\":\"endpoint not found\"}");
    if (!std::all_of(serviceKey.begin(), serviceKey.end(), [](unsigned char character)
        { return std::isalnum(character) || character == '_' || character == '-'; }))
        return MakeResponse(400, "application/json", "{\"error\":\"invalid service key\"}");

    bool found = false;
    bool enabled = false;
    bool commandReady = false;
    {
        std::lock_guard<std::mutex> lock(_statusMutex);
        for (HubWebManagedServiceStatus const& service : _status.Services)
        {
            if (service.Key == serviceKey)
            {
                found = true;
                enabled = service.Enabled;
                commandReady = service.State == "running" && service.CanSendCommands && !service.CommandPending;
                break;
            }
        }
    }
    if (!found && action != "configure")
        return MakeResponse(404, "application/json", "{\"error\":\"managed service not found\"}");
    if (!enabled && action != "configure")
        return MakeResponse(403, "application/json", "{\"error\":\"managed service is disabled\"}");

    HubWebServiceCommand command;
    command.ServiceKey = serviceKey;
    command.Start = action == "start";
    std::future<std::string> dispatchResult;
    if (action == "configure")
    {
        if ((session.AccessFlags & HUB_ADMIN_ACCESS_ALL_LOCAL) != HUB_ADMIN_ACCESS_ALL_LOCAL)
            return MakeResponse(403, "application/json", "{\"error\":\"Full local administrator access is required.\"}");
        if (!HubProcessSupervisor::IsWorldKey(serviceKey))
            return MakeResponse(400, "application/json", "{\"error\":\"World keys must be world or world-<id>.\"}");
        auto const form = ParseForm(body);
        auto value = [&](char const* key) { auto it = form.find(key); return it == form.end() ? std::string() : it->second; };
        command.Configure = true;
        command.Name = value("name"); command.ExecutablePath = value("executablePath");
        command.ConfigPath = value("configPath"); command.WorkingDirectory = value("workingDirectory");
        command.DispatchResult = std::make_shared<std::promise<std::string>>();
        dispatchResult = command.DispatchResult->get_future();
    }
    if (action == "command")
    {
        // Arbitrary CLI commands have full console authority, including account management.
        if ((session.AccessFlags & HUB_ADMIN_ACCESS_ALL_LOCAL) != HUB_ADMIN_ACCESS_ALL_LOCAL)
            return MakeResponse(403, "application/json", "{\"error\":\"full local administrator access is required for world commands\"}");
        if (!HubProcessSupervisor::IsWorldKey(serviceKey) || !commandReady)
            return MakeResponse(409, "application/json", "{\"error\":\"worldserver is not ready or a command is pending\"}");
        auto const form = ParseForm(body);
        auto const value = form.find("command");
        if (value == form.end() || value->second.empty() || value->second.size() > 1024 ||
            value->second.find_first_not_of(" .") == std::string::npos ||
            std::any_of(value->second.begin(), value->second.end(), [](unsigned char c) { return c < 32 || c == 127; }))
            return MakeResponse(400, "application/json", "{\"error\":\"enter one command up to 1024 bytes without control characters\"}");
        command.WorldCommand = value->second;
        command.DispatchResult = std::make_shared<std::promise<std::string>>();
        dispatchResult = command.DispatchResult->get_future();
    }
    {
        std::lock_guard<std::mutex> lock(_commandMutex);
        if (_commands.size() >= 64 || (action == "command" &&
            std::any_of(_commands.begin(), _commands.end(), [](HubWebServiceCommand const& queued)
                { return !queued.WorldCommand.empty(); })))
            return MakeResponse(409, "application/json", "{\"error\":\"command queue is busy; try again after the response\"}");
        _commands.push_back(std::move(command));
    }
    if (dispatchResult.valid() && dispatchResult.wait_for(std::chrono::seconds(2)) == std::future_status::ready)
    {
        std::string const error = dispatchResult.get();
        if (!error.empty())
            return MakeResponse(409, "application/json", "{\"error\":\"" + JsonEscape(error) + "\"}");
        if (action == "configure")
            return MakeResponse(200, "application/json", "{\"saved\":true}");
    }
    return MakeResponse(202, "application/json", "{\"accepted\":true}");
}

std::string HubWebServer::HandleAccounts(std::string const& action,
    std::map<std::string, std::string> const& headers, std::string const& body)
{
    using Skyfire::Auth::AccountAdministration;
    AuthenticatedSession session;
    if (!FindSession(headers, session))
        return MakeResponse(401, "application/json", "{\"error\":\"Authentication required.\"}");
    if ((session.AccessFlags & HUB_ADMIN_ACCESS_ALL_LOCAL) != HUB_ADMIN_ACCESS_ALL_LOCAL)
        return MakeResponse(403, "application/json", "{\"error\":\"Full hub administrator access is required.\"}");
    auto csrf = headers.find("x-hub-csrf");
    if (csrf == headers.end() || csrf->second != session.CsrfToken)
        return MakeResponse(403, "application/json", "{\"error\":\"Invalid request token.\"}");
    if (!AccountAdministration::IsEnabled())
        return MakeResponse(503, "application/json", "{\"error\":\"Account administration is disabled. Configure the hub LoginDatabaseInfo and apply auth database updates.\"}");
    auto const form = ParseForm(body);
    if (action == "result")
    {
        auto id = form.find("job");
        std::lock_guard<std::mutex> lock(_accountJobsMutex);
        auto job = id == form.end() ? _accountJobs.end() : _accountJobs.find(id->second);
        if (job == _accountJobs.end() || job->second.OwnerToken != session.CsrfToken)
            return MakeResponse(404, "application/json", "{\"error\":\"Account request not found. Check the account state before retrying.\"}");
        if (job->second.Result.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
            return MakeResponse(202, "application/json", "{\"pending\":true}");
        auto const reply = job->second.Result.get();
        return MakeResponse(reply.Status, "application/json", reply.Body);
    }
    if (AccountAdministration::IsReadAction(action))
    {
        auto const reply = AccountAdministration::Handle(action, form, session.Username);
        return MakeResponse(reply.Status, "application/json", reply.Body);
    }
    if (action != "create" && action != "update" && action != "ban" && action != "unban" &&
        action != "mute" && action != "unmute" && action != "gm" && action != "rbac" && action != "ip-ban" && action != "ip-unban")
        return MakeResponse(404, "application/json", "{\"error\":\"Unknown account action.\"}");
    HubWebServiceCommand command;
    command.ServiceKey = "world";
    command.AccountRequest = AccountAdministration::EncodeRequest(action, form, session.Username);
    if (command.AccountRequest.empty())
        return MakeResponse(400, "application/json", "{\"error\":\"Account request is too large.\"}");
    command.AccountResult = std::make_shared<std::promise<Skyfire::Auth::AccountAdminReply>>();
    AccountJob job;
    job.OwnerToken = session.CsrfToken;
    job.Result = command.AccountResult->get_future().share();
    auto const now = std::chrono::steady_clock::now();
    job.ExpiresAt = now + std::chrono::minutes(10);
    std::string const id = ByteArrayToHexStr(SkyFire::Crypto::GetRandomBytes<16>());
    {
        std::lock_guard<std::mutex> jobsLock(_accountJobsMutex);
        for (auto it = _accountJobs.begin(); it != _accountJobs.end();)
            if (it->second.ExpiresAt <= now) it = _accountJobs.erase(it); else ++it;
        if (_accountJobs.size() >= 64)
            return MakeResponse(429, "application/json", "{\"error\":\"Account request limit reached. Wait before sending more changes.\"}");
        std::lock_guard<std::mutex> commandLock(_commandMutex);
        if (_commands.size() >= 64)
            return MakeResponse(409, "application/json", "{\"error\":\"Hub command queue is busy.\"}");
        _accountJobs.emplace(id, std::move(job));
        _commands.push_back(std::move(command));
    }
    return MakeResponse(202, "application/json", "{\"job\":\"" + id + "\"}");
}

std::string HubWebServer::ServeAsset(std::string const& target) const
{
    std::string relativePath;
    if (target == "/" || target == "/index.html")
        relativePath = "index.html";
    else if (target == "/app.css")
        relativePath = "app.css";
    else if (target == "/app.js")
        relativePath = "app.js";
    else if (target == "/status.js")
        relativePath = "status.js";
    else if (target == "/accounts.js")
        relativePath = "accounts.js";
    else if (target == "/assets/skyfire-logo.png")
        relativePath = "assets/skyfire-logo.png";
    else if (target == "/favicon.ico")
        relativePath = "favicon.ico";
    else
        return MakeResponse(404, "text/plain; charset=utf-8", "Not found");

    std::string const path = _webRoot + '/' + relativePath;
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file)
    {
        SF_LOG_ERROR("server.hub", "Unable to serve hub web asset %s.", path.c_str());
        return MakeResponse(404, "text/plain; charset=utf-8", "Web asset not found");
    }

    std::ostringstream contents;
    contents << file.rdbuf();
    return MakeResponse(200, GetContentType(relativePath), contents.str());
}

bool HubWebServer::FindSession(std::map<std::string, std::string> const& headers,
    AuthenticatedSession& session)
{
    std::string const token = GetCookie(headers, "hub_session");
    if (token.empty())
        return false;

    auto const now = std::chrono::steady_clock::now();
    std::lock_guard<std::mutex> lock(_authMutex);
    PurgeExpiredSessionsLocked(now);
    auto const found = _sessions.find(token);
    if (found == _sessions.end())
        return false;

    found->second.ExpiresAt = now + std::chrono::seconds(_sessionTimeoutSeconds);
    session = found->second;
    return true;
}

void HubWebServer::PurgeExpiredSessionsLocked(std::chrono::steady_clock::time_point now)
{
    for (auto iterator = _sessions.begin(); iterator != _sessions.end();)
    {
        if (iterator->second.ExpiresAt <= now)
            iterator = _sessions.erase(iterator);
        else
            ++iterator;
    }
}
