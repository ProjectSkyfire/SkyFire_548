/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace
{
    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << message << '\n';

        return condition;
    }

    std::string ReadFile(std::string const& path)
    {
        std::ifstream input(path.c_str());
        if (!input)
        {
            std::cerr << "Could not open file: " << path << '\n';
            return "";
        }

        std::ostringstream buffer;
        buffer << input.rdbuf();
        return buffer.str();
    }

    bool Contains(std::string const& haystack, char const* needle)
    {
        return haystack.find(needle) != std::string::npos;
    }
}

int main()
{
    std::string const accountMgrHeader =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Accounts/AccountMgr.h");
    std::string const accountMgr =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Accounts/AccountMgr.cpp");
    std::string const accountCommands =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/scripts/Commands/cs_account.cpp");
    std::string const worldSession =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Server/WorldSession.h");
    std::string const characterHandler =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Handlers/CharacterHandler.cpp");
    std::string const pendingSql =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/sql/pending_updates/auth/2026_08_04_02_auth_email_login_conversion.sql");

    bool passed = true;
    passed &= Expect(Contains(accountMgrHeader, "ConvertToEmailLogin"),
        "Account manager should expose a self-service email conversion API.");
    passed &= Expect(Contains(accountMgr, "LOGIN_UPD_ACCOUNT_EMAIL_LOGIN_CONVERSION"),
        "Email conversion should persist email, registration mail, and conversion flag together.");
    passed &= Expect(Contains(accountMgr, "LOGIN_SEL_ACCOUNT_ID_BY_EMAIL_ADDRESS"),
        "Email conversion should reject email addresses already assigned to another account.");
    passed &= Expect(Contains(accountMgr, "LOGIN_SEL_ACCOUNT_ID_BY_EMAIL_LOGIN_IDENTITY"),
        "Email conversion should reject email login identities already assigned to another account.");
    passed &= Expect(Contains(accountCommands, ".account convert email"),
        "Account commands should advertise the player email conversion syntax.");
    passed &= Expect(Contains(worldSession, "UsedEmailLogin"),
        "World sessions should remember whether authentication used an email login.");
    passed &= Expect(Contains(characterHandler, "Convert your login with .account convert email"),
        "Legacy username login should prompt the player to convert.");
    passed &= Expect(!Contains(pendingSql, "ADD COLUMN IF NOT EXISTS"),
        "Pending auth SQL should avoid unsupported ALTER TABLE ADD COLUMN IF NOT EXISTS syntax.");
    passed &= Expect(Contains(pendingSql, "INFORMATION_SCHEMA"),
        "Pending auth SQL should guard the conversion flag column with an older-MySQL-compatible existence check.");

    return passed ? 0 : 1;
}
