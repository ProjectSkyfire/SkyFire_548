/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_ACCOUNT_IDENTITY_H
#define SF_ACCOUNT_IDENTITY_H

#include "AccountOpResult.h"
#include "Authentication/SRP6.h"
#include "Define.h"

#include <string>

namespace Skyfire::Auth
{
    uint32 GetId(std::string const& username);
    bool GetName(uint32 accountId, std::string& name);
    bool CheckPassword(uint32 accountId, std::string password);
    AccountOpResult ChangePassword(uint32 accountId, std::string newPassword);
    AccountOpResult ConvertToEmailLogin(uint32 accountId, std::string email, std::string newPassword);

    void DeleteLoginIdentities(uint32 accountId);
    void DeleteEmailLoginIdentities(uint32 accountId);
    void RefreshAccountLoginIdentities(
        uint32 accountId,
        std::string const& username,
        std::string const& password,
        SkyFire::Crypto::SRP6::Salt const& usernameSalt,
        SkyFire::Crypto::SRP6::Verifier const& usernameVerifier);
    bool IsEmailLoginAssignedToAnotherAccount(uint32 accountId, std::string const& canonicalEmail);
}

#endif
