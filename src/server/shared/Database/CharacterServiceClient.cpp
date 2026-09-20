/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "CharacterServiceClient.h"
#include "Config.h"
#include "Log.h"
#include "PreparedStatement.h"
#include "Transaction.h"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <limits>
#include <locale>
#include <sstream>
#include <stdexcept>

namespace
{
    using Bytes = std::vector<std::uint8_t>;
    constexpr std::size_t Maximum = 64 * 1024 * 1024;
    void U32(Bytes& out, std::uint32_t value)
    { for (int shift : {24,16,8,0}) out.push_back(std::uint8_t(value >> shift)); }
    void Blob(Bytes& out, std::string const& value)
    {
        if (value.size() > Maximum || out.size() + value.size() + 4 > Maximum) throw std::runtime_error("Character request too large");
        U32(out, std::uint32_t(value.size())); out.insert(out.end(),value.begin(),value.end());
    }
    std::string Hex(unsigned char const* bytes, std::size_t size)
    {
        char const* digits = "0123456789abcdef"; std::string out;
        for (std::size_t i=0;i<size;++i) { out += digits[bytes[i] >> 4]; out += digits[bytes[i] & 15]; }
        return out;
    }
    std::string RandomId()
    {
        unsigned char bytes[16];
        if (RAND_bytes(bytes,sizeof(bytes)) != 1) throw std::runtime_error("Character request identity unavailable");
        return Hex(bytes,sizeof(bytes));
    }
    struct Reader
    {
        Bytes const& Data; std::size_t Position = 0;
        std::uint8_t U8() { if (Position == Data.size()) throw std::runtime_error("Truncated character reply"); return Data[Position++]; }
        std::uint32_t U32() { std::uint32_t value=0; for (int i=0;i<4;++i) value=(value<<8)|U8(); return value; }
        std::string Blob() { auto size=U32(); if (size>Data.size()-Position) throw std::runtime_error("Truncated character field"); std::string out(Data.begin()+Position,Data.begin()+Position+size); Position+=size; return out; }
        void End() { if (Position != Data.size()) throw std::runtime_error("Trailing character reply data"); }
    };
}

struct CharacterServiceClient::State
{
    boost::asio::io_context Io;
    boost::asio::ssl::context Tls{boost::asio::ssl::context::tls_client};
    std::unique_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> Socket;
    std::string Host, Catalog, Instance;
    unsigned short Port = 54930;
    template<class Start> void Wait(Start start)
    {
        boost::asio::steady_timer timer(Io); timer.expires_after(std::chrono::seconds(30));
        boost::system::error_code result = boost::asio::error::timed_out;
        timer.async_wait([&](auto error) { if (!error && Socket) { boost::system::error_code ignored; Socket->lowest_layer().close(ignored); } });
        start([&](boost::system::error_code error, auto...) { result=error; timer.cancel(); });
        Io.run(); Io.restart();
        if (result) throw std::runtime_error("Character service transport failed");
    }
    Bytes Exchange(Bytes const& body)
    {
        if (body.size() > 8 * 1024 * 1024) throw std::runtime_error("Character request too large");
        Bytes packet; U32(packet,std::uint32_t(body.size())); packet.insert(packet.end(),body.begin(),body.end());
        Wait([&](auto complete) { boost::asio::async_write(*Socket,boost::asio::buffer(packet),complete); });
        Bytes header(4); Wait([&](auto complete) { boost::asio::async_read(*Socket,boost::asio::buffer(header),complete); });
        Reader input{header}; auto size=input.U32();
        if (!size || size>Maximum) throw std::runtime_error("Invalid character response size");
        Bytes reply(size); Wait([&](auto complete) { boost::asio::async_read(*Socket,boost::asio::buffer(reply),complete); });
        if (reply.front() != 0) throw std::runtime_error("Character service rejected the operation; inspect its log");
        reply.erase(reply.begin()); return reply;
    }
    void Connect()
    {
        Socket = std::make_unique<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>(Io,Tls);
        Socket->set_verify_mode(boost::asio::ssl::verify_peer);
        Socket->set_verify_callback(boost::asio::ssl::host_name_verification(Host));
        auto endpoint=boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(Host),Port);
        Wait([&](auto complete) { Socket->lowest_layer().async_connect(endpoint,complete); });
        Wait([&](auto complete) { Socket->async_handshake(boost::asio::ssl::stream_base::client,complete); });
        X509* certificate=SSL_get1_peer_certificate(Socket->native_handle());
        if (!certificate) throw std::runtime_error("Missing character service certificate");
        auto subject=X509_get_subject_name(certificate);
        int index=X509_NAME_get_index_by_NID(subject,NID_commonName,-1);
        unsigned char* text=nullptr;
        int size=index<0 ? -1 : ASN1_STRING_to_UTF8(&text,X509_NAME_ENTRY_get_data(X509_NAME_get_entry(subject,index)));
        bool identity=size>0 && X509_NAME_get_index_by_NID(subject,NID_commonName,index)==-1 &&
            std::string(reinterpret_cast<char*>(text),size)==sConfigMgr->GetStringDefault("CharacterService.NodeKey","characters-1");
        OPENSSL_free(text); X509_free(certificate);
        if (!identity) throw std::runtime_error("Character service certificate identity mismatch");
        Bytes hello{0,1}; U32(hello,std::uint32_t(sConfigMgr->GetIntDefault("RealmID",0))); Blob(hello,Instance); Blob(hello,Catalog);
        Exchange(hello);
    }
};

