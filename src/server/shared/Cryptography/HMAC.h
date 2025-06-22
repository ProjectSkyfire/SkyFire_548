/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _HMAC_H
#define _HMAC_H

#include "CryptoConstants.h"
#include "Define.h"
#include "Errors.h"
#include <array>
#include <string>
#include <string_view>
#include <openssl/hmac.h>
#include <openssl/evp.h>

class BigNumber;

namespace SkyFire::Impl
{
    struct HMACImpl
    {
        typedef EVP_MD const* (*HashCreator)();

        static EVP_MAC* MakeMAC() { return EVP_MAC_fetch(NULL, "HMAC", NULL); }
        static EVP_MAC_CTX* MakeCTX(EVP_MAC* mac) { return EVP_MAC_CTX_new(mac); }
        static void DestroyCTX(EVP_MAC_CTX* ctx) { EVP_MAC_CTX_free(ctx); }
        static void DestroyMAC(EVP_MAC* mac) { EVP_MAC_free(mac); }
    };

    template <HMACImpl::HashCreator HashCreator, size_t DigestLength>
    class GenericHMAC
    {
        public:
            static constexpr size_t DIGEST_LENGTH = DigestLength;
            using Digest = std::array<uint8, DIGEST_LENGTH>;

            template <typename Container>
            static Digest GetDigestOf(Container const& seed, uint8 const* data, size_t len)
            {
                GenericHMAC hash(seed);
                hash.UpdateData(data, len);
                hash.Finalize();
                return hash.GetDigest();
            }

            template <typename Container, typename... Ts>
            static auto GetDigestOf(Container const& seed, Ts&&... pack) -> std::enable_if_t<!(std::is_integral_v<std::decay_t<Ts>> || ...), Digest>
            {
                GenericHMAC hash(seed);
                (hash.UpdateData(std::forward<Ts>(pack)), ...);
                hash.Finalize();
                return hash.GetDigest();
            }

            GenericHMAC(uint8 const* seed, size_t len) : _mac(HMACImpl::MakeMAC()), _ctx(HMACImpl::MakeCTX(_mac))
            {
                int result = EVP_MAC_init(_ctx, seed, len, _params);
                ASSERT(result == 1);
            }
            template <typename Container>
            GenericHMAC(Container const& container) : GenericHMAC(std::data(container), std::size(container)) {}

            ~GenericHMAC()
            {
                if (!_ctx)
                    return;
                HMACImpl::DestroyCTX(_ctx);
                HMACImpl::DestroyMAC(_mac);
                _ctx = NULL;
                _mac = NULL;
            }

            void UpdateData(uint8 const* data, size_t len)
            {
                int result = EVP_MAC_update(_ctx, data, len);
                ASSERT(result == 1);
            }
            void UpdateData(std::string_view str) { UpdateData(reinterpret_cast<uint8 const*>(str.data()), str.size()); }
            void UpdateData(std::string const& str) { UpdateData(std::string_view(str)); } /* explicit overload to avoid using the container template */
            void UpdateData(char const* str) { UpdateData(std::string_view(str)); } /* explicit overload to avoid using the container template */
            template <typename Container>
            void UpdateData(Container const& c) { UpdateData(std::data(c), std::size(c)); }

            void Finalize()
            {
                size_t length = 0;
                int result = EVP_MAC_final(_ctx, _digest.data(), &length, sizeof(_digest));
                ASSERT(result == 1);
                ASSERT(length == DIGEST_LENGTH);
                HMACImpl::DestroyCTX(_ctx);
                HMACImpl::DestroyMAC(_mac);
                _ctx = NULL;
                _mac = NULL;
            }

            Digest const& GetDigest() const { return _digest; }
        private:
            EVP_MAC* _mac;
            EVP_MAC_CTX* _ctx;
            Digest _digest = { };
            OSSL_PARAM _params[2] = { OSSL_PARAM_construct_utf8_string("digest", const_cast<char*>("SHA1"), 0), OSSL_PARAM_construct_end() };
    };
}

namespace SkyFire::Crypto
{
    using HMAC_SHA1 = SkyFire::Impl::GenericHMAC<EVP_sha1, Constants::SHA1_DIGEST_LENGTH_BYTES>;
    using HMAC_SHA256 = SkyFire::Impl::GenericHMAC<EVP_sha256, Constants::SHA256_DIGEST_LENGTH_BYTES>;
}
#endif
