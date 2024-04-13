/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _AUTHCRYPT_H
#define _AUTHCRYPT_H

#include "Cryptography/ARC4.h"

class BigNumber;

class AuthCrypt
{
public:
    AuthCrypt();

    void Init(BigNumber* K);
    void DecryptRecv(uint8*, size_t);
    void EncryptSend(uint8*, size_t);

    bool IsInitialized() const { return _initialized; }

private:
    ARC4 _clientDecrypt;
    ARC4 _serverEncrypt;
    bool _initialized;
};
#endif
