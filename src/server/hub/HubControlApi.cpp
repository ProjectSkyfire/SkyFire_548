/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "HubBackupGuard.h"
#include "HubWebServer.h"
#include "HubControlProtocol.h"
#include "HubBackupSchedule.h"
#include "HubProcessSupervisor.h"
#include "Cluster/RealmDirectory.h"
#include "Cryptography/CryptoRandom.h"
#include "Database/DatabaseEnv.h"
#include "Utilities/Util.h"
#include "Log.h"
#include <openssl/crypto.h>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

namespace
{
    std::string Quote(std::string const& value)
    {
        std::ostringstream out; out << '"';
        for (unsigned char c : value)
            if (c == '"' || c == '\\') out << '\\' << c;
            else if (c < 32) out << "\\u" << std::hex << std::setw(4) << std::setfill('0') << unsigned(c);
            else out << c;
        out << '"'; return out.str();
    }
    std::string Response(int status, std::string const& body)
    {
        return "HTTP/1.1 " + std::to_string(status) + " Control\r\nContent-Type: application/json\r\nCache-Control: no-store\r\nConnection: close\r\nContent-Length: " +
            std::to_string(body.size()) + "\r\n\r\n" + body;
    }
    std::string Error(int status, char const* message) { return Response(status,"{\"error\":"+Quote(message)+"}"); }
    std::string Header(std::map<std::string,std::string> const& headers, char const* name)
    { auto found = headers.find(name); return found == headers.end() ? "" : found->second; }
    bool Form(std::string const& body, std::map<std::string,std::string>& result, std::size_t limit = 4)
    {
        if (body.empty() || body.size() > 4096) return false;
        auto decode = [](std::string const& value, std::string& out)
        {
            auto hex = [](unsigned char c) { return c >= '0' && c <= '9' ? c-'0' : c >= 'a' && c <= 'f' ? c-'a'+10 : c >= 'A' && c <= 'F' ? c-'A'+10 : -1; };
            for (std::size_t i = 0; i < value.size(); ++i)
            {
                unsigned char c = value[i];
                if (c == '%')
                {
                    if (i + 2 >= value.size() || hex(value[i+1]) < 0 || hex(value[i+2]) < 0) return false;
                    c = static_cast<unsigned char>((hex(value[i+1]) << 4) | hex(value[i+2])); i += 2;
                }
                else if (c == '+') c = ' ';
                if (c < 32 || c > 126) return false;
                out += char(c);
            }
            return true;
        };
        std::size_t start = 0;
        do
        {
            auto end = body.find('&',start); auto pair = body.substr(start,end == std::string::npos ? end : end-start);
            auto equal = pair.find('='); std::string key,value;
            if (equal == std::string::npos || !decode(pair.substr(0,equal),key) || !decode(pair.substr(equal+1),value) ||
                !result.emplace(key,value).second || result.size() > limit) return false;
            if (end == std::string::npos) break;
            start = end+1;
        } while (start <= body.size());
        return true;
    }
}

