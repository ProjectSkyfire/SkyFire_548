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
    std::string const player =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Entities/Player/Player.cpp");
    std::string const playerHeader =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Entities/Player/Player.h");
    std::string const spellEffects =
        ReadFile(std::string(SKYFIRE_SOURCE_DIR) + "/src/server/game/Spells/SpellEffectsProgression.cpp");

    bool passed = true;
    passed &= Expect(Contains(playerHeader, "bool HasTalentSpellCooldown(SpellInfo const* talentSpellInfo) const;"),
        "Player should expose a talent cooldown guard for unlearn paths.");
    passed &= Expect(Contains(player, "bool Player::HasTalentSpellCooldown(SpellInfo const* talentSpellInfo) const"),
        "Player should implement the talent cooldown guard.");
    passed &= Expect(Contains(player, "HasSpellCooldown(talentSpellInfo->Id)"),
        "Talent removal should reject cooldowns on the talent spell itself.");
    passed &= Expect(Contains(player, "talentSpellInfo->Effects[i].Effect == SPELL_EFFECT_LEARN_SPELL"),
        "Talent removal should inspect spells taught by the talent.");
    passed &= Expect(Contains(player, "HasSpellCooldown(talentSpellInfo->Effects[i].TriggerSpell)"),
        "Talent removal should reject cooldowns on spells taught by the talent.");
    passed &= Expect(Contains(player, "if (HasTalentSpellCooldown(unlearnSpellProto))"),
        "RemoveTalent should use the cooldown guard before removing spells.");
    passed &= Expect(Contains(spellEffects, "if (!player->RemoveTalent(m_glyphIndex))"),
        "RemoveTalent failure should be handled by the remove-talent spell effect.");
    passed &= Expect(Contains(spellEffects, "player->SendTalentsInfoData();"),
        "Blocked talent removal should resend talent state to the client.");

    return passed ? 0 : 1;
}
