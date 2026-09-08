/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef SF_AUTHCODES_H
#define SF_AUTHCODES_H

enum class AuthResult
{
    WOW_SUCCESS = 0x00,
    WOW_FAIL_BANNED = 0x03,
    WOW_FAIL_UNKNOWN_ACCOUNT = 0x04,
    WOW_FAIL_INCORRECT_PASSWORD = 0x05,
    WOW_FAIL_ALREADY_ONLINE = 0x06,
    WOW_FAIL_NO_TIME = 0x07,
    WOW_FAIL_DB_BUSY = 0x08,
    WOW_FAIL_VERSION_INVALID = 0x09,
    WOW_FAIL_VERSION_UPDATE = 0x0A,
    WOW_FAIL_INVALID_SERVER = 0x0B,
    WOW_FAIL_SUSPENDED = 0x0C,
    WOW_FAIL_FAIL_NOACCESS = 0x0D,
    WOW_SUCCESS_SURVEY = 0x0E,
    WOW_FAIL_PARENTCONTROL = 0x0F,
    WOW_FAIL_LOCKED_ENFORCED = 0x10,
    WOW_FAIL_TRIAL_ENDED = 0x11,
    WOW_FAIL_OVERMIND_CONVERTED = 0x12,
    WOW_FAIL_ANTI_INDULGENCE = 0x13,
    WOW_FAIL_EXPIRED = 0x14,
    WOW_FAIL_NO_GAME_ACCOUNT = 0x15,
    WOW_FAIL_BILLING_LOCK = 0x16,
    WOW_FAIL_IGR_WITHOUT_BNET = 0x17,
    WOW_FAIL_AA_LOCK = 0x18,
    WOW_FAIL_UNLOCKABLE_LOCK = 0x19,
    WOW_FAIL_MUST_USE_BNET = 0x20,
    WOW_FAIL_OTHER = 0xFF
};

enum LoginResult
{
    LOGIN_OK = 0x00,
    LOGIN_INVALID_CHALLENGE_MESSAGE = 0x01,
    LOGIN_SRP_ERROR = 0x02,
    LOGIN_INVALID_PROOF_MESSAGE = 0x03,
    LOGIN_BAD_SERVER_PROOF = 0x04,
    LOGIN_INVALID_RECODE_MESSAGE = 0x05,
    LOGIN_BAD_SERVER_RECODE_PROOF = 0x06,
    LOGIN_UNKNOWN_ACCOUNT = 0x07,
    LOGIN_UNKNOWN_ACCOUNT_PIN = 0x08,
    LOGIN_UNKNOWN_ACCOUNT_CALL = 0x09,
    LOGIN_INCORRECT_PASSWORD = 0x0A,
    LOGIN_FAILED = 0x0B,
    LOGIN_SERVER_DOWN = 0x0C,
    LOGIN_BANNED = 0x0D,
    LOGIN_BADVERSION = 0x0E,
    LOGIN_ALREADYONLINE = 0x0F,
    LOGIN_NOTIME = 0x10,
    LOGIN_DBBUSY = 0x11,
    LOGIN_SUSPENDED = 0x12,
    LOGIN_PARENTALCONTROL = 0x13,
    LOGIN_LOCKED_ENFORCED = 0x14,
    LOGIN_DISCONNECTED = 0x15,
    LOGIN_ACCOUNT_CONVERTED = 0x16,
    LOGIN_ANTI_INDULGENCE = 0x17,
    LOGIN_EXPIRED = 0x18,
    LOGIN_TRIAL_EXPIRED = 0x19,
    LOGIN_NO_GAME_ACCOUNT = 0x1A,
    LOGIN_AUTH_OUTAGE = 0x1B,
    LOGIN_GAME_ACCOUNT_LOCKED = 0x1C,
    LOGIN_NO_BATTLENET_MANAGER = 0x1D,
    LOGIN_NO_BATTLENET_APPLICATION = 0x1E,
    LOGIN_MALFORMED_ACCOUNT_NAME = 0x1F,
    LOGIN_USE_GRUNT = 0x20,
    LOGIN_TOO_FAST = 0x21,
    LOGIN_CHARGEBACK = 0x22,
    LOGIN_IGR_WITHOUT_BNET = 0x23,
    LOGIN_UNLOCKABLE_LOCK = 0x24,
    LOGIN_CONVERSION_REQUIRED = 0x25,
    LOGIN_UNABLE_TO_DOWNLOAD_MODULE = 0x26,
    LOGIN_NO_GAME_ACCOUNTS_IN_REGION = 0x27,
    LOGIN_ACCOUNT_LOCKED = 0x28,
    LOGIN_SSO_FAILED = 0x29,
};

// Wire result for AUTH_MIGRATE_ACCOUNT, the launcher-facing pre-login
// account migration command (authnet). Not part of the classic GRUNT
// protocol - this is only ever sent to/from the SkyFire Launcher.
enum class AuthMigrateResult : uint8
{
    MIGRATE_OK = 0x00,
    MIGRATE_NAME_NOT_EXIST = 0x01,
    MIGRATE_PASS_INCORRECT = 0x02,
    MIGRATE_PASS_TOO_LONG = 0x03,
    MIGRATE_EMAIL_TOO_LONG = 0x04,
    MIGRATE_EMAIL_INVALID = 0x05,
    MIGRATE_EMAIL_ALREADY_EXIST = 0x06,
    MIGRATE_ACCOUNT_BANNED = 0x07,
    MIGRATE_FAILED = 0xFF
};

// Wire result for AUTH_AUTHNET_LOGIN_GRANT, a launcher-only pre-login
// command that validates account credentials before the authnet game flow
// is allowed to continue.
enum class AuthnetLoginGrantResult : uint8
{
    GRANT_OK = 0x00,
    GRANT_NAME_NOT_EXIST = 0x01,
    GRANT_PASS_INCORRECT = 0x02,
    GRANT_PASS_TOO_LONG = 0x03,
    GRANT_IDENTITY_INVALID = 0x04,
    GRANT_ACCOUNT_BANNED = 0x05,
    GRANT_FAILED = 0xFF
};

enum ExpansionFlags
{
    POST_BC_EXP_FLAG = 0x2,
    PRE_BC_EXP_FLAG = 0x1,
    NO_VALID_EXP_FLAG = 0x0
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
