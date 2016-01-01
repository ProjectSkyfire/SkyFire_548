/*
 * Copyright (C) 2011-2016 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2016 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SF_AUTHCODES_H
#define SF_AUTHCODES_H

enum AuthResult
{
    WOW_SUCCESS                                  = 0x00,
    WOW_FAIL_BANNED                              = 0x03,
    WOW_FAIL_UNKNOWN_ACCOUNT                     = 0x04,
    WOW_FAIL_INCORRECT_PASSWORD                  = 0x05,
    WOW_FAIL_ALREADY_ONLINE                      = 0x06,
    WOW_FAIL_NO_TIME                             = 0x07,
    WOW_FAIL_DB_BUSY                             = 0x08,
    WOW_FAIL_VERSION_INVALID                     = 0x09,
    WOW_FAIL_VERSION_UPDATE                      = 0x0A,
    WOW_FAIL_INVALID_SERVER                      = 0x0B,
    WOW_FAIL_SUSPENDED                           = 0x0C,
    WOW_FAIL_FAIL_NOACCESS                       = 0x0D,
    WOW_SUCCESS_SURVEY                           = 0x0E,
    WOW_FAIL_PARENTCONTROL                       = 0x0F,
    WOW_FAIL_LOCKED_ENFORCED                     = 0x10,
    WOW_FAIL_TRIAL_ENDED                         = 0x11,
    WOW_FAIL_OVERMIND_CONVERTED                  = 0x12,
    WOW_FAIL_ANTI_INDULGENCE                     = 0x13,
    WOW_FAIL_EXPIRED                             = 0x14,
    WOW_FAIL_NO_GAME_ACCOUNT                     = 0x15,
    WOW_FAIL_BILLING_LOCK                        = 0x16,
    WOW_FAIL_IGR_WITHOUT_BNET                    = 0x17,
    WOW_FAIL_AA_LOCK                             = 0x18,
    WOW_FAIL_UNLOCKABLE_LOCK                     = 0x19,
    WOW_FAIL_MUST_USE_BNET                       = 0x20,
    WOW_FAIL_OTHER                               = 0xFF
};

enum LoginResult
{
    LOGIN_OK                                     = 0x00,
    LOGIN_INVALID_CHALLENGE_MESSAGE              = 0x01,
    LOGIN_SRP_ERROR                              = 0x02,
    LOGIN_INVALID_PROOF_MESSAGE                  = 0x03,
    LOGIN_BAD_SERVER_PROOF                       = 0x04,
    LOGIN_INVALID_RECODE_MESSAGE                 = 0x05,
    LOGIN_BAD_SERVER_RECODE_PROOF                = 0x06,
    LOGIN_UNKNOWN_ACCOUNT                        = 0x07,
    LOGIN_UNKNOWN_ACCOUNT_PIN                    = 0x08,
    LOGIN_UNKNOWN_ACCOUNT_CALL                   = 0x09,
    LOGIN_INCORRECT_PASSWORD                     = 0x0A,
    LOGIN_FAILED                                 = 0x0B,
    LOGIN_SERVER_DOWN                            = 0x0C,
    LOGIN_BANNED                                 = 0x0D,
    LOGIN_BADVERSION                             = 0x0E,
    LOGIN_ALREADYONLINE                          = 0x0F,
    LOGIN_NOTIME                                 = 0x10,
    LOGIN_DBBUSY                                 = 0x11,
    LOGIN_SUSPENDED                              = 0x12,
    LOGIN_PARENTALCONTROL                        = 0x13,
    LOGIN_LOCKED_ENFORCED                        = 0x14,
    LOGIN_ACCOUNT_CONVERTED                      = 0x15,
    LOGIN_ANTI_INDULGENCE                        = 0x16, 
    LOGIN_EXPIRED                                = 0x17,
    LOGIN_TRIAL_EXPIRED                          = 0x18,
    LOGIN_NO_GAME_ACCOUNT                        = 0x19,
    LOGIN_AUTH_OUTAGE                            = 0x1A,
    LOGIN_GAME_ACCOUNT_LOCKED                    = 0x1B,
    LOGIN_NO_BATTLENET_MANAGER                   = 0x1C,
    LOGIN_NO_BATTLENET_APPLICATION               = 0x1D,
    LOGIN_MALFORMED_ACCOUNT_NAME                 = 0x1E,
    LOGIN_USE_GRUNT                              = 0x1F,
    LOGIN_TOO_FAST                               = 0x20,
    LOGIN_CHARGEBACK                             = 0x21,
    LOGIN_IGR_WITHOUT_BNET                       = 0x22,
    LOGIN_UNLOCKABLE_LOCK                        = 0x23,
    LOGIN_UNABLE_TO_DOWNLOAD_MODULE              = 0x24,
    LOGIN_NO_GAME_ACCOUNTS_IN_REGION             = 0x25,
    LOGIN_ACCOUNT_LOCKED                         = 0x26,
    LOGIN_SSO_FAILED                             = 0x27,
};

enum ExpansionFlags
{
    POST_BC_EXP_FLAG                            = 0x2,
    PRE_BC_EXP_FLAG                             = 0x1,
    NO_VALID_EXP_FLAG                           = 0x0
};

struct RealmBuildInfo
{
    int Build;
    int MajorVersion;
    int MinorVersion;
    int BugfixVersion;
    int HotfixVersion;
};

namespace AuthHelper
{
    RealmBuildInfo const* GetBuildInfo(int build);
    bool IsAcceptedClientBuild(int build);
    bool IsPostBCAcceptedClientBuild(int build);
    bool IsPreBCAcceptedClientBuild(int build);
};

#endif
