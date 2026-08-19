/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AccountIdentity.h"
#include "DatabaseEnv.h"
#include "LoginIdentity.h"
#include "Util.h"

#include <algorithm>
#include <vector>

namespace Skyfire::Auth
{
namespace
{
    constexpr uint8 LOGIN_IDENTITY_LEGACY_USERNAME = 0;
    constexpr uint8 LOGIN_IDENTITY_EMAIL = 1;

    bool NormalizeSrpIdentityString(std::string& utf8String, size_t maxLength)
    {
        std::vector<wchar_t> buffer(maxLength + 1);
        size_t convertedLength = maxLength;
        if (!Utf8toWStr(utf8String, buffer.data(), convertedLength))
            return false;

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif
        std::transform(buffer.data(), buffer.data() + convertedLength, buffer.data(), wcharToUpperOnlyLatin);
#ifdef _MSC_VER
#pragma warning(default: 4996)
#endif

        return WStrToUtf8(buffer.data(), convertedLength, utf8String);
    }

    void StoreLoginIdentityVerifier(
        uint32 accountId,
        uint8 identityType,
        std::string const& identity,
        std::string const& canonical,
        SkyFire::Crypto::SRP6::Salt const& salt,
        SkyFire::Crypto::SRP6::Verifier const& verifier)
    {
        if (!accountId || identity.empty() || canonical.empty())
            return;

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_REP_ACCOUNT_LOGIN_IDENTITY);
        stmt->setUInt32(0, accountId);
        stmt->setUInt8(1, identityType);
        stmt->setString(2, identity);
        stmt->setString(3, canonical);
        stmt->setBinary(4, salt);
        stmt->setBinary(5, verifier);
        LoginDatabase.DirectExecute(stmt);
    }

    void StoreLegacyLoginIdentityVerifier(
        uint32 accountId,
        std::string const& username,
        SkyFire::Crypto::SRP6::Salt const& salt,
        SkyFire::Crypto::SRP6::Verifier const& verifier)
    {
        Skyfire::Auth::LoginIdentity loginIdentity = Skyfire::Auth::NormalizeLoginIdentity(username);
        if (!loginIdentity.Valid || loginIdentity.Kind != Skyfire::Auth::LoginIdentityKind::LegacyUsername)
            return;

        StoreLoginIdentityVerifier(accountId, LOGIN_IDENTITY_LEGACY_USERNAME, loginIdentity.Original,
            loginIdentity.Canonical, salt, verifier);
    }

    void StoreEmailLoginIdentityVerifier(uint32 accountId, std::string email, std::string const& password)
    {
        if (!accountId || email.empty())
            return;

        if (!NormalizeSrpIdentityString(email, MAX_EMAIL_STR))
            return;

        Skyfire::Auth::LoginIdentity loginIdentity = Skyfire::Auth::NormalizeLoginIdentity(email);
        if (!loginIdentity.Valid || loginIdentity.Kind != Skyfire::Auth::LoginIdentityKind::Email)
            return;

        auto [salt, verifier] = SkyFire::Crypto::SRP6::MakeRegistrationData(loginIdentity.Original, password);
        StoreLoginIdentityVerifier(accountId, LOGIN_IDENTITY_EMAIL, loginIdentity.Original, loginIdentity.Canonical,
            salt, verifier);
    }

    void StoreCurrentEmailLoginIdentityVerifiers(uint32 accountId, std::string const& password)
    {
        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_GET_EMAILS_BY_ID);
        stmt->setUInt32(0, accountId);
        if (PreparedQueryResult result = LoginDatabase.Query(stmt))
        {
            StoreEmailLoginIdentityVerifier(accountId, (*result)[0].GetString(), password);
            StoreEmailLoginIdentityVerifier(accountId, (*result)[1].GetString(), password);
        }
    }
}

void DeleteLoginIdentities(uint32 accountId)
{
    if (!accountId)
        return;

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_ACCOUNT_LOGIN_IDENTITIES);
    stmt->setUInt32(0, accountId);
    LoginDatabase.DirectExecute(stmt);
}

void DeleteEmailLoginIdentities(uint32 accountId)
{
    if (!accountId)
        return;

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_DEL_ACCOUNT_EMAIL_LOGIN_IDENTITIES);
    stmt->setUInt32(0, accountId);
    LoginDatabase.DirectExecute(stmt);
}

bool IsEmailLoginAssignedToAnotherAccount(uint32 accountId, std::string const& canonicalEmail)
{
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_ID_BY_EMAIL_ADDRESS);
    stmt->setString(0, canonicalEmail);
    stmt->setString(1, canonicalEmail);
    stmt->setUInt32(2, accountId);
    if (LoginDatabase.Query(stmt))
        return true;

    stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_ID_BY_EMAIL_LOGIN_IDENTITY);
    stmt->setString(0, canonicalEmail);
    stmt->setUInt32(1, accountId);
    return bool(LoginDatabase.Query(stmt));
}

