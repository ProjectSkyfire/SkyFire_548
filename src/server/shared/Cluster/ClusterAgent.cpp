/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "ClusterAgent.h"
#include "CertificateTools.h"
#include <ctime>
#include "ChatProtocol.h"
#include "HandoffClient.h"
#include "RealmDirectory.h"
#include "Configuration/Config.h"
#include "Log.h"
#include <boost/asio/connect.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/ssl.h>
#include <filesystem>
#include <chrono>
#include <algorithm>
#include <atomic>

namespace Skyfire::Cluster
{
    bool LoadAgentOptions(Node advertisement, AgentOptions& options, std::string& error)
    {
        options.Enabled = sConfigMgr->GetBoolDefault("Cluster.Enable", false);
        options.RealmDirectoryEnabled = sConfigMgr->GetBoolDefault("Cluster.RealmDirectory.Enable", false);
        Realms::Client.Configure(options.RealmDirectoryEnabled);
        options.HandoffEnabled = sConfigMgr->GetBoolDefault("Cluster.Handoff.Enable", false);
        if (!options.Enabled)
        {
            if (options.HandoffEnabled || options.RealmDirectoryEnabled) { error = "Cluster handoff and realm directory require Cluster.Enable."; return false; }
            Handoff::ConfigureClient(options); return true;
        }
        if (options.RealmDirectoryEnabled && !options.HandoffEnabled)
        { error = "Cluster.RealmDirectory.Enable requires Cluster.Handoff.Enable to bind selected world routes."; return false; }
        if (options.RealmDirectoryEnabled && advertisement.Type != Service::Auth)
        { error = "Cluster.RealmDirectory.Enable is an authserver option."; return false; }
        options.Host = sConfigMgr->GetStringDefault("Cluster.HubHost", "localhost");
        int const port = sConfigMgr->GetIntDefault("Cluster.HubPort", 9100);
        int const capacity = sConfigMgr->GetIntDefault("Cluster.Capacity", 0);
        advertisement.Key = sConfigMgr->GetStringDefault("Cluster.NodeKey", "");
        advertisement.Name = sConfigMgr->GetStringDefault("Cluster.NodeName", advertisement.Key);
        if (advertisement.Name.empty()) advertisement.Name = advertisement.Key;
        advertisement.Address = sConfigMgr->GetStringDefault("Cluster.AdvertiseAddress", "");
        if (port < 1 || port > 65535 || capacity < 0 || options.Host.empty() || options.Host.size() > 255 ||
            !ValidUtf8(options.Host) || !ValidKey(advertisement.Key) || advertisement.Name.empty() ||
            advertisement.Name.size() > 100 || !ValidUtf8(advertisement.Name) || !advertisement.Port ||
            (advertisement.Type == Service::World && (advertisement.Realms.empty() || advertisement.Realms.size() > 64)))
        { error = "Invalid cluster identity, endpoint, realm list or capacity configuration."; return false; }
        boost::system::error_code ec;
        auto const address = boost::asio::ip::make_address(advertisement.Address, ec);
        if (ec || address.is_unspecified() || address.is_multicast())
        { error = "Cluster.AdvertiseAddress must be a concrete numeric address reachable by clients."; return false; }
        try
        {
            auto const directory = std::filesystem::absolute(sConfigMgr->GetFilename()).parent_path();
            auto path = [&](char const* key)
            {
                std::filesystem::path value = sConfigMgr->GetStringDefault(key, "");
                if (value.empty()) return std::string();
                if (value.is_relative()) value = directory / value;
                return value.lexically_normal().string();
            };
            options.Certificate = path("Cluster.Certificate"); options.PrivateKey = path("Cluster.PrivateKey");
            options.CA = path("Cluster.CA"); options.CRL = path("Cluster.CRL");
        }
        catch (std::filesystem::filesystem_error const&) { error = "Cannot resolve cluster certificate paths."; return false; }
        if (options.Certificate.empty() || options.PrivateKey.empty() || options.CA.empty())
        { error = "Cluster mode requires a client certificate, private key and trusted hub CA."; return false; }
        advertisement.Capacity = std::uint32_t(capacity);
        if (options.HandoffEnabled) advertisement.Capabilities |= 64;
        if (options.RealmDirectoryEnabled) advertisement.Capabilities |= 128;
        options.Port = std::uint16_t(port);
        options.Advertisement = std::move(advertisement);
        Handoff::ConfigureClient(options);
        return true;
    }

