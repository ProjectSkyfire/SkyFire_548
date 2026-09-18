/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "HandoffClient.h"
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <atomic>
#include <chrono>
#include <mutex>

namespace Skyfire::Cluster::Handoff
{
    namespace
    {
        std::mutex OptionsLock;
        AgentOptions Options;
        std::atomic<unsigned> InFlight{0};
        std::string Hex(unsigned char const* bytes, std::size_t size)
        {
            char const* digits = "0123456789abcdef"; std::string out;
            for (std::size_t i = 0; i < size; ++i) { out += digits[bytes[i] >> 4]; out += digits[bytes[i] & 15]; }
            return out;
        }
    }
    std::string RandomToken()
    {
        std::array<unsigned char,32> bytes{};
        return RAND_bytes(bytes.data(),int(bytes.size())) == 1 ? Hex(bytes.data(),bytes.size()) : "";
    }
    std::string Evidence(void const* bytes, std::size_t size)
    {
        std::array<unsigned char,32> digest{}; unsigned length = 0;
        if (!bytes || !size || EVP_Digest(bytes,size,digest.data(),&length,EVP_sha256(),nullptr) != 1 || length != digest.size()) return "";
        return Hex(digest.data(),digest.size());
    }
    void ConfigureClient(AgentOptions const& options)
    { std::lock_guard<std::mutex> guard(OptionsLock); Options = options; }
    bool Enabled()
    { std::lock_guard<std::mutex> guard(OptionsLock); return Options.Enabled && Options.HandoffEnabled; }
    Result Call(Request const& request, std::string* token, std::string* diagnostic)
    {
        if (diagnostic) *diagnostic = "Client disabled, overloaded or TLS configuration unavailable";
        if (token) token->clear();
        AgentOptions options;
        { std::lock_guard<std::mutex> guard(OptionsLock); options = Options; }
        if (!options.Enabled || !options.HandoffEnabled) return Result::Unavailable;
        // Bound blocking work in protocol-owning threads; never retry an uncertain mutation.
        if (InFlight.fetch_add(1) >= 8) { --InFlight; return Result::Unavailable; }
        struct Guard { ~Guard() { --InFlight; } } guard;
        try
        {
            auto payload = Encode(request); Request validated;
            if (!Decode(payload.Bytes,validated)) return Result::Invalid;
            boost::asio::io_context io;
            boost::asio::ssl::context tls(boost::asio::ssl::context::tls_client);
            if (!SSL_CTX_set_min_proto_version(tls.native_handle(),TLS1_2_VERSION)) return Result::Unavailable;
            tls.load_verify_file(options.CA); tls.use_certificate_chain_file(options.Certificate);
            tls.use_private_key_file(options.PrivateKey,boost::asio::ssl::context::pem);
            if (!SSL_CTX_check_private_key(tls.native_handle())) return Result::Unavailable;
            boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream(io,tls);
            boost::asio::ip::tcp::resolver resolver(io);
            stream.set_verify_mode(boost::asio::ssl::verify_peer);
            stream.set_verify_callback(boost::asio::ssl::host_name_verification(options.Host));
            if (!SSL_set_tlsext_host_name(stream.native_handle(),options.Host.c_str())) return Result::Unavailable;
            auto out = Frame(RequestType,payload);
            std::array<std::uint8_t,HeaderSize> bytes{}; Header header;
            std::vector<std::uint8_t> body;
            Result result = Result::Unavailable; std::string received;
            auto fail = [&](char const* stage) { if (diagnostic) *diagnostic = stage; io.stop(); };
            if (diagnostic) *diagnostic = "Deadline expired during name resolution";
            resolver.async_resolve(options.Host,std::to_string(options.Port),
                [&](boost::system::error_code ec, boost::asio::ip::tcp::resolver::results_type endpoints)
            {
                if (ec) { fail("Hub name resolution failed"); return; }
                if (diagnostic) *diagnostic = "Deadline expired during TCP connection";
                boost::asio::async_connect(stream.next_layer(),endpoints,[&](boost::system::error_code error, auto const&)
                {
                    if (error) { fail("Hub TCP connection failed"); return; }
                    if (diagnostic) *diagnostic = "Deadline expired during TLS handshake";
                    stream.async_handshake(boost::asio::ssl::stream_base::client,[&](boost::system::error_code handshake)
                    {
                        if (handshake) { fail("Hub TLS handshake or certificate verification failed"); return; }
                        if (diagnostic) *diagnostic = "Deadline expired while sending request";
                        boost::asio::async_write(stream,boost::asio::buffer(out),[&](boost::system::error_code write, std::size_t)
                        {
                            if (write) { fail("Handoff request write failed"); return; }
                            if (diagnostic) *diagnostic = "Deadline expired awaiting reply header";
                            boost::asio::async_read(stream,boost::asio::buffer(bytes),[&](boost::system::error_code read, std::size_t)
                            {
                                if (read || !DecodeHeader(bytes,header) || header.Version != ProtocolVersion || header.Type != ReplyType || !header.Length)
                                { fail("Invalid or unavailable handoff reply header"); return; }
                                body.resize(header.Length);
                                if (diagnostic) *diagnostic = "Deadline expired awaiting reply body";
                                boost::asio::async_read(stream,boost::asio::buffer(body),[&](boost::system::error_code data, std::size_t)
                                {
                                    Reader response(body); std::uint8_t code;
                                    if (!data && response.U8(code) && code <= std::uint8_t(Result::Capacity) &&
                                        response.String(received,64) && response.End() &&
                                        (received == "-" || IsToken(received))) result = Result(code);
                                    if (diagnostic) *diagnostic = result == Result::Unavailable ? "Invalid or unavailable handoff reply body" : "Hub returned result " + std::to_string(unsigned(result));
                                    io.stop();
                                });
                            });
                        });
                    });
                });
            });
            io.run_for(std::chrono::seconds(5));
            resolver.cancel(); boost::system::error_code ignored; stream.next_layer().close(ignored);
            if (result == Result::Ok && token && IsToken(received)) *token = std::move(received);
            return result;
        }
        catch (std::exception const&) { return Result::Unavailable; }
    }
}
