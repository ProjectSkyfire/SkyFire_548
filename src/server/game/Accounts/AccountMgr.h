/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_ACCMGR_H
#define SF_ACCMGR_H

#include "Auth/AccountOpResult.h"
#include "Common.h"
#include "Platform/Singleton.h"
#include "RBAC.h"

// AccountOpResult, MAX_ACCOUNT_STR, and MAX_EMAIL_STR live in the shared,
// dependency-free Auth/AccountOpResult module - deliberately not the full
// Auth/AccountIdentity.h (which pulls in SRP6/OpenSSL), since AccountMgr.h
// is included practically everywhere and doesn't need those declarations
// itself; only AccountMgr.cpp's implementation does.
using AccountOpResult = Skyfire::Auth::AccountOpResult;

enum PasswordChangeSecurity
{
    PW_NONE,
    PW_EMAIL,
    PW_RBAC
};

struct AccountTwoFactorInfo
{
    bool Exists = false;
    bool Enabled = false;
    std::string Secret;
    uint64 LastUsedStep = 0;
};

namespace rbac
{
    typedef std::map<uint32, rbac::RBACPermission*> RBACPermissionsContainer;
    typedef std::map<uint8, rbac::RBACPermissionContainer> RBACDefaultPermissionsContainer;
}

class AccountMgr
{
    friend class Skyfire::Singleton<AccountMgr, Skyfire::NullMutex>;

private:
    AccountMgr();
    ~AccountMgr();

public:
    AccountOpResult CreateAccount(std::string username, std::string password, std::string email);
    static AccountOpResult DeleteAccount(uint32 accountId);
    static AccountOpResult ChangeUsername(uint32 accountId, std::string newUsername, std::string newPassword);
    static AccountOpResult ChangePassword(uint32 accountId, std::string newPassword);
    static AccountOpResult ChangeEmail(uint32 accountId, std::string newEmail);
    static AccountOpResult ChangeRegEmail(uint32 accountId, std::string newEmail);
    static AccountOpResult ConvertToEmailLogin(uint32 accountId, std::string email, std::string newPassword);
    static bool CheckPassword(uint32 accountId, std::string password);
    static bool CheckEmail(uint32 accountId, std::string newEmail);
    static bool GetTwoFactorInfo(uint32 accountId, AccountTwoFactorInfo& info);
    static std::string StartTwoFactorSetup(uint32 accountId);
    static bool ConfirmTwoFactorSetup(uint32 accountId, std::string const& token, uint32 window = 1);
    static bool DisableTwoFactor(uint32 accountId);

    static uint32 GetId(std::string const& username);
    static AccountTypes GetSecurity(uint32 accountId);
    static AccountTypes GetSecurity(uint32 accountId, int32 realmId);
    static bool GetName(uint32 accountId, std::string& name);
    static bool GetEmail(uint32 accountId, std::string& email);
    static uint32 GetCharactersCount(uint32 accountId);

    static bool normalizeString(std::string& utf8String);
    static bool IsPlayerAccount(AccountTypes gmlevel);
    static bool IsAdminAccount(AccountTypes gmlevel);
    static bool IsConsoleAccount(AccountTypes gmlevel);
    static bool HasPermission(uint32 accountId, uint32 permission, uint32 realmId);

    void UpdateAccountAccess(rbac::RBACData* rbac, uint32 accountId, uint8 securityLevel, int32 realmId);

    void LoadRBAC();
    rbac::RBACPermission const* GetRBACPermission(uint32 permission) const;

    rbac::RBACPermissionsContainer const& GetRBACPermissionList() const { return _permissions; }
    rbac::RBACPermissionContainer const& GetRBACDefaultPermissions(uint8 secLevel);

private:
    void ClearRBAC();
    rbac::RBACPermissionsContainer _permissions;
    rbac::RBACDefaultPermissionsContainer _defaultPermissions;
};

#define sAccountMgr Skyfire::Singleton<AccountMgr, Skyfire::NullMutex>::instance()
#endif
