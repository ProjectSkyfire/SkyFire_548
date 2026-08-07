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

    std::string Section(std::string const& source, char const* begin, char const* end)
    {
        std::string::size_type const start = source.find(begin);
        if (start == std::string::npos)
            return "";

        std::string::size_type const finish = source.find(end, start);
        if (finish == std::string::npos)
            return source.substr(start);

        return source.substr(start, finish - start);
    }
}

int main()
{
    std::string const source = ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/scripts/Spells/spell_dk.cpp");
    std::string const antiMagicZone = Section(source, "// 50461 - Anti-Magic Zone", "// 50453 - Bloodworms Health Leech");

    bool passed = true;
    passed &= Expect(!antiMagicZone.empty(), "Anti-Magic Zone script section should be present.");
    passed &= Expect(!Contains(antiMagicZone, "GetTotalAttackPowerValue"),
        "Anti-Magic Zone should not scale from attack power in 5.4.8.");
    passed &= Expect(!Contains(antiMagicZone, "WeaponAttackType::BASE_ATTACK"),
        "Anti-Magic Zone should not read base attack power in 5.4.8.");
    passed &= Expect(Contains(antiMagicZone, "reductionSpell->Effects[EFFECT_0].CalcValue(GetCaster())"),
        "Anti-Magic Zone legacy fallback should read the 5.4.8 reduction amount.");
    passed &= Expect(Contains(antiMagicZone, "amount = -1"),
        "Anti-Magic Zone legacy fallback should not add an absorb cap.");
    passed &= Expect(Contains(source, "SPELL_DK_ANTI_MAGIC_ZONE_AREA = 51052"),
        "Anti-Magic Zone should track the 5.4.8 area trigger spell.");
    passed &= Expect(Contains(source, "SPELL_DK_ANTI_MAGIC_ZONE_REDUCTION = 145629"),
        "Anti-Magic Zone should apply the 5.4.8 damage reduction aura.");
    passed &= Expect(Contains(source, "dynObj->GetSpellId() != SPELL_DK_ANTI_MAGIC_ZONE_AREA"),
        "Anti-Magic Zone dynamic object handling should be scoped to the zone spell.");
    passed &= Expect(Contains(source, "caster->AddAura(SPELL_DK_ANTI_MAGIC_ZONE_REDUCTION, target)"),
        "Anti-Magic Zone should apply the reduction aura through the caster.");

    return passed ? 0 : 1;
}
