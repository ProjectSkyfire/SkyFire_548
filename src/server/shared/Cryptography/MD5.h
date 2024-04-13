/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _AUTH_MD5_H
#define _AUTH_MD5_H

#include "Define.h"
#include <openssl/evp.h>
#include <string>

class MD5Hash
{
public:
    MD5Hash();
    ~MD5Hash();
    void UpdateData(const std::string& str);
    void UpdateData(const uint8* data, size_t len);
    void Finalize(const uint8* data, uint32 len);
private:
    EVP_MD_CTX* m_ctx;
};
#endif

