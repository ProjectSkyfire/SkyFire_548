/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _AUTHCRYPT_H
#define _AUTHCRYPT_H

#include "ARC4.h"
#include "AuthDefines.h"
#include <array>

class AuthCrypt
{
    public:
        AuthCrypt();

        void Init(SessionKey const& K);
        void DecryptRecv(uint8* data, size_t len);
        void EncryptSend(uint8* data, size_t len);

        bool IsInitialized() const { return _initialized; }

    private:
        SkyFire::Crypto::ARC4 _clientDecrypt;
        SkyFire::Crypto::ARC4 _serverEncrypt;
        bool _initialized;
};
#endif