    struct Agent::State
    {
        struct Session;
        boost::asio::io_context Io;
        boost::asio::ssl::context Tls{boost::asio::ssl::context::tls_client};
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> Work{Io.get_executor()};
        boost::asio::steady_timer RetryTimer{Io}, StopTimer{Io};
        AgentOptions Options;
        std::function<AgentSample()> Sample;
        std::shared_ptr<Session> Current;
        std::atomic<bool> Registered{false};
        bool Stopping = false;
        unsigned Backoff = 1;
        std::time_t RenewalCheck = 0;
        void Connect();
        void Failed(Session* source, char const* reason, bool permanent = false);
        void Finish();
        void Shutdown();
    };
    struct Agent::State::Session : std::enable_shared_from_this<Session>
    {
        State& Owner;
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> Stream;
        boost::asio::ip::tcp::resolver Resolver;
        boost::asio::steady_timer Deadline, Pulse;
        std::array<std::uint8_t, HeaderSize> HeaderBytes{};
        Header Reply;
        std::vector<std::uint8_t> Out, In;
        Message Pending = Message::Register;
        bool Closed = false, Registered = false, Busy = true, Ready = false;
        std::uint32_t Lease = 15;
        std::vector<Realms::Query> DirectoryQueries, DirectoryBatch;
        std::size_t DirectoryOffset = 0;
        std::uint64_t DirectoryStarted = 0;
        explicit Session(State& owner) : Owner(owner), Stream(owner.Io, owner.Tls), Resolver(owner.Io), Deadline(owner.Io), Pulse(owner.Io) { }
        void Close()
        {
            if (Owner.Options.RealmDirectoryEnabled) Realms::Client.Clear();
            Owner.Registered = false;
            Closed = true; Resolver.cancel(); Deadline.cancel(); Pulse.cancel();
            boost::system::error_code ec;
            Stream.next_layer().cancel(ec); Stream.next_layer().close(ec);
        }
        void Fail(char const* reason, bool permanent = false)
        {
            if (Closed) return;
            Close(); Owner.Failed(this, reason, permanent);
        }
        void Limit()
        {
            Deadline.expires_after(std::chrono::seconds(5));
            auto self = shared_from_this();
            Deadline.async_wait([self](boost::system::error_code ec) { if (!ec) self->Fail("cluster request timed out"); });
        }
        void Start()
        {
            Limit();
            Stream.set_verify_mode(boost::asio::ssl::verify_peer);
            Stream.set_verify_callback(boost::asio::ssl::host_name_verification(Owner.Options.Host));
            if (!SSL_set_tlsext_host_name(Stream.native_handle(), Owner.Options.Host.c_str()))
            { Fail("cannot configure TLS server identity", true); return; }
            auto self = shared_from_this();
            Resolver.async_resolve(Owner.Options.Host, std::to_string(Owner.Options.Port),
                [self](boost::system::error_code ec, boost::asio::ip::tcp::resolver::results_type endpoints)
            {
                if (self->Closed) return;
                if (ec) { self->Fail("hub name resolution failed"); return; }
                boost::asio::async_connect(self->Stream.next_layer(), endpoints, [self](boost::system::error_code connectError, boost::asio::ip::tcp::endpoint)
                {
                    if (self->Closed) return;
                    if (connectError) { self->Fail("hub connection failed"); return; }
                    self->Stream.async_handshake(boost::asio::ssl::stream_base::client, [self](boost::system::error_code tlsError)
                    {
                        if (self->Closed) return;
                        if (tlsError) { self->Fail("hub TLS verification or handshake failed"); return; }
                        auto const& node = self->Owner.Options.Advertisement;
                        Writer out; out.String(node.Key); out.String(node.Name); out.U8(std::uint8_t(node.Type));
                        out.String(node.Address); out.U16(node.Port); out.U32(node.Realm); out.U32(node.Build);
                        out.U32(node.Capacity); out.U32(node.Capabilities);
                        self->Send(Message::Register, out);
                    });
                });
            });
        }
        void Send(Message kind, Writer const& body)
        {
            if (Closed) return;
            Busy = true; Pending = kind; Out = Frame(kind, body); Limit();
            auto self = shared_from_this();
            boost::asio::async_write(Stream, boost::asio::buffer(Out), [self](boost::system::error_code ec, std::size_t)
            {
                if (self->Closed) return;
                if (ec) { self->Fail("cluster write failed"); return; }
                boost::asio::async_read(self->Stream, boost::asio::buffer(self->HeaderBytes), [self](boost::system::error_code readError, std::size_t)
                {
                    if (self->Closed) return;
                    if (readError) { self->Fail("hub disconnected"); return; }
                    if (!DecodeHeader(self->HeaderBytes, self->Reply) || self->Reply.Version != ProtocolVersion ||
                        (self->Reply.Type != Message::Ack && self->Reply.Type != Message::Error &&
                         !(self->Pending == Realms::RequestType && self->Reply.Type == Realms::ReplyType) &&
                         !(self->Pending == Message::Revocations && self->Reply.Type == Message::RevocationList) &&
                         !(self->Pending == Message::Renew && self->Reply.Type == Message::Renewed)))
                    { self->Fail("incompatible or malformed hub protocol response", true); return; }
                    self->In.resize(self->Reply.Length);
                    boost::asio::async_read(self->Stream, boost::asio::buffer(self->In), [self](boost::system::error_code bodyError, std::size_t)
                    {
                        if (self->Closed) return;
                        if (bodyError) self->Fail("incomplete hub response"); else self->Complete();
                    });
                });
            });
        }
        void Complete()
        {
            Deadline.cancel(); Busy = false;
            if (Reply.Type == Message::RevocationList || Reply.Type == Message::Renewed)
            {
                Reader reader(In); std::string value;
                if (!reader.String(value, 60000) || !reader.End()) { Fail("invalid certificate response"); return; }
                auto const& options = Owner.Options;
                if (Reply.Type == Message::Renewed)
                {
                    if (!Certificates::InstallCertificate(value, options.Certificate, options.PrivateKey, options.CA, options.Advertisement.Key))
                    { Fail("certificate renewal validation failed"); return; }
                    Fail("certificate renewed; reconnecting"); return;
                }
                if (!Certificates::InstallRevocations(value, options.CRL, options.CA))
                { Fail("revocation list validation failed"); return; }
                if (!Certificates::PeerAllowed(Stream.native_handle(), options.CA, options.CRL))
                { Fail("hub certificate revoked or expired"); return; }
                if (Owner.Stopping) { RequestStop(); return; }
                if (std::time(nullptr) >= Owner.RenewalCheck)
                {
                    Owner.RenewalCheck = std::time(nullptr) + 3600;
                    try
                    {
                        std::unique_ptr<X509, decltype(&X509_free)> cert(Certificates::Certificate(options.Certificate), X509_free);
                        // The hub controls renewal eligibility. Standard window is fourteen days.
                        if (cert && Certificates::Expires(cert.get()) <= std::time(nullptr) + 86400 * 14)
                        { Writer csr; csr.String(Certificates::Request(options.PrivateKey, options.Advertisement.Key)); Send(Message::Renew, csr); return; }
                    }
                    catch (...) { Fail("cannot read certificate for renewal"); return; }
                }
                AfterMetrics(); return;
            }
            if (Reply.Type == Realms::ReplyType)
            {
                std::vector<Realms::Route> routes;
                if (!Realms::DecodeReply(In,DirectoryBatch,routes)) { Fail("invalid realm directory response",true); return; }
                for (auto const& route : routes) if (route.State == Realms::Status::Ready)
                {
                    boost::system::error_code ec;
                    auto address = boost::asio::ip::make_address(route.Address,ec);
                    if (ec || address.is_unspecified() || address.is_multicast()) { Fail("invalid realm endpoint",true); return; }
                }
                Realms::Client.Store(routes,DirectoryStarted);
                if (Owner.Stopping) { RequestStop(); return; }
                if (!QueryDirectory()) SchedulePulse();
                return;
            }
            Reader reader(In);
            std::uint16_t code;
            if (Reply.Type == Message::Error)
            {
                std::string reason;
                if (!reader.U16(code) || !reader.String(reason, 256) || !reader.End())
                { Fail("malformed hub error response", true); return; }
                // Trusted peer error descriptions contain no credentials; bound by the decoder.
                SF_LOG_ERROR("server.cluster", "Hub rejected node '%s': %s", Owner.Options.Advertisement.Key.c_str(), reason.c_str());
                if (Pending == Message::Renew) { SchedulePulse(); return; }
                Fail("registration/lifecycle rejected", code != std::uint16_t(Error::Conflict) && code != std::uint16_t(Error::NotRegistered) &&
                    !(code == std::uint16_t(Error::Identity) && !Owner.Options.CRL.empty()));
                return;
            }
            if (!reader.U16(code) || code != std::uint16_t(Pending) || !reader.U32(Lease) || Lease < 5 || Lease > 300 || !reader.End())
            { Fail("invalid hub acknowledgement", true); return; }
            if (Pending == Message::Deregister) { Owner.Finish(); return; }
            if (Pending == Message::Register) Registered = true;
            if (Owner.Stopping) { RequestStop(); return; }
            if (Pending == Message::Register && !Owner.Options.Advertisement.Realms.empty())
            {
                Writer realms; realms.U16(std::uint16_t(Owner.Options.Advertisement.Realms.size()));
                for (auto id : Owner.Options.Advertisement.Realms) realms.U32(id);
                Send(Message::Realms, realms); return;
            }
            if (Pending == Message::Register || Pending == Message::Realms) { Publish(true); return; }
            if (Pending == Message::Ready || Pending == Message::Heartbeat) Owner.Registered = true;
            Owner.Backoff = 1;
            if (Owner.Options.Advertisement.Type == Service::Chat &&
                (Pending == Message::Ready || Pending == Message::Heartbeat))
            { Send(Chat::MetricsType, Chat::EncodeMetrics(Owner.Sample().Chat)); return; }
            if (!Owner.Options.CRL.empty()) { Writer empty; Send(Message::Revocations, empty); return; }
            AfterMetrics();
        }
        void AfterMetrics()
        {
            if (Owner.Options.RealmDirectoryEnabled && Ready)
            {
                DirectoryQueries = Realms::Client.Queries(); DirectoryOffset = 0;
                if (QueryDirectory()) return;
            }
            SchedulePulse();
        }
        bool QueryDirectory()
        {
            if (DirectoryOffset >= DirectoryQueries.size()) return false;
            auto end = (std::min)(DirectoryOffset + Realms::BatchSize,DirectoryQueries.size());
            DirectoryBatch.assign(DirectoryQueries.begin() + DirectoryOffset,DirectoryQueries.begin() + end);
            DirectoryOffset = end; DirectoryStarted = Realms::Now();
            Send(Realms::RequestType,Realms::EncodeQuery(DirectoryBatch)); return true;
        }
        void SchedulePulse()
        {
            Pulse.expires_after(std::chrono::seconds(Owner.Options.RealmDirectoryEnabled ? 1u : (std::max)(1u, (std::min)(5u, Lease / 3))));
            auto self = shared_from_this();
            Pulse.async_wait([self](boost::system::error_code ec) { if (!ec && !self->Closed) self->Publish(false); });
        }
        void Publish(bool force)
        {
            if (Owner.Stopping) { RequestStop(); return; }
            auto const sample = Owner.Sample();
            Writer out;
            bool const changed = force || Ready != sample.Ready;
            if (changed) { Ready = sample.Ready; out.U8(Ready ? 1 : 0); }
            out.U32(sample.Load);
            Send(changed ? Message::Ready : Message::Heartbeat, out);
        }
        void RequestStop()
        {
            Pulse.cancel();
            if (Busy) return;
            if (!Registered) { Owner.Finish(); return; }
            Writer empty; Send(Message::Deregister, empty);
        }
    };
    void Agent::State::Connect()
    {
        if (Stopping) { Finish(); return; }
        try
        {
            Tls.use_certificate_chain_file(Options.Certificate);
            Tls.use_private_key_file(Options.PrivateKey, boost::asio::ssl::context::pem);
        }
        catch (...) { RetryTimer.expires_after(std::chrono::seconds(5)); RetryTimer.async_wait([this](boost::system::error_code ec) { if (!ec) Connect(); }); return; }
        auto session = std::make_shared<Session>(*this);
        Current = session;
        session->Start();
    }
    void Agent::State::Failed(Session* source, char const* reason, bool permanent)
    {
        if (!Current || Current.get() != source) return;
        Current.reset();
        if (Stopping) { Finish(); return; }
        SF_LOG_WARN("server.cluster", "Node '%s': %s; %s.", Options.Advertisement.Key.c_str(), reason,
            permanent ? "cluster registration disabled until restart; game service remains running" : "will reconnect; game service remains running");
        if (permanent) return;
        // Capped exponential backoff with stable per-node jitter avoids synchronized reconnects.
        unsigned jitter = 0; for (unsigned char c : Options.Advertisement.Key) jitter = (jitter * 33 + c) % 500;
        RetryTimer.expires_after(std::chrono::milliseconds(Backoff * 1000 + jitter));
        Backoff = (std::min)(30u, Backoff * 2);
        RetryTimer.async_wait([this](boost::system::error_code ec) { if (!ec) Connect(); });
    }
    void Agent::State::Finish()
    {
        Stopping = true; RetryTimer.cancel(); StopTimer.cancel();
        if (Current) { Current->Close(); Current.reset(); }
        Work.reset();
    }
    void Agent::State::Shutdown()
    {
        Stopping = true; RetryTimer.cancel();
        StopTimer.expires_after(std::chrono::seconds(2));
        StopTimer.async_wait([this](boost::system::error_code ec) { if (!ec) Finish(); });
        auto session = Current;
        if (session) session->RequestStop(); else Finish();
    }
    Agent::~Agent() { Stop(); }
    bool Agent::IsRegistered() const { return _state && _state->Registered.load(); }
    bool Agent::Start(AgentOptions options, std::function<AgentSample()> sample, std::string& error)
    {
        Handoff::ConfigureClient(options);
        Realms::Client.Configure(options.RealmDirectoryEnabled);
        if (!options.Enabled) return true;
        if (_state) { error = "Cluster agent is already started."; return false; }
        try
        {
            auto state = std::make_shared<State>();
            state->Options = std::move(options); state->Sample = std::move(sample);
            if (!SSL_CTX_set_min_proto_version(state->Tls.native_handle(), TLS1_2_VERSION))
            { error = "Cannot require TLS 1.2 for cluster client."; return false; }
            state->Tls.load_verify_file(state->Options.CA);
            state->Tls.use_certificate_chain_file(state->Options.Certificate);
            state->Tls.use_private_key_file(state->Options.PrivateKey, boost::asio::ssl::context::pem);
            if (!SSL_CTX_check_private_key(state->Tls.native_handle())) { error = "Cluster certificate/private key mismatch."; return false; }
            _state = state;
            boost::asio::post(state->Io, [ptr = state.get()] { ptr->Connect(); });
            _thread = std::thread([state]
            {
                try { state->Io.run(); }
                catch (std::exception const&)
                {
                    SF_LOG_ERROR("server.cluster", "Cluster agent stopped after an internal error; game service remains running.");
                    state->Finish();
                }
            });
            return true;
        }
        catch (std::exception const&) { _state.reset(); error = "Cannot start cluster agent; check TLS files and thread resources."; return false; }
    }
    void Agent::Stop()
    {
        if (!_state) return;
        auto state = _state;
        boost::asio::post(state->Io, [ptr = state.get()] { ptr->Shutdown(); });
        if (_thread.joinable()) _thread.join();
        _state.reset();
    }
}
