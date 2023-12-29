/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "SHA1.h"
#include "BigNumber.h"
#include <stdarg.h>

SHA1Hash::SHA1Hash() : m_ctx(EVP_MD_CTX_new())
{
    EVP_DigestInit_ex(m_ctx, EVP_sha1(), NULL);
}

SHA1Hash::~SHA1Hash()
{
    EVP_MD_CTX_free(m_ctx);
}

void SHA1Hash::UpdateData(const uint8 *dta, int len)
{
    EVP_DigestUpdate(m_ctx, dta, len);
}

void SHA1Hash::UpdateData(const std::string &str)
{
    UpdateData((uint8 const*)str.c_str(), str.length());
}

void SHA1Hash::UpdateBigNumbers(BigNumber* bn0, ...)
{
    va_list v;
    BigNumber* bn;

    va_start(v, bn0);
    bn = bn0;
    while (bn)
    {
        UpdateData(bn->AsByteArray(), bn->GetNumBytes());
        bn = va_arg(v, BigNumber*);
    }
    va_end(v);
}

void SHA1Hash::Initialize()
{
    EVP_DigestInit(m_ctx, EVP_sha1());
}

void SHA1Hash::Finalize(void)
{
    uint32 length = SHA_DIGEST_LENGTH;
    EVP_DigestFinal_ex(m_ctx, m_digest, &length);
}

