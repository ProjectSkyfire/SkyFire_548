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
    std::string const dkSource = ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/scripts/Spells/spell_dk.cpp");
    std::string const masterySource = ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/scripts/Spells/spell_mastery.cpp");
    std::string const deathStrike = Section(dkSource, "// 49998 - Death Strike", "// 89832 - Death Strike");
    std::string const scentOfBlood = Section(dkSource, "// 49509 - Scent of Blood", "// 55090 - Scourge Strike");
    std::string const bloodShield = Section(masterySource, "// Called by 45470 - Death Strike (Heal)", "// Hand of Light");

    bool passed = true;
    passed &= Expect(!deathStrike.empty(), "Death Strike script section should be present.");
    passed &= Expect(!scentOfBlood.empty(), "Scent of Blood script section should be present.");
    passed &= Expect(!bloodShield.empty(), "Blood Shield script section should be present.");

    passed &= Expect(Contains(dkSource, "SPELL_DK_SCENT_OF_BLOOD_PASSIVE = 49509"),
        "Scent of Blood should track the passive proc aura.");
    passed &= Expect(Contains(dkSource, "SCENT_OF_BLOOD_DEATH_STRIKE_BONUS_PCT = 20"),
        "Scent of Blood should use the MoP 20 percent Death Strike bonus per stack.");

    passed &= Expect(Contains(scentOfBlood, "PROC_FLAG_DONE_MELEE_AUTO_ATTACK"),
        "Scent of Blood should only check melee autoattack procs.");
    passed &= Expect(Contains(scentOfBlood, "PROC_FLAG_DONE_MAINHAND_ATTACK"),
        "Scent of Blood should require main-hand autoattacks.");
    passed &= Expect(Contains(scentOfBlood, "PROC_FLAG_DONE_OFFHAND_ATTACK"),
        "Scent of Blood should reject off-hand autoattacks.");
    passed &= Expect(Contains(scentOfBlood, "PROC_EX_NORMAL_HIT | PROC_EX_CRITICAL_HIT"),
        "Scent of Blood should require successful autoattacks.");
    passed &= Expect(!Contains(scentOfBlood, "PreventDefaultAction"),
        "Scent of Blood should let the DBC proc cast the stack buff and energize Runic Power.");
    passed &= Expect(!Contains(scentOfBlood, "RemoveAuraFromStack(GetId())"),
        "Scent of Blood should not use the pre-MoP proc consume pattern.");

    passed &= Expect(Contains(deathStrike, "scentOfBlood->GetStackAmount()"),
        "Death Strike should read Scent of Blood stacks.");
    passed &= Expect(Contains(deathStrike, "AddPct(heal, scentOfBloodStacks * SCENT_OF_BLOOD_DEATH_STRIKE_BONUS_PCT)"),
        "Death Strike should increase its heal per Scent of Blood stack.");
    passed &= Expect(Contains(deathStrike, "AddPct(minimumHeal, scentOfBloodStacks * SCENT_OF_BLOOD_DEATH_STRIKE_BONUS_PCT)"),
        "Death Strike should increase its minimum heal per Scent of Blood stack.");
    passed &= Expect(Contains(deathStrike, "GetCaster()->RemoveAura(SPELL_DK_SCENT_OF_BLOOD)"),
        "Death Strike should consume Scent of Blood after using the next Death Strike.");
    passed &= Expect(!Contains(bloodShield, "SPELL_DK_SCENT_OF_BLOOD"),
        "Blood Shield should not apply Scent of Blood a second time after Death Strike heal is boosted.");

    return passed ? 0 : 1;
}