CharacterServiceClient::CharacterServiceClient() : _state(std::make_unique<State>()) { }
CharacterServiceClient::~CharacterServiceClient() = default;
bool CharacterServiceClient::Enabled() { return sConfigMgr->GetBoolDefault("CharacterService.Enable",false); }
bool CharacterServiceClient::Open(std::map<std::uint32_t,std::string> const& catalog)
{
    try
    {
        static std::string const instance = RandomId(); // One writer incarnation across every pool connection.
        _state->Instance=instance; _state->Host=sConfigMgr->GetStringDefault("CharacterService.Host","127.0.0.1");
        int port=sConfigMgr->GetIntDefault("CharacterService.Port",54930);
        if (port<1 || port>65535) return false;
        _state->Port=static_cast<unsigned short>(port);
        auto path=[](char const* key) {
            auto value=std::filesystem::path(sConfigMgr->GetStringDefault(key,""));
            if (value.empty()) throw std::runtime_error("Missing character service TLS setting");
            return (value.is_relative() ? std::filesystem::path(sConfigMgr->GetFilename()).parent_path()/value : value).string();
        };
        _state->Tls.load_verify_file(path("Cluster.CA"));
        _state->Tls.use_certificate_chain_file(path("Cluster.Certificate"));
        _state->Tls.use_private_key_file(path("Cluster.PrivateKey"),boost::asio::ssl::context::pem);
        if (SSL_CTX_set_min_proto_version(_state->Tls.native_handle(),TLS1_2_VERSION)!=1) return false;
        Bytes bytes; for(auto const& entry:catalog) { U32(bytes,entry.first); Blob(bytes,entry.second); }
        unsigned char digest[SHA256_DIGEST_LENGTH]; SHA256(bytes.data(),bytes.size(),digest);
        _state->Catalog=Hex(digest,sizeof(digest)); _state->Connect(); return true;
    }
    catch (std::exception const&)
    { SF_LOG_ERROR("sql.driver","Character service startup failed; no direct character database fallback is allowed."); return false; }
}

