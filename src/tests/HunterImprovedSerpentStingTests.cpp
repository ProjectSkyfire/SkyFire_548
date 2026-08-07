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
    std::string const hunterScript =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/scripts/Spells/spell_hunter.cpp");
    std::string pendingSql =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/sql/pending_updates/world/2026_08_07_00_world_hunter_improved_serpent_sting.sql");

    for (std::string::iterator ch = pendingSql.begin(); ch != pendingSql.end(); ++ch)
        if (*ch == '`')
            *ch = ' ';

    bool passed = true;
    passed &= Expect(Contains(hunterScript, "SPELL_HUNTER_IMPROVED_SERPENT_STING             = 82834"),
        "Hunter script should track the MoP Improved Serpent Sting passive spell.");
    passed &= Expect(Contains(hunterScript, "SPELL_HUNTER_IMPROVED_SERPENT_STING_DAMAGE      = 83077"),
        "Hunter script should use the MoP Improved Serpent Sting direct-damage spell.");
    passed &= Expect(Contains(hunterScript, "SPELL_HUNTER_SERPENT_STING_AURA                 = 118253"),
        "Hunter script should track the actual Serpent Sting periodic aura spell.");
    passed &= Expect(Contains(hunterScript, "periodicTotal = aurEff->GetAmount() * int32(aurEff->GetTotalTicks())"),
        "Improved Serpent Sting should calculate 15 percent from the total periodic effect.");
    passed &= Expect(Contains(hunterScript, "SPELL_HUNTER_IMPROVED_SERPENT_STING_DAMAGE, &instantDamage"),
        "Improved Serpent Sting should cast the direct-damage spell with a custom amount.");
    passed &= Expect(Contains(hunterScript, "EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER"),
        "Improved Serpent Sting spellmod handler should bind to the MoP 50 percent effect.");
    passed &= Expect(Contains(pendingSql, "(82834, 'spell_hun_improved_serpent_sting')"),
        "Pending SQL should bind Improved Serpent Sting to spell 82834.");
    passed &= Expect(Contains(pendingSql, "(118253, 'spell_hun_serpent_sting')"),
        "Pending SQL should bind the actual Serpent Sting periodic aura script.");

    return passed ? 0 : 1;
}