std::string HubWebServer::HandleControl(std::string const& method, std::string const& path,
    std::map<std::string,std::string> const& headers, std::string const& body, bool loopback)
{
    auto token = Header(headers,"x-skyfire-control");
    if (!loopback || _controlToken.size() != 64 || token.size() != _controlToken.size() ||
        CRYPTO_memcmp(token.data(),_controlToken.data(),token.size())) return Error(403,"Control gateway authentication required.");
    auto action = path.substr(21);
    if (method == "POST" && action == "session")
    {
        auto address = Header(headers,"x-skyfire-client"); boost::system::error_code ec;
        auto parsed = boost::asio::ip::make_address(address,ec);
        if (ec || parsed.is_unspecified() || parsed.is_multicast()) return Error(400,"Concrete client address required.");
        return HandleLogin(body,"control:"+parsed.to_string(),false);
    }
    if (action == "backup/jobs") return HandleBackupJobs(method,headers,body,true);
    if (action == "backup/schedules") return HandleBackupSchedule(method,headers,body,true);
    if (method == "POST" && action == "commands") return ControlCommand(headers,body);
    AuthenticatedSession session;
    if (!FindSession(headers,session) || !session.Remote || !(session.AccessFlags & Skyfire::Control::Remote))
        return Error(401,"Remote session expired or revoked.");
    if (method == "POST" && action == "logout") return HandleLogout(headers);
    if (!(session.AccessFlags & Skyfire::Control::View)) return Error(403,"Viewer permission required.");
    if (method == "GET" && action == "status") return ControlStatus(session);
    if (method == "GET" && action.compare(0,9,"commands/") == 0)
    {
        auto id = action.substr(9);
        if (!Skyfire::Control::RequestId(id)) return Error(400,"Invalid request identifier.");
        std::lock_guard<std::mutex> lock(_controlMutex);
        auto found = _controlResults.find(id);
        if (found == _controlResults.end() || found->second.Owner != session.Username || found->second.ExpiresAt <= std::chrono::steady_clock::now())
            return Error(404,"Result unavailable; inspect server state before submitting a new command.");
        return Response(200,"{\"requestId\":"+Quote(id)+",\"state\":"+Quote(found->second.State)+"}");
    }
    return Error(404,"Unsupported control endpoint.");
}

bool HubWebServer::AuditControl(std::string const& id, std::string const& phase, std::string const& actor,
    std::string const& action, std::string const& target, std::string const& outcome)
{
    auto insert = HubDatabase.GetPreparedStatement(HUB_INS_CONTROL_AUDIT);
    insert->setString(0,id); insert->setString(1,phase); insert->setString(2,actor);
    insert->setString(3,action); insert->setString(4,target); insert->setString(5,outcome);
    HubDatabase.DirectExecute(insert);
    auto select = HubDatabase.GetPreparedStatement(HUB_SEL_CONTROL_AUDIT); select->setString(0,id);
    if (auto rows = HubDatabase.Query(select)) do
    {
        auto fields = rows->Fetch();
        if (fields[0].GetString() == phase && fields[1].GetString() == actor && fields[2].GetString() == outcome) return true;
    } while (rows->NextRow());
    SF_LOG_ERROR("server.hub","Control audit persistence failed for request %s phase %s.",id.c_str(),phase.c_str());
    return false;
}