void RefreshAccountLoginIdentities(
    uint32 accountId,
    std::string const& username,
    std::string const& password,
    SkyFire::Crypto::SRP6::Salt const& usernameSalt,
    SkyFire::Crypto::SRP6::Verifier const& usernameVerifier)
{
    DeleteLoginIdentities(accountId);
    StoreLegacyLoginIdentityVerifier(accountId, username, usernameSalt, usernameVerifier);
    StoreCurrentEmailLoginIdentityVerifiers(accountId, password);
}

uint32 GetId(std::string const& username)
{
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_GET_ACCOUNT_ID_BY_USERNAME);
    stmt->setString(0, username);
    PreparedQueryResult result = LoginDatabase.Query(stmt);

    return (result) ? (*result)[0].GetUInt32() : 0;
}

bool GetName(uint32 accountId, std::string& name)
{
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_GET_USERNAME_BY_ID);
    stmt->setUInt32(0, accountId);
    PreparedQueryResult result = LoginDatabase.Query(stmt);

    if (result)
    {
        name = (*result)[0].GetString();
        return true;
    }

    return false;
}

bool CheckPassword(uint32 accountId, std::string password)
{
    std::string username;

    if (!GetName(accountId, username))
        return false;

    NormalizeSrpIdentityString(username, MAX_ACCOUNT_STR);
    NormalizeSrpIdentityString(password, MAX_ACCOUNT_STR);

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_CHECK_PASSWORD);
    stmt->setUInt32(0, accountId);
    if (PreparedQueryResult result = LoginDatabase.Query(stmt))
    {
        SkyFire::Crypto::SRP6::Salt salt = (*result)[0].GetBinary<SkyFire::Crypto::SRP6::SALT_LENGTH>();
        SkyFire::Crypto::SRP6::Verifier verifier = (*result)[1].GetBinary<SkyFire::Crypto::SRP6::VERIFIER_LENGTH>();
        if (SkyFire::Crypto::SRP6::CheckLogin(username, password, salt, verifier))
            return true;
    }

    return false;
}

AccountOpResult ChangePassword(uint32 accountId, std::string newPassword)
{
    std::string username;

    if (!GetName(accountId, username))
        return AccountOpResult::AOR_NAME_NOT_EXIST;                          // account doesn't exist

    if (utf8length(newPassword) > MAX_ACCOUNT_STR)
        return AccountOpResult::AOR_PASS_TOO_LONG;

    NormalizeSrpIdentityString(username, MAX_ACCOUNT_STR);
    NormalizeSrpIdentityString(newPassword, MAX_ACCOUNT_STR);

    auto [salt, verifier] = SkyFire::Crypto::SRP6::MakeRegistrationData(username, newPassword);

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_LOGON);
    stmt->setBinary(0, salt);
    stmt->setBinary(1, verifier);
    stmt->setUInt32(2, accountId);

    LoginDatabase.Execute(stmt);
    RefreshAccountLoginIdentities(accountId, username, newPassword, salt, verifier);

    return AccountOpResult::AOR_OK;
}

AccountOpResult ConvertToEmailLogin(uint32 accountId, std::string email, std::string newPassword)
{
    std::string username;

    if (!GetName(accountId, username))
        return AccountOpResult::AOR_NAME_NOT_EXIST;

    if (utf8length(email) > MAX_EMAIL_STR)
        return AccountOpResult::AOR_EMAIL_TOO_LONG;

    if (utf8length(newPassword) > MAX_ACCOUNT_STR)
        return AccountOpResult::AOR_PASS_TOO_LONG;

    Skyfire::Auth::LoginIdentity loginIdentity = Skyfire::Auth::NormalizeLoginIdentity(email);
    if (!loginIdentity.Valid || loginIdentity.Kind != Skyfire::Auth::LoginIdentityKind::Email)
        return AccountOpResult::AOR_EMAIL_INVALID;

    if (IsEmailLoginAssignedToAnotherAccount(accountId, loginIdentity.Canonical))
        return AccountOpResult::AOR_EMAIL_ALREADY_EXIST;

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_ACCOUNT_EMAIL_LOGIN_CONVERSION);
    stmt->setString(0, loginIdentity.Original);
    stmt->setString(1, loginIdentity.Original);
    stmt->setUInt32(2, accountId);
    LoginDatabase.DirectExecute(stmt);

    return ChangePassword(accountId, newPassword);
}
}
