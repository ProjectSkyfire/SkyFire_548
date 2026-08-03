/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

    bool AnySqlFileContains(std::vector<std::string> const& paths, char const* spellId, char const* scriptName)
    {
        std::string const compactNeedle = std::string("(") + spellId + ",'" + scriptName + "')";
        std::string const spacedNeedle = std::string("(") + spellId + ", '" + scriptName + "')";

        for (std::vector<std::string>::const_iterator itr = paths.begin(); itr != paths.end(); ++itr)
        {
            std::ifstream input(itr->c_str());
            if (!input)
                continue;

            std::ostringstream buffer;
            buffer << input.rdbuf();
            std::string sql = buffer.str();
            for (std::string::iterator ch = sql.begin(); ch != sql.end(); ++ch)
                if (*ch == '`')
                    *ch = ' ';

            if (Contains(sql, compactNeedle.c_str()) || Contains(sql, spacedNeedle.c_str()))
                return true;
        }

        return false;
    }

    bool TestDevotionAuraDoesNotLoadObsoleteAuraMasteryScripts()
    {
        std::string const source = ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/scripts/Spells/spell_paladin.cpp");

        bool passed = true;
        passed &= Expect(!Contains(source, "new spell_pal_aura_mastery();"),
            "Devotion Aura should not load the obsolete Aura Mastery script");
        passed &= Expect(!Contains(source, "new spell_pal_aura_mastery_immune();"),
            "Devotion Aura immunity should not load the obsolete Concentration Aura filter");
        return passed;
    }

    bool TestDevotionAuraSqlDoesNotBindObsoleteAuraMasteryScripts()
    {
        std::vector<std::string> const paths = {
            std::string(SKYFIRE_SOURCE_DIR) + "/sql/pending_updates/world/2026_08_03_00_world_aura_mastery.sql",
            std::string(SKYFIRE_SOURCE_DIR) + "/sql/updates/world/2026_08_03_world_14.sql"
        };

        bool passed = true;
        passed &= Expect(!AnySqlFileContains(paths, "31821", "spell_pal_aura_mastery"),
            "World SQL should not bind Devotion Aura to the obsolete Aura Mastery script");
        passed &= Expect(!AnySqlFileContains(paths, "64364", "spell_pal_aura_mastery_immune"),
            "World SQL should not bind Devotion Aura immunity to the obsolete Concentration Aura filter");
        return passed;
    }
}

int main()
{
    bool passed = true;

    passed &= TestDevotionAuraDoesNotLoadObsoleteAuraMasteryScripts();
    passed &= TestDevotionAuraSqlDoesNotBindObsoleteAuraMasteryScripts();

    return passed ? 0 : 1;
}