std::string HubWebServer::ControlCommand(std::map<std::string,std::string> const& headers, std::string const& body)
{
    using namespace Skyfire::Control;
    std::unique_lock<std::mutex> admission(_controlAdmissionMutex);
    std::map<std::string,std::string> form; Command decoded;
    bool const valid = Form(body,form) && Decode(form,decoded);
    auto id = form["id"];
    if (!RequestId(id))
    {
        id = ByteArrayToHexStr(SkyFire::Crypto::GetRandomBytes<16>());
        std::transform(id.begin(),id.end(),id.begin(),[](unsigned char c) { return char(std::tolower(c)); });
    }
    auto existing = HubDatabase.GetPreparedStatement(HUB_SEL_CONTROL_AUDIT); existing->setString(0,id);
    if (HubDatabase.Query(existing)) return Error(409,"Request identifier already used; inspect the existing result or server state. It was not executed again.");
    AuthenticatedSession session;
    bool const authenticated = FindSession(headers,session) && session.Remote && (session.AccessFlags & Remote);
    auto actor = authenticated ? session.Username : "-";
    auto action = valid ? decoded.Action : "invalid"; auto target = valid ? decoded.Target : "-";
    if (!AuditControl(id,"attempt",actor,action,target,"received")) return Error(503,"Audit storage unavailable; no command dispatched.");
    auto deny = [&](int status, char const* outcome, char const* message)
    {
        if (!AuditControl(id,"rejected",actor,action,target,outcome)) return Error(503,"Audit storage unavailable; no command dispatched.");
        return Error(status,message);
    };
    if (!authenticated) return deny(401,"authentication","Remote session expired or revoked.");
    if (!(session.AccessFlags & Operate)) return deny(403,"permission","Operator permission required.");
    if (Header(headers,"x-hub-csrf") != session.CsrfToken) return deny(403,"csrf","Invalid request token.");
    if (!valid) return deny(400,"invalid","Unsupported typed command or invalid parameters.");
    if (decoded.ClusterAction.empty())
    {
        std::lock_guard<std::mutex> lock(_statusMutex);
        auto service = std::find_if(_status.Services.begin(),_status.Services.end(),[&](auto const& s) { return s.Key == decoded.Target; });
        if (service == _status.Services.end() || !service->Enabled)
            return deny(404,"not_allowed","Service is not in the enabled local supervisor allowlist.");
        if (!decoded.WorldCommand.empty() && (!service->IsWorld || !service->CanSendCommands || service->State != "running" || service->CommandPending))
            return deny(409,"not_ready","Worldserver is not ready for a lifecycle command.");
    }
    {
        std::lock_guard<std::mutex> lock(_controlMutex);
        auto now = std::chrono::steady_clock::now();
        for (auto it = _controlResults.begin(); it != _controlResults.end();)
            if (now >= it->second.ExpiresAt) it = _controlResults.erase(it); else ++it;
        if (_controlResults.size() >= 512) return deny(429,"capacity","Command result capacity reached.");
    }
    HubWebServiceCommand command;
    command.ServiceKey = decoded.Target; command.Actor = session.Username; command.Start = decoded.Start;
    command.WorldCommand = decoded.WorldCommand; command.ClusterAction = decoded.ClusterAction;
    command.CorrelationId = id; command.ControlAction = decoded.Action;
    command.DispatchResult = std::make_shared<std::promise<std::string>>(); auto result = command.DispatchResult->get_future();
    {
        std::lock_guard<std::mutex> lock(_commandMutex);
        if (_commands.size() >= 64) return deny(429,"busy","Command queue is full.");
        if (!AuditControl(id,"admitted",actor,action,target,"queued")) return Error(503,"Audit storage unavailable; no command dispatched.");
        { std::lock_guard<std::mutex> resultLock(_controlMutex); _controlResults[id] = {actor,"queued",std::chrono::steady_clock::now()+std::chrono::minutes(10)}; }
        _commands.push_back(std::move(command));
    }
    admission.unlock();
    result.wait_for(std::chrono::seconds(2));
    std::lock_guard<std::mutex> lock(_controlMutex);
    return Response(202,"{\"requestId\":"+Quote(id)+",\"state\":"+Quote(_controlResults[id].State)+"}");
}

void HubWebServer::CompleteControlCommand(HubWebServiceCommand const& command, bool accepted)
{
    if (command.CorrelationId.empty()) return;
    std::string state = accepted ? "dispatched" : "rejected";
    if (!AuditControl(command.CorrelationId,"dispatch",command.Actor,command.ControlAction,command.ServiceKey,state)) state = "unknown";
    std::lock_guard<std::mutex> lock(_controlMutex);
    auto found = _controlResults.find(command.CorrelationId);
    if (found != _controlResults.end()) found->second.State = state;
}

