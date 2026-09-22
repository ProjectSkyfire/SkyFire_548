/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_HUB_CERTIFICATES_H
#define SKYFIRE_HUB_CERTIFICATES_H
#include <string>
#include <openssl/x509.h>
namespace Skyfire::HubCertificates
{
    bool Initialize(std::string& error);
    void Shutdown();
    bool Enabled();
    std::string ListenerPath(std::string const& kind);
    void Update();
    std::string Status(); // Public metadata only; never private keys or enrollment tokens.
    std::string Token(std::string const& node, unsigned role, std::string const& names, std::string const& actor);
    std::string Enroll(std::string const& token, std::string const& csr);
    std::string Renew(X509* current, std::string const& csr);
    std::string Revocations();
    bool Allowed(X509* certificate);
    bool Revoke(std::string const& node, std::string const& actor, std::string& error);
    bool Regenerate(std::string const& node, std::string const& actor, std::string& error);
    bool Prepare(std::string const& config, unsigned role, std::string& error);
}
#endif