std::vector<std::uint8_t> CharacterServiceClient::Encode(char const* sql)
{ Bytes out{0}; Blob(out,sql); return out; }
std::vector<std::uint8_t> CharacterServiceClient::Encode(PreparedStatement const* statement)
{
    Bytes out{1}; U32(out,statement->m_index); U32(out,std::uint32_t(statement->statement_data.size()));
    for (auto const& parameter:statement->statement_data)
    {
        if (parameter.type==TYPE_NULL) { out.push_back(0); continue; }
        if (parameter.type==TYPE_STRING || parameter.type==TYPE_BINARY)
        {
            auto size=parameter.binary.size();
            if (parameter.type==TYPE_STRING && size && parameter.binary.back()==0) --size;
            out.push_back(parameter.type==TYPE_STRING ? 2 : 3);
            Blob(out,std::string(parameter.binary.begin(),parameter.binary.begin()+size)); continue;
        }
        std::ostringstream number; number.imbue(std::locale::classic()); number<<std::setprecision(std::numeric_limits<double>::max_digits10);
        switch(parameter.type)
        {
            case TYPE_BOOL: number<<int(parameter.data.boolean); break;
            case TYPE_UI8: number<<unsigned(parameter.data.ui8); break;
            case TYPE_UI16: number<<parameter.data.ui16; break;
            case TYPE_UI32: number<<parameter.data.ui32; break;
            case TYPE_UI64: number<<parameter.data.ui64; break;
            case TYPE_I8: number<<int(parameter.data.i8); break;
            case TYPE_I16: number<<parameter.data.i16; break;
            case TYPE_I32: number<<parameter.data.i32; break;
            case TYPE_I64: number<<parameter.data.i64; break;
            case TYPE_FLOAT: number<<parameter.data.f; break;
            case TYPE_DOUBLE: number<<parameter.data.d; break;
            default: throw std::runtime_error("Invalid character statement parameter");
        }
        out.push_back(1); Blob(out,number.str());
    }
    return out;
}
std::vector<std::uint8_t> CharacterServiceClient::Request(std::uint8_t operation, Bytes body)
{
    try
    {
        Bytes request{operation}; Blob(request,RandomId()); request.insert(request.end(),body.begin(),body.end());
        // No blind transport retry: an uncertain write must not be presented as success.
        return _state->Exchange(request);
    }
    catch (std::exception const&)
    {
        SF_LOG_ERROR("sql.driver","Character service operation failed or commit outcome is uncertain. Stopping this world to prevent continued gameplay on unsaved or missing state.");
        std::abort();
    }
}
void CharacterServiceClient::Execute(char const* sql) { Request(1,Encode(sql)); }
void CharacterServiceClient::Execute(PreparedStatement const* statement) { Request(1,Encode(statement)); }
void CharacterServiceClient::Execute(Transaction const& transaction)
{
    Bytes body; U32(body,std::uint32_t(transaction.m_queries.size()));
    for (auto const& entry:transaction.m_queries)
    {
        auto encoded=entry.type==SQL_ELEMENT_RAW ? Encode(entry.element.query) : Encode(entry.element.stmt);
        body.insert(body.end(),encoded.begin(),encoded.end());
    }
    Request(3,std::move(body));
}
CharacterServiceRows CharacterServiceClient::Rows(Bytes const& bytes)
{
    try
    {
        Reader input{bytes}; CharacterServiceRows out; auto fields=input.U32();
        if (!fields || fields>1024) throw std::runtime_error("Invalid character result columns");
        for(std::uint32_t i=0;i<fields;++i) out.Types.push_back(input.U32());
        auto count=input.U32(); if(count>1000000 || std::uint64_t(count)*fields>bytes.size()) throw std::runtime_error("Invalid character result rows");
        for(std::uint32_t i=0;i<count;++i)
        {
            std::vector<std::optional<std::string>> row;
            for(std::uint32_t j=0;j<fields;++j) { auto present=input.U8(); if(present>1) throw std::runtime_error("Invalid character null field"); row.push_back(present ? std::optional<std::string>(input.Blob()) : std::nullopt); }
            out.Rows.push_back(std::move(row));
        }
        input.End(); return out;
    }
    catch (std::exception const&) { SF_LOG_ERROR("sql.driver","Invalid character service result; stopping world."); std::abort(); }
}
CharacterServiceRows CharacterServiceClient::Query(char const* sql) { return Rows(Request(2,Encode(sql))); }
CharacterServiceRows CharacterServiceClient::Query(PreparedStatement const* statement) { return Rows(Request(2,Encode(statement))); }
unsigned long CharacterServiceClient::Escape(char* to,char const* from,unsigned long length)
{
    char* start=to;
    for(unsigned long i=0;i<length;++i)
        switch(from[i])
        {
            case '\0': *to++='\\'; *to++='0'; break;
            case '\n': *to++='\\'; *to++='n'; break;
            case '\r': *to++='\\'; *to++='r'; break;
            case '\032': *to++='\\'; *to++='Z'; break;
            case '\\': case '\'': case '"': *to++='\\'; *to++=from[i]; break;
            default: *to++=from[i];
        }
    *to=0; return static_cast<unsigned long>(to-start);
}