std::string HubWebServer::ControlStatus(AuthenticatedSession const& session)
{
    using namespace Skyfire;
    HubWebStatusSnapshot status;
    { std::lock_guard<std::mutex> lock(_statusMutex); status = _status; }
    std::ostringstream out;
    out << "{\"username\":" << Quote(session.Username) << ",\"csrf\":" << Quote(session.CsrfToken)
        << ",\"permissions\":{\"role\":" << Quote(Control::Role(session.AccessFlags))
        << ",\"operate\":" << ((session.AccessFlags & Control::Operate) ? "true" : "false")
        << ",\"recovery\":" << ((session.AccessFlags & Control::Recovery) ? "true" : "false") << "},"
        << "\"hub\":{\"uptimeSeconds\":" << status.UptimeSeconds << "},\"services\":[";
    bool comma = false;
    for (auto const& service : status.Services)
    {
        if (comma) out << ','; comma = true;
        out << "{\"key\":" << Quote(service.Key) << ",\"name\":" << Quote(service.Name)
            << ",\"state\":" << Quote(service.State) << ",\"enabled\":" << (service.Enabled ? "true" : "false")
            << ",\"world\":" << (service.IsWorld ? "true" : "false") << ",\"uptimeSeconds\":" << service.UptimeSeconds
            << ",\"metricsAvailable\":" << (service.MetricsAvailable ? "true" : "false") << ",\"players\":" << service.Players
            << ",\"updateTimeMs\":" << service.UpdateTimeMs << ",\"cpuBasisPoints\":" << service.CpuBasisPoints << '}';
    }
    out << "],\"nodes\":["; comma = false;
    auto now = Cluster::Realms::Now();
    for (auto const& node : status.ClusterNodes)
    {
        if (comma) out << ','; comma = true;
        bool live = node.Live && now < node.ExpiresAt;
        std::uint64_t connections = 0;
        for (auto const& route : status.AuthIngress)
        {
            auto found = route.ConnectionsByNode.find(node.Key);
            if (found != route.ConnectionsByNode.end()) connections += found->second;
        }
        out << "{\"key\":" << Quote(node.Key) << ",\"name\":" << Quote(node.Name)
            << ",\"service\":" << Quote(node.Type == Cluster::Service::World ? "world" : node.Type == Cluster::Service::Map ? "mapserver" : "auth")
            << ",\"live\":" << (live ? "true" : "false") << ",\"ready\":" << (live && node.Ready ? "true" : "false")
            << ",\"policy\":" << Quote(Cluster::AdministrationName(node.Admin)) << ",\"load\":" << node.Load
            << ",\"hubConnections\":" << connections << ",\"capacity\":" << node.Capacity << ",\"leaseSeconds\":" << (live ? (node.ExpiresAt-now)/1000 : 0)
            << ",\"realms\":[";
        bool realmComma = false; for (auto realm : node.Realms) { if (realmComma) out << ','; realmComma = true; out << realm; }
        out << "]}";
    }
    out << "],\"realms\":["; comma = false;
    if (Auth::AccountAdministration::IsEnabled())
        if (auto rows = LoginDatabase.Query(LoginDatabase.GetPreparedStatement(LOGIN_SEL_CLUSTER_REALMLIST))) do
        {
            auto fields = rows->Fetch();
            auto route = Cluster::Realms::Resolve({fields[0].GetUInt32(),fields[11].GetUInt32()},status.ClusterNodes,now,true);
            bool available = !(fields[7].GetUInt8() & 3) && route.State == Cluster::Realms::Status::Ready;
            if (comma) out << ','; comma = true;
            out << "{\"id\":" << fields[0].GetUInt32() << ",\"name\":" << Quote(fields[1].GetString())
                << ",\"state\":" << Quote((fields[7].GetUInt8() & 3) ? "offline" : Cluster::Realms::Name(route.State))
                << ",\"locked\":" << (fields[9].GetUInt8() ? "true" : "false")
                << ",\"address\":" << Quote(available ? route.Address : "") << ",\"port\":" << (available ? route.Port : 0) << '}';
        } while (rows->NextRow());
    out << "],\"routes\":["; comma = false;
    for (auto const& route : status.AuthIngress)
    {
        if (comma) out << ','; comma = true;
        out << "{\"name\":" << Quote(route.Name) << ",\"enabled\":" << (route.Enabled ? "true" : "false")
            << ",\"active\":" << route.Active << ",\"accepted\":" << route.Accepted << ",\"routed\":" << route.Routed
            << ",\"rejected\":" << route.Rejected << ",\"failures\":" << route.ConnectFailures
            << ",\"eligibleNodes\":" << route.AvailableNodes << '}';
    }
    auto worker = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_WORKER));
    out << "],\"backup\":{\"available\":" << (worker && worker->Fetch()[0].GetUInt64()!=0 ? "true" : "false")
        << ",\"scheduled\":false,\"jobs\":[]}}";
    return Response(200,out.str());
}

