/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "CertificateTools.h"
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/x509v3.h>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#include <sddl.h>
#else
#include <fcntl.h>
#include <unistd.h>
#endif
namespace Skyfire::Certificates
{
    namespace
    {
        template<class T, auto F> using Owned = std::unique_ptr<T, decltype(F)>;
        void Require(bool ok) { if (!ok) throw std::runtime_error("Certificate operation failed"); }
        std::string Bytes(BIO* bio)
        { char* value = nullptr; auto size = BIO_get_mem_data(bio, &value); Require(size > 0); return {value, std::size_t(size)}; }
        bool ValidCrl(X509_CRL* crl, X509* ca)
        {
            Owned<EVP_PKEY, EVP_PKEY_free> key(X509_get_pubkey(ca), EVP_PKEY_free);
            return crl && key && X509_NAME_cmp(X509_CRL_get_issuer(crl), X509_get_subject_name(ca)) == 0 &&
                X509_CRL_verify(crl, key.get()) == 1 && X509_cmp_current_time(X509_CRL_get0_lastUpdate(crl)) < 0 &&
                X509_cmp_current_time(X509_CRL_get0_nextUpdate(crl)) > 0;
        }
    }
    std::string Read(std::string const& path, std::size_t maximum)
    {
        std::ifstream input(path, std::ios::binary);
        Require(bool(input)); std::string result;
        char chunk[4096];
        while (input.read(chunk, sizeof(chunk)) || input.gcount())
        { Require(result.size() + std::size_t(input.gcount()) <= maximum); result.append(chunk, std::size_t(input.gcount())); }
        Require(input.eof()); return result;
    }
    std::string Base64(unsigned char const* data, std::size_t size)
    {
        Require(size <= 45000); std::string value(4 * ((size + 2) / 3), '\0');
        int length = EVP_EncodeBlock(reinterpret_cast<unsigned char*>(value.data()), data, int(size));
        Require(length == int(value.size())); return value;
    }
    std::string Unbase64(std::string const& value)
    {
        Require(!value.empty() && value.size() <= 60000 && value.size() % 4 == 0);
        Require(value.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=") == std::string::npos);
        std::string result(value.size(), '\0');
        int size = EVP_DecodeBlock(reinterpret_cast<unsigned char*>(result.data()),
            reinterpret_cast<unsigned char const*>(value.data()), int(value.size()));
        Require(size >= 0); if (value.back() == '=') --size; if (value[value.size()-2] == '=') --size;
        Require(size >= 0);
        result.resize(std::size_t(size));
        Require(Base64(reinterpret_cast<unsigned char const*>(result.data()), result.size()) == value); return result;
    }
    std::string Random()
    {
        unsigned char bytes[32]; Require(RAND_bytes(bytes, 32) == 1);
        std::string out; for (auto c : bytes) { out += "0123456789abcdef"[c >> 4]; out += "0123456789abcdef"[c & 15]; } return out;
    }
    std::string Digest(std::string const& value)
    {
        unsigned char bytes[32]; unsigned length = 0;
        Require(EVP_Digest(value.data(), value.size(), bytes, &length, EVP_sha256(), nullptr) == 1 && length == 32);
        std::string out; for (auto c : bytes) { out += "0123456789abcdef"[c >> 4]; out += "0123456789abcdef"[c & 15]; } return out;
    }
    void Write(std::string const& path, std::string const& value)
    {
        auto target = std::filesystem::path(path);
        std::filesystem::create_directories(target.parent_path());
        auto temporary = target; temporary += "." + Random() + ".tmp";
#ifdef _WIN32
        PSECURITY_DESCRIPTOR descriptor = nullptr;
        Require(ConvertStringSecurityDescriptorToSecurityDescriptorW(L"D:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;FA;;;OW)",
            SDDL_REVISION_1, &descriptor, nullptr) != 0);
        SECURITY_ATTRIBUTES attributes{sizeof(SECURITY_ATTRIBUTES), descriptor, FALSE};
        HANDLE file = CreateFileW(temporary.c_str(), GENERIC_WRITE, 0, &attributes, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
        LocalFree(descriptor); Require(file != INVALID_HANDLE_VALUE);
        DWORD written = 0;
        bool ok = WriteFile(file, value.data(), DWORD(value.size()), &written, nullptr) && written == value.size() && FlushFileBuffers(file);
        CloseHandle(file);
        if (!ok || !MoveFileExW(temporary.c_str(), target.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
        { DeleteFileW(temporary.c_str()); Require(false); }
#else
        int file = open(temporary.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0600); Require(file >= 0);
        std::size_t written = 0;
        while (written < value.size()) { auto n = write(file, value.data()+written, value.size()-written); if (n <= 0) break; written += std::size_t(n); }
        bool ok = written == value.size() && fsync(file) == 0; close(file);
        if (!ok || rename(temporary.c_str(), target.c_str()) != 0) { unlink(temporary.c_str()); Require(false); }
#endif
    }
    X509* Certificate(std::string const& path)
    { if (!std::filesystem::exists(path)) return nullptr; auto pem = Read(path); Owned<BIO, BIO_free> bio(BIO_new_mem_buf(pem.data(), int(pem.size())), BIO_free); return PEM_read_bio_X509(bio.get(), nullptr, nullptr, nullptr); }
    EVP_PKEY* Key(std::string const& path)
    { auto pem = Read(path); Owned<BIO, BIO_free> bio(BIO_new_mem_buf(pem.data(), int(pem.size())), BIO_free); return PEM_read_bio_PrivateKey(bio.get(), nullptr, [](char*, int, int, void*) { return 0; }, nullptr); }
    EVP_PKEY* NewKey()
    {
        Owned<EVP_PKEY_CTX, EVP_PKEY_CTX_free> context(EVP_PKEY_CTX_new_id(EVP_PKEY_EC, nullptr), EVP_PKEY_CTX_free);
        Require(context && EVP_PKEY_keygen_init(context.get()) == 1 && EVP_PKEY_CTX_set_ec_paramgen_curve_nid(context.get(), NID_X9_62_prime256v1) == 1);
        EVP_PKEY* key = nullptr; Require(EVP_PKEY_keygen(context.get(), &key) == 1); return key;
    }
    std::string Pem(X509* cert)
    { Owned<BIO, BIO_free> bio(BIO_new(BIO_s_mem()), BIO_free); Require(PEM_write_bio_X509(bio.get(), cert) == 1); return Bytes(bio.get()); }
    std::string Pem(EVP_PKEY* key)
    { Owned<BIO, BIO_free> bio(BIO_new(BIO_s_mem()), BIO_free); Require(PEM_write_bio_PrivateKey(bio.get(), key, nullptr, nullptr, 0, nullptr, nullptr) == 1); return Bytes(bio.get()); }
    std::string Pem(X509_CRL* crl)
    { Owned<BIO, BIO_free> bio(BIO_new(BIO_s_mem()), BIO_free); Require(PEM_write_bio_X509_CRL(bio.get(), crl) == 1); return Bytes(bio.get()); }
    std::string Wire(X509* cert)
    { unsigned char* data = nullptr; int n = i2d_X509(cert, &data); Require(n > 0); auto out = Base64(data, std::size_t(n)); OPENSSL_free(data); return out; }
    std::string Wire(X509_CRL* crl)
    { unsigned char* data = nullptr; int n = i2d_X509_CRL(crl, &data); Require(n > 0); auto out = Base64(data, std::size_t(n)); OPENSSL_free(data); return out; }
    std::string Serial(X509* certificate)
    {
        Owned<BIGNUM, BN_free> bn(ASN1_INTEGER_to_BN(X509_get_serialNumber(certificate), nullptr), BN_free);
        Require(bool(bn)); char* text = BN_bn2hex(bn.get()); Require(text != nullptr); std::string result(text); OPENSSL_free(text); return result;
    }
    std::string Identity(X509* certificate)
    {
        auto* subject = X509_get_subject_name(certificate); int index = X509_NAME_get_index_by_NID(subject, NID_commonName, -1);
        if (index < 0 || X509_NAME_get_index_by_NID(subject, NID_commonName, index) >= 0) return {};
        auto* value = X509_NAME_ENTRY_get_data(X509_NAME_get_entry(subject, index));
        return {reinterpret_cast<char const*>(ASN1_STRING_get0_data(value)), std::size_t(ASN1_STRING_length(value))};
    }
    int Role(X509* certificate)
    {
        char value[32]{}; int n = X509_NAME_get_text_by_NID(X509_get_subject_name(certificate), NID_organizationalUnitName, value, sizeof(value));
        return n == 14 && std::string(value, 13) == "SkyFire-role-" && value[13] >= '1' && value[13] <= '5' ? value[13]-'0' : 0;
    }
    std::int64_t Expires(X509* certificate)
    {
        std::tm time{}; Require(ASN1_TIME_to_tm(X509_get0_notAfter(certificate), &time) == 1);
#ifdef _WIN32
        return _mkgmtime(&time);
#else
        return timegm(&time);
#endif
    }
    std::string Request(std::string const& keyPath, std::string const& identity)
    {
        Owned<EVP_PKEY, EVP_PKEY_free> key(Key(keyPath), EVP_PKEY_free);
        Owned<X509_REQ, X509_REQ_free> request(X509_REQ_new(), X509_REQ_free); Require(key && request);
        Require(X509_REQ_set_version(request.get(), 0) == 1 && X509_REQ_set_pubkey(request.get(), key.get()) == 1);
        Owned<X509_NAME, X509_NAME_free> subject(X509_NAME_new(), X509_NAME_free); Require(bool(subject));
        Require(X509_NAME_add_entry_by_NID(subject.get(), NID_commonName, MBSTRING_ASC,
            reinterpret_cast<unsigned char const*>(identity.c_str()), -1, -1, 0) == 1 &&
            X509_REQ_set_subject_name(request.get(), subject.get()) == 1 && X509_REQ_sign(request.get(), key.get(), EVP_sha256()) > 0);
        unsigned char* data = nullptr; int n = i2d_X509_REQ(request.get(), &data); Require(n > 0);
        auto out = Base64(data, std::size_t(n)); OPENSSL_free(data); return out;
    }
    bool InstallCertificate(std::string const& encoded, std::string const& path, std::string const& keyPath,
        std::string const& caPath, std::string const& identity)
    {
        try
        {
            auto der = Unbase64(encoded); auto* bytes = reinterpret_cast<unsigned char const*>(der.data());
            Owned<X509, X509_free> cert(d2i_X509(nullptr, &bytes, long(der.size())), X509_free), ca(Certificate(caPath), X509_free);
            Owned<EVP_PKEY, EVP_PKEY_free> key(Key(keyPath), EVP_PKEY_free), issuer(ca ? X509_get_pubkey(ca.get()) : nullptr, EVP_PKEY_free);
            if (!cert || !ca || !key || !issuer || bytes != reinterpret_cast<unsigned char const*>(der.data()+der.size()) ||
                Identity(cert.get()) != identity || X509_check_private_key(cert.get(), key.get()) != 1 ||
                X509_check_ca(cert.get()) != 0 || X509_NAME_cmp(X509_get_issuer_name(cert.get()), X509_get_subject_name(ca.get())) != 0 ||
                X509_verify(cert.get(), issuer.get()) != 1 || X509_cmp_current_time(X509_get0_notBefore(cert.get())) >= 0 ||
                X509_cmp_current_time(X509_get0_notAfter(cert.get())) <= 0) return false;
            Write(path, Pem(cert.get())); return true;
        }
        catch (...) { return false; }
    }
    bool InstallRevocations(std::string const& encoded, std::string const& path, std::string const& caPath)
    {
        try
        {
            auto der = Unbase64(encoded); auto* bytes = reinterpret_cast<unsigned char const*>(der.data());
            Owned<X509_CRL, X509_CRL_free> crl(d2i_X509_CRL(nullptr, &bytes, long(der.size())), X509_CRL_free);
            Owned<X509, X509_free> ca(Certificate(caPath), X509_free);
            if (!ca || !crl || bytes != reinterpret_cast<unsigned char const*>(der.data()+der.size()) || !ValidCrl(crl.get(), ca.get())) return false;
            if (std::filesystem::exists(path))
            {
                auto old = Read(path); Owned<BIO, BIO_free> bio(BIO_new_mem_buf(old.data(), int(old.size())), BIO_free);
                Owned<X509_CRL, X509_CRL_free> prior(PEM_read_bio_X509_CRL(bio.get(), nullptr, nullptr, nullptr), X509_CRL_free);
                if (!prior) return false;
                Owned<ASN1_INTEGER, ASN1_INTEGER_free> number(static_cast<ASN1_INTEGER*>(X509_CRL_get_ext_d2i(crl.get(), NID_crl_number, nullptr, nullptr)), ASN1_INTEGER_free),
                    previous(static_cast<ASN1_INTEGER*>(X509_CRL_get_ext_d2i(prior.get(), NID_crl_number, nullptr, nullptr)), ASN1_INTEGER_free);
                if (!number || !previous || ASN1_INTEGER_cmp(number.get(), previous.get()) < 0) return false;
                if (ASN1_INTEGER_cmp(number.get(), previous.get()) == 0) return Pem(crl.get()) == Pem(prior.get());
            }
            Write(path, Pem(crl.get())); return true;
        }
        catch (...) { return false; }
    }
    bool PeerAllowed(SSL* stream, std::string const& caPath, std::string const& crlPath)
    {
        if (crlPath.empty()) return true;
        try
        {
            struct Cache
            {
                std::string CAPath, CRLPath;
                std::filesystem::file_time_type CAStamp{}, CRLStamp{};
                Owned<X509, X509_free> CA{nullptr, X509_free};
                Owned<X509_CRL, X509_CRL_free> CRL{nullptr, X509_CRL_free};
            };
            // Character RPCs share long-lived TLS streams. Reverify signatures when
            // files change, while checking freshness and peer serial on every call.
            static thread_local Cache cache;
            auto caStamp = std::filesystem::last_write_time(caPath), crlStamp = std::filesystem::last_write_time(crlPath);
            if (!cache.CRL || cache.CAPath != caPath || cache.CRLPath != crlPath || cache.CAStamp != caStamp || cache.CRLStamp != crlStamp)
            {
                Owned<X509, X509_free> ca(Certificate(caPath), X509_free);
                auto pem = Read(crlPath); Owned<BIO, BIO_free> bio(BIO_new_mem_buf(pem.data(), int(pem.size())), BIO_free);
                Owned<X509_CRL, X509_CRL_free> crl(PEM_read_bio_X509_CRL(bio.get(), nullptr, nullptr, nullptr), X509_CRL_free);
                if (!ca || !ValidCrl(crl.get(), ca.get())) return false;
                cache.CA = std::move(ca); cache.CRL = std::move(crl);
                cache.CAPath = caPath; cache.CRLPath = crlPath; cache.CAStamp = caStamp; cache.CRLStamp = crlStamp;
            }
            Owned<X509, X509_free> peer(SSL_get1_peer_certificate(stream), X509_free);
            X509_REVOKED* revoked = nullptr;
            return peer && X509_cmp_current_time(X509_CRL_get0_lastUpdate(cache.CRL.get())) < 0 &&
                X509_cmp_current_time(X509_CRL_get0_nextUpdate(cache.CRL.get())) > 0 && X509_cmp_current_time(X509_get0_notAfter(peer.get())) > 0 &&
                X509_NAME_cmp(X509_get_issuer_name(peer.get()), X509_get_subject_name(cache.CA.get())) == 0 &&
                X509_CRL_get0_by_cert(cache.CRL.get(), &revoked, peer.get()) == 0;
        }
        catch (...) { return false; }
    }
}
