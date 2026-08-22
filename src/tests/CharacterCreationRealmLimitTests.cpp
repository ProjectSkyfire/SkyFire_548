/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifndef SKYFIRE_SOURCE_DIR
#define SKYFIRE_SOURCE_DIR "."
#endif

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

    std::string ExtractFunction(std::string const& source, char const* signature)
    {
        size_t const start = source.find(signature);
        if (start == std::string::npos)
            return "";

        size_t const openBrace = source.find('{', start);
        if (openBrace == std::string::npos)
            return "";

        uint32_t depth = 0;
        for (size_t i = openBrace; i < source.size(); ++i)
        {
            if (source[i] == '{')
                ++depth;
            else if (source[i] == '}')
            {
                if (--depth == 0)
                    return source.substr(start, i - start + 1);
            }
        }

        return "";
    }

    bool Contains(std::string const& haystack, char const* needle)
    {
        return haystack.find(needle) != std::string::npos;
    }
}

int main()
{
    std::string const characterDatabase =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/shared/Database/Implementation/CharacterDatabase.cpp");
    std::string const characterDatabaseHeader =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/shared/Database/Implementation/CharacterDatabase.h");
    std::string const characterHandler =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Handlers/CharacterHandler.cpp");
    std::string const world =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/World/World.cpp");
    std::string const loginDatabase =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/shared/Database/Implementation/LoginDatabase.cpp");
    std::string const loginDatabaseHeader =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/shared/Database/Implementation/LoginDatabase.h");

    std::string const handleCharCreateCallback =
        ExtractFunction(characterHandler, "void WorldSession::HandleCharCreateCallback(PreparedQueryResult result, CharacterCreateInfo* createInfo)");
    std::string const updateRealmCharCount =
        ExtractFunction(world, "void World::_UpdateRealmCharCount(uint32 accountId, PreparedQueryResult resultCharCount)");
    std::string const syncRealmCharacterCounts =
        ExtractFunction(world, "void World::SyncRealmCharacterCounts()");

    bool passed = true;
    passed &= Expect(Contains(characterDatabaseHeader, "CHAR_SEL_SUM_CHARS_BY_REALM"),
        "Character database statements should include a realm-filtered character count.");
    passed &= Expect(Contains(characterDatabase, "CHAR_SEL_SUM_CHARS_BY_REALM") &&
        Contains(characterDatabase, "SELECT COUNT(guid) FROM characters WHERE account = ? AND realm = ?"),
        "Character creation should have a prepared statement that counts only characters on the selected realm.");
    passed &= Expect(!handleCharCreateCallback.empty(),
        "HandleCharCreateCallback should exist.");
    passed &= Expect(Contains(handleCharCreateCallback, "CHAR_SEL_SUM_CHARS_BY_REALM") &&
        Contains(handleCharCreateCallback, "stmt->setUInt32(1, GetVirtualRealmID())"),
        "The per-realm character limit should count only characters on the selected realm.");
    passed &= Expect(Contains(characterDatabase, "CHAR_SEL_CHARACTER_COUNT") &&
        Contains(characterDatabase, "SELECT realm, COUNT(guid) FROM characters WHERE account = ? GROUP BY realm"),
        "Realm character cache refreshes should return separate counts per account realm.");
    passed &= Expect(!updateRealmCharCount.empty(),
        "World::_UpdateRealmCharCount should exist.");
    passed &= Expect(Contains(updateRealmCharCount, "resultCharCount->NextRow()") &&
        Contains(updateRealmCharCount, "std::map<uint32, uint8> realmCharCounts"),
        "Realm character cache refreshes should apply each realm's own character count.");
    passed &= Expect(Contains(world, "RealmCharCountCallback(accountId, result)") &&
        Contains(updateRealmCharCount, "stmt->setUInt32(0, accountId)"),
        "Realm character cache refreshes should keep the account id even when the character-count query returns no rows.");
    passed &= Expect(Contains(loginDatabaseHeader, "LOGIN_UPD_REALM_CHARACTERS") &&
        Contains(loginDatabase, "UPDATE realmcharacters SET numchars = ? WHERE acctid = ? AND realmid = ?"),
        "Login database should prepare realm character count updates.");
    passed &= Expect(!syncRealmCharacterCounts.empty(),
        "World startup should have a realm character count sync.");
    passed &= Expect(Contains(world, "SyncRealmCharacterCounts();"),
        "World startup should invoke the realm character count sync.");
    passed &= Expect(Contains(syncRealmCharacterCounts, "LOGIN_INS_REALM_CHARACTERS_INIT") &&
        Contains(syncRealmCharacterCounts, "realmNameStore.begin()") &&
        Contains(syncRealmCharacterCounts, "UPDATE realmcharacters SET numchars = 0 WHERE realmid = %u") &&
        Contains(syncRealmCharacterCounts, "deleteInfos_Name IS NULL") &&
        Contains(syncRealmCharacterCounts, "if (count)") &&
        Contains(syncRealmCharacterCounts, "LoginDatabase.DirectCommitTransaction(trans)"),
        "World startup should rebuild realmcharacters per local realm without wiping other realms.");
    passed &= Expect(Contains(loginDatabase, "SELECT r.id, a.id, 0 FROM realmlist r CROSS JOIN account a LEFT JOIN realmcharacters rc ON rc.realmid = r.id AND rc.acctid = a.id WHERE rc.acctid IS NULL"),
        "Realm character init should create missing rows per account and per realm, not just per account.");

    return passed ? 0 : 1;
}
