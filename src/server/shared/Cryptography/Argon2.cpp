/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Argon2.h"
#include <argon2/argon2.h>

/*static*/ Optional<std::string> SkyFire::Crypto::Argon2::Hash(std::string const& password, BigNumber const& salt, uint32 nIterations, uint32 kibMemoryCost)
{
    char buf[ENCODED_HASH_LEN];
    std::vector<uint8> saltBytes = salt.ToByteVector();
    int status = argon2id_hash_encoded(
        nIterations,
        kibMemoryCost,
        PARALLELISM,
        password.c_str(), password.length(),
        saltBytes.data(), saltBytes.size(),
        HASH_LEN, buf, ENCODED_HASH_LEN
    );

    if (status == ARGON2_OK)
        return std::string(buf);

    return {};
}

/*static*/ bool SkyFire::Crypto::Argon2::Verify(std::string const& password, std::string const& hash)
{
    int status = argon2id_verify(hash.c_str(), password.c_str(), password.length());
    return (status == ARGON2_OK);
}