std::string HubWebServer::BackupSchedules(bool canEdit)
{
    auto rows = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_SCHEDULES));
    if (!rows || rows->GetRowCount() != 4) return Error(503,"Backup schedule storage unavailable; apply the hub migration.");
    auto worker = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_WORKER));
    bool const available = worker && worker->Fetch()[0].GetUInt64()!=0;
    std::ostringstream out;
    out << "{\"available\":" << (available ? "true" : "false") << ",\"scheduleConfigurationAvailable\":true,\"canEdit\":" << (canEdit ? "true" : "false")
        << ",\"timezone\":\"per_schedule\",\"executionState\":\"hub_managed_maintenance\",\"schedules\":[";
    bool comma = false;
    do
    {
        auto f = rows->Fetch(); if (comma) out << ','; comma = true;
        out << "{\"target\":" << Quote(f[0].GetString()) << ",\"enabled\":" << unsigned(f[1].GetUInt8())
            << ",\"mode\":" << Quote(f[2].GetString()) << ",\"intervalMinutes\":" << f[3].GetUInt32()
            << ",\"minuteOfDay\":" << f[4].GetUInt32() << ",\"weekday\":" << unsigned(f[5].GetUInt8())
            << ",\"revision\":" << f[6].GetUInt32() << ",\"timeZone\":" << Quote(f[8].GetString()) << '}';
    } while (rows->NextRow());
    out << "]}"; return Response(200,out.str());
}

std::string HubWebServer::HandleBackupSchedule(std::string const& method,
    std::map<std::string,std::string> const& headers, std::string const& body, bool remote)
{
    using namespace Skyfire::Control;
    AuthenticatedSession session;
    bool authenticated = FindSession(headers,session) && (!remote || (session.Remote && (session.AccessFlags & Remote)));
    bool canEdit = authenticated && (session.AccessFlags & Operate) != 0;
    if (method == "GET")
    {
        if (!authenticated) return Error(401,"Authentication required.");
        if (!(session.AccessFlags & View)) return Error(403,"Viewer permission required.");
        return BackupSchedules(canEdit);
    }
    if (method != "POST") return Error(405,"Use GET or POST.");
    if (HubNodeRestartActive) return Error(409,"Server node restart is in progress.");
    std::lock_guard<std::mutex> lock(_controlAdmissionMutex);
    std::map<std::string,std::string> form; Skyfire::Backup::Schedule schedule;
    bool valid = Form(body,form,9) && Skyfire::Backup::Decode(form,schedule);
    auto id = form["id"];
    if (!RequestId(id))
    {
        id = ByteArrayToHexStr(SkyFire::Crypto::GetRandomBytes<16>());
        std::transform(id.begin(),id.end(),id.begin(),[](unsigned char c) { return char(std::tolower(c)); });
    }
    auto previous = HubDatabase.GetPreparedStatement(HUB_SEL_CONTROL_AUDIT); previous->setString(0,id);
    if (HubDatabase.Query(previous)) return Error(409,"Request already used. Reload the saved schedule before retrying.");
    std::string actor = authenticated ? session.Username : "-", target = valid ? schedule.Target : "-";
    if (!AuditControl(id,"attempt",actor,"backup.schedule",target,"received")) return Error(503,"Audit storage unavailable; schedule unchanged.");
    auto deny = [&](int status, char const* outcome, char const* message)
    {
        if (!AuditControl(id,"rejected",actor,"backup.schedule",target,outcome)) return Error(503,"Audit storage unavailable.");
        return Error(status,message);
    };
    if (!authenticated) return deny(401,"authentication","Authentication required.");
    if (!canEdit) return deny(403,"permission","Operator permission required to change backup schedules.");
    if (Header(headers,"x-hub-csrf") != session.CsrfToken) return deny(403,"csrf","Invalid request token.");
    if (!valid) return deny(400,"invalid","Invalid backup schedule. Use interval (15..10080 minutes), daily, or weekly; time zone must be UTC or server.");
    auto update = HubDatabase.GetPreparedStatement(HUB_UPD_BACKUP_SCHEDULE);
    update->setUInt8(0,uint8(schedule.Enabled)); update->setString(1,schedule.Mode);
    update->setUInt32(2,schedule.IntervalMinutes); update->setUInt32(3,schedule.MinuteOfDay);
    update->setUInt8(4,uint8(schedule.Weekday)); update->setString(5,id); update->setString(6,actor);
    update->setString(7,schedule.TimeZone); update->setString(8,schedule.Target); update->setUInt32(9,schedule.Revision);
    HubDatabase.DirectExecute(update);
    auto rows = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_SCHEDULES));
    if (!rows) return Error(503,"Cannot verify save outcome. Reload before retrying.");
    bool saved = false;
    do
    {
        auto f = rows->Fetch();
        if (f[0].GetString() == schedule.Target && f[7].GetString() == id && f[6].GetUInt32() == schedule.Revision+1)
            saved = true;
    } while (rows->NextRow());
    if (!saved) return deny(409,"conflict","Schedule changed or could not be saved. Reload its current settings before retrying.");
    if (!AuditControl(id,"applied",actor,"backup.schedule",target,"saved")) return Error(503,"Schedule may be saved, but audit verification failed. Reload before retrying.");
    return BackupSchedules(true);
}

