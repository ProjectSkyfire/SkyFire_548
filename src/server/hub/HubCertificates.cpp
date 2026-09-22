/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "HubCertificates.h"
#include "Cluster/CertificateTools.h"
#include "Cluster/ClusterProtocol.h"
#include "Configuration/Config.h"
#include "Log.h"
#include <boost/asio/ip/address.hpp>
#include <openssl/pem.h>
#include <openssl/x509v3.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <mutex>
#include <regex>
#include <ctime>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace Skyfire::HubCertificates
{
    namespace
    {
        namespace fs = std::filesystem;
        namespace C = Skyfire::Certificates;
        template<class T, auto F> using Owned = std::unique_ptr<T, decltype(F)>;
        std::recursive_mutex Mutex;
        bool Active = false;
        fs::path Root, CaPath, KeyPath;
        Owned<X509, X509_free> Ca(nullptr, X509_free);
        Owned<EVP_PKEY, EVP_PKEY_free> SigningKey(nullptr, EVP_PKEY_free);
        unsigned Days = 90, RenewDays = 14;
        std::time_t NextUpdate = 0;
#ifdef _WIN32
        HANDLE StoreLock = INVALID_HANDLE_VALUE;
#else
        int StoreLock = -1;
#endif
        struct Profile { std::string Node, Names, Certificate, Key; unsigned Role = 0; };
        std::map<std::string, Profile> Profiles;
        std::map<std::string, std::string> Issued;
        std::set<std::string> Revoked, Blocked;
        bool SafeNode(std::string const& node)
        {
            return std::regex_match(node, std::regex("[A-Za-z0-9][A-Za-z0-9_-]{0,63}")) &&
                !std::regex_match(node, std::regex("CON|PRN|AUX|NUL|COM[0-9]|LPT[0-9]", std::regex::icase));
        }
        void Require(bool ok) { if (!ok) throw std::runtime_error("Certificate authority operation failed; check its configuration and protected store."); }
        std::string Read(fs::path const& path) { return C::Read(path.string()); }
        void Write(fs::path const& path, std::string const& value) { C::Write(path.string(), value); }
        fs::path ConfigPath(char const* key, std::string const& fallback)
        { return (fs::absolute(sConfigMgr->GetFilename()).parent_path() / sConfigMgr->GetStringDefault(key, fallback)).lexically_normal(); }
        void Audit(std::string const& action, std::string const& node, std::string const& actor)
        { SF_LOG_INFO("server.pki", "Certificate action %s for node %s by %s.", action.c_str(), node.c_str(), actor.c_str()); }
        std::string San(std::string const& names)
        {
            Require(!names.empty() && names.size() <= 512);
            std::istringstream input(names); std::string name, result; unsigned count = 0;
            while (std::getline(input, name, ','))
            {
                Require(++count <= 16 && !name.empty() && name.size() <= 253);
                boost::system::error_code ec; auto address = boost::asio::ip::make_address(name, ec);
                if (!ec) Require(!address.is_unspecified() && !address.is_multicast());
                else Require(std::regex_match(name, std::regex("[A-Za-z0-9](?:[A-Za-z0-9.-]*[A-Za-z0-9])?")) && name.find("..") == std::string::npos);
                if (!result.empty()) result += ',';
                result += (ec ? "DNS:" : "IP:") + name;
            }
            Require(count != 0 && names.back() != ','); return result;
        }
        void Extension(X509* cert, int nid, std::string const& value)
        {
            X509V3_CTX context; X509V3_set_ctx(&context, Ca.get() ? Ca.get() : cert, cert, nullptr, nullptr, 0);
            Owned<X509_EXTENSION, X509_EXTENSION_free> extension(X509V3_EXT_conf_nid(nullptr, &context, nid, value.c_str()), X509_EXTENSION_free);
            Require(extension && X509_add_ext(cert, extension.get(), -1) == 1);
        }
        Owned<X509, X509_free> Make(std::string const& node, unsigned role, std::string const& names, EVP_PKEY* key, bool authority = false)
        {
            Owned<X509, X509_free> cert(X509_new(), X509_free); Require(bool(cert));
            Require(X509_set_version(cert.get(), 2) == 1);
            auto serial = C::Random().substr(0, 32); BIGNUM* raw = nullptr; Require(BN_hex2bn(&raw, serial.c_str()) > 0);
            Owned<BIGNUM, BN_free> bn(raw, BN_free); Owned<ASN1_INTEGER, ASN1_INTEGER_free> number(BN_to_ASN1_INTEGER(bn.get(), nullptr), ASN1_INTEGER_free);
            Require(number && X509_set_serialNumber(cert.get(), number.get()) == 1 && X509_set_pubkey(cert.get(), key) == 1);
            auto* subject = X509_get_subject_name(cert.get());
            Require(X509_NAME_add_entry_by_NID(subject, NID_commonName, MBSTRING_ASC,
                reinterpret_cast<unsigned char const*>(node.c_str()), -1, -1, 0) == 1);
            if (!authority)
            {
                auto value = "SkyFire-role-" + std::to_string(role);
                Require(X509_NAME_add_entry_by_NID(subject, NID_organizationalUnitName, MBSTRING_ASC,
                    reinterpret_cast<unsigned char const*>(value.c_str()), -1, -1, 0) == 1);
            }
            Require(X509_set_issuer_name(cert.get(), authority ? subject : X509_get_subject_name(Ca.get())) == 1);
            Require(X509_gmtime_adj(X509_getm_notBefore(cert.get()), -60) != nullptr);
            auto expires = std::time(nullptr) + std::time_t(authority ? 3650 : Days) * 86400;
            if (!authority) expires = (std::min)(expires, std::time_t(C::Expires(Ca.get())));
            Require(expires > std::time(nullptr) + 86400 && ASN1_TIME_set(X509_getm_notAfter(cert.get()), expires));
            Extension(cert.get(), NID_basic_constraints, authority ? "critical,CA:TRUE,pathlen:0" : "critical,CA:FALSE");
            Extension(cert.get(), NID_key_usage, authority ? "critical,keyCertSign,cRLSign" : "critical,digitalSignature,keyEncipherment");
            Extension(cert.get(), NID_subject_key_identifier, "hash");
            if (!authority)
            { Extension(cert.get(), NID_ext_key_usage, "clientAuth,serverAuth"); Extension(cert.get(), NID_subject_alt_name, San(names)); }
            Require(X509_sign(cert.get(), authority ? key : SigningKey.get(), EVP_sha256()) > 0); return cert;
        }
        void SaveProfile(Profile const& profile)
        {
            Require(SafeNode(profile.Node) && profile.Role >= 1 && profile.Role <= 5);
            Write(Root / "profiles" / profile.Node, std::to_string(profile.Role) + '\n' + profile.Names + '\n' + profile.Certificate + '\n' + profile.Key + '\n');
            Profiles[profile.Node] = profile;
        }
        void CancelTokens(std::string const& node)
        {
            for (auto const& file : fs::directory_iterator(Root / "tokens"))
            { std::istringstream input(Read(file.path())); std::string target; std::getline(input, target); if (target == node) fs::remove(file.path()); }
        }
        void PruneExpired()
        {
            for (auto it = Issued.begin(); it != Issued.end();)
            {
                auto path = Root / "issued" / (it->first + ".pem");
                Owned<X509, X509_free> cert(C::Certificate(path.string()), X509_free); Require(bool(cert));
                if (C::Expires(cert.get()) <= std::time(nullptr))
                { fs::remove(path); fs::remove(Root / "revoked" / it->first); Revoked.erase(it->first); it = Issued.erase(it); }
                else ++it;
            }
        }
        void RefreshCrl()
        {
            // A durable node block is also a durable intent to revoke every serial,
            // including recovery after a disk error interrupted the first attempt.
            for (auto const& entry : Issued) if (Blocked.count(entry.second) && !Revoked.count(entry.first))
            { Write(Root / "revoked" / entry.first, "revoked\n"); Revoked.insert(entry.first); }
            Owned<X509_CRL, X509_CRL_free> crl(X509_CRL_new(), X509_CRL_free); Require(bool(crl));
            Require(X509_CRL_set_version(crl.get(), 1) == 1 && X509_CRL_set_issuer_name(crl.get(), X509_get_subject_name(Ca.get())) == 1);
            Owned<ASN1_TIME, ASN1_TIME_free> now(ASN1_TIME_set(nullptr, std::time(nullptr) - 60), ASN1_TIME_free),
                next(ASN1_TIME_set(nullptr, std::time(nullptr) + 86400), ASN1_TIME_free);
            Require(now && next && X509_CRL_set1_lastUpdate(crl.get(), now.get()) == 1 && X509_CRL_set1_nextUpdate(crl.get(), next.get()) == 1);
            auto numberPath = Root / "crl-number";
            auto number = fs::exists(numberPath) ? std::stoull(Read(numberPath)) + 1 : 1ULL;
            Require(number > 0 && number < 0x7fffffffffffffffULL);
            Write(numberPath, std::to_string(number)); // Reserve before publishing, including across crashes.
            Owned<ASN1_INTEGER, ASN1_INTEGER_free> sequence(ASN1_INTEGER_new(), ASN1_INTEGER_free);
            Require(sequence && ASN1_INTEGER_set_uint64(sequence.get(), number) == 1 &&
                X509_CRL_add1_ext_i2d(crl.get(), NID_crl_number, sequence.get(), 0, 0) == 1);
            for (auto const& serial : Revoked)
            {
                auto found = Issued.find(serial); if (found == Issued.end()) continue;
                Owned<X509, X509_free> cert(C::Certificate((Root / "issued" / (serial + ".pem")).string()), X509_free); Require(bool(cert));
                if (C::Expires(cert.get()) <= std::time(nullptr)) continue;
                Owned<X509_REVOKED, X509_REVOKED_free> entry(X509_REVOKED_new(), X509_REVOKED_free); Require(bool(entry));
                Require(X509_REVOKED_set_serialNumber(entry.get(), X509_get_serialNumber(cert.get())) == 1 &&
                    X509_REVOKED_set_revocationDate(entry.get(), now.get()) == 1 && X509_CRL_add0_revoked(crl.get(), entry.get()) == 1);
                entry.release();
            }
            Require(X509_CRL_sort(crl.get()) == 1 && X509_CRL_sign(crl.get(), SigningKey.get(), EVP_sha256()) > 0);
            Write(Root / "revocations.pem", C::Pem(crl.get()));
            NextUpdate = std::time(nullptr) + 3600;
        }
        std::string Issue(Profile const& profile, EVP_PKEY* key)
        {
            PruneExpired(); Require(Issued.size() < 1024 && EVP_PKEY_get_security_bits(key) >= 112);
            auto cert = Make(profile.Node, profile.Role, profile.Names, key);
            auto serial = C::Serial(cert.get());
            Write(Root / "issued" / (serial + ".pem"), C::Pem(cert.get())); Issued[serial] = profile.Node;
            if (!profile.Certificate.empty()) Write(profile.Certificate, C::Pem(cert.get()));
            SaveProfile(profile); return C::Wire(cert.get());
        }
        Owned<EVP_PKEY, EVP_PKEY_free> CsrKey(std::string const& value)
        {
            auto der = C::Unbase64(value); Require(der.size() <= 8192);
            auto* p = reinterpret_cast<unsigned char const*>(der.data());
            Owned<X509_REQ, X509_REQ_free> request(d2i_X509_REQ(nullptr, &p, long(der.size())), X509_REQ_free);
            Require(request && p == reinterpret_cast<unsigned char const*>(der.data() + der.size()));
            Owned<EVP_PKEY, EVP_PKEY_free> key(X509_REQ_get_pubkey(request.get()), EVP_PKEY_free);
            Require(key && EVP_PKEY_get_security_bits(key.get()) >= 112 && X509_REQ_verify(request.get(), key.get()) == 1); return key;
        }
    }
    bool Enabled() { std::lock_guard<std::recursive_mutex> lock(Mutex); return Active; }
    void Shutdown()
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        Active = false; Profiles.clear(); Issued.clear(); Revoked.clear(); Blocked.clear(); Ca.reset(); SigningKey.reset();
#ifdef _WIN32
        if (StoreLock != INVALID_HANDLE_VALUE) { CloseHandle(StoreLock); StoreLock = INVALID_HANDLE_VALUE; }
#else
        if (StoreLock >= 0) { close(StoreLock); StoreLock = -1; }
#endif
    }
    std::string ListenerPath(std::string const& kind)
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        if (kind == "CA") return CaPath.string();
        auto const& p = Profiles.at("skyfire-hub-authority");
        return kind == "Certificate" ? p.Certificate : p.Key;
    }
    bool Initialize(std::string& error)
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        if (!sConfigMgr->GetBoolDefault("Hub.PKI.Enable", false)) return true;
        try
        {
            Require(sConfigMgr->GetBoolDefault("Hub.Cluster.Enable", false));
            Root = ConfigPath("Hub.PKI.Directory", "pki"); CaPath = ConfigPath("Hub.PKI.CA", "pki/ca.pem"); KeyPath = ConfigPath("Hub.PKI.PrivateKey", "pki/ca-key.pem");
            fs::create_directories(Root);
#ifdef _WIN32
            StoreLock = CreateFileW((Root / "authority.lock").c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
            Require(StoreLock != INVALID_HANDLE_VALUE);
#else
            fs::permissions(Root, fs::perms::owner_all, fs::perm_options::replace);
            StoreLock = open((Root / "authority.lock").c_str(), O_RDWR | O_CREAT, 0600);
            Require(StoreLock >= 0 && flock(StoreLock, LOCK_EX | LOCK_NB) == 0);
#endif
            int days = sConfigMgr->GetIntDefault("Hub.PKI.ValidDays", 90), renew = sConfigMgr->GetIntDefault("Hub.PKI.RenewDays", 14);
            Require(days >= 7 && days <= 365 && renew >= 1 && renew < days); Days = unsigned(days); RenewDays = unsigned(renew);
            if (!fs::exists(CaPath) && !fs::exists(KeyPath))
            {
                auto trusted = ConfigPath("Hub.Cluster.CA", "");
                Require(!fs::is_regular_file(trusted)); // Never silently replace an existing cluster's trust root.
                SigningKey.reset(C::NewKey()); auto cert = Make("SkyFire Cluster Authority", 0, "", SigningKey.get(), true);
                Write(KeyPath, C::Pem(SigningKey.get())); Write(CaPath, C::Pem(cert.get()));
            }
            Ca.reset(C::Certificate(CaPath.string())); SigningKey.reset(C::Key(KeyPath.string()));
            Require(Ca && SigningKey && X509_check_ca(Ca.get()) > 0 && X509_check_private_key(Ca.get(), SigningKey.get()) == 1 && C::Expires(Ca.get()) > std::time(nullptr) + 86400);
            for (auto const& folder : {"profiles", "issued", "revoked", "blocked", "tokens", "consumed"}) fs::create_directories(Root / folder);
            for (auto const& file : fs::directory_iterator(Root / "profiles"))
            {
                if (file.path().extension() == ".tmp") continue;
                auto node = file.path().filename().string(); Require(SafeNode(node) && Profiles.size() < 128);
                std::istringstream input(Read(file.path())); Profile p; p.Node = node;
                std::string role; std::getline(input, role); p.Role = unsigned(std::stoul(role));
                std::getline(input, p.Names); std::getline(input, p.Certificate); std::getline(input, p.Key);
                Require(p.Role >= 1 && p.Role <= 5); San(p.Names); Profiles[node] = p;
            }
            for (auto const& file : fs::directory_iterator(Root / "issued"))
            {
                if (file.path().extension() != ".pem") continue;
                Owned<X509, X509_free> cert(C::Certificate(file.path().string()), X509_free); Require(bool(cert));
                if (C::Expires(cert.get()) <= std::time(nullptr)) { fs::remove(Root / "revoked" / C::Serial(cert.get())); fs::remove(file.path()); continue; }
                Require(Issued.size() < 1024);
                auto serial = C::Serial(cert.get()); Require(file.path().filename() == serial + ".pem"); Issued[serial] = C::Identity(cert.get());
                if (fs::exists(Root / "revoked" / serial)) Revoked.insert(serial);
            }
            for (auto const& file : fs::directory_iterator(Root / "blocked")) Blocked.insert(file.path().filename().string());
            auto legacyDirectory = sConfigMgr->GetStringDefault("Hub.PKI.ImportDirectory", "");
            if (!legacyDirectory.empty())
            {
                Owned<EVP_PKEY, EVP_PKEY_free> publicKey(X509_get_pubkey(Ca.get()), EVP_PKEY_free); Require(bool(publicKey));
                for (auto const& file : fs::directory_iterator(ConfigPath("Hub.PKI.ImportDirectory", "")))
                {
                    if (!file.is_regular_file() || (file.path().extension() != ".pem" && file.path().extension() != ".crt")) continue;
                    Owned<X509, X509_free> cert(C::Certificate(file.path().string()), X509_free);
                    if (!cert || X509_check_ca(cert.get()) || X509_verify(cert.get(), publicKey.get()) != 1 || C::Expires(cert.get()) <= std::time(nullptr)) continue;
                    auto node = C::Identity(cert.get()), serial = C::Serial(cert.get());
                    Require(SafeNode(node));
                    if (!Issued.count(serial))
                    { Require(Issued.size() < 1024); Write(Root / "issued" / (serial + ".pem"), C::Pem(cert.get())); Issued[serial] = node; }
                    if (fs::exists(Root / "revoked" / serial)) Revoked.insert(serial);
                }
            }
            if (!Profiles.count("skyfire-hub-authority"))
            {
                Profile p; p.Node = "skyfire-hub-authority"; p.Role = 1;
                p.Names = sConfigMgr->GetStringDefault("Hub.PKI.HubNames", "localhost,127.0.0.1"); San(p.Names);
                p.Certificate = (Root / "hub-certificate.pem").string(); p.Key = (Root / "hub-private-key.pem").string();
                Owned<EVP_PKEY, EVP_PKEY_free> key(C::NewKey(), EVP_PKEY_free);
                Write(p.Key, C::Pem(key.get())); Issue(p, key.get());
            }
            RefreshCrl(); Active = true; NextUpdate = 0; Update(); return true;
        }
        catch (std::exception const&) { error = "Cannot initialize hub certificate authority. Check CA/key pairing, validity and protected store; import the existing cluster CA before enabling PKI."; return false; }
    }
    std::string Token(std::string const& node, unsigned role, std::string const& names, std::string const& actor)
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        Require(Active && SafeNode(node) && node != "skyfire-hub-authority" && role >= 1 && role <= 5); San(names);
        auto profile = Profiles.find(node);
        Require(profile == Profiles.end() || (profile->second.Key.empty() && profile->second.Role == role));
        unsigned outstanding = 0;
        for (auto const& file : fs::directory_iterator(Root / "tokens"))
        {
            auto value = Read(file.path()); auto end = value.find_last_of('\n');
            if (end != std::string::npos && std::stoll(value.substr(end + 1)) <= std::time(nullptr)) fs::remove(file.path());
            else ++outstanding;
        }
        Require(outstanding < 128);
        CancelTokens(node);
        Require(Profiles.count(node) || Profiles.size() < 128);
        auto token = C::Random();
        Write(Root / "tokens" / C::Digest(token), node + '\n' + std::to_string(role) + '\n' + names + '\n' + std::to_string(std::time(nullptr) + 600));
        Audit("enrollment-token", node, actor); return token;
    }
    std::string Enroll(std::string const& token, std::string const& csr)
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        Require(Active && token.size() == 64 && token.find_first_not_of("0123456789abcdef") == std::string::npos);
        auto hash = C::Digest(token); auto ticket = Root / "tokens" / hash;
        std::istringstream input(Read(ticket)); Profile p; std::string role, expires;
        std::getline(input, p.Node); std::getline(input, role); std::getline(input, p.Names); std::getline(input, expires);
        p.Role = unsigned(std::stoul(role)); Require(SafeNode(p.Node) && p.Role >= 1 && p.Role <= 5 &&
            std::stoll(expires) > std::time(nullptr) && (Profiles.count(p.Node) || Profiles.size() < 128)); San(p.Names);
        auto existing = Profiles.find(p.Node);
        Require(existing == Profiles.end() || (existing->second.Key.empty() && existing->second.Role == p.Role));
        auto key = CsrKey(csr);
        fs::rename(ticket, Root / "consumed" / hash); // Burn before signing: no concurrent or uncertain reuse.
        auto previous = Issued;
        auto result = Issue(p, key.get());
        for (auto const& item : previous) if (item.second == p.Node)
        { Write(Root / "revoked" / item.first, "re-enrolled\n"); Revoked.insert(item.first); }
        fs::remove(Root / "consumed" / hash);
        fs::remove(Root / "blocked" / p.Node); Blocked.erase(p.Node);
        NextUpdate = 0; RefreshCrl();
        Audit("enrolled", p.Node, "one-time-token"); return result;
    }
    bool Allowed(X509* certificate)
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        if (!Active) return true;
        return certificate && Issued.count(C::Serial(certificate)) && C::Identity(certificate) != "skyfire-hub-authority" &&
            !Blocked.count(C::Identity(certificate)) && !Revoked.count(C::Serial(certificate)) &&
            C::Expires(certificate) > std::time(nullptr);
    }
    std::string Renew(X509* current, std::string const& csr)
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex); Require(Active && Allowed(current));
        auto profile = Profiles.find(C::Identity(current)); Require(profile != Profiles.end() && Issued.count(C::Serial(current)));
        Require(C::Expires(current) <= std::time(nullptr) + std::int64_t(RenewDays) * 86400);
        Require(C::Expires(Ca.get()) > C::Expires(current));
        auto key = CsrKey(csr);
        Require(X509_check_private_key(current, key.get()) == 1); // Public-key equality; no remote key substitution.
        // A lost renewal reply returns the existing replacement, rather than issuing repeatedly.
        for (auto const& issued : Issued) if (issued.second == profile->first && !Revoked.count(issued.first))
        {
            Owned<X509, X509_free> candidate(C::Certificate((Root / "issued" / (issued.first + ".pem")).string()), X509_free);
            if (candidate && C::Expires(candidate.get()) > C::Expires(current) && X509_check_private_key(candidate.get(), key.get()) == 1)
                return C::Wire(candidate.get());
        }
        return Issue(profile->second, key.get());
    }
    std::string Revocations()
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex); Require(Active);
        auto pem = Read(Root / "revocations.pem"); Owned<BIO, BIO_free> bio(BIO_new_mem_buf(pem.data(), int(pem.size())), BIO_free);
        Owned<X509_CRL, X509_CRL_free> crl(PEM_read_bio_X509_CRL(bio.get(), nullptr, nullptr, nullptr), X509_CRL_free); Require(bool(crl)); return C::Wire(crl.get());
    }
    bool Revoke(std::string const& node, std::string const& actor, std::string& error)
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        try
        {
            Require(Active && SafeNode(node) && node != "skyfire-hub-authority" && (Profiles.count(node) ||
                std::any_of(Issued.begin(), Issued.end(), [&](auto const& entry) { return entry.second == node; })));
            Write(Root / "blocked" / node, "revoked\n"); Blocked.insert(node);
            CancelTokens(node);
            for (auto const& item : Issued) if (item.second == node)
            { Write(Root / "revoked" / item.first, "revoked\n"); Revoked.insert(item.first); }
            RefreshCrl(); Audit("revoked", node, actor); return true;
        }
        catch (...) { NextUpdate = 0; error = "Revocation could not be fully published. Node may already be blocked; check PKI storage and retry."; return false; }
    }
    bool Regenerate(std::string const& node, std::string const& actor, std::string& error)
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        try
        {
            auto found = Profiles.find(node); Require(Active && node != "skyfire-hub-authority" && found != Profiles.end() && !found->second.Key.empty());
            auto previous = Issued;
            Owned<EVP_PKEY, EVP_PKEY_free> key(C::Key(found->second.Key), EVP_PKEY_free); Require(bool(key)); Issue(found->second, key.get());
            for (auto const& item : previous) if (item.second == node)
            { Write(Root / "revoked" / item.first, "superseded\n"); Revoked.insert(item.first); }
            fs::remove(Root / "blocked" / node); Blocked.erase(node); RefreshCrl(); Audit("regenerated", node, actor); return true;
        }
        catch (...) { NextUpdate = 0; error = "Cannot regenerate this certificate. Remote nodes require a new enrollment token; check PKI storage."; return false; }
    }
    void Update()
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        if (!Active || std::time(nullptr) < NextUpdate) return;
        try
        {
            PruneExpired(); RefreshCrl();
            for (auto const& entry : Profiles)
            {
                auto const& p = entry.second; if (p.Key.empty() || Blocked.count(p.Node)) continue;
                Owned<X509, X509_free> cert(C::Certificate(p.Certificate), X509_free);
                if (!cert || (C::Expires(cert.get()) < C::Expires(Ca.get()) && C::Expires(cert.get()) - std::time(nullptr) <= std::int64_t(RenewDays) * 86400))
                { Owned<EVP_PKEY, EVP_PKEY_free> key(C::Key(p.Key), EVP_PKEY_free); Require(bool(key)); Issue(p, key.get()); Audit("renewed", p.Node, "automatic"); }
            }
        }
        catch (...) { NextUpdate = std::time(nullptr) + 60; SF_LOG_ERROR("server.pki", "Certificate renewal or CRL publication failed; check PKI store."); }
    }
    std::string Status()
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        std::ostringstream out; out << "{\"enabled\":" << (Active ? "true" : "false") << ",\"nodes\":[";
        bool first = true;
        for (auto const& entry : Profiles)
        {
            if (!first) out << ','; first = false;
            std::int64_t expiry = 0; std::string serial;
            for (auto const& issued : Issued) if (issued.second == entry.first && !Revoked.count(issued.first))
            {
                Owned<X509, X509_free> cert(C::Certificate((Root / "issued" / (issued.first + ".pem")).string()), X509_free);
                if (cert && C::Expires(cert.get()) > expiry) { expiry = C::Expires(cert.get()); serial = issued.first; }
            }
            out << "{\"node\":\"" << entry.first << "\",\"role\":" << entry.second.Role << ",\"revoked\":" << (Blocked.count(entry.first) ? "true" : "false")
                << ",\"local\":" << (entry.second.Key.empty() ? "false" : "true") << ",\"expires\":" << expiry
                << ",\"serial\":\"" << serial << "\",\"names\":\"" << entry.second.Names << "\"}";
        }
        out << "]}"; return out.str();
    }
    bool Prepare(std::string const& config, unsigned role, std::string& error)
    {
        std::lock_guard<std::recursive_mutex> lock(Mutex);
        if (!Active) return true;
        try
        {
            auto text = Read(config); bool python = role == 3 || role == 4;
            auto setting = [&](std::string const& name)
            {
                std::regex pattern("(^|\\n)[ \\t]*" + std::regex_replace(name, std::regex("\\."), "\\.") + "[ \\t]*=[ \\t]*[\"']?([^\"'\\r\\n#;]+)");
                Require(std::distance(std::sregex_iterator(text.begin(), text.end(), pattern), std::sregex_iterator()) <= 1);
                std::smatch match; if (!std::regex_search(text, match, pattern)) return std::string();
                auto value = match[2].str(); while (!value.empty() && value.back() == ' ') value.pop_back(); return value;
            };
            auto node = setting(python ? "node_key" : "Cluster.NodeKey"); Require(SafeNode(node) && !Blocked.count(node));
            auto address = setting(python ? "advertise_address" : "Cluster.AdvertiseAddress");
            if (address.empty()) address = "127.0.0.1";
            auto names = address;
            if (address == "127.0.0.1" || address == "::1") names += ",localhost";
            San(names);
            bool changedNames = false;
            Profile p; auto existing = Profiles.find(node);
            if (existing != Profiles.end())
            { p = existing->second; Require(!p.Key.empty() && p.Role == role); changedNames = p.Names != names; p.Names = names; }
            else
            {
                Require(Profiles.size() < 128); p.Node = node; p.Role = role; p.Names = address;
                if (address == "127.0.0.1" || address == "::1") p.Names += ",localhost";
                auto directory = Root / "nodes" / node;
                p.Certificate = (directory / "certificate.pem").string(); p.Key = (directory / "private-key.pem").string();
                Owned<EVP_PKEY, EVP_PKEY_free> key(C::NewKey(), EVP_PKEY_free); Write(p.Key, C::Pem(key.get())); Issue(p, key.get()); Audit("provisioned", node, "managed-launch");
            }
            Owned<X509, X509_free> certificate(C::Certificate(p.Certificate), X509_free);
            if (changedNames || !certificate || C::Expires(certificate.get()) - std::time(nullptr) <= std::int64_t(RenewDays) * 86400)
            { Owned<EVP_PKEY, EVP_PKEY_free> key(C::Key(p.Key), EVP_PKEY_free); Require(bool(key)); Issue(p, key.get()); }
            auto put = [&](std::string const& name, fs::path const& path)
            {
                std::string value = path.generic_string(); Require(value.find_first_of("\"\r\n") == std::string::npos);
                std::regex pattern("(^|\\n)[ \\t]*" + std::regex_replace(name, std::regex("\\."), "\\.") + "[ \\t]*=[^\\r\\n]*");
                Require(std::distance(std::sregex_iterator(text.begin(), text.end(), pattern), std::sregex_iterator()) <= 1);
                std::smatch match;
                auto replacement = name + " = \"" + value + "\"";
                if (std::regex_search(text, match, pattern)) text.replace(std::size_t(match.position()), std::size_t(match.length()), match[1].str() + replacement);
                else text += '\n' + replacement + '\n';
            };
            put(python ? "certificate" : "Cluster.Certificate", p.Certificate); put(python ? "private_key" : "Cluster.PrivateKey", p.Key);
            auto crlPath = Root / "nodes" / node / "revocations.pem";
            Write(crlPath, Read(Root / "revocations.pem"));
            put(python ? "ca" : "Cluster.CA", CaPath); put(python ? "crl" : "Cluster.CRL", crlPath);
            auto backup = config + ".pre-pki"; if (!fs::exists(backup)) Write(backup, Read(config));
            Write(config, text); return true;
        }
        catch (...) { error = "Certificate provisioning failed; check node identity, issuer configuration and protected files. Revoked nodes require explicit regeneration."; return false; }
    }
}
