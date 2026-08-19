/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_ACCOUNT_IDENTITY_H
#define SF_ACCOUNT_IDENTITY_H

#include "Define.h"
#include "SRP6.h"

#include <string>

#define MAX_ACCOUNT_STR 16
#define MAX_EMAIL_STR 64

namespace Skyfire::Auth
{
    enum class AccountOpResult
    {
        AOR_OK,
        AOR_NAME_TOO_LONG,
        AOR_PASS_TOO_LONG,
        AOR_EMAIL_TOO_LONG,
        AOR_EMAIL_INVALID,
        AOR_EMAIL_ALREADY_EXIST,
        AOR_NAME_ALREADY_EXIST,
        AOR_NAME_NOT_EXIST,
        AOR_DB_INTERNAL_ERROR
    };

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