std::string HubWebServer::BackupJobs(bool administrator)
{
    auto worker = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_WORKER));
    if (!worker) return Error(503,"Backup storage unavailable; apply the hub migration.");
    auto f = worker->Fetch();
    std::ostringstream out;
    out << "{\"available\":" << (f[0].GetUInt64()!=0 ? "true" : "false")
        << ",\"canRestore\":" << (administrator && f[8].GetBool() ? "true" : "false")
        << ",\"archiveBytes\":" << f[9].GetUInt64() << ",\"archiveCount\":" << f[10].GetUInt32()
        << ",\"storageTotal\":" << f[2].GetUInt64() << ",\"storageFree\":" << f[3].GetUInt64()
        << ",\"maintenance\":" << (f[4].GetBool() ? "true" : "false") << ",\"recoverySafe\":" << (f[5].GetBool() ? "true" : "false")
        << ",\"automation\":" << Quote(f[11].GetString())
        << ",\"canRun\":" << (administrator ? "true" : "false") << ",\"targets\":" << Quote(f[1].GetString()) << ",\"jobs\":[";
    auto rows = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_JOBS));
    bool comma = false;
    if (rows) do
    {
        auto row = rows->Fetch(); if (comma) out << ','; comma = true;
        out << "{\"id\":" << Quote(row[0].GetString()) << ",\"target\":" << Quote(row[1].GetString())
            << ",\"actor\":" << Quote(row[2].GetString()) << ",\"state\":" << Quote(row[3].GetString())
            << ",\"createdAt\":" << row[4].GetUInt64() << ",\"finishedAt\":" << row[5].GetUInt64()
            << ",\"bytes\":" << row[6].GetUInt64() << ",\"sha256\":" << Quote(row[7].GetString())
            << ",\"message\":" << Quote(row[8].GetString()) << ",\"kind\":" << Quote(row[9].GetString()) << ",\"sourceId\":" << Quote(row[10].GetString())
            << ",\"verifiedAt\":" << row[11].GetUInt64() << ",\"verificationSeconds\":" << row[12].GetUInt32()
            << ",\"pinned\":" << (row[13].GetBool() ? "true" : "false") << '}';
    } while (rows->NextRow());
    out << "]}"; return Response(200,out.str());
}

