/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_ACCOUNT_OP_RESULT_H
#define SF_ACCOUNT_OP_RESULT_H

// Deliberately dependency-free: AccountMgr.h (included practically
// everywhere) only needs this enum and these constants, not the
// SRP6/OpenSSL-dependent account-identity operations in AccountIdentity.h.
// Keep it that way - anything added here should not need a new #include.

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
}

#endif
