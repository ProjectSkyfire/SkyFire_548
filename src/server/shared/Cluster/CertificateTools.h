/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_CERTIFICATE_TOOLS_H
#define SKYFIRE_CERTIFICATE_TOOLS_H
#include <openssl/ssl.h>
#include <string>
#include <cstdint>
namespace Skyfire::Certificates
{
    std::string Read(std::string const& path, std::size_t maximum = 1024 * 1024);
    void Write(std::string const& path, std::string const& value);
    std::string Random();
    std::string Digest(std::string const& value);
    std::string Base64(unsigned char const* data, std::size_t size);
    std::string Unbase64(std::string const& value);
    std::string Pem(X509* certificate);
    std::string Pem(EVP_PKEY* key);
    std::string Pem(X509_CRL* crl);
    std::string Wire(X509* certificate);
    std::string Wire(X509_CRL* crl);
    std::string Serial(X509* certificate);
    std::string Identity(X509* certificate);
    int Role(X509* certificate);
    std::int64_t Expires(X509* certificate);
    EVP_PKEY* NewKey();
    X509* Certificate(std::string const& path);
    EVP_PKEY* Key(std::string const& path);
    std::string Request(std::string const& keyPath, std::string const& identity);
    bool InstallCertificate(std::string const& encoded, std::string const& path,
        std::string const& keyPath, std::string const& caPath, std::string const& identity);
    bool InstallRevocations(std::string const& encoded, std::string const& path, std::string const& caPath);
    bool PeerAllowed(SSL* stream, std::string const& caPath, std::string const& crlPath);
    bool PeerAllowed(SSL* stream); // Native daemon Cluster.CA / Cluster.CRL configuration.
}
#endif