std::string HubWebServer::HandleBackupJobs(std::string const& method,
    std::map<std::string,std::string> const& headers, std::string const& body, bool remote)
{
    using namespace Skyfire::Control;
    AuthenticatedSession session;
    if (!FindSession(headers,session) || (remote && (!session.Remote || !(session.AccessFlags & Remote))))
        return Error(401,"Authentication required.");
    bool administrator = (session.AccessFlags & Administrator) == Administrator;
    if (!(session.AccessFlags & View)) return Error(403,"Viewer permission required.");
    if (method == "GET") return BackupJobs(administrator);
    if (method != "POST") return Error(405,"Use GET or POST.");
    if (HubNodeRestartActive) return Error(409,"Server node restart is in progress.");
    if (!administrator) return Error(403,"Administrator permission required to run backups.");
    if (Header(headers,"x-hub-csrf") != session.CsrfToken) return Error(403,"Invalid request token.");
    std::map<std::string,std::string> form;
    if (!Form(body,form,4) || !RequestId(form["id"])) return Error(400,"Invalid backup request.");
    if (form.count("pin"))
    {
        if (form.size()!=3 || !RequestId(form["archiveId"]) || (form["pin"]!="0" && form["pin"]!="1"))
            return Error(400,"Invalid archive protection request.");
        std::lock_guard<std::mutex> lock(_controlAdmissionMutex);
        auto previous = HubDatabase.GetPreparedStatement(HUB_SEL_CONTROL_AUDIT); previous->setString(0,form["id"]);
        if (HubDatabase.Query(previous)) return Error(409,"Request id already used.");
        if (!AuditControl(form["id"],"attempt",session.Username,"backup.pin",form["archiveId"],form["pin"]))
            return Error(503,"Audit unavailable.");
        auto update = HubDatabase.GetPreparedStatement(HUB_UPD_BACKUP_PIN);
        update->setUInt8(0,form["pin"]=="1" ? 1 : 0); update->setString(1,form["archiveId"]);
        HubDatabase.DirectExecute(update);
        auto verify = HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_JOB); verify->setString(0,form["archiveId"]);
        auto row = HubDatabase.Query(verify);
        if (!row || row->Fetch()[5].GetString()!="completed" || row->Fetch()[4].GetBool()!=(form["pin"]=="1"))
        {
            AuditControl(form["id"],"rejected",session.Username,"backup.pin",form["archiveId"],"unavailable");
            return Error(409,"Archive is unavailable or retention has already started. Refresh history.");
        }
        AuditControl(form["id"],"applied",session.Username,"backup.pin",form["archiveId"],"processed");
        return BackupJobs(true);
    }
    bool restore = form.count("sourceId") != 0, release = form.count("release") != 0;
    if (release)
    {
        if (form.size()!=2 || form["release"]!="maintenance") return Error(400,"Invalid maintenance request.");
        std::lock_guard<std::mutex> backupLock(HubBackupAdmission);
        if (HubNodeRestartActive) return Error(409,"Server node restart is in progress.");
        auto previous = HubDatabase.GetPreparedStatement(HUB_SEL_CONTROL_AUDIT); previous->setString(0,form["id"]);
        if (HubDatabase.Query(previous)) return Error(409,"Request id already used.");
        if (!AuditControl(form["id"],"attempt",session.Username,"backup.release","hub","received")) return Error(503,"Audit unavailable.");
        HubDatabase.DirectExecute("UPDATE hub_backup_worker SET maintenance=0 WHERE id=1 AND recovery_safe=1 AND services_stopped=1 AND NOT EXISTS(SELECT 1 FROM hub_backup_jobs WHERE state IN ('queued','running')) AND NOT EXISTS(SELECT 1 FROM hub_backup_cycles WHERE active_slot=1)");
        if (HubBackupMaintenance()) return Error(409,"Recovery is unfinished or failed. Resolve it before ending maintenance.");
        AuditControl(form["id"],"applied",session.Username,"backup.release","hub","released");
        return BackupJobs(true);
    }
    if ((restore && (form.size()!=4 || !RequestId(form["sourceId"]) || form["confirm"]!="RESTORE " + form["target"])) ||
        (!restore && form.size()!=2) ||
        (form["target"]!="auth" && form["target"]!="characters" && form["target"]!="world" && form["target"]!="hub"))
        return Error(400,"Choose an allowlisted target; restore requires its source id and typed confirmation.");
    std::lock_guard<std::mutex> backupLock(HubBackupAdmission);
    if (HubNodeRestartActive) return Error(409,"Server node restart is in progress.");
    std::lock_guard<std::mutex> lock(_controlAdmissionMutex);
    auto existing = HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_JOB); existing->setString(0,form["id"]);
    if (auto row = HubDatabase.Query(existing))
    {
        auto f = row->Fetch();
        if (f[0].GetString()!=form["target"] || f[1].GetString()!=session.Username ||
            f[2].GetString()!=(restore ? "restore" : "backup") || (restore && f[3].GetString()!=form["sourceId"]))
            return Error(409,"Request id already used.");
        return BackupJobs(true);
    }
    auto previous = HubDatabase.GetPreparedStatement(HUB_SEL_CONTROL_AUDIT); previous->setString(0,form["id"]);
    if (HubDatabase.Query(previous)) return Error(409,"Request id already used.");
    std::string action = restore ? "backup.restore" : "backup.now";
    if (!AuditControl(form["id"],"attempt",session.Username,action,form["target"],"received"))
        return Error(503,"Audit storage unavailable; job not queued.");
    auto worker = HubDatabase.Query(HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_WORKER));
    if (!worker || worker->Fetch()[0].GetUInt64()==0) return Error(503,"Backup worker is offline.");
    auto state = worker->Fetch();
    if (restore)
    {
        if (form["target"]=="hub") return Error(409,"Hub database recovery requires the offline recovery command.");
        if (("," + state[1].GetString() + ",").find("," + form["target"] + ",") == std::string::npos)
            return Error(409,"Restore target is not configured in the backup worker.");
        if (!state[8].GetBool()) return Error(409,"Live restore is disabled in the backup worker configuration.");
        if (!state[6].GetBool() || state[7].GetUInt64()==0 || state[4].GetBool())
            return Error(409,"Gracefully stop all game services and cluster nodes before restoring; existing maintenance must be resolved.");
        auto source = HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_JOB); source->setString(0,form["sourceId"]);
        auto sourceRow = HubDatabase.Query(source);
        if (!sourceRow || sourceRow->Fetch()[0].GetString()!=form["target"]) return Error(400,"Backup source does not match target.");
        // Persist maintenance before job admission. A crash stays fenced until an administrator releases it.
        HubDatabase.DirectExecute("UPDATE hub_backup_worker SET maintenance=1,recovery_safe=1 WHERE id=1");
        if (!HubBackupMaintenance()) return Error(503,"Cannot establish recovery maintenance.");
        auto insert = HubDatabase.GetPreparedStatement(HUB_INS_RESTORE_JOB);
        insert->setString(0,form["id"]); insert->setString(1,session.Username); insert->setString(2,form["sourceId"]);
        HubDatabase.DirectExecute(insert);
    }
    else
    {
        if (state[4].GetBool()) return Error(409,"Recovery maintenance blocks new backups.");
        auto insert = HubDatabase.GetPreparedStatement(HUB_INS_BACKUP_JOB);
        insert->setString(0,form["id"]); insert->setString(1,form["target"]);
        insert->setString(2,session.Username); insert->setString(3,form["target"]); HubDatabase.DirectExecute(insert);
    }
    auto verify = HubDatabase.GetPreparedStatement(HUB_SEL_BACKUP_JOB); verify->setString(0,form["id"]);
    if (!HubDatabase.Query(verify))
    {
        AuditControl(form["id"],"rejected",session.Username,action,form["target"],"unavailable_or_busy");
        return Error(409,"Target unavailable or another job is active. Refresh history; release maintenance if necessary.");
    }
    AuditControl(form["id"],"queued",session.Username,action,form["target"],"queued");
    return BackupJobs(true);
}
