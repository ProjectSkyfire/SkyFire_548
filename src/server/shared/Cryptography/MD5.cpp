/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "MD5.h"
#include "Common.h"

MD5Hash::MD5Hash() : m_ctx(EVP_MD_CTX_new())
{
    EVP_DigestInit_ex(m_ctx, EVP_md5(), NULL);
}

MD5Hash::~MD5Hash()
{
    EVP_MD_CTX_free(m_ctx);
}

void MD5Hash::UpdateData(const std::string &str)
{
    EVP_DigestUpdate(m_ctx, (uint8 const*)str.c_str(), str.length());
}

void MD5Hash::UpdateData(const uint8* data, size_t len)
{
    EVP_DigestUpdate(m_ctx, data, len);
}

void MD5Hash::Finalize(const uint8* data, uint32 len)
{
    EVP_DigestFinal_ex(m_ctx, (uint8*)data, &len);
}
