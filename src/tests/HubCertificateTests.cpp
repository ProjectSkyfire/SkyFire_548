/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "HubCertificates.h"
#include "Cluster/CertificateTools.h"
#include "Config.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <memory>

namespace fs = std::filesystem;
namespace H = Skyfire::HubCertificates;
namespace C = Skyfire::Certificates;
std::string Configuration;
std::string ConfigMgr::GetStringDefault(char const*, std::string const& fallback) { return fallback; }
bool ConfigMgr::GetBoolDefault(char const* key, bool fallback) { return std::string(key) == "Hub.PKI.Enable" || std::string(key) == "Hub.Cluster.Enable" || fallback; }
int ConfigMgr::GetIntDefault(char const*, int fallback) { return fallback; }
std::string const& ConfigMgr::GetFilename() { return Configuration; }
void Check(bool result) { if (!result) throw std::runtime_error("Certificate lifecycle assertion failed"); }
template<class F> void Rejected(F&& action) { bool failed = false; try { action(); } catch (...) { failed = true; } Check(failed); }

int main()
{
    auto root = fs::temp_directory_path() / ("skyfire-pki-" + C::Random());
    fs::create_directories(root); Configuration = (root / "hub.conf").string();
    int result = 0;
    try
    {
        std::string error; Check(H::Initialize(error));
        std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> key(C::NewKey(), EVP_PKEY_free);
        auto keyPath = (root / "client-key.pem").string(), certPath = (root / "client.pem").string();
        C::Write(keyPath, C::Pem(key.get())); auto csr = C::Request(keyPath, "world-1");
        Rejected([&] { H::Token("..", 2, "localhost", "test"); });
        Rejected([&] { H::Token("world-1", 6, "localhost", "test"); });
        auto token = H::Token("world-1", 2, "localhost,127.0.0.1", "test");
        auto wire = H::Enroll(token, csr);
        Rejected([&] { H::Enroll(token, csr); });
        Check(C::InstallCertificate(wire, certPath, keyPath, H::ListenerPath("CA"), "world-1"));
        std::unique_ptr<X509, decltype(&X509_free)> cert(C::Certificate(certPath), X509_free);
        Check(H::Allowed(cert.get()) && C::Role(cert.get()) == 2);
        auto pending = H::Token("world-1", 2, "localhost", "test");
        auto previousCrl = H::Revocations(); auto crlPath = (root / "client-crl.pem").string();
        Check(C::InstallRevocations(previousCrl, crlPath, H::ListenerPath("CA")));
        Check(H::Revoke("world-1", "test", error) && !H::Allowed(cert.get()));
        Rejected([&] { H::Enroll(pending, csr); });
        Check(C::InstallRevocations(H::Revocations(), crlPath, H::ListenerPath("CA")));
        Check(!C::InstallRevocations(previousCrl, crlPath, H::ListenerPath("CA")));
        auto replacement = H::Enroll(H::Token("world-1", 2, "localhost", "test"), csr);
        Check(!H::Allowed(cert.get()));
        Check(C::InstallCertificate(replacement, certPath, keyPath, H::ListenerPath("CA"), "world-1"));
        cert.reset(C::Certificate(certPath)); Check(H::Allowed(cert.get()));
        Rejected([&] { H::Renew(cert.get(), csr); }); // Not yet in its renewal window.
        auto managed = (root / "chat.conf").string();
        C::Write(managed, "Cluster.NodeKey = \"chat-1\"\nCluster.AdvertiseAddress = \"127.0.0.1\"\n");
        Check(H::Prepare(managed, 5, error));
        Check(C::Read(managed).find("Cluster.CRL") != std::string::npos);
        Check(H::Regenerate("chat-1", "test", error));
        H::Shutdown(); Check(H::Initialize(error));
        Check(H::Allowed(cert.get())); // Durable issuance / revocation state survives restart.
    }
    catch (std::exception const& error) { std::cerr << error.what() << '\n'; result = 1; }
    H::Shutdown(); fs::remove_all(root); return result;
}